#include <algorithm>
#include <utility>
#include <map>
#include <assert.h>
#include "jmCommon.h"
#include "jmSystem.h"
#include "jmVectorShape.h"
#include "jmUtilities.h"
#include "jmCamera.h"
//#include "jmGuiBar.h"
//#include "jmGuiText.h"
//#include "jmGuiTextInput.h"
#include "jmLayer.h"
#include "jmSpriteLayer.h"
#include "jmVectorLayer.h"
#include "jmVectorShapeDrawing.h"
#include "jmVectorShapeUtilities.h"
#include "jmSprite.h"
#include "jmSpriteModifiers.h"
#include "jmStandardSprite.h"
#include "jmCompositeSprite.h"
#include "jmSourceSprite.h"
#include "jmMap.h"


namespace jugimap {



//--------------------------------------------------------------------------------


/*
void Map::deleteWidgets()
{

    for(GuiWidget * w : mWidgets){
        if(w==GuiWidget::GetInteracted()){
            GuiWidget::_SetInteractedWidget(nullptr);
        }
        delete w;
    }
    mWidgets.clear();

}
*/

void Map::initCustomParameters()
{

    for(CParameter &PV : mParameters){
        // nothing from the editor
    }


    /*
    for(ncine::SceneNode* n: nodeChildren()){
        BaseLayer *l = static_cast<LayerNode*>(n)->baseLayer;
        if(l->kind()==LayerKind::SPRITE_LAYER){
            static_cast<SpriteLayer*>(l)->initCustomParameters();

        }if(l->kind()==LayerKind::SPRITE_LAYERS_GROUP){
            static_cast<SpriteLayersGroup*>(l)->initCustomParameters();

        }if(l->kind()==LayerKind::VECTOR_LAYERS_GROUP){
            static_cast<VectorLayersGroup*>(l)->initCustomParameters();

        }else{
            assert(false);
        }

    }
    */


    for(Layer *l :mLayers){
        l->updatePropertiesViaExtraParameters();
    }

}


void Map::clearExternalDataLinks()
{
    for(Layer *l : mLayers){
        l->clearExternalDataLinks();
    }
}


void Map::init(MapType _mapType)
{

    mType = _mapType;

    //UpdateLayersVectors();        // this should not be needed !


    //for(ncine::SceneNode* n: nodeChildren()){
    //    Layer *l = static_cast<LayerNode*>(n)->baseLayer;
    for(Layer *l : mLayers){
        l->init();
    }

    //----
    //rebuildWidgets();
    //setWidgetsToInitialState();

    //---- transform ncine nodes, this will provide abs coordinates and updated bounding box and colliders for sprites

    //setEngineNodesEnabled(true);    // Must set child nodes to enabled so that node()->update() updates all !
    unsigned long int lastFrameUpdated = mNode->getLastFrameUpdated();
    node()->update(1);
    mNode->setLastFrameUpdated(lastFrameUpdated);
    //setEngineNodesEnabled(false);   // Return enabled status to correct one!

}


void Map::initAsWorldMap()
{

    assert(mType==MapType::WORLD);

    //for(ncine::SceneNode* n: nodeChildren()){
    //    Layer *l = static_cast<LayerNode*>(n)->baseLayer;
    for(Layer *l : mLayers){
        l->_setLayerType(LayerType::WORLD);         // Not really needed for world maps but lets make it correct as a loaded map has layers defined as parallax or screen layers.
    }

    //----
    initCustomParameters();
    updateBoundingBox(true);
    normalize();
    mWorldMapSize = Vec2i(mWorldBoundingBox.GetWidth(), mWorldBoundingBox.GetHeight());

    //----
    //setEngineNodesEnabled(true);    // Must set child nodes to enabled so that node()->update() updates all !
    unsigned long int lastFrameUpdated = mNode->getLastFrameUpdated();
    mNode->update(1);
    mNode->setLastFrameUpdated(lastFrameUpdated);
    //setEngineNodesEnabled(false);   // Return enabled status to correct one!
}


void Map::initAsParallaxMap(Vec2i _worldMapSize)
{

    //for(ncine::SceneNode* n: nodeChildren()){
    //    Layer *l = static_cast<LayerNode*>(n)->baseLayer;
    for(Layer *l : mLayers){
        if(l->layerType()!=LayerType::PARALLAX && l->layerType()!=LayerType::PARALLAX_STRETCHING_SINGLE_SPRITE){
            l->_setLayerType(LayerType::PARALLAX);
        }
    }

    mWorldMapSize = _worldMapSize;

    initCustomParameters();
    updateBoundingBox(true);
    normalize();
    setLayersPlanes();

    //----
    //setEngineNodesEnabled(true);    // Must set child nodes to enabled so that node()->update() updates all !
    unsigned long int lastFrameUpdated = mNode->getLastFrameUpdated();
    mNode->update(1);
    mNode->setLastFrameUpdated(lastFrameUpdated);
    //setEngineNodesEnabled(false);   // Return enabled status to correct one!

}


void Map::initAsScreenMap(Vec2i _screenMapDesignSize)
{

    assert(mType==MapType::SCREEN);

    //for(ncine::SceneNode* n: nodeChildren()){
    //    Layer *l = static_cast<LayerNode*>(n)->baseLayer;
    for(Layer *l : mLayers){
        if(l->layerType()!=LayerType::SCREEN && l->layerType()!=LayerType::SCREEN_STRETCHING_SINGLE_SPRITE){
            l->_setLayerType(LayerType::SCREEN);
        }
    }
    mScreenMapDesignSize = _screenMapDesignSize;


    initCustomParameters();
    updateBoundingBox(true);
    normalize();
    setLayersPlanes();

    //--- align layer sprites
    for(Layer *l : mLayers){

        if(l->name()=="settings menu"){
            DummyFunction();
        }
        if(l->layerType()==LayerType::SCREEN){



            float xAlignLEFT = 0.0;
            float xAlignRIGHT = mScreenMapDesignSize.x - l->mLayersPlaneSize.x;
            float yAlignTOP = 0.0;
            float yAlignBOTTOM = mScreenMapDesignSize.y - l->mLayersPlaneSize.y;

            if(settings.yOrientation()==Yorientation::UP){
                yAlignTOP = mScreenMapDesignSize.y - l->mLayersPlaneSize.y;
                yAlignBOTTOM = 0.0;
            }

            Vec2f alignPos;
            alignPos.x = xAlignLEFT * (1.0 - l->pp().alignPosition.x/100.0) +  xAlignRIGHT * (l->pp().alignPosition.x/100.0);
            //alignPos.y = yAlignTOP * (1.0 - l->pp().alignPosition.y/100.0) +  yAlignBOTTOM * (l->pp().alignPosition.y/100.0);
            alignPos.y = yAlignBOTTOM * (1.0 - l->pp().alignPosition.y/100.0) +  yAlignTOP * (l->pp().alignPosition.y/100.0);
            alignPos += Vec2iToVec2f(l->pp().alignOffset);


            if(l->kind()==LayerKind::SPRITE_LAYER){
            //----------------------------------------------------
                SpriteLayer * sl = static_cast<SpriteLayer*>(l);

                for(Sprite *s : sl->sprites()){
                    Vec2f pos = s->position();
                    pos += alignPos;
                    s->setPosition(pos);
                }
                sl->mWorldBoundingBox.min += alignPos;
                sl->mWorldBoundingBox.max += alignPos;


            }else if(l->kind()==LayerKind::VECTOR_LAYER){
            //----------------------------------------------------
                VectorLayer * vl = static_cast<VectorLayer*>(l);

                for(VectorShape *vs : vl->mShapes){
                    MoveGeometricShape(vs, alignPos);
                }

            }

        }else{

            // empty - we handle stretch modes at layer update

        }
    }

    //----
    //setEngineNodesEnabled(true);    // Must set child nodes to enabled so that node()->update() updates all !
    unsigned long int lastFrameUpdated = mNode->getLastFrameUpdated();
    mNode->update(1);
    mNode->setLastFrameUpdated(lastFrameUpdated);
    //setEngineNodesEnabled(false);   // Return enabled status to correct one!
}



void Map::updateGeometry(int flags)
{

    /*
    //--- enable layer nodes in case some are hidden
    for(ncine::SceneNode* n: nodeChildren()){
        LayerElement *l = static_cast<LayerNode*>(n)->layerElement;
        n->setEnabled(true);

        if(l->kind()==LayerKind::SPRITE_LAYERS_GROUP || l->kind()==LayerKind::VECTOR_LAYERS_GROUP){
            for(ncine::SceneNode* n2: l->nodeChildren()){
                n2->setEnabled(true);
            }
        }
    }
    */

    //----
    unsigned long int lastFrameUpdated = mNode->getLastFrameUpdated();
    node()->update(1);
    updateBoundingBox(flags);
    mNode->setLastFrameUpdated(lastFrameUpdated);


    /*
    //--- set enable to correct values
    for(ncine::SceneNode* n: nodeChildren()){
        LayerElement *l = static_cast<LayerNode*>(n)->layerElement;
        n->setEnabled(!l->isHidden());

        if(l->kind()==LayerKind::SPRITE_LAYERS_GROUP || l->kind()==LayerKind::VECTOR_LAYERS_GROUP){
            for(ncine::SceneNode* n2: l->nodeChildren()){
                LayerElement *l2 = static_cast<LayerNode*>(n2)->layerElement;
                n2->setEnabled(!l2->isHidden());
            }
        }
    }
    */

}


void Map::setEngineNodesEnabled(bool _enableAll)
{

    for(ncine::SceneNode* n: nodeChildren()){
        LayerElement *l = static_cast<LayerNode*>(n)->layerElement;

        if(l->kind()==LayerKind::SPRITE_LAYER){
            l->setEngineNodesEnabled(_enableAll);

        }else if (l->kind()==LayerKind::SPRITE_LAYERS_GROUP){
            for(ncine::SceneNode* n2: l->nodeChildren()){
                LayerElement *l2 = static_cast<LayerNode*>(n2)->layerElement;
                l2->setEngineNodesEnabled(_enableAll);
            }
        }
    }

}


void Map::updateBoundingBox(int flags)
{

    print("UpdateBoundingBox map:" + mName);

    mWorldBoundingBox.setInitialSize();
    for(SpriteLayer* sl : mSpriteLayers){
        sl->updateBoundingBox();
        mWorldBoundingBox.Unite(sl->mWorldBoundingBox);

        print("layer:"+ sl->mName + " BB min x:" + std::to_string(sl->mWorldBoundingBox.min.x) + " y:" + std::to_string(sl->mWorldBoundingBox.min.y)
                                   + " BB max x:" + std::to_string(sl->mWorldBoundingBox.max.x) + " y:" + std::to_string(sl->mWorldBoundingBox.max.y));
    }

    if(mWorldBoundingBox.min.x > mWorldBoundingBox.max.x ){
        mWorldBoundingBox.min.set(0,0);
        mWorldBoundingBox.max.set(0,0);
    }

    print("map:"+ mName + " BB min x:" + std::to_string(mWorldBoundingBox.min.x) + " y:" + std::to_string(mWorldBoundingBox.min.y)
                           + " BB max x:" + std::to_string(mWorldBoundingBox.max.x) + " y:" + std::to_string(mWorldBoundingBox.max.y));


#ifdef JUGI_EDITOR

    mTilesBoundingBoxOLD = mTilesBoundingBox;

    mNumSprites = 0;
    mTilesBoundingBox.setInitialSize();

    for(SpriteLayer* L : mSpriteLayers){
        if(L->mSprites.size()>0){
            mTilesBoundingBox.Unite(L->mTilesBoundingBox);
        }
        mNumSprites += L->mSprites.size();
    }

    //if(mTilesBoundingBox.IsInitialSize()){
    //    mTilesBoundingBox.setEmptySize();
    //}

    OnUpdatedBoundingBox(flags);

#endif

    mBoundingBoxUpdateRequired = false;
}


void Map::rebuildQuickAccessContainers()
{

    mLayers.clear();
    mSpriteLayers.clear();
    mVectorLayers.clear();

    for(ncine::SceneNode* n: nodeChildren()){
        LayerElement *l = static_cast<LayerNode*>(n)->layerElement;

        if(l->kind()==LayerKind::SPRITE_LAYER){
            mLayers.push_back(static_cast<Layer*>(l));
            mSpriteLayers.push_back(static_cast<SpriteLayer*>(l));

        }else if(l->kind()==LayerKind::VECTOR_LAYER){
            mLayers.push_back(static_cast<Layer*>(l));
            mVectorLayers.push_back(static_cast<VectorLayer*>(l));

        }else if(l->kind()==LayerKind::SPRITE_LAYERS_GROUP){

            for(ncine::SceneNode* n2: l->nodeChildren()){
                LayerElement *l2 = static_cast<LayerNode*>(n2)->layerElement;    assert(l2->kind()==LayerKind::SPRITE_LAYER);
                mLayers.push_back(static_cast<Layer*>(l2));
                mSpriteLayers.push_back(static_cast<SpriteLayer*>(l2));
            }

        }else if(l->kind()==LayerKind::VECTOR_LAYERS_GROUP){

            for(ncine::SceneNode* n2: l->nodeChildren()){
                LayerElement *l2 = static_cast<LayerNode*>(n2)->layerElement;    assert(l2->kind()==LayerKind::VECTOR_LAYER);
                mLayers.push_back(static_cast<Layer*>(l2));
                mVectorLayers.push_back(static_cast<VectorLayer*>(l2));
            }

        }else{
            assert(false);
        }

    }
}


void Map::normalize()
{

    if(IsEqual(mWorldBoundingBox.min, Vec2f(0.0, 0.0))) return;

    //---
    //for(ncine::SceneNode* n: nodeChildren()){
    //    Layer *l = static_cast<LayerNode*>(n)->baseLayer;
    for(Layer *l : mLayers){

        if(l->kind()==LayerKind::SPRITE_LAYER){
        //----------------------------------------------------
            SpriteLayer * sl = static_cast<SpriteLayer*>(l);

            // normalize sprite positions
            for(Sprite *s : sl->sprites()){
                Vec2f pos = s->position();
                pos -= mWorldBoundingBox.min;
                s->setPosition(pos);
            }

            sl->mWorldBoundingBox.min -=  mWorldBoundingBox.min;
            sl->mWorldBoundingBox.max -=  mWorldBoundingBox.min;

        }else if(l->kind()==LayerKind::VECTOR_LAYER){
        //----------------------------------------------------
            VectorLayer * vl = static_cast<VectorLayer*>(l);

            for(VectorShape *vs : vl->mShapes){
                MoveGeometricShape(vs, -mWorldBoundingBox.min);
            }
        }
    }

    mWorldBoundingBox = BoundingBoxFloat(0,0, mWorldBoundingBox.GetWidth(), mWorldBoundingBox.GetHeight());



    mWorldBoundingBox = BoundingBoxFloat(999999.0, 999999.0, -999999.0, -999999.0);

    //for(ncine::SceneNode* n: nodeChildren()){
    //    Layer *l = static_cast<LayerNode*>(n)->baseLayer;
    for(Layer *l : mLayers){
        if(l->kind()==LayerKind::SPRITE_LAYER){
            SpriteLayer * sl = static_cast<SpriteLayer*>(l);
            mWorldBoundingBox.Unite(sl->mWorldBoundingBox);

            //print("layer:"+ l->name + " BB min x:" + std::to_string(sl->boundingBox.min.x) + " y:" + std::to_string(sl->boundingBox.min.y)
            //                           + " BB max x:" + std::to_string(sl->boundingBox.max.x) + " y:" + std::to_string(sl->boundingBox.max.y));
        }
    }
    print("normalized map:"+ mName + " BB min x:" + std::to_string(mWorldBoundingBox.min.x) + " y:" + std::to_string(mWorldBoundingBox.min.y)
                           + " BB max x:" + std::to_string(mWorldBoundingBox.max.x) + " y:" + std::to_string(mWorldBoundingBox.max.y));


    mNumTilesX = int(mWorldBoundingBox.GetWidth()) / mTileWidth;
    if(int(mWorldBoundingBox.GetWidth()) % mTileWidth > 0) {
        mNumTilesX++;
    }

    mNumTilesY = int(mWorldBoundingBox.GetHeight()) / mTileHeight;
    if(int(mWorldBoundingBox.GetHeight()) % mTileHeight > 0) {
        mNumTilesY++;
    }

}


void Map::setLayersPlanes()
{

    // Parallax and screen maps can have more layers which visually form one layers plane. Layers which form a layers plane must have the same
    // parallax (or screen) parameters! The layers planes are independent from each other - in the code bellow we obtain sizes for layers planes and
    // normalize the layers relative to their plane.


    //--- layers of the same plane are obtained via 'attachToLayer' name and share the same parameters
    //for(Layer *l : layers){

    //for(ncine::SceneNode* n: nodeChildren()){
    //    Layer *l = static_cast<LayerNode*>(n)->baseLayer;
    for(Layer *l : mLayers){
        if(mType==MapType::PARALLAX && l->layerType()!=LayerType::PARALLAX) continue;
        if(mType==MapType::SCREEN && l->layerType()!=LayerType::SCREEN) continue;


        if(l->mPP.attachTolayer.empty()){
            l->mPP.attachTolayer = l->mName;

        }else{
            Layer *aLayer = FindLayerWithName(this, l->pp().attachTolayer);
            if(aLayer){
                l->mPP.parallaxFactor = aLayer->mPP.parallaxFactor;
                l->mLayerType = aLayer->mLayerType;
                l->mPP.alignPosition = aLayer->mPP.alignPosition;
                l->mPP.alignOffset = aLayer->mPP.alignOffset;
                l->mPP.alignOffset_ObtainFromMap = aLayer->mPP.alignOffset_ObtainFromMap;
                l->mPP.tilingCount = aLayer->mPP.tilingCount;
                l->mPP.tilingCountAutoSpread = aLayer->mPP.tilingCountAutoSpread;
                l->mPP.tilingSpacing = aLayer->mPP.tilingSpacing;
                l->mPP.tilingSpacingDelta = aLayer->mPP.tilingSpacingDelta;
            }
        }
    }


    //----- gather layers belonging to same plane (sharing the same 'linkedLayerName')
    std::map<std::string, std::vector<Layer*>>layersPlanes;
    std::map<std::string, std::vector<Layer*>>::iterator it;

    //for(Layer *l : layers){
    //for(ncine::SceneNode* n: nodeChildren()){
    //    Layer *l = static_cast<LayerNode*>(n)->baseLayer;
    for(Layer *l : mLayers){
        if(mType==MapType::PARALLAX && l->layerType()!=LayerType::PARALLAX) continue;
        if(mType==MapType::SCREEN && l->layerType()!=LayerType::SCREEN) continue;

        std::pair<std::map<std::string, std::vector<Layer*>>::iterator, bool> p = layersPlanes.insert(std::pair<std::string, std::vector<Layer*>>(l->mPP.attachTolayer, std::vector<Layer*>()));
        it = p.first;
        std::vector<Layer*> &planeLayers = it->second;
        planeLayers.push_back(l);
    }


    //-----
    for(it=layersPlanes.begin(); it!=layersPlanes.end(); it++){

        std::vector<Layer*> &planeLayers = it->second;

        //--- obtain plane bounding box
        BoundingBoxFloat planeBoundingBox(999999.0, 999999.0, -999999.0, -999999.0);

        for(Layer *l : planeLayers){
            if(l->kind()==LayerKind::SPRITE_LAYER){
                SpriteLayer * sl = static_cast<SpriteLayer*>(l);
                planeBoundingBox.Unite(sl->mWorldBoundingBox);
            }
        }

        print("Layer: "+ planeLayers.front()->name() + "  planeBoundingBox BB min x:" + std::to_string(planeBoundingBox.min.x) + " y:" + std::to_string(planeBoundingBox.min.y)
                               + " BB max x:" + std::to_string(planeBoundingBox.max.x) + " y:" + std::to_string(planeBoundingBox.max.y));
        print("Layer: "+ planeLayers.front()->name() + "  planeBoundingBox BB width:" + std::to_string(planeBoundingBox.GetWidth()) + " height:" + std::to_string(planeBoundingBox.GetHeight()));


        if(planeBoundingBox.IsValid()==false){
            continue;
        }

        //--- get align offsets from map if needed
        for(Layer *l : planeLayers){
            if(l->mPP.alignOffset_ObtainFromMap.x){

                if(l->mPP.alignPosition.x==0){      // left
                    l->mPP.alignOffset.x = planeBoundingBox.min.x - mWorldBoundingBox.min.x;

                }else if(l->mPP.alignPosition.x==100){      // right
                    l->mPP.alignOffset.x = planeBoundingBox.max.x - mWorldBoundingBox.max.x;
                }
            }

            if(l->mPP.alignOffset_ObtainFromMap.y){

                float yAlignBottomOffset = planeBoundingBox.max.y - mWorldBoundingBox.max.y;
                float yAlignTopOffset = planeBoundingBox.min.y - mWorldBoundingBox.min.y;


                if(settings.yOrientation()==Yorientation::UP){
                    yAlignBottomOffset = planeBoundingBox.min.y - mWorldBoundingBox.min.y;
                    yAlignTopOffset = planeBoundingBox.max.y - mWorldBoundingBox.max.y;
                }

                if(l->mPP.alignPosition.y==0){      // bottom
                    //l->mPP.alignOffset.y = yAlignTopOffset;
                    l->mPP.alignOffset.y = yAlignBottomOffset;

                }else if(l->mPP.alignPosition.y==100){  // top
                    //l->mPP.alignOffset.y = yAlignBottomOffset;
                    l->mPP.alignOffset.y = yAlignTopOffset;
                }
            }

            print("alignOffset layer: "+ l->name() + " alignOffset x:" + std::to_string(l->mPP.alignOffset.x) + " y:" + std::to_string(l->mPP.alignOffset.y));
        }

        //--- normalize layers relative to plane bounding box
        for(Layer *l : planeLayers){

            if(l->kind()==LayerKind::SPRITE_LAYER){
            //----------------------------------------------------
                SpriteLayer * sl = static_cast<SpriteLayer*>(l);

                // normalize sprite positions
                for(Sprite *s : sl->sprites()){
                    Vec2f pos = s->position();
                    pos -= planeBoundingBox.min;
                    s->setPosition(pos);
                }
                sl->mWorldBoundingBox.min -=  planeBoundingBox.min;
                sl->mWorldBoundingBox.max -=  planeBoundingBox.min;


            }else if(l->kind()==LayerKind::VECTOR_LAYER){
            //----------------------------------------------------
                VectorLayer * vl = static_cast<VectorLayer*>(l);

                for(VectorShape *vs : vl->mShapes){
                    MoveGeometricShape(vs, -planeBoundingBox.min);
                }
                vl->mWorldBoundingBox.min -=  planeBoundingBox.min;
                vl->mWorldBoundingBox.max -=  planeBoundingBox.min;
            }

            print("Layer: " + l->name()+ "  BB after normalize  min x:" + std::to_string(l->mWorldBoundingBox.min.x) + " y:" + std::to_string(l->mWorldBoundingBox.min.y)
                                   + " BB max x:" + std::to_string(l->mWorldBoundingBox.max.x) + " y:" + std::to_string(l->mWorldBoundingBox.max.y));


            //---
            l->mLayersPlaneSize = Vec2f(planeBoundingBox.GetWidth(),planeBoundingBox.GetHeight());

            // some safety stuff which should not happen
            if(l->mLayersPlaneSize.x < 1){
                l->mLayersPlaneSize.x = 50;
                assert(false);
            }
            if(l->mLayersPlaneSize.y < 1){
                l->mLayersPlaneSize.y = 50;
                assert(false);
            }
        }

        //--- tile layers
        Vec2f layersPlaneSize = planeLayers.front()->mLayersPlaneSize;
        LayerType layerType = planeLayers.front()->mLayerType;
        Vec2i tilingCount = planeLayers.front()->mPP.tilingCount;
        Vec2i tilingCountAutoSpread = planeLayers.front()->mPP.tilingCountAutoSpread;
        Vec2i tilingSpacing = planeLayers.front()->mPP.tilingSpacing;
        Vec2i tilingSpacingDelta = planeLayers.front()->mPP.tilingSpacingDelta;
        Vec2f parallaxFactor = planeLayers.front()->mPP.parallaxFactor;

        std::vector<std::vector<Sprite*>>origSprites(planeLayers.size());
        std::vector<std::vector<VectorShape*>>origShapes(planeLayers.size());

        if(tilingCount.x>1 || tilingCountAutoSpread.x==1){

            float widthMax = 0;
            if(layerType==LayerType::PARALLAX){
                widthMax = mWorldMapSize.x * parallaxFactor.x;

                // There are situations depending of the view position and zoom factor that require bigger 'widthRequired'!
                // To not complicate lets make it as big as map size!
                if(widthMax<mWorldMapSize.x){
                    widthMax = mWorldMapSize.x;
                }

            }else if(layerType==LayerType::SCREEN){
                widthMax = mScreenMapDesignSize.x;
            }


            float xStepDef = layersPlaneSize.x;
            float xStepCumulative = 0.0;
            int count = 1;

            while(layersPlaneSize.x<widthMax){

                float xStepVar = iRand(-tilingSpacingDelta.x/2.0, tilingSpacingDelta.x/2.0);
                float xStep = xStepDef + tilingSpacing.x + xStepVar;
                if(xStep < xStepDef/2.0){
                    xStep = xStepDef/2.0;
                }
                if(xStep < 10){
                    xStep = 10;
                }

                for(int j=0; j<planeLayers.size(); j++){
                    Layer *l = planeLayers[j];

                    if(l->kind()==LayerKind::SPRITE_LAYER){
                    //----------------------------------------------------
                        SpriteLayer * sl = static_cast<SpriteLayer*>(l);

                        if(origSprites[j].empty()){
                            for(Sprite *s : sl->sprites()){
                                origSprites[j].push_back(s);
                            }
                        }

                        //----
                        for(Sprite *s : origSprites[j] ){
                            Vec2f pos = s->position();
                            //pos.x += xStep*i;
                            pos.x += xStepCumulative + xStep;
                            //Sprite *sprCopy = s->MakePassiveCopy();
                            Sprite *sprCopy = s->makeActiveCopy();
                            //sprCopy->GetLayer()->AddSprite(sprCopy);        // Passive copies are not inserted into layer at creation!
                            sprCopy->setPosition(pos);
                        }
                        l->mLayersPlaneSize.x += xStep;
                        l->mWorldBoundingBox.max.x += xStep;


                    }else if(l->kind()==LayerKind::VECTOR_LAYER){
                    //----------------------------------------------------
                        VectorLayer * vl = static_cast<VectorLayer*>(l);

                        if(origShapes[j].empty()){
                            origShapes[j] = vl->shapes();
                        }

                        //----
                        for(VectorShape *vs : origShapes[j]){
                            //VectorShape *vsCopy = VectorShape::Copy(vs);
                            VectorShape *vsCopy = new VectorShape(*vs);
                            vl->AddVectorShape(vsCopy);
                            //MoveGeometricShape(vsCopy->GetGeometricShape(), Vec2f(xStep*i, 0.0));
                            MoveGeometricShape(vsCopy, Vec2f(xStepCumulative + xStep, 0.0));

                        }
                        vl->mLayersPlaneSize.x += xStep;
                        vl->mWorldBoundingBox.max.x += xStep;
                    }
                }

                layersPlaneSize.x += xStep;
                xStepCumulative += xStep;

                count++;
                if(tilingCountAutoSpread.x==0 && tilingCount.x==count){
                    break;
                }
            }

        }

        //---- clear origSprites lists before tiling in y direction
        for(int j=0; j<planeLayers.size(); j++){
            origSprites[j].clear();
            origShapes[j].clear();
        }


        //----
        if(tilingCount.y>1 || tilingCountAutoSpread.y==1){

            float heightMax = 0;
            if(layerType==LayerType::PARALLAX){
                heightMax = mWorldMapSize.y * parallaxFactor.y;

                // There are situations depending of the view position and zoom factor that require bigger 'widthRequired'!
                // To not complicate lets make it as big as map size!
                if(heightMax<mWorldMapSize.y){
                    heightMax = mWorldMapSize.y;
                }

            }else if(layerType==LayerType::SCREEN){
                heightMax = mScreenMapDesignSize.y;
            }


            float yStepDef = layersPlaneSize.y;
            float yStepCumulative = 0.0;
            int count = 1;

            while(layersPlaneSize.y<heightMax){

                float yStepVar = iRand(-tilingSpacingDelta.y/2.0, tilingSpacingDelta.y/2.0);
                float yStep = yStepDef + tilingSpacing.y + yStepVar;
                if(yStep < yStepDef/2.0){
                    yStep = yStepDef/2.0;
                }
                if(yStep < 10){
                    yStep = 10;
                }

                for(int j=0; j<planeLayers.size(); j++){
                    Layer *l = planeLayers[j];

                    if(l->kind()==LayerKind::SPRITE_LAYER){
                    //----------------------------------------------------
                        SpriteLayer * sl = static_cast<SpriteLayer*>(l);

                        if(origSprites[j].empty()){
                            for(Sprite *s : sl->sprites()){
                                origSprites[j].push_back(s);
                            }
                        }

                        //----
                        for(Sprite *s : origSprites[j] ){
                            Vec2f pos = s->position();
                            //pos.y += xStep*i;
                            pos.y += yStepCumulative + yStep;
                            //Sprite *sprCopy = s->MakePassiveCopy();
                            //sprCopy->GetLayer()->AddSprite(sprCopy);        // Passive copies are not inserted into layer at creation!
                            Sprite *sprCopy = s->makeActiveCopy();
                            sprCopy->setPosition(pos);
                        }
                        l->mLayersPlaneSize.y += yStep;
                        l->mWorldBoundingBox.max.y += yStep;


                    }else if(l->kind()==LayerKind::VECTOR_LAYER){
                    //----------------------------------------------------
                        VectorLayer * vl = static_cast<VectorLayer*>(l);

                        if(origShapes[j].empty()){
                            origShapes[j] = vl->shapes();
                        }

                        //----
                        for(VectorShape *vs : origShapes[j]){
                            //VectorShape *vsCopy = VectorShape::Copy(vs);
                            VectorShape *vsCopy = new VectorShape(*vs);
                            vl->AddVectorShape(vsCopy);
                            //MoveGeometricShape(vsCopy->GetGeometricShape(), Vec2f(xStep*i, 0.0));
                            MoveGeometricShape(vsCopy, Vec2f(yStepCumulative + yStep, 0.0));

                        }
                        vl->mLayersPlaneSize.y += yStep;
                        vl->mWorldBoundingBox.max.y += yStep;
                    }
                }

                layersPlaneSize.y += yStep;
                yStepCumulative += yStep;

                count++;
                if(tilingCountAutoSpread.y==0 && tilingCount.y==count){
                    break;
                }
            }

        }



    }

}


void Map::modifyXParallaxFactorsForFixedMapWidth(Vec2i _designViewport, Vec2i _newViewport)
{

    if(_designViewport.x==_newViewport.x) return;
    if(mWorldMapSize.x <= _designViewport.x) return;

    // boundingBox is original from editor and normalized (not modified by SetLayersPlanes) !
    float fXstoredDef = (mWorldBoundingBox.GetWidth() - _designViewport.x)/float(mWorldMapSize.x - _designViewport.x);

    float fXnew = 0;
    if(mWorldBoundingBox.GetWidth()>_newViewport.x  &&  mWorldMapSize.x > _newViewport.x){
        fXnew = (mWorldBoundingBox.GetWidth() - _newViewport.x)/float(mWorldMapSize.x - _newViewport.x);
    }

    //for(Layer *l : layers){
    //for(ncine::SceneNode* n: nodeChildren()){
    //    Layer *l = static_cast<LayerNode*>(n)->baseLayer;
    for(Layer *l : mLayers){
        if(l->layerType()!=LayerType::PARALLAX) continue;

        float delta = l->mPP.parallaxFactor.x/fXstoredDef;
        Vec2f f(fXnew * delta, l->mPP.parallaxFactor.y);
        l->mPP.parallaxFactor = f;
    }

}


void Map::modifyYParallaxFactorsForFixedMapHeight(Vec2i _designViewport, Vec2i _newViewport)
{

    if(_designViewport.y==_newViewport.y) return;
    if(mWorldMapSize.y <= _designViewport.y) return;

    float fYstoredDef = (mWorldBoundingBox.GetHeight() - _designViewport.y)/float(mWorldMapSize.y - _designViewport.y);

    float fYnew = 0;
    if(mWorldBoundingBox.GetHeight()>_newViewport.y  &&  mWorldMapSize.y > _newViewport.y){
        fYnew = (mWorldBoundingBox.GetHeight() - _newViewport.y)/float(mWorldMapSize.y - _newViewport.y);
    }

    //for(Layer *l : layers){
    //for(ncine::SceneNode* n: nodeChildren()){
    //    Layer *l = static_cast<LayerNode*>(n)->baseLayer;
    for(Layer *l : mLayers){
        if(l->layerType()!=LayerType::PARALLAX) continue;

        float delta = l->mPP.parallaxFactor.y/fYstoredDef;
        Vec2f f(l->mPP.parallaxFactor.x, fYnew * delta);
        l->mPP.parallaxFactor = f;
    }

}


void Map::updateCameraDependentObjects()
{
    //if(mVisible==false) return;
    if(hidden) return;

    //for(ncine::SceneNode* n: nodeChildren()){
    //    Layer *l = static_cast<LayerNode*>(n)->baseLayer;
    for(Layer *l : mLayers){
        l->updateCameraDependentObjects();
    }

}


SpriteLayer* Map::addSpriteLayer(const std::string &_name)
{

    SpriteLayer *sl = new SpriteLayer(this);
    if(mType==MapType::SCREEN){
        sl->_setLayerType(LayerType::SCREEN);
    }else if(mType==MapType::PARALLAX){
        sl->_setLayerType(LayerType::PARALLAX);
    }else if(mType==MapType::WORLD){
        sl->_setLayerType(LayerType::WORLD);
    }
    sl->mName = _name;
    sl->mMap = this;

    /*
    int zOrder = mZOrderStart;
    for(Layer *l : mLayers){
        if(l->kind()==LayerKind::VECTOR_LAYER) continue;
        if(l->kind()==LayerKind::SPRITE_LAYER){
            zOrder = static_cast<SpriteLayer*>(l)->findLastZOrder();
        }
    }
    sl->mZOrder = zOrder + settings.GetZOrderStep();
    */
    //mNode->addChildNode(sl->node());

    addChildLayer(sl);

    sl->init();

    return sl;
}


jugimap::VectorLayer* Map::addVectorLayer(const std::string &_name)
{

    VectorLayer *vl = new VectorLayer(this);
    vl->mName = _name;
    vl->mMap = this;
    //mNode->addChildNode(vl->node());

    addChildLayer(vl);

    vl->init();

    return vl;

}


/*
void Map::setCamera(Camera *_camera)
{

    mCameras.push_back(_camera);
    if(mCamera == nullptr){
        mCamera = _camera;
    }

}
*/


void Map::drawShapes(ShapeDrawer &drawer)
{

    for(ncine::SceneNode* n : nodeChildren()){
        //Map *m = static_cast<Map*>(n);
        //m->drawShapes(drawer);
    }

}


/*
void Map::UpdateLayersVectors()
{

    mLayers.clear();
    mSpriteLayers.clear();
    mVectorLayers.clear();


    for(ncine::SceneNode* n: nodeChildren()){
        LayerElement *l = static_cast<LayerNode*>(n)->layerElement;

        if(l->kind()==LayerKind::SPRITE_LAYER){
            mLayers.push_back(static_cast<Layer*>(l));
            mSpriteLayers.push_back(static_cast<SpriteLayer*>(l));

        }else if(l->kind()==LayerKind::VECTOR_LAYER){
            mLayers.push_back(static_cast<Layer*>(l));
            mVectorLayers.push_back(static_cast<VectorLayer*>(l));

        }else if(l->kind()==LayerKind::SPRITE_LAYERS_GROUP){

            for(ncine::SceneNode* n2: l->nodeChildren()){
                LayerElement *l2 = static_cast<LayerNode*>(n2)->layerElement;    assert(l2->kind()==LayerKind::SPRITE_LAYER);
                mLayers.push_back(static_cast<Layer*>(l2));
                mSpriteLayers.push_back(static_cast<SpriteLayer*>(l2));
            }

        }else if(l->kind()==LayerKind::VECTOR_LAYERS_GROUP){

            for(ncine::SceneNode* n2: l->nodeChildren()){
                LayerElement *l2 = static_cast<LayerNode*>(n2)->layerElement;    assert(l2->kind()==LayerKind::VECTOR_LAYER);
                mLayers.push_back(static_cast<Layer*>(l2));
                mVectorLayers.push_back(static_cast<VectorLayer*>(l2));
            }

        }else{
            assert(false);
        }

    }

}
*/



void Map::eUpdateTextSprites_SetTexts(const SM_ChangeChildText &_modifier, int &_numUpdated)
{
    for(SpriteLayer* L : mSpriteLayers){
        L->eUpdateTextSprites_SetTexts(_modifier, _numUpdated);
    }
}


void Map::updateSprites_SetParameters(const SM_ChangeChildParameter &_modifier, int &_numUpdated)
{
    for(SpriteLayer* l : mSpriteLayers){
        l->updateSprites_SetParameters(_modifier, _numUpdated);
    }
    for(VectorLayer* l : mVectorLayers){
        for(VectorShape *vs : l->shapes()){
            if(vs->name()==_modifier.childSpriteTag){
                for(CParameter &p : vs->parameters().list()){
                    if(p.name==_modifier.parameterName){
                        p.value = _modifier.parameterValue;
                        _numUpdated++;
                        break;
                    }
                }
            }
        }
    }

}


void Map::changeEllipsesToBezierCurves(int _dataFlag)
{

    for(Layer *l : mLayers){
        if(l->kind()==LayerKind::VECTOR_LAYER){
            VectorLayer * vl = static_cast<VectorLayer*>(l);
            vl->changeEllipsesToBezierCurves(_dataFlag);

        }else if(l->kind()==LayerKind::SPRITE_LAYER){
            SpriteLayer * sl = static_cast<SpriteLayer*>(l);
            sl->changeEllipsesToBezierCurves(_dataFlag);
        }
    }

}


//-------------------------------------------------------------------------

/*
void Map::rebuildWidgets()
{

    //---- delete widgets
    for(GuiWidget * w : mWidgets){
        if(w==GuiWidget::GetInteracted()){
            GuiWidget::_SetInteractedWidget(nullptr);
        }
        delete w;
    }
    mWidgets.clear();


    //----
    std::vector<Sprite*>sprites;
    CollectSpritesWithConstParameter(this, sprites, "widget");

    if(sprites.empty()==false){

        for(Sprite *s : sprites){

            std::string widget = s->sourceSprite()->constantParameters().value("widget");

            if(widget=="button"){
                mWidgets.push_back(new GuiButton(s));

            }else if(widget=="slider" && s->type()==SpriteType::COMPOSITE){
                mWidgets.push_back(new GuiSlider(static_cast<CompositeSprite*>(s)));

            }else if(widget=="bar"){
                mWidgets.push_back(new GuiBar(s));

            }else if(widget=="textInput" && s->type()==SpriteType::COMPOSITE){
                mWidgets.push_back(new GuiTextInput(static_cast<CompositeSprite*>(s)));

            }
        }
    }


    //----
    //for(ncine::SceneNode* n: nodeChildren()){
    //    Layer *l = static_cast<LayerNode*>(n)->baseLayer;
    for(Layer *l : mLayers){

        if(l->kind()==LayerKind::VECTOR_LAYER){
            VectorLayer * vl = static_cast<VectorLayer*>(l);
            for(VectorShape * vs : vl->shapes()){
                if(vs->parameters().value("widget")=="textField"){
                    mWidgets.push_back(new GuiTextField(vs, this));
                }
            }
        }
    }
}



void Map::setWidgetsToInitialState()
{

    for(GuiWidget * w : mWidgets){
        w->SetToInitialState();
    }
}


void Map::updateWidgets()
{

    if(mWidgetsActive==false) return;

    for(GuiWidget * w : mWidgets){
        w->Update();
    }

}


GuiWidget* Map::findWidget(const std::string &_name)
{

    for(GuiWidget * w : mWidgets){
        if(w->GetName()==_name){
            return w;
        }
    }

    return nullptr;
}


GuiWidget* Map::findWidget(int _tag)
{

    for(GuiWidget * w : mWidgets){
        if(w->GetTag()==_tag){
            return w;
        }
    }

    return nullptr;

}

*/

//-----------------------------------------------------------------------

/*
Map::Map()
{
    onConstruct();

}
*/

//Map::Map(int _zOrderStart): mZOrderStart(_zOrderStart)
//{
//    onConstruct();
//}



Map::Map(const std::string &_nameID) : LayerElement(_nameID)
{
    onConstruct();
}


//Map::Map(const std::string &_nameID, int _zOrderStart) : LayerElement(_nameID), mZOrderStart(_zOrderStart)
//{
//    onConstruct();
//}


void Map::deleteContent()
{
    //deleteWidgets();

    LayerElement::deleteContent();
}



#ifndef JUGI_EDITOR


void Map::onConstruct(){

    mKind = LayerKind::MAP;
    mNode = new LayerNode(this);

    //mTilesBoundingBox.setInitialSize();
    mWorldBoundingBox.setInitialSize();
}


Map::~Map()
{
    //deleteWidgets();
}

#endif


/*
bool ContainChildWidthNameID_dupl(Map *map, const std::string &nameID)
{

    for(SpriteLayer* l : map->spriteLayers()){
        //for(ESprite* o : l->mSprites){
        for(ncine::SceneNode*n : l->nodeChildren()){
            Sprite * o = castNodeToSprite(n);
            if(o->mName==nameID){
                return true;
            }
            if(o->mSourceSprite->sourceCompositeSprite()){
                bool contain = ContainChildWidthNameID_dupl(o->mSourceSprite->sourceCompositeSprite()->map(), nameID);
                if(contain) return true;
            }
        }
    }

    return false;
}
*/


}

