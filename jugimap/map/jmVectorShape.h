#ifndef JUGIMAP_VECTOR_SHAPES_H
#define JUGIMAP_VECTOR_SHAPES_H

#include <vector>

#include "jmCommon.h"
#include "jmCommonFunctions.h"
#include "jmParameter.h"
#include "jmGlobal.h"




namespace jugi{
namespace base{
class TGraphics;
class TColor;
}
//namespace openGL{
//class Image;
//}
class TShapeEngine;
}

namespace jugimap{

class Sprite;
class MapLoader_bin;
class VectorLayer;



struct TPointMarkers{

    enum Kind
    {
        markerA,
        markerB,
        markerC
    };

    Kind GetImageA() { return Kind::markerA; }
    Kind GetImageB() { return Kind::markerB; }
    Kind GetImageC() { return Kind::markerC; }

};
extern TPointMarkers PointMarkers;




class VectorShape
{
public:
    friend class MapLoader_bin;

    VectorShape(ShapeKind _kind):mKind(_kind){}


     ///\brief Destructor.
     virtual ~VectorShape(){}


    jugimap::BoundingBoxInt boundingBox(){ return mBoundingBox; }


     jugimap::BoundingBoxFloat boundingBoxF()
     {
         return jugimap::BoundingBoxFloat(mBoundingBox.min.x, mBoundingBox.min.y, mBoundingBox.max.x, mBoundingBox.max.y);
     }


     ///\brief Returns the kind of this shape.
     ShapeKind kind() {return mKind;}


     void _setParentLayer(VectorLayer *_parentLayer){mParentLayer = _parentLayer;}
     VectorLayer* parentLayer(){ return mParentLayer; }


     const std::string & name(){ return mName; }

     int id(){ return mId; }

     bool isExtra(){ return mExtra; }

     /// Returns the *data flags* of this vector shape.
     int dataFlags() { return mDataFlags; }

     /// Returns a reference to the vector of data parameters in this vector shape.
     CParameters& parameters() { return mParameters; }


     /// Returns the length of this vector shape.
     float pathLength(){return mPathLength; }


     //------
     /// Returns a point along the path at the given parametric position *r*.
     Vec2f obtainPathPoint(float r);


     /// Returns a point along the path at the given parametric position *r*.
     Vec2f obtainPathPoint(float r, float &directionAngle);


     //------
     std::vector<PathPoint>& pathPoints(){ return mPoints; }


     std::vector<ShapePoint>& shapePoints(){ return mCPoints; }


     ///\brief Returns *true* if this vector shape has a geometric object which is closed; otherwise returns false.
     bool isClosed(){ return mClosed;}


     void _setClosed(bool _closed){ mClosed = _closed;}


     bool isRectangle();


     //--- single point shape
     void setSinglePoint(Vec2i p);
     void setSinglePoint(int _x, int _y);
     Vec2i singlePoint();
     Vec2f singlePointF();

     //--- ellipse
     float a(){ return ma; }
     float b(){ return mb; }
     Vec2f center(){ return mCenter; }


     //---- HACK for quickly creating shapes for collision
     void _setCenter(Vec2f _pos){ mCenter = _pos;}
     void _setA(float _a){ ma = _a;}
     void _setB(float _b){ mb = _b;}


     //-------
     virtual void updatePathPoints();
     virtual void updateBoundingBox();
     void setPathPointsAngles();

//protected:

     ShapeKind mKind = ShapeKind::NOT_DEFINED;
     VectorLayer * mParentLayer = nullptr;              // LINK
     std::vector<ShapePoint>mCPoints;
     std::vector<PathPoint>mPoints;
     bool mClosed = false;

     //---- custom data
     std::string mName;
     int mId = 0;
     unsigned char mDataFlags = 0;                      //
     CParameters mParameters;
     bool mExtra = false;                                 //
     float mPathLength = 0.0;
     jugimap::BoundingBoxInt mBoundingBox;


#ifdef JUGI_EDITOR

     friend class jugi::TShapeEngine;


     static VectorShape* Copy(VectorShape *_Shape);
     static VectorShape* Create(jugimap::ShapeKind _kind);

     //virtual ~EShape(){}

     void ResetPointPointers();
     virtual void SetEditPoint(ShapePoint* _CPoint);
     virtual void SetEditBPoint(Vec2f* _BPoint);
     virtual void StartEditing(int _state);

     virtual int Logic_CreateShape(){ return 0; }
     virtual int Logic_MoveShape();
     virtual int Logic_MovePoint(){ return 0; }

