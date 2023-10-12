#ifndef PLAYER__ENTITIES__BOX2D_FIXTURE__H
#define PLAYER__ENTITIES__BOX2D_FIXTURE__H

#include <memory>
#include <string>
#include <vector>
#include "box2d.h"
#include "jmCommon.h"
#include "jpEntityGlobal.h"


class b2World;

namespace pugi{
class xml_node;
}

namespace jugimap{


class Sprite;
class SpriteLayer;
class SourceSprite;
class StandardSprite;
class VectorShape;
class Entity;
class Entity;
class SourceLibrary;


struct SourceEntityCfg;
struct SlidingGroundCfg;
struct ConveyorGroundCfg;



template <typename T>
b2Vec2 GetUnitNormal(T p1, T p2)
{

    float dist = DistanceTwoPoints(p1, p2);

    return b2Vec2((p2.y-p1.y)/dist, -(p2.x-p1.x)/dist);

}



struct FixtureUserData
{

    //static int idCounter;
    //int id = 0;
    VectorShape * vectorShape = nullptr;                     // LINK
    SourceSprite * sourceSprite = nullptr;                  // LINK
    EntityCategory *category = nullptr;
    //EntityRole bodyKind = EntityRole::UNKNOWN;
    //unsigned int contactBits = std::numeric_limits< unsigned int>::max();
    //unsigned int contactMask = std::numeric_limits< unsigned int>::max();
    bool contactDisabled = false;
    int properties = FixtureProperties::NONE;

    SlidingGroundCfg *slidingGroundCfg = nullptr;
    ConveyorGroundCfg *conveyorGroundCfg = nullptr;         // LINK




    int blockedDir = 0;
    std::vector<float>angles;
    std::vector<Direction>blockedDirections;
    std::vector<b2Vec2>normals;

    //std::vector<std::pair<b2Vec2,Direction>>normals;         // normals for edge shapes and chain shapes
    void obtainNormals();
};



class FixtureUserDataGroup
{
public:

    void create(SourceLibrary &_sourceLibrary, SourceEntityCfg *_sourceEntityCfg);
    void create(SpriteLayer * _spriteLayer, SourceEntityCfg *_sourceEntityCfg);
    void create(SourceSprite * _sourceSprite, SourceEntityCfg *_sourceEntityCfg);

    FixtureUserData*getFixtureUserData(VectorShape *_vectorShape);
    void collectFixtureUserData(std::vector<FixtureUserData*> &collectedObjects, SourceSprite *_sourceSprite);
    std::vector<FixtureUserData> & fixtureUserDatas(){ return mFixtureUserDatas; }

private:
    std::vector<FixtureUserData>mFixtureUserDatas;
    //FixtureUserData mDummy;

    void _create(const std::vector<SourceSprite*> &_sourceSprites, SourceEntityCfg *_sourceEntityCfg);

};


//extern FixtureUserDataGroup gFixtureShapeAndDataManager;




struct SourceFixture
{
    FixtureUserData *fud = nullptr;          // LINK
    //FixtureUserData fud;          // LINK
    b2FixtureDef mB2FixtureDef;

    bool temporaryFixture = false;

    //--- b2Shape data
    b2Shape::Type mShapeType;
    std::vector<b2Vec2>mVertices;
    b2Vec2 mPrevVertex;
    b2Vec2 mNextVertex;
    bool createChainLoop = false;
    b2Vec2 mCenter;
    float mRadius;
};



/*
struct BodyUserData
{

    BodyUserData(){}
    BodyUserData(EntityType _bodyRole) : mBodyRole(_bodyRole){}

    EntityType mBodyRole = EntityType::UNKNOWN;
    Entity *mEntity = nullptr;                    // LINK

};
*/


struct DirectionAndAngle
{

    DirectionAndAngle(Direction _direction, float _angle) : mDirection(_direction), mAngle(_angle){}

    Direction mDirection = Direction::NONE;
    float mAngle = 0.0f;

};



DirectionAndAngle GetBlockedDirectionFromSurfaceNormal(b2Vec2 normal);



//unsigned int GetFixtureRoleCollisionBits(FixtureRole role);




}






#endif
