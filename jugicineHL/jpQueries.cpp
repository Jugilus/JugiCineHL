#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmSprite.h"
#include "jmCompositeSprite.h"
#include "jmStreams.h"
#include "jmUtilities.h"

#include "logic/jpLogicState.h"
#include "gfxObjects/jpGfxObjectsCommon.h"
#include "gfxObjects/jpAnimationObjects.h"
#include "jpPlayedScene.h"
#include "jpPlayedApp.h"
#include "jpInput.h"
#include "jpUser.h"
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


    //if(mSignal->dbgName == "inputCommand-fire"){
    //    std::string dbgInfo = mSignal->getDbgInfo();
    //    print(dbgInfo);
    //}


    if(mSignal->type()==SignalType::BOOL){

        BoolSignal *vs = static_cast<BoolSignal*>(mSignal);

        if(mStyle==Style::ACTIVE_STARTED){
            return vs->activeStarted(true);

        }else if(mStyle==Style::ACTIVE){
            return vs->active(true);

        }else if(mStyle==Style::ACTIVE_ENDED){
            return vs->activeEnded(true);

        }else if(mStyle==Style::ACTIVE_CHANGED){
            if(mAnyValueChanged){
                return vs->valueChanged();
            }else{
                return vs->activeChanged(true);
            }

        }else if(mStyle==Style::NOT_ACTIVE){
            return vs->notActive(true);

        }


    }else if(mSignal->type()==SignalType::INT){

        IntSignal *vs = static_cast<IntSignal*>(mSignal);

        if(mStyle==Style::ACTIVE_STARTED){
            return vs->activeStarted(mIntValue);

        }else if(mStyle==Style::ACTIVE){
            return vs->active(mIntValue);

        }else if(mStyle==Style::ACTIVE_ENDED){
            return vs->activeEnded(mIntValue);

        }else if(mStyle==Style::ACTIVE_CHANGED){
            if(mAnyValueChanged){
                return vs->valueChanged();
            }else{
                return vs->activeChanged(mIntValue);
            }

        }else if(mStyle==Style::NOT_ACTIVE){
            return vs->notActive(mIntValue);

        }


    }else if(mSignal->type()==SignalType::FLOAT){

        FloatSignal *vs = static_cast<FloatSignal*>(mSignal);

        if(mStyle==Style::ACTIVE_STARTED){
            return vs->activeStarted(mFloatValue);

        }else if(mStyle==Style::ACTIVE){
            return vs->active(mFloatValue);

        }else if(mStyle==Style::ACTIVE_ENDED){
            return vs->activeEnded(mFloatValue);

        }else if(mStyle==Style::ACTIVE_CHANGED){
            if(mAnyValueChanged){
                return vs->valueChanged();
            }else{
                return vs->activeChanged(mFloatValue);
            }

        }else if(mStyle==Style::NOT_ACTIVE){
            return vs->notActive(mFloatValue);

        }

    }else if(mSignal->type()==SignalType::BITSET){

        BitsetSignal *vs = static_cast<BitsetSignal*>(mSignal);

        if(mStyle==Style::ACTIVE_STARTED){
            return vs->activeStarted(mIntValue);

        }else if(mStyle==Style::ACTIVE){
            return vs->active(mIntValue);

        }else if(mStyle==Style::ACTIVE_ENDED){
            return vs->activeEnded(mIntValue);

        }else if(mStyle==Style::ACTIVE_CHANGED){
            if(mAnyValueChanged){
                return vs->valueChanged();
            }else{
                return vs->activeChanged(mIntValue);
            }

        }else if(mStyle==Style::NOT_ACTIVE){
            return vs->notActive(mIntValue);

        }


    }else if(mSignal->type()==SignalType::STRING){

        StringSignal *vs = static_cast<StringSignal*>(mSignal);

        if(mStyle==Style::ACTIVE_STARTED){
            return vs->activeStarted(mStringValue);

        }else if(mStyle==Style::ACTIVE){
            return vs->active(mStringValue);

        }else if(mStyle==Style::ACTIVE_ENDED){
            return vs->activeEnded(mStringValue);

        }else if(mStyle==Style::ACTIVE_CHANGED){
            if(mAnyValueChanged){
                return vs->valueChanged();
            }else{
                return vs->activeChanged(mStringValue);
            }

        }else if(mStyle==Style::NOT_ACTIVE){
            return vs->notActive(mStringValue);

        }

    }


    return false;

}


