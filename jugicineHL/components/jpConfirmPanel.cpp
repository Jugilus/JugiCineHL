#include <chrono>
#include <assert.h>
#include <sstream>
#include "pugixml/pugixml.hpp"
#include "jmSystem.h"

#include "gui/widgets/jpGuiButton.h"
#include "gui/widgets/jpGuiTextField.h"
#include "gui/jpGuiSystem.h"

#include "jpCompound.h"
#include "jpPlayedApp.h"
#include "jpPlayedScene.h"
#include "jpUtilities.h"
#include "jpConfirmPanel.h"



namespace jugimap{



ConfirmPanel::ConfirmPanel(const pugi::xml_node &_node)
{
    mName = "confirmPanel";

    mCfg.reset(new Cfg());
    //mModal = _node.attribute("modal").as_bool("true");

    //---
    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());

        if(nodeName=="useWidgets"){
            mCfg->mTextField = n.attribute("textField").as_string("");
            mCfg->mYesButton = n.attribute("YesButton").as_string("");
            mCfg->mNoButton = n.attribute("NoButton").as_string("");

        }else if(nodeName=="signals"){
            mCfg->mSigConfirmsYes = n.attribute("confirmYes").as_string("");
            mCfg->mSigConfirmsNo = n.attribute("confirmNo").as_string("");
        }

    }

}


bool ConfirmPanel::initConnections(PlayedScene *_scene)
{

    if(mInitialized) return true;

    //if(_scene->guiSystem()->isInitialized()==false){
    //    if(_scene->guiSystem()->initConnections(_scene)==false){
    //        return false;
    //    }
    //}


    dbgSystem.addMessage("init component '" + mName + "'");

    mParentPlayerScene = _scene;

    //---
    mYesButton = dynamic_cast<GuiButton*>(ObtainGuiWidget(_scene, mCfg->mYesButton, WidgetType::BUTTON));
    if(mYesButton==nullptr){
        return false;
    }
    mUsedWidgets.push_back(mYesButton);

    //---
    mNoButton = dynamic_cast<GuiButton*>(ObtainGuiWidget(_scene, mCfg->mNoButton, WidgetType::BUTTON));
    if(mNoButton==nullptr){
        return false;
    }
    mUsedWidgets.push_back(mNoButton);

    //---
    mTextField = dynamic_cast<GuiTextField*>(ObtainGuiWidget(_scene, mCfg->mTextField, WidgetType::TEXT_FIELD));
    if(mTextField==nullptr){
        return false;
    }
    mUsedWidgets.push_back(mTextField);

    //---
    mTrigger_Closed = mParentPlayerScene->triggers().addTrigger("confirmPanelClosed");
    if(mTrigger_Closed==nullptr){
        return false;
    }
    //---
    mResult_Yes = mParentPlayerScene->triggers().addTrigger("confirmPanel_resultYes");
    if(mResult_Yes==nullptr){
        return false;
    }

    //---
    mResult_No = mParentPlayerScene->triggers().addTrigger("confirmPanel_resultNo");
    if(mResult_No==nullptr){
        return false;
    }


    //---
    if(mCfg->mSigConfirmsYes.empty()==false){
        app->signalParserManager()->parseSignalAccessor(_scene, mCfg->mSigConfirmsYes, mQConfirmsYes);
        if(mQConfirmsYes.isValid()==false){
            return false;
        }
        //mQConfirmsYes.mOwnedSignal = true;
    }

    if(mCfg->mSigConfirmsNo.empty()==false){
        app->signalParserManager()->parseSignalAccessor(_scene, mCfg->mSigConfirmsNo, mQConfirmsNo);
        if(mQConfirmsNo.isValid()==false){
            return false;
        }
        //mQConfirmsNo.mOwnedSignal = true;
    }


    //---
    mInitialized = true;

    dbgSystem.removeLastMessage();

    return true;
}


void ConfirmPanel::start()
{

    //startOpenTransition();

    //---
    if(mData){
        if(mData->mTextSegments.empty()==false){
            mTextField->Start(mData->mTextSegments.front());
        }
    }

}


void ConfirmPanel::update(UpdateMode &_updateMode)
{

    //OverlayComponent::update(_updateMode);     // // must call parent class which manages transitions!

    //if(mState==State::NORMAL){

        if(mYesButton->isPressedStarted()){
            mParentPlayerScene->activatedTriggersUpdater().addTrigger(mResult_Yes);
            startCloseTransition();

        }else if(mNoButton->isPressedStarted()){
            mParentPlayerScene->activatedTriggersUpdater().addTrigger(mResult_No);
            startCloseTransition();
        }
    //}

}



void ConfirmPanel::onStateEnded()
{

}


}
