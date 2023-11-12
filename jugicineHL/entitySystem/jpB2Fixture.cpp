#include <cmath>
#include <cassert>
#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmMap.h"
#include "jmCamera.h"
#include "jmSceneLayout.h"
#include "jmCommonFunctions.h"
#include "jmSourceContainers.h"
#include "jmVectorShape.h"
#include "jmSpriteLayer.h"
#include "jmCompositeSprite.h"
#include "jmStandardSprite.h"
#include "jmUtilities.h"

#include "items/jpItemsCommon.h"
#include "jpPlayedApp.h"
#include "jpPlayedScene.h"
#include "jpUtilities.h"
//#include "jpActionsCommon.h"

#include "movements/jpMovementCommon.h"
#include "jpEntitySystem.h"
#include "jpSourceEntity.h"
#include "jpB2Fixture.h"



namespace jugimap{




//===========================================================================================


void FixtureUserData::obtainNormals()
{

    if(vectorShape->kind()==ShapeKind::POLYLINE || vectorShape->kind()==ShapeKind::RECTANGLE){

        std::vector<PathPoint>& mPoints = vectorShape->pathPoints();

        for(unsigned int i=0; i<mPoints.size(); i++){
            if(i>0){
                b2Vec2 norm = GetUnitNormal(mPoints[i-1], mPoints[i]);
                DirectionAndAngle dir = GetBlockedDirectionFromSurfaceNormal(norm);
                angles.push_back(dir.mAngle);
                blockedDirections.push_back(dir.mDirection);
                normals.push_back(norm);
                blockedDir |= static_cast<int>(dir.mDirection);
            }
        }
    }

}


//============================================================================================


void FixtureUserDataGroup::create(SourceLibrary &_sourceLibrary, SourceEntityCfg *_sourceEntityCfg)
{


    std::vector<SourceSprite*>usedSourceSprites;

    for(SourceGroup *group : _sourceLibrary.sourceGroups()){
        for(SourceSet *set : group->sourceSets()){
            for(SourceSprite *ss : set->sourceSprites()){
                if(ss->kind()==SpriteType::STANDARD){
                    usedSourceSprites.push_back(ss);
                }
            }
        }
    }

    _create(usedSourceSprites, _sourceEntityCfg);


    /*
    int count = 0;

    for(SourceGroup *group : _sourceLibrary.sourceGroups()){
        for(SourceSet *set : group->sourceSets()){
            for(SourceSprite *ss : set->sourceSprites()){
                if(ss->kind()==SpriteType::STANDARD){
                    SourceImage *si = ss->sourceImages().front();
                    for(VectorShape *vs : si->spriteShapes()){
                        if(vs->kind()==ShapeKind::POLYLINE || vs->kind()==ShapeKind::RECTANGLE || vs->kind()==ShapeKind::ELLIPSE){
                            count++;
                        }
                    }
                }
            }
        }
    }


    mFixtureUserDatas.resize(count);

    count = 0;
    for(SourceGroup *group : _sourceLibrary.sourceGroups()){
        for(SourceSet *set : group->sourceSets()){
            for(SourceSprite *ss : set->sourceSprites()){
                if(ss->kind()==SpriteType::STANDARD){
                    SourceImage *si = ss->sourceImages().front();
                    for(VectorShape *vs : si->spriteShapes()){
                        if(vs->kind()==ShapeKind::POLYLINE || vs->kind()==ShapeKind::RECTANGLE || vs->kind()==ShapeKind::ELLIPSE){
                            FixtureUserData &fud = mFixtureUserDatas[count];
                            fud.sourceSprite = ss;
                            fud.vectorShape = vs;
                            fud.obtainNormals();
                            count++;
                        }
                    }
                }
            }
        }
    }
    */

}


void FixtureUserDataGroup::create(SpriteLayer * _spriteLayer, SourceEntityCfg *_sourceEntityCfg)
{


    std::vector<SourceSprite*>usedSourceSprites;

    for(Sprite *s : _spriteLayer->sprites()){
        if(s->type()!=SpriteType::STANDARD) continue;
        SourceSprite *ss = s->sourceSprite();

        if(StdVector::contains(usedSourceSprites, ss) == false){
            usedSourceSprites.push_back(ss);
        }
    }

    _create(usedSourceSprites, _sourceEntityCfg);
}


void FixtureUserDataGroup::create(SourceSprite *_sourceSprite, SourceEntityCfg *_sourceEntityCfg)
{

        _create({_sourceSprite}, _sourceEntityCfg);

}


void FixtureUserDataGroup::_create(const std::vector<SourceSprite *> &_sourceSprites, SourceEntityCfg *_sourceEntityCfg)
{


    int count = 0;
    for(SourceSprite *ss : _sourceSprites){
        SourceImage *si = ss->sourceImages().front();
        for(VectorShape *vs : si->spriteShapes()){
            if(vs->kind()==ShapeKind::POLYLINE || vs->kind()==ShapeKind::RECTANGLE || vs->kind()==ShapeKind::ELLIPSE){
                count++;
            }
        }
    }

    mFixtureUserDatas.resize(count);

    count = 0;
    for(SourceSprite *ss : _sourceSprites){


        int fixtureProperties = FixtureProperties::NONE;
        if(ss->constantParameters().exists("ePassthroughFromBellow")){
            fixtureProperties |= FixtureProperties::PASSTHROUGH_FROM_BELLOW;
        }


        SlidingGroundCfg *slidingGroundCfg = nullptr;
        if(ss->constantParameters().exists("eSlidingGround")){
            std::string slidingGround = ss->constantParameters().value("eSlidingGround");
            slidingGroundCfg = _sourceEntityCfg->entitySystem->extraGroundTypesCfgsGroup()->getSlidingGroundCfg(slidingGround, false);
        }

        ConveyorGroundCfg * conveyorGroundCfg = nullptr;
        if(ss->constantParameters().exists("eConveyorGround")){
            std::string conveyerGround = ss->constantParameters().value("eConveyorGround");
            conveyorGroundCfg = _sourceEntityCfg->entitySystem->extraGroundTypesCfgsGroup()->getConveyorGroundCfg(conveyerGround, false);
        }


        SourceImage *si = ss->sourceImages().front();
        for(VectorShape *vs : si->spriteShapes()){
            if(vs->kind()==ShapeKind::POLYLINE || vs->kind()==ShapeKind::RECTANGLE || vs->kind()==ShapeKind::ELLIPSE){
                FixtureUserData &fud = mFixtureUserDatas[count];
                fud.sourceSprite = ss;
                fud.vectorShape = vs;
                fud.properties = fixtureProperties;
                fud.slidingGroundCfg = slidingGroundCfg;
                fud.conveyorGroundCfg = conveyorGroundCfg;
                fud.obtainNormals();
                count++;
            }
        }
    }

}


FixtureUserData *FixtureUserDataGroup::getFixtureUserData(VectorShape *_vectorShape)
{

    for(FixtureUserData &fud : mFixtureUserDatas){
        if(fud.vectorShape == _vectorShape){
            return &fud;
        }
    }

    assert(false);
    return nullptr;

}


void FixtureUserDataGroup::collectFixtureUserData(std::vector<FixtureUserData *> &collectedObjects, SourceSprite *_sourceSprite)
{

    for(FixtureUserData &fud : mFixtureUserDatas){
        if(fud.sourceSprite == _sourceSprite){
            collectedObjects.push_back(&fud);
        }
    }

}



//FixtureUserDataGroup gFixtureShapeAndDataManager;


//============================================================================================


//int FixtureUserData::idCounter = 0;


DirectionAndAngle GetBlockedDirectionFromSurfaceNormal(b2Vec2 normal)
{


    float angle = 180 * atan2(normal.y, normal.x)/b2_pi;
    if(angle<0.0f) angle += 360.0f;

    /*
    if(angle>44.0f && angle <136.0f){
        // ground
        return DirectionAndAngle(Direction::DOWN, angle);

    }else if (angle>136.0f && angle <224.0f){
        //return TerrainSurfaceDirection::RIGHT_WALL;
        return DirectionAndAngle(Direction::RIGHT, angle);


    }else if (angle>224.0f && angle <314.0f){
        //return TerrainSurfaceDirection::CEILING;
        return DirectionAndAngle(Direction::UP, angle);
    }
    */
    if(angle>0.01 && angle <179.9){
        // ground
        return DirectionAndAngle(Direction::DOWN, angle);

    }else if (angle>136.0f && angle <224.0f){
        //return TerrainSurfaceDirection::RIGHT_WALL;
        return DirectionAndAngle(Direction::RIGHT, angle);


    }else if (angle>224.0f && angle <314.0f){
        //return TerrainSurfaceDirection::CEILING;
        return DirectionAndAngle(Direction::UP, angle);
    }

    //return TerrainSurfaceDirection::LEFT_WALL;
    return DirectionAndAngle(Direction::LEFT, angle);

}


//============================================================================================

/*
unsigned int GetFixtureRoleCollisionBits(FixtureRole role)
{

    unsigned int bits = 0;


    if(role == FixtureRole::ACTOR_BODY){
        bits |= static_cast<unsigned int>(FixtureRole::BLOCKING_ENVIRONMENT);
        bits |= static_cast<unsigned int>(FixtureRole::PATHWAY_SENSOR);
        bits |= static_cast<unsigned int>(FixtureRole::ENVIRONMENT_OBJECT_SENSOR);

    }else if(role == FixtureRole::ACTOR_GROUND_SENSOR){
        bits |= static_cast<unsigned int>(FixtureRole::BLOCKING_ENVIRONMENT);

    }else if(role == FixtureRole::BLOCKING_ENVIRONMENT){
        bits |= static_cast<unsigned int>(FixtureRole::ACTOR_BODY);
        bits |= static_cast<unsigned int>(FixtureRole::ACTOR_GROUND_SENSOR);

    }else if(role == FixtureRole::PATHWAY_SENSOR){
        bits |= static_cast<unsigned int>(FixtureRole::ACTOR_BODY);

    }else if(role == FixtureRole::ENVIRONMENT_OBJECT_SENSOR){
        bits |= static_cast<unsigned int>(FixtureRole::ACTOR_BODY);

    }


    return bits;

}
*/



}
