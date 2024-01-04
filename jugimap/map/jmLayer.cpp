#include <assert.h>
#include <algorithm>
#include "jmSystem.h"
#include "jmParameter.h"
#include "jmSourceSprite.h"
#include "jmSprite.h"
#include "jmSpriteLayer.h"
#include "jmCompositeSprite.h"
#include "jmVectorShape.h"
#include "jmUtilities.h"
#include "jmCamera.h"
#include "jmMap.h"
#include "jmLayer.h"


using namespace jugimap;



namespace jugimap {


int ParallaxAndScreenLayerParameters::typeLastSet = ParallaxAndScreenLayerParameters::typePARALLAX_LAYER;


//--------------------------------------------------------------------------------------

Layer::Layer() : LayerElement()
{
    mPP.type = ParallaxAndScreenLayerParameters::typeLastSet;
}


Layer::Layer(Map *_map) : LayerElement()
{
    mMap = _map;
    mPP.type = ParallaxAndScreenLayerParameters::typeLastSet;
}


Layer::Layer(const std::string &_name, Map *_map) : LayerElement(_name)
{
    mMap = _map;
    mPP.type = ParallaxAndScreenLayerParameters::typeLastSet;
}


Layer::Layer(const Layer &_SrcLayer) : LayerElement(_SrcLayer)
{

    //mMap = _SrcLayer.mMap;
    //mParentComposedSprite = _SrcLayer.mParentComposedSprite;
    //mZOrder = _SrcLayer.mZOrder;
    mWorldBoundingBox = _SrcLayer.mWorldBoundingBox;
    mPP = _SrcLayer.mPP;
    mLayerType = _SrcLayer.mLayerType;
    mLayersPlaneSize = _SrcLayer.mLayersPlaneSize;
    mParallaxOffset = _SrcLayer.mParallaxOffset;

}




bool Layer::UpdateParallaxOffset()
{

    WorldMapCamera *camera = dynamic_cast<WorldMapCamera*>(map()->camera());
    assert(camera);
    Vec2i mapSize = camera->GetWorldSize();
    Vec2f pointedWorldPos = camera->GetPointedPosition();
    BoundingBoxFloat mapScrollableRect = camera->GetMapScrollableRect();

    float fX = 0.0;
    if(mapScrollableRect.GetWidth()>0){
        fX = (pointedWorldPos.x-mapScrollableRect.min.x)/(mapScrollableRect.GetWidth());
    }

    float fY = 0.0;
    if(mapScrollableRect.GetHeight()>0){
        fY = (pointedWorldPos.y-mapScrollableRect.min.y)/(mapScrollableRect.GetHeight());
    }

    Vec2f pRange(mapScrollableRect.GetWidth()*(1.0-mPP.parallaxFactor.x), mapScrollableRect.GetHeight()*(1.0-mPP.parallaxFactor.y));

    //----
    float xAlignLEFT = pRange.x * fX;
    float xAlignRIGHT = mapSize.x - mLayersPlaneSize.x - pRange.x*(1-fX);
    float yAlignBOTTOM = pRange.y * fY;
    float yAlignTOP = mapSize.y - mLayersPlaneSize.y - pRange.y*(1-fY);

    if(settings.yOrientation()==Yorientation::DOWN){
        yAlignTOP = pRange.y * fY;
        yAlignBOTTOM = mapSize.y - mLayersPlaneSize.y - pRange.y*(1-fY);
    }

    //----
    Vec2f parallaxOffsetCurrent;
    parallaxOffsetCurrent.x = xAlignLEFT * (1.0 - mPP.alignPosition.x/100.0) +  xAlignRIGHT * (mPP.alignPosition.x/100.0);
    //parallaxOffsetCurrent.y = yAlignTOP * (1.0 - mPP.alignPosition.y/100.0) +  yAlignBOTTOM * (mPP.alignPosition.y/100.0);
    parallaxOffsetCurrent.y = yAlignBOTTOM * (1.0 - mPP.alignPosition.y/100.0) +  yAlignTOP * (mPP.alignPosition.y/100.0);      // need to be tested !!!

    parallaxOffsetCurrent = parallaxOffsetCurrent + Vec2iToVec2f(mPP.alignOffset);

    //----
    if(IsEqual(parallaxOffsetCurrent, mParallaxOffset)==false){
        mParallaxOffset = parallaxOffsetCurrent;
        return true;
    }

    return false;
}



CompositeSprite* Layer::rootCompositeSprite()
{

    CompositeSprite* cs = mParentCompositeSprite;
    while(cs){
        CompositeSprite *csParent = cs->parentLayer()->parentCompositeSprite();
        if(csParent){
            cs = csParent;
        }else{
            break;
        }
    }

    return cs;

}



Map *Layer::rootMap()
{

    CompositeSprite* rootCS = rootCompositeSprite();
    if(rootCS){
        return rootCS->parentLayer()->map();
    }

    return mMap;

}



}



