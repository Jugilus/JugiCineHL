#include <assert.h>
#include "jmCommonFunctions.h"
#include "jmVectorShape.h"
#include "jmVectorShapeDrawing.h"
#include "jmVectorShapeUtilities.h"



namespace jugimap{


bool IsPolygonConvex(std::vector<Vec2f> &vertices, bool &cw)
{

    bool negative = false;
    bool positive = false;
    int nVertices = vertices.size();

    for (int i = 0; i < nVertices; i++){

        int iNext = (i + 1) % nVertices;
        int iNextNext = (iNext + 1) % nVertices;

        Vec2f v1 = vertices[iNext] - vertices[i];
        Vec2f v2 = vertices[iNextNext] - vertices[iNext];

        float cross_product = v1.x*v2.y - v1.y*v2.x;

        if(cross_product < 0){
            negative = true;
            cw = true;

        }else if (cross_product > 0){
            positive = true;
            cw = false;
        }
        if (negative && positive) return false;
    }

    return true;
}



bool SameGeometricShapes(std::vector<VectorShape *> &shapes1, std::vector<VectorShape *> &shapes2)
{

    if(shapes1.size()!=shapes2.size()){
        return false;

    }else if(shapes1.size()==1 && shapes2.size()==1){
        return GeometricShapesEqual(shapes1[0], shapes2[0]);

    }else{

        for(VectorShape * vs1 : shapes1){
            bool foundSameShape = false;
            for(VectorShape * vs2 : shapes2){
                if(GeometricShapesEqual(vs1, vs2)){
                    foundSameShape = true;
                    break;
                };
            }
            if(foundSameShape==false){
                return false;
            }
        }
    }

    return true;
}



bool GeometricShapesEqual(VectorShape *gs1, VectorShape *gs2)
{

    if(gs1->kind() != gs2->kind()) return false;
    if(gs1->isClosed() != gs2->isClosed()){
        return false;
    }

    std::vector<ShapePoint>&cPoints1 = gs1->shapePoints();
    std::vector<ShapePoint>&cPoints2 = gs2->shapePoints();


    if(cPoints1.size() != cPoints2.size()){
        return false;
    }
    for(int i=0; i<cPoints1.size(); i++){
        if(IsEqual(cPoints1[i], cPoints2[i])==false){
            return false;
        }
        if(IsEqual(cPoints1[i].BPnext, cPoints2[i].BPnext)==false){
            return false;
        }
        if(IsEqual(cPoints1[i].BPprev, cPoints2[i].BPprev)==false){
            return false;
        }
    }

    /*
    if(gs1->GetKind()==ShapeKind::POLYLINE){
        PolyLineShape *poly1 = static_cast<PolyLineShape *>(gs1);
        PolyLineShape *poly2 = static_cast<PolyLineShape *>(gs2);

        if(poly1->vertices.size() != poly2->vertices.size()){
            return false;
        }
        if(poly1->IsClosed() != poly2->IsClosed()){
            return false;
        }

        for(int i=0; i<poly1->vertices.size(); i++){
            //if(poly1->vertices[i].Equals(poly2->vertices[i])==false){
            if(IsEqual(poly1->vertices[i], poly2->vertices[i])==false){
                return false;
            }
        }


    }else if(gs1->GetKind()==ShapeKind::BEZIER_POLYCURVE){
        BezierShape *bezpoly1 = static_cast<BezierShape *>(gs1);
        BezierShape *bezpoly2 = static_cast<BezierShape *>(gs2);

        if(bezpoly1->vertices.size() != bezpoly2->vertices.size()){
            return false;
        }
        if(bezpoly1->IsClosed() != bezpoly2->IsClosed()){
            return false;
        }

        for(int i=0; i<bezpoly1->vertices.size(); i++){
            //if(bezpoly1->vertices[i].P.Equals(bezpoly2->vertices[i].P)==false){
            if(IsEqual(bezpoly1->vertices[i].P, bezpoly2->vertices[i].P)==false){
                return false;
            }
            //if(bezpoly1->vertices[i].CPprevious.Equals(bezpoly2->vertices[i].CPprevious)==false){
            if(IsEqual(bezpoly1->vertices[i].CPprevious, bezpoly2->vertices[i].CPprevious)==false){
                return false;
            }
            //if(bezpoly1->vertices[i].CPnext.Equals(bezpoly2->vertices[i].CPnext)==false){
            if(IsEqual(bezpoly1->vertices[i].CPnext, bezpoly2->vertices[i].CPnext)==false){
                return false;
            }
        }

    }else if(gs1->GetKind()==ShapeKind::ELLIPSE){
        EllipseShape *ellipse1 = static_cast<EllipseShape *>(gs1);
        EllipseShape *ellipse2 = static_cast<EllipseShape *>(gs2);

        //if(ellipse1->center.Equals(ellipse2->center)==false){
        if(IsEqual(ellipse1->center, ellipse2->center)==false){
            return false;
        }

        if(AreEqual(ellipse1->a, ellipse2->a)==false || AreEqual(ellipse1->b, ellipse2->b)==false){
            return false;
        }


    }else if(gs1->GetKind()==ShapeKind::SINGLE_POINT){
        SinglePointShape *sp1 = static_cast<SinglePointShape *>(gs1);
        SinglePointShape *sp2 = static_cast<SinglePointShape *>(gs2);

        //if(sp1->point.Equals(sp2->point)==false){
        if(IsEqual(sp1->point, sp2->point)==false){
            return false;
        }
    }
    */

    return true;
}

/*

void _AddVertexToPolyline(std::vector<Vec2f>&Vertices, Vec2f v,  float pointsDistanceMin)
{
    if(Vertices.empty()){
        Vertices.push_back(v);
    }else{
        float dist = v.Distance(Vertices.back());
        if(dist>=pointsDistanceMin){
            Vertices.push_back(v);
        }
    }
}


void BezierPolycurveToPolyline(std::vector<BezierVertex>&bezierVertices,  std::vector<Vec2f>&vertices)
{

    float pointsDistanceMin = 2.0;
    vertices.clear();
    if(bezierVertices.empty()) return;


    for(int i=0; i<bezierVertices.size()-1; i++){

        Vec2f BP1 = bezierVertices[i].P;
        Vec2f BP2 = bezierVertices[i].CPnext;
        Vec2f BP3 = bezierVertices[i+1].CPprevious;
        Vec2f BP4 = bezierVertices[i+1].P;

        float length14 = BP1.Distance(BP4);
        if(length14 <= 2){
            _AddVertexToPolyline(vertices, BP1, pointsDistanceMin);
            continue;
        }

        // straight line between BP1 and BP4
        //if( AreSameVec2f(BP1,BP2) && AreSameVec2f(BP3,BP4)){
        if( BP1.Equals(BP2) && BP3.Equals(BP4)){
            _AddVertexToPolyline(vertices, BP1, pointsDistanceMin);
            _AddVertexToPolyline(vertices, BP4, pointsDistanceMin);
            continue;
        }

        float t = 0.0;
        float length = BP1.Distance(BP2) + BP2.Distance(BP3) + BP3.Distance(BP4);
        float f = length14/length;
        float dStep = 0.05 * f;            // this looks ok

        _AddVertexToPolyline(vertices, BP1, pointsDistanceMin);

        while(t <= 1.00-dStep){
            float t2 = 1.0-t;
            float xB =  BP1.x * t2*t2*t2 + 3*BP2.x * t*t2*t2 + 3*BP3.x * t2*t*t + BP4.x * t*t*t;
            float yB =  BP1.y * t2*t2*t2 + 3*BP2.y * t*t2*t2 + 3*BP3.y * t2*t*t + BP4.y * t*t*t;

            _AddVertexToPolyline(vertices, Vec2f(xB,yB), pointsDistanceMin);
            t += dStep;
        }

        //last point
        _AddVertexToPolyline(vertices, BP4, pointsDistanceMin);
    }

}

*/


void MoveGeometricShape(VectorShape *gs, Vec2f dPos)
{


    //std::vector<TShapePoint>&points = gs->GetPath();

    for(PathPoint &p: gs->pathPoints()){
        //p += dPos;          // NO becouse it clear distance and smooth corner parameters!
        p.x += dPos.x;
        p.y += dPos.y;
    }

    for(ShapePoint &p: gs->shapePoints()){
        p.x += dPos.x;
        p.y += dPos.y;
        p.BPnext.x += dPos.x;
        p.BPnext.y += dPos.y;
        p.BPprev.x += dPos.x;
        p.BPprev.y += dPos.y;
    }



    /*
    if(gs->GetKind()==ShapeKind::POLYLINE){

        PolyLineShape *poly = static_cast<PolyLineShape *>(gs);

        for(int i=0; i<poly->vertices.size(); i++){
            poly->vertices[i] = poly->vertices[i] + dPos;
        }

    }else if(gs->GetKind()==ShapeKind::BEZIER_POLYCURVE){

        BezierShape *bezpoly = static_cast<BezierShape *>(gs);

        for(int i=0; i<bezpoly->vertices.size(); i++){
            bezpoly->vertices[i].P = bezpoly->vertices[i].P + dPos;
            bezpoly->vertices[i].CPprevious = bezpoly->vertices[i].CPprevious + dPos;
            bezpoly->vertices[i].CPnext = bezpoly->vertices[i].CPnext + dPos;
        }
        //for(int i=0; i<bezpoly->polylineVertices.size(); i++){
        //    bezpoly->polylineVertices[i] = bezpoly->polylineVertices[i] + dPos;
        //}


    }else if(gs->GetKind()==ShapeKind::ELLIPSE){

        EllipseShape *ellipse = static_cast<EllipseShape *>(gs);
        ellipse->center = ellipse->center + dPos;

    }else if(gs->GetKind()==ShapeKind::SINGLE_POINT){

        SinglePointShape *sp = static_cast<SinglePointShape *>(gs);
        sp->point = sp->point + dPos;
    }

    */

    //---- path
    //for(int i=0; i<gs->points().size(); i++){
        //gs->GetPathPoints()[i] = gs->GetPathPoints()[i] + dPos;           // NO becouse it clear distance and smooth corner parameters!
    //    gs->points()[i].x += dPos.x;
    //    gs->points()[i].y += dPos.y;
    //}

}

/*
void DrawTransformedGeometricShape(Drawer *drawer, GeometricShape *vs,  AffineMat3f m, float scale)
{

    Vec2f v, v1, v2;

    if(vs->GetKind()==ShapeKind::POLYLINE){

        PolyLineShape *poly = static_cast<PolyLineShape*>(vs);
        for(int i=0; i<int(poly->vertices.size())-1; i++){
            v1 = poly->vertices[i];
            v2 = poly->vertices[i+1];

            v1 = m.Transform(v1);
            v2 = m.Transform(v2);

            drawer->Line(v1,v2);

        }
        if(poly->IsClosed()){
            v1 = poly->vertices[poly->vertices.size()-1];
            v2 = poly->vertices[0];

            v1 = m.Transform(v1);
            v2 = m.Transform(v2);

            drawer->Line(v1,v2);
        }

    }else if(vs->GetKind()==ShapeKind::BEZIER_POLYCURVE){



        //BezierShape *bezpoly = static_cast<BezierShape*>(vs);
        for(int i=0; i<int(vs->GetPath().size())-1; i++){
            v1 = vs->GetPath()[i];
            v2 = vs->GetPath()[i+1];

            v1 = m.Transform(v1);
            v2 = m.Transform(v2);

            drawer->Line(v1,v2);
        }



    }else if(vs->GetKind()==ShapeKind::ELLIPSE){

        EllipseShape *e = static_cast<EllipseShape*>(vs);
        v = e->center;

        v = m.Transform(v);

        drawer->EllipseOutline(v, Vec2f(scale*e->a, scale*e->b));


    }else if(vs->GetKind()==ShapeKind::SINGLE_POINT){

        SinglePointShape *p = static_cast<SinglePointShape*>(vs);
        v = p->point;
        v = m.Transform(v);
        drawer->Dot(v);

    }

}

*/


/*
void DrawGeometricShape(Drawer* drawer, GeometricShape *vs, jugimap::Vec2f offset)
{


    Vec2f v, v1, v2;

    if(vs->GetKind()==ShapeKind::POLYLINE){

        PolyLineShape *poly = static_cast<PolyLineShape*>(vs);

        for(int i=0; i<int(poly->vertices.size())-1; i++){
            v1 = poly->vertices[i] + offset;
            v2 = poly->vertices[i+1] + offset;

            drawer->Line(v1,v2);
        }
        if(poly->IsClosed()){
            v1 = poly->vertices[poly->vertices.size()-1] + offset;
            v2 = poly->vertices[0] + offset;

            drawer->Line(v1,v2);
        }

    }else if(vs->GetKind()==ShapeKind::BEZIER_POLYCURVE){


        for(int i=0; i<int(vs->GetPath().size())-1; i++){
            v1 = vs->GetPath()[i] + offset;
            v2 = vs->GetPath()[i+1] + offset;

            drawer->Line(v1,v2);
        }




    }else if(vs->GetKind()==ShapeKind::ELLIPSE){

        EllipseShape *e = static_cast<EllipseShape*>(vs);
        v = e->center + offset;;
        drawer->EllipseOutline(v, Vec2f(e->a,e->b));


    }else if(vs->GetKind()==ShapeKind::SINGLE_POINT){

        SinglePointShape *p = static_cast<SinglePointShape*>(vs);
        v = p->point + offset;
        drawer->Dot(v);
    }

}

*/



void GetShapePointsFromBezierSegment(std::vector<PathPoint> &Points, Vec2f BP1, Vec2f BP2, Vec2f BP3, Vec2f BP4, int i)
{


    double distBP2 = distToSegment(BP2, BP1, BP4);
    double distBP3 = distToSegment(BP3, BP1, BP4);

    if(distBP2 < bezierShapeToPolyThreshold && distBP3 < bezierShapeToPolyThreshold){     // odsek BP1-BP4 je priblizno straight line


        if(Points.empty()){
            Points.push_back(PathPoint(BP1, 0.0));
            if(i>1){
                Points.back().smoothCorner = true;
            }
        }else{
            double dist = DistanceTwoPoints(Points.back().x, Points.back().y, BP1.x, BP1.y);
            if(dist>=1.0){
                Points.push_back(PathPoint(BP1, Points.back().distance+dist));
                if(i>1){
                    Points.back().smoothCorner = true;
                }
            }else{
                if(i==1){
                    Points.back().smoothCorner = false;         // change to sharp corner
                }
            }
        }


        double dist = DistanceTwoPoints(Points.back().x, Points.back().y, BP4.x, BP4.y);
        if(dist>=1.0){
            Points.push_back(PathPoint(BP4, Points.back().distance+dist));
            if(i>1){
                Points.back().smoothCorner = true;
            }

        }else{
            if(i==1){
                Points.back().smoothCorner = false;         // change to sharp corner
            }
        }




    }else{  // subdivide bezier odsek

        Vec2f L1 = BP1;
        Vec2f L2 = (BP1+BP2)*0.5;
        Vec2f H = (BP2+BP3)*0.5;
        Vec2f L3 = (L2+H)*0.5;

        Vec2f R4 = BP4;
        Vec2f R3 = (BP3+BP4)*0.5;
        Vec2f R2 = (H+R3)*0.5;

        Vec2f L4 = (L3+R2)*0.5;
        Vec2f R1 = L4;

        GetShapePointsFromBezierSegment(Points, L1, L2, L3, L4, i+1);
        GetShapePointsFromBezierSegment(Points, R1, R2, R3, R4, i+1);
    }

}


float bezierShapeToPolyThreshold = 1.0f;


VectorShape* ConvertEllipseToBezierCurve(VectorShape *ellipse)
{
    assert(ellipse->kind()==ShapeKind::ELLIPSE);

    float a = ellipse->a();
    float b = ellipse->b();

    float f = 0.5522847498;

    VectorShape *vs = new VectorShape(ShapeKind::BEZIER_POLYCURVE);
    vs->mClosed = ellipse->mClosed;

    ShapePoint sp1 = ellipse->shapePoints().at(1);      // bottom point
    sp1.BPprev.set(sp1.x - a*f, sp1.y);
    sp1.BPnext.set(sp1.x + a*f, sp1.y);
    vs->shapePoints().push_back(sp1);

    ShapePoint sp2 = ellipse->shapePoints().at(2);      // right
    sp2.BPprev.set(sp2.x, sp2.y - b*f);
    sp2.BPnext.set(sp2.x, sp2.y + b*f);
    vs->shapePoints().push_back(sp2);

    ShapePoint sp3 = ellipse->shapePoints().at(3);      // top
    sp3.BPprev.set(sp3.x + a*f, sp3.y);
    sp3.BPnext.set(sp3.x - a*f, sp3.y);
    vs->shapePoints().push_back(sp3);

    ShapePoint sp4 = ellipse->shapePoints().at(4);      // left
    sp4.BPprev.set(sp4.x, sp4.y + b*f);
    sp4.BPnext.set(sp4.x, sp4.y - b*f);
    vs->shapePoints().push_back(sp4);

    vs->updatePathPoints();


    //---
    vs->mName = ellipse->mName;
    vs->mDataFlags = ellipse->mDataFlags;
    vs->mId = ellipse->mId;
    vs->mParameters = ellipse->mParameters;
    vs->mExtra = ellipse->mExtra;


    return vs;

}




}
