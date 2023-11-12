#include <chrono>
#include <assert.h>
#include <sstream>
#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmMap.h"
#include "jmCamera.h"
#include "jmSceneLayout.h"
#include "jmCommonFunctions.h"
#include "jmSpriteLayer.h"
#include "jmVectorLayer.h"
#include "jmStandardSprite.h"
#include "jmCompositeSprite.h"
#include "jmVectorShape.h"
#include "jmSprite.h"
#include "jmSourceSprite.h"
#include "jmVectorShapeUtilities.h"
#include "jmUtilities.h"
#include "items/jpItemsCommon.h"
#include "jpPlayedApp.h"
#include "jpPlayedScene.h"
#include "jpUtilities.h"
#include "jpLogicState.h"
#include "jpLogicStateCfg.h"

//-----
#include "jpB2Body.h"
#include "jpB2Utilities.h"
#include "jpB2World.h"
#include "jpB2DebugDraw.h"
#include "jpSourceEntity.h"
#include "jpEntityCommon.h"
#include "jpEntitySystem.h"
#include "movements/jpMovementBase.h"
#include "task/jpTaskBase.h"
#include "jpEntityLogicState.h"
#include "jpEntityGroups.h"
#include "jpEntityUtilities.h"
#include "jpEntity.h"



namespace jugimap{



using namespace jugimap;





void EntityMapElement::init(Sprite *_sprite, VectorShape *_vectorShape, SpriteLayer *_spriteLayer)
{

    mSprite = _sprite;
    mVectorShape = _vectorShape;
    mSpriteLayer = _spriteLayer;

    if(mSpriteLayer){
        mType = Type::SPRITE_LAYER;

    }else if(mVectorShape){
        mType = Type::VECTOR_SHAPE;
        mRootCompositeSprite = mVectorShape->parentLayer()->rootCompositeSprite();

    }else if(mSprite){
        mType = Type::SPRITE;
        mRootCompositeSprite = mSprite->parentLayer()->rootCompositeSprite();
        if(mRootCompositeSprite==nullptr && mSprite->type()==SpriteType::COMPOSITE){
            mRootCompositeSprite = static_cast<CompositeSprite*>(mSprite);
        }
    }

}


CParameters & EntityMapElement::parameters()
{

    if(mVectorShape){
        return mVectorShape->parameters();

    }else if(mSprite){
        return mSprite->parameters();

    }else if(mSpriteLayer){
        return mSpriteLayer->parameters();

    }


    assert(false);
    return mSprite->parameters();
}


Vec2f EntityMapElement::position()
{

    if(mVectorShape){

        if(mSprite){

            // situation when the vector shape is stored inside a composite sprite
            assert(mSprite->type()==SpriteType::COMPOSITE);

            Vec2f worldPos = mSprite->absPosition();
            return worldPos;
            //pos += worldPos;

            //return pos;

        }else{

            Vec2f pos = mVectorShape->pathPoints().front();
            return pos;
        }


    }else if(mSprite){
        return mSprite->position();

    }else if(mSpriteLayer){
        return Vec2f{0.0f,0.0f};

    }



    assert(false);
    return mSprite->position();

}


float EntityMapElement::rotation()
{

    if(mVectorShape){
        return 0.0f;

    }else if(mSpriteLayer){
        return 0.0f;
    }


    return mSprite->rotation();

}


void EntityMapElement::setPositionFromBody(Vec2f _pos)
{

    if(mVectorShape || mSpriteLayer){

        // do nothing

    }else if(mSprite){
        mSprite->setPosition(_pos);
    }

     // currently position for a vector shape can not be set

}


void EntityMapElement::setRotationFromBody(float _rotation)
{


    if(mVectorShape || mSpriteLayer){

        // do nothing

    }else if(mSprite){
        mSprite->setRotation(_rotation);
    }

    // currently rotation for a vector shape can not be set

}


/*
Entity::Entity(SourceEntity *_sourceEntity) : mSourceEntity(_sourceEntity)
{

    //mType = BaseObjectType::BEHAVIOR;
    //mType = EntityType::ACTOR;
}
*/


Entity::Entity(SourceEntity *_sourceEntity) : mSourceEntity(_sourceEntity)
{

}

/*
EntityType Entity::type() const
{
    return mSourceEntity->sourceEntityCfg()->type;

}
*/


Entity::~Entity()
{

    for(MovementEngine* be : mMovementEngines){
        delete be;
    }

    for(TaskEngine *at : mTasks){
        delete at;
    }


    if(mSigMovableObject){
        delete mSigMovableObject;
    }

}


EntitySystem * Entity::parentEntitySystem()
{
    return mSourceEntity->sourceEntityCfg()->entitySystem;
}


EntityRole Entity::mainShapeRole() const
{
    return mSourceEntity->sourceEntityCfg()->category->role;
}


bool Entity::build(Sprite *_sprite, VectorShape *_vectorShape, SpriteLayer *_spriteLayer)
{

    dbgSystem.addMessage("Building actor '" + mSourceEntity->sourceEntityCfg()->name + "' ...");


    mMapElement.init(_sprite, _vectorShape, _spriteLayer);
    //mSprite = _sprite;
    //mVectorShape = _vectorShape;
    mCurrentEngine = parentEntitySystem()->movementEnginesManager()->getDummyMovementEngine();       // initial engine
    mCurrentTaskEngine = parentEntitySystem()->taskEnginesManager()->getDummyTaskEngine();

    SourceEntityCfg *sourceEntityCfg = mSourceEntity->sourceEntityCfg();
    EntityLogicStateCfg *enginesControllerCfg = sourceEntityCfg->enginesControllerCfg;


    if(enginesControllerCfg){

        MovementEnginesManager *mem = parentEntitySystem()->movementEnginesManager();
        std::map<std::string, std::vector<MovementEngineCfg*>> movementEngineCfgs;

        for(LogicStateCfg* _stateCfg : enginesControllerCfg->childStatesCfgs()){
            EntityLogicStateCfg* stateCfg = static_cast<EntityLogicStateCfg*>(_stateCfg);

            if(stateCfg->movementEngine.empty()==false){
                MovementEngineCfg* cfg = mem->getMovementEngineCfg(stateCfg->movementEngine);
                if(cfg==nullptr){
                    return false;
                }
                movementEngineCfgs[cfg->factory->typeSignature()].push_back(cfg);
            }
        }

        for(auto &elem : movementEngineCfgs){
            const std::string &typeSignature = elem.first;
            MovementEngine* movementEngine = mem->createMovementEngine(typeSignature);
            assert(movementEngine);
            movementEngine->createDataObjects(elem.second);
            mMovementEngines.push_back(movementEngine);

            movementEngine->init(this);
        }

        //----
        if(enginesControllerCfg->childStatesCfgs().empty()==false){
            mEntityController.reset(new LogicState(enginesControllerCfg, this));
            if(mEntityController->build(enginesControllerCfg)==false){
                return false;
            }
        }
    }



    EntityLogicStateCfg * taskHandlerCfg = sourceEntityCfg->taskControllerCfg;
    if(taskHandlerCfg){

        TaskEngineManager *mem = parentEntitySystem()->taskEnginesManager();
        std::map<std::string, std::vector<TaskEngineCfg*>> deciderCfgs;

        for(LogicStateCfg* _stateCfg : taskHandlerCfg->childStatesCfgs()){
            EntityLogicStateCfg * stateCfg = static_cast<EntityLogicStateCfg*>(_stateCfg);
            if(stateCfg->taskEngine.empty()==false){
                TaskEngineCfg* cfg = mem->getTaskEngineCfg(stateCfg->taskEngine);
                if(cfg==nullptr){
                    return false;
                }
                deciderCfgs[cfg->factory->typeSignature()].push_back(cfg);
            }
        }

        for(auto &elem : deciderCfgs){
            const std::string &typeSignature = elem.first;
            TaskEngine* decider = mem->createTaskEngine(typeSignature);
            assert(decider);
            decider->createDataObjects(elem.second);
            mTasks.push_back(decider);

            decider->init(this);
        }

        //----
        if(taskHandlerCfg->childStatesCfgs().empty()==false){
            mTaskController.reset(new LogicState(taskHandlerCfg, this));
            if(mTaskController->build(taskHandlerCfg)==false){
                return false;
            }
        }

        /*
        if(taskHandlerCfg->mPointToPointMovementTaskCfgs.empty()==false){
            PointToPointMovementTask *task = new PointToPointMovementTask();
            mTasks.push_back(task);
            task->dataObjects().resize(taskHandlerCfg->mPointToPointMovementTaskCfgs.size());
            for(unsigned int i=0; i<taskHandlerCfg->mPointToPointMovementTaskCfgs.size(); i++){
                PointToPointMovementTaskCfg * cfg = taskHandlerCfg->mPointToPointMovementTaskCfgs.at(i);
                PointToPointMovementTaskData & data = task->dataObjects().at(i);
                data.cfg = cfg;
            }
        }

        //----
        if(taskHandlerCfg->mStatesCfgs.empty()==false){
            mTaskController.reset(new EntityController(taskHandlerCfg, this));
            if(mTaskController->build()==false){
                return false;
            }
        }
        */
    }



    if(sourceEntity()->sourceEntityCfg()->name=="fancyPlatformA_light"){
        DummyFunction();
    }

    mLinkedGroupID = mMapElement.parameters().intValue("eLinkedGroupID", 0);      // Must be set before 'initConnections' !
    mLinkedEntityID = mMapElement.parameters().intValue("eLinkedEntityID", 0);      // Must be set before 'initConnections' !

    if(mMapElement.rootCompositeSprite()){
        mLinkedGroupID = mMapElement.rootCompositeSprite()->parameters().intValue("eLinkedGroupID", 0);      // Must be set before 'initConnections' !
    }


    //----
    //if(mSourceEntity->sourceEntityCfg()->category->role==EntityRole::TRANSPORTER){
     //   mSignals.push_back(new IntBitsSignal(static_cast<unsigned char>(SignalID::TRANSPORTER_STATUS)));
    //
    //}else
    if(mainShapeRole()==EntityRole::ACTOR){
        mSigMovableObject = new BitsetSignal(static_cast<unsigned char>(SignalID::MOVABLE_OBJECT));
        mSigMovableObject->setName("mSigMovableObject");
        mSignals.push_back(mSigMovableObject);

        mSigBlockedDirection = new BitsetSignal();
        mSignals.push_back(mSigBlockedDirection);

    }else if(mainShapeRole()==EntityRole::MOVABLE_OBJECT){
        mSigBlockedDirection = new BitsetSignal();
        mSignals.push_back(mSigBlockedDirection);

    }


    //if(mMovementEngines.empty()==false){
        //mDirectionSignal = new IntSignal(static_cast<unsigned char>(SignalID::DIRECTION));
        //mSignals.push_back(mDirectionSignal);
    //}


    dbgSystem.removeLastMessage();
    return true;
}


bool Entity::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("Initializing entity '" + mSourceEntity->sourceEntityCfg()->name + "' ...");

