#include <chrono>
#include <assert.h>
#include <sstream>
#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmGuiButton.h"
#include "jmGuiTextField.h"
#include "jmCommonFunctions.h"
#include "jmTextSprite.h"
#include "jpPlayedApp.h"
#include "jpPlayedScene.h"
#include "jpUtilities.h"
#include "jpMessagePanel.h"


namespace jugimap{



MessagePanel::MessagePanel(const pugi::xml_node &_node)
{
    mName = "messagePanel";

    mCfg.reset(new Cfg());
    mModal = _node.attribute("modal").as_bool("true");

    //---
    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());

        if(nodeName=="useWidgets"){
            mCfg->mTextField = n.attribute("textField").as_string("");
            mCfg->mOkButton = n.attribute("OkButton").as_string("");
        }
    }
}


bool MessagePanel::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("init component '" + mName + "'");

    mParentPlayerScene = _scene;

    //---
    mOkButton = dynamic_cast<GuiButton*>(ObtainGuiWidget(_scene, mCfg->mOkButton, GuiWidgetKind::BUTTON));
    if(mOkButton==nullptr){
        return false;
    }
    mUsedWidgets.push_back(mOkButton);

    //---
    mTextField = dynamic_cast<GuiTextField*>(ObtainGuiWidget(_scene, mCfg->mTextField, GuiWidgetKind::TEXT_FIELD));
    if(mTextField==nullptr){
        return false;
    }
    mUsedWidgets.push_back(mTextField);

    //TextSegment *ts = ObtainTextSegment("gui/44");
    //mTextField->Start(ts);

    //---
    mTrigger_Closed = mParentPlayerScene->triggers().addTrigger("messagePanelClosed");
    if(mTrigger_Closed==nullptr){
        return false;
    }

    dbgSystem.removeLastMessage();

    return true;
}


void MessagePanel::start()
{

    startOpenTransition();                  // must call this!

    //---
    if(mData){
        if(mData->mTextSegments.empty()==false){
            mTextField->Start(mData->mTextSegments.front());
        }
    }

}


void MessagePanel::update(UpdateMode _updateMode)
{

    OverlayComponent::update(_updateMode);     // // must call parent class which manages transitions!

    if(mState==State::NORMAL){

        if(mOkButton->IsPressed()){
            startCloseTransition();
        }
    }

}


void MessagePanel::onStateEnded()
{

}


}
