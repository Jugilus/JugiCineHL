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
#include "jpObjectParser.h"


namespace jugimap{



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

        if(_signalAccessor.type()==SignalAccessorType::QUERY){
            psp.obtainValue(static_cast<SignalQuery&>(_signalAccessor), stateSignal);

        }else if(_signalAccessor.type()==SignalAccessorType::SETTER){
            dbgSystem.addMessage("Setting state signals is not available! Use 'doSetState' command!");
        }


    }else if(signalOrigin=="USERS"){

        if(psp.signalNamePartAt(0)=="ACTIVE_USER"){

            //ParsedSignalPath psp(signalNameAndValue);

            if(_signalAccessor.type()==SignalAccessorType::QUERY){
                psp.obtainValue(static_cast<SignalQuery&>(_signalAccessor), app->usersDatabase()->activeUserSignal());

            }else if(_signalAccessor.type()==SignalAccessorType::SETTER){
                _signalAccessor._setSignal(app->usersDatabase()->activeUserSignal());
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

            if(_signalAccessor.type()==SignalAccessorType::QUERY){
                psp.obtainValue(static_cast<SignalQuery&>(_signalAccessor), key);

            }else if(_signalAccessor.type()==SignalAccessorType::SETTER){
                dbgSystem.addMessage("Setting 'keyboard' signals is not possible!");
            }

        }else if(inputSource=="COMMAND"){

            //ParsedSignalPath psp(signalNameAndValue);

            BoolSignal *command = app->inputSystem()->getGameInputCommand(psp.signalFullName());
            if(command==nullptr){
                return;
            }

            if(_signalAccessor.type()==SignalAccessorType::QUERY){
                psp.obtainValue(static_cast<SignalQuery&>(_signalAccessor), command);

            }else if(_signalAccessor.type()==SignalAccessorType::SETTER){
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

        if(_signalAccessor.type()==SignalAccessorType::QUERY){
            animatedObject->obtainSignal_signalQuery(static_cast<SignalQuery&>(_signalAccessor), psp);

        }else if(_signalAccessor.type()==SignalAccessorType::SETTER){
            animatedObject->obtainSignal_signalSetter(static_cast<SignalSetter&>(_signalAccessor), psp);
        }


    }else if(signalOrigin=="SETTINGS"){

        SystemSettings* settings = app->systemSettings();

        //ParsedSignalPath psp(signalNameAndValue);

        if(_signalAccessor.type()==SignalAccessorType::QUERY){
            settings->obtainSignal_signalQuery(static_cast<SignalQuery&>(_signalAccessor), psp);

        }else if(_signalAccessor.type()==SignalAccessorType::SETTER){
            settings->obtainSignal_signalSetter(static_cast<SignalSetter&>(_signalAccessor), psp);
        }


    }else if(signalOrigin.substr(0,4)=="GFX:"){

        std::string objPath = signalOrigin.substr(4);

        //GfxObject* gfxObject = scene->gfxObjectsGroup()->getGfxObject(objPath);
        GfxObject* gfxObject = scene->gfxObjectStorage().getObject(objPath);
        if(gfxObject==nullptr){
            return;
        }

        //ParsedSignalPath psp(signalNameAndValue);

        if(_signalAccessor.type()==SignalAccessorType::QUERY){
            gfxObject->obtainSignal_signalQuery(static_cast<SignalQuery&>(_signalAccessor), psp);

        }else if(_signalAccessor.type()==SignalAccessorType::SETTER){
            gfxObject->obtainSignal_signalSetter(static_cast<SignalSetter&>(_signalAccessor), psp);
        }

    }

}


bool CoreSignalsParser::parseSignalAccessorNEW(PlayedScene *scene, SignalParsingInfo &_spi, const std::string &_signalValue)
{


    LogicState *currentLogicState = _spi.logicState;


    //ParsedSignalPath psp(_spi.signalName);

    //-----
    if(_spi.originObjectPath=="STATES"){

        if(currentLogicState==nullptr){
            dbgSystem.addMessage("Logic state is not available! You can access logic states only from the logic actions!");
            return false;
        }

        //ParsedSignalPath psp(signalNameAndValue);

        BoolSignal *stateSignal = nullptr;

        if(_spi.signalIdentifierPath=="THIS"){
            stateSignal = &(currentLogicState->stateSignal());

        }else{
            LogicState* state = ObtainLogicStateFromPath2(scene, currentLogicState, _spi.signalIdentifierPath);      // !

            if(state==nullptr){
                return false;
            }
            stateSignal = &(state->stateSignal());
        }

        if(_spi.signalAccessorType==SignalAccessorType::QUERY){
            //psp.obtainValue(static_cast<SignalQuery&>(_signalAccessor), stateSignal);

        }else if(_spi.signalAccessorType==SignalAccessorType::SETTER){
            dbgSystem.addMessage("Setting state signals is not available! Use 'doSetState' command!");
            return false;
        }

        _spi.signal = stateSignal;


    }else if(_spi.originObjectPath=="USERS"){

        if(_spi.signalIdentifierPath=="ACTIVE_USER"){

            _spi.signal = app->usersDatabase()->activeUserSignal();

            /*
            if(_signalAccessor.type()==SignalAccessorType::QUERY){
                psp.obtainValue(static_cast<SignalQuery&>(_signalAccessor), app->usersDatabase()->activeUserSignal());

            }else if(_signalAccessor.type()==SignalAccessorType::SETTER){
                _signalAccessor._setSignal(app->usersDatabase()->activeUserSignal());
            }
            */
        }else{
            return false;
        }


    }else if(_spi.originObjectPath.substr(0,6)=="INPUT:"){
        std::string inputSource = _spi.originObjectPath.substr(6);

        if(inputSource=="KEYBOARD"){

            _spi.signal = keyboard.keySignalForKeyName(_spi.signalIdentifierPath);
            if(_spi.signal==nullptr){
                return false;
            }

            if(_spi.signalAccessorType==SignalAccessorType::QUERY){
                //psp.obtainValue(static_cast<SignalQuery&>(_signalAccessor), key);

            }else if(_spi.signalAccessorType==SignalAccessorType::SETTER){
                dbgSystem.addMessage("Setting 'keyboard' signals is not possible!");
                return false;
            }

        }else if(inputSource=="COMMAND"){

            //ParsedSignalPath psp(signalNameAndValue);

            _spi.signal = app->inputSystem()->getGameInputCommand(_spi.signalIdentifierPath);
            if(_spi.signal==nullptr){
                return false;
            }

            if(_spi.signalAccessorType==SignalAccessorType::QUERY){
                //psp.obtainValue(static_cast<SignalQuery&>(_signalAccessor), command);

            }else if(_spi.signalAccessorType==SignalAccessorType::SETTER){
                dbgSystem.addMessage("Setting 'command' signals is not possible!");
                return false;
            }

        }


    }else if(_spi.originObjectPath.substr(0,16)=="ANIMATED_OBJECT:"){

        std::string aniObjectName = _spi.originObjectPath.substr(16);

        AnimationObject *animatedObject = scene->animationManager()->getAnimationObject(aniObjectName);
        if(animatedObject==nullptr){
            return false;
        }

        if(animatedObject->obtainSignal(_spi)==false){
            return false;
        }

        /*
        if(_spi.signalAccessorType==SignalAccessorType::QUERY){
            animatedObject->obtainSignal_signalQuery(static_cast<SignalQuery&>(_signalAccessor), psp);


        }else if(_spi.signalAccessorType==SignalAccessorType::SETTER){
            animatedObject->obtainSignal_signalSetter(static_cast<SignalSetter&>(_signalAccessor), psp);
        }
        */


    }else if(_spi.originObjectPath=="SETTINGS"){

        SystemSettings* settings = app->systemSettings();

        assert(false);

        // TO DO

        if(_spi.signalAccessorType==SignalAccessorType::QUERY){
            //settings->obtainSignal_signalQuery(static_cast<SignalQuery&>(_signalAccessor), psp);

        }else if(_spi.signalAccessorType==SignalAccessorType::SETTER){
            //settings->obtainSignal_signalSetter(static_cast<SignalSetter&>(_signalAccessor), psp);
        }


    }else if(_spi.originObjectPath.substr(0,4)=="GFX:"){

        std::string objPath = _spi.originObjectPath.substr(4);

        //GfxObject* gfxObject = scene->gfxObjectsGroup()->getGfxObject(objPath);
        GfxObject* gfxObject = scene->gfxObjectStorage().getObject(objPath);
        if(gfxObject==nullptr){
            return false;
        }

        assert(false);

        // TO DO

        if(_spi.signalAccessorType==SignalAccessorType::QUERY){
            //gfxObject->obtainSignal_signalQuery(static_cast<SignalQuery&>(_signalAccessor), psp);

        }else if(_spi.signalAccessorType==SignalAccessorType::SETTER){
            //gfxObject->obtainSignal_signalSetter(static_cast<SignalSetter&>(_signalAccessor), psp);
        }

    }


    _spi.signalType = _spi.signal->type();
    _spi.signalID = _spi.signal->id();

    return true;

}


void CoreSignalsParser::parseItemDataPair(PlayedScene *scene, const std::string &_path, ItemDataPair &_itemDataPair, ParseItemMode _parseItemMode, BaseObject *obj1, BaseObject *obj2)
{

    dbgSystem.addMessage("CoreSignalsParser does not have implemented parser!");

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

    /*
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


    */


    SignalParser *signalParser = obtainSignalParser(parserKeyword);
    if(signalParser==nullptr){
        dbgSystem.addMessage("Parse signal accessor from string '" + _path +"' error! No signal parser found for keyword '" + parserKeyword + "'!");
        return;
    }


    signalParser->parseSignalAccessor(_scene, _path, _signalAccessor, obj1, obj2);


    if(_signalAccessor.notUsed()){
        return;
    }

    if(_signalAccessor.signal()){
        if(_signalAccessor.signal()->id() == static_cast<unsigned char>(SignalID::UPDATED_ON_SIGNAL_QUERY)){

            LogicState *currentLogicState = nullptr;

            if(obj2){
                if(obj2->baseType()==BaseObjectType::LOGIC_STATE){
                    currentLogicState = static_cast<LogicState*>(obj2);
                }
            }


            if(currentLogicState){
                currentLogicState->customUpdatedSignals().push_back(_signalAccessor.signal());
            }
            _signalAccessor._setOwnedSignal(true);   // Custom signals are owned by signal accessors !
        }
        return;
    }


    if(_signalAccessor.signal()==nullptr){
        dbgSystem.addMessage("Error parsing '" + _path +"' ! No signal found for the signal accessor!");
    }

}


/*
void SignalParserManager::parseSignalAccessorNEW(PlayedScene *_scene, const std::string &_signalPath, const std::string &_signalValue, SignalAccessor &_signalAccessor, SignalParsingInfo &_signalParsingInfo, const ActionOriginObjects &_originObjects)
{

    std::vector<std::string>pathParts = StdString::splitString(_signalPath, "/");

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
        dbgSystem.addMessage("Parse signal query from string '" + _signalPath +"' error! Signal origin is empty string!");
        return;
    }


    if(parseSignalAccessorStyle(signalStyle, _signalAccessor)==false){
        return;
    }


    std::string parserKeyword = signalOrigin;




    SignalParser *signalParser = obtainSignalParser(parserKeyword);
    if(signalParser==nullptr){
        dbgSystem.addMessage("Parse signal accessor from string '" + _signalPath +"' error! No signal parser found for keyword '" + parserKeyword + "'!");
        return;
    }


    signalParser->parseSignalAccessorNEW(_scene, _signalPath, _signalValue, _signalAccessor, _signalParsingInfo, _originObjects);


    if(_signalAccessor.notUsed()){
        return;
    }


    //if(_signalAccessor.signal()==nullptr){
    //    dbgSystem.addMessage("Error parsing '" + _path +"' ! No signal found for the signal accessor!");
    //}

}
*/


void SignalParserManager::parseItemDataPair(PlayedScene *_scene, const std::string &_path, ItemDataPair &_itemDataPair, ParseItemMode _parseItemMode, BaseObject *obj1, BaseObject *obj2)
{

    std::string parserKeyword;

    size_t index = _path.find_first_of("/");
    if(index != std::string::npos){
        parserKeyword = _path.substr(0,index);
    }else{
        parserKeyword = _path;
    }


    SignalParser *signalParser = obtainSignalParser(parserKeyword);
    if(signalParser==nullptr){
        dbgSystem.addMessage("Parse signal accessor from string '" + _path +"' error! No signal parser found for keyword '" + parserKeyword + "'!");
        return;
    }


    signalParser->parseItemDataPair(_scene, _path, _itemDataPair, _parseItemMode, obj1, obj2);


    if(_parseItemMode==ParseItemMode::ITEM && _itemDataPair.item==nullptr){
        dbgSystem.addMessage("Error parsing '" + _path +"' ! The item not found!");

    }else if(_parseItemMode==ParseItemMode::DATA && _itemDataPair.data==nullptr){
        dbgSystem.addMessage("Error parsing '" + _path +"' ! The data not found!");
    }

}


/*
void SignalParserManager::parseObjectName(PlayedScene *_scene, const std::string &_path, std::string &_objectName, BaseObject* obj1, BaseObject *obj2)
{

    std::string parserKeyword;

    size_t index = _path.find_first_of("/");
    if(index != std::string::npos){
        parserKeyword = _path.substr(0,index);
    }else{
        parserKeyword = _path;
    }


    SignalParser *signalParser = obtainSignalParser(parserKeyword);
    if(signalParser==nullptr){
        dbgSystem.addMessage("Parse signal accessor from string '" + _path +"' error! No signal parser found for keyword '" + parserKeyword + "'!");
        return;
    }


    signalParser->parseObjectName(_scene, _path, _objectName, obj1, obj2);


    if(_objectName.empty()){
        dbgSystem.addMessage("Error parsing '" + _path +"' ! The name not found!");

    }

}

*/


bool SignalParserManager::parseSignalAccessorNEW(PlayedScene *_scene, SignalParsingInfo &_spi, const std::string &_path, const std::string &_signalValue)
{


    if(_spi.parseMainPaths(_path)==false){
        return false;
    }

    SignalParser *parser = obtainObjectParserNEW(_spi.originObjectPath);
    if(parser==nullptr){
        return false;
    }

    if(parser->parseSignalAccessorNEW(_scene, _spi, _signalValue)==false){
        return false;
    }

    return true;

}


bool SignalParserManager::parseObject(PlayedScene *_scene, SignalParsingInfo &_spi, const std::string &_path)
{

    if(_spi.parseMainPaths(_path)==false){
        return false;
    }

    SignalParser *parser = obtainObjectParserNEW(_spi.originObjectPath);
    if(parser==nullptr){
        return false;
    }

    if(parser->parseObject(_scene, _spi)==false){
        return false;
    }

    return true;

}


bool SignalParserManager::parseItemDataPairNEW(PlayedScene *_scene, ItemDataParsingHelper &_dpi, const std::string &_path)
{

    if(_dpi.parseMainPaths(_path)==false){
        return false;
    }

    SignalParser *parser = obtainObjectParserNEW(_dpi.originObjectPath);
    if(parser==nullptr){
        return false;
    }

    if(parser->parseItemDataPairNEW(_scene, _dpi)==false){
        return false;
    }

    return true;

}



SignalParser* SignalParserManager::obtainObjectParserNEW(const std::string &_path, bool _setErrorMessage)
{

    std::string parserKeyword = _path;

    size_t index = _path.find_first_of("/");
    if(index != std::string::npos){
        parserKeyword = _path.substr(0,index);
    }

    index = _path.find_first_of(":");
    if(index != std::string::npos){
        parserKeyword = parserKeyword.substr(0,index);
    }


    SignalParser *signalParser = nullptr;

    for(SignalParser *sp : mSignalParsers){
        if(StdVector::contains(sp->keyWords(), parserKeyword)){
            signalParser = sp;
            break;
        }
    }


    if(signalParser==nullptr && _setErrorMessage){
        dbgSystem.addMessage("Object parser not found for path '" +_path + "' (parser keyword '" + parserKeyword + "')!");
    }

    return signalParser;

}



SignalParser* SignalParserManager::obtainSignalParser(std::string parserKeyword)
{

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

    return signalParser;

}


bool SignalParserManager::parseSignalAccessorStyle(const std::string & style, SignalAccessor &_signalAccessor)
{


    if(_signalAccessor.type()==SignalAccessorType::QUERY){

        SignalQuery &_signalQuerry = static_cast<SignalQuery&>(_signalAccessor);

        if(_signalQuerry.parseStyle(style)==false){
            return false;
        }

        /*
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
        */


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

/*
bool SignalParserManager::obtainRootItems(PlayedScene *_scene, std::vector<std::string> &_rootItemsPath, std::vector<Item *> &_rootItems, BaseObject *_obj1, BaseObject *_obj2)
{

    for(const std::string &s : _rootItemsPath){
        std::string parserKeyword = s;

        SignalParser *signalParser = obtainSignalParser(parserKeyword);
        if(signalParser==nullptr){
            dbgSystem.addMessage("Obtain root items error! No signal parser found for keyword '" + parserKeyword + "'!");
            return false;
        }

        Item *rootItem = signalParser->obtainRootItem(_scene, s, _obj1, _obj2);
        if(rootItem==nullptr){
            return false;
        }

        _rootItems.push_back(rootItem);
    }

    return true;

}
*/

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




bool ParsedSignalPath::obtainValue(SignalAccessor &_signalAccessor, Signal *_signal)
{

    if(_signalAccessor.mRightSide.mValueNotRequired){
        _signalAccessor.mSignal = _signal;
        return true;
    }

    //if(parse_query(_signal)==false){
    //    return false;
    //}

    if(_signalAccessor.mRightSide.mSigData){             // value is another signal
        _signalAccessor.mSignal = _signal;
        return true;
    }


    //---
    if(_signal->type()==SignalType::BOOL){

        if(mValue.empty() || mValue=="TRUE" || mValue=="ON" || mValue=="1"){
            _signalAccessor.mRightSide.mBoolValue = true;

        }else if(mValue=="FALSE" || mValue=="OFF" || mValue=="0"){
            _signalAccessor.mRightSide.mBoolValue = false;

        }else{

            dbgSystem.addMessage("Wrong value for signal '" + mSignalFullName +"' !");
            return false;
        }


    }else if(_signal->type()==SignalType::INT){

        if(mValue.empty()){
            dbgSystem.addMessage("Missing value for signal '" + mSignalFullName +"' !");
            return false;
        }

        IntSignalExtraData * extraData = static_cast<IntSignal*>(_signal)->extraData();

        if(extraData && extraData->namedValues){

            bool found = false;
            for(auto &nameValue : *extraData->namedValues){
                if(nameValue.first==mValue){
                    _signalAccessor.mRightSide.mIntValue = nameValue.second;
                    found = true;
                    break;
                }
            }

            if(found==false){
                dbgSystem.addMessage("Value '" + mValue + "' not found in the values set for the signal '" + mSignalFullName +"' !");
                return false;
            }

        }else if(mValue=="ANY"){
            _signalAccessor.mRightSide.mAnyValueChanged = true;

        }else{
            _signalAccessor.mRightSide.mIntValue = StdString::stringToInt(mValue, 0);

        }


    }else if(_signal->type()==SignalType::FLOAT){

        if(mValue.empty()){
            dbgSystem.addMessage("Missing value for signal '" + mSignalFullName +"' !");
            return false;
        }


        FloatSignalExtraData * extraData = static_cast<FloatSignal*>(_signal)->extraData();

        if(extraData && extraData->namedValues){

            bool found = false;
            for(auto &nameValue : *extraData->namedValues){
                if(nameValue.first==mValue){
                    _signalAccessor.mRightSide.mFloatValue = nameValue.second;
                    found = true;
                    break;
                }
            }

            if(found==false){
                dbgSystem.addMessage("Value '" + mValue + "' not found in the values set for the signal '" + mSignalFullName +"' !");
                return false;
            }

        }else if(mValue=="ANY"){
            _signalAccessor.mRightSide.mAnyValueChanged = true;

        }else{
            _signalAccessor.mRightSide.mFloatValue = StdString::stringToFloat(mValue, 0.0f);
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

        BitflagsSignalExtraData * extraData = static_cast<BitsetSignal*>(_signal)->extraData();

        if(extraData && extraData->namedValues){

            bool found = false;
            for(auto &nn : *extraData->namedValues){
                if(nn.first==mFlagName){
                    _signalAccessor.mRightSide.mIntValue = nn.second;
                    found = true;
                    break;
                }
            }

            if(found==false){
                dbgSystem.addMessage("Flag name '" + mValue + "' not found for signal '" + mSignalFullName +"' !");
                return false;
            }

        //}else{
        //    dbgSystem.addMessage("Missing values set for signal '" + mSignalFullName +"' (required for bitflag signals) !");
       //     return false;

        }


        if(mValue.empty() || mValue=="TRUE" || mValue=="ON" || mValue=="1"){
            _signalAccessor.mRightSide.mBoolValue = true;

        }else if(mValue=="FALSE" || mValue=="OFF" || mValue=="0"){
            _signalAccessor.mRightSide.mBoolValue = false;

        }else{

            dbgSystem.addMessage("Wrong value for signal '" + mSignalFullName +"' !");
            return false;
        }


    }else if(_signal->type()==SignalType::STRING){

        if(mValue.empty()){
            dbgSystem.addMessage("Missing value for signal '" + mSignalFullName +"' !");
            return false;
        }

        StringSignalExtraData * extraData = static_cast<StringSignal*>(_signal)->extraData();

        if(extraData && extraData->namedValues){

            bool found = false;
            for(auto &nameValue : *extraData->namedValues){
                if(nameValue.first==mValue){
                    _signalAccessor.mRightSide.mStringValue = nameValue.second;
                    found = true;
                    break;
                }
            }

            if(found==false){
                dbgSystem.addMessage("Value '" + mValue + "' not found in the values set for the signal '" + mSignalFullName +"' !");
                return false;
            }

        }else if(mValue=="ANY"){
            _signalAccessor.mRightSide.mAnyValueChanged = true;

        }else{
            _signalAccessor.mRightSide.mStringValue = mValue;
            if(_signalAccessor.mRightSide.mStringValue=="''"){        // KEY WORD for empty string
                _signalAccessor.mRightSide.mStringValue.clear();
            }
        }

    }

    _signalAccessor.mSignal = _signal;         // serves as flag so we set it at the end if ok

    return true;


}



/*

bool ParsedSignalPath::obtainValue(SignalQuery &_signalQuery, Signal *_signal )
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

        IntSignalExtraData * extraData = dynamic_cast<IntSignalExtraData*>(_signal->extraData());

        if(extraData && extraData->namedValues){

            bool found = false;
            for(auto &nameValue : *extraData->namedValues){
                if(nameValue.first==mValue){
                    _signalQuery.mIntValue = nameValue.second;
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


        FloatSignalExtraData * extraData = dynamic_cast<FloatSignalExtraData*>(_signal->extraData());

        if(extraData && extraData->namedValues){

            bool found = false;
            for(auto &nameValue : *extraData->namedValues){
                if(nameValue.first==mValue){
                    _signalQuery.mFloatValue = nameValue.second;
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

        BitflagsSignalExtraData * extraData = dynamic_cast<BitflagsSignalExtraData*>(_signal->extraData());

        if(extraData && extraData->namedValues){

            bool found = false;
            for(auto &nn : *extraData->namedValues){
                if(nn.first==mFlagName){
                    _signalQuery.mIntValue = nn.second;
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

        StringSignalExtraData * extraData = dynamic_cast<StringSignalExtraData*>(_signal->extraData());

        if(extraData && extraData->namedValues){

            bool found = false;
            for(auto &nameValue : *extraData->namedValues){
                if(nameValue.first==mValue){
                    _signalQuery.mStringValue = nameValue.second;
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
            _signalQuery.mStringValue = mValue;
            if(_signalQuery.mStringValue=="''"){        // KEY WORD for empty string
                _signalQuery.mStringValue.clear();
            }
        }

    }

    _signalQuery.mSignal = _signal;         // serves as flag so we set it at the end if ok

    return true;

}


bool ParsedSignalPath::obtainValue(SignalSetter &_signalSetter, Signal *_signal , std::vector<NamedIntValue> *_valuesSet)
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
            for(NamedIntValue &nn : *_valuesSet){
                if(nn.name==mValue){
                    _signalSetter.mIntValue = nn.value;
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


        if(mSignalNameParts.size()<2){
            dbgSystem.addMessage("Missing flag name for signal '" + mSignalFullName +"' !");
            return false;
        }

        mFlagName = mSignalNameParts.back();        // last part is always flag



        if(_valuesSet){

            bool found = false;
            for(NamedIntValue &nn : *_valuesSet){
                if(nn.name==mFlagName){
                    _signalSetter.mIntValue = nn.value;
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
*/


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
