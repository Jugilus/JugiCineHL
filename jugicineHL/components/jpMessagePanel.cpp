#include <chrono>
#include <assert.h>
#include <sstream>
#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmCommonFunctions.h"
#include "jmTextSprite.h"

#include "gui/jpGuiSystem.h"
#include "gui/widgets/jpGuiButton.h"
#include "gui/widgets/jpGuiTextField.h"

#include "jpCompound.h"
#include "jpPlayedApp.h"
#include "jpPlayedScene.h"
#include "jpUtilities.h"
#include "jpMessagePanel.h"


namespace jugimap{



MessagePanel::MessagePanel(const std::string &_name)
{
     mName = _name;
     mCfg.reset(new Cfg());

     mMessagePanelSignalParser = new MessagePanelSignalParser({mName});
     app->signalParserManager()->addAndStoreSignalParser(mMessagePanelSignalParser);
}



MessagePanel::MessagePanel(const pugi::xml_node &_node)
{

    mName = "messagePanel";

    mCfg.reset(new Cfg());
    //mModal = _node.attribute("modal").as_bool("true");

    //---
    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());

        if(nodeName=="useWidgets"){
            mCfg->mTextField = n.attribute("textField").as_string("");
            mCfg->mOkButton = n.attribute("OkButton").as_string("");

        }else if(nodeName=="signals"){
            mCfg->mSigClose = n.attribute("close").as_string("");
        }
    }
}


bool MessagePanel::initCfg(PlayedScene *_scene, const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading component '" + mName + " ...");

    mParentPlayerScene = _scene;


    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){

        std::string nodeName = std::string(n.name());

        if(nodeName=="useWidgets"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="OkButton"){
                    mCfg->mOkButton = a.as_string();

                }else if(attributeName=="textField"){
                    mCfg->mTextField = a.as_string();

                }else{

                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                    return false;
                }

            }

        }else if(nodeName=="signals"){


            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="close"){
                     mCfg->mSigClose = a.as_string();

                }else{

                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                    return false;
                }

            }
        }

    }


    dbgSystem.removeLastMessage();

    return true;

}



bool MessagePanel::initConnections(PlayedScene *_scene)
{

    if(mInitialized) return true;


    dbgSystem.addMessage("Initializing component '" + mName + "'");

    mParentPlayerScene = _scene;

    //---
    mOkButton = dynamic_cast<GuiButton*>(ObtainGuiWidget(_scene, mCfg->mOkButton, WidgetType::BUTTON));
    if(mOkButton==nullptr){
        return false;
    }
    mUsedWidgets.push_back(mOkButton);

    //---
    mTextField = dynamic_cast<GuiTextField*>(ObtainGuiWidget(_scene, mCfg->mTextField, WidgetType::TEXT_FIELD));
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

    if(mCfg->mSigClose.empty()==false){

        if(mSQclose.parse(mCfg->mSigClose)==false){
            return false;
        }
        if(mSQclose.initConnections(_scene)==false){
            return false;
        }

        //app->signalParserManager()->parseSignalAccessor(_scene, mCfg->mSigClose, mSQclose);
        //if(mSQclose.isValid()==false){
        //    return false;
        //}
    }


    //---
    mInitialized = true;

    //---
    dbgSystem.removeLastMessage();
    return true;
}


void MessagePanel::start()
{

    //startOpenTransition();

    mSigClose.reset();

    //---
    if(mData){
        if(mData->mTextSegments.empty()==false){
            mTextField->Start(mData->mTextSegments.front());
        }
    }

}


void MessagePanel::update(UpdateMode &_updateMode)
{


    //OverlayComponent::update(_updateMode);     // // must call parent class which manages transitions!

    //if(mState==State::NORMAL){

        //if(mOkButton->isPressedStarted()){
       //     startCloseTransition();
        //}


    if(mSQclose.active()){
        mSigClose.setValue(true);
        startCloseTransition();
    }
    //}

}


void MessagePanel::onStateEnded()
{

}


void MessagePanel::obtainSignal_signalQuery(SignalQuery &_signalQuery, ParsedSignalPath &_psp, bool _setErrorMessage)
{

    if(_psp.signalFullName()=="close"){
        _psp.obtainValue(_signalQuery, &mSigClose);
    }

    if(_setErrorMessage && _signalQuery.signal()==nullptr){
        dbgSystem.addMessage("Get signal '" + _psp.signalFullName() + "' error! The signal is unknown!");
    }

}


void MessagePanel::obtainSignal_signalSetter(SignalSetter &_signalSetter, ParsedSignalPath &_psp, bool _setErrorMessage)
{

    if(_setErrorMessage && _signalSetter.signal()==nullptr){
        dbgSystem.addMessage("Set signal '" + _psp.signalFullName() + "' error! The signal is unknown!");
    }

}


//==================================================================================


void MessagePanelSignalParser::parseSignalAccessor(PlayedScene *scene, const std::string &_path, SignalAccessor &_signalAccessor, BaseObject *obj1, BaseObject *obj2)
{


    MessagePanel *messagePanel = dynamic_cast<MessagePanel*>(scene->componentsGroup()->getComponent("messagePanel"));

    if(messagePanel==nullptr){
        dbgSystem.addMessage("Signal parser error! Component 'messagePanel' not found!");
        return;
    }


    std::vector<std::string>pathParts = StdString::splitString(_path, "/");

    std::string signalOrigin;
    std::string signalPath;
    std::string signalStyle;


    if(pathParts.size()>0){
        signalOrigin = pathParts[0];
    }
    if(pathParts.size()>1){
        signalPath = pathParts[1];
    }
    if(pathParts.size()>2){
        signalStyle = pathParts[2];
    }


    //-----

    if(signalPath.empty()==false){

        ParsedSignalPath psp(signalPath);
        //psp.parsePath_name_value(signalPath);

        if(_signalAccessor.type()==SignalAccessorType::QUERY){
            messagePanel->obtainSignal_signalQuery(static_cast<SignalQuery&>(_signalAccessor), psp);

        }else if(_signalAccessor.type()==SignalAccessorType::SETTER){
            messagePanel->obtainSignal_signalSetter(static_cast<SignalSetter&>(_signalAccessor), psp);
        }
    }

    if(_signalAccessor.signal()){
        return;
    }


    //----
    dbgSystem.addMessage("Error parsing '" + _path +"' !");
    return;

}



}