void SignalSetter::set()
{

    if(mSignal->type()==SignalType::BOOL){

        BoolSignal *vs = static_cast<BoolSignal*>(mSignal);

        bool value = mBoolValue;

        if(mSigData){
            assert(mSigData->type()==SignalType::BOOL);
            BoolSignal *sigData = static_cast<BoolSignal*>(mSigData);
            value = sigData->value();
        }

        vs->setValue_onNextFrame(value);


    }else if(mSignal->type()==SignalType::INT){

        IntSignal *vs = static_cast<IntSignal*>(mSignal);

        int value = mIntValue;

        if(mSigData){
            assert(mSigData->type()==SignalType::INT);
            IntSignal *sigData = static_cast<IntSignal*>(mSigData);
            value = sigData->value();
        }

        vs->setValue_onNextFrame(value);


    }else if(mSignal->type()==SignalType::FLOAT){

        FloatSignal *vs = static_cast<FloatSignal*>(mSignal);

        float value = mFloatValue;

        if(mSigData){
            assert(mSigData->type()==SignalType::FLOAT);
            FloatSignal *sigData = static_cast<FloatSignal*>(mSigData);
            value = sigData->value();
        }

        vs->setValue_onNextFrame(value);


    }else if(mSignal->type()==SignalType::BITSET){

        BitsetSignal *vs = static_cast<BitsetSignal*>(mSignal);

        unsigned int flags = mIntValue;
        bool flagsState = mBoolValue;

        if(mSigData){
            assert(mSigData->type()==SignalType::BITSET);
            BitsetSignal *sigData = static_cast<BitsetSignal*>(mSigData);
            flags = mSigDataFlag;
            flagsState = sigData->active(mSigDataFlag);
        }

        vs->setFlags_onNextFrame(flags, flagsState);


    }else if(mSignal->type()==SignalType::STRING){

        StringSignal *vs = static_cast<StringSignal*>(mSignal);

        std::string value = mStringValue;

        if(mSigData){
            assert(mSigData->type()==SignalType::STRING);
            StringSignal *sigData = static_cast<StringSignal*>(mSigData);
            value = sigData->value();
        }

        vs->setValue_onNextFrame(value);

    }

    if(mSignal->callback()){
        mSignal->callback()->onSetBySignalSetter(mSignal);
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

    std::vector<std::string>signals = StdString::splitString(text, ",");

    for(const std::string &signal : signals){


        if(signal.size()>=2){

            if(signal.substr(0,1)=="("){
                if(signal.substr(signal.length()-2, 1)!=")"){
                    dbgSystem.addMessage("Error parsing string '" + text + "'! Missing ')' !");
                    return false;
                }

                std::string triggersGroup = signal.substr(1, signal.length()-2);

                mChildrenQueries.push_back(CompositeQuery(Kind::AND));
                mChildrenQueries.back().parse(triggersGroup);

                continue;

            }else if(signal.substr(0,1)=="["){
                if(signal.substr(signal.length()-2, 1)!="]"){
                    dbgSystem.addMessage("Error parsing string '" + text + "'! Missing ']' !");
                    return false;
                }

                std::string triggersGroup = signal.substr(1, signal.length()-2);

                mChildrenQueries.push_back(CompositeQuery(Kind::OR));
                mChildrenQueries.back().parse(triggersGroup);

                continue;
            }

        }


        if(signal.empty()){
            dbgSystem.addMessage("Error parsing string '" + text + "'!");
            return false;
        }


        mSignalQueryCfgs.push_back(signal);

    }


    return true;

}



bool CompositeQuery::initConnections(PlayedScene *_scene, BaseObject *obj1, BaseObject *obj2)
{


    for(const std::string &path : mSignalQueryCfgs){
        //TriggerResult triggerStyle = TriggerResult::ACTIVE;


        mSignalQueries.emplace_back(SignalQuery());
        SignalQuery &signalQuery = mSignalQueries.back();

        app->signalParserManager()->parseSignalAccessor(_scene, path, signalQuery, obj1, obj2);

        if(signalQuery.mNotUsed){
            mSignalQueries.pop_back();
            continue;
        }

        if(signalQuery.isValid()==false){
            return false;
        }

        //if(path == "INPUT_COMMAND/fire/END"){
        //    signalQuery.mSignal->setName("inputCommand-fire");
        //}

    }


    for(CompositeQuery &tg : mChildrenQueries){
        if(tg.initConnections(_scene, obj1, obj2)==false){
            return false;
        }

    }

    return true;
}


bool CompositeQuery::active()
{


    for(int i=0; i<mSignalQueries.size(); i++){

        bool r = mSignalQueries[i].active();

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


    for(CompositeQuery &tg : mChildrenQueries){

        bool r = tg.active();

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


void CoreSignalsParser::parseSignalAccessor(PlayedScene *scene, const std::string &_path, SignalAccessor &_signalAccessor, BaseObject *obj1, BaseObject *obj2)
{

    if(obj1){

    }else{

    }

    LogicState *currentLogicState = nullptr;

    if(obj2){
        if(obj2->baseType()==BaseObjectType::LOGIC_STATE){
            currentLogicState = static_cast<LogicState*>(obj2);
        }
    }


    std::vector<std::string>pathParts = StdString::splitString(_path, "/");

    std::string signalOrigin;
    std::string signalNameAndValue;
    std::string signalStyle;


    if(pathParts.size()>0){
        signalOrigin = pathParts[0];
    }
    if(pathParts.size()>1){
        signalNameAndValue = pathParts[1];
    }
    if(pathParts.size()>2){
        signalStyle = pathParts[2];
    }


    ParsedSignalPath psp(signalNameAndValue);

    //-----
    if(signalOrigin=="STATES"){

        if(currentLogicState==nullptr){
            dbgSystem.addMessage("Current logic state is not a valid logic state! You can access core logic states only within the core state machine!");
            return;
        }

        //ParsedSignalPath psp(signalNameAndValue);

        BoolSignal *stateSignal = nullptr;

        if(psp.signalNamePartAt(0)=="THIS"){
            stateSignal = &(currentLogicState->stateSignal());

        }else{
            LogicState* state = ObtainLogicStateFromPath2(scene, currentLogicState, psp.signalFullName());      // !

            if(state==nullptr){
                return;
            }
            stateSignal = &(state->stateSignal());
        }

        if(_signalAccessor.mType==SignalAccessor::Type::QUERY){
            psp.obtainValue(static_cast<SignalQuery&>(_signalAccessor), stateSignal);

        }else if(_signalAccessor.mType==SignalAccessor::Type::SETTER){
            dbgSystem.addMessage("Setting state signals is not available! Use 'doSetState' command!");
        }


    }else if(signalOrigin=="USERS"){

        if(psp.signalNamePartAt(0)=="ACTIVE_USER"){

            //ParsedSignalPath psp(signalNameAndValue);

            if(_signalAccessor.mType==SignalAccessor::Type::QUERY){
                psp.obtainValue(static_cast<SignalQuery&>(_signalAccessor), app->usersDatabase()->activeUserSignal());

            }else if(_signalAccessor.mType==SignalAccessor::Type::SETTER){
                _signalAccessor.mSignal = app->usersDatabase()->activeUserSignal();
            }
        }


    }else if(signalOrigin.substr(0,6)=="INPUT:"){
        std::string inputSource = signalOrigin.substr(6);

        if(inputSource=="KEYBOARD"){

            //ParsedSignalPath psp(signalNameAndValue);
            BoolSignal *key = keyboard.keySignalForKeyName(psp.signalFullName());
            if(key==nullptr){
                return;
            }

            if(_signalAccessor.mType==SignalAccessor::Type::QUERY){
                psp.obtainValue(static_cast<SignalQuery&>(_signalAccessor), key);

            }else if(_signalAccessor.mType==SignalAccessor::Type::SETTER){
                dbgSystem.addMessage("Setting 'keyboard' signals is not possible!");
            }

        }else if(inputSource=="COMMAND"){

            //ParsedSignalPath psp(signalNameAndValue);

            BoolSignal *command = app->inputSystem()->getGameInputCommand(psp.signalFullName());
            if(command==nullptr){
                return;
            }

            if(_signalAccessor.mType==SignalAccessor::Type::QUERY){
                psp.obtainValue(static_cast<SignalQuery&>(_signalAccessor), command);

            }else if(_signalAccessor.mType==SignalAccessor::Type::SETTER){
                dbgSystem.addMessage("Setting 'command' signals is not possible!");
            }

        }


    }else if(signalOrigin.substr(0,16)=="ANIMATED_OBJECT:"){

        std::string aniObjectName = signalOrigin.substr(16);

        AnimationObject *animatedObject = scene->animationManager()->getAnimationObject(aniObjectName);
        if(animatedObject==nullptr){
            return;
        }

        //ParsedSignalPath psp(signalNameAndValue);

        if(_signalAccessor.mType==SignalAccessor::Type::QUERY){
            animatedObject->obtainSignal_signalQuery(static_cast<SignalQuery&>(_signalAccessor), psp);

        }else if(_signalAccessor.mType==SignalAccessor::Type::SETTER){
            animatedObject->obtainSignal_signalSetter(static_cast<SignalSetter&>(_signalAccessor), psp);
        }


    }else if(signalOrigin=="SETTINGS"){

        SystemSettings* settings = app->systemSettings();

        //ParsedSignalPath psp(signalNameAndValue);

        if(_signalAccessor.mType==SignalAccessor::Type::QUERY){
            settings->obtainSignal_signalQuery(static_cast<SignalQuery&>(_signalAccessor), psp);

        }else if(_signalAccessor.mType==SignalAccessor::Type::SETTER){
            settings->obtainSignal_signalSetter(static_cast<SignalSetter&>(_signalAccessor), psp);
        }


    }else if(signalOrigin.substr(0,4)=="GFX:"){

        std::string objPath = signalOrigin.substr(4);

        GfxObject* gfxObject = scene->gfxObjectsGroup()->getGfxObject(objPath);
        if(gfxObject==nullptr){
            return;
        }

        //ParsedSignalPath psp(signalNameAndValue);

        if(_signalAccessor.mType==SignalAccessor::Type::QUERY){
            gfxObject->obtainSignal_signalQuery(static_cast<SignalQuery&>(_signalAccessor), psp);

        }else if(_signalAccessor.mType==SignalAccessor::Type::SETTER){
            gfxObject->obtainSignal_signalSetter(static_cast<SignalSetter&>(_signalAccessor), psp);
        }

    }


    return;

}



//=====================================================================================


SignalParserManager::~SignalParserManager()
{

    for(SignalParser *sp : mSignalParsers){
        delete sp;
    }

}


void SignalParserManager::addAndStoreSignalParser(SignalParser *_signalParser)
{

    mSignalParsers.push_back(_signalParser);
}


void SignalParserManager::parseSignalAccessor(PlayedScene *_scene, const std::string &_path, SignalAccessor &_signalAccessor, BaseObject *obj1, BaseObject *obj2)
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


    parseSignalAccessorStyle(signalStyle, _signalAccessor);


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
        dbgSystem.addMessage("Parse signal accessor from string '" + _path +"' error! No signal parser found for keyword '" + parserKeyword + "'!");
        return;
    }


    signalParser->parseSignalAccessor(_scene, _path, _signalAccessor, obj1, obj2);

    if(_signalAccessor.mNotUsed){
        return;
    }

    if(_signalAccessor.mSignal){
        if(_signalAccessor.mSignal->id() == static_cast<unsigned char>(SignalID::CUSTOM_ENTITY_SIGNAL)){

            LogicState *currentLogicState = nullptr;

            if(obj2){
                if(obj2->baseType()==BaseObjectType::LOGIC_STATE){
                    currentLogicState = static_cast<LogicState*>(obj2);
                }
            }


            if(currentLogicState){
                currentLogicState->customUpdatedSignals().push_back(_signalAccessor.mSignal);
            }
            _signalAccessor.mOwnedSignal = true;   // Custom signals are owned by signal accessors !
        }
        return;
    }


    if(_signalAccessor.mSignal==nullptr){
        dbgSystem.addMessage("Error parsing '" + _path +"' ! No signal found for the signal accessor!");
    }

}



bool SignalParserManager::parseSignalAccessorStyle(const std::string & style, SignalAccessor &_signalAccessor)
{


    if(_signalAccessor.mType==SignalAccessor::Type::QUERY){

        SignalQuery &_signalQuerry = static_cast<SignalQuery&>(_signalAccessor);

        if(style==""){
            _signalQuerry.mStyle = SignalQuery::Style::ACTIVE;

        }else{

            //_signalQuerry.mStyle = _ParseTriggerStyle(syle);


            if(style=="START"){
                _signalQuerry.mStyle = SignalQuery::Style::ACTIVE_STARTED;

             }else if(style=="ACTIVE"){
                _signalQuerry.mStyle = SignalQuery::Style::ACTIVE;

            }else if(style=="END"){
               _signalQuerry.mStyle = SignalQuery::Style::ACTIVE_ENDED;

            }else if(style=="CHANGED"){
               _signalQuerry.mStyle = SignalQuery::Style::ACTIVE_CHANGED;

            }else if(style=="NOT_ACTIVE"){
               _signalQuerry.mStyle = SignalQuery::Style::NOT_ACTIVE;

            }else{
                dbgSystem.addMessage("Error parsing signal query style from string'" + style +"' !");
                return false;
            }
        }


    //}else if(_signalAccessor.mType==SignalAccessor::Type::SETTER){



        //SignalSetter &_signalSetter = static_cast<SignalSetter&>(_signalAccessor);
        //_signalSetter.mStyle = SignalSetter::Style::SET;

        /*
        if(style=="TRUE"){
            _signalSetter.mFlagState = true;

        }else if(style=="FALSE"){
            _signalSetter.mFlagState = false;

        }else{
            dbgSystem.addMessage("Error parsing signal setter style from string'" + style +"' !");
            return false;

        }
        */

    }


    return true;
}


//======================================================================================================


ParsedSignalPath::ParsedSignalPath(const std::string &_fullPath) : mFullPath(_fullPath)
{

    std::vector<std::string> signalParts = StdString::splitString(mFullPath, "=");

    if(signalParts.size()>0){
        mSignalFullName = signalParts[0];
        mSignalNameParts = StdString::splitString(mSignalFullName, ":");
    }

    if(signalParts.size()>1){
        mValue = signalParts[1];
    }

}


const std::string & ParsedSignalPath::signalNamePartAt(int i)
{

    if(i>=0 && i<mSignalNameParts.size()){
        return mSignalNameParts[i];
    }

    return mEmpty;

}



/*
void ParsedSignalPath::parsePath_name_value(const std::string &_path)
{

    std::vector<std::string> signalParts = StdString::splitString(_path, ":");

    if(signalParts.size()>0){
        signalName = signalParts[0];
    }
    if(signalParts.size()>1){
        valueName = signalParts[1];
    }
    if(signalParts.size()>2){
        flagName = signalParts[1];
        valueName = signalParts[2];
    }

}
*/

bool ParsedSignalPath::obtainValue(SignalQuery &_signalQuery, Signal *_signal , std::vector<NamedValue> *_valuesSet)
{

    if(_signalQuery.valueNotRequired){
        _signalQuery.mSignal = _signal;
        return true;
    }

    //if(parse_query(_signal)==false){
    //    return false;
    //}


    //---
    if(_signal->type()==SignalType::BOOL){

        if(mValue.empty() || mValue=="TRUE" || mValue=="ON" || mValue=="1"){
            _signalQuery.mBoolValue = true;

        }else if(mValue=="FALSE" || mValue=="OFF" || mValue=="0"){
            _signalQuery.mBoolValue = false;

        }else{

            dbgSystem.addMessage("Wrong value for signal '" + mSignalFullName +"' !");
            return false;
        }


    }else if(_signal->type()==SignalType::INT){

        if(mValue.empty()){
            dbgSystem.addMessage("Missing value for signal '" + mSignalFullName +"' !");
            return false;
        }


        if(_valuesSet){

            bool found = false;
            for(NamedValue &nn : *_valuesSet){
                if(nn.name==mValue){
                    _signalQuery.mIntValue = nn.intValue;
                    found = true;
                }
            }

            if(found==false){
                dbgSystem.addMessage("Value '" + mValue + "' not found in the values set for the signal '" + mSignalFullName +"' !");
                return false;
            }

        }else if(mValue=="ANY"){
            _signalQuery.mAnyValueChanged = true;

        }else{
            _signalQuery.mIntValue = StdString::stringToInt(mValue, 0);

        }


    }else if(_signal->type()==SignalType::FLOAT){

        if(mValue.empty()){
            dbgSystem.addMessage("Missing value for signal '" + mSignalFullName +"' !");
            return false;
        }

        if(mValue=="ANY"){
            _signalQuery.mAnyValueChanged = true;

        }else{
            _signalQuery.mFloatValue = StdString::stringToFloat(mValue, 0.0f);
        }


    }else if(_signal->type()==SignalType::BITSET){

        if(mSignalNameParts.size()<2){
            dbgSystem.addMessage("Missing flag name for signal '" + mSignalFullName +"' !");
            return false;
        }

        mFlagName = mSignalNameParts.back();        // last part is always flag

        //if(mFlagName.empty()){
        //    dbgSystem.addMessage("Missing flag name for signal '" + mSignalFullName +"' !");
        //    return false;
        //}



        if(_valuesSet){

            bool found = false;
            for(NamedValue &nn : *_valuesSet){
                if(nn.name==mFlagName){
                    _signalQuery.mIntValue = nn.intValue;
                    found = true;
                    break;
                }
            }

            if(found==false){
                dbgSystem.addMessage("Flag name '" + mValue + "' not found for signal '" + mSignalFullName +"' !");
                return false;
            }

        }else{
            dbgSystem.addMessage("Missing values set for signal '" + mSignalFullName +"' (required for bitflag signals) !");
            return false;
        }


        //----
        if(mValue.empty() || mValue=="TRUE" || mValue=="ON" || mValue=="1"){
            _signalQuery.mBoolValue = true;

        }else if(mValue=="FALSE" || mValue=="OFF" || mValue=="0"){
            _signalQuery.mBoolValue = false;

        }else{

            dbgSystem.addMessage("Wrong value for signal '" + mSignalFullName +"' !");
            return false;
        }


    }else if(_signal->type()==SignalType::STRING){

        if(mValue.empty()){
            dbgSystem.addMessage("Missing value for signal '" + mSignalFullName +"' !");
            return false;
        }

        if(mValue=="ANY"){
            _signalQuery.mAnyValueChanged = true;

        }else{
            _signalQuery.mStringValue = mValue;
            if(_signalQuery.mStringValue=="''"){        // KEY WORD for empty string
                _signalQuery.mStringValue.clear();
            }
        }

    }

    _signalQuery.mSignal = _signal;         // serves as flag so we set it at the end if ok

    return true;

}


bool ParsedSignalPath::obtainValue(SignalSetter &_signalSetter, Signal *_signal , std::vector<NamedValue> *_valuesSet)
{

    if(_signalSetter.valueNotRequired){
        _signalSetter.mSignal = _signal;
        return true;
    }

    if(_signalSetter.mSigData){             // value is another signal
        _signalSetter.mSignal = _signal;
        return true;
    }

    //if(parse_setter(_signal)==false){
    //    return false;
    //}

    //---
    if(_signal->type()==SignalType::BOOL){

        if(mValue.empty() || mValue=="TRUE" || mValue=="ON" || mValue=="1"){
            _signalSetter.mBoolValue = true;

        }else if(mValue=="FALSE" || mValue=="OFF" || mValue=="0"){
            _signalSetter.mBoolValue = false;

        }else{

            dbgSystem.addMessage("Wrong value for signal '" + mSignalFullName +"' !");
            return false;
        }


    }else if(_signal->type()==SignalType::INT){

        if(mValue.empty()){
            dbgSystem.addMessage("Missing value for signal '" + mSignalFullName +"' !");
            return false;
        }


        if(_valuesSet){

            bool found = false;
            for(NamedValue &nn : *_valuesSet){
                if(nn.name==mValue){
                    _signalSetter.mIntValue = nn.intValue;
                    found = true;
                }
            }

            if(found==false){
                dbgSystem.addMessage("Value '" + mValue + "' not found in the values set for the signal '" + mSignalFullName +"' !");
                return false;
            }

        }else if(mValue=="ANY"){
            _signalSetter.mAnyValueChanged = true;

        }else{
            _signalSetter.mIntValue = StdString::stringToInt(mValue, 0);

        }


    }else if(_signal->type()==SignalType::FLOAT){

        if(mValue.empty()){
            dbgSystem.addMessage("Missing value for signal '" + mSignalFullName +"' !");
            return false;
        }

        if(mValue=="ANY"){
            _signalSetter.mAnyValueChanged = true;

        }else{
            _signalSetter.mFloatValue = StdString::stringToFloat(mValue, 0.0f);
        }


    }else if(_signal->type()==SignalType::BITSET){

        /*
        if(mFlagName.empty()){
            dbgSystem.addMessage("Missing flag name for signal '" + mSignalFullName +"' !");
            return false;
        }
        if(mValue.empty()){
            dbgSystem.addMessage("Missing value for signal '" + mSignalFullName +"' !");
            return false;
        }
        */
        if(mSignalNameParts.size()<2){
            dbgSystem.addMessage("Missing flag name for signal '" + mSignalFullName +"' !");
            return false;
        }

        mFlagName = mSignalNameParts.back();        // last part is always flag



        if(_valuesSet){

            bool found = false;
            for(NamedValue &nn : *_valuesSet){
                if(nn.name==mFlagName){
                    _signalSetter.mIntValue = nn.intValue;
                    found = true;
                }
            }

            if(found==false){
                dbgSystem.addMessage("Flag name '" + mValue + "' not found for signal '" + mSignalFullName +"' !");
                return false;
            }

        }else{
            dbgSystem.addMessage("Missing values set for signal '" + mSignalFullName +"' (required for bitflag signals) !");
            return false;
        }


        //----
        if(mValue.empty() || mValue=="TRUE" || mValue=="ON" || mValue=="1"){
            _signalSetter.mBoolValue = true;

        }else if(mValue=="FALSE" || mValue=="OFF" || mValue=="0"){
            _signalSetter.mBoolValue = false;

        }else{

            dbgSystem.addMessage("Wrong value for signal '" + mSignalFullName +"' !");
            return false;
        }


    }else if(_signal->type()==SignalType::STRING){

        if(mValue.empty()){
            dbgSystem.addMessage("Missing value for signal '" + mSignalFullName +"' !");
            return false;
        }

        if(mValue=="ANY"){
            _signalSetter.mAnyValueChanged = true;

        }else{
            _signalSetter.mStringValue = mValue;
        }

    }


    _signalSetter.mSignal = _signal;         // serves as flag so we set it at the end if ok

    return true;

}


/*
bool ParsedSignalPath::parse_query(Signal* _signal)
{

    std::vector<std::string> signalParts = StdString::splitString(mFullPath, "=");

    if(signalParts.size()>0){
        mSignalFullName = signalParts[0];
    }


    if(_signal->type()==SignalType::BOOL){

        if(signalParts.size()>1){
            mValue = signalParts[1];
        }
        if(signalParts.size()>2){
            dbgSystem.addMessage("Boolean signal syntax error!");
            return false;
        }


    }else if(_signal->type()==SignalType::INT || _signal->type()==SignalType::FLOAT || _signal->type()==SignalType::STRING){

        if(signalParts.size()>1){
            mValue = signalParts[1];
        }
        if(mValue.empty()){
            dbgSystem.addMessage("Missing value for signal '" + mSignalFullName +"' !");
            return false;
        }
        if(signalParts.size()>2){
            dbgSystem.addMessage("Signal syntax error!");
            return false;
        }


    }else if(_signal->type()==SignalType::BITSET){

        if(signalParts.size()>1){
            mFlagName = signalParts[1];
        }

        if(mFlagName.empty()){
            dbgSystem.addMessage("Missing flag name for signal '" + mSignalFullName +"' !");
            return false;
        }

        if(signalParts.size()>2){
            mValue = signalParts[2];
        }
        //if(valueName.empty()){
        //    dbgSystem.addMessage("Missing value for signal '" + signalName +"' !");
        //    return false;
        //}
        if(signalParts.size()>3){
            dbgSystem.addMessage("Bitflags signal syntax error!");
            return false;
        }
    }


    return true;

}


bool ParsedSignalPath::parse_setter(Signal *_signal)
{

    return parse_query(_signal);

}
*/



}
