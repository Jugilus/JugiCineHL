#include <chrono>
#include <assert.h>
#include <sstream>
#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmMap.h"
#include "jmCamera.h"
#include "jmSceneLayout.h"
#include "jmCommonFunctions.h"
#include "jmGuiText.h"
#include "jmTextSprite.h"
#include "jmUtilities.h"

#include "items/jpItemsCommon.h"
#include "jpPlayedApp.h"
#include "jpPlayedScene.h"
#include "jpItemsTable.h"
#include "jpUtilities.h"
#include "jpActionsCommon.h"

#include "movements/jpMovementBase.h"
#include "jpB2Body.h"
#include "jpSourceEntity.h"
#include "jpEntity.h"
#include "jpB2World.h"
#include "jpB2Callbacks.h"



namespace jugimap{





void EntityContactListener::BeginContact(b2Contact* contact)
{

    //if(contact->IsTouching()==false){
    //    return;
    //}

    mFixtureA = contact->GetFixtureA();
    mFixtureB = contact->GetFixtureB();

    int childIndexA = contact->GetChildIndexA();
    int childIndexB = contact->GetChildIndexB();

    b2Shape* mShapeA = mFixtureA->GetShape();
    b2Shape* mShapeB = mFixtureB->GetShape();

    assert(mFixtureA->GetUserData().pointer);
    assert(mFixtureB->GetUserData().pointer);
    FixtureUserData *fudA = reinterpret_cast<FixtureUserData *>(mFixtureA->GetUserData().pointer);
    FixtureUserData *fudB = reinterpret_cast<FixtureUserData *>(mFixtureB->GetUserData().pointer);
    Entity *entityA = reinterpret_cast<Entity *>(mFixtureA->GetBody()->GetUserData().pointer);
    Entity *entityB = reinterpret_cast<Entity *>(mFixtureB->GetBody()->GetUserData().pointer);


    if(contact->IsTouching()==false){
        //return;
    }

    //if(entityA->type()==EntityType::BLOCKING_ENVIRONMENT && entityB->type()==EntityType::ENVIRONMENT_OBJECT){
    //    DummyFunction();
    //}
    //if(entityB->type()==EntityType::ENVIRONMENT_OBJECT && entityA->type()==EntityType::BLOCKING_ENVIRONMENT){
   //     DummyFunction();
    //}


    //if(entityA->type()==EntityType::ACTOR){
    if(entityA->mainShapeRole()==EntityRole::ACTOR || entityA->mainShapeRole()==EntityRole::MOVABLE_OBJECT){
        mActor = entityA;
        mActorFixture = mFixtureA;
        mActorFixtureChildIndex = childIndexA;

        mEntity = entityB;
        mEntityFixture = mFixtureB;
        mEntityFixtureChildIndex = childIndexB;

    //}else if(entityB->type()==EntityType::ACTOR){
    }else if(entityB->mainShapeRole()==EntityRole::ACTOR || entityB->mainShapeRole()==EntityRole::MOVABLE_OBJECT){
        mActor = entityB;
        mActorFixture = mFixtureB;
        mActorFixtureChildIndex = childIndexB;

        mEntity = entityA;
        mEntityFixture = mFixtureA;
        mEntityFixtureChildIndex = childIndexA;

    }else{
        assert(false);
    }

    _beginContact(contact);


    /*

    if(fudA->role == FixtureRole::BLOCKING_ENVIRONMENT && fudB->role == FixtureRole::ACTOR_GROUND_SENSOR){

        assert(entityB->type()==EntityType::ACTOR);
        Actor *a = static_cast<Actor*>(entityB);
        //EBehavior_Player_Gravity *b = static_cast<EBehavior_Player_Gravity*>(a->behavior());

        //print("Begin contact between the GROUND SENSOR of entity '" + a->name() + "' and a blocking sprite '" + fudA->sprite->mSourceSprite->mName + "'");
        a->addGroundSensorFixture(mFixtureA, childIndexA);

    }else if(fudA->role == FixtureRole::ACTOR_GROUND_SENSOR && fudB->role == FixtureRole::BLOCKING_ENVIRONMENT){

        assert(entityA->type()==EntityType::ACTOR);
        Actor *a = static_cast<Actor*>(entityA);
        //EBehavior_Player_Gravity *b = static_cast<EBehavior_Player_Gravity*>(a->behavior());

        //print("Begin contact between the GROUND SENSOR of entity '" + e->name() + "' and a blocking sprite '" + fudB->sprite->mSourceSprite->mName + "'");
        a->addGroundSensorFixture(mFixtureB, childIndexB);


    }else if(fudA->role == FixtureRole::PATHWAY_SENSOR && fudB->role == FixtureRole::ACTOR_BODY){

        assert(entityB->type()==EntityType::ACTOR);
        Actor *a = static_cast<Actor*>(entityB);
        //EBehavior_Player_Gravity *b = static_cast<EBehavior_Player_Gravity*>(a->behavior());
        assert(entityA->type()==EntityType::PATHWAY);
        EPathway *pathway = static_cast<EPathway*>(entityA);

        print("Begin contact between actor '" + a->sourceEntity()->name() + "' and pathway '" + entityA->sourceEntity()->name()+ "'");

        a->addOverlappedEntity(pathway);

    }else if(fudA->role == FixtureRole::ACTOR_BODY && fudB->role == FixtureRole::PATHWAY_SENSOR){

        assert(entityA->type()==EntityType::ACTOR);
        Actor *a = static_cast<Actor*>(entityA);
        //EBehavior_Player_Gravity *b = static_cast<EBehavior_Player_Gravity*>(a->behavior());
        assert(entityB->type()==EntityType::PATHWAY);
        EPathway *pathway = static_cast<EPathway*>(entityB);

        print("Begin contact between actor '" + a->sourceEntity()->name() + "' and pathway '" + entityB->sourceEntity()->name()+ "'");

        a->addOverlappedEntity(pathway);
    }

    */

}


void EntityContactListener::_beginContact(b2Contact* contact)
{

    FixtureUserData *actorFUD = reinterpret_cast<FixtureUserData *>(mActorFixture->GetUserData().pointer);
    FixtureUserData *entityFUD = reinterpret_cast<FixtureUserData *>(mEntityFixture->GetUserData().pointer);


    //if(actorFUD->category->role==EntityRole::GROUND_SENSOR){
    //    if(mEntityFixture->IsSensor()==false){      //every solid object is registered
    //        mActor->groundSensorFixtures().addFixture(mEntityFixture, mEntityFixtureChildIndex);
    //    }
    //}


    mActor->contactTrigger().onBeginContact(EntityShape(mActor, actorFUD->category),  mActorFixture, mActorFixtureChildIndex,
                                            EntityShape(mEntity, entityFUD->category), mEntityFixture, mEntityFixtureChildIndex);

    mEntity->contactTrigger().onBeginContact(EntityShape(mEntity, entityFUD->category), mEntityFixture, mEntityFixtureChildIndex,
                                             EntityShape(mActor, actorFUD->category), mActorFixture, mActorFixtureChildIndex);

}


void EntityContactListener::EndContact(b2Contact* contact)
{

    mFixtureA = contact->GetFixtureA();
    mFixtureB = contact->GetFixtureB();
    int childIndexA = contact->GetChildIndexA();
    int childIndexB = contact->GetChildIndexB();
    assert(mFixtureA->GetUserData().pointer);
    assert(mFixtureB->GetUserData().pointer);
    const FixtureUserData *fudA = reinterpret_cast<FixtureUserData *>(mFixtureA->GetUserData().pointer);
    const FixtureUserData *fudB = reinterpret_cast<FixtureUserData *>(mFixtureB->GetUserData().pointer);
    Entity *entityA = reinterpret_cast<Entity *>(mFixtureA->GetBody()->GetUserData().pointer);
    Entity *entityB = reinterpret_cast<Entity *>(mFixtureB->GetBody()->GetUserData().pointer);

    if(contact->IsTouching()==false){
    //    return;
    }

    //if(entityA->type()==EntityType::ACTOR){
    if(entityA->mainShapeRole()==EntityRole::ACTOR || entityA->mainShapeRole()==EntityRole::MOVABLE_OBJECT){
        mActor = entityA;
        mActorFixture = mFixtureA;
        mActorFixtureChildIndex = childIndexA;

        mEntity = entityB;
        mEntityFixture = mFixtureB;
        mEntityFixtureChildIndex = childIndexB;

    //}else if(entityB->type()==EntityType::ACTOR){
    }else if(entityB->mainShapeRole()==EntityRole::ACTOR || entityB->mainShapeRole()==EntityRole::MOVABLE_OBJECT){

        mActor = entityB;
        mActorFixture = mFixtureB;
        mActorFixtureChildIndex = childIndexB;

        mEntity = entityA;
        mEntityFixture = mFixtureA;
        mEntityFixtureChildIndex = childIndexA;

    }else{
        assert(false);
    }

    _endContact(contact);


    /*

    if(fudA->role == FixtureRole::BLOCKING_ENVIRONMENT && fudB->role == FixtureRole::ACTOR_GROUND_SENSOR){

        assert(entityB->type()==EntityType::ACTOR);
        Actor *a = static_cast<Actor*>(entityB);
        //EBehavior_Player_Gravity *b = static_cast<EBehavior_Player_Gravity*>(a->behavior());
        //print("End contact between the GROUND SENSOR of entity '" + e->name() + "' and a blocking sprite '" + fudA->sprite->mSourceSprite->mName + "'");

        a->removeGroundSensorFixture(mFixtureA, childIndexA);

        if(a->holeSensorFixtureIds().empty() && a->movementEngineType()==BehaviorEngineType::GROUND_MOVEMENT){
            a->onNoGroundBeneathTheFeet();
        }

    }else if(fudA->role == FixtureRole::ACTOR_GROUND_SENSOR && fudB->role == FixtureRole::BLOCKING_ENVIRONMENT){

        assert(entityA->type()==EntityType::ACTOR);
        Actor *a = static_cast<Actor*>(entityA);
        //EBehavior_Player_Gravity *b = static_cast<EBehavior_Player_Gravity*>(a->behavior());

        //print("End contact between the GROUND SENSOR of entity '" + e->name() + "' and a blocking sprite '" + fudB->sprite->mSourceSprite->mName + "'");

        a->removeGroundSensorFixture(mFixtureB, childIndexB);

        if(a->holeSensorFixtureIds().empty() && a->movementEngineType()==BehaviorEngineType::GROUND_MOVEMENT){
            a->onNoGroundBeneathTheFeet();
        }

    }else if(fudA->role == FixtureRole::PATHWAY_SENSOR && fudB->role == FixtureRole::ACTOR_BODY){

        assert(entityB->type()==EntityType::ACTOR);
        Actor *a = static_cast<Actor*>(entityB);
        //EBehavior_Player_Gravity *b = static_cast<EBehavior_Player_Gravity*>(a->behavior());
        assert(entityA->type()==EntityType::PATHWAY);
        EPathway *pathway = static_cast<EPathway*>(entityA);

        print("End contact between actor '" + a->sourceEntity()->name() + "' and pathway '" + entityA->sourceEntity()->name()+ "'");

        a->removeOverlappedEntity(pathway);

    }else if(fudA->role == FixtureRole::ACTOR_BODY && fudB->role == FixtureRole::PATHWAY_SENSOR){

        assert(entityA->type()==EntityType::ACTOR);
        Actor *a = static_cast<Actor*>(entityA);
        //EBehavior_Player_Gravity *b = static_cast<EBehavior_Player_Gravity*>(a->behavior());
        assert(entityB->type()==EntityType::PATHWAY);
        EPathway *pathway = static_cast<EPathway*>(entityB);

        print("End contact between actor '" + a->sourceEntity()->name() + "' and pathway '" + entityB->sourceEntity()->name()+ "'");

        a->removeOverlappedEntity(pathway);
    }

    */
}


void EntityContactListener::_endContact(b2Contact* contact)
{

    FixtureUserData *actorFUD = reinterpret_cast<FixtureUserData *>(mActorFixture->GetUserData().pointer);
    FixtureUserData *entityFUD = reinterpret_cast<FixtureUserData *>(mEntityFixture->GetUserData().pointer);

    //if(actorFUD->category->role==EntityRole::GROUND_SENSOR){
    //    if(mEntityFixture->IsSensor()==false){      //every solid object is registered
    //        mActor->groundSensorFixtures().removeFixture(mEntityFixture, mEntityFixtureChildIndex);

    //    }
    //}


    mActor->contactTrigger().onEndConctact(EntityShape(mActor, actorFUD->category), mActorFixture, mActorFixtureChildIndex,
                                            EntityShape(mEntity, entityFUD->category), mEntityFixture, mEntityFixtureChildIndex);

    mEntity->contactTrigger().onEndConctact(EntityShape(mEntity, entityFUD->category), mEntityFixture, mEntityFixtureChildIndex,
                                             EntityShape(mActor, actorFUD->category), mActorFixture, mActorFixtureChildIndex);


}



void EntityContactListener::PreSolve(b2Contact *contact, const b2Manifold *oldManifold)
{

    const b2Manifold* manifold = contact->GetManifold();

    if (manifold->pointCount == 0){
        return;
    }

    mFixtureA = contact->GetFixtureA();
    mFixtureB = contact->GetFixtureB();
    int childIndexA = contact->GetChildIndexA();
    int childIndexB = contact->GetChildIndexB();
    assert(mFixtureA->GetUserData().pointer);
    assert(mFixtureB->GetUserData().pointer);
    Entity *entityA = reinterpret_cast<Entity *>(mFixtureA->GetBody()->GetUserData().pointer);
    Entity *entityB = reinterpret_cast<Entity *>(mFixtureB->GetBody()->GetUserData().pointer);


    b2PointState state1[b2_maxManifoldPoints], state2[b2_maxManifoldPoints];
    b2GetPointStates(state1, state2, oldManifold, manifold);

   // b2WorldManifold worldManifold;
    contact->GetWorldManifold(&mWorldManifold);

    if(state2[0]!=b2_addState && state2[0]!=b2_persistState){
        return;
    }
    if(manifold->pointCount>1){
        if(state2[1]!=b2_addState && state2[1]!=b2_persistState){
            return;
        }
    }

    bool reverseNormal = false;

    mDynamicEntity1 = mDynamicEntity2 = mNonDynamicEntity = nullptr;
    mDynamicEntity1Fixture = mDynamicEntity2Fixture = mNonDynamicEntityFixture = nullptr;
    mDynamicEntity1FixtureChildIndex = mDynamicEntity2FixtureChildIndex = mNonDynamicEntityFixtureChildIndex = -1;


    //if(entityA->mainShapeRole()==EntityRole::ACTOR || entityA->mainShapeRole()==EntityRole::MOVABLE_OBJECT){
    if(entityA->sourceEntity()->sourceEntityCfg()->category->mB2BodyType==b2BodyType::b2_dynamicBody){
        mDynamicEntity1 = entityA;
        mDynamicEntity1Fixture = mFixtureA;
        mDynamicEntity1FixtureChildIndex = childIndexA;

    }else{
        mNonDynamicEntity = entityA;
        mNonDynamicEntityFixture = mFixtureA;
        mNonDynamicEntityFixtureChildIndex = childIndexA;


    }

    //if(entityB->mainShapeRole()==EntityRole::ACTOR || entityB->mainShapeRole()==EntityRole::MOVABLE_OBJECT){
    if(entityB->sourceEntity()->sourceEntityCfg()->category->mB2BodyType==b2BodyType::b2_dynamicBody){
        if(mDynamicEntity1==nullptr){
            mDynamicEntity1 = entityB;
            mDynamicEntity1Fixture = mFixtureB;
            mDynamicEntity1FixtureChildIndex = childIndexB;
        }else{
            mDynamicEntity2 = entityB;
            mDynamicEntity2Fixture = mFixtureB;
            mDynamicEntity2FixtureChildIndex = childIndexB;
        }
    }else{
        mNonDynamicEntity = entityB;
        mNonDynamicEntityFixture = mFixtureB;
        mNonDynamicEntityFixtureChildIndex = childIndexB;

        reverseNormal = true;
    }



    FixtureUserData *actorFUD = reinterpret_cast<FixtureUserData *>(mActorFixture->GetUserData().pointer);
    FixtureUserData *entityFUD = reinterpret_cast<FixtureUserData *>(mEntityFixture->GetUserData().pointer);


    //if(fudA->role == FixtureRole::BLOCKING_ENVIRONMENT && fudB->role == FixtureRole::ACTOR_BODY){

    //if(entityFUD->bodyKind == EntityBodyKind::STATIC_SOLID_BODY || entityFUD->bodyKind == EntityBodyKind::KINEMATIC_SOLID_BODY){
    if(mDynamicEntity1 && mDynamicEntity2){

        Entity* actor1 = nullptr;
        Entity* movableObject = nullptr;

        const FixtureUserData *fud1= reinterpret_cast<FixtureUserData *>(mDynamicEntity1Fixture->GetUserData().pointer);
        const FixtureUserData *fud2= reinterpret_cast<FixtureUserData *>(mDynamicEntity2Fixture->GetUserData().pointer);
            //const FixtureUserData *fudB = reinterpret_cast<FixtureUserData *>(mFixtureB->GetUserData().pointer);

        // normal points from  entity A to B, which is in this case from entity1 to entity2 !

        if(mDynamicEntity1->entityMovingGroup() && mDynamicEntity2->entityMovingGroup()){
            DummyFunction();
        }

        if(mWorldManifold.normal.y<0){

            // entity1 is above entity2

            if(mDynamicEntity1->currentEngine()->type()==MovementEngineType::GROUND_MOVEMENT){
                if((mDynamicEntity1->statusFlagsRef() & EntityStatusFlags::MOVING_ON_GROUND)==0){
                    if(fud2->slidingGroundCfg){
                        contact->SetFriction(0.0f);
                    }else{
                        contact->SetFriction(1.0f);
                    }
                }else{
                    contact->SetFriction(0.0f);
                }
            }

        }else if(mWorldManifold.normal.y>0){

            // entity2 is above entity1

            if(mDynamicEntity2->currentEngine()->type()==MovementEngineType::GROUND_MOVEMENT){
                if((mDynamicEntity2->statusFlagsRef() & EntityStatusFlags::MOVING_ON_GROUND)==0){
                    if(fud1->slidingGroundCfg){
                        contact->SetFriction(0.0f);
                    }else{
                        contact->SetFriction(1.0f);
                    }
                }else{
                    contact->SetFriction(0.0f);
                }
            }
        }



        if(contact->IsEnabled()){

            // we want to normal always point from the second (contacted) entity to the first

            storeContactPoints(mDynamicEntity1->contactPoints(), mDynamicEntity1->contactPointsCounterRef(), manifold,
                               mDynamicEntity2Fixture, mDynamicEntity2FixtureChildIndex, state2[0], true);

            storeContactPoints(mDynamicEntity2->contactPoints(), mDynamicEntity2->contactPointsCounterRef(), manifold,
                               mDynamicEntity1Fixture, mDynamicEntity1FixtureChildIndex, state2[0], false);
        }

        /*
        if(fud1->category->role==EntityRole::MOVABLE_OBJECT && fud2->category->role==EntityRole::ACTOR){
            mDynamicEntity2->body()->B2Body()->SetLinearVelocity({0.0f, 0.0f});
        }else if(fud2->category->role==EntityRole::MOVABLE_OBJECT && fud1->category->role==EntityRole::ACTOR){
            mDynamicEntity1->body()->B2Body()->SetLinearVelocity({0.0f, 0.0f});
        }
        */


    }else{

        assert(mDynamicEntity1 && mNonDynamicEntity);

        const FixtureUserData *fudDynamic= reinterpret_cast<FixtureUserData *>(mDynamicEntity1Fixture->GetUserData().pointer);
        const FixtureUserData *fudNonDynamic= reinterpret_cast<FixtureUserData *>(mNonDynamicEntityFixture->GetUserData().pointer);


        if(mDynamicEntity1->currentEngine()->type()==MovementEngineType::GROUND_MOVEMENT){
            //if(mDynamicEntity1->groundMovementEngine()->isMoving()==false){
            if((mDynamicEntity1->statusFlagsRef() & EntityStatusFlags::MOVING_ON_GROUND)==0){
                if(fudNonDynamic->slidingGroundCfg){
                    contact->SetFriction(0.0f);
                }else{
                    contact->SetFriction(0.8f);
                    //contact->SetFriction(1.0f);
                }
            }else{
                contact->SetFriction(0.0f);
            }


        }else if(mDynamicEntity1->currentEngine()->type()==MovementEngineType::JUMP_MOVEMENT){

            if(fudNonDynamic->properties & FixtureProperties::PASSTHROUGH_FROM_BELLOW){

                b2Vec2 vel = mDynamicEntity1-> body()->B2Body()->GetLinearVelocity();
                float actorBottomPos = mDynamicEntity1->body()->B2Body()->GetPosition().y - mDynamicEntity1->body()->sourceBody()->centerToBottomDistance();

                for(int i=0; i<manifold->pointCount; i++){
                    b2Vec2 pContact = mWorldManifold.points[i];
                    if(vel.y>0.0f || actorBottomPos < pContact.y){      // this seems to produce good and smooth results
                        contact->SetEnabled(false);
                    }
                }
            }


        }else if(mDynamicEntity1->currentEngine()->type()==MovementEngineType::PATHWAY_MOVEMENT){
            contact->SetEnabled(false);

        }else{
            contact->SetFriction(0.0f);
        }


        if(mNonDynamicEntity->mainShapeRole()==EntityRole::TRANSPORTER){


            Entity *transporter = entityA;

            if(transporter->sourceEntity()->sourceEntityCfg()->passableFromBellow){
                if(mDynamicEntity1->currentEngine()->type()==MovementEngineType::JUMP_MOVEMENT){

                    float actorBottomPos = mDynamicEntity1->body()->B2Body()->GetPosition().y - mDynamicEntity1->body()->sourceBody()->centerToBottomDistance();
                    float liftTopPos = transporter->body()->B2Body()->GetPosition().y + transporter->body()->sourceBody()->centerToTopDistance() - 3.0f * b2_linearSlop;

                    if(mDynamicEntity1->body()->B2Body()->GetLinearVelocity().y>0.0f){      // comparing actual dimensions is the only way to make this reliable
                        if(actorBottomPos < liftTopPos){
                            contact->SetEnabled(false);
                        }
                    }else{
                        // this bellow doesnt work for ratating platforms
                        // I must implement a flag that some platform are not passable from bellow !
                        //float minTransporterHeightInMeters =  gWorldInfo.minTransporterHeightInMeters();
                        //if(actorBottomPos < liftTopPos - minTransporterHeightInMeters){
                        //    contact->SetEnabled(false);
                        //}
                    }
                }
            }
        }


        if(contact->IsEnabled()){
            //storeContactPoints(mActor->contactPoints(), mActor->contactPointsCounterRef(), manifold, mFixtureA, childIndexA, state2[0], false);
            storeContactPoints(mDynamicEntity1->contactPoints(), mDynamicEntity1->contactPointsCounterRef(), manifold, mNonDynamicEntityFixture, mNonDynamicEntityFixtureChildIndex, state2[0], reverseNormal);
        }
    }

}



void EntityContactListener::storeContactPoints(std::vector<ContactPointInfo> &_contactPoints, int &contactPointsCounter, const b2Manifold* manifold, b2Fixture *_fixture, int _shapeIndex, b2PointState _pointState, bool _reverseNormal)
{

    int startIndex = contactPointsCounter;

    contactPointsCounter += manifold->pointCount;
    if(contactPointsCounter >=_contactPoints.size()){
        _contactPoints.resize(contactPointsCounter+1);

        print( " !!! _contactPoints.resize :" + std::to_string(contactPointsCounter+1));
    }

    for(int p=startIndex; p<contactPointsCounter; p++){
        ContactPointInfo &cp = _contactPoints[p];
        for(int i=0; i<manifold->pointCount; i++){
            cp.position = mWorldManifold.points[i];
            cp.normal = (_reverseNormal)? -mWorldManifold.normal : mWorldManifold.normal;
            cp.separation = mWorldManifold.separations[i];
            cp.normalImpulse = manifold->points[i].normalImpulse;
            cp.tangentImpulse = manifold->points[i].tangentImpulse;
            cp.fixture = _fixture;
            cp.shapeIndex = _shapeIndex;
            cp.state = _pointState;
        }
    }

}



//=====================================================================================================


bool EntityContactFilter::ShouldCollide(b2Fixture *fixtureA, b2Fixture *fixtureB)
{



    const FixtureUserData *fudA = reinterpret_cast<FixtureUserData *>(fixtureA->GetUserData().pointer);
    const FixtureUserData *fudB = reinterpret_cast<FixtureUserData *>(fixtureB->GetUserData().pointer);
    Entity *entityA = reinterpret_cast<Entity *>(fixtureA->GetBody()->GetUserData().pointer);
    Entity *entityB = reinterpret_cast<Entity *>(fixtureB->GetBody()->GetUserData().pointer);


    //if(fudA->contactDisabled || fudB->contactDisabled){
    //    return false;
    //}
    if(fudA->category->role==EntityRole::CEILING_SENSOR){
        DummyFunction();
    }
    if(fudB->category->role==EntityRole::CEILING_SENSOR){
        DummyFunction();
    }
    if(fudA->category->role==EntityRole::CEILING_SENSOR && fudB->category->role==EntityRole::PASSABLE_AREA){
        DummyFunction();
    }
    if(fudA->category->role==EntityRole::PASSABLE_AREA && fudB->category->role==EntityRole::CEILING_SENSOR){
        DummyFunction();
    }

    if(entityA->entityMovingGroup()){
        DummyFunction();
    }
    if(entityA->entityMovingGroup() && entityA->entityMovingGroup()==entityB->entityMovingGroup()){
        return entityA->entityMovingGroup()->membersCollision();
    }

    bool collide = (fudA->category->contactMask & fudB->category->contactBits) != 0 && (fudA->category->contactBits & fudB->category->contactMask) != 0;

    if(collide==false){
        DummyFunction();
    }

    /*
    const b2Filter& filterA = fixtureA->GetFilterData();
    const b2Filter& filterB = fixtureB->GetFilterData();

    if (filterA.groupIndex == filterB.groupIndex && filterA.groupIndex != 0)
    {
        return filterA.groupIndex > 0;
    }

    bool collide = (filterA.maskBits & filterB.categoryBits) != 0 && (filterA.categoryBits & filterB.maskBits) != 0;
    */

    return collide;

}




}
