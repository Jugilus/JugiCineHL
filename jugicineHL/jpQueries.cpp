#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmSprite.h"
#include "jmCompositeSprite.h"
#include "jmStreams.h"
#include "jmUtilities.h"

#include "gfxObjects/jpAnimationObjects.h"
#include "jpItemsTable.h"
#include "jpPlayedScene.h"
#include "jpPlayedApp.h"
#include "jpInput.h"
#include "jpUtilities.h"

//#include "scripting/jpBehavior.h"
//#include "jpSourceEntity.h"
//#include "jpEntity.h"
//#include "jpEntityUtilities.h"
#include "jpQueries.h"


namespace jugimap{



SignalAccessor::~SignalAccessor()
{

    if(mOwnedSignal){
        delete mSignal;
    }

}


//==================================================================================


bool SignalQuery::active(){

    if(mSignal->type()==ValueStateType::BOOL){

        BoolSignal *vs = static_cast<BoolSignal*>(mSignal);
        //return vs->activeFromStyle(mStyle);

        if(mStyle==Style::ACTIVE_STARTED){
            return vs->activeStarted();

        }else if(mStyle==Style::ACTIVE){
            return vs->active();

        }else if(mStyle==Style::ACTIVE_ENDED){
            return vs->activeEnded();

        }else if(mStyle==Style::NOT_ACTIVE){
            return vs->notActive();

        }

    }else if(mSignal->type()==ValueStateType::INT){

        IntSignal *vs = static_cast<IntSignal*>(mSignal);
        //return vs->activeFromStyle(mIntValue, mStyle);

        if(mStyle==Style::ACTIVE_STARTED){
            return vs->activeStarted(mIntValue);

        }else if(mStyle==Style::ACTIVE){
            return vs->active(mIntValue);

        }else if(mStyle==Style::ACTIVE_ENDED){
            return vs->activeEnded(mIntValue);

        }else if(mStyle==Style::NOT_ACTIVE){
            return vs->notActive(mIntValue);

        }

    }else if(mSignal->type()==ValueStateType::INT_BITS){

        IntBitsSignal *vs = static_cast<IntBitsSignal*>(mSignal);

        if(mStyle==Style::ACTIVE_STARTED){
            return vs->activeStarted(mIntValue);

        }else if(mStyle==Style::ACTIVE){
            return vs->active(mIntValue);

        }else if(mStyle==Style::ACTIVE_ENDED){
            return vs->activeEnded(mIntValue);

        }else if(mStyle==Style::NOT_ACTIVE){
            return vs->notActive(mIntValue);

        }
    }


    return false;

}




void SignalSetter::set()
{

    if(mSignal->type()==ValueStateType::BOOL){

        BoolSignal *vs = static_cast<BoolSignal*>(mSignal);

        if(mStyle==Style::SET){
            //vs->_setActive(true);
            vs->_setOnNextFrame();

        }else if(mStyle==Style::UNSET){
            //vs->_Set(false);
            vs->_unsetOnNextFrame();
        }


    }else if(mSignal->type()==ValueStateType::INT){

        IntSignal *vs = static_cast<IntSignal*>(mSignal);
        //return vs->activeFromStyle(mIntValue, mStyle);

        if(mStyle==Style::SET){
            //vs->_set(mIntValue);
            vs->_setOnNextFrame(mIntValue);

        }else if(mStyle==Style::UNSET){
            //vs->_set(0);
            vs->_unsetOnNextFrame(mIntValue);
        }

    }else if(mSignal->type()==ValueStateType::INT_BITS){

        IntBitsSignal *vs = static_cast<IntBitsSignal*>(mSignal);

        if(mStyle==Style::SET){
            //vs->_set(mIntValue);
            vs->_setFlagOnNextFrame(mIntValue);

        }else if(mStyle==Style::UNSET){
            //vs->_unset(mIntValue);
            vs->_unsetFlagOnNextFrame(mIntValue);
        }

    }

}


//===================================================================================


bool CompositeQuery::parse(std::string text)
{

    StdString::removeStringWhiteSpacesOnStartAndEnd(text);


    if(mKind==Kind::UNKNOWN){    // root group

        if(text.size()>=2){
            if(text.substr(0,1)=="("){
                if(text.substr(text.length()-2, 1)!=")"){
                    dbgSystem.addMessage("Erros parsing string '" + text + "'! Missing ')' !");
                    return false;
                }

                text = text.substr(1, text.length()-2);
                mKind = Kind::AND;

            }else if(text.substr(0,1)=="["){
                if(text.substr(text.length()-1, 1)!="]"){
                    dbgSystem.addMessage("Erros parsing string '" + text + "'! Missing ']' !");
                    return false;
                }

                text = text.substr(1, text.length()-2);
                mKind = Kind::OR;
            }
        }
    }


    if(mKind==Kind::UNKNOWN){
        mKind = Kind::AND;
    }


    //---

    std::vector<std::string>triggers = StdString::splitString(text, ",");

    for(const std::string &trigger : triggers){


        if(trigger.size()>=2){

            if(trigger.substr(0,1)=="("){
                if(trigger.substr(trigger.length()-2, 1)!=")"){
                    dbgSystem.addMessage("Error parsing string '" + text + "'! Missing ')' !");
                    return false;
                }

                std::string triggersGroup = trigger.substr(1, trigger.length()-2);

                mChildTriggersGroups.push_back(CompositeQuery(Kind::AND));
                mChildTriggersGroups.back().parse(triggersGroup);

                continue;

            }else if(trigger.substr(0,1)=="["){
                if(trigger.substr(trigger.length()-2, 1)!="]"){
                    dbgSystem.addMessage("Error parsing string '" + text + "'! Missing ']' !");
                    return false;
                }

                std::string triggersGroup = trigger.substr(1, trigger.length()-2);

                mChildTriggersGroups.push_back(CompositeQuery(Kind::OR));
                mChildTriggersGroups.back().parse(triggersGroup);

                continue;
            }

        }


        if(trigger.empty()){
            dbgSystem.addMessage("Error parsing string '" + text + "'!");
            return false;
        }


        mTriggersCfg.push_back(trigger);

    }


    return true;

}



bool CompositeQuery::initConnections(PlayedScene *_scene, void *obj1, void *obj2, void *obj3)
{


    for(const std::string &path : mTriggersCfg){
        //TriggerResult triggerStyle = TriggerResult::ACTIVE;


        mTriggers.emplace_back(SignalQuery());
        SignalQuery &signalQuery = mTriggers.back();

        app->signalParserManager()->parseSignalAccessor(_scene, path, signalQuery, obj1, obj2, obj3);

        if(signalQuery.mNotUsed){
            mTriggers.pop_back();
            continue;
        }

        if(signalQuery.isValid()==false){
            return false;
        }

    }


    for(CompositeQuery &tg : mChildTriggersGroups){
        if(tg.initConnections(_scene, obj1, obj2, obj3)==false){
            return false;
        }

    }

    return true;
}


bool CompositeQuery::isConditionTrue()
{


    for(int i=0; i<mTriggers.size(); i++){
        //bool r = mTriggers[i]->activeFromStyle(mTriggerStyles[i]);
        bool r = mTriggers[i].active();
        if(mKind==Kind::AND){
            if(r==false){
                return false;
            }
        }else if(mKind==Kind::OR){
            if(r==true){
                return true;
            }
        }
    }


    for(CompositeQuery &tg : mChildTriggersGroups){

        bool r = tg.isConditionTrue();

        if(mKind==Kind::AND){
            if(r==false){
                return false;
            }
        }else if(mKind==Kind::OR){
            if(r==true){
                return true;
            }
        }
    }

     if(mKind==Kind::AND){
        return true;
     }

     return false;      // mKind==Kind::OR

}

//=====================================================================================


void CoreSignalsParser::parseSignalAccessor(PlayedScene *scene, const std::string &_path, SignalAccessor &_signalAccessor, void *obj1, void *obj2, void *obj3)
{


    std::vector<std::string>pathParts = StdString::splitString(_path, "/");

    std::string signalOrigin;
    std::string signalName;
    std::string signalStyle;


    if(pathParts.size()>0){
        signalOrigin = pathParts[0];
    }
    if(pathParts.size()>1){
        signalName = pathParts[1];
    }
    if(pathParts.size()>2){
        signalStyle = pathParts[2];
    }


    //-----
    if(signalOrigin=="INPUT_COMMAND"){
        _signalAccessor.mSignal = app->inputSystem()->getGameInputCommand(signalName);
        return;

    }else if(signalOrigin.substr(0,16)=="ANIMATED_OBJECT:"){
        DummyFunction();

        std::string aniObject = signalOrigin.substr(16);

        AnimationObject *animatedObject = scene->animationManager()->getAnimationObject(aniObject);
        if(animatedObject==nullptr){
            return;
        }

        if(_signalAccessor.mType==SignalAccessor::Type::QUERY){
            animatedObject->obtainSignal_signalQuery(static_cast<SignalQuery&>(_signalAccessor), signalName);

        }else if(_signalAccessor.mType==SignalAccessor::Type::SETTER){
            animatedObject->obtainSignal_signalSetter(static_cast<SignalSetter&>(_signalAccessor), signalName);
        }


    }


    dbgSystem.addMessage("Error parsing '" + _path +"' !");
    return;

}



//=====================================================================================


SignalParserManager::~SignalParserManager()
{

    for(SignalParser *sp : mSignalParsers){
        delete sp;
    }

}


void SignalParserManager::addSignalParser(SignalParser *_signalParser)
{

    mSignalParsers.push_back(_signalParser);
}


void SignalParserManager::parseSignalAccessor(PlayedScene *_scene, const std::string &_path, SignalAccessor &_signalAccessor, void *obj1, void *obj2, void *obj3)
{

    std::vector<std::string>pathParts = StdString::splitString(_path, "/");

    std::string signalOrigin;
    std::string signalName;
    std::string signalStyle;


    if(pathParts.size()>0){
        signalOrigin = pathParts[0];
    }
    if(pathParts.size()>1){
        signalName = pathParts[1];
    }
    if(pathParts.size()>2){
        signalStyle = pathParts[2];
    }


    if(signalOrigin==""){
        dbgSystem.addMessage("Parse signal query from string '" + _path +"' error! Signal origin is empty string!");
        return;
    }


    std::string parserKeyword = signalOrigin;

    std::vector<std::string>parts = StdString::splitString(parserKeyword, ":");
    if(parts.size()>1){
        parserKeyword = parts[0];
    }


    SignalParser *signalParser = nullptr;
    for(SignalParser *sp : mSignalParsers){
        if(StdVector::contains(sp->keyWords(), parserKeyword)){
            signalParser = sp;
            break;
        }
    }

    if(signalParser==nullptr){
        dbgSystem.addMessage("Parse signal query from string '" + _path +"' error! No signal parser found for keyword '" + parserKeyword + "'!");
        return;
    }


    signalParser->parseSignalAccessor(_scene, _path, _signalAccessor, obj1, obj2, obj3);

    if(_signalAccessor.mNotUsed){
        return;
    }


    parseSignalAccessorStyle(signalStyle, _signalAccessor);


}



bool SignalParserManager::parseSignalAccessorStyle(const std::string & syle, SignalAccessor &_signalAccessor)
{


    if(_signalAccessor.mType==SignalAccessor::Type::QUERY){

        SignalQuery &_signalQuerry = static_cast<SignalQuery&>(_signalAccessor);

        if(syle==""){
            _signalQuerry.mStyle = SignalQuery::Style::ACTIVE;

        }else{

            //_signalQuerry.mStyle = _ParseTriggerStyle(syle);


            if(syle=="START"){
                _signalQuerry.mStyle = SignalQuery::Style::ACTIVE_STARTED;

             }else if(syle=="ACTIVE"){
                _signalQuerry.mStyle = SignalQuery::Style::ACTIVE;

            }else if(syle=="END"){
               _signalQuerry.mStyle = SignalQuery::Style::ACTIVE_ENDED;

            }else if(syle=="NOT_ACTIVE"){
               _signalQuerry.mStyle = SignalQuery::Style::NOT_ACTIVE;

            }else{
                dbgSystem.addMessage("Error parsing signal query style from string'" + syle +"' !");
                return false;
            }
        }


    }else if(_signalAccessor.mType==SignalAccessor::Type::SETTER){

        SignalSetter &_signalSetter = static_cast<SignalSetter&>(_signalAccessor);

        if(syle==""){
            _signalSetter.mStyle = SignalSetter::Style::SET;

        }else{

            if(syle=="SET"){
                _signalSetter.mStyle = SignalSetter::Style::SET;

            }else if(syle=="UNSET"){
                _signalSetter.mStyle = SignalSetter::Style::UNSET;

            }else{
                dbgSystem.addMessage("Error parsing signal setter style from string'" + syle +"' !");
                return false;

            }
        }

    }

    return true;
}





}
