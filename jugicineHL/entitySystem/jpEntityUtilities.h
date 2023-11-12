#ifndef ENTITY_SYSTEM__ENTITY_UTILITIES__H
#define ENTITY_SYSTEM__ENTITY_UTILITIES__H


#include "string"
#include "vector"
#include "box2d/box2d.h"
#include "jmCommonFunctions.h"

#include "jpGlobal.h"
#include "jpQueries.h"







namespace jugimap{


class PlayedScene;
class LogicState;
//class BehaviorState;
struct SignalAccessor;
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

private:
    void parse_signalQuery(PlayedScene *_scene, Entity *_entity, LogicState *_state, const std::string &_signalNameAndValue, SignalQuery &_signalQuery);
    void parse_signalSetter(PlayedScene *_scene, Entity *_entity, LogicState *_state, const std::string &_signalNameAndValue, SignalSetter &_signalSetter);

};




}


#endif
