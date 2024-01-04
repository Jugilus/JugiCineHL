#ifndef ENTITY_SYSTEM__ENTITY_UTILITIES__H
#define ENTITY_SYSTEM__ENTITY_UTILITIES__H


#include <string>
#include <vector>
#include <tuple>

#include "box2d/box2d.h"
#include "jmCommonFunctions.h"

#include "jpGlobal.h"
#include "jpObjectParser.h"
#include "jpQueries.h"







namespace jugimap{

class EntitySystem;
class Entity;
class SourceEntity;
class OriginObjectObtainer;
class PlayedScene;
class LogicState;
//class BehaviorState;
class SignalAccessor;
struct MovementEngineData;
struct TaskEngineData;


//void ObtainSignalFromPath(const std::string &_path, PlayedScene *_scene, Entity *_actor, BehaviorState * _state, SignalAccessor &_signalAccessor);


MovementEngineData* ObtainMovementEngineData(Entity* _entity, const std::string &name, bool throwError=true);

TaskEngineData* ObtainTaskEngineData(Entity* _entity, const std::string &name, bool throwError=true);


/*
struct EntitySignalStrings
{
    std::string engineCfgName;
    std::string taskCfgName;
    std::string signalName;
    std::string signalValue;
    std::string signalValueState;

    bool parse(const std::string &_path);

    bool getBoolValue(bool &_state, bool _setErrorMessage=true) const;
    bool getIntValue(int &_value) const;

};

*/


class EntitySignalsParser : public SignalParser
{
public:

    EntitySignalsParser(std::vector<std::string> _keyWords) : SignalParser(_keyWords){}

    void parseSignalAccessor(PlayedScene *_scene, const std::string &_path, SignalAccessor &_signalAccessor, BaseObject *obj1=nullptr, BaseObject *obj2=nullptr) override;
    bool parseSignalAccessorNEW(PlayedScene *_scene, SignalParsingInfo &_spi, const std::string &_signalValue) override;

    void parseItemDataPair(PlayedScene *scene, const std::string &_path, ItemDataPair &_itemDataPair, ParseItemMode _parseItemMode, BaseObject *obj1 = nullptr, BaseObject *obj2 = nullptr) override;
    bool parseItemDataPairNEW(PlayedScene *_scene, ItemDataParsingHelper &_dpi) override;

    //Item * obtainRootItem(PlayedScene *scene, const std::string &_path, BaseObject *_obj1 = nullptr, BaseObject *_obj2 = nullptr) override;
    bool parseObjectName(PlayedScene *_scene, PrintParsingHelper &_pph) override;

    bool parseObject(PlayedScene *_scene, SignalParsingInfo &_spi) override;

private:
    bool parseEntitySignal(PlayedScene *_scene, SignalParsingInfo &_spi, const std::string &_signalValue);
    bool parseEntityManagerSignal(PlayedScene *_scene, SignalParsingInfo &_spi, const std::string &_signalValue);
    bool parseOriginEntity(PlayedScene *_scene, const std::string &_originPath);



    void parse_signalQuery(PlayedScene *_scene, Entity *_entity, LogicState *_state, const std::string &_signalNameAndValue, SignalQuery &_signalQuery);
    void parse_signalSetter(PlayedScene *_scene, Entity *_entity, LogicState *_state, const std::string &_signalNameAndValue, SignalSetter &_signalSetter);

    void reset();

    LogicState *mThisLogicState = nullptr;               // LINK
    EntitySystem* mEntitySystem = nullptr;               // LINK
    Entity* mThisEntity = nullptr;                       // LINK
    Entity* mOriginEntity = nullptr;                     // LINK
    SourceEntity* mOriginSourceEntity = nullptr;         // LINK
    OriginObjectObtainer* mOriginObtainer = nullptr;     // LINK (parented later)

    bool mAccessorNotUsed = false;


};




}


#endif