    std::vector<std::string> &errorMessages = dbgSystem.errorMessages();

    DbgSystem & tmpDbgSystem = dbgSystem;

    if( mSourceEntity->name() =="fancyPlatformA_light"){
        DummyFunction();
    }

    if(mainShapeRole() != EntityRole::OBJECT_WITHOUT_SHAPE ){

        mBody.reset(new Body());

        Sprite *s = mMapElement.sprite();
        VectorShape *vs = mMapElement.vectorShape();
        SpriteLayer *sl = mMapElement.spriteLayer();

        if(vs){

            Vec2f pos = vs->pathPoints().front();
            Vec2f dPosVertices = -pos;

            if(s){
                // situation when the vector shape is stored inside a composite sprite
                assert(s->type()==SpriteType::COMPOSITE);
                pos = s->absPosition();
                dPosVertices.set(0.0f, 0.0f);
            }

            //assert(mainShapeRole()==EntityRole::PASSABLE_AREA);

            mSourceEntity->sourceBody()->addSourceFixtureFromMapVectorShape(vs, dPosVertices);

            mBody->init(mSourceEntity->sourceBody(), this, pos, 0.0f);


        }else if(s){

            if(mSourceEntity->sourceEntityCfg()->name=="doorA"){
                DummyFunction();
            }

            mBody->init(mSourceEntity->sourceBody(), this, s->position(), s->rotation());


        }else if(sl){

            //if(mSourceEntity->sourceEntityCfg()->entityMapElement==EnumEntityMapElement::SPRITE_LAYER){
                mBody->init(mSourceEntity->sourceBody(), this, {0.0f, 0.0f}, 0.0f);
            //}


        }else{

            assert(false);
        }






        //if(mSprite){
        //    mBody->init(mSourceEntity->sourceBody(), this, mSprite->position(), mSprite->rotation());

        //}else if(mVectorShape){
        //    mBody->init(mSourceEntity->sourceBody(), this, mSprite->position(), mSprite->rotation());

        //}
    }



    for(MovementEngine* be : mMovementEngines){

        if(be->initDataObjectsConnections(_scene, this)==false){
            return false;
        }

        if(be->type()==MovementEngineType::GROUND_MOVEMENT){
            mStatusFlags |= EntityStatusFlags::GROUNDED_ON_START;
        }
    }

    if(mSourceEntity->name()=="hero"){
        DummyFunction();
    }

    //----
    if(mEntityController.get() != nullptr){
        if(mEntityController->initConnections(_scene)==false){
            return false;
        }

        //--- set start state
        LogicState *startState = mEntityController->findChildState(sourceEntity()->sourceEntityCfg()->startState, false);
        if(startState==nullptr){
            //return false;
            startState = mEntityController->childStates().front();
        }

        const std::string direction = mMapElement.parameters().value("eDirection", "NONE");
        mDirection = GetDirectionFromString(direction);

        //mDirection = Direction::LEFT;   // direction must be set before we start movement engine
        //mEntityController->setCurrentState(startState);
        mEntityController->setNextChildState(startState);

        EntityCustomLogicState *stateData = dynamic_cast<EntityCustomLogicState *>(startState->customState());  assert(stateData);
        if(stateData->movementEngineData()){
            startMovementEngine(stateData->movementEngineData());
        }
    }



    //----

    for(TaskEngine *at : mTasks){

        if(at->initDataObjectsConnections(_scene, this)==false){
            return false;
        }

        /*
        if(at->type()==TaskType::POINT_TO_POINT_TRANSPORTER){
            PointToPointMovementTask *t = static_cast<PointToPointMovementTask*>(at);
            for(PointToPointMovementTaskData& d : t->dataObjects()){
                if(d.initConnections(_scene, this)==false){
                    return false;
                }
            }
        }
        */
    }

    //----
    if(mTaskController.get() != nullptr){
        if(mTaskController->initConnections(_scene)==false){
            return false;
        }

        //--- set start state
        LogicState *startState = mTaskController->findChildState(sourceEntity()->sourceEntityCfg()->taskHandlerStartState, false);
        if(startState==nullptr){
            //return false;
            startState = mTaskController->childStates().front();
        }
        //mTaskController->setCurrentState(startState);
        mTaskController->setNextChildState(startState);

        EntityCustomLogicState *stateData = dynamic_cast<EntityCustomLogicState *>(startState->customState());  assert(stateData);
        if(stateData->taskEngineData()){
            startTaskEngine(stateData->taskEngineData());
        }
    }





    /*
    if(mAiController.get() != nullptr){
        if(mAiController->initConnections(_scene)==false){
            return false;
        }
    }
    */


    //----

    //if(mSourceEntity->sourceEntityCfg()->bodyType != EntityBodyType::NONE ){

    //----
    dbgSystem.removeLastMessage();
    return true;

}


void Entity::forceTransformFromSprite()
{

    Sprite *sprite = mMapElement.sprite();
    if(sprite){
        Vec2f posP = sprite->position(Sprite::IncludedPositions::ANI_TRANSLATION);
        float rotationDeg = sprite->rotation(true);
        b2Vec2 posM = gWorldInfo.pixelsToMeters(posP);
        float rotationRad = rotationDeg*b2_pi/180.0f;
        print("posP y=" + std::to_string(posP.y));
        mBody->B2Body()->SetTransform(posM, rotationRad);
    }

}


void Entity::reset()
{

    mBlockedDirections = 0;
    mContactPointsCount = 0;

}


void Entity::start()
{

    if(sourceEntity()->name()=="hero"){
        DummyFunction();
    }

    if(mBody){
        if(mStatusFlags & EntityStatusFlags::GROUNDED_ON_START){
            mBody->B2Body()->SetGravityScale(1.0f);
        }
    }



    //if(mDirection==Direction::NONE){
    //    mDirection = Direction::LEFT;
    //}

    /*
    if(mMovementEngineType == MovementEngineType::UNKNOWN){
        if(mGroundMovementEngine->currentData()==nullptr){
            doGroundMovement(&mGroundMovementEngine->groundMovementDatas().front());
        }
    }
    */

}


