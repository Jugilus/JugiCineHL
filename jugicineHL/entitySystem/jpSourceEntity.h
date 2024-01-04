#ifndef PLAYER__ENTITIES__SOURCE_ENTITIES__H
#define PLAYER__ENTITIES__SOURCE_ENTITIES__H

#include <memory>
#include <string>
#include <vector>
#include "box2d.h"
#include "jmCommon.h"
#include "jmStorage.h"
#include "jpEntityGlobal.h"
#include "jpEntityLogicState.h"


namespace pugi{
class xml_node;
}


namespace jugimap{


class Entity;
class PlayedScene;
class EntitySystem;
class SourceSprite;
class SourceEntity;
class SourceBody;
class SpriteLayer;
class FixtureUserDataGroup;
struct EntityCategory;
struct EntityLogicStateCfg;
struct MovementEngineCfg;

struct AttributeSetCfg;
struct AttributeLogicStateCfg;
//struct EnginesControllerCfg;
//struct TaskControllerCfg;
//struct EnvironmentObjectBehaviorCfg;


//-------------------------------------------------------------------------------

struct ActorPathwayContactPointOffset
{
    std::string actorName;
    b2Vec2 offsetM{0.0f, 0.0f};
};



struct PathwaySourceEntityCfg
{

    bool initCfg(const pugi::xml_node &_node);

    b2Vec2 getContactPointOffsetForActor(const std::string &_actorName);


    PathwayOrientation orientation = PathwayOrientation::VERTICAL;
    //float centeringMarginM = 1.0f;
    CenteringStyle centeringStyle = CenteringStyle::NONE;

    std::vector<ActorPathwayContactPointOffset>actorsPathwayContactPointOffsets;

};


//-------------------------------------------------------------------------------


struct SpawnedEntityCfg
{
    std::string spawnedSourceEntityName;
    SourceEntity *spawnedSourceEntity = nullptr;
    std::string spawnerEngineName;
    MovementEngineCfg *spawnerMovementEngineCfg = nullptr;
    Direction spawnerDirection = Direction::INHERITED;
    //b2Vec2 offsetM{0.0f, 0.0f};
    Vec2f offsetP;
};


struct SpawnerCfg
{

    bool initCfg(const pugi::xml_node &_node);
    bool initConnections(PlayedScene *_scene, EntitySystem* _entitySystem);

    Vec2f getOffsetForSpawnedEntity(SourceEntity* _sourceEntity, MovementEngineCfg* _currentEngineCfg,  Direction _direction);

    std::vector<SpawnedEntityCfg>spawnedEntitiesCfgs;

};



//-------------------------------------------------------------------------------


struct SourceEntityCfg
{

    //SourceEntityCfg(EntityType _type, const std::string &_name, EntityBodyType _bodyType) : type(_type), bodyType(_bodyType), name(_name){}
    //~SourceEntityCfg();

    SourceEntityCfg(const std::string &_name, EntityCategory * _entityCategory) :
        name(_name),
        category(_entityCategory)
    {}

    bool initCfg(const pugi::xml_node &_node);


    std::string name;
    EntityCategory *category = nullptr;
    //EntityBodyProperties entityBodyProperties;
    EnumEntityMapElement entityMapElement = EnumEntityMapElement::UNKNOWN;



    std::string item;
    std::string animation;
    std::string relativeMover;

    //EntityType type = EntityType::UNKNOWN;
    //EntityBodyType bodyType = EntityBodyType::SOLID;

    EntityLogicStateCfg *enginesControllerCfg = nullptr;  //LINK
    std::string startState;
    //bool ownedEnginesControllerCfg = false;

    EntityLogicStateCfg *taskControllerCfg = nullptr;               // LINK
    std::string taskHandlerStartState;

    AttributeSetCfg *attributeSetCfg = nullptr;
    AttributeLogicStateCfg *attributeControllerCfg = nullptr;

    //----
    std::unique_ptr<PathwaySourceEntityCfg>mPathwaySourceEntityCfg;

