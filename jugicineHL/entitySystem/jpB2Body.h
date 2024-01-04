#ifndef PLAYER__ENTITIES__BOX2D_BODY__H
#define PLAYER__ENTITIES__BOX2D_BODY__H

#include <memory>
#include <string>
#include <vector>
#include "box2d.h"
#include "jpB2Fixture.h"



namespace jugimap{


class SourceSprite;
class SpriteLayer;
class Entity;
struct EntityCategory;
struct SourceEntityCfg;






class SourceBody
{
public:

    //bool initBody(EntityType _bodyRole, EntityBodyType _bodyType, b2Vec2 _bodyCenterPos);
    bool initBody(SourceEntityCfg *_sourceEntityCfg, b2Vec2 _bodyCenterPos, SourceSprite *_sourceSprite, jugimap::SpriteLayer *_spriteLayer);
    //bool addBodyFixture(FixtureRole _fixtureRole, SourceSprite *_sourceSprite);
    //bool addSpriteFixtures(SourceSprite *_sourceSprite);



    bool addSourceFixtureFromMapVectorShape(VectorShape *_vectorShape, Vec2f dPos);

    SourceFixture* getSourceFixture(EntityCategory *_category, bool _setErrorMessage=true);


    const b2BodyDef & B2bodyDef() const { return mB2BodyDef; }
    //EntityType bodyRole() const { return mBodyRole; }
    EntityCategory* category(){ return mCategory; }
    //EntityRole entityBodyKind(){ return mBodyKind; }
    std::vector<SourceFixture> &sourceFixtures() { return mSourceFixtures; }

    SourceSprite* sourceSprite(){ return mSourceSprite; }

    float centerToTopDistance();
    float centerToBottomDistance();

    float centerToLeftDistance();
    float centerToRightDistance();




private:
    b2BodyDef mB2BodyDef;

    //EntityRole mBodyKind = EntityRole::UNKNOWN;
    //EntityBodyProperties mBodyProperties;
    EntityCategory *mCategory = nullptr;


    std::vector<SourceFixture>mSourceFixtures;
    BoundingBoxFloat mBodyBoundingBox;
    b2Vec2 mAnchorPoint{0.0f, 0.0f};                            // relative to sprite image center !
    std::vector<FixtureUserData>mSensorFixturesUserData;
    //std::vector<FixtureUserData>mFixtureUserDatas;
    FixtureUserDataGroup mFixtureUserDataGroup;

    //---
    SourceSprite *mSourceSprite = nullptr;      // for actor source bodies


    bool addStandardSpriteMainShapeFixture(SourceEntityCfg *_sourceEntityCfg);
    bool addSpriteLayerMainShapeFixtures(SourceEntityCfg *_sourceEntityCfg, jugimap::SpriteLayer *_spriteLayer, Vec2f _anchorPoint = Vec2f(0.0f, 0.0f));
    bool addVectorShapeBodyFixture(SourceEntityCfg *_sourceEntityCfg);
    bool addSensorsFixtures(std::vector<CustomSensorSourceEntityCfg> &_sensorCfgs);


    //void setBodyDef(b2BodyDef &_b2BodyDef);
    void setFixtureDef(EntityCategory *_category, b2FixtureDef &_b2FixtureDef);
   // bool ObtainFixtureUserDatas(SourceSprite *_sourceSprite);

};


//=================================================================================================


class Body
{
public:
    Body(SourceBody *_sourceBody, Entity *_parentEntity);

    //bool init(SourceBody *_sourceBody, Entity *_parentEntity, Vec2f _bodyPos, float _bodyAngle);

    void createB2Body(Vec2f _bodyPos, float _bodyAngle);
    void destroyB2Body();

    b2Body * B2Body(){ return mB2Body; }
    Entity * parentEntity(){ return mParentEntity; }
    SourceBody * sourceBody(){ return mSourceBody; }

    b2Fixture * getFixtureOfCategory(EntityCategory *_category, bool _setErrorMessage=true);

   // void addTemporaryFixture(EntityCategory *_category);
    void destroyFixture(EntityCategory *_category);


    void createFixture(const SourceFixture &_sf);

    //b2Body * createExtraShapeBody(const SourceFixture &_sf);
    //void destroyExtraShapeBody(b2Body *_body);



private:
    SourceBody *mSourceBody = nullptr;     // LINK
    Entity *mParentEntity = nullptr;                            // LINK
    b2Body * mB2Body = nullptr;                                   // LINK, the object is managed by b2World


    //std::vector<b2Body*>mExtraShapeBodies;

};




}






#endif
