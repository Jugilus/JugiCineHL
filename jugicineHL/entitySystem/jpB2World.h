#ifndef PLAYER__ENTITIES__BOX2D_WORLD__H
#define PLAYER__ENTITIES__BOX2D_WORLD__H

#include <memory>
#include <string>
#include <vector>
#include "box2d.h"
#include "jmCommon.h"
#include "jpGlobal.h"


class b2World;

namespace pugi{
class xml_node;
}

namespace jugimap{


class Sprite;
class SourceSprite;
class StandardSprite;
class VectorShape;
class Entity;
class Entity;




class WorldInfo
{
public:



    void init();

    //b2World* world(){ return mB2World.get(); }



    inline float pixelsToMeters(float pixels)
    {
        return pixels / mNumPixelsForOneMeter;

    }


    inline b2Vec2 pixelsToMeters(b2Vec2 pixels)
    {
        return { pixels.x/mNumPixelsForOneMeter, pixels.y/mNumPixelsForOneMeter} ;

    }

    inline b2Vec2 pixelsToMeters(Vec2f pixels)
    {
        return { pixels.x/mNumPixelsForOneMeter, pixels.y/mNumPixelsForOneMeter} ;

    }


    inline float metersToPixels(float meters)
    {
        return mNumPixelsForOneMeter * meters;

    }

    template<typename T>
    inline T metersToPixels(b2Vec2 meters)
    {
        return { mNumPixelsForOneMeter * meters.x, mNumPixelsForOneMeter * meters.y } ;

    }


    inline float minGeometryBlockMeters()
    {
        return pixelsToMeters(mMinGeometryBlockInPixels);

    }

    inline float minGeometryBlockPixels()
    {
        return mMinGeometryBlockInPixels;

    }

    inline float ceilingSensorHeightMeters(){
        return minGeometryBlockMeters() * 1.5;
    }


    //inline float minTransporterHeightInMeters()
    //{
    //    return pixelsToMeters(mMinTransporterHeightP);

    //}

    //inline float minTransporterHeightInPixels()
    //{
    //    return mMinTransporterHeightP;

    //}


    b2Vec2 pressureVelocity(b2Vec2 _groundNormal) { return b2Vec2(-_groundNormal.x * mNormalPressure, -_groundNormal.y * mNormalPressure); }


private:
    float mNumPixelsForOneMeter = 50;
    //float mMinGeometryBlockInPixels = 50;
    float mMinGeometryBlockInPixels = 40;       // should be a bit smaller than tile size

    float mNormalPressure = 5.0f;


    //float mMinTransporterHeightP = 10;

    //std::unique_ptr<b2World>mB2World;
    //b2World *mB2World = nullptr;

};



extern WorldInfo gWorldInfo;


//================================================================================================






}






#endif
