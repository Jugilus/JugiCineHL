#ifndef PLAYER__ENTITY__ENTITY_SYSTEM__H
#define PLAYER__ENTITY__ENTITY_SYSTEM__H

#include <memory>
#include <string>
#include <vector>

#include "jmStorage.h"
#include "jmSignal.h"

#include "jpGlobal.h"
#include "components/jpComponentsCommon.h"
#include "jpEntityPhase.h"
#include "jpEntityCommon.h"


namespace pugi{
class xml_node;
}

namespace jugimap{

class CompositeSprite;
class PlayedScene;
struct EntityLogicStateCfg;
struct AttributeLogicStateCfg;
class Entity;
//class TasksHandlersCfgsGroup;
//class TasksCfgsGroup;
//class EnginesControllersCfgsGroup;
class DebugDraw;
class WorldCameraManager;
class EntityContactListener;
class EntityContactFilter;
class ExtraGroundTypesCfgsGroup;
class AnimationManager;
class ShapeDrawer;
class MovementEnginesManager;
class TaskEngineManager;
class SourceEntitiesGroup;
class EntitySignalsParser;
struct AttributeSetCfg;
class EntityPhasesHandler;
//struct EntityPhasesHandlerCfg;



class EntitySystem : public Component
{
public:
    EntitySystem();
    ~EntitySystem();

    bool initCfg(PlayedScene *playerScene, const pugi::xml_node &_node) override;
    bool initConnections(PlayedScene *_scene) override;

    bool createMapEntities(PlayedScene *_scene);

    void start() override;
    bool startingPhaseUpdate() override;
    void update(UpdateMode &_updateMode) override;
    void drawShapes(ShapeDrawer &_drawer, int _flags=0) override;
    void onStateEnded() override;


    //void spawnEntity(SourceEntity* _sourceEntity);
    //void removeEntity(Entity* _entity);


    std::vector<Entity*> & entities(){ return mEntities; }

    SimpleStorage<EntityLogicStateCfg*> & enginesControllersCfsStorage() { return mEnginesControllersCfgsStorage; }
    SimpleStorage<EntityLogicStateCfg*> & taskControllersCfgsStorage() { return mTaskControllerCfgsStorage; }

    SimpleStorage<AttributeSetCfg*> & attributeSetsStorage(){ return mAttributeSetsStorage; }
    SimpleStorage<AttributeLogicStateCfg*> & attributeControllersCfgsStorage() { return mAttributeControllersCfgsStorage; }


    ExtraGroundTypesCfgsGroup * extraGroundTypesCfgsGroup() { return mExtraGroundTypesCfgsGroup.get(); }

    b2World * world(){ return mB2World.get(); }

    Entity* getActor(const std::string &_name, bool _setErrorMessage = true);
    Entity* getActor(const std::string &_name, int _linkedGroupID, int _linkedEntityID, CompositeSprite *_rootCompositeSprite, bool _setErrorMessage = true);
    //void collectActorsViaLinkedGroupID(std::vector<Entity*> &_collectedActors, const std::string &_name, int _linkedGroupID);

    SignalIdentifiers &signalIdentifiers() { return mSignalIdentifiers; }
    SignalStorage* signalStorage() override { return &mSignalStorage;}

    PlayedScene* parentPlayerScene(){ return mParentPlayerScene; }

    //void doDebugDraw(bool _doDebugDraw){ mDoDebugDraw = _doDebugDraw; }
    //bool isDoDebugDraw(){ return mDoDebugDraw; }

    //MovementConfigurations * movementConfigurations() { return mMovementConfiguration; }

    //AnimationManager * animationManager(){ return mAnimationManager.get(); }

    EntityCategoriesGroup & entityCategoriesGroup(){ return mContactCategories; }
    FilteredContactSignalsStorage & filteredContactTriggersGroup(){ return mFilteredContactTriggersGroup; }
    EntityGroupsManager & entityGroupsManager(){ return mEntityMovingGroupsManager; }

