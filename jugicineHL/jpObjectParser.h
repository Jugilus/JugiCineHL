#ifndef PLAYER__OBJECT_PARSER_H
#define PLAYER__OBJECT_PARSER_H

#include <assert.h>
#include <string>
#include <vector>
#include "jmSignal.h"

//#include "character/jpCharacterBase.h"
#include "data/jpDataUtilities.h"
#include "jpGlobal.h"



namespace jugimap{


class LogicState;
class PlayedScene;
class SignalAccessor;
struct SignalParsingInfo;
struct PrintParsingHelper;
struct ItemDataParsingHelper;


/*
struct ActionOriginObjects
{
    BaseObject *rootParentObject = nullptr;
    LogicState *logicState = nullptr;

};
*/

class SignalParser
{
public:

    virtual ~SignalParser(){}

    SignalParser(std::vector<std::string> _keyWords) : mKeyWords(_keyWords){}
    std::vector<std::string> & keyWords(){ return mKeyWords; }

    virtual void parseSignalAccessor(PlayedScene *_scene, const std::string &_path, SignalAccessor &_signalAccessor, BaseObject* obj1=nullptr, BaseObject *obj2=nullptr) = 0;
    virtual bool parseSignalAccessorNEW(PlayedScene *_scene, SignalParsingInfo &_signalParsingInfo, const std::string &_signalValue){ return false; }

    virtual void parseItemDataPair(PlayedScene *_scene, const std::string &_path, ItemDataPair &_itemDataPair, ParseItemMode _parseItemMode, BaseObject* obj1=nullptr, BaseObject *obj2=nullptr) {}
    virtual bool parseItemDataPairNEW(PlayedScene *_scene, ItemDataParsingHelper &_dpi) { return true; }

    virtual bool parseObject(PlayedScene *_scene, SignalParsingInfo &_spi) { return true; }


    virtual bool parseObjectName(PlayedScene *_scene, PrintParsingHelper &_pph) { return true; }

    //virtual Item * obtainRootItem(PlayedScene *scene, const std::string &_path, BaseObject* _obj1=nullptr, BaseObject* _obj2=nullptr){ return nullptr; }


protected:
    std::vector<std::string>mKeyWords;

};


class CoreSignalsParser : public SignalParser
{
public:


    CoreSignalsParser(std::vector<std::string> _keyWords) : SignalParser(_keyWords){}

    void parseSignalAccessor(PlayedScene *scene, const std::string &_path, SignalAccessor &_signalAccessor, BaseObject *obj1=nullptr, BaseObject *obj2=nullptr) override;
    bool parseSignalAccessorNEW(PlayedScene *scene, SignalParsingInfo &_spi, const std::string &_signalValue) override;

    void parseItemDataPair(PlayedScene *scene, const std::string &_path, ItemDataPair &_itemDataPair, ParseItemMode _parseItemMode, BaseObject *obj1 = nullptr, BaseObject *obj2 = nullptr) override;
    //Item * obtainRootItem(PlayedScene *scene, const std::string &_path, BaseObject* _obj1=nullptr, BaseObject* _obj2=nullptr) override;


};




//------------------------------------------------------------------

class SignalParserManager
{
public:

    ~SignalParserManager();

    void addAndStoreSignalParser(SignalParser *_signalParser);


    void parseSignalAccessor(PlayedScene *_scene, const std::string &_path, SignalAccessor &_signalAccessor, BaseObject *obj1=nullptr, BaseObject* obj2=nullptr);
    //void parseSignalAccessorNEW(PlayedScene *scene, const std::string &_signalPath, const std::string &_signalValue, SignalAccessor &_signalAccessor, SignalParsingInfo &_signalParsingInfo, const ActionOriginObjects &_originObjects);

    void parseItemDataPair(PlayedScene *_scene, const std::string &_path, ItemDataPair &_itemDataPair, ParseItemMode _parseItemMode, BaseObject *obj1=nullptr, BaseObject* obj2=nullptr);

    //void parseObjectName(PlayedScene *_scene, const std::string &_path, std::string &_objectName, BaseObject* obj1=nullptr, BaseObject *obj2=nullptr);

    //bool obtainRootItems(PlayedScene *_scene, std::vector<std::string> &_rootItemsPath, std::vector<Item*>&_rootItems, BaseObject *_obj1=nullptr,  BaseObject *_obj2=nullptr);

    bool parseSignalAccessorNEW(PlayedScene *_scene, SignalParsingInfo &_spi,  const std::string &_path, const std::string &_signalValue);
    bool parseObject(PlayedScene *_scene, SignalParsingInfo &_spi,  const std::string &_path);
    bool parseItemDataPairNEW(PlayedScene *_scene, ItemDataParsingHelper &_dpi, const std::string &_path);
    SignalParser *obtainObjectParserNEW(const std::string &_path, bool _setErrorMessage=true);

private:
    std::vector<SignalParser*>mSignalParsers;

    SignalParser *obtainSignalParser(std::string parserKeyword);



    bool parseSignalAccessorStyle(const std::string & syle, SignalAccessor &_signalAccessor);
};


//------------------------------------------------------------------



class ParsedSignalPath
{
public:

    ParsedSignalPath(const std::string &_fullPath);

    //void parsePath_name_value(const std::string &_path);
    const std::string & signalFullName(){ return mSignalFullName; }
    const std::string & signalNamePartAt(int i);
    std::vector<std::string> & signalNameParts(){ return mSignalNameParts; }

    //bool obtainValue(SignalQuery &_signalQuery, Signal* _signal);
    //bool obtainValue(SignalSetter &_signalSetter, Signal* _signal);

    bool obtainValue(SignalAccessor &_signalAccessor, Signal* _signal);



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







}


#endif // PLAYER__QUERIES_H
