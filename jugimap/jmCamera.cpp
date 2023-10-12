#include <algorithm>
#include "jmSystem.h"
#include "jmCommonFunctions.h"
#include "jmGlobal.h"
#include "jmMap.h"
#include "jmCamera.h"



namespace jugimap {




void Camera::SetScale(float _scale)
{
    mScale = _scale;
    mChangeFlags |= Change::SCALE;
    UpdateTransformation();
}


void Camera::SetRotation(float _rotation)
{
    mRotation = _rotation;
    mChangeFlags |= Change::ROTATION;
    UpdateTransformation();
}



void Camera::UpdateViewportCamera()
{
    //if(ncCamera==nullptr) return;

    ncine::Camera::ViewValues viewValues = mViewportCamera.viewValues();
    viewValues.rotation = GetRotation();
    viewValues.scale = GetScale() * settings.appScale();
    viewValues.position.set(-GetProjectedMapPosition().x, -GetProjectedMapPosition().y);
    mViewportCamera.setView(viewValues);

    //print("Camera::UpdateViewportCamera  pos x:" + std::to_string(viewValues.position.x) + "  y:" + std::to_string(viewValues.position.y));

}




//====================================================================


bool ScreenMapCamera::allowMapScaling = true;
bool ScreenMapCamera::allowMapRotation = true;


ScreenMapCamera::ScreenMapCamera(Vec2i _screenMapDesignSize, Vec2f _mapPosition, float _mapScale, float _mapRotation, Vec2f _handle) : ScreenMapCamera()
{
    Init(_screenMapDesignSize, _mapPosition, _mapScale, _mapRotation, _handle);
}


void ScreenMapCamera::Init(Vec2i _screenMapDesignSize, Vec2f _mapPosition, float _mapScale, float _mapRotation, Vec2f _handle)
{

    mScreenMapDesignSize = _screenMapDesignSize;
    mMapPosition = _mapPosition;
    mScale = _mapScale;
    mRotation = _mapRotation;
    mMapHandle = _handle;
    mChangeFlags = Change::ALL;
    UpdateTransformation();
}


void ScreenMapCamera::SetMapHandle(Vec2f _mapHandle)
{
    mMapHandle = _mapHandle;
    mChangeFlags |= Change::HANDLE;
    UpdateTransformation();
}


void ScreenMapCamera::SetMapPosition(Vec2f _mapPosition)
{
    mMapPosition = _mapPosition;
    mChangeFlags |= Change::POSITION;
    UpdateTransformation();
}


void ScreenMapCamera::UpdateTransformation()
{

    if(allowMapScaling==false){
        mScale = 1.0;
        mChangeFlags &= ~Change::SCALE;   //clear 'changedSCALE' bit
    }
    if(allowMapRotation==false){
        mRotation = 0.0;
        mChangeFlags &= ~Change::ROTATION;
    }


    float realScale = mScale * settings.appScale();

    //---
    //matrix = MakeTransformationMatrix(mapPosition, Vec2f(scale,scale), Vec2i(), rotation, mapHandle);
    //invMatrix = matrix.Invert();

    //projectedMapPosition = matrix.Transform(Vec2f(0.0, 0.0));
    mMatrix = MakeTransformationMatrix(mMapPosition, Vec2f(realScale,realScale), mRotation, mMapHandle);
    mInvMatrix = mMatrix.inverse();

    mProjectedMapPosition = transform(Vec2f(0.0, 0.0), mMatrix);

    mViewport = TransformRectf(BoundingBoxFloat(0,0,mScreenMapDesignSize.x,mScreenMapDesignSize.y), mMatrix);
    //mapVisibleRect = Rectf(0,0, glob::screenSize.x, glob::screenSize.y);
    mMapVisibleRect = BoundingBoxFloat(0,0, settings.GetScreenSize().x, settings.GetScreenSize().y);

    //---
    UpdateViewportCamera();
}


//====================================================================


bool WorldMapCamera::allowScaling = true;
bool WorldMapCamera::allowRotation = true;



WorldMapCamera::WorldMapCamera(jugimap::BoundingBoxFloat _viewport, Vec2i _worldmapSize) : WorldMapCamera()
{
    Init(_viewport, _worldmapSize);
}


void WorldMapCamera::Init(jugimap::BoundingBoxFloat _viewport, Vec2i _worldmapSize)
{
    mViewport = _viewport;
    mWorldMapSize = _worldmapSize;
    mChangeFlags = Change::ALL;
    UpdateTransformation();
}


/*
void WorldMapCamera::InitPointedPosition(Vec2f _pointedPosition)
{
    pointedPosition = previousPointedPosition = _pointedPosition;
    changeFlags |= Change::POSITION;
    UpdateTransformation();
}
*/


void WorldMapCamera::SetPointedPosition(Vec2f _pointedPosition, bool _captureForLerpDrawing)
{
    if(_captureForLerpDrawing){
        mPointedPosition = mPreviousPointedPosition = _pointedPosition;
    }
    mPreviousPointedPosition = mPointedPosition;
    mPointedPosition = _pointedPosition;
    mChangeFlags |= Change::POSITION;
    UpdateTransformation();
}


void WorldMapCamera::SetConstrainedInScrollableRect(bool _constrainedInScrollableRect)
{
    mConstrainedInScrollableRect = _constrainedInScrollableRect;
    UpdateTransformation();
}


Vec2f WorldMapCamera::GetPointedPosition()
{

    //if(settings.LerpDrawingEnabled()){
    //    return previousPointedPosition + (pointedPosition - previousPointedPosition) * settings.GetLerpDrawingFactor();
    //}
    return mPointedPosition;
}



void WorldMapCamera::UpdateTransformation()
{


    if(allowScaling==false){
        mScale = 1.0;
        mChangeFlags &= ~Change::SCALE;
    }
    if(allowRotation==false){
        mRotation = 0.0;
        mChangeFlags &= ~Change::ROTATION;
    }


    float realScale = mScale * settings.appScale();


    if(mConstrainedInScrollableRect){
        Vec2f mapNoScrollingBorder((mViewport.GetWidth()/2.0)/realScale, (mViewport.GetHeight()/2.0)/realScale);
        if(mapNoScrollingBorder.x>mWorldMapSize.x/2.0) mapNoScrollingBorder.x = mWorldMapSize.x/2.0;
        if(mapNoScrollingBorder.y>mWorldMapSize.y/2.0) mapNoScrollingBorder.y = mWorldMapSize.y/2.0;

        mMapScrollableRect.min = Vec2f(mapNoScrollingBorder.x,mapNoScrollingBorder.y);
        mMapScrollableRect.max = Vec2f(mWorldMapSize.x - mapNoScrollingBorder.x, mWorldMapSize.y - mapNoScrollingBorder.y);

        mPointedPosition.x = ClampValue(mPointedPosition.x, mMapScrollableRect.min.x, mMapScrollableRect.max.x);
        mPointedPosition.y = ClampValue(mPointedPosition.y, mMapScrollableRect.min.y, mMapScrollableRect.max.y);

    }else{
        mMapScrollableRect = BoundingBoxFloat(0.0,0.0, mWorldMapSize.x, mWorldMapSize.y);
    }


    //AffineMat3f m = MakeTransformationMatrix(-viewport.GetCenter(), Vec2f(scale,scale), Vec2i(), rotation);
    //Vec2f p = m.Transform(pointedPosition.x, pointedPosition.y);
    //projectedMapPosition = -p;
    //matrixA = MakeTransformationMatrix(projectedMapPosition, Vec2f(scale,scale), Vec2i(), rotation);
    //invMatrixA = matrixA.Invert();


    ncine::Matrix4x4f m = MakeTransformationMatrix(-mViewport.GetCenter(), Vec2f(realScale,realScale), -mRotation);       // - rotation
    Vec2f p = transform(mPointedPosition, m);
    mProjectedMapPosition = -p;
    mMatrix = MakeTransformationMatrix(mProjectedMapPosition, Vec2f(realScale,realScale), -mRotation);
    mInvMatrix = mMatrix.inverse();


    //----
    Vec2f p1 = MapPointFromScreenPoint(Vec2f(mViewport.min.x, mViewport.min.y));
    Vec2f p2 = MapPointFromScreenPoint(Vec2f(mViewport.max.x, mViewport.min.y));
    Vec2f p3 = MapPointFromScreenPoint(Vec2f(mViewport.max.x, mViewport.max.y));
    Vec2f p4 = MapPointFromScreenPoint(Vec2f(mViewport.min.x, mViewport.max.y));
    mMapVisibleRect.min.x = ClampValue<float>(std::min({p1.x, p2.x, p3.x, p4.x}), 0, mWorldMapSize.x);
    mMapVisibleRect.min.y = ClampValue<float>(std::min({p1.y, p2.y, p3.y, p4.y}), 0, mWorldMapSize.y);
    mMapVisibleRect.max.x = ClampValue<float>(std::max({p1.x, p2.x, p3.x, p4.x}), 0, mWorldMapSize.x);
    mMapVisibleRect.max.y = ClampValue<float>(std::max({p1.y, p2.y, p3.y, p4.y}), 0, mWorldMapSize.y);

    //---
    UpdateViewportCamera();

}




}
