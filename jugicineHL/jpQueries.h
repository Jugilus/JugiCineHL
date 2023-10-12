#ifndef PLAYER__QUERIES_H
#define PLAYER__QUERIES_H

#include <assert.h>
#include <string>
#include <vector>
#include "jmSignal.h"



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
    int mIntValue = 0;

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
        NOT_ACTIVE,
        UNKNOWN
    };


    SignalQuery(){ mType = Type::QUERY; }


    void init(Signal *_signal, Style _style){ mSignal=_signal; mStyle=_style; }
    bool isValid(){ return mSignal!=nullptr && mStyle!=Style::UNKNOWN; }

    bool active();

    void set();


    Style mStyle = Style::UNKNOWN;

};


//==============================================================


struct SignalSetter : public SignalAccessor
{

    enum class Style : unsigned char
    {
        SET,
        UNSET,
        UNKNOWN
    };



    SignalSetter(){ mType = Type::SETTER; }


    void init(Signal *_signal, Style _style){ mSignal=_signal; mStyle=_style; }
    bool isValid(){ return mSignal!=nullptr && mStyle!=Style::UNKNOWN; }
    void set();


    Style mStyle = Style::UNKNOWN;

};


//==============================================================



class CompositeQuery : public BoolSignal
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

    bool initConnections(PlayedScene *_scene, void * obj1=nullptr, void * obj2=nullptr, void * obj3=nullptr);

    bool isConditionTrue();

    std::vector<SignalQuery> & triggers(){ return mTriggers; }
    std::vector<CompositeQuery> & childTriggersGroups(){ return mChildTriggersGroups; }
    std::vector<std::string> & triggersCfg(){ return mTriggersCfg;}


private:
    Kind mKind = Kind::UNKNOWN;

    std::vector<SignalQuery>mTriggers;
    //std::vector<TriggerResult>mTriggerStyles;

    std::vector<CompositeQuery>mChildTriggersGroups;

    std::vector<std::string>mTriggersCfg;

};


//------------------------------------------------------------------

class SignalParser
{
public:

    virtual ~SignalParser(){}

    SignalParser(std::vector<std::string> _keyWords) : mKeyWords(_keyWords){}
    std::vector<std::string> & keyWords(){ return mKeyWords; }

    virtual void parseSignalAccessor(PlayedScene *scene, const std::string &_path, SignalAccessor &_signalAccessor, void * obj1=nullptr, void * obj2=nullptr, void * obj3=nullptr ) = 0;


protected:
    std::vector<std::string>mKeyWords;

};


class CoreSignalsParser : public SignalParser
{
public:


    CoreSignalsParser(std::vector<std::string> _keyWords) : SignalParser(_keyWords){}

    void parseSignalAccessor(PlayedScene *scene, const std::string &_path, SignalAccessor &_signalAccessor, void * obj1=nullptr, void * obj2=nullptr, void * obj3=nullptr) override;

};




//------------------------------------------------------------------

class SignalParserManager
{
public:

    ~SignalParserManager();

    void addSignalParser(SignalParser *_signalParser);


    void parseSignalAccessor(PlayedScene *_scene, const std::string &_path, SignalAccessor &_signalAccessor, void * obj1=nullptr, void * obj2=nullptr, void * obj3=nullptr);



private:
    std::vector<SignalParser*>mSignalParsers;

    bool parseSignalAccessorStyle(const std::string & syle, SignalAccessor &_signalAccessor);
};





}


#endif // PLAYER__QUERIES_H
