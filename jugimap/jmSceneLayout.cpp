#include <string>
#include <sstream>
#include "jmSystem.h"
#include "jmMapLoader_bin.h"
#include "jmSourceSprite.h"
#include "jmMap.h"
#include "jmSprite.h"
#include "jmFont.h"
#include "jmInput.h"
#include "jmCamera.h"
#include "jmGuiCommon.h"
#include "jmVectorShapeDrawing.h"
#include "jmSceneLayout.h"



namespace jugimap {




bool SceneMapInfo::operator==(SceneMapInfo &other)
{

    // viewport
    if(xMinRel != other.xMinRel) return false;
    if(xMinAbs != other.xMinAbs) return false;

    if(yMinRel != other.yMinRel) return false;
    if(yMinAbs != other.yMinAbs) return false;

    if(xMaxRel != other.xMaxRel) return false;
    if(xMaxAbs != other.xMaxAbs) return false;

    if(yMaxRel != other.yMaxRel) return false;
    if(yMaxAbs != other.yMaxAbs) return false;

    if(mapType != other.mapType) return false;
    //if(cameraKind != other.cameraKind) return false;

    if(name != other.name) return false;
    if(mapRelativeFilePath != other.mapRelativeFilePath) return false;
    if(worldSceneMapForParallaxMap != other.worldSceneMapForParallaxMap) return false;

    if(parameters.size() != other.parameters.size()) return false;

    for(int i=0; i<parameters.size(); i++){
        if(parameters.at(i).name != other.parameters.at(i).name) return false;
        if(parameters.at(i).value != other.parameters.at(i).value) return false;
    }


    return true;

}



BoundingBoxInt SceneMapInfo::viewportRect(Vec2i appDesignResolution)
{

    BoundingBoxInt bb;

    bb.min.x = (xMinRel * appDesignResolution.x / 100.0f) + xMinAbs*settings.appScale();
    bb.min.y = (yMinRel * appDesignResolution.y / 100.0f) + yMinAbs*settings.appScale();
    bb.max.x = (xMaxRel * appDesignResolution.x / 100.0f) + xMaxAbs*settings.appScale();
    bb.max.y = (yMaxRel * appDesignResolution.y / 100.0f) + yMaxAbs*settings.appScale();

    return bb;

}


BoundingBoxInt SceneMapInfo::viewportRect(BoundingBoxInt appBoundingBox)
{

    BoundingBoxInt bb = viewportRect(appBoundingBox.GetSize());
    bb.Move(appBoundingBox.min.x, appBoundingBox.min.y);

    return bb;
}


//========================================================================



SceneMap::~SceneMap()
{

    /*
    if(mNcineViewport) delete mNcineViewport;

    if(mCamera) delete mCamera;

    if(mMap && mSharedMap==false){
        delete mMap;
    }
    */

    deleteContent();

}



void SceneMap::deleteContent()
{


    if(mNcineViewport){
        delete mNcineViewport;
        mNcineViewport = nullptr;
    }

    if(mCamera){
        delete mCamera;
        mCamera = nullptr;
    }

    if(mMap && mSharedMap==false){
        delete mMap;
        mMap = nullptr;
    }

}


bool SceneMap::isVisible()
{
    return (mNcineViewport->rootNode()!=nullptr);
}



void SceneMap::setVisible(bool _visible)
{
    if(_visible){
        mNcineViewport->setRootNode(mMap->node());
        //mMap->setCamera(mCamera);

    }else{
        mNcineViewport->setRootNode(nullptr);
        //mMap->setCamera(nullptr);
    }
}




}
