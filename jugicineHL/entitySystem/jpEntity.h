#ifndef PLAYER__ENTITY__ENTITY__H
#define PLAYER__ENTITY__ENTITY__H

#include <memory>
#include <string>
#include <vector>

#include "jmCommon.h"
#include "jmAnimationCommon.h"

#include "jmStorage.h"
#include "jpGlobal.h"
#include "data/jpDataUtilities.h"
#include "jpEntityCommon.h"



namespace pugi{
class xml_node;
}

namespace jugimap{


class Sprite;
class StandardSprite;
class SpriteLayer;

class Item;
class EntitySystem;
class LogicState;
class Body;
struct SourceEntityCfg;
class SourceEntity;

class MovementEngine;
struct MovementEngineData;
class MovementEngineFactory;

class TaskEngine;
class TaskEngineData;
class TaskEngineFactory;

class AttributeSet;
class EntityPhasesHandler;

class GORelativeMover;



class EntityMapElement
{
public:

    enum class Type
    {
        SPRITE,
        SPRITE_LAYER,
        VECTOR_SHAPE
    };


    ~EntityMapElement();

    void init(Sprite *_sprite, VectorShape *_vectorShape, SpriteLayer* _spriteLayer);

    Type type(){ return mType;}
    CParameters &parameters();
    Vec2f position();
    float rotation();

    void setPositionFromBody(Vec2f _pos);
    void setRotationFromBody(float _rotation);

    void setVisible(bool _visible);
    void setAlpha(float _alpha);
    void removeFromMap();
    void addToMap();

    Sprite * sprite() const { return mSprite; }
    VectorShape * vectorShape() { return mVectorShape; }
    SpriteLayer * spriteLayer() { return mSpriteLayer; }
    CompositeSprite* rootCompositeSprite(){ return mRootCompositeSprite; }


private:
    Type mType = Type::SPRITE;
    Sprite *mSprite = nullptr;                                      // LINK
    VectorShape* mVectorShape = nullptr;                            // LINK
    SpriteLayer* mSpriteLayer = nullptr;

    CompositeSprite* mRootCompositeSprite = nullptr;

    bool mOwnedSprite = false;

};



//=======================================================================================



class Entity : public BaseObject
{
public:
    friend class AppConfigurationLoader_xml;


    Entity();
    ~Entity();

    bool build(PlayedScene *_scene, SourceEntity *_sourceEntity, Sprite *_sprite, VectorShape *_vectorShape, SpriteLayer *_spriteLayer);

    bool initConnections1(PlayedScene *_scene);
    bool initConnections2(PlayedScene *_scene);
    bool initConnections(PlayedScene *_scene);

    void addToWorld();
    void removeFromWorld();

    EntityRole mainShapeRole() const;
    SourceEntity * sourceEntity() const { return mSourceEntity; }
    EntitySystem * parentEntitySystem();

    void forceTransformFromSprite();

    void reset();

    void start();
    bool startingPhaseUpdate();

    void preUpdate_resolveContacts();
    void preUpdate_resolveContacts_part2();

    void preUpdate(UpdateMode &_updateMode);
    void preUpdate_CheckActorGrouping();

    void update_Controller(UpdateMode &_updateMode);
    void update_Movement(UpdateMode &_updateMode);
    void postUpdate(UpdateMode &_updateMode);

    void updateSpriteTransform();
    void spawnEntity(SourceEntity *_sourceEntity);

    //----
    SignalStorage * signalStorage() override { return &mSignalStorage;}
    std::vector<Signal*> & updateSignals(){ return mUpdateSignals; }
    //ItemDataLUT * itemDataLUT() override { return &mItemDataLUT; }
    //EntityPhasesHandler * entityPhasesHandler(){ return mEntityPhasesHandler.get(); }


