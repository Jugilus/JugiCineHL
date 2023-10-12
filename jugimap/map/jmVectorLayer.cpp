#include "jmMap.h"
#include "jmVectorShape.h"
#include "jmVectorShapeUtilities.h"
#include "jmVectorLayer.h"



namespace jugimap {



VectorLayersGroup::VectorLayersGroup() : LayerElement()
{
    OnVectorLayersGroupConstruct();
}


VectorLayersGroup::VectorLayersGroup(const std::string &_nameID) : LayerElement(_nameID)
{
    OnVectorLayersGroupConstruct();
}


#ifndef JUGI_EDITOR

void VectorLayersGroup::OnVectorLayersGroupConstruct()
{
    mKind = LayerKind::VECTOR_LAYERS_GROUP;
    mNode = new LayerNode(this);
}


void VectorLayersGroup::rebuildQuickAccessContainers()
{
    assert(parent() && parent()->kind()==LayerKind::MAP);
    Map *m = static_cast<Map*>(parent());
    m->rebuildQuickAccessContainers();
}


#endif


//==========================================================================



void VectorLayer::updatePropertiesViaExtraParameters()
{

    Layer::updatePropertiesViaExtraParameters();


}


void VectorLayer::updateCameraDependentObjects()
{

    if(map()){
        if(map()->type()==MapType::PARALLAX){

            //if(GetParallaxLayerMode()==ParallaxLayerMode::NO_CHANGE ||
            //         GetParallaxLayerMode()==ParallaxLayerMode::TILING_X ||
            //         GetParallaxLayerMode()==ParallaxLayerMode::TILING_Y ||
            //         GetParallaxLayerMode()==ParallaxLayerMode::TILING_XY )
            //{

            //if(GetParallaxLayerMode()==ParallaxLayerMode::STANDARD){
            if(layerType()==LayerType::PARALLAX){
                UpdateParallaxOffset();
            }
        }
    }
}


int tileIndex(double value){ return int(std::round(std::floor(value))); }


void VectorLayer::updateBoundingBox(int flags)
{

    mWorldBoundingBox.setInitialSize();
    for(VectorShape* vs : mShapes){
        //for(PathPoint &p : vs->pathPoints()){
        //    mWorldBoundingBox.Expand(p);
        //}
        mWorldBoundingBox.Unite(vs->mBoundingBox);
    }

#ifdef JUGI_EDITOR

    if(map()==nullptr) return;  // when using collision engine with ist own vector layer
    mTilesBoundingBox.min.x = tileIndex(mWorldBoundingBox.min.x/map()->tileWidth());
    mTilesBoundingBox.min.y = tileIndex(mWorldBoundingBox.min.y/map()->tileHeight());
    mTilesBoundingBox.max.x = tileIndex(mWorldBoundingBox.max.x/map()->tileWidth());
    mTilesBoundingBox.max.y = tileIndex(mWorldBoundingBox.max.y/map()->tileHeight());
    //mTilesBoundingBox = TilesBoundingBoxFromBoundingBox(mBoundingBox, CurrentProject.tileWidth, CurrentProject.tileHeight);
#endif

}




VectorLayer::VectorLayer(Map *_map) : Layer(_map)
{
    OnVectorLayerConstruct();
}


VectorLayer::VectorLayer(const std::string &_name, Map *_map) : Layer(_name, _map)
{
    OnVectorLayerConstruct();
}


VectorLayer::~VectorLayer()
{
    deleteVectorShapes();
}


void VectorLayer::changeEllipsesToBezierCurves(int _dataFlag)
{

    std::vector<VectorShape*>ellipses;

    for(int i = int(mShapes.size())-1; i>=0; i-- ){
        VectorShape *vs = mShapes.at(i);
        if(vs->kind()==ShapeKind::ELLIPSE && vs->dataFlags()==_dataFlag){
            ellipses.push_back(vs);
            mShapes.erase(mShapes.begin()+i);
        }
    }

    for(VectorShape *vs : ellipses){
        VectorShape *bezierShape = ConvertEllipseToBezierCurve(vs);
        mShapes.push_back(bezierShape);
    }

}


void VectorLayer::deleteContent()
{

    deleteVectorShapes();

    Layer::deleteContent();

}


void VectorLayer::deleteVectorShapes()
{

    hidden = false;
    //mBoundingBox.setEmptySize();
    mWorldBoundingBox.setInitialSize();
    for(VectorShape *s : mShapes){
        delete s;
    }
    mShapes.clear();

#ifdef JUGI_EDITOR
    //mTilesBoundingBox.setEmptySize();
    mTilesBoundingBox.setInitialSize();
#endif
}





#ifndef JUGI_EDITOR


void VectorLayer::OnVectorLayerConstruct()
{
    mKind = LayerKind::VECTOR_LAYER;
    mNode = new LayerNode(this);
}


//EVectorLayer::EVectorLayer()
//{
//    mKind = LayerKind::VECTOR_LAYER;
//    mNode = new LayerNode(this);
//}

//EVectorLayer::EVectorLayer(const std::string &_name) : Layer(_name)
//{
//    mKind = LayerKind::VECTOR_LAYER;
//    mNode = new LayerNode(this);
//}

/*
VectorLayer::~VectorLayer()
{
    for(VectorShape* vs : mShapes) delete vs;
    mShapes.clear();
}
*/


void VectorLayer::AddLoadedShape(VectorShape *_shape)
{
    mShapes.push_back(_shape);
}

#endif




}
