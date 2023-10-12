#include <chrono>
#include <assert.h>
#include <sstream>
#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmGuiButton.h"
#include "jmCommonFunctions.h"
#include "jmTextSprite.h"
#include "jmGuiText.h"
#include "items/jpItemsCommon.h"
#include "actions/jpCommands_Logic.h"
#include "jpGfxObjectsCommon.h"
#include "jpPlayedApp.h"
#include "jpPlayedScene.h"
#include "jpUser.h"
#include "jpItemsTable.h"
#include "jpUtilities.h"
#include "jpUserSelector.h"



namespace jugimap{



using namespace jugimap;



UserSelector::UserSelector(const pugi::xml_node &_node)
{

    mName = "userSelector";
    mCfg.reset(new Cfg());

    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){

        std::string nodeName = std::string(n.name());

        if(nodeName=="useWidgets"){
            mCfg->mUsersTable = n.attribute("usersTable").as_string("");
            mCfg->mAddNewUserButton = n.attribute("addNewUserButton").as_string("");
            mCfg->mBDeleteUserButton = n.attribute("deleteUserButton").as_string("");

        }else if(nodeName=="useComponent1"){
            mCfg->mNewUserPanelComponent =  n.attribute("newUserPanelComponent").as_string("");
            mCfg->mNewUserPanelComponentTransition = n.attribute("transition").as_string("");
            mCfg->mNewUserPanelComponentTransitionShow = n.attribute("show").as_string("");

        }else if(nodeName=="useComponent2"){
            mCfg->mConfirmDeleteUserComponent = n.attribute("confirmDeleteUserComponent").as_string("");
            mCfg->mConfirmDeleteUserComponentTransition = n.attribute("transition").as_string("");
            mCfg->mConfirmDeleteUserComponentTransitionShow = n.attribute("show").as_string("");

            for(pugi::xml_node nChild = n.first_child(); nChild; nChild = nChild.next_sibling()){

                std::string childNodeName = std::string(nChild.name());

                if(childNodeName=="data"){
                    mCfg->mConfirmDeleteTextSegment = nChild.attribute("textSegment").as_string("");
                }
            }

        }
    }

}




bool UserSelector::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("init component 'userSelector'");

    mParentPlayerScene = _scene;

    //---
    mUsersTable = dynamic_cast<GuiTable*>(ObtainGuiWidget(_scene, mCfg->mUsersTable, GuiWidgetKind::TABLE));
    if(mUsersTable==nullptr){
        return false;
    }
    mSelectorWidgets.push_back(mUsersTable);

    //---
    mAddNewUserButton = dynamic_cast<GuiButton*>(ObtainGuiWidget(_scene, mCfg->mAddNewUserButton, GuiWidgetKind::BUTTON));
    if(mAddNewUserButton==nullptr){
        return false;
    }
    mSelectorWidgets.push_back(mAddNewUserButton);

    //---
    mBDeleteUserButton = dynamic_cast<GuiButton*>(ObtainGuiWidget(_scene, mCfg->mBDeleteUserButton, GuiWidgetKind::BUTTON));
    if(mBDeleteUserButton==nullptr){
        return false;
    }
    mSelectorWidgets.push_back(mBDeleteUserButton);


    //----
    mNewUserPanelComponent = dynamic_cast<OverlayComponent*>(_scene->overlayComponentsGroup()->getComponent(mCfg->mNewUserPanelComponent));
    if(mNewUserPanelComponent==nullptr){
        return false;
    }

    mAOpenNewUserPanel.reset(new ADoTransition(mCfg->mNewUserPanelComponentTransition, mCfg->mNewUserPanelComponentTransitionShow, ""));
    mACloseNewUserPanel.reset(new ADoTransition(mCfg->mNewUserPanelComponentTransition, "", mCfg->mNewUserPanelComponentTransitionShow));

    if(mAOpenNewUserPanel->initConnections(_scene)==false){
        return false;
    }
    if(mACloseNewUserPanel->initConnections(_scene)==false){
        return false;
    }

    //----
    mConfirmDeleteUserComponent = dynamic_cast<OverlayComponent*>(_scene->overlayComponentsGroup()->getComponent(mCfg->mConfirmDeleteUserComponent));
    if(mConfirmDeleteUserComponent==nullptr){
        return false;
    }


    mAOpenConfirmDeletePanel.reset(new ADoTransition(mCfg->mConfirmDeleteUserComponentTransition, mCfg->mConfirmDeleteUserComponentTransitionShow, ""));
    mACloseConfirmDeletePanel.reset(new ADoTransition(mCfg->mConfirmDeleteUserComponentTransition, "", mCfg->mConfirmDeleteUserComponentTransitionShow));

    if(mAOpenConfirmDeletePanel->initConnections(_scene)==false){
        return false;
    }
    if(mACloseConfirmDeletePanel->initConnections(_scene)==false){
        return false;
    }

    mConfirmPanel_resultYes = mParentPlayerScene->triggers().getTrigger("confirmPanel_resultYes");
    if(mConfirmPanel_resultYes==nullptr){
        return false;
    }

    mConfirmPanel_resultNo = mParentPlayerScene->triggers().getTrigger("confirmPanel_resultNo");
    if(mConfirmPanel_resultNo==nullptr){
        return false;
    }

    // data
    TextSegment * confirmDeleteTextSegment = ObtainTextSegment(mCfg->mConfirmDeleteTextSegment);
    if(confirmDeleteTextSegment==nullptr){
        return false;
    }
    mConfirmDeleteUserComponentData.reset(new ComponentData());
    mConfirmDeleteUserComponentData->mTextSegments.push_back(confirmDeleteTextSegment);

    //----
    mTSActiveUserName= dynamic_cast<GTextSprite*>(mParentPlayerScene->gfxObjectsGroup()->getGfxObject("activeUserName"));
    if(mTSActiveUserName==nullptr){
        return false;
    }

    mVarNewUserName = dynamic_cast<StringVar*>(mNewUserPanelComponent->variables().getVariable("newUserName"));
    if(mVarNewUserName==nullptr){
        return false;
    }


    //--- set users items stuff
    mUsersNamesItems = app->usersDatabase()->userNamesItems();
    mUsersTable->setItemsGroup(mUsersNamesItems);
    UserProfile *activeUser = app->usersDatabase()->activeUser();
    if(activeUser){
        GameItem *activeItem = mUsersNamesItems->getItem(activeUser->name());
        mUsersTable->setSelectedItem(activeItem);
    }


    //---
    dbgSystem.removeLastMessage();

    return true;
}