    EntityMapElement & mapElement(){ return mMapElement; }
    Sprite * sprite() const { return mMapElement.sprite(); }
    VectorShape * vectorShape() { return mMapElement.vectorShape(); }
    Body *body(){ return mBody.get(); }


    unsigned int & statusFlagsRef(){ return mStatusFlags; }
    int linkedGroupID(){ return mLinkedGroupID;}
    int linkedEntityID(){ return mLinkedEntityID; }
    Direction direction(){ return mDirection; }
    //void _setDirection(Direction _direction){ mDirection = _direction; }

    b2Vec2 velocity(){ return mVelocity;}
    void _setVelocity(b2Vec2 _velocity){ mVelocity = _velocity; }
    float angularVelocity(){ return mAngularVelocity;}
    void _setAngularVelocity(float _angularVelocity){ mAngularVelocity = _angularVelocity; }

    b2Vec2 relativeVelocity(){ return mRelVelocity;}
    b2Vec2 engineVelocity(){ return mEngineVelocity;}
    b2Vec2 netoVelocity(){ return mEngineVelocity - mRelVelocity; }

    EntityMovingGroup * entityMovingGroup(){ return mEntityMovingGroup; }
    void setEntityGroup(EntityMovingGroup *_entityGroup){ mEntityMovingGroup = _entityGroup; }


    AnimationPlayer & animationPlayer() { return mAnimationPlayer; }
    LogicState* entityController() const { return mEntityController.get(); }

    std::vector<MovementEngine*> & movementEngines() { return mMovementEngines; }
    MovementEngine* currentEngine(){ return mCurrentEngine; }

    MovementEngine* getMovementEngine(MovementEngineFactory* _factory);
    MovementEngineData* getMovementEngineData(const std::string &_name);

    bool startMovementEngine(MovementEngineData *_movementEngineData);

    GORelativeMover* relativeMover(){ return mRelativeMover.get(); }
    //PointToPointMovementTask* pointToPointMovemenTask() const;

    std::vector<TaskEngine*> & tasks(){ return mTasks; }
    TaskEngine* currentTask() const { return mCurrentTaskEngine; }
    TaskEngine* getTaskEngine(TaskEngineFactory* _factory);
    TaskEngineData* getTaskData(const std::string &_name);
    //Decider* decider() const { return mDecider; }
    bool startTaskEngine(TaskEngineData *_taskEngineData);

    Item* item() override { return mItem; }

    Vec2f position() override { return mMapElement.position(); }

    bool initialized(){ return mInitialized; }

    std::vector<ContactPointInfo> & contactPoints(){ return mContactPoints; }
    int & contactPointsCounterRef(){ return mContactPointsCount; }

    int & blockedDirectionsRef() { return mBlockedDirections; }


    //void doPointToPointMovementTask(PointToPointMovementTaskData *_data);

    //FixtureGroup & groundSensorFixtures(){ return mGroundSensorFixtures; }


    ContactTrigger &contactTrigger(){ return mContactTrigger; }

    Entity* lift(){ return mLift; }

    std::vector<Signal*> & signals(){ return mSignals; }

    IntSignal &customActSignal(){ return mSigCustomAct; }
    int addCustomActName(const std::string &_name);

    BoolSignal * itemPickableSignal(){ return mSigItemPickable; }

    //Signal *getSignal(SignalID _signalID);

    PoolObjectPos & poolPos(){ return mPoolPos; }

    AnimationInstance* currentAnimationInstance(){ return mCurrentAnimationInstance; }
    void setCurrentAnimationInstance(AnimationInstance* _animationInstance){ mCurrentAnimationInstance = _animationInstance; }


    ObjectSignal & spawnEntitySignal(){ return  mSigSpawnEntity; }
    ObjectSignal & spawnedSingleEntitySignal(){ return mSigSpawnedSingleEntity; }