    MovementEnginesManager *movementEnginesManager(){ return mMovementEnginesManager.get(); }
    TaskEngineManager * taskEnginesManager(){ return mTaskEngineManager.get(); }

    EntitySignalsParser *entitySignalsParser(){ return mEntitySignalsParser; }

    SourceEntitiesGroup* sourceEntitiesGroup(){ return mSourceEntitiesGroup.get(); }

    EntityLifespanHandler & entityLifespanHandler(){ return mEntityLifespanHandler; }

    //SimpleStorage<EntityPhasesHandler> & entityPhasesHandlerStorage(){ return mEntityPhasesHandlerStorage; }
    //SimpleStorage<EntityPhasesHandlerCfg> & entityPhasesHandlerCfgStorage(){ return mEntityPhasesHandlerCfgStorage; }

    std::vector<Entity*> & spawnedEntities(){ return mSpawnedEntities; }

private:


    PlayedScene *mParentPlayerScene = nullptr;


    SimpleStorage<EntityLogicStateCfg*> mEnginesControllersCfgsStorage{"EnginesControllersCfgs"};
    SimpleStorage<EntityLogicStateCfg*> mTaskControllerCfgsStorage{"TaskControllerCfgs"};

    SimpleStorage<AttributeLogicStateCfg*> mAttributeControllersCfgsStorage{"AttributeControllerCfgs"};
    SimpleStorage<AttributeSetCfg*> mAttributeSetsStorage{"AttributeSetsCfgs"};

    std::unique_ptr<ExtraGroundTypesCfgsGroup> mExtraGroundTypesCfgsGroup;
    std::unique_ptr<SourceEntitiesGroup> mSourceEntitiesGroup;

    //SimpleStorage<EntityPhasesHandler>mEntityPhasesHandlerStorage{"EntityPhasesHandlersStorage"};
    //SimpleStorage<EntityPhasesHandlerCfg>mEntityPhasesHandlerCfgStorage{"EntityPhasesHandlerCfgsStorage"};



    std::vector<Entity*>mEntities;                                                     // OWNED
    std::vector<Entity*>mSpawnedEntities;

    std::unique_ptr<b2World>mB2World;
    std::unique_ptr<DebugDraw>mDebugDraw;
    std::unique_ptr<WorldCameraManager>mWorldCameraManager;

    std::unique_ptr<EntityContactListener>mEntityContactListener;
    std::unique_ptr<EntityContactFilter>mEntityContactFilter;


    std::unique_ptr<MovementEnginesManager>mMovementEnginesManager;
    std::unique_ptr<TaskEngineManager>mTaskEngineManager;

    EntityLifespanHandler mEntityLifespanHandler;

    float mTimeStepS = 1.0f / 60.0f;
    int32 mVelocityIterations = 6;
    int32 mPositionIterations = 2;

    EntityCategoriesGroup mContactCategories;
    FilteredContactSignalsStorage mFilteredContactTriggersGroup;
    EntityGroupsManager mEntityMovingGroupsManager;

    EntitySignalsParser *mEntitySignalsParser = nullptr;        // LINK

    //----
    ObjectSignal mSigRemoveEntity;
    ObjectSignal mSigSpawnEntity;

    SignalStorage mSignalStorage;
    SignalIdentifiers mSignalIdentifiers;

    SpawnerAndSpawnedGroup mSpawnerAndSpawnedGroup;

    //MovementConfigurations* mMovementConfiguration = nullptr;

    //std::vector<PathMovementCfg*>mPathMovementConfigurations;
    //std::vector<GroundMovementCfg*>mGroundMovementConfigurations;
    //std::vector<JumpMovementCfg*>mJumpMovementConfigurations;

    void _update(UpdateMode &_updateMode, std::vector<Entity*> &_entities);


};



//------------------------------------------------------------------------------------



class EntitySystemFactory : public CustomComponentFactory
{
public:

    EntitySystemFactory(const std::string &_typeSignature) : CustomComponentFactory(_typeSignature){}

    Component * createComponent() override { return new EntitySystem(); }


};







}






#endif
