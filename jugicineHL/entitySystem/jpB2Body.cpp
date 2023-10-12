#include <chrono>
#include <assert.h>
#include <sstream>
#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmMap.h"
#include "jmCamera.h"
#include "jmSceneLayout.h"
#include "jmCommonFunctions.h"
#include "jmVectorShape.h"
#include "jmStandardSprite.h"
#include "jmCompositeSprite.h"
#include "jmSpriteLayer.h"
#include "jmUtilities.h"
#include "items/jpItemsCommon.h"
#include "jpPlayedApp.h"
#include "jpPlayedScene.h"
#include "jpItemsTable.h"
#include "jpUtilities.h"
#include "jpActionsCommon.h"
#include "scripting/jpBehavior.h"
#include "jpEntity.h"
#include "jpSourceEntity.h"
#include "jpEntitySystem.h"
#include "jpB2Utilities.h"
#include "jpB2World.h"
#include "jpB2Body.h"



namespace jugimap{




//bool SourceBody::initBody(EntityBod _bodyRole, EntityBodyType _bodyType, b2Vec2 _bodyCenterPos)
bool SourceBody::initBody(SourceEntityCfg *_sourceEntityCfg, b2Vec2 _bodyCenterPos, SourceSprite *_sourceSprite, jugimap::SpriteLayer *_spriteLayer)
{

    //mBodyRole = _bodyRole;
    //mBodyType = _bodyType;
    //mBodyKind = _sourceEntityCfg->category->mEntityBodyKind;
    //mBodyProperties = _sourceEntityCfg->entityBodyProperties;
    mCategory = _sourceEntityCfg->category;
    mAnchorPoint = _bodyCenterPos;

    mB2BodyDef.type = mCategory->mB2BodyType;
    mB2BodyDef.fixedRotation = _sourceEntityCfg->fixedRotation;

    //setBodyDef(mBodyKind, mBodyProperties, mB2BodyDef);

    mSourceSprite = _sourceSprite;


    if(_sourceEntityCfg->name == "fancyPlatformA"){
        DummyFunction();
    }

    if(_sourceEntityCfg->entityMapElement==EnumEntityMapElement::SPRITE){
        assert(mSourceSprite);

        if(mSourceSprite->kind()==SpriteType::STANDARD){
            if(addStandardSpriteMainShapeFixture(_sourceEntityCfg)==false){
                return false;
            }


        }else if(mSourceSprite->kind()==SpriteType::COMPOSITE){

            CompositeSprite *sourceCS = mSourceSprite->sourceCompositeSprite();
            assert(sourceCS);


            Vec2f handle = mSourceSprite->sourceCompositeSprite()->handleForMapCompositeSprites();
            Vec2f anchorPointP = handle;

            std::vector<Layer*>spriteLayers;
            std::string _parameter = "eMainShape";
            CollectLayersWithParameter(sourceCS->map(), spriteLayers, _parameter, "", LayerKind::SPRITE_LAYER);

            for(Layer* l : spriteLayers){
                if(addSpriteLayerMainShapeFixtures(_sourceEntityCfg, static_cast<SpriteLayer*>(l), anchorPointP)==false){
                    return false;
                }
            }

            mAnchorPoint.x = gWorldInfo.pixelsToMeters(anchorPointP.x);
            mAnchorPoint.y = gWorldInfo.pixelsToMeters(anchorPointP.y);
            //print("Center To Top: " + std::to_string(centerToTopDistance()));
            //print("Center To Bottom: " + std::to_string(centerToBottomDistance()));

        }

        //--- extra shapes fixtures
        if(_sourceEntityCfg->mCustomSensorSourceEntityCfgs.empty()==false){
            addSensorsFixtures(_sourceEntityCfg->mCustomSensorSourceEntityCfgs);
        }


    }else if(_sourceEntityCfg->entityMapElement==EnumEntityMapElement::SPRITE_LAYER){
        assert(_spriteLayer);

        if(addSpriteLayerMainShapeFixtures(_sourceEntityCfg, _spriteLayer)==false){
            return false;
        }


    }else if(_sourceEntityCfg->entityMapElement==EnumEntityMapElement::VECTOR_SHAPE){

        if(addVectorShapeBodyFixture(_sourceEntityCfg)==false){
            return false;
        }


    }

    return true;
}


bool SourceBody::addStandardSpriteMainShapeFixture(SourceEntityCfg *_sourceEntityCfg)
{

    dbgSystem.addMessage("Add body fixture for standard source sprite '" + mSourceSprite->name() + "' ...");

    //if(ObtainFixtureUserDatas(mSourceSprite)==false){
    //    return false;
    //}

    mFixtureUserDataGroup.create(mSourceSprite, _sourceEntityCfg);
    if(mFixtureUserDataGroup.fixtureUserDatas().empty()){
       dbgSystem.addMessage("No shapes found for the source sprite '" + mSourceSprite->name() + "'!");
        return false;
    }



    //std::vector<FixtureUserData*>fuds;

    ///FixtureUserDataGroup *fixtureUserDataGroup = static_cast<PlayedScene*>(app->activeScene())->entitySystem()->fixtureUserDataGroup();

    //fixtureUserDataGroup->collectFixtureShapeAndUserData(fuds, _sourceSprite);

    //if(fuds.empty()){
     //   dbgSystem.addMessage("No shapes found for the source sprite '" + _sourceSprite->name() + "'!");
    //    return false;
    //}

    //---
    mBodyBoundingBox.setInitialSize();


    SourceImage *si = mSourceSprite->sourceImages().front();
    Vec2f handle = Vec2iToVec2f(si->handle());
    Vec2f anchorPointP(handle.x - si->width() * 0.5f, handle.y - si->height()*0.5f);


    for(FixtureUserData & fud : mFixtureUserDataGroup.fixtureUserDatas()){

        mSourceFixtures.push_back(SourceFixture());
        SourceFixture &sf = mSourceFixtures.back();

        sf.fud = &fud;
        sf.fud->category = mCategory;
        setFixtureDef(sf.fud->category, sf.mB2FixtureDef);
        sf.mB2FixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(&fud);

        if(mCategory->role==EntityRole::TRANSPORTER){
            sf.createChainLoop = true;                    // needed to be tested
        }

        Vec2f sprPos =  -anchorPointP;

        //---
        //createB2ShapeVerticesForTileSprite(sf, {0.0f, 0.0f}, nullptr);
        createB2ShapeVerticesForTileSprite(sf, sprPos, nullptr);

        //---
        BoundingBoxFloat bb = fud.vectorShape->boundingBoxF();
        bb.min.x = gWorldInfo.pixelsToMeters(bb.min.x);
        bb.min.y = gWorldInfo.pixelsToMeters(bb.min.y);
        bb.max.x = gWorldInfo.pixelsToMeters(bb.max.x);
        bb.max.y = gWorldInfo.pixelsToMeters(bb.max.y);
        mBodyBoundingBox.Unite(bb);
    }

    mAnchorPoint.x = gWorldInfo.pixelsToMeters(anchorPointP.x);
    mAnchorPoint.y = gWorldInfo.pixelsToMeters(anchorPointP.y);
    //mAnchorPoint.x = gWorldInfo.pixelsToMeters(si->handle().x);
    //mAnchorPoint.y = gWorldInfo.pixelsToMeters(si->handle().y);

    print("Center To Top: " + std::to_string(centerToTopDistance()));
    print("Center To Bottom: " + std::to_string(centerToBottomDistance()));
    //--- get anchor point
    //SourceImage *si = mSourceSprite->sourceImages().front();
    //mAnchorPoint.x = gWorldInfo.pixelsToMeters(si->handle().x);
    //mAnchorPoint.y = gWorldInfo.pixelsToMeters(si->handle().y);


    dbgSystem.removeLastMessage();
    return true;


}


bool SourceBody::addVectorShapeBodyFixture(SourceEntityCfg *_sourceEntityCfg)
{

    if(mCategory->role==EntityRole::PATHWAY){
        dbgSystem.addMessage("Add sensors fixtures for pathway entity ...");

    }else if(mCategory->role==EntityRole::PASSABLE_AREA){
        dbgSystem.addMessage("Add sensors fixtures for area entity ...");

    }else{
        dbgSystem.addMessage("Wrong entity body kind '" + GetEntityRoleString(mCategory->role) + " '!");
        dbgSystem.addMessage("The entity kind for vector shapes must be '" + GetEntityRoleString(EntityRole::PASSABLE_AREA) + "' or '" +
                             GetEntityRoleString(EntityRole::PATHWAY) + "' !");

        return false;
    }


    mSensorFixturesUserData.resize(1);

    for(int i=0; i<mSensorFixturesUserData.size(); i++ ){

        FixtureUserData *fud = &mSensorFixturesUserData[i];
        mSourceFixtures.push_back(SourceFixture());
        SourceFixture &sf = mSourceFixtures.back();

        sf.fud = fud;
        //sf.fud->bodyKind = mBodyKind;
        //sf.fud->contactBits = _sourceEntityCfg->category->mContactBits;
        //sf.fud->contactMask = _sourceEntityCfg->category->mContactMask;
        sf.fud->category = mCategory;
        setFixtureDef(sf.fud->category, sf.mB2FixtureDef);
        sf.mB2FixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(fud);

        // no shape is being defined (they will be defined from the map vector shapes)
    }


    dbgSystem.removeLastMessage();
    return true;

}


bool SourceBody::addSensorsFixtures(std::vector<CustomSensorSourceEntityCfg> &_sensorCfgs)
{

    if(mCategory->role != EntityRole::ACTOR){
        dbgSystem.addMessage( "Using add-on sensors is possible only with '"+ GetEntityRoleString(EntityRole::ACTOR) + "'!");
        return false;
    }

    if(mSourceSprite==nullptr){
        dbgSystem.addMessage( "Using add-on sensors is possible only with sprite based entities!'");
        return false;
    }


    dbgSystem.addMessage("Adding sensors fixtures for source sprite '" + mSourceSprite->name() + "' ...");


    mSensorFixturesUserData.resize(_sensorCfgs.size());


    for(int i=0; i<_sensorCfgs.size(); i++ ){

        FixtureUserData *fud = &mSensorFixturesUserData[i];
        CustomSensorSourceEntityCfg &sensorCfg = _sensorCfgs[i];

        EntityRole bodyKind = sensorCfg.category->role;
        mSourceFixtures.push_back(SourceFixture());
        SourceFixture &sf = mSourceFixtures.back();
        sf.temporaryFixture = sensorCfg.temporary;

        sf.fud = fud;

        //sf.fud->bodyKind = bodyKind;
        //sf.fud->contactBits = sensorCfg.category->mContactBits;
        //sf.fud->contactMask = sensorCfg.category->mContactMask;
        sf.fud->category =  sensorCfg.category;
        setFixtureDef(sf.fud->category, sf.mB2FixtureDef);
        //sf.mB2FixtureDef.isSensor = true;
        sf.mB2FixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(fud);

        if(bodyKind == EntityRole::GROUND_SENSOR){

            // the shape is a rectangle polygon bellow the body
            sf.mShapeType = b2Shape::Type::e_polygon;
            sf.mVertices.resize(4);
            float d = 0.1f;
            sf.mVertices[0] = b2Vec2(mBodyBoundingBox.min.x + d, mBodyBoundingBox.min.y - gWorldInfo.minGeometryBlockMeters());
            sf.mVertices[1] = b2Vec2(mBodyBoundingBox.max.x - d , mBodyBoundingBox.min.y - gWorldInfo.minGeometryBlockMeters());
            sf.mVertices[2] = b2Vec2(mBodyBoundingBox.max.x - d, mBodyBoundingBox.min.y);
            sf.mVertices[3] = b2Vec2(mBodyBoundingBox.min.x + d, mBodyBoundingBox.min.y);

        }else if(bodyKind == EntityRole::CEILING_SENSOR){

            // the shape is a rectangle polygon above the body
            sf.mShapeType = b2Shape::Type::e_polygon;
            sf.mVertices.resize(4);
            float d = 0.1f;
            sf.mVertices[0] = b2Vec2(mBodyBoundingBox.min.x + d, mBodyBoundingBox.max.y);
            sf.mVertices[1] = b2Vec2(mBodyBoundingBox.max.x - d, mBodyBoundingBox.max.y);
            sf.mVertices[2] = b2Vec2(mBodyBoundingBox.max.x - d, mBodyBoundingBox.max.y + gWorldInfo.ceilingSensorHeightMeters());
            sf.mVertices[3] = b2Vec2(mBodyBoundingBox.min.x + d, mBodyBoundingBox.max.y + gWorldInfo.ceilingSensorHeightMeters());


        }else if(bodyKind == EntityRole::CUSTOM_SENSOR){

            assert(mSourceSprite);

            sf.fud->contactDisabled = true;

            if(mSourceSprite->kind()==SpriteType::STANDARD){

                Vec2f handle,anchorPointP;

                VectorShape *vs = nullptr;
                for(SourceImage *si : mSourceSprite->sourceImages()){
                    vs = FindSpriteExtraShapeWithProperties(si, sensorCfg.extraVectorShapeData, false, ShapeKind::RECTANGLE);
                    if(vs){
                        handle = Vec2iToVec2f(si->handle());
                        anchorPointP.set(handle.x - si->width() * 0.5f, handle.y - si->height()*0.5f);
                        break;
                    }
                }

                if(vs==nullptr){
                    dbgSystem.addMessage("The sprite of custom sensor of category'" +sensorCfg.category->name +
                                         " has no extra vector shape with data = " + std::to_string(sensorCfg.extraVectorShapeData));
                    dbgSystem.addMessage("Warning the vector shape must be a rectangle!");
                    return false;
                }

                sf.mShapeType = b2Shape::Type::e_polygon;
                sf.mVertices.resize(vs->mPoints.size());

                for(unsigned int i=0; i<vs->mPoints.size(); i++){
                    sf.mVertices[i].Set(gWorldInfo.pixelsToMeters(vs->mPoints[i].x - anchorPointP.x), gWorldInfo.pixelsToMeters(vs->mPoints[i].y - anchorPointP.y));
                }



            }else{
                dbgSystem.addMessage("Unknown add-on sensor body kind '" + GetEntityRoleString(bodyKind) + "'!");
                return false;
            }
        }

    }

    dbgSystem.removeLastMessage();
    return true;

}


bool SourceBody::addSpriteLayerMainShapeFixtures(SourceEntityCfg *_sourceEntityCfg, jugimap::SpriteLayer *_spriteLayer, Vec2f _anchorPoint)
{


    //TileGrid tileGrid(Vec2i(_spriteLayer->map()->numTilesX(), _spriteLayer->map()->numTilesY()),
    //                  Vec2i(_spriteLayer->map()->tileWidth(), _spriteLayer->map()->tileHeight()));

    std::unique_ptr<TileGrid> tileGrid;

    if(_spriteLayer->spriteLayerContent()==SpriteLayerContent::GRID_SPRITES){
        tileGrid.reset(new TileGrid(Vec2i(_spriteLayer->map()->numTilesX(), _spriteLayer->map()->numTilesY()),
                                    Vec2i(_spriteLayer->map()->tileWidth(), _spriteLayer->map()->tileHeight())));
    }



    mFixtureUserDataGroup.create(_spriteLayer, _sourceEntityCfg);
    if(mFixtureUserDataGroup.fixtureUserDatas().empty()){
       dbgSystem.addMessage("No shapes found for the sprites in the sprite layer '" + _spriteLayer->name() + "'!");
        return false;
    }

    std::vector<FixtureUserData*> fuds;

    int fixtureCounter = 0;
    for(Sprite *s : _spriteLayer->sprites()){
        if(s->type()!=SpriteType::STANDARD) continue;
        StandardSprite *stdSprite = static_cast<StandardSprite*>(s);

        fuds.clear();
        mFixtureUserDataGroup.collectFixtureUserData(fuds, stdSprite->sourceSprite());

        if(fuds.empty()==false){
            if(tileGrid != nullptr){
                tileGrid->AddSprite(stdSprite);      // no harm if we add the same sprite more times !
            }
            fixtureCounter += fuds.size();
        }

    }

    mSourceFixtures.resize(fixtureCounter);


    //b2FixtureDef fixtureDef;
    //if(mBodyRole==EntityType::BLOCKING_ENVIRONMENT){
    //    fixtureDef.friction = 1.0;
    //    fixtureDef.isSensor = false;
    //}

    mBodyBoundingBox.setInitialSize();

    fixtureCounter = 0;
    for(Sprite *s : _spriteLayer->sprites()){
        if(s->type()!=SpriteType::STANDARD) continue;
        StandardSprite *stdSprite = static_cast<StandardSprite*>(s);

        //mFixtureSprites[count].sprite = stdSprite;
        //createB2ShapeForTileSprite(&mFixtureSprites[count], _bodyPos, tileGrid, mBody, _fixtureRole, false);

        fuds.clear();
        mFixtureUserDataGroup.collectFixtureUserData(fuds, stdSprite->sourceSprite());

        for(FixtureUserData *fud : fuds){

            SourceFixture &sf = mSourceFixtures[fixtureCounter];
            sf.fud = fud;
            //sf.fud->bodyKind = mBodyKind;
            //sf.fud->contactBits = _sourceEntityCfg->category->mContactBits;
            //sf.fud->contactMask = _sourceEntityCfg->category->mContactMask;
            sf.fud->category = mCategory;
            setFixtureDef(sf.fud->category, sf.mB2FixtureDef);
            sf.mB2FixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(fud);

            if(mCategory->role==EntityRole::TRANSPORTER){
                sf.createChainLoop = true;                    // needed to be tested
            }
            //---
            Vec2f spritePos = stdSprite->position() - _anchorPoint;
            //spritePos -= _bodyPos;                // ???  not needed as

            SourceImage *si = s->sourceSprite()->sourceImages().front();
            Vec2f handle = Vec2iToVec2f(si->handle());
            Vec2f anchorPointP(handle.x - si->width() * 0.5f, handle.y - si->height()*0.5f);

            spritePos -= anchorPointP;


            createB2ShapeVerticesForTileSprite(sf, spritePos, tileGrid.get());

            fixtureCounter++ ;

            //---
            BoundingBoxFloat bb = fud->vectorShape->boundingBoxF();
            bb.min.x = gWorldInfo.pixelsToMeters(spritePos.x + bb.min.x);
            bb.min.y = gWorldInfo.pixelsToMeters(spritePos.y + bb.min.y);
            bb.max.x = gWorldInfo.pixelsToMeters(spritePos.x + bb.max.x);
            bb.max.y = gWorldInfo.pixelsToMeters(spritePos.y + bb.max.y);
            mBodyBoundingBox.Unite(bb);

        }

    }



    return true;

}


bool SourceBody::addSourceFixtureFromMapVectorShape(VectorShape *_vectorShape, Vec2f dPos)
{

    dbgSystem.addMessage("Add fxture from map vector shape ...");


    assert(mSourceFixtures.size()==1);

    SourceFixture &sf = mSourceFixtures.front();
    sf.fud->vectorShape = _vectorShape;
    createB2ShapeVerticesForTileSprite(sf, dPos, nullptr);


    dbgSystem.removeLastMessage();
    return true;
}


SourceFixture* SourceBody::getSourceFixture(EntityCategory *_category, bool _setErrorMessage)
{


    for(SourceFixture &sf : mSourceFixtures){
        if(sf.fud->category==_category){
            return &sf;
        }
    }

    if(_setErrorMessage){
        if(_setErrorMessage){
            dbgSystem.addMessage("SourceFixture of category '" + _category->name + "' not found!");
        }
    }

    return nullptr;


}


/*
void SourceBody::setBodyDef(b2BodyDef &_b2BodyDef)
{



    if(mBodyProperties.bodyType==EntityBodyType::DYNAMIC){
        _b2BodyDef.type = b2BodyType::b2_dynamicBody;

    }else if(mBodyProperties.bodyType==EntityBodyType::STATIC){
        _b2BodyDef.type = b2BodyType::b2_staticBody;

    }else if(mBodyProperties.bodyType==EntityBodyType::KINEMATIC){
        _b2BodyDef.type = b2BodyType::b2_kinematicBody;

    }

    _b2BodyDef.fixedRotation = _bodyProperties.fixedRotation;

}
*/


void SourceBody::setFixtureDef(EntityCategory *_category, b2FixtureDef &_b2FixtureDef)
{

    /*
    if(_fixtureRole==FixtureRole::ACTOR_BODY){
        _b2FixtureDef.density = 1.0;
        _b2FixtureDef.friction = 0.0f;
        _b2FixtureDef.restitution = 0.0;
        _b2FixtureDef.isSensor = false;

    }else if(_fixtureRole==FixtureRole::BLOCKING_ENVIRONMENT){
        _b2FixtureDef.density = 1.0;
        _b2FixtureDef.friction = 1.0f;
        _b2FixtureDef.restitution = 0.0;
        _b2FixtureDef.isSensor = false;

    }else if(_fixtureRole==FixtureRole::ACTOR_GROUND_SENSOR){
        _b2FixtureDef.density = 0.0f;
        _b2FixtureDef.friction = 0.0f;
        _b2FixtureDef.restitution = 0.0;
        _b2FixtureDef.isSensor = true;

    }else if(_fixtureRole==FixtureRole::PATHWAY_SENSOR || _fixtureRole==FixtureRole::AREA_SENSOR){
        _b2FixtureDef.density = 0.0f;
        _b2FixtureDef.friction = 0.0f;
        _b2FixtureDef.restitution = 0.0;
        _b2FixtureDef.isSensor = true;

    }else if(_fixtureRole==FixtureRole::ENVIRONMENT_OBJECT_SENSOR){
        _b2FixtureDef.density = 0.0f;
        _b2FixtureDef.friction = 0.0f;
        _b2FixtureDef.restitution = 0.0;
        _b2FixtureDef.isSensor = true;

    }else if(_fixtureRole==FixtureRole::ENVIRONMENT_OBJECT_SOLID){
        _b2FixtureDef.density = 1.0f;
        _b2FixtureDef.friction = 0.0f;
        _b2FixtureDef.restitution = 0.0;
        _b2FixtureDef.isSensor = false;
    }


    if(mBodyType==EntityBodyType::SOLID){
        _b2FixtureDef.isSensor = false;

    }else if(mBodyType==EntityBodyType::SENSOR){
        _b2FixtureDef.isSensor = true;

    }
    */

    // sensor properties


    if(_category->sensor){
        _b2FixtureDef.density = 0.0f;
        _b2FixtureDef.friction = 0.0f;
        _b2FixtureDef.restitution = 0.0;
        _b2FixtureDef.isSensor = true;

    }else{

        if(_category->mB2BodyType == b2BodyType::b2_dynamicBody){
            _b2FixtureDef.density = 1.0;
            _b2FixtureDef.friction = 0.0f;
            _b2FixtureDef.restitution = 0.0;
            if(_category->role==EntityRole::ACTOR){

            }else if(_category->role==EntityRole::MOVABLE_OBJECT){
                _b2FixtureDef.density = 10.0;
            }


        }else{
            _b2FixtureDef.density = 1.0;
            _b2FixtureDef.friction = 1.0f;
            _b2FixtureDef.restitution = 0.0;
            _b2FixtureDef.isSensor = false;

        }

        _b2FixtureDef.isSensor = false;

    }





}



float SourceBody::centerToTopDistance()
{

    //return  mBodyBoundingBox.GetHeight() - mAnchorPoint.y;

    // mAnhorPoint is relative to center of image.

    return  mBodyBoundingBox.GetHeight()/2.0f - mAnchorPoint.y;

}


float SourceBody::centerToBottomDistance()
{

    //return mAnchorPoint.y;

    return  mBodyBoundingBox.GetHeight()/2.0f + mAnchorPoint.y;
}


float SourceBody::centerToLeftDistance()
{

    return  mBodyBoundingBox.GetWidth()/2.0f + mAnchorPoint.x;
}


float SourceBody::centerToRightDistance()
{
    return  mBodyBoundingBox.GetWidth()/2.0f - mAnchorPoint.x;
}




/*
bool SourceBody::ObtainFixtureUserDatas(SourceSprite *_sourceSprite)
{


    if(_sourceSprite->kind()!=SpriteType::STANDARD){
        dbgSystem.addMessage("Source sprite is not of type 'Standard sprite'! Only standard sprites can be used as entitites!");
        return false;
    }

    SourceImage *si = _sourceSprite->sourceImages().front();
    int shapeCounter = 0;
    for(VectorShape *vs : si->spriteShapes()){
        if(vs->kind()==ShapeKind::POLYLINE || vs->kind()==ShapeKind::RECTANGLE || vs->kind()==ShapeKind::ELLIPSE){
            shapeCounter++;
        }
    }

    if(shapeCounter==0){
        dbgSystem.addMessage("Source sprite has defined no sprite shape suitable for the entity body!");
        return false;
    }

    mFixtureUserDatas.resize(shapeCounter);

    shapeCounter = 0;
    for(VectorShape *vs : si->spriteShapes()){
        if(vs->kind()==ShapeKind::POLYLINE || vs->kind()==ShapeKind::RECTANGLE || vs->kind()==ShapeKind::ELLIPSE){
            FixtureUserData &fud = mFixtureUserDatas[shapeCounter];
            fud.sourceSprite = _sourceSprite;
            fud.vectorShape = vs;
            fud.obtainNormals();
            shapeCounter++;
        }
    }


    return true;

}
*/


//=================================================================================================



bool Body::init(SourceBody *_sourceBody, Entity *_parentEntity, Vec2f _bodyPos, float _bodyAngle)
{

    mSourceBody = _sourceBody;
    mParentEntity = _parentEntity;

    b2BodyDef bodyDef = mSourceBody->B2bodyDef();
    bodyDef.position = gWorldInfo.pixelsToMeters(_bodyPos);
    bodyDef.angle = _bodyAngle * b2_pi / 180.0f ;
    bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(mParentEntity);
    mB2Body = _parentEntity->parentEntitySystem()->world()->CreateBody(&bodyDef);


    //---

    std::vector<SourceFixture> &sourceFixtures = mSourceBody->sourceFixtures();

    /*
    if(_parentEntity->type()==EntityType::PATHWAY){
        // These entities does not have defined shared vertex data in source fixtures (as the shapes are the map vector shapes)
        // We need to recreate fixture vertex data for the current vector shape!

        EPathway *e = static_cast<EPathway*>(_parentEntity);
        VectorShape *vs = e->vectorShape();
        assert(sourceFixtures.size()==1);
        SourceFixture &sf = sourceFixtures.front();
        assert(sf.fud->role==FixtureRole::PATHWAY_SENSOR);

        sf.fud->vectorShape = vs;
        createB2ShapeVerticesForTileSprite(sf, -_bodyPos, nullptr);

    }else if(_parentEntity->type()==EntityType::AREA){
        // These entities does not have defined shared vertex data in source fixtures (as the shapes are the map vector shapes)
        // We need to recreate fixture vertex data for the current vector shape!

        Actor *a = static_cast<Actor*>(_parentEntity);
        VectorShape *vs = a->vectorShape();
        assert(sourceFixtures.size()==1);
        SourceFixture &sf = sourceFixtures.front();
        assert(sf.fud->role==FixtureRole::AREA_SENSOR);

        sf.fud->vectorShape = vs;
        createB2ShapeVerticesForTileSprite(sf, -_bodyPos, nullptr);
    }

    */


    for(const SourceFixture &sf : sourceFixtures){
        if(sf.temporaryFixture){
            continue;
        }
        createFixture(sf);
    }

    return true;
}


void Body::createFixture(const SourceFixture &_sf)
{

    b2FixtureDef bodyFixtureDef = _sf.mB2FixtureDef;
   // bodyFixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(_sfd.fud);      // not needed as _sfd.mB2FixtureDef has set the user data pointer



    if(_sf.mShapeType==b2Shape::Type::e_polygon){

        b2PolygonShape shape;
        shape.Set(_sf.mVertices.data(), _sf.mVertices.size());
        bodyFixtureDef.shape = &shape;
        mB2Body->CreateFixture(&bodyFixtureDef);

    }else if(_sf.mShapeType==b2Shape::Type::e_edge){

        b2EdgeShape shape;
        //shape.SetTwoSided(_sfd.mVertices[0], _sfd.mVertices[1]);
        shape.SetOneSided(_sf.mPrevVertex, _sf.mVertices[0], _sf.mVertices[1], _sf.mNextVertex);
        bodyFixtureDef.shape = &shape;
        mB2Body->CreateFixture(&bodyFixtureDef);

    }else if(_sf.mShapeType==b2Shape::Type::e_chain){

        b2ChainShape shape;
        if(_sf.createChainLoop){
            shape.CreateLoop(_sf.mVertices.data(), _sf.mVertices.size());
        }else{
            shape.CreateChain(_sf.mVertices.data(),  _sf.mVertices.size(), _sf.mPrevVertex, _sf.mNextVertex);
        }
        bodyFixtureDef.shape = &shape;
        mB2Body->CreateFixture(&bodyFixtureDef);

    }else if(_sf.mShapeType==b2Shape::Type::e_circle){

        b2CircleShape shape;
        shape.m_p.Set( _sf.mCenter.x, _sf.mCenter.y );
        shape.m_radius = _sf.mRadius;
        bodyFixtureDef.shape = &shape;
        mB2Body->CreateFixture(&bodyFixtureDef);
    }
}


b2Fixture * Body::getFixtureOfCategory(EntityCategory *_category, bool _setErrorMessage)
{


    b2Fixture * f = mB2Body->GetFixtureList();
    while(f){
        const FixtureUserData *fud = reinterpret_cast<FixtureUserData *>(f->GetUserData().pointer);
        if(fud->category==_category){
            return f;
        }
        f = f->GetNext();
    }

    if(_setErrorMessage){
        dbgSystem.addMessage("Shape (fixture) of category '" + _category->name + "' not found!");
    }

    return nullptr;


}

/*
void Body::addTemporaryFixture(EntityCategory *_category)
{

    SourceFixture *sf = mSourceBody->getSourceFixture(_category);
    assert(sf);

    createFixture(*sf);

}
*/

void Body::destroyFixture(EntityCategory *_category)
{

    for(b2Fixture* f = mB2Body->GetFixtureList(); f; f = f->GetNext()){
        const FixtureUserData *fud = reinterpret_cast<FixtureUserData *>(f->GetUserData().pointer);
        if(fud->category==_category){
            mB2Body->DestroyFixture(f);
            break;
        }
    }

}


/*

b2Body *Body::createExtraShapeBody(const SourceFixture &_sf)
{

    b2BodyDef bodyDef;
    bodyDef.type = b2BodyType::b2_dynamicBody;
    bodyDef.position = mB2Body->GetPosition();
    bodyDef.angle = mB2Body->GetAngle();
    bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(mParentEntity);

    b2Body * body = app->entitySystem()->world()->CreateBody(&bodyDef);
    createFixture(body, _sf);

    mExtraShapeBodies.push_back(body);

    return body;

}



void Body::destroyExtraShapeBody(b2Body *_body)
{

    bool erased = false;

    for(int i = int(mExtraShapeBodies.size())-1; i>=0; i-- ){
        b2Body *body = mExtraShapeBodies[i];
        if(body==_body){
            //print("Removed contacted between entityA '" + ect.mTwoEntitiesContact.entityA()->sourceEntity()->name() +
            //       "' and entityB '" + ect.mTwoEntitiesContact.entityB()->sourceEntity()->name() + "' !");

            mExtraShapeBodies.erase(mExtraShapeBodies.begin()+i);
            erased = true;
        }
    }

    assert(erased);

    app->entitySystem()->world()->DestroyBody(_body);

}

*/


}
