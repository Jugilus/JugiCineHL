#ifndef PLAYER__ENTITY__ENTITY_SYSTEM__H
#define PLAYER__ENTITY__ENTITY_SYSTEM__H

#include <memory>
#include <string>
#include <vector>
#include "jpGlobal.h"
#include "components/jpComponentsCommon.h"
#include "jpEntityCommon.h"


namespace pugi{
class xml_node;
}

namespace jugimap{

class CompositeSprite;
class PlayedScene;
class Entity;
//class TasksHandlersCfgsGroup;
//class TasksCfgsGroup;
class EnginesControllersCfgsGroup;
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
    void update(UpdateMode _updateMode) override;
    void drawShapes(ShapeDrawer &_drawer, int _flags=0) override;
    void onStateEnded() override;


    //Actor* getActor(const std::string &_name, bool _setErrorMessage = true);
    //std::vector<Actor*> &actors(){ return mActors; }

    //SourceEntitiesGroup *sourceEntitiesGroup(){ return mSourceEntitiesGroup.get(); }
    //SourceEntitiesCfgsGroup * sourceEntititesCfgsGroup(){ return mSourceEntitiesCfgsGroup.get(); }

    //EnginesCfgsGroup * enginesCfgsGroup() { return mEnginesCfgsGroup.get(); }
    EnginesControllersCfgsGroup * enginesControllersCfsGroup() { return mEnginesControllersCfgsGroup.get(); }
    //TasksCfgsGroup * tasksCfgsGroup(){ return mTasksCfgsGroup.get(); }

    EnginesControllersCfgsGroup * taskControllersCfgsGroup() { return mTaskControllerCfgsGroup.get(); }
    //SourceEntity* getSourceEntitiy(const std::string &_name, bool _setErrorMessage = true);
    ExtraGroundTypesCfgsGroup * extraGroundTypesCfgsGroup() { return mExtraGroundTypesCfgsGroup.get(); }

    b2World * world(){ return mB2World.get(); }

    Entity* getActor(const std::string &_name, bool _setErrorMessage = true);
    Entity* getActor(const std::string &_name, int _linkedGroupID, int _linkedEntityID, CompositeSprite *_rootCompositeSprite, bool _setErrorMessage = true);
    void collectActorsViaLinkedGroupID(std::vector<Entity*> &_collectedActors, const std::string &_name, int _linkedGroupID);


    PlayedScene* parentPlayerScene(){ return mParentPlayerScene; }

    //void doDebugDraw(bool _doDebugDraw){ mDoDebugDraw = _doDebugDraw; }
    //bool isDoDebugDraw(){ return mDoDebugDraw; }

    //MovementConfigurations * movementConfigurations() { return mMovementConfiguration; }

    //AnimationManager * animationManager(){ return mAnimationManager.get(); }

    EntityCategoriesGroup & entityCategoriesGroup(){ return mContactCategories; }
    FilteredContactTriggersGroup & filteredContactTriggersGroup(){ return mFilteredContactTriggersGroup; }
    EntityGroupsManager & entityGroupsManager(){ return mEntityMovingGroupsManager; }

    MovementEnginesManager *movementEnginesManager(){ return mMovementEnginesManager.get(); }
    TaskEngineManager * taskEnginesManager(){ return mTaskEngineManager.get(); }

    EntitySignalsParser *entitySignalsParser(){ return mEntitySignalsParser.get(); }

private:


    PlayedScene *mParentPlayerScene = nullptr;

    //std::vector<SourceEntity*>mSourceEntities;
    //std::unique_ptr<SourceEntitiesGroup> mSourceEntitiesGroup;
    //std::unique_ptr<SourceEntitiesCfgsGroup> mSourceEntitiesCfgsGroup;

    //std::unique_ptr<EnginesCfgsGroup> mEnginesCfgsGroup;

    std::unique_ptr<EnginesControllersCfgsGroup> mEnginesControllersCfgsGroup;
    //std::unique_ptr<FixtureUserDataGroup> mFixtureUserDataGroup;
    //std::unique_ptr<TasksCfgsGroup> mTasksCfgsGroup;
    std::unique_ptr<EnginesControllersCfgsGroup> mTaskControllerCfgsGroup;
    std::unique_ptr<ExtraGroundTypesCfgsGroup>mExtraGroundTypesCfgsGroup;
    std::unique_ptr<SourceEntitiesGroup> mSourceEntitiesGroup;


    std::vector<Entity*>mActors;                                                     // OWNED

    std::unique_ptr<b2World>mB2World;
    std::unique_ptr<DebugDraw>mDebugDraw;
    //bool mDoDebugDraw = true;
    std::unique_ptr<WorldCameraManager>mWorldCameraManager;

    std::unique_ptr<EntityContactListener>mEntityContactListener;
    std::unique_ptr<EntityContactFilter>mEntityContactFilter;

    //std::unique_ptr<AnimationManager>mAnimationManager;

    std::unique_ptr<MovementEnginesManager>mMovementEnginesManager;
    std::unique_ptr<TaskEngineManager>mTaskEngineManager;

    float mTimeStepS = 1.0f / 60.0f;
    int32 mVelocityIterations = 6;
    int32 mPositionIterations = 2;

    EntityCategoriesGroup mContactCategories;

    FilteredContactTriggersGroup mFilteredContactTriggersGroup;
    EntityGroupsManager mEntityMovingGroupsManager;

    std::unique_ptr<EntitySignalsParser>mEntitySignalsParser;

    //MovementConfigurations* mMovementConfiguration = nullptr;

    //std::vector<PathMovementCfg*>mPathMovementConfigurations;
    //std::vector<GroundMovementCfg*>mGroundMovementConfigurations;
    //std::vector<JumpMovementCfg*>mJumpMovementConfigurations;

    void _update(UpdateMode _updateMode, std::vector<Entity*> &_entities);


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
