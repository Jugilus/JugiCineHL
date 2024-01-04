#ifndef PLAYER__QUERIES_H
#define PLAYER__QUERIES_H

#include <assert.h>
#include <string>
#include <vector>
#include "jmSignal.h"

//#include "character/jpCharacterBase.h"
#include "data/jpDataUtilities.h"
#include "jpGlobal.h"



namespace jugimap{


class Item;
class PlayedScene;
class LogicState;
class DoSetSignal;
class ACOnSignalNEW;
class ParsedSignalPath;
struct ActionOriginObjects;
struct SignalParsingInfo;
class SignalParser;
class LogicStateCfg;




class OriginObjectObtainer;
class SignalObtainer;


enum class SignalAccessorType
{
    QUERY,
    SETTER,
    UNKNOWN
};



struct SignalParsingInfo
{

    SignalParsingInfo();
    ~SignalParsingInfo();

    BaseObject *rootParentObject = nullptr;
    LogicState *logicState = nullptr;

    std::string fullPath;
    std::string originObjectPath;
    std::string signalIdentifierPath;
    std::string signalStylePath;

    bool parseMainPaths(const std::string &_fullSignalPath);


    SignalAccessorType signalAccessorType = SignalAccessorType::UNKNOWN;

    //----
    bool accessorNotUsed = false;
    SignalType signalType = SignalType::UNKNOWN;
    unsigned char signalID = 0;
    int signalIndex = -1;

    //---
    unsigned int bitFlagValue = 0;

    int predefinedIntValue = 0;
    float predefinedFloatValue = 0;
    std::string predefinedStringValue;
    bool predefiendValueDefined = false;
    //---

    //----
    Signal *signal = nullptr;                           // LINK
    BaseObject * originObject = nullptr;         // LINK

    //---
    OriginObjectObtainer *originObjectObtainer = nullptr;   // LINK ( ownership transfered to signal accessor )
    SignalObtainer *signalObtainer = nullptr;      // LINK ( ownership transfered to signal accessor )

};



struct SignalAccessorRightSide
{

    SignalAccessorRightSide(){}
    SignalAccessorRightSide(const SignalAccessorRightSide &_src);
    SignalAccessorRightSide& operator =(const SignalAccessorRightSide &_src);
    ~SignalAccessorRightSide();

    int mIntValue = 0;
    float mFloatValue = 0.0f;
    bool mBoolValue = false;
    std::string mStringValue;
    bool mAnyValueChanged = false;
    bool mValueNotRequired = false;

    Signal *mSigData = nullptr;                         // LINK static signal which provides value for 'mSignal'
    int mSigDataFlag = 0;                               // in the case 'mSigData' is a bitset signal

    BaseObject *mObject = nullptr;
    OriginObjectObtainer* mObjectObtainer = nullptr;    // OWNED
};


class SignalAccessor
{
public:
    friend class DoSetSignal;
    friend class ParsedSignalPath;
    friend class ACOnSignalNEW;






    SignalAccessor(SignalAccessorType _type);
    SignalAccessor(const SignalAccessor &_type);
    ~SignalAccessor();


    SignalAccessorType type(){ return mType; }
    Signal* signal(){ return mSignal; }
    //SignalParsingInfo &signalParsingInfo(){ return mSignalParsingInfo; }

    bool notUsed(){ return mNotUsed; }

    void _setSignal(Signal *_signal){ mSignal = _signal; }
    void _setOwnedSignal(bool _ownedSignal){ mOwnedSignal = _ownedSignal; }
    void _setValueNotRequired(bool _valueNotRequired){ mRightSide.mValueNotRequired = _valueNotRequired; }
    void _setNotUsed(bool _notUsed){ mNotUsed = _notUsed; }

    bool parseBoolSignal(PlayedScene *_scene, const std::string & _leftSignalPath, bool _rightSideValue, BaseObject *_rootParentObject);
    bool parse(PlayedScene *_scene, const std::string & _signalFullPath, BaseObject *_rootParentObject, LogicState *_logicState);

    bool parseDirectSignalValue(const std::string & _signalValue, SignalParsingInfo &_spi, const std::string & _signalPath);
    virtual bool parseStyle(const std::string & style, bool _setErrorMessage=true){ return true;}


protected:

    SignalAccessorType mType = SignalAccessorType::UNKNOWN;
    Signal *mSignal = nullptr;                  // LINK
    OriginObjectObtainer* mSignalOriginObtainer = nullptr;      // OWNED
    SignalObtainer* mSignalObtainer = nullptr;                  // OWNED
    int mSignalIndex = -1;


    /*
    int mIntValue = 0;
    float mFloatValue = 0.0f;
    bool mBoolValue = false;
    std::string mStringValue;
    bool mAnyValueChanged = false;
    bool mValueNotRequired = false;

    Signal *mSigData = nullptr;         // LINK static signal which provides value for 'mSignal'
    int mSigDataFlag = 0;               // in the case 'mSigData' is a bitset signal
    */

    SignalAccessorRightSide mRightSide;

    bool mOwnedSignal = false;
    bool mNotUsed = false;

};



class SignalQuery : public SignalAccessor
{
public:

    enum class Style : unsigned char
    {
        ACTIVE,
        ACTIVE_STARTED,
        ACTIVE_ENDED,
        ACTIVE_CHANGED,
        NOT_ACTIVE,
        UNKNOWN
    };


    SignalQuery();


    void init(Signal *_signal, Style _style){ mSignal=_signal; mStyle=_style; }