bool Entity::startingPhaseUpdate()
{

    preUpdate_resolveContacts();
    mContactPointsCount = 0;        // reset !


    bool doStartingPhase = false;

    //mContactTrigger.preUpdate_postBox2dStep();

    if(mBody){
        if(mStatusFlags & EntityStatusFlags::GROUNDED_ON_START){

            // when an entity settles on the ground its Box2D body enters into the sleep state
            doStartingPhase = mBody->B2Body()->IsAwake();
            if(doStartingPhase==false){

                if(sourceEntity()->name()=="hero"){
                    DummyFunction();
                }


                mBody->B2Body()->SetGravityScale(0.0f);
                //mBody->B2Body()->SetSleepingAllowed(false);
                mStatusFlags |= EntityStatusFlags::STARTED;
            }
        }
    }

    mContactTrigger.postUpdateSignals();

    //--- reset
    //mBlockedDirections = 0;
    //mContactPointsCount = 0;

    return doStartingPhase;

}


void Entity::preUpdate_resolveContacts()
{

    //EngineUpdateParameters eup;

    mEup = EngineUpdateParameters();

    // If body is not awake we skip resolving contacts to retain blocked directions
    if(mBody==nullptr){
        return;
    }
    if(mBody->B2Body()->IsAwake()==false){
        return;
    }

    if(mSourceEntity->name()=="hero"){
        DummyFunction();
    }

    if(mCurrentEngine==nullptr){
        return;
    }


    if(mSourceEntity->name()=="hero"){
        DummyFunction();
    }


    //-----
    b2Vec2 contactPointWithLift{0.0f, 0.0f};
    mLift = nullptr;



    ContactPointInfo *groundContactPointLeft = nullptr;
    ContactPointInfo *groundContactPointRight = nullptr;
    SlidingGroundCfg *slidingGroundCfg = nullptr;
    ConveyorGroundCfg *conveyorGroundCfg = nullptr;
    mBlockedDirections = 0;


    float maxSlopeNormalY = 0.0f;
    if(mCurrentEngine->type()==MovementEngineType::GROUND_MOVEMENT){
        GroundMovementDataBase *dataBase = static_cast<GroundMovementDataBase*>(mCurrentEngine->currentData());
        maxSlopeNormalY = dataBase->yMinNormalForMaxSlope;
    }


    if(mContactPointsCount>0){

        if(mContactPointsCount>1){
            DummyFunction();
        }

        b2Vec2 norm{0.0f, 0.0f};

        for(int i=0; i<mContactPointsCount; i++){
            ContactPointInfo &cp = mContactPoints[i];

            // Normals point from other entity to this entity!

            if(norm.x != 0.0f || norm.y != 0.0f){
                if(norm.x != cp.normal.x || norm.y != cp.normal.y){
                    mEup.contactsWithDifferentGroundNormals = true;
                }
            }
            norm = cp.normal;

            if(cp.normal.y > 0.0f){
                mBlockedDirections |= static_cast<int>(Direction::DOWN);

                if(groundContactPointLeft==nullptr){
                    groundContactPointLeft = &cp;
                }else{
                    if(cp.position.x < groundContactPointLeft->position.x){
                        groundContactPointLeft = &cp;
                    }
                }

                if(groundContactPointRight==nullptr){
                    groundContactPointRight = &cp;
                }else{
                    if(cp.position.x > groundContactPointRight->position.x){
                        groundContactPointRight = &cp;
                    }
                }

            }else if(cp.normal.y < 0.0f){
                mBlockedDirections |= static_cast<int>(Direction::UP);
            }


            FixtureUserData *otherFud = reinterpret_cast<FixtureUserData*>(cp.fixture->GetUserData().pointer);
            Entity *otherEntity = reinterpret_cast<Entity*>(cp.fixture->GetBody()->GetUserData().pointer);

            //if(cp.normal.y < maxSlopeNormal.y){
            //if(cp.normal.y <= maxSlopeNormalY){
            if(cp.normal.y < maxSlopeNormalY){
                if(cp.normal.x>0){
                    mBlockedDirections |= static_cast<int>(Direction::LEFT);
                }else if(cp.normal.x<0){
                    mBlockedDirections |= static_cast<int>(Direction::RIGHT);
                }
            }

            if(otherFud->slidingGroundCfg){
                slidingGroundCfg = otherFud->slidingGroundCfg;
            }

            if(otherFud->conveyorGroundCfg){
                conveyorGroundCfg = otherFud->conveyorGroundCfg;
            }

            /*
            if(otherEntity->mainShapeRole()==EntityRole::TRANSPORTER){
                if(mCurrentEngine->type()==MovementEngineType::GROUND_MOVEMENT){
                    if(cp.normal.y > 0.0f ){         // lift bellow this actor
                        DummyFunction();
                        mLift = otherEntity;
                        contactPointWithLift = cp.position;
                    }
                }

            }
            */

            /*
            if(mCurrentEngine->type()==MovementEngineType::GROUND_MOVEMENT){

                if(otherEntity->mainShapeRole()==EntityRole::TRANSPORTER){
                    if(cp.normal.y > 0.0f ){         // lift bellow this actor
                        DummyFunction();
                        mLift = otherEntity;
                        contactPointWithLift = cp.position;
                    }

                }else if(otherEntity->netoVelocity().x != 0.0f || otherEntity->netoVelocity().y != 0.0f){

                    //situation when other object is moving but is not trnasporter, in example a movable object on a lift and actor standing on the movable object

                    if(mSourceEntity->name()=="hero"){
                        DummyFunction();
                    }

                    if(cp.normal.y > 0.0f ){         // lift bellow this actor
                        DummyFunction();
                        mLift = otherEntity;
                        contactPointWithLift = cp.position;
                    }
                }
            }
            */


            EntityContactSignal *ect = mContactTrigger.findEntityContactTriggerWithEntityB(otherEntity);
            if(ect){
                ect->contactNormalBtoA = cp.normal;
                ect->contactPosition = cp.position;
            }else{
                DummyFunction();
            }

            /*
            //--- check for transporter
            if(mCurrentEngine->type()==MovementEngineType::GROUND_MOVEMENT){

                if(cp.normal.y > 0.0f ){         // other entity is bellow this entity

                    if(otherEntity->mainShapeRole()==EntityRole::TRANSPORTER){
                        mLift = otherEntity;
                        contactPointWithLift = cp.position;

                    }else if(otherEntity->netoVelocity().x != 0.0f || otherEntity->netoVelocity().y != 0.0f){

                        // situation when other entity is not transporter but it is moving
                        // an example is a movable object on a lift and this entity standing on the movable object
                        //   -> the movable object is treated as transporter of this entity
                        mLift = otherEntity;
                        contactPointWithLift = cp.position;
                    }
                }
            }

            if(mainShapeRole()==EntityRole::MOVABLE_OBJECT && otherEntity->mainShapeRole()==EntityRole::MOVABLE_OBJECT){
                //if(mEntityMovingGroup){
                //    print("movable cp.normal x=" + std::to_string(cp.normal.x) + "  y= " +std::to_string(cp.normal.y));
                //}
            }
            */

            /*
            //--- check for movable object
            if(otherEntity->mainShapeRole()==EntityRole::MOVABLE_OBJECT){
                if(mCurrentEngine->type()==MovementEngineType::GROUND_MOVEMENT && otherEntity->currentEngine()->type()==MovementEngineType::GROUND_MOVEMENT){
                    if(cp.normal.x == 1.0f){        // movable object is LEFT of this actor
                        if(mCurrentEngine->direction()==Direction::LEFT){
                            mMovableObject = otherEntity;
                            movableObjectFlags |= MovableObject::ON_LEFT;
                        }

                    }else if(cp.normal.x == -1.0f){     // movable object is RIGHT of this actor
                    //}else if(cp.normal.x < -0.707){     // movable object is RIGHT of this actor
                        if(mCurrentEngine->direction()==Direction::RIGHT){
                            mMovableObject = otherEntity;
                            movableObjectFlags |= MovableObject::ON_RIGHT;
                        }

                    }
                    if(cp.normal.y > 0.0f ){         // other entity is bellow this entity
                        mStatusFlags |= EntityStatusFlags::MOVABLE_OBJECT_BELLOW;
                    }
                }
            }
            */

        }
    }

    //---

    if(groundContactPointLeft && groundContactPointRight){

        //if(mDirection==Direction::LEFT){
        if(mCurrentEngine->velocity().x < 0.0f){
            mEup.groundNormal = groundContactPointLeft->normal;
            //mSlopeNormal = groundContactPointLeft->normal;
            //mGroundMovement->setSlopeNormal(groundContactPointLeft->normal);
            //mSliding->setSlopeNormal(groundContactPointLeft->normal);
            //mPrevContactNormal = groundContactPointLeft->normal;

           // print(" groundContactPoint LEFT pos x=" + std::to_string(groundContactPointLeft->position.x) + " y=" + std::to_string(groundContactPointLeft->position.y) +
           //                           " normal x=" + std::to_string(groundContactPointLeft->normal.x) + " y=" + std::to_string(groundContactPointLeft->normal.y) +
           //                           " body vel x=" + std::to_string(b->GetLinearVelocity().x) + " y=" + std::to_string(b->GetLinearVelocity().y)
           //       );

        //}else if(mDirection==Direction::RIGHT){
        }else if(mCurrentEngine->velocity().x >= 0.0f){
            mEup.groundNormal = groundContactPointRight->normal;
            //mSlopeNormal = groundContactPointRight->normal;
            //mGroundMovement->setSlopeNormal(groundContactPointRight->normal);
            //mSliding->setSlopeNormal(groundContactPointRight->normal);
            //mPrevContactNormal = groundContactPointRight->normal;

            //print(" groundContactPoint RIGHT pos x=" + std::to_string(groundContactPointLeft->position.x) + " y=" + std::to_string(groundContactPointLeft->position.y) +
            //                          " normal x=" + std::to_string(groundContactPointLeft->normal.x) + " y=" + std::to_string(groundContactPointLeft->normal.y) +
            //                           " body vel x=" + std::to_string(b->GetLinearVelocity().x) + " y=" + std::to_string(b->GetLinearVelocity().y)
            //                         );
        }

        //mGroundContact = true;
         mEup.groundContact = true;

    }else{
        if(mCurrentEngine->type()==MovementEngineType::GROUND_MOVEMENT){

            // Check for possible ground contact in situations like downhill or small gap!
            checkGroundContactViaContactSignal();
        }
    }



    /*
    if(mLift){
        mRelVelocity = - mLift->netoVelocity();
        if (mLift->angularVelocity() != 0){

           // m_movingContactPoint stores the world position where we are contacting the platform,
           //  assigned in the PreSolve callback
           // contactVector is our position relative to the axis of rotation of the platform
           b2Vec2 contactVector = contactPointWithLift - mLift->body()->B2Body()->GetPosition();
           b2Vec2 contactAngularVelocity = b2Vec2( -1 * mLift->angularVelocity() * contactVector.y,
                                                        mLift->angularVelocity() * contactVector.x);
           mRelVelocity -= contactAngularVelocity;
        }


    }else{
        if(mSourceEntity->sourceEntityCfg()->name=="hero"){
            DummyFunction();
        }

        mRelVelocity.SetZero();
    }
    */


    if(mCurrentEngine->type()==MovementEngineType::GROUND_MOVEMENT){
        GroundMovementEngineBase *ge = static_cast<GroundMovementEngineBase*>(mCurrentEngine);

        ge->setGroundSliding(slidingGroundCfg);
        ge->setGroundConveyor(conveyorGroundCfg);

        //if(eup.groundNormal.y <= maxSlopeNormalY){
        //    ge->setGroundSteepSlope(true);
        //}
        //print(" Conveyor ground = '" + conveyorGroundCfg->name );
    }


    //---
    /*
    if(mSigMovableObject){
        if(movableObjectFlags==1){
            DummyFunction();
        }
        mSigMovableObject->_setAllFlags(movableObjectFlags);
    }
    */

    if(mSigBlockedDirection){
        mSigBlockedDirection->setValue(mBlockedDirections);
    }

    if((mBlockedDirections & static_cast<int>(Direction::DOWN))==0){
        DummyFunction();
    }


    //---
    mContactPointsCount = 0;        // reset !



    //return eup;

}