    //void obtainSignal_signalQuery(SignalQuery &_signalQuery, const std::string &_path, const std::string &stateMovementEngineCfg, bool _setErrorMessage=true);
    //void obtainSignal_signalSetter(SignalSetter &_signalSetter, const std::string &_path, const std::string &stateMovementEngineCfg, bool _setErrorMessage=true);

    void obtainSignal_signalQuery(SignalQuery &_signalQuery, ParsedSignalPath &_psp, bool _setErrorMessage=true);
    void obtainSignal_signalSetter(SignalSetter &_signalSetter, ParsedSignalPath &_psp, bool _setErrorMessage=true);



private:
    SourceEntity* mSourceEntity = nullptr;          // LINK
    EntityMapElement mMapElement;
    std::unique_ptr<Body>mBody;

    int mLinkedGroupID = 0;                // unique identifier of a map entiy
    int mLinkedEntityID = 0;                // unique identifier of a map entiy

    unsigned int mStatusFlags = EntityStatusFlags::NONE;

    //---
    std::vector<MovementEngine*>mMovementEngines;                        // OWNED
    MovementEngine * mCurrentEngine = nullptr;                      // LINK
    std::unique_ptr<LogicState>mEntityController;

    //---
    std::vector<TaskEngine*>mTasks;
    TaskEngine* mCurrentTaskEngine = nullptr;
    std::unique_ptr<LogicState>mTaskController;

    //Decider *mDecider = nullptr;
    //---
    std::unique_ptr<AttributeSet>mAttributeSet;
    std::unique_ptr<LogicState>mAttributeController;

    //std::unique_ptr<EntityPhasesHandler>mEntityPhasesHandler;

    //std::unique_ptr<Item>mItem;

    Item * mItem = nullptr;                 // LINK - ACQUIRED

    //---
    AnimationPlayer mAnimationPlayer;
    AnimationInstance *mCurrentAnimationInstance = nullptr;         // LINK

    //---
    EntityMovingGroup *mEntityMovingGroup = nullptr;        // LINK

    //---
   // FixtureGroup mGroundSensorFixtures;


    //---
    std::vector<ContactPointInfo>mContactPoints;
    int mContactPointsCount = 0;


    //---
    //ItemDataLUT mItemDataLUT;

    //---
    ContactTrigger mContactTrigger;


    Entity * mLift = nullptr;                               // LINK
    Entity * mMovableObject = nullptr;

    //--- SIGNALS
    std::vector<Signal*>mSignals;                            // LINKS
    SignalStorage mSignalStorage;
    std::vector<Signal*>mUpdateSignals;

    BitsetSignal *mSigMovableObject = nullptr;              // OWNED
    BitsetSignal *mSigBlockedDirection = nullptr;           // OWNED
    BoolSignal *mSigItemPickable = nullptr;                 // OWNED

    ObjectSignal mSigSpawnEntity;
    ObjectSignal mSigSpawnedSingleEntity;
    IntSignal mSigCustomAct;
    std::vector<std::pair<std::string, int>>mCustomActs;



    //IntSignal *mDirectionSignal = nullptr;                  // LINK
    std::unique_ptr<GORelativeMover>mRelativeMover;

    //---
    bool mInitialized = false;

    EngineUpdateParameters mEup;

    //---
    Direction mDirection = Direction::NONE;
    int mBlockedDirections = 0;
    b2Vec2 mEngineVelocity{0.0f, 0.0f};
    b2Vec2 mVelocity{0.0f, 0.0f};
    //b2Vec2 mPressureVelocity{0.0f, 0.0f};
    b2Vec2 mRelVelocity{0.0f, 0.0f};
    float mAngularVelocity = 0.0f;

    //---
    PoolObjectPos mPoolPos;

    MovementEngine* _obtainEngineForSignal(const std::string & engineCfgName, const std::string & signalName);
    TaskEngine* _obtainTaskEngineForSignal(const std::string & engineCfgName, const std::string & signalName);


    void checkGroundContactViaContactSignal();

};





}






#endif
