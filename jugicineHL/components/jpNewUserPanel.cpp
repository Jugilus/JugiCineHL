#include <chrono>
#include <assert.h>
#include <sstream>
#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmCommonFunctions.h"
#include "jmTextSprite.h"

#include "gui/jpGuiSystem.h"
#include "gui/widgets/jpGuiButton.h"
#include "gui/widgets/jpGuiTextInput.h"

#include "jpCompound.h"
#include "jpGfxObjectsCommon.h"
#include "jpPlayedApp.h"
#include "jpPlayedScene.h"
#include "jpUser.h"
#include "jpUtilities.h"
#include "jpNewUserPanel.h"



namespace jugimap{



NewUserPanel::NewUserPanel(const pugi::xml_node &_node)
{
    mName = "newUserPanel";

    mCfg.reset(new Cfg());
    //mModal = _node.attribute("modal").as_bool("true");

    //---
    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());

        if(nodeName=="useWidgets"){
            mCfg->mTextInput = n.attribute("textInput").as_string("");
            mCfg->mOkButton = n.attribute("OkButton").as_string("");
            mCfg->mCancelButton = n.attribute("CancelButton").as_string("");
        }
    }

    //---
    mVarNewUserName = static_cast<StringVar*>(mVariables.addVariable(new StringVar("newUserName", "")));

}


bool NewUserPanel::initConnections(PlayedScene *_scene)
{

    if(mInitialized) return true;


    dbgSystem.addMessage("init component 'newUserPanel'");

    mParentPlayerScene = _scene;

    //---
    mOkButton = dynamic_cast<GuiButton*>(ObtainGuiWidget(_scene, mCfg->mOkButton, WidgetType::BUTTON));
    if(mOkButton==nullptr){
        return false;
    }
    mUsedWidgets.push_back(mOkButton);

    //---
    mCancelButton = dynamic_cast<GuiButton*>(ObtainGuiWidget(_scene, mCfg->mCancelButton, WidgetType::BUTTON));
    if(mCancelButton==nullptr){
        return false;
    }
    mUsedWidgets.push_back(mCancelButton);

    //---
    mTextInput = dynamic_cast<GuiTextInput*>(ObtainGuiWidget(_scene, mCfg->mTextInput, WidgetType::TEXT_INPUT));
    if(mTextInput==nullptr){
        return false;
    }
    mUsedWidgets.push_back(mTextInput);

    //---
    mTSActiveUserName = dynamic_cast<GTextSprite*>(mParentPlayerScene->gfxObjectsGroup()->getGfxObject("activeUserName"));
    if(mTSActiveUserName==nullptr){
        return false;
    }

    mTrigger_Closed = mParentPlayerScene->triggers().addTrigger("newUserPanelClosed");
    if(mTrigger_Closed==nullptr){
        return false;
    }


    //---
    mInitialized = true;


    //---
    dbgSystem.removeLastMessage();
    return true;
}


void NewUserPanel::start()
{

    //startOpenTransition();                  // must call this!

    //---
    mAddingFirstUser = app->usersDatabase()->users().empty();
    mCancelButton->setDisabled(mAddingFirstUser);
    mVarNewUserName->setValue("");
    mTextInput->start("");

}


void NewUserPanel::update(UpdateMode &_updateMode)
{

    //OverlayComponent::update(_updateMode);     // // must call parent class which manages transitions!

    //if(mState==State::NORMAL){

        mOkButton->setDisabled(mAddingFirstUser && mTextInput->text().empty());

        if(mOkButton->isPressedStarted()){
            mVarNewUserName->setValue(mTextInput->text());
            finishNewUserPanel();

        }else if(mCancelButton->isPressedStarted()){
            mVarNewUserName->setValue("");
            finishNewUserPanel();

        }else if(mTextInput->isValueChanged()){
            mVarNewUserName->setValue(mTextInput->text());
            finishNewUserPanel();
        }
    //}

}


void NewUserPanel::finishNewUserPanel()
{

    if(mAddingFirstUser && mVarNewUserName->value().empty()){
        return;
    }

    //--- add user (empty string and name duplicate is checked by usersDatabase::addUser)
    if(app->usersDatabase()->addUser(mVarNewUserName->value())){
        app->usersDatabase()->setActiveUser(mVarNewUserName->value());

        //--- set active user text sprite
        mTSActiveUserName->textSprite()->setText(mVarNewUserName->value());

    }else{
        mVarNewUserName->setValue("");
    }

    // close the panel

    startCloseTransition();


}


void NewUserPanel::onStateEnded()
{
    mCancelButton->setDisabled(false);
}


}