void Entity::preUpdate_resolveContacts_part2()
{



    float actorBottomPos = mBody->B2Body()->GetPosition().y - mBody->sourceBody()->centerToBottomDistance();


    unsigned int movableObjectFlags = 0;
    mStatusFlags &= ~EntityStatusFlags::MOVABLE_OBJECT_BELLOW;

    b2Vec2 contactPointWithLift{0.0f, 0.0f};
    mLift = nullptr;


    for(EntityContactSignal *ect : mContactTrigger.contactedEntitiesTriggers()){

        if(ect->contactNormalBtoA.x==0.0f && ect->contactNormalBtoA.y==0.0f)      // contacts with sensors
        {
            continue;
        }

        Entity* otherEntity = ect->mTwoEntitiesContact.entityShapeB().entity;

        if(mCurrentEngine->type()==MovementEngineType::GROUND_MOVEMENT){

            if(otherEntity->mainShapeRole() == EntityRole::MOVABLE_OBJECT && otherEntity->currentEngine()->type() == MovementEngineType::GROUND_MOVEMENT){

                if(ect->contactNormalBtoA.y > 0.0f ){         // other entity is bellow this entity
                    mStatusFlags |= EntityStatusFlags::MOVABLE_OBJECT_BELLOW;
                }

                float otherEntityBottomPos = otherEntity->body()->B2Body()->GetPosition().y - otherEntity->body()->sourceBody()->centerToBottomDistance();
                if(std::abs(actorBottomPos-otherEntityBottomPos)<0.5){

                    //if(ect->contactNormalBtoA.x == 1.0f){        // movable object is LEFT of this actor
                    if(ect->contactNormalBtoA.x > 0.707f){        // movable object is LEFT of this actor
                        if(mCurrentEngine->direction()==Direction::LEFT){
                            mMovableObject = otherEntity;
                            movableObjectFlags |= MovableObject::ON_LEFT;
                        }

                    //}else if(ect->contactNormalBtoA.x == -1.0f){     // movable object is RIGHT of this actor
                    }else if(ect->contactNormalBtoA.x < -0.707){     // movable object is RIGHT of this actor
                        if(mCurrentEngine->direction()==Direction::RIGHT){
                            mMovableObject = otherEntity;
                            movableObjectFlags |= MovableObject::ON_RIGHT;
                        }

                    }
                }else{
                    if(mSourceEntity->sourceEntityCfg()->name=="hero"){
                        DummyFunction();
                    }
                }

            }

            if(ect->contactNormalBtoA.y > 0.0f ){         // other entity is bellow this entity

                if(otherEntity->mainShapeRole()==EntityRole::TRANSPORTER){
                    mLift = otherEntity;
                    contactPointWithLift =  ect->contactPosition;

                }else if(otherEntity->netoVelocity().x != 0.0f || otherEntity->netoVelocity().y != 0.0f){

                    // situation when other entity is not transporter but it is moving
                    // an example is a movable object on a lift and this entity standing on the movable object
                    //   -> the movable object is treated as transporter of this entity
                    mLift = otherEntity;
                    contactPointWithLift = ect->contactPosition;
                }
            }
        }


        if(mCurrentEngine->type()==MovementEngineType::GROUND_MOVEMENT){


        }

        if(mainShapeRole()==EntityRole::MOVABLE_OBJECT && otherEntity->mainShapeRole()==EntityRole::MOVABLE_OBJECT){
            //if(mEntityMovingGroup){
            //    print("movable cp.normal x=" + std::to_string(cp.normal.x) + "  y= " +std::to_string(cp.normal.y));
            //}
        }

    }


    //---
    if(mSigMovableObject){
        if(mSigMovableObject->active(MovableObject::MOVED)){     // this bit is set from other place !
            movableObjectFlags |= MovableObject::MOVED;
        }
        mSigMovableObject->setValue(movableObjectFlags);
    }


    //---
    if(mLift){
        mRelVelocity = - mLift->netoVelocity();
        if (mLift->angularVelocity() != 0){

           // m_movingContactPoint stores the world position where we are contacting the platform,
           //  assigned in the PreSolve callback
           // contactVector is our position relative to the axis of rotation of the platform
           b2Vec2 contactVector = contactPointWithLift - mLift->body()->B2Body()->GetPosition();
           b2Vec2 contactAngularVelocity = b2Vec2( -1 * mLift->angularVelocity() * contactVector.y,
                                                        mLift->angularVelocity() * contactVector.x);
           mRelVelocity -= contactAngularVelocity;
        }

        /*
        b2Body *liftBody = mLift->body()->B2Body();
        if(liftBody->GetLinearVelocity().y != 0.0f){
            DummyFunction();
        }
        mRelVelocity = - liftBody->GetLinearVelocity();
        if (liftBody->GetAngularVelocity() != 0)
        {
           // m_movingContactPoint stores the world position where we are contacting the platform,
           //  assigned in the PreSolve callback
           // contactVector is our position relative to the axis of rotation of the platform
           b2Vec2 contactVector = contactPointWithLift - liftBody->GetPosition();
           b2Vec2 contactAngularVelocity = b2Vec2( -1 * liftBody->GetAngularVelocity() * contactVector.y,
                                                        liftBody->GetAngularVelocity() * contactVector.x);
           mRelVelocity -= contactAngularVelocity;
        }
        */

    }else{
        if(mSourceEntity->sourceEntityCfg()->name=="hero"){
            DummyFunction();
        }

        mRelVelocity.SetZero();
    }


}