     virtual void SetCPointPosition(ShapePoint* _CPoint, int _x, int _y){}
     int GetIndexOfControlPoint(ShapePoint * _CPoint);
     virtual void MoveContent(int dx, int dy);
     // ------------------------------------------------
     virtual void Draw(jugi::base::TGraphics &Graphics);
     virtual void Draw(jugi::base::TGraphics &Graphics, const jugi::base::TColor &_Color, float _alpha=1.0f);
     virtual void Draw(jugi::base::TGraphics &Graphics, const ncine::Matrix4x4f &mTransform);
     virtual void DrawColorFill(jugi::base::TGraphics &Graphics);
     virtual void DrawColorFill(jugi::base::TGraphics &Graphics, const jugi::base::TColor &_Color, float _alpha);
     //virtual void DrawRectangleSpecial(base::TGraphics &Graphics){}
     virtual void DrawRectangleCrossLines(jugi::base::TGraphics &Graphics){}
     virtual void DrawControlPoints(jugi::base::TGraphics &Graphics);
     virtual void DrawControlPoint(jugi::base::TGraphics &Graphics, ShapePoint *_CP, const jugi::base::TColor &_Color);
     //void DrawMarkerImage(jugi::base::TGraphics& Graphics, jugi::base::TImageGL& _Image,  double _x, double _y);
     //void DrawMarkerImage(jugi::base::TGraphics& Graphics, jugi::openGL::Image* _Image,  double _x, double _y);
     void DrawMarkerImage(jugi::base::TGraphics& Graphics, TPointMarkers::Kind _marker,  double _x, double _y);
     virtual void DrawBoundingBox(jugi::base::TGraphics &Graphics, int _endOffset=0);
     virtual void DrawBoundingBox(jugi::base::TGraphics &Graphics, const jugi::base::TColor &_Color, float _alpha, int _endOffset=0);

     BoundingBoxInt DrawParameters(jugi::base::TGraphics &Graphics);

     //--- LOGIC
     static unsigned long long shapeMapIdCounter;
     unsigned long long shapeMapId = 0;          // vsak novi shape v layerju dobi svoj id (za undo/redo identifikacijo)

     int state = stateIDLE;
     static const int stateIDLE = 0;
     static const int stateCREATE_SHAPE = 1;
     static const int stateMOVE_POINT = 2;
     static const int stateMOVE = 3;

     Vec2i PMouseNormal;
     Vec2i PMouseNormalOld;
     Vec2i PMouseNormalSnapped;
     Vec2i PMouseNormalSnappedOld;
     Vec2i PMouseNormalStart;
     ShapePoint CPMouseOverStartPos;
     ShapePoint* CPMouseOver = nullptr;         // flag link
     Vec2f* BPMouseOver = nullptr;                       // flag link
     ShapePoint* CPedited = nullptr;            // flag link
     Vec2f* BPedited = nullptr;                          // flag link
     Vec2f PVirtual;
     ShapePoint* CPafterPvirtual  = nullptr;           // flag link

     //TShapeCustomPoint* MouseOverCustomPoint = nullptr;
     //TShapeCustomPoint* editedCustomPoint = nullptr;
     int indexPointAfterPPMouseOver = -1;

     static const int EDITING_FINISHED = 1;
     static const int CREATE_SHAPE_TERMINATED = 2;
     static const int EDITED_SHAPE_CHANGED = 3;
     bool shapeChanged = false;

     Sprite *LinkObject = nullptr;                // pomožni link pri editiranju collider shapes
     VectorShape * LinkShape = nullptr;              // pomožni link pri editiranju collider shapes

#endif






protected:

     VectorShape(){}

     static const int CENTER=0;
     static const int TOP=1;
     static const int RIGHT=2;
     static const int BOTTOM=3;
     static const int LEFT=4;

     Vec2f mCenter;
     float ma = 0.0f;
     float mb = 0.0f;


     void UpdateRectanglePointsFromControlPoints();
     void UpdatePolylinePointsFromControlPoints();
     void UpdateBezierPolylinePointsFromControlPoints();
     void UpdateEllipsePointsFromControlPoints();
     void UpdateSinglePointFromControlPoints();


 };


//================================================================================

/*

class FrmRectangleShape : public VectorShape
{
public:

    FrmRectangleShape();
    FrmRectangleShape(int _x, int _y, int _width, int _height);


    void updatePoints() override;

private:
    void FrmRectangleShapeInit();

};

//================================================================================


class FrmPolyLineShape : public VectorShape
{
public:

    FrmPolyLineShape();


    void updatePoints() override;

private:
    void FrmPolyLineShapeInit();

};


//================================================================================


class FrmBezierShape : public VectorShape
{
public:
    FrmBezierShape();



    void updatePoints() override;

private:

    void FrmBezierShapeInit();

};

//void GetShapePointsFromBezierSegment(std::vector<TShapePoint> &Points, Vec2f BP1, Vec2f BP2, Vec2f BP3, Vec2f BP4, int i);


//================================================================================


class FrmEllipseShape : public VectorShape
{
public:

    FrmEllipseShape();

    float a(){ return std::abs(mCPoints[RIGHT].x - mCPoints[CENTER].x); }
    float b(){ return std::abs(mCPoints[TOP].y - mCPoints[CENTER].y); }
    Vec2f center(){ return Vec2f(mCPoints[CENTER].x, mCPoints[CENTER].y); }

    //---- HACK for quickly creating shapes for collision
    void _setCenter(Vec2f _pos){ mCenter = _pos;}
    void _setA(float _a){ ma = _a;}
    void _setB(float _b){ mb = _b;}


    static const int CENTER=0;
    static const int TOP=1;
    static const int RIGHT=2;
    static const int BOTTOM=3;
    static const int LEFT=4;

    void updatePoints() override;

private:
    void FrmEllipseShapeInit();


    Vec2f mCenter;
    float ma = 0.0f;
    float mb = 0.0f;

};


//================================================================================


class FrmSinglePointShape : public VectorShape
{
public:

    FrmSinglePointShape();
    FrmSinglePointShape(int _x, int _y);
    void set(int _x, int _y);
    Vec2i get();
    Vec2f getF();


protected:
    void updatePoints();

private:
    void FrmSinglePointShapeInit();

};

*/






}
#endif // VECTORSHAPE_H