    bool isValid(){ return mSignal!=nullptr && mStyle!=Style::UNKNOWN; }
    bool active();

    bool parseStyle(const std::string & style, bool _setErrorMessage=true) override;

private:
    Style mStyle = Style::UNKNOWN;

};


//==============================================================


class SignalSetter : public SignalAccessor
{
public:

    SignalSetter() : SignalAccessor(SignalAccessorType::SETTER){}


    void init(Signal *_signal){ mSignal=_signal; }
    bool isValid(){ return mSignal!=nullptr; }
    void set();

};


//==============================================================



class CompositeQuery
{
public:

    enum class Kind
    {
        AND,
        OR,
        UNKNOWN
    };


    CompositeQuery() : mKind(Kind::UNKNOWN){}
    CompositeQuery(Kind _kind) : mKind(_kind){}

    bool parse(std::string text);
    bool parseNEW(std::string text);
    bool initConnections(PlayedScene *_scene, BaseObject* obj1=nullptr, BaseObject* obj2=nullptr);

    bool initConnectionsNEW(PlayedScene *_scene, BaseObject* _rootParentObject, LogicState* _logicState);

    bool active();


    std::vector<SignalQuery> & signalQueries(){ return mSignalQueries; }
    std::vector<CompositeQuery> & childrenQueries(){ return mChildrenQueries; }
    std::vector<std::string> & signalQueryCfgs(){ return mSignalQueryCfgs;}


private:
    Kind mKind = Kind::UNKNOWN;

    std::vector<SignalQuery>mSignalQueries;
    std::vector<CompositeQuery>mChildrenQueries;
    std::vector<std::string>mSignalQueryCfgs;

    bool _parse(const std::string &text, int &pos, int _depth);

};


//==============================================================

struct SignalIdentifier;



struct SignalIdentifier
{
    SignalType type = SignalType::UNKNOWN;
    unsigned char id = 0;
    std::string signalPath;

    std::vector<std::pair<std::string, unsigned int>>*bitflagsIdentifier = nullptr;         // LINK
    std::vector<std::pair<std::string, int>>*intValuesIdentifier = nullptr;                 // LINK
    std::vector<std::pair<std::string, float>>*floatValuesIdentifier = nullptr;             // LINK
    std::vector<std::pair<std::string, std::string>>*stringValuesIdentifier = nullptr;      // LINK
};


class SignalIdentifiers
{
public:

    SignalIdentifier * add_query(SignalType _type, unsigned char _id, const std::string &_signalPath);
    SignalIdentifier * add_setter(SignalType _type, unsigned char _id, const std::string &_signalPath);

    bool getParsingInfo_query(SignalParsingInfo &_signalInfo, const std::string &_signalPath, const std::string &_signalValue, bool _setErrorMessage=true);
    bool getParsingInfo_setter(SignalParsingInfo &_signalInfo, const std::string &_signalPath, const std::string &_signalValue, bool _setErrorMessage=true);


    std::vector<SignalIdentifier> & identifiers_query(){ return mSignalIdentifiers_query; }
    std::vector<SignalIdentifier> & identifiers_setter(){ return mSignalIdentifiers_setter; }

private:
    std::vector<SignalIdentifier>mSignalIdentifiers_query;          // LINK
    std::vector<SignalIdentifier>mSignalIdentifiers_setter;        // LINK


    bool getParsingInfo(std::vector<SignalIdentifier>&_signalIdentifiers, SignalParsingInfo &_signalInfo, const std::string &_signalPath, const std::string &_signalValue, bool _setErrorMessage=true);

};


class SignalStorage
{
public:

    void setIdentifiers(SignalIdentifiers * _identifiers);

    SignalIdentifier * addSignal_query(Signal* _signal, const std::string &_signalPath);
    SignalIdentifier * addSignal_setter(Signal* _signal, const std::string &_signalPath);

    //void getParsingInfo(SignalParsingInfo &_signalInfo, const std::string &_signaId, const std::string &_extraId, bool _setErrorMessage=true);
    //void getParsingInfo(SignalParsingInfo &_signalInfo, const std::string &_path, bool _setErrorMessage=true);

    //Signal *getSignal(int _index);
    Signal* getSignal_query(int index);
    Signal* getSignal_setter(int index);

    std::vector<Signal*>& signals_query(){ return mSignals_query; }
    std::vector<Signal*>& signals_setter(){ return mSignals_setter; }
    SignalIdentifiers* identifiers(){ return mIdentifiers; }


private:
    std::vector<Signal*>mSignals_query;                         // LINKS
    std::vector<Signal*>mSignals_setter;                         // LINKS
    SignalIdentifiers *mIdentifiers = nullptr;                  // LINK

};


//===================================================================================


class UpdatedBoolSignal : public BoolSignal
{
public:

    UpdatedBoolSignal();
    virtual void update() = 0;


};





class SignalObtainer
{
public:

    SignalObtainer(BaseObject * _signalOriginObject ) : mSignalOriginObject(_signalOriginObject){}
    virtual ~SignalObtainer(){}
    virtual SignalObtainer* copy() = 0;

    BaseObject* signalOriginObject() { return mSignalOriginObject; }
    void setSignalOriginObject(BaseObject *_signalOriginObject){ mSignalOriginObject = _signalOriginObject; }


    virtual Signal* findSignal() = 0;

protected:
    BaseObject *mSignalOriginObject = nullptr;


};




}


#endif // PLAYER__QUERIES_H