void Entity::checkGroundContactViaContactSignal()
{


    /*
    for(auto &f : mGroundSensorFixtures.fixtures()){
        b2Fixture *fixture = f.first;
        FixtureUserData *fud = reinterpret_cast<FixtureUserData*>(fixture->GetUserData().pointer);
        if(fud->blockedDir & static_cast<int>(Direction::DOWN)){
            mBlockedDirections |= static_cast<int>(Direction::DOWN);
            mEup.groundNormal.Set(0.0f, 1.0f);
        }
    }
    */

    for(EntityContactSignal *ect : mContactTrigger.contactedEntitiesTriggers()){
        assert(ect->mTwoEntitiesContact.entityShapeA().entity == this);
        if(ect->mTwoEntitiesContact.entityShapeA().category->role == EntityRole::GROUND_SENSOR){

            Entity *contactedEntity = ect->mTwoEntitiesContact.entityShapeB().entity;
            EntityCategory *contactedCategory = ect->mTwoEntitiesContact.entityShapeB().category;
            if(contactedCategory->shapeRole != EntityCategory::ShapeRole::MAIN_SHAPE) continue;
            if(contactedCategory->sensor) continue;

            for(TwoEntitiesContact::DualFixture &df : ect->mTwoEntitiesContact.dualFixtures()){
                b2Fixture * contactedFixture = df.fixtureB;
                FixtureUserData *fudContacted = reinterpret_cast<FixtureUserData*>(contactedFixture->GetUserData().pointer);

                /*
                if(contactedCategory->mB2BodyType==b2BodyType::b2_dynamicBody){
                    assert(contactedCategory->role==EntityRole::ACTOR || contactedCategory->role==EntityRole::MOVABLE_OBJECT);
                    assert(contactedFixture->GetType()==b2Shape::e_polygon);

                    // for polygon fixtures we don't know blocked direction so we check out if the contacted entity is bellow via geometry
                    float contactedEntityTop = contactedEntity->body()->B2Body()->GetPosition().y - contactedEntity->body()->sourceBody()->centerToBottomDistance();
                    float thisEntityBottom = body()->B2Body()->GetPosition().y + body()->sourceBody()->centerToTopDistance() - 3.0f * b2_linearSlop;

                    if(contactedEntityTop < thisEntityBottom){
                        mBlockedDirections |= static_cast<int>(Direction::DOWN);
                        mEup.groundNormal.Set(0.0f, 1.0f);
                    }

                }else{
                    assert(contactedFixture->GetType()==b2Shape::e_edge || contactedFixture->GetType()==b2Shape::e_chain);
                */
                    if(fudContacted->blockedDir & static_cast<int>(Direction::DOWN)){
                        mBlockedDirections |= static_cast<int>(Direction::DOWN);
                        mEup.groundNormal.Set(0.0f, 1.0f);
                    }

                //}
            }
        }
    }

}





void Entity::preUpdate(UpdateMode &_updateMode)
{

    if(sourceEntity()->name()=="hero"){
        DummyFunction();
    }




    if(mSourceEntity->sourceEntityCfg()->category->mB2BodyType==b2BodyType::b2_dynamicBody){
        preUpdate_resolveContacts_part2();
    }


    if(mTaskController.get()){
        mTaskController->preUpdate(_updateMode);
    }
    if(mEntityController.get()){
        mEntityController->preUpdate(_updateMode);

        if(mEntityController->activeChildState()){
            EntityCustomLogicState *stateData = dynamic_cast<EntityCustomLogicState *>(mEntityController->activeChildState()->customState());  assert(stateData);

            //--- set the current state animation trigger if the animation has stalled
            if(mAnimationPlayer.GetAnimationInstance() && mAnimationPlayer.GetAnimationInstance()==stateData->animationInstance()){
                if(mAnimationPlayer.GetState()==AnimationPlayerState::STALLED){

                    stateData->animationSignal().setValue(false);
                }
            }
        }

    }

    //mContactTrigger.preUpdate_postBox2dStep();
    //Signal::preUpdateSignals(mSignals);
    // mCurrentEngine->preUpdateSignals();
    // mCurrentTaskEngine->preUpdateSignals();

}


void Entity::preUpdate_CheckActorGrouping()
{

    if(mSigMovableObject){
        if(mEntityMovingGroup==nullptr){
            if(mSigMovableObject->activeStarted(MovableObject::MOVED)){
                if(mMovableObject){     // should always be true but there is one frame of delay and maybe mMovableObject can dissapear in that frame
                    mEntityMovingGroup = parentEntitySystem()->entityGroupsManager().storeNewEntityGroup(new MovingMovableObjectGroup());
                    mEntityMovingGroup->setEntities({this, mMovableObject}, this);

                    Direction movableObjectPosition = Direction::NONE;
                    if(mSigMovableObject->active(MovableObject::ON_RIGHT)){
                        movableObjectPosition = Direction::RIGHT;
                    }else if(mSigMovableObject->active(MovableObject::ON_LEFT)){
                        movableObjectPosition = Direction::LEFT;
                    }
                    static_cast<MovingMovableObjectGroup*>(mEntityMovingGroup)->initBehavior(mSigMovableObject, movableObjectPosition);

                }else{
                    mSigMovableObject->setFlags(MovableObject::MOVED, false);
                }
            }
        }
    }

}



void Entity::update_Controller(UpdateMode &_updateMode)
{

    //mUpdateMode = _updateMode;

    if(sourceEntity()->name()=="box"){
        DummyFunction();
    }
    if(sourceEntity()->name()=="hero"){
        DummyFunction();
    }

    b2Body *b = nullptr;

    //EngineUpdateParameters eup;


    if(mBody.get()){
        b = mBody->B2Body();
        //eup = resolveContacts();
        mEup.blockedDirections = mBlockedDirections;
        mEup.timeStep = 1.0f / 60.0f;
        mEup.actorPosM = b->GetPosition();
        mEup.prevEngineVelocity = mEngineVelocity;
        mEup.entity = this;
    }





    //---
    mVelocity.SetZero();
    mAngularVelocity = 0.0f;


    // TASKS
    //----------------------------------------------------------------------------------------

    if(mTaskController.get()){
        mTaskController->update(_updateMode);

        LogicState * nextState = mTaskController->nextChildState();
        if(nextState){
            EntityCustomLogicState *stateData = dynamic_cast<EntityCustomLogicState *>(nextState->customState());  assert(stateData);
            TaskEngineData *data = stateData->taskEngineData();
            if(data){
                startTaskEngine(data);
            }
        }
    }
    if(mCurrentTaskEngine){

        TaskUpdateParameters tup;
        tup.timeStep = mEup.timeStep;

        //if(mCurrentTask->type()==TaskType::POINT_TO_POINT_TRANSPORTER){
        //    PointToPointMovementTask *task = static_cast<PointToPointMovementTask*>(mCurrentTask);
        //    task->update();
        //}

        mCurrentTaskEngine->update(tup);
    }



    // CONTROLLER
    //----------------------------------------------------------------------------------------
    //---
    if(mEntityController.get()){

        mEntityController->update(_updateMode);

        // if engine is going to change we must change it before the call Box2d step!
        // Box2d step and entity update must always be callled with the same active engine! ( NOT Box2D step -> change engine -> update entity )
        LogicState * nextState = mEntityController->nextChildState();
        if(nextState){
            EntityCustomLogicState *stateData = dynamic_cast<EntityCustomLogicState *>(nextState->customState());  assert(stateData);
            MovementEngineData *data = stateData->movementEngineData();
            if(data){
                startMovementEngine(data);
            }
        }

        LogicState * currentState = mEntityController->activeChildState();
        if(currentState){
            if(mSourceEntity->name()=="doorA"){
                DummyFunction();
            }

            EntityCustomLogicState *stateData = dynamic_cast<EntityCustomLogicState *>(currentState->customState());  assert(stateData);
            mCurrentAnimationInstance = stateData->animationInstance();
            if(mCurrentAnimationInstance){
                int aniPlayerFlags = 0;
                if(mAnimationPlayer.GetAnimationInstance() != mCurrentAnimationInstance){
                    aniPlayerFlags = mAnimationPlayer.Play(mCurrentAnimationInstance);
                 }else{
                    aniPlayerFlags = mAnimationPlayer.Update();
                }

                if(aniPlayerFlags & AnimationPlayerFlags::ANIMATED_PROPERTIES_CHANGED){
                    mCurrentAnimationInstance->UpdateAnimatedSprites(true);
                }
            }
        }


    }

}



