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
class BehaviorState;
struct SignalAccessor;
struct MovementEngineData;
struct TaskEngineData;


//void ObtainSignalFromPath(const std::string &_path, PlayedScene *_scene, Entity *_actor, BehaviorState * _state, SignalAccessor &_signalAccessor);


MovementEngineData* ObtainMovementEngineData(Entity* _entity, const std::string &name, bool throwError=true);

TaskEngineData* ObtainTaskEngineData(Entity* _entity, const std::string &name, bool throwError=true);



class EntitySignalsParser : public SignalParser
{
public:

    EntitySignalsParser(std::vector<std::string> _keyWords) : SignalParser(_keyWords){}

    void parseSignalAccessor(PlayedScene *scene, const std::string &_path, SignalAccessor &_signalAccessor, void* obj1=nullptr, void* obj2=nullptr, void* obj3=nullptr) override;

private:
    void parse_signalQuery(Entity *_entity, BehaviorState *_state, const std::string &_path, SignalQuery &_signalQuery);
    void parse_signalSetter(Entity *_entity, BehaviorState *_state, const std::string &_path, SignalSetter &_signalSetter);

};




}


#endif
