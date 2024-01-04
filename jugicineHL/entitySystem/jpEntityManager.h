#ifndef ENTITY_SYSTEM__ENTITY_MANAGER__H
#define ENTITY_SYSTEM__ENTITY_MANAGER__H


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



/*
class EntityManager
{
public:


    void spawnEntity(SourceEntity* _sourceEntity);
    void removeEntity(Entity* _entity);
    void transferItem(Entity *_srcEntity, Item *_srcItem, Entity *_dstEntity, Item *_dstItem);



private:
    EntitySystem* mEntitySystem = nullptr;      // LINK


};

*/



}


#endif