void Entity::update_Movement(UpdateMode &_updateMode)
{

    if(sourceEntity()->name()=="box"){
        DummyFunction();
    }
    if(sourceEntity()->name()=="hero"){
        DummyFunction();

        if(mSigMovableObject->active(MovableObject::MOVED)){
            print("MovableObject 1");
        }else{
            print("MovableObject   0");
        }

    }



    if(mBody.get()==nullptr){
        return;
    }
    b2Body *b = mBody->B2Body();


    if(mBody.get()){
        b = mBody->B2Body();
        //eup = resolveContacts();
        mEup.blockedDirections = mBlockedDirections;
        mEup.timeStep = 1.0f / 60.0f;
        mEup.actorPosM = b->GetPosition();
        mEup.prevEngineVelocity = mEngineVelocity;
        mEup.entity = this;
    }


    //---
    mVelocity.SetZero();
    mAngularVelocity = 0.0f;



    mStatusFlags &= ~EntityStatusFlags::MOVING_ON_GROUND;

    if(mCurrentEngine){

        mEngineVelocity = mCurrentEngine->update(mEup);
        mDirection = mCurrentEngine->direction();

        if(mEntityMovingGroup && (mStatusFlags & EntityStatusFlags::GROUP_LEADER)==0){
            mEngineVelocity = mEntityMovingGroup->leader()->engineVelocity();
        }

        //if(sourceEntity()->name()=="hero" && mMovableObject){
         //   print("Direction = " +GetDirectionString(mDirection));
        //}

        if(mStatusFlags & EntityStatusFlags::FORCED_ENTITY_TRANSFORMATION){
            return;
        }

        if(mCurrentEngine->type()==MovementEngineType::GROUND_MOVEMENT){


            b2Vec2 pressureVelocity = gWorldInfo.pressureVelocity(mEup.groundNormal);

            if(mEup.groundContact==false)   // situation when there is a sudden downhill slope bellow the actor
            {
                mEngineVelocity.x /= 2.0;        // this hack seems to work well to enusure the body get in touch with ground asap
                mEngineVelocity.y *= 2.0;

                if(std::abs(mEngineVelocity.y) < std::abs(mEngineVelocity.x)/2.0f){
                    //mEngineVelocity.y = - std::abs(mEngineVelocity.x)/2.0f;
                    pressureVelocity.y *= 2.0f;
                }
             }

            //mVelocity = mEngineVelocity + mPressureVelocity + mRelVelocity;
            mVelocity = mEngineVelocity + pressureVelocity - mRelVelocity;


            if(mEngineVelocity.x != 0.0f || mEngineVelocity.y != 0.0f){
                mStatusFlags |= EntityStatusFlags::MOVING_ON_GROUND;
            }





            if(sourceEntity()->name()=="hero"){
                //print("Hero mEngineVelocity x=" + std::to_string(mEngineVelocity.x));
                //b2Vec2 bodyVel = b->GetLinearVelocity();
                //print("Hero bodyVel x=" + std::to_string(bodyVel.x));

                /*
                print("Hero mRelVelocity y=" + std::to_string(mRelVelocity.y));

                if(mLift){
                    print("Lift velocity y=" + std::to_string(mLift->body()->B2Body()->GetLinearVelocity().y));
                }else{
                    print("No Lift!");
                }
                */
            }


            //if(mMovableObject){
            //    mMovableObject->currentEngine()->setExtraVelocity(mVelocity);
            //}

            //float speed = std::sqrt(b->GetLinearVelocity().x*b->GetLinearVelocity().x + b->GetLinearVelocity().y*b->GetLinearVelocity().y);

            //print("moveEngineVelocity x=" + std::to_string(moveEngineVelocity.x) +" y=" + std::to_string(moveEngineVelocity.y) +
            //      "  pressureVelocity x=" + std::to_string(mPressureVelocity.x) +" y=" + std::to_string(mPressureVelocity.y));
            //print("body speed =" + std::to_string(speed));


        }else if(mCurrentEngine->type()==MovementEngineType::JUMP_MOVEMENT){

            mVelocity = mEngineVelocity;

        }else if(mCurrentEngine->type()==MovementEngineType::PATH_POINT_TO_POINT_MOVEMENT){
            //PathPointToPointMovementEngine * ptpEngine = static_cast<PathPointToPointMovementEngine*>(mCurrentEngine);

            //b2Vec2 currentPos = ptpEngine->currentPosition();
            //b2Vec2 bodyPos = b->GetPosition();
            //float dist = b2Distance(currentPos, bodyPos);
            //print("currentPos x=" + std::to_string(currentPos.x) +"  y="+ std::to_string(currentPos.y)+"  bodyPos x=" + std::to_string(bodyPos.x)+" y="+ std::to_string(bodyPos.y) +"  dist=" + std::to_string(dist));

            //if(ptpEngine->state()==PathPTPMovementState::STANDING && ptpEngine->stateEllapsedTimeS()==0.0f){
            if(mCurrentEngine->fixedPositionSet()){

                //b2Vec2 currentPoint = ptpEngine->currentPathPoint();
                b2Vec2 currentPoint = mCurrentEngine->fixedPosition();

                //b2Vec2 bodyPos = b->GetPosition();
                //b2Vec2 bodyVel = b->GetLinearVelocity();
                //float dist = b2Distance(currentPoint, bodyPos);
                //print("----- currentPoint x=" + std::to_string(currentPoint.x) +"  y="+ std::to_string(currentPoint.y)+" dist=" + std::to_string(dist)+"  bodyVel x=" + std::to_string(bodyVel.x)+" y="+ std::to_string(bodyVel.y));
                //print("currentPoint x=" + std::to_string(currentPoint.x) +"  y="+ std::to_string(currentPoint.y)+
                //      " bodyPos x=" + std::to_string(bodyPos.x) +"  y="+ std::to_string(bodyPos.y)+
                //      " bodyVel x=" + std::to_string(bodyVel.x)+" y="+ std::to_string(bodyVel.y));

                if(mMapElement.sprite()->name() == "dbgRotator"){
                    DummyFunction();
                }

                /*
                float angle = ptpEngine->angularVelocityGenerator().currentAngle();
                angle *= b2_pi / 180.f;
                */
                float angle = b->GetAngle();

                b->SetTransform(currentPoint, angle);
            }

            mVelocity = mEngineVelocity;
            mAngularVelocity = mCurrentEngine->angularVelocity();


        }else if(mCurrentEngine->type()==MovementEngineType::PATHWAY_MOVEMENT){

            /*
            PathMovementEngine * pathEngine = static_cast<PathMovementEngine*>(mCurrentEngine);

            b2Vec2 currentPos = pathEngine->currentPositionM();
            b2Vec2 bodyPos = b->GetPosition();
            b2Vec2 bodyContactPos(bodyPos.x + pathEngine->currentData()->actorPathwayContactOffsetM.x, bodyPos.y + pathEngine->currentData()->actorPathwayContactOffsetM.y);
            //float dist = b2Distance(currentPos, bodyPos);
            //print("currentPos x=" + std::to_string(currentPos.x) +"  y="+ std::to_string(currentPos.y)+"  bodyPos x=" + std::to_string(bodyPos.x)+" y="+ std::to_string(bodyPos.y) +"  dist=" + std::to_string(dist));

            if(pathEngine->state()==PathMovementState::STANDING && pathEngine->stateEllapsedTimeS()==0.0f){

                b2Vec2 currentPos = pathEngine->currentPositionM();

                //b2Vec2 currentPoint = ptpEngine->currentPathPoint();
                b2Vec2 bodyPos = b->GetPosition();
                b2Vec2 bodyVel = b->GetLinearVelocity();
                //float dist = b2Distance(currentPoint, bodyPos);
                //print("----- currentPoint x=" + std::to_string(currentPoint.x) +"  y="+ std::to_string(currentPoint.y)+" dist=" + std::to_string(dist)+"  bodyVel x=" + std::to_string(bodyVel.x)+" y="+ std::to_string(bodyVel.y));
                //print("currentPoint x=" + std::to_string(currentPoint.x) +"  y="+ std::to_string(currentPoint.y)+
                //      " bodyPos x=" + std::to_string(bodyPos.x) +"  y="+ std::to_string(bodyPos.y)+
                //      " bodyVel x=" + std::to_string(bodyVel.x)+" y="+ std::to_string(bodyVel.y));

                //b->SetTransform(currentPos, 0.0f);
            }

            b2Vec2 bodyVel = b->GetLinearVelocity();
            //float dist = b2Distance(currentPoint, bodyPos);

            if(pathEngine->state()==PathMovementState::MOVING){
                print("engineVelocity x=" + std::to_string(mEngineVelocity.x) +"  y="+ std::to_string(mEngineVelocity.y)+
                      " currentPos x=" + std::to_string(currentPos.x) +"  y="+ std::to_string(currentPos.y)+
                      " bodyContactPos x=" + std::to_string(bodyContactPos.x) +"  y="+ std::to_string(bodyContactPos.y)+
                      " bodyVel x=" + std::to_string(bodyVel.x)+" y="+ std::to_string(bodyVel.y));

            }
            */


            mVelocity = mEngineVelocity;

        }else if(mCurrentEngine->type()==MovementEngineType::ANIMATED_BEHAVIOR){

            //AnimatedBehaviorEngine *engine = static_cast<AnimatedBehaviorEngine*>(mCurrentEngine);

            mVelocity = mEngineVelocity;

        }
    }


    if(mMapElement.sprite() && mMapElement.sprite()->name() == "dbgRotator"){
        //print("angular velocity = " + std::to_string(mAngularVelocity));
    }



    /*
    if(sourceEntity()->name()=="player"){
       print("");
       print("prev body velocity x=" + std::to_string(b->GetLinearVelocity().x) + " y=" + std::to_string(b->GetLinearVelocity().y));
       print("prev body pos x=" + std::to_string(b->GetTransform().p.x) + " y=" + std::to_string(b->GetTransform().p.y));
       print("engine velocity x=" + std::to_string(mEngineVelocity.x) + " y=" + std::to_string(mEngineVelocity.y));
       print("velocity x=" + std::to_string(mVelocity.x) + " y=" + std::to_string(mVelocity.y));
       print("ground normal x=" + std::to_string(eup.groundNormal.x)+" y="+ std::to_string(eup.groundNormal.y));
    }
    */

    b->SetLinearVelocity(mVelocity);
    b->SetAngularVelocity(mAngularVelocity);

    /*
    if(mainShapeRole()==EntityRole::MOVABLE_OBJECT){
        if(mVelocity.x==0.0f && mVelocity.y==0.0f && mAngularVelocity==0.0f){
            b->SetType(b2BodyType::b2_kinematicBody);
        }else{
            b->SetType(b2BodyType::b2_dynamicBody);
        }
    }
    */

    //---
    //mOverlappedEntitiesStart.clear();
    //mOverlappedEntitiesEnd.clear();


}



