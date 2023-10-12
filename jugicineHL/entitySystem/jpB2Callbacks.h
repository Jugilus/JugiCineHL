#ifndef PLAYER__ENTITY__BOX2D_CALLBACKS__H
#define PLAYER__ENTITY__BOX2D_CALLBACKS__H

#include <memory>
#include <string>
#include <vector>
#include "box2d.h"
#include "jmCommon.h"
#include "jpGlobal.h"



namespace jugimap{


class WorldMapCamera;

class PlayedScene;
class SourceSprite;
class SourceBody;
struct EntityControllerCfg;

class Entity;
class Entity;
struct FixtureUserData;


//-------------------------------------------------------------------------------------------------


class EntityContactListener : public b2ContactListener
{
public:


    void BeginContact(b2Contact* contact) override;
    void EndContact(b2Contact* contact) override;
    void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;
    //void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;


    void storeContactPoints(std::vector<ContactPointInfo> &_contactPoints, int &contactPointsCounter, const b2Manifold* manifold, b2Fixture *_fixture, int _shapeIndex, b2PointState _pointState, bool _reverseNormal);


    b2Fixture* mFixtureA = nullptr;
    b2Fixture* mFixtureB = nullptr;
    b2WorldManifold mWorldManifold;





private:

    Entity* mActor = nullptr;
    Entity* mEntity = nullptr;

    b2Fixture *mActorFixture = nullptr;
    b2Fixture *mEntityFixture = nullptr;

    int mActorFixtureChildIndex = 0;
    int mEntityFixtureChildIndex = 0;


    Entity *mDynamicEntity1 = nullptr;
    Entity *mDynamicEntity2 = nullptr;
    Entity *mNonDynamicEntity = nullptr;

    b2Fixture *mDynamicEntity1Fixture = nullptr;
    b2Fixture *mDynamicEntity2Fixture = nullptr;
    b2Fixture *mNonDynamicEntityFixture = nullptr;

    int mDynamicEntity1FixtureChildIndex = 0;
    int mDynamicEntity2FixtureChildIndex = 0;
    int mNonDynamicEntityFixtureChildIndex = 0;


    void _beginContact(b2Contact* contact);
    void _endContact(b2Contact* contact);

};


class EntityContactFilter : public b2ContactFilter
{
public:

    bool ShouldCollide(b2Fixture *fixtureA, b2Fixture *fixtureB) override;

};

//extern EntityContactListener myContactListener;





}






#endif
