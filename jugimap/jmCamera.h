#ifndef JUGIMAP_CAMERA_H
#define JUGIMAP_CAMERA_H


#include <cmath>
#include <vector>
#include <string>
#include <ncine/Camera.h>
#include <ncine/Viewport.h>
#include "jmCommon.h"
#include "jmGlobal.h"




namespace jugimap {



class ScreenMapCamera;
class WorldMapCamera;


/// \ingroup Cameras
/// \brief The base camera class.
class Camera
{
public:



    /// The camera change flags.
    enum Change{
        NONE = 0,
        POSITION = 1,
        SCALE = 2,
        ROTATION = 4,
        HANDLE = 8,
        ALL = POSITION | SCALE | ROTATION | HANDLE
    };


    virtual ~Camera(){}


    /// Returns the kind of camera.
    CameraKind GetKind(){ return mKind; }

    /// Returns the viewport - a screen rectangle where the map is projected.
    BoundingBoxFloat GetViewport(){ return mViewport; }

    /// Returns the position of the projected map's origin point.
    Vec2f GetProjectedMapPosition() { return mProjectedMapPosition;}

    /// Returns the scale of projection.
    float GetScale(){ return mScale; }

    /// Returns the rotation of projection.
    float GetRotation(){ return mRotation; }

    /// Set the scale of projection.
    void SetScale(float _scale);

    /// Set the rotation of projection.
    void SetRotation(float _rotation);

    /// Returns the map point from the given **_screenPoint**.
    Vec2f MapPointFromScreenPoint(Vec2f _screenPoint){ return transform(_screenPoint, mInvMatrix);}

    /// Returns the screen point from the given **_mapPoint**.
    Vec2f ScreenPointFromMapPoint(Vec2f _mapPoint){ return transform(_mapPoint, mMatrix);}

    /// Returns the visible area of the map screen.
    BoundingBoxFloat GetMapVisibleRect(){return mMapVisibleRect;}


    const ncine::Matrix4x4f & GetMatrix(){ return mMatrix; }


    /// Returns the change value.
    int GetChangeFlags(){ return mChangeFlags; }

    /// Clears the change value.
    void ClearChangeFlags(){ mChangeFlags = 0; }


    ncine::Camera* viewportCamera(){ return &mViewportCamera;}
    //ncine::Viewport *ncViewport = nullptr;


protected:
    CameraKind mKind = CameraKind::NOT_DEFINED;
    float mScale = 1.0;
    float mRotation = 0.0;
    Vec2f mProjectedMapPosition;        // bottom-left corner
    BoundingBoxFloat mViewport;
    //AffineMat3f mMatrixA;
    //jugimap::AffineMat3f mInvMatrixA;
    ncine::Matrix4x4f mMatrix;
    ncine::Matrix4x4f mInvMatrix;
    BoundingBoxFloat mMapVisibleRect;
    int mChangeFlags = 0;

    ncine::Camera mViewportCamera;


    virtual void UpdateTransformation() = 0;

    void UpdateViewportCamera();


};



/// \ingroup Cameras
/// \brief  A camera for screen maps.
///
/// The ScreenMapCamera object defines transformation from the map coordinates to the screen coordinates.
class ScreenMapCamera : public Camera
{
public:
    static bool allowMapScaling;
    static bool allowMapRotation;


    ScreenMapCamera(){ mKind = CameraKind::SCREEN; }


    ScreenMapCamera(Vec2i _screenMapDesignSize, Vec2f _mapPosition, float _mapScale=1.0, float _mapRotation=0.0, Vec2f _handle=Vec2f());

    /// Initialize a screen map camera from the given parameters.
    void Init(Vec2i _screenMapDesignSize, Vec2f _mapPosition, float _mapScale=1.0, float _mapRotation=0.0, Vec2f _handle=Vec2f());

    /// Set the position of the map on the screen.
    void SetMapPosition(Vec2f _mapPosition);

    /// Set the handle point of the map.
    void SetMapHandle(Vec2f _mapHandle);

    /// Returns the size of the map;
    Vec2i GetScreenMapDesignSize(){ return mScreenMapDesignSize;}

    /// Returns the position of the map on the screen.
    Vec2f GetMapPosition(){ return mMapPosition; }

    /// Returns the handle point of the map.
    Vec2f GetMapHandle(){ return mMapHandle; }


protected:
    Vec2i mScreenMapDesignSize;
    Vec2f mMapPosition;
    Vec2f mMapHandle;

    virtual void UpdateTransformation() override;

};


/// \ingroup Cameras
/// \brief A camera for world maps and parallax maps.
/// The WorldMapCamera object defines transformation from the map coordinates to the screen viewport coordinates.
class WorldMapCamera : public Camera
{

public:
    static bool allowScaling;
    static bool allowRotation;


    WorldMapCamera(){ mKind = CameraKind::WORLD; }

    WorldMapCamera(BoundingBoxFloat _viewport, Vec2i _worldmapSize);

    /// Initialize a world camera from the given **_viewport** and **_worldMapSize**.
    void Init(BoundingBoxFloat _viewport, Vec2i _worldmapSize);

    /// Returns the pointed map point. That is the point which is shown in the center of the viewport.
    Vec2f GetPointedPosition();

    /// Set the pointed map point. That is the point which is shown in the center of the viewport.
    void SetPointedPosition(Vec2f _pointedPosition, bool _captureForLerpDrawing=false);

    ///\brief Set the *constrainedInScrollableRect* flag. This constrain the pointed map position to the scrollable area of the map in order
    /// to prevent outsides of the map coming into viewport. If camera scales or rotates the outside of the map may still come into viewport.
    void SetConstrainedInScrollableRect(bool _constrainedInScrollableRect);

    /// Returns true if *constrainedInScrollableRect* is set to true; otherwise returns false.
    bool IsConstrainedInScrollableRect(){ return mConstrainedInScrollableRect;}

    /// Returns the map size.
    jugimap::Vec2i GetWorldSize(){ return mWorldMapSize;}

    /// Returns the scrollable rectangle area of the map.
    BoundingBoxFloat GetMapScrollableRect(){ return mMapScrollableRect; }


protected:
    Vec2i mWorldMapSize;
    BoundingBoxFloat mMapScrollableRect;
    //---
    Vec2f mPointedPosition;
    Vec2f mPreviousPointedPosition;
    //---
    bool mConstrainedInScrollableRect = true;

    virtual void UpdateTransformation() override;

};



}



#endif