void Entity::postUpdate(UpdateMode &_updateMode)
{

    if(mTaskController.get()){
        mTaskController->postUpdate(_updateMode);
    }
    if(mEntityController.get()){
        mEntityController->postUpdate(_updateMode);

        /*
        // if engine is going to change we must change it before the call Box2d step!
        // Box2d step and entity update must always be callled with the same active engine! ( NOT Box2D step -> change engine -> update entity )
        BehaviorState * nextState = mEnginesController->nextState();
        if(nextState){
            MovementEngineData *data = nextState->movementEngineData();
            if(data){
                startMovementEngine(data);
            }
        }
        */
    }

    //---
    mContactTrigger.postUpdateSignals();
    //Signal::postUpdateSignals(mSignals);
    //mCurrentEngine->postUpdateSignals();
    //mCurrentTaskEngine->postUpdateSignals();
}



void Entity::updateSpriteTransform()
{

    if(mBody==nullptr) return;

    if(mStatusFlags & EntityStatusFlags::FORCED_ENTITY_TRANSFORMATION){
        return;
    }


    b2Body *b = mBody->B2Body();


    //print("body position x=" + std::to_string(b->GetPosition().x) + " y=" + std::to_string(b->GetPosition().y));
    //print("Actor '" + mSourceEntity->name() + "' velocity x=" + std::to_string(b->GetLinearVelocity().x) + "  y=" + std::to_string(b->GetLinearVelocity().y));

    Vec2f posP = gWorldInfo.metersToPixels<Vec2f>(mBody->B2Body()->GetPosition());

    if(mainShapeRole()==EntityRole::PASSABLE_AREA){
        //return;
        DummyFunction();
    }

    mMapElement.setPositionFromBody(gWorldInfo.metersToPixels<Vec2f>(mBody->B2Body()->GetPosition()));
    mMapElement.setRotationFromBody(mBody->B2Body()->GetAngle()*180.0f/b2_pi);

    //mSprite->setPosition(gWorldInfo.metersToPixels<Vec2f>(mBody->B2Body()->GetPosition()));
    // mSprite->setRotation(mBody->B2Body()->GetAngle()*180.0f/b2_pi);

    //print("body pix position x=" + std::to_string(mSprite->position().x) + " y=" + std::to_string(mSprite->position().y));

}


bool Entity::startMovementEngine(MovementEngineData *_movementEngineData)
{

    if(mCurrentEngine->currentData()==_movementEngineData){
        return false;
    }

    MovementEngine *nextEngine = getMovementEngine(_movementEngineData->factory);
    if(nextEngine->start(_movementEngineData)==false){
        return false;
    }
    mCurrentEngine->resetSignals();

    mCurrentEngine = nextEngine;
    mDirection = mCurrentEngine->direction();

    return true;
}


bool Entity::startTaskEngine(TaskEngineData *_taskEngineData)
{

    if(mCurrentTaskEngine->currentData()==_taskEngineData){
        return false;
    }

    TaskEngine *nextTaskEngine = getTaskEngine(_taskEngineData->factory);
    if(nextTaskEngine->start(_taskEngineData)==false){
        return false;
    }
    //mCurrentTask->resetSignals();

    mCurrentTaskEngine = nextTaskEngine;

    return true;

}



MovementEngine* Entity::getMovementEngine(MovementEngineFactory * _factory)
{

    for(MovementEngine* be : mMovementEngines){
        if(be->factory()==_factory){
            return be;
        }
    }

    return nullptr;
}


MovementEngineData* Entity::getMovementEngineData(const std::string &_name)
{

    for(MovementEngine* be : mMovementEngines){
        MovementEngineData *data = be->getMovementEngineData(_name, false);
        if(data){
            return data;
        }
    }

    return nullptr;
}


TaskEngine* Entity::getTaskEngine(TaskEngineFactory * _factory)
{

    for(TaskEngine* te : mTasks){
        if(te->factory()==_factory){
            return te;
        }
    }

    return nullptr;
}


TaskEngineData* Entity::getTaskData(const std::string &_name)
{

    for(TaskEngine* be : mTasks){
        TaskEngineData *data = be->getData(_name, false);
        if(data){
            return data;
        }
    }

    return nullptr;
}


