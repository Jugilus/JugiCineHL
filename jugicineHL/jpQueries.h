#ifndef PLAYER__QUERIES_H
#define PLAYER__QUERIES_H

#include <assert.h>
#include <string>
#include <vector>
#include "jmSignal.h"

#include "jpGlobal.h"



namespace jugimap{



class PlayedScene;


struct SignalAccessor
{

    enum class Type
    {
        QUERY,
        SETTER,
        UNKNOWN
    };


    ~SignalAccessor();


    Type mType = Type::UNKNOWN;
    Signal *mSignal = nullptr;                  // LINK

    //int mFlagValue = 0;
    int mIntValue = 0;
    float mFloatValue = 0.0f;
    bool mBoolValue = false;
    std::string mStringValue;
    bool mAnyValueChanged = false;
    bool valueNotRequired = false;


    bool mOwnedSignal = false;
    bool mNotUsed = false;

};



struct SignalQuery : public SignalAccessor
{

    enum class Style : unsigned char
    {
        ACTIVE,
        ACTIVE_STARTED,
        ACTIVE_ENDED,
        ACTIVE_CHANGED,
        NOT_ACTIVE,
        UNKNOWN
    };


    SignalQuery(){ mType = Type::QUERY; }


    void init(Signal *_signal, Style _style){ mSignal=_signal; mStyle=_style; }
    bool isValid(){ return mSignal!=nullptr && mStyle!=Style::UNKNOWN; }

    bool active();



    Style mStyle = Style::UNKNOWN;

};


//==============================================================


struct SignalSetter : public SignalAccessor
{
    /*
    enum class Style : unsigned char
    {
        SET,
        UNSET,
        UNKNOWN
    };
    */


    SignalSetter(){ mType = Type::SETTER; }


    //void init(Signal *_signal, Style _style){ mSignal=_signal; mStyle=_style; }
    void init(Signal *_signal){ mSignal=_signal; }
    //bool isValid(){ return mSignal!=nullptr && mStyle!=Style::UNKNOWN; }
    bool isValid(){ return mSignal!=nullptr; }
    void set();


    //Style mStyle = Style::UNKNOWN;
    Signal *mSigData = nullptr;         // static signal which provides value for 'mSignal'
    int mSigDataFlag = 0;               // in the case 'mSigData' is a bitset signal

};


//==============================================================



class CompositeQuery //: public BoolSignal
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
    bool initConnections(PlayedScene *_scene, BaseObject* obj1=nullptr, BaseObject* obj2=nullptr);

    bool active();



    std::vector<SignalQuery> & signalQueries(){ return mSignalQueries; }
    std::vector<CompositeQuery> & childrenQueries(){ return mChildrenQueries; }
    std::vector<std::string> & signalQueryCfgs(){ return mSignalQueryCfgs;}


private:
    Kind mKind = Kind::UNKNOWN;

    std::vector<SignalQuery>mSignalQueries;
    //std::vector<TriggerResult>mTriggerStyles;

    std::vector<CompositeQuery>mChildrenQueries;

    std::vector<std::string>mSignalQueryCfgs;

};

//------------------------------------------------------------------

class SignalParser
{
public:

    virtual ~SignalParser(){}

    SignalParser(std::vector<std::string> _keyWords) : mKeyWords(_keyWords){}
    std::vector<std::string> & keyWords(){ return mKeyWords; }

    virtual void parseSignalAccessor(PlayedScene *scene, const std::string &_path, SignalAccessor &_signalAccessor, BaseObject* obj1=nullptr, BaseObject *obj2=nullptr) = 0;


protected:
    std::vector<std::string>mKeyWords;

};


class CoreSignalsParser : public SignalParser
{
public:


    CoreSignalsParser(std::vector<std::string> _keyWords) : SignalParser(_keyWords){}

    void parseSignalAccessor(PlayedScene *scene, const std::string &_path, SignalAccessor &_signalAccessor, BaseObject *obj1=nullptr, BaseObject *obj2=nullptr) override;

};




//------------------------------------------------------------------

class SignalParserManager
{
public:

    ~SignalParserManager();

    void addAndStoreSignalParser(SignalParser *_signalParser);


    void parseSignalAccessor(PlayedScene *_scene, const std::string &_path, SignalAccessor &_signalAccessor, BaseObject *obj1=nullptr, BaseObject* obj2=nullptr);



private:
    std::vector<SignalParser*>mSignalParsers;

    bool parseSignalAccessorStyle(const std::string & syle, SignalAccessor &_signalAccessor);
};


//------------------------------------------------------------------


struct NamedValue
{

    NamedValue(const std::string &_name, int _number) : name(_name), intValue(_number){}
    std::string name;
    int intValue = 0;

};



class ParsedSignalPath
{
public:

    ParsedSignalPath(const std::string &_fullPath);

    //void parsePath_name_value(const std::string &_path);
    const std::string & signalFullName(){ return mSignalFullName; }
    const std::string & signalNamePartAt(int i);
    std::vector<std::string> & signalNameParts(){ return mSignalNameParts; }

    bool obtainValue(SignalQuery &_signalQuery, Signal* _signal, std::vector<NamedValue> *_valuesSet = nullptr);
    bool obtainValue(SignalSetter &_signalSetter, Signal* _signal, std::vector<NamedValue> *_valuesSet = nullptr);


private:
    std::string mFullPath;
    std::string mSignalFullName;
    std::vector<std::string>mSignalNameParts;
    std::string mEmpty;
    std::string mFlagName;
    std::string mValue;

    //bool parse_query(Signal* _signal);
    //bool parse_setter(Signal* _signal);

};



class UpdatedBoolSignal : public BoolSignal
{
public:

    UpdatedBoolSignal() : BoolSignal(static_cast<unsigned char>(SignalID::CUSTOM_ENTITY_SIGNAL)){}
    virtual void update() = 0;

};



}


#endif // PLAYER__QUERIES_H