void UserSelector::start()
{

    mState = State::SELECTOR;
    mParentPlayerScene->widgetManager()->appendToUsedWidgets(mSelectorWidgets);

    //---
    mBDeleteUserButton->SetDisabled(app->usersDatabase()->users().size()<=1);
}


void UserSelector::update(UpdateMode _updateMode)
{

    if(_updateMode==UpdateMode::MODAL_OVERLAY){
        CheckConfirmPanelTriggers();
        return;
    }

    //---
    if(mState==State::SELECTOR){

        //--- CHANGE ACTIVE USER
        if(mUsersTable->IsValueChanged()){
            GameItem* item = mUsersTable->selectedItem();
            if(item){
                app->usersDatabase()->setActiveUser(item->name());

                //--- set active user text sprite
                if(mTSActiveUserName){
                    mTSActiveUserName->textSprite()->setText(item->name());
                }
            }
        }

        //--- ADD NEW USER - OPEN NEW USER PANEL
        if(mAddNewUserButton->IsPressed()){

            mNewUserPanelComponent->setTransitions(mAOpenNewUserPanel.get(), mACloseNewUserPanel.get());
            mNewUserPanelComponent->_setModal(true);
            mParentPlayerScene->startOverlayComponent(mNewUserPanelComponent);

            mState = State::NEW_USER_PANEL;


        //--- DELETE USER
        }else if(mBDeleteUserButton->IsPressed()){

            GameItem *item = mUsersTable->selectedItem();
            if(item){
                //--- CONFIRM DELETE - OPEN CONFIRM DELETE PANEL
                mConfirmDeleteUserComponent->setTransitions(mAOpenConfirmDeletePanel.get(), mACloseConfirmDeletePanel.get());
                mConfirmDeleteUserComponent->_setModal(true);

                //replace
                TextSegment *ts = mConfirmDeleteUserComponentData.get()->mTextSegments.front();
                ts->Replace("userName", item->name());

                mConfirmDeleteUserComponent->setData(mConfirmDeleteUserComponentData.get());
                mParentPlayerScene->startOverlayComponent(mConfirmDeleteUserComponent);

                mState = State::CONFIRM_DELETE_PANEL;

            }


            /*
            GameItem *item = mUsersTable->selectedItem();
            if(item){
                if(app->usersDatabase()->deleteUser(item->name())){
                    //--- set active user text sprite
                    if(app->usersDatabase()->activeUser()){
                        mTSActiveUserName->textSprite()->setText(app->usersDatabase()->activeUser()->name());
                    }
                    //---
                    mBDeleteUserButton->SetDisabled(app->usersDatabase()->users().size()<=1);
                }
            }
            */
        }

    }else if(mState==State::NEW_USER_PANEL){

        if(mNewUserPanelComponent->isClosed()){
            mBDeleteUserButton->SetDisabled(app->usersDatabase()->users().size()<=1);
            mState = State::SELECTOR;
        }

    }else if(mState==State::CONFIRM_DELETE_PANEL){

        if(mConfirmDeleteUserComponent->isClosed()){
            mState = State::SELECTOR;
        }

    }

}


void UserSelector::CheckConfirmPanelTriggers()
{

    if(mConfirmPanel_resultYes->isTriggered()){

        GameItem *item = mUsersTable->selectedItem();
        if(item){
            if(app->usersDatabase()->deleteUser(item->name())){
                //--- set active user text sprite
                if(app->usersDatabase()->activeUser()){
                    mTSActiveUserName->textSprite()->setText(app->usersDatabase()->activeUser()->name());
                }
                //---
                mBDeleteUserButton->SetDisabled(app->usersDatabase()->users().size()<=1);
            }
        }
    }

}


void UserSelector::onStateEnded()
{

    mParentPlayerScene->widgetManager()->removeFromUsedWidgets(mSelectorWidgets);
}


}