/*
void Entity::obtainSignal_signalQuery(SignalQuery &_signalQuery, const std::string &_path, const std::string &stateMovementEngineCfg, bool _setErrorMessage)
{


    EntitySignalStrings ess;


    if(ess.parse(_path)==false){
        return;
    }



    //---
    if(ess.engineCfgName.empty()==false){
        //signals stored in engine

        MovementEngine *behaviorEngine = _obtainEngineForSignal(ess.engineCfgName, ess.signalName);
        if(behaviorEngine==nullptr){
            return;
        }
        behaviorEngine->obtainSignal_signalQuery(_signalQuery, ess, _setErrorMessage);

        return;
    }

    if(ess.taskCfgName.empty()==false){
        //signals stored in task engine

        TaskEngine *taskEngine = _obtainTaskEngineForSignal(ess.taskCfgName, ess.signalName);
        if(taskEngine==nullptr){
            return;
        }
        taskEngine->obtainSignal_signalQuery(_signalQuery, ess, _setErrorMessage);

        return;
    }

    // signals stored in entity

    if(ess.signalName=="MOVABLE_OBJECT"){
        if(mSigMovableObject){
            _signalQuery.mSignal = mSigMovableObject;

            _signalQuery.mIntValue = GetMovableObjectFromString_signalQuery(ess.signalValue);
            if(_signalQuery.mIntValue == MovableObject::UNKNOWN){
                dbgSystem.addMessage("Unknown movable object value '" + ess.signalValue +" ' !");
                return;
            }


        }else{
            dbgSystem.addMessage("Signal '"+ ess.signalName+ " ' can used only for entities with 'ACTOR' role!");
            return;
        }

    }else if(ess.signalName=="BLOCKED_DIRECTION"){
         if(mSigBlockedDirection){
             _signalQuery.mSignal = mSigBlockedDirection;

             _signalQuery.mIntValue = static_cast<int>(GetDirectionFromString(ess.signalValue));
             if(_signalQuery.mIntValue == static_cast<int>(Direction::NONE)){
                 dbgSystem.addMessage("Unknown direction value '" + ess.signalValue +" ' !");
                 return;
             }

         }else{
             dbgSystem.addMessage("Signal '"+ ess.signalName+ " ' can used only for entities with dynamic body (actor, movable object) !");
             return;
         }

    }

    if(_signalQuery.mSignal==nullptr &&_setErrorMessage){
        dbgSystem.addMessage("Get signal '" + ess.signalName + "' error! The signal is unknown!");
    }
}


void Entity::obtainSignal_signalSetter(SignalSetter &_signalSetter, const std::string &_path, const std::string &stateMovementEngineCfg, bool _setErrorMessage)
{

    EntitySignalStrings ess;
    if(ess.parse(_path)==false){
        return;
    }

    //---
    if(ess.engineCfgName.empty()==false){

        //signals stored in behavior engine
        MovementEngine *engine = _obtainEngineForSignal(ess.engineCfgName, ess.signalName);
        if(engine==nullptr){
            return;
        }
        engine->obtainSignal_signalSetter(_signalSetter, ess, _setErrorMessage);

        return;
    }

    if(ess.taskCfgName.empty()==false){

        //signals stored in task engine
        TaskEngine *taskEngine = _obtainTaskEngineForSignal(ess.taskCfgName, ess.signalName);
        if(taskEngine==nullptr){
            return;
        }
        taskEngine->obtainSignal_signalSetter(_signalSetter, ess, _setErrorMessage);

        return;
    }


    // signals stroed in entity
    if(ess.signalName=="MOVABLE_OBJECT"){
        if(mSigMovableObject){
            _signalSetter.mSignal = mSigMovableObject;

            _signalSetter.mIntValue = GetMovableObjectFromString_signalSetter(ess.signalValue);
            if(_signalSetter.mIntValue == MovableObject::UNKNOWN){
                dbgSystem.addMessage("Unknown movable object value '" + ess.signalValue +" ' !");
                return;
            }

            if(ess.getBoolValue(_signalSetter.mBoolValue)==false){
                return;
            }

        }else{

            dbgSystem.addMessage("Signal '"+ ess.signalName+ " ' can used only for entities with 'ACTOR' role!");
            return;
        }

    }

    if(_signalSetter.mSignal==nullptr &&_setErrorMessage){
        dbgSystem.addMessage("Get signal '" + ess.signalName + "' error! The signal is unknown or not available for setting it!");
    }

}
*/



void Entity::obtainSignal_signalQuery(SignalQuery &_signalQuery, ParsedSignalPath &_psp, bool _setErrorMessage)
{


    //EntitySignalStrings ess;


    //if(ess.parse(_path)==false){
    //    return;
    //}



    //---
    if(_psp.signalNamePartAt(0)=="ENGINE"){
        //signals stored in engine

        MovementEngine *behaviorEngine = _obtainEngineForSignal(_psp.signalNamePartAt(1), _psp.signalFullName());
        if(behaviorEngine==nullptr){
            return;
        }
        behaviorEngine->obtainSignal_signalQuery(_signalQuery, _psp, _setErrorMessage);

        return;
    }

    if(_psp.signalNamePartAt(0)=="TASK"){
        //signals stored in task engine

        TaskEngine *taskEngine = _obtainTaskEngineForSignal(_psp.signalNamePartAt(1), _psp.signalFullName());
        if(taskEngine==nullptr){
            return;
        }
        taskEngine->obtainSignal_signalQuery(_signalQuery, _psp, _setErrorMessage);

        return;
    }

    // signals stored in entity

    if(_psp.signalNamePartAt(0)=="MOVABLE_OBJECT"){
        if(mSigMovableObject){
            _psp.obtainValue(_signalQuery, mSigMovableObject, &gMovableObjectNamedValues_query);

            //_signalQuery.mSignal = mSigMovableObject;

            //_signalQuery.mIntValue = GetMovableObjectFromString_signalQuery(ess.signalValue);
            //if(_signalQuery.mIntValue == MovableObject::UNKNOWN){
            //    dbgSystem.addMessage("Unknown movable object value '" + ess.signalValue +" ' !");
            //    return;
            //}


        }else{
            dbgSystem.addMessage("Signal '"+ _psp.signalFullName() + " ' can used only for entities with 'ACTOR' role!");
            return;
        }

    }else if(_psp.signalNamePartAt(0)=="BLOCKED_DIRECTION"){
         if(mSigBlockedDirection){
             _psp.obtainValue(_signalQuery, mSigBlockedDirection, &gDirectionNamedValues);

             //_signalQuery.mSignal = mSigBlockedDirection;

             //_signalQuery.mIntValue = static_cast<int>(GetDirectionFromString(ess.signalValue));
             //if(_signalQuery.mIntValue == static_cast<int>(Direction::NONE)){
             //    dbgSystem.addMessage("Unknown direction value '" + ess.signalValue +" ' !");
             //    return;
             //}

         }else{
             dbgSystem.addMessage("Signal '"+ _psp.signalFullName()+ " ' can used only for entities with dynamic body (actor, movable object) !");
             return;
         }

    }


    if(_signalQuery.mSignal==nullptr &&_setErrorMessage){
        dbgSystem.addMessage("Get signal '" + _psp.signalFullName() + "' error! The signal is unknown!");
    }
}


void Entity::obtainSignal_signalSetter(SignalSetter &_signalSetter, ParsedSignalPath &_psp, bool _setErrorMessage)
{

    //EntitySignalStrings ess;
    //if(ess.parse(_path)==false){
    //    return;
    //}

    //---
    if(_psp.signalNamePartAt(0)=="ENGINE"){

        //signals stored in behavior engine
        MovementEngine *behaviorEngine = _obtainEngineForSignal(_psp.signalNamePartAt(1), _psp.signalFullName());
        if(behaviorEngine==nullptr){
            return;
        }
        behaviorEngine->obtainSignal_signalSetter(_signalSetter, _psp, _setErrorMessage);

        return;
    }

    if(_psp.signalNamePartAt(0)=="TASK"){

        //signals stored in task engine
        TaskEngine *taskEngine = _obtainTaskEngineForSignal(_psp.signalNamePartAt(1), _psp.signalFullName());
        if(taskEngine==nullptr){
            return;
        }
        taskEngine->obtainSignal_signalSetter(_signalSetter, _psp, _setErrorMessage);

        return;
    }


    // signals stroed in entity
    if(_psp.signalNamePartAt(0)=="MOVABLE_OBJECT"){
        if(mSigMovableObject){
            _psp.obtainValue(_signalSetter, mSigMovableObject, &gMovableObjectNamedValues_setter);

            //_signalSetter.mSignal = mSigMovableObject;

            //_signalSetter.mIntValue = GetMovableObjectFromString_signalSetter(ess.signalValue);
            //if(_signalSetter.mIntValue == MovableObject::UNKNOWN){
            //    dbgSystem.addMessage("Unknown movable object value '" + ess.signalValue +" ' !");
            //    return;
            //}

            //if(ess.getBoolValue(_signalSetter.mBoolValue)==false){
            //    return;
            //}

        }else{

            dbgSystem.addMessage("Signal '"+ _psp.signalFullName() + " ' can used only for entities with 'ACTOR' role!");
            return;
        }

    }

    if(_signalSetter.mSignal==nullptr &&_setErrorMessage){
        dbgSystem.addMessage("Get signal '" + _psp.signalFullName() + "' error! The signal is unknown or not available for setting it!");
    }

}





MovementEngine* Entity::_obtainEngineForSignal(const std::string &engineCfgName, const std::string &signalName)
{


    MovementEngine *engine = nullptr;
    MovementEngineData *engineData = nullptr;

    for(MovementEngine* be : mMovementEngines){
        engineData = be->getMovementEngineData(engineCfgName, false);
        if(engineData){
            engine = be;
            break;
        }
    }

    if(engine==nullptr){
        dbgSystem.addMessage("Get signal '" + signalName + "' error!  The behavior engine cfg with name '" + engineCfgName + "' not found !");
        return nullptr;
    }

    return engine;

}


TaskEngine* Entity::_obtainTaskEngineForSignal(const std::string &engineCfgName, const std::string &signalName)
{

    TaskEngine *engine = nullptr;
    TaskEngineData *engineData = nullptr;

    for(TaskEngine* te : mTasks){
        engineData = te->getData(engineCfgName, false);
        if(engineData){
            engine = te;
            break;
        }
    }

    if(engine==nullptr){
        dbgSystem.addMessage("Get signal '" + signalName + "' error!  The task engine cfg with name '" + engineCfgName + "' not found !");
        return nullptr;
    }

    return engine;

}


}
