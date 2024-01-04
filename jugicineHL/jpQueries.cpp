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
#include "jpObjectParser.h"
#include "jpQueries.h"


namespace jugimap{




SignalAccessorRightSide::SignalAccessorRightSide(const SignalAccessorRightSide &_src)
{

    if(_src.mObjectObtainer){
        mObjectObtainer = _src.mObjectObtainer->copy();
    }
    mIntValue = _src.mIntValue;
    mFloatValue = _src.mFloatValue;
    mBoolValue = _src.mBoolValue;
    mStringValue = _src.mStringValue;
    mAnyValueChanged = _src.mAnyValueChanged;
    mValueNotRequired = _src.mValueNotRequired;

    mSigData = _src.mSigData;
    mSigDataFlag = _src.mSigDataFlag;
}


SignalAccessorRightSide& SignalAccessorRightSide::operator=(const SignalAccessorRightSide &_src)
{
    if(&_src==this){
        return *this;
    }

    if(_src.mObjectObtainer){
        mObjectObtainer = _src.mObjectObtainer->copy();
    }
    mIntValue = _src.mIntValue;
    mFloatValue = _src.mFloatValue;
    mBoolValue = _src.mBoolValue;
    mStringValue = _src.mStringValue;
    mAnyValueChanged = _src.mAnyValueChanged;
    mValueNotRequired = _src.mValueNotRequired;

    mSigData = _src.mSigData;
    mSigDataFlag = _src.mSigDataFlag;

    return *this;

}


SignalAccessorRightSide::~SignalAccessorRightSide()
{
    if(mObjectObtainer){
        delete mObjectObtainer;
    }
}


SignalAccessor::SignalAccessor(SignalAccessorType _type) : mType(_type){}


SignalAccessor::SignalAccessor(const SignalAccessor &_src)
{

    mType = _src.mType;
    mSignal = _src.mSignal;

    if(_src.mSignalOriginObtainer){
        mSignalOriginObtainer = _src.mSignalOriginObtainer->copy();
    }
    if(_src.mSignalObtainer){
        mSignalObtainer = _src.mSignalObtainer->copy();
    }


    mSignalIndex = _src.mSignalIndex;

    mRightSide = _src.mRightSide;

    mOwnedSignal = _src.mOwnedSignal;
    mNotUsed = _src.mNotUsed;

}


SignalAccessor::~SignalAccessor()
{

    if(mSignalOriginObtainer){
        delete mSignalOriginObtainer;
    }
    if(mSignalObtainer){
        delete mSignalObtainer;
    }

    if(mOwnedSignal){
        delete mSignal;
    }

}


bool SignalAccessor::parseBoolSignal(PlayedScene *_scene, const std::string & _leftSignalPath, bool _rightSideValue, BaseObject *_rootParentObject)
{

    dbgSystem.addMessage("Parsing signal '" + _leftSignalPath +"' ..." );


    SignalParsingInfo spi;
    spi.rootParentObject = _rootParentObject;
    spi.signalAccessorType = mType;


    //---- LEFT SIDE
    if(app->signalParserManager()->parseSignalAccessorNEW(_scene, spi, _leftSignalPath, "")==false){
        return false;
    }
    parseStyle(spi.signalStylePath);


    mSignal = spi.signal;
    mSignalIndex = spi.signalIndex;
    mSignalOriginObtainer = spi.originObjectObtainer;
    mSignalObtainer = spi.signalObtainer;
    if(spi.signalType==SignalType::BITSET){
        assert(spi.bitFlagValue!=0);
        mRightSide.mIntValue = spi.bitFlagValue;
    }
    mNotUsed = spi.accessorNotUsed;


    //---- RIGHT SIDE
    mRightSide.mBoolValue = _rightSideValue;


    dbgSystem.removeLastMessage();
    return true;

}


bool SignalAccessor::parse(PlayedScene *_scene, const std::string &_signalFullPath, BaseObject *_rootParentObject, LogicState *_logicState)
{

    dbgSystem.addMessage("Parsing signal '" + _signalFullPath +"' ..." );

    if(_signalFullPath=="ENTITY:CONTACTED:iGamepad/ITEM_PICKABLE=TRUE"){
        DummyFunction();
    }


    SignalParsingInfo spi;
    spi.rootParentObject = _rootParentObject;
    spi.logicState = _logicState;
    spi.signalAccessorType = mType;

    //---
    std::vector<std::string>parts = StdString::splitString(_signalFullPath, "=");
    if(mType==SignalAccessorType::QUERY && !(parts.size()==1 || parts.size()==2)){
        dbgSystem.addMessage("Error parsing string '" + _signalFullPath + "'!");
        return false;
    }
    if(mType==SignalAccessorType::SETTER &&  parts.size()!=2){
        dbgSystem.addMessage("Error parsing string '" + _signalFullPath + "'!");
        return false;
    }

    const std::string & leftSide = parts[0];
    std::string rightSide;
    if(parts.size()==2){
        rightSide = parts[1];
    }

    bool rightSideIsSignal = false;
    if(rightSide.size()>7 && rightSide.substr(0,7)=="SIGNAL:"){
        rightSide = rightSide.substr(7);
        rightSideIsSignal = true;
    }


    //---- LEFT SIDE
    if(app->signalParserManager()->parseSignalAccessorNEW(_scene, spi, leftSide, rightSide)==false){
        return false;
    }
    parseStyle(spi.signalStylePath);


    mSignal = spi.signal;
    mSignalIndex = spi.signalIndex;
    mSignalOriginObtainer = spi.originObjectObtainer;
    mSignalObtainer = spi.signalObtainer;
    if(spi.signalType==SignalType::BITSET){
        assert(spi.bitFlagValue!=0);
        mRightSide.mIntValue = spi.bitFlagValue;
    }
    mNotUsed = spi.accessorNotUsed;


    //---- RIGHT SIDE
    if(mNotUsed==false){
        if(rightSideIsSignal){

            SignalParsingInfo spiRightSide;
            spiRightSide.rootParentObject = _rootParentObject;
            spiRightSide.logicState = _logicState;
            spiRightSide.signalAccessorType = mType;

            if(app->signalParserManager()->parseSignalAccessorNEW(_scene, spiRightSide, rightSide, "")==false){
                return false;
            }
            if(spiRightSide.signal==nullptr){
                dbgSystem.addMessage("Signal parsing error! Signal on the right side must be static (known at initialization)!");
            }
            if(spiRightSide.signalType!=spi.signalType){
                dbgSystem.addMessage("Signal on the right side is of the different type than on the left side!");
            }

            mRightSide.mSigData = spiRightSide.signal;
            mRightSide.mSigDataFlag = spiRightSide.bitFlagValue;

        }else if(spi.signalType==SignalType::BASE_OBJECT){

            SignalParsingInfo spiRightSide;
            spiRightSide.rootParentObject = _rootParentObject;
            spiRightSide.logicState = _logicState;
            spiRightSide.signalAccessorType = mType;


            if(app->signalParserManager()->parseObject(_scene, spiRightSide, rightSide)==false){
                return false;
            }

            mRightSide.mObject = spiRightSide.originObject;
            mRightSide.mObjectObtainer = spiRightSide.originObjectObtainer;


        }else{

            if(parseDirectSignalValue(rightSide, spi, leftSide)==false){
                return false;
            }
        }
    }



    dbgSystem.removeLastMessage();
    return true;

}


bool SignalAccessor::parseDirectSignalValue(const std::string &_signalValue, SignalParsingInfo &_spi, const std::string & _signalPath)
{

    //---
    if(_spi.signalType==SignalType::BOOL || _spi.signalType==SignalType::BITSET){

        if(_signalValue.empty() || _signalValue=="TRUE" || _signalValue=="ON" || _signalValue=="1"){
            mRightSide.mBoolValue = true;

        }else if(_signalValue=="FALSE" || _signalValue=="OFF" || _signalValue=="0"){
            mRightSide.mBoolValue = false;

        }else{

            dbgSystem.addMessage("Wrong value '" + _signalValue +" for signal with path '" +_signalPath + "'!");
            return false;
        }


    }else if(_spi.signalType==SignalType::INT){

        if(_signalValue.empty()){
            dbgSystem.addMessage("Missing value for integer signal with path '" +_signalPath + "'!");
            return false;
        }

        if(_spi.predefiendValueDefined){
            mRightSide.mIntValue = _spi.predefinedIntValue;

        }else if(_signalValue=="ANY"){
            mRightSide.mAnyValueChanged = true;

        }else{
            if(StdString::integerNumber(_signalValue, mRightSide.mIntValue)==false){
                return false;
            }
        }


    }else if(_spi.signalType==SignalType::FLOAT){

        if(_signalValue.empty()){
            dbgSystem.addMessage("Missing value for float signal with path '" +_signalPath + "'!");
            return false;
        }

        if(_spi.predefiendValueDefined){
            mRightSide.mFloatValue = _spi.predefinedFloatValue;

        }else if(_signalValue=="ANY"){
            mRightSide.mAnyValueChanged = true;

        }else{
            if(StdString::floatNumber(_signalValue, mRightSide.mFloatValue)==false){
                return false;
            }
        }


    }else if(_spi.signalType==SignalType::STRING){

        if(_signalValue.empty()){
            dbgSystem.addMessage("Missing value for signal '" + _signalPath +"' !");
            return false;
        }

        if(_spi.predefiendValueDefined){
            mRightSide.mStringValue = _spi.predefinedStringValue;

        }else if(_signalValue=="ANY"){
            mRightSide.mAnyValueChanged = true;

        }else{
            mRightSide.mStringValue = _signalValue;
        }

        if(mRightSide.mStringValue=="''"){        // KEY WORD for empty string
            mRightSide.mStringValue.clear();
        }

    }else if(_spi.signalType==SignalType::UNKNOWN){
        assert(false);

    }

    return true;
}


//==================================================================================


bool SignalQuery::parseStyle(const std::string &_style, bool _setErrorMessage)
{


    if(_style==""){
        mStyle = SignalQuery::Style::ACTIVE;

    }else if(_style=="START"){
        mStyle = SignalQuery::Style::ACTIVE_STARTED;

     }else if(_style=="ACTIVE"){
        mStyle = SignalQuery::Style::ACTIVE;

    }else if(_style=="END"){
       mStyle = SignalQuery::Style::ACTIVE_ENDED;

    }else if(_style=="CHANGED"){
       mStyle = SignalQuery::Style::ACTIVE_CHANGED;

    }else if(_style=="NOT_ACTIVE"){
       mStyle = SignalQuery::Style::NOT_ACTIVE;

    }else{
        if(_setErrorMessage){
            dbgSystem.addMessage("Error parsing signal query style from string'" + _style +"' !");
        }
        return false;
    }

    return true;
}


SignalQuery::SignalQuery() : SignalAccessor(SignalAccessorType::QUERY)
{

}


bool SignalQuery::active()
{

    if(mNotUsed) return true;

    //if(mSignal->dbgName == "inputCommand-fire"){
    //    std::string dbgInfo = mSignal->getDbgInfo();
    //    print(dbgInfo);
    //}

    if(mSignalOriginObtainer){

        BaseObject* signalOrigin = mSignalOriginObtainer->findOriginObject();
        if(signalOrigin == nullptr){
            return false;
        }

        if(mSignalObtainer){

            mSignalObtainer->setSignalOriginObject(signalOrigin);
            mSignal = mSignalObtainer->findSignal();
            if(mSignal==nullptr){
                return false;
            }

        }else{
            assert(mSignalIndex !=-1);
            mSignal = signalOrigin->signalStorage()->getSignal_query(mSignalIndex);

        }

    }else if(mSignalObtainer){

       mSignal = mSignalObtainer->findSignal();
       if(mSignal==nullptr){
           return false;
       }
    }


    assert(mSignal);

    //----
    if(mSignal->id()==SignalID::UPDATED_ON_SIGNAL_QUERY){
        if(mSignal->type()==SignalType::BOOL){
            UpdatedBoolSignal *ubs = static_cast<UpdatedBoolSignal*>(mSignal);
            ubs->update();
        }
    }


    //----
    if(mSignal->type()==SignalType::BOOL){

        BoolSignal *vs = static_cast<BoolSignal*>(mSignal);

        bool value = mRightSide.mBoolValue;
        if(mRightSide.mSigData){
            assert(mRightSide.mSigData->type()==SignalType::BOOL);
            BoolSignal *vsRightSide = static_cast<BoolSignal*>(mRightSide.mSigData);
            value = vsRightSide->value();
        }

        if(value==false){
            DummyFunction();
        }

        value = true;

        if(mStyle==Style::ACTIVE_STARTED){
            return vs->activeStarted(value);

        }else if(mStyle==Style::ACTIVE){
            return vs->active(value);

        }else if(mStyle==Style::ACTIVE_ENDED){
            return vs->activeEnded(value);

        }else if(mStyle==Style::ACTIVE_CHANGED){
            if(mRightSide.mAnyValueChanged){
                return vs->valueChanged();
            }else{
                return vs->activeChanged(value);
            }

        }else if(mStyle==Style::NOT_ACTIVE){
            return vs->notActive(value);

        }


    }else if(mSignal->type()==SignalType::INT){

        IntSignal *vs = static_cast<IntSignal*>(mSignal);

        int mIntValue = mRightSide.mIntValue;
        if(mRightSide.mSigData){
            assert(mRightSide.mSigData->type()==SignalType::INT);
            IntSignal *vsRightSide = static_cast<IntSignal*>(mRightSide.mSigData);
            mIntValue = vsRightSide->value();
        }

        if(mStyle==Style::ACTIVE_STARTED){
            return vs->activeStarted(mIntValue);

        }else if(mStyle==Style::ACTIVE){
            return vs->active(mIntValue);

        }else if(mStyle==Style::ACTIVE_ENDED){
            return vs->activeEnded(mIntValue);

        }else if(mStyle==Style::ACTIVE_CHANGED){
            if(mRightSide.mAnyValueChanged){
                return vs->valueChanged();
            }else{
                return vs->activeChanged(mIntValue);
            }

        }else if(mStyle==Style::NOT_ACTIVE){
            return vs->notActive(mIntValue);

        }


    }else if(mSignal->type()==SignalType::FLOAT){

        FloatSignal *vs = static_cast<FloatSignal*>(mSignal);

        float mFloatValue = mRightSide.mFloatValue;
        if(mRightSide.mSigData){
            assert(mRightSide.mSigData->type()==SignalType::FLOAT);
            FloatSignal *vsRightSide = static_cast<FloatSignal*>(mRightSide.mSigData);
            mFloatValue = vsRightSide->value();
        }

        if(mStyle==Style::ACTIVE_STARTED){
            return vs->activeStarted(mFloatValue);

        }else if(mStyle==Style::ACTIVE){
            return vs->active(mFloatValue);

        }else if(mStyle==Style::ACTIVE_ENDED){
            return vs->activeEnded(mFloatValue);

        }else if(mStyle==Style::ACTIVE_CHANGED){
            if(mRightSide.mAnyValueChanged){
                return vs->valueChanged();
            }else{
                return vs->activeChanged(mFloatValue);
            }

        }else if(mStyle==Style::NOT_ACTIVE){
            return vs->notActive(mFloatValue);

        }

    }else if(mSignal->type()==SignalType::BITSET){

        BitsetSignal *vs = static_cast<BitsetSignal*>(mSignal);

        int flagValue = mRightSide.mIntValue;
        if(mRightSide.mSigData){
            assert(mRightSide.mSigData->type()==SignalType::BITSET);
            BitsetSignal *vsRightSide = static_cast<BitsetSignal*>(mRightSide.mSigData);
            flagValue = vsRightSide->value();
        }


        if(mStyle==Style::ACTIVE_STARTED){
            return vs->activeStarted(flagValue);

        }else if(mStyle==Style::ACTIVE){
            return vs->active(flagValue);

        }else if(mStyle==Style::ACTIVE_ENDED){
            return vs->activeEnded(flagValue);

        }else if(mStyle==Style::ACTIVE_CHANGED){
            if(mRightSide.mAnyValueChanged){
                return vs->valueChanged();
            }else{
                return vs->activeChanged(flagValue);
            }

        }else if(mStyle==Style::NOT_ACTIVE){
            return vs->notActive(flagValue);

        }


    }else if(mSignal->type()==SignalType::STRING){

        StringSignal *vs = static_cast<StringSignal*>(mSignal);

        std::string mStringValue = mRightSide.mStringValue;
        if(mRightSide.mSigData){
            assert(mRightSide.mSigData->type()==SignalType::STRING);
            StringSignal *vsRightSide = static_cast<StringSignal*>(mRightSide.mSigData);
            mStringValue = vsRightSide->value();
        }

        if(mStyle==Style::ACTIVE_STARTED){
            return vs->activeStarted(mStringValue);

        }else if(mStyle==Style::ACTIVE){
            return vs->active(mStringValue);

        }else if(mStyle==Style::ACTIVE_ENDED){
            return vs->activeEnded(mStringValue);

        }else if(mStyle==Style::ACTIVE_CHANGED){
            if(mRightSide.mAnyValueChanged){
                return vs->valueChanged();
            }else{
                return vs->activeChanged(mStringValue);
            }

        }else if(mStyle==Style::NOT_ACTIVE){
            return vs->notActive(mStringValue);

        }



    }else if(mSignal->type()==SignalType::BASE_OBJECT){

        ObjectSignal *vs = static_cast<ObjectSignal*>(mSignal);

        if(mRightSide.mObjectObtainer){
            mRightSide.mObject = mRightSide.mObjectObtainer->findOriginObject();
            if(mRightSide.mObject==nullptr){
                return false;
            }
        }

        if(mStyle==Style::ACTIVE_STARTED){
            return vs->activeStarted(mRightSide.mObject);

        }else if(mStyle==Style::ACTIVE){
            return vs->active(mRightSide.mObject);

        }else if(mStyle==Style::ACTIVE_ENDED){
            return vs->activeEnded(mRightSide.mObject);

        }else if(mStyle==Style::ACTIVE_CHANGED){
            if(mRightSide.mAnyValueChanged){
                return vs->valueChanged();
            }else{
                return vs->activeChanged(mRightSide.mObject);
            }

        }else if(mStyle==Style::NOT_ACTIVE){
            return vs->notActive(mRightSide.mObject);

        }



    }

    return false;
}


//==================================================================================


void SignalSetter::set()
{

    if(mNotUsed) return;


    if(mSignalOriginObtainer){

        BaseObject* signalOrigin = mSignalOriginObtainer->findOriginObject();
        if(signalOrigin == nullptr){
            return;
        }

        if(mSignalObtainer){

            mSignalObtainer->setSignalOriginObject(signalOrigin);
            mSignal = mSignalObtainer->findSignal();
            if(mSignal==nullptr){
                return;
            }

        }else{
            assert(mSignalIndex !=-1);
            mSignal = signalOrigin->signalStorage()->getSignal_query(mSignalIndex);
        }

    }else if(mSignalObtainer){

       mSignal = mSignalObtainer->findSignal();
       if(mSignal==nullptr){
           return;
       }
    }


    if(mSignal->type()==SignalType::BOOL){

        BoolSignal *vs = static_cast<BoolSignal*>(mSignal);

        bool value = mRightSide.mBoolValue;

        if(mRightSide.mSigData){
            assert(mRightSide.mSigData->type()==SignalType::BOOL);
            BoolSignal *sigData = static_cast<BoolSignal*>(mRightSide.mSigData);
            value = sigData->value();
        }

        vs->setValue_onNextFrame(value);


    }else if(mSignal->type()==SignalType::INT){

        IntSignal *vs = static_cast<IntSignal*>(mSignal);

        int value = mRightSide.mIntValue;

        if(mRightSide.mSigData){
            assert(mRightSide.mSigData->type()==SignalType::INT);
            IntSignal *sigData = static_cast<IntSignal*>(mRightSide.mSigData);
            value = sigData->value();
        }

        vs->setValue_onNextFrame(value);


    }else if(mSignal->type()==SignalType::FLOAT){

        FloatSignal *vs = static_cast<FloatSignal*>(mSignal);

        float value = mRightSide.mFloatValue;

        if(mRightSide.mSigData){
            assert(mRightSide.mSigData->type()==SignalType::FLOAT);
            FloatSignal *sigData = static_cast<FloatSignal*>(mRightSide.mSigData);
            value = sigData->value();
        }

        vs->setValue_onNextFrame(value);


    }else if(mSignal->type()==SignalType::BITSET){

        BitsetSignal *vs = static_cast<BitsetSignal*>(mSignal);

        unsigned int flags = mRightSide.mIntValue;
        bool flagsState = mRightSide.mBoolValue;

        if(mRightSide.mSigData){
            assert(mRightSide.mSigData->type()==SignalType::BITSET);
            BitsetSignal *sigData = static_cast<BitsetSignal*>(mRightSide.mSigData);
            flags = mRightSide.mSigDataFlag;
            flagsState = sigData->active(mRightSide.mSigDataFlag);
        }

        vs->setFlags_onNextFrame(flags, flagsState);


    }else if(mSignal->type()==SignalType::STRING){

        StringSignal *vs = static_cast<StringSignal*>(mSignal);

        std::string value = mRightSide.mStringValue;

        if(mRightSide.mSigData){
            assert(mRightSide.mSigData->type()==SignalType::STRING);
            StringSignal *sigData = static_cast<StringSignal*>(mRightSide.mSigData);
            value = sigData->value();
        }

        vs->setValue_onNextFrame(value);

    }else if(mSignal->type()==SignalType::BASE_OBJECT){

        ObjectSignal *vs = static_cast<ObjectSignal*>(mSignal);

        if(mRightSide.mObjectObtainer){
            mRightSide.mObject = mRightSide.mObjectObtainer->findOriginObject();
            if(mRightSide.mObject==nullptr){
                return;
            }
        }
        assert(mRightSide.mObject);

        vs->setValue_onNextFrame(mRightSide.mObject);

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



bool CompositeQuery::parseNEW(std::string text)
{

    mKind = Kind::AND;
    StdString::removeStringWhiteSpacesOnStartAndEnd(text);

    if(text.empty()){
        return false;
    }

    int pos = 0;
    bool ok = _parse(text, pos, 0);

    return ok;

}


bool CompositeQuery::_parse(const std::string &text, int &pos, int _depth)
{

    std::string queryCfg;

    while(pos < text.length()){
        char c = text[pos];
        pos++;

        if(c == '('){

            if(pos==1 && text.back()==')'){
                mKind = Kind::AND;

            }else{
                mChildrenQueries.push_back(CompositeQuery(Kind::AND));
                bool ok = mChildrenQueries.back()._parse(text, pos, _depth+1);
                if(ok==false){
                    return false;
                }
            }


        }else if(c == '['){

            if(pos==1 && text.back()==']'){
                mKind = Kind::OR;

            }else{
                mChildrenQueries.push_back(CompositeQuery(Kind::OR));
                bool ok = mChildrenQueries.back()._parse(text, pos, _depth+1);
                if(ok==false){
                    return false;
                }
            }

        }else if(c == ')'){

            if(mKind==Kind::AND){

                StdString::removeStringWhiteSpacesOnStartAndEnd(queryCfg);
                if(queryCfg.empty()){
                    return false;
                }
                mSignalQueryCfgs.push_back(queryCfg);
                //pos = i+1;
                return true;

            }else{
                return false;
            }


        }else if(c == ']'){

            if(mKind==Kind::OR){

                StdString::removeStringWhiteSpacesOnStartAndEnd(queryCfg);
                if(queryCfg.empty()){
                    return false;
                }
                mSignalQueryCfgs.push_back(queryCfg);
                //pos = i+1;
                return true;

            }else{
                return false;
            }


        }else if(c == ','){

            StdString::removeStringWhiteSpacesOnStartAndEnd(queryCfg);
            if(queryCfg.empty()==false){
                mSignalQueryCfgs.push_back(queryCfg);
                queryCfg.clear();
            }

        }else{
            queryCfg += std::string(1, c);
        }

    }

    if(_depth==0){
        StdString::removeStringWhiteSpacesOnStartAndEnd(queryCfg);
        mSignalQueryCfgs.push_back(queryCfg);
    }

    return _depth==0;

}


bool CompositeQuery::initConnections(PlayedScene *_scene, BaseObject *obj1, BaseObject *obj2)
{


    for(const std::string &path : mSignalQueryCfgs){
        //TriggerResult triggerStyle = TriggerResult::ACTIVE;


        mSignalQueries.emplace_back(SignalQuery());
        SignalQuery &signalQuery = mSignalQueries.back();

        app->signalParserManager()->parseSignalAccessor(_scene, path, signalQuery, obj1, obj2);

        if(signalQuery.notUsed()){
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


bool CompositeQuery::initConnectionsNEW(PlayedScene *_scene, BaseObject* _rootParentObject, LogicState* _logicState)
{


    for(const std::string &path : mSignalQueryCfgs){

        mSignalQueries.emplace_back(SignalQuery());
        SignalQuery &signalQuery = mSignalQueries.back();

        if(signalQuery.parse(_scene, path, _rootParentObject, _logicState)==false){
            return false;
        }

        if(signalQuery.notUsed()){
            mSignalQueries.pop_back();
            continue;
        }

        //if(signalQuery.isValid()==false){
        //    return false;
        //}
    }


    for(CompositeQuery &tg : mChildrenQueries){
        if(tg.initConnectionsNEW(_scene, _rootParentObject, _logicState)==false){
            return false;
        }

    }

    return true;
}


bool CompositeQuery::active()
{

    if(mSignalQueries.empty() && mChildrenQueries.empty()){
        return true;
    }



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


//===================================================================================


SignalIdentifier *SignalIdentifiers::add_query(SignalType _type, unsigned char _id, const std::string &_signalPath)
{

    mSignalIdentifiers_query.emplace_back();
    mSignalIdentifiers_query.back().type = _type;
    mSignalIdentifiers_query.back().id = _id;
    mSignalIdentifiers_query.back().signalPath = _signalPath;

    return &mSignalIdentifiers_query.back();
}


SignalIdentifier *SignalIdentifiers::add_setter(SignalType _type, unsigned char _id, const std::string &_signalPath)
{

    mSignalIdentifiers_setter.emplace_back();
    mSignalIdentifiers_setter.back().type = _type;
    mSignalIdentifiers_setter.back().id = _id;
    mSignalIdentifiers_setter.back().signalPath = _signalPath;

    return &mSignalIdentifiers_setter.back();
}


bool SignalIdentifiers::getParsingInfo_query(SignalParsingInfo &_signalInfo, const std::string &_signalPath, const std::string &_signalValue, bool _setErrorMessage)
{
    return getParsingInfo(mSignalIdentifiers_query, _signalInfo, _signalPath, _signalValue, _setErrorMessage);

}


bool SignalIdentifiers::getParsingInfo_setter(SignalParsingInfo &_signalInfo, const std::string &_signalPath, const std::string &_signalValue, bool _setErrorMessage)
{
    return getParsingInfo(mSignalIdentifiers_setter, _signalInfo, _signalPath, _signalValue, _setErrorMessage);
}


bool SignalIdentifiers::getParsingInfo(std::vector<SignalIdentifier>&_signalIdentifiers, SignalParsingInfo &_signalInfo, const std::string &_signalPath, const std::string &_signalValue, bool _setErrorMessage)
{

    // for bitflag signals the bitflag name is part of the _path !

    std::string bitflagSignalPath;
    std::string bitflagName;

    size_t index = _signalPath.find_last_of(":");
    if(index != std::string::npos){
        bitflagSignalPath = _signalPath.substr(0, index);
        bitflagName = _signalPath.substr(index+1);
    }

    for(unsigned int i=0; i<_signalIdentifiers.size(); i++){
        SignalIdentifier &si = _signalIdentifiers[i];

        if(si.type==SignalType::BITSET){

            // the last part of path is extra
            /*
            std::string signalPath;
            std::string extraPath;

            size_t index = _path.find_last_of(":");
            if(index != std::string::npos){
                signalPath = _path.substr(0, index+1);
                extraPath = _path.substr(index+1);
            }
            */

            assert(si.bitflagsIdentifier);

            if(si.signalPath == bitflagSignalPath){

                _signalInfo.signalIndex = i;
                _signalInfo.signalType = si.type;
                _signalInfo.signalID = si.id;

                //---
                for(const auto &p : *si.bitflagsIdentifier){
                    if(p.first == bitflagName){
                        _signalInfo.bitFlagValue = p.second;
                        break;
                    }
                }

                if(_signalInfo.bitFlagValue==0){
                    if(_setErrorMessage){
                        dbgSystem.addMessage("Signal parsing info for path '" + _signalPath + "' error! Bitflag name '" + bitflagName + "' not found!");
                    }
                    return false;
                }

                return true;
            }

        }else{

            if(si.signalPath == _signalPath ){
                _signalInfo.signalIndex = i;
                _signalInfo.signalType = si.type;
                _signalInfo.signalID = si.id;

                if(_signalValue.empty()==false){
                    if(si.type== SignalType::INT){
                        if(si.intValuesIdentifier){
                            for(const auto & p : *si.intValuesIdentifier){
                                if(p.first == _signalValue){
                                    _signalInfo.predefinedIntValue = p.second;
                                    _signalInfo.predefiendValueDefined = true;
                                    break;
                                }
                            }
                        }

                    }else if(si.type == SignalType::FLOAT){
                        if(si.floatValuesIdentifier){
                            for(const auto & p : *si.floatValuesIdentifier){
                                if(p.first == _signalValue){
                                    _signalInfo.predefinedFloatValue = p.second;
                                    _signalInfo.predefiendValueDefined = true;
                                    break;
                                }
                            }
                        }

                    }else if(si.type == SignalType::FLOAT){
                        if(si.stringValuesIdentifier){
                            for(const auto & p : *si.stringValuesIdentifier){
                                if(p.first == _signalValue){
                                    _signalInfo.predefinedStringValue = p.second;
                                    _signalInfo.predefiendValueDefined = true;
                                    break;
                                }
                            }
                        }
                    }
                }

                return true;
            }
        }
    }


    if(_setErrorMessage){
        dbgSystem.addMessage("Signal parsing info for path '" + _signalPath + "' error!");
    }

    return false;

}





//====================================================================================


void SignalStorage::setIdentifiers(SignalIdentifiers *_identifiers)
{
    mIdentifiers = _identifiers;

}


SignalIdentifier *SignalStorage::addSignal_query(Signal* _signal, const std::string &_signalPath)
{
    assert(_signal);

    mSignals_query.push_back(_signal);

    if(mIdentifiers->identifiers_query().size() < mSignals_query.size()){
        return mIdentifiers->add_query(_signal->type(), _signal->id(), _signalPath);
    }

    return nullptr;
}


SignalIdentifier *SignalStorage::addSignal_setter(Signal* _signal, const std::string &_signalPath)
{
    assert(_signal);

    mSignals_setter.push_back(_signal);
    if(mIdentifiers->identifiers_setter().size() < mSignals_setter.size()){
        return mIdentifiers->add_setter(_signal->type(), _signal->id(), _signalPath);
    }

    return nullptr;

}


Signal* SignalStorage::getSignal_query(int index)
{
    assert(index>=0 && index<mSignals_query.size());

    return mSignals_query[index];

}


Signal* SignalStorage::getSignal_setter(int index)
{
    assert(index>=0 && index<mSignals_setter.size());

    return mSignals_setter[index];

}



//--------------------------------------------------------------------------------------

SignalParsingInfo::SignalParsingInfo()
{

}


SignalParsingInfo::~SignalParsingInfo()
{
    /*
    if(signalOriginObtainer){
        delete signalOriginObtainer;
    }
    if(signalObtainer){
        delete signalObtainer;
    }
    */
}


bool SignalParsingInfo::parseMainPaths(const std::string &_fullSignalPath)
{

    fullPath = _fullSignalPath;

    std::vector<std::string>pathParts = StdString::splitString(_fullSignalPath, "/");

    if(pathParts.size()>0){
        originObjectPath = pathParts[0];
    }
    if(pathParts.size()>1){
        signalIdentifierPath = pathParts[1];
    }
    if(pathParts.size()>2){
        signalStylePath = pathParts[2];
    }

    if(originObjectPath==""){
        dbgSystem.addMessage("Parse signal query from string '" + _fullSignalPath +"' error! Signal origin is an empty string!");
        return false;
    }

    return true;

}

//---------------------------------------------------------------------------------------------

UpdatedBoolSignal::UpdatedBoolSignal() : BoolSignal(static_cast<unsigned char>(SignalID::UPDATED_ON_SIGNAL_QUERY))
{

}




}