    //----
    //std::unique_ptr<EnginesControllerCfg>ownedEnginesControllerCfg;
    std::vector<CustomSensorSourceEntityCfg>mCustomSensorSourceEntityCfgs;

    std::unique_ptr<SpawnerCfg>mSpawnerCfg;


    //----
    bool passableFromBellow = false;
    Margin centeringMargin;

    //----
    //unsigned int collisionCategoryBits = 0;

    bool fixedRotation = true;
    bool spawnedOnce = false;

    EntitySystem * entitySystem = nullptr;  // LINK

};


//-------------------------------------------------------------------------------


/*
class SourceEntitiesCfgsGroup
{
public:

    SourceEntityCfg* getSourceEntitiyCfg(const std::string &_name, bool _setErrorMessage = true);
    std::vector<SourceEntityCfg> &sourceEntitiesCfgs(){ return mSourceEntitiesCfgs; }

private:
    std::vector<SourceEntityCfg>mSourceEntitiesCfgs;

};
*/

//-------------------------------------------------------------------------------


class SourceEntity : public BaseObject
{
public:


    SourceEntity(SourceEntityCfg *_sourceEntityCfg);
    bool createSourceBody(PlayedScene *_scene, SourceSprite *_sourceSprite, SpriteLayer *_spriteLayer);

    //EntityType type() const { return mSourceEntityCfg->type; }
    EntityRole entityBodyKind(); //{ return mSourceEntityCfg->category;}
    const std::string & name(){ return mSourceEntityCfg->name; }
    SourceEntityCfg* sourceEntityCfg() const { return mSourceEntityCfg; }
    SourceSprite * sourceSprite() const { return mSourceSprite; }
    SpriteLayer * spriteLayer() const { return mSpriteLayer; }
    SourceBody* sourceBody() const { return mSourceBody.get(); }
    SignalIdentifiers & signalIdentifiers(){ return mSignalIdentifiers; }

    void _setSpriteLayer(SpriteLayer *_spriteLayer){ mSpriteLayer = _spriteLayer; }

    PoolStorage<Entity*, 10> & entityPool(){ return mEntityPool; }

    std::vector<Entity*> &spawners(){ return mSpawners; }


private:
    SourceEntityCfg * mSourceEntityCfg = nullptr;       // LINK

    SourceSprite * mSourceSprite = nullptr;             // LINK
    SpriteLayer * mSpriteLayer = nullptr;               // LINK

    std::unique_ptr<SourceBody>mSourceBody;
    SignalIdentifiers mSignalIdentifiers;

    PoolStorage<Entity*, 10> mEntityPool;

    std::vector<Entity*>mSpawners;

};



//-------------------------------------------------------------------------------


class SourceEntitiesGroup
{
public:

    SourceEntitiesGroup();
    ~SourceEntitiesGroup();

    bool create(PlayedScene *_scene);

    SourceEntity* getSourceEntitiy(const std::string &_name, bool _setErrorMessage = true);
    std::vector<SourceEntity> &sourceEntities(){ return mSourceEntities; }

    SourceEntityCfg* getSourceEntitiyCfg(const std::string &_name, bool _setErrorMessage = true);
    std::vector<SourceEntityCfg> &sourceEntitiesCfgs(){ return mSourceEntitiesCfgs; }


private:
    std::vector<SourceEntity>mSourceEntities;
    std::vector<SourceEntityCfg>mSourceEntitiesCfgs;
    //std::unique_ptr<FixtureUserDataGroup> mFixtureUserDataGroup;

    bool createSingleSpriteSourceEntities(PlayedScene *_scene);
    bool createSpriteLayerSourceEntities(PlayedScene *_scene);
    bool createVectorShapeSourceEntities(PlayedScene *_scene);
    //void createSourceEntitiesCfgsForSpriteLayerEntities(PlayedScene *_scene, EntityBodyKind _entityType);


};




}






#endif
