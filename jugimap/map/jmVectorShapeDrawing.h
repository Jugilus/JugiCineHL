#ifndef JUGIMAP_DRAWING_H
#define JUGIMAP_DRAWING_H

#include "memory"
#include "ncine/imgui.h"
#include "ncine/SceneNode.h"
#include "ncine/Texture.h"
#include "jmCommon.h"
#include "jmColor.h"
//#include "jmLayers.h"
#include "jmUsingDeclarations.h"



namespace jugimap {


class Camera;
class Map;
class App;
//class VectorShape;;




///\ingroup MapElements
///\brief The ShapeDrawer class is used for drawing geometric shapes.
class ShapeDrawer
{
public:
friend class App;

    ShapeDrawer();
    ~ShapeDrawer();

    void begin();

    void end();


    ImDrawList& drawList(){ return *mDrawList; }


    ///\brief  Set camera for the drawer.
    ///
    /// Camera provides transformation for drawing shapes into the screen space.
    /// The drawer object does NOT take ownership of the camera object. To clear camera assign *nullptr*.
    void setCamera(Camera* _camera){ mCamera = _camera; }


    ///\brief  Obtain current camera used by the drawer.
    Camera* camera(){ return mCamera; }



    ///\brief Set outline color to the given *_outlineColor*.
    void setOutlineColor(ColorRGBA _outlineColor){ mColor = _outlineColor; mOutlineColor = ImColor(mColor.r, mColor.g, mColor.b, mColor.a); }


    ///\brief  Obtain current drawing color.
    ColorRGBA color(){ return mColor; }


    float dotRadius(){ return mDotRadius; }


    void setDotRadius(float r) { mDotRadius = r; }


    ///\brief Add geomeric shape *_shape* to the render list.
    ///
    /// The given matrix object *_m* and scaling *_scale* provide transformation from the local space into the world space.
    /// If shape coordinates are already in the world space just use identity matrix and set scale to 1.0.

    void drawShape(VectorShape *_shape, const ncine::Matrix4x4f &_mLocalToWorld = ncine::Matrix4x4f(), Vec2f _scaleLocalToWorld = Vec2f(1.0f, 1.0f));

    void drawShape_withCamera(VectorShape *_shape, const ncine::Matrix4x4f &_mLocalToWorld = ncine::Matrix4x4f(), Vec2f _scaleLocalToWorld = Vec2f(1.0f, 1.0f));


    void drawLine(Vec2f p1, Vec2f p2);


    void drawLine_withCamera(Vec2f p1, Vec2f p2);


    void drawRectangle(BoundingBoxFloat r);


    void drawRectangle_withCamera(BoundingBoxFloat r);


    void drawDot(Vec2f pos);


    void drawDot_withCamera(Vec2f pos);


    void drawRectFilled(BoundingBoxFloat r);


    void drawEllipse(jugimap::Vec2f _center, jugimap::Vec2f _radius);


    void drawEllipse_withCamera(jugimap::Vec2f _center, jugimap::Vec2f _radius);


    void drawTexture(ncine::Texture* _texture, Vec2f dstMin, Vec2f dstMax, Vec2f uvMin, Vec2f uvMax);


    void drawText(const std::string &_text, Vec2f pos);


    //void drawShape(GeometricShape *_shape, ncine::Matrix4x4f _m, Vec2f _scale);


    //void drawLine(jugimap::Vec2f p1, jugimap::Vec2f p2, AffineMat3f _m = AffineMat3f());


    //void drawRectangle(jugimap::Rectf r, AffineMat3f _m = AffineMat3f());


    //void drawDot(Vec2f p,  AffineMat3f _m = AffineMat3f());


    //Vec2i size(){ return mSize; }
    //void setSize(Vec2i _size){ mSize = _size; }

private:
    //Vec2i mSize;
    ImDrawList* mDrawList = nullptr;
    ImColor mOutlineColor = ImColor(0, 255, 255, 255);
    ImColor mFillColor = ImColor(255, 255, 255, 255);

    ColorRGBA mColor = ColorRGBA(255,255,255);
    Camera *mCamera = nullptr;                         // LINK

    float mDotRadius = 2;


    ///\brief Draw a line from the given position *p1* to *p2*.
    //virtual void _drawLine(jugimap::Vec2f p1, jugimap::Vec2f p2);


    ///\brief Draw the outline of the given rectangle *rect*.
    //virtual void _drawRectangle(const jugimap::BoundingBoxFloat &rect);

    ///\brief Draw the outline of an ellipse defined by the given center point *c* and radius *r*.
    //virtual void _drawEllipse(jugimap::Vec2f c, jugimap::Vec2f r);


    ///\brief Draw a dot at the given position *p*.
    //virtual void _drawDot(jugimap::Vec2f p);


    //void drawWithCameraTransform(GeometricShape *s, AffineMat3f _m);



    //void drawTransformedGeometricShape(GeometricShape *vs, AffineMat3f m);

    void drawTransformedShape(VectorShape *vs, const ncine::Matrix4x4f &_m = ncine::Matrix4x4f(), Vec2f _scale = Vec2f(1.0f, 1.0f));
};


//=======================================================================================


///\brief Draw all map shapes for the given *map*. You must first set the used map camera to the drawer!
void DrawVectorShapes(ShapeDrawer& drawer, Map *map);


///\brief Draw all sprites collision shapes for the *node* which is usually Map object but it can be also SpriteLayer. You must first set the used map camera to the drawer!
void DrawSpriteCollisionShapes(ShapeDrawer &drawer, ncine::SceneNode *node);


}


#endif
