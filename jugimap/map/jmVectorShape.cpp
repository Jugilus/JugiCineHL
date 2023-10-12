#include <assert.h>
#include "jmVectorShapeUtilities.h"
#include "jmVectorShape.h"



using namespace jugimap;

namespace jugimap{



TPointMarkers PointMarkers;


void VectorShape::updatePathPoints()
{

    if(kind()==ShapeKind::RECTANGLE){
        UpdateRectanglePointsFromControlPoints();

    }else if(kind()==ShapeKind::POLYLINE){
        UpdatePolylinePointsFromControlPoints();

    }else if(kind()==ShapeKind::BEZIER_POLYCURVE){
        UpdateBezierPolylinePointsFromControlPoints();

    }else if(kind()==ShapeKind::ELLIPSE){
        UpdateEllipsePointsFromControlPoints();

    }else if(kind()==ShapeKind::SINGLE_POINT){
        UpdateSinglePointFromControlPoints();

    }

}


void VectorShape::updateBoundingBox()
{

    mBoundingBox.setInitialSize();
    for(Vec2f &P : mPoints){
        mBoundingBox.Expand(Vec2i(P.x,P.y));
    }

}


void VectorShape::setPathPointsAngles()
{

    if(mName=="test2"){
        DummyFunction();
    }

    float sharpCornerThreshold = 10.0f;

    for(int i=0; i<int(mPoints.size())-1; i++){

        float directionAngle = AngleBetweenTwoPoints(mPoints[i], mPoints[i+1]);
        if(directionAngle<0) directionAngle += 360;

        mPoints[i].angle = directionAngle;

        /*
        if(i-1>=0){
            mPoints[i].smoothCorner = (std::abs(mPoints[i-1].angle - mPoints[i].angle)) < sharpCornerThreshold ;

            if(mName=="test2" && mPoints[i].smoothCorner==false){
                DummyFunction();
            }
        }
        */

    }

    if(mClosed){
        mPoints[mPoints.size()-1].angle = mPoints[0].angle;

        //mPoints[0].smoothCorner = (std::abs(mPoints[mPoints.size()-1].angle - mPoints[0].angle)) < sharpCornerThreshold ;

    }else{
        //mPoints[0].smoothCorner = false;
        mPoints[mPoints.size()-1].smoothCorner = false;
    }


    if(mKind==ShapeKind::BEZIER_POLYCURVE){

        for(PathPoint &pp : mPoints){
            pp.smoothCorner = true;
        }
        for(ShapePoint &sp : mCPoints){
            if(IsEqual(sp.BPprev, sp.BPnext)){      // only when BPprev and BPnext have the same position as sp -> SHARP CORNER
                for(int i=0; i<mPoints.size(); i++){
                    PathPoint &pp = mPoints[i];
                //for(PathPoint &pp : mPoints){
                    if(pp.x == sp.x && pp.y == sp.y){
                        pp.smoothCorner = false;
                        break;
                    }
                }

            }
        }
        if(mClosed){
            mPoints.back().smoothCorner = mPoints.front().smoothCorner;     // back point is duplicate of front point
        }
    }


}


Vec2f VectorShape::obtainPathPoint(float p)
{

    float distance = p * mPathLength;
    int indexP1 = 0;
    int indexP2 = mPoints.size()-1;

    while(indexP2 - indexP1 > 1){

        int indexCheck = (indexP1+indexP2)/2;

        if(mPoints[indexCheck].distance > distance){
            indexP2 = indexCheck;

        }else if(mPoints[indexCheck].distance < distance){
            indexP1 = indexCheck;

        }else{
            return Vec2f(mPoints[indexCheck].x, mPoints[indexCheck].y);

        }
    }

    double distP1P2 = DistanceTwoPoints(mPoints[indexP1], mPoints[indexP2]);
    double fDist = 0;
    if(AreEqual(distP1P2, 0.0)==false){
        fDist = (distance-mPoints[indexP1].distance)/distP1P2;
    }
    Vec2f pos = MakeVec2fAtDistanceFactorFromP1(mPoints[indexP1], mPoints[indexP2], fDist);

    return pos;

}


Vec2f VectorShape::obtainPathPoint(float p, float &directionAngle)
{

    float distance = p * mPathLength;
    int indexP1 = 0;
    int indexP2 = mPoints.size()-1;

    while(indexP2 - indexP1 > 1){

        int indexCheck = (indexP1+indexP2)/2;

        if(mPoints[indexCheck].distance > distance){
            indexP2 = indexCheck;

        }else if(mPoints[indexCheck].distance < distance){
            indexP1 = indexCheck;

        }else{

            if(indexCheck+1<mPoints.size()){
                directionAngle = AngleBetweenTwoPoints(mPoints[indexCheck], mPoints[indexCheck+1]);
            }else if(indexCheck-1>=0){
                directionAngle = AngleBetweenTwoPoints(mPoints[indexCheck-1], mPoints[indexCheck]);
            }else{
                directionAngle = 0.0;
            }
            if(directionAngle<0) directionAngle += 360;
            return Vec2f(mPoints[indexCheck].x, mPoints[indexCheck].y);

        }
    }

    //Vec2f pos = GetPointAtDistanceFromP1( Points[indexP1],  Points[indexP2], distance-Points[indexP1].distance);

    double distP1P2 = DistanceTwoPoints(mPoints[indexP1], mPoints[indexP2]);
    double fDist = 0;
    if(AreEqual(distP1P2, 0.0)==false){
        fDist = (distance-mPoints[indexP1].distance)/distP1P2;
    }
    Vec2f pos = MakeVec2fAtDistanceFactorFromP1(mPoints[indexP1], mPoints[indexP2], fDist);


    //---:
    //directionAngle = Points[indexP1].angle;
    directionAngle = AngleBetweenTwoPoints(mPoints[indexP1], mPoints[indexP2]);
    if(directionAngle<0) directionAngle += 360;


    //print("");
    //print("indexP1:" + std::to_string(indexP1) + " indexP2:" + std::to_string(indexP2));
    //print("A directionAngle:" + std::to_string(directionAngle));

    int indexPrevious = indexP1-1;
    if(indexPrevious<0 && mClosed){
        indexPrevious = mPoints.size()-1;
        if(DistanceTwoPoints(mPoints[0],mPoints[indexPrevious])<0.5){
            indexPrevious--;
        }
    }

    if(indexPrevious>=0){
        if(mPoints[indexP1].smoothCorner){

            //float prevDirectionAngle = Points[indexPrevious].angle;
            float prevDirectionAngle = AngleBetweenTwoPoints(mPoints[indexPrevious], mPoints[indexP1]);
            if(prevDirectionAngle<0) prevDirectionAngle += 360;

            if(std::abs(prevDirectionAngle-directionAngle)>180){

                if(prevDirectionAngle<directionAngle){
                    prevDirectionAngle += 360;
                    //print("   modify: prevDirectionAngle + 360");
                }else{
                    directionAngle += 360;
                    //print("   modify: directionAngle + 360");
                }
            }

            //print("B1 prevDirectionAngle: " + std::to_string(prevDirectionAngle));


            //if(sgn(prevDirectionAngle)==sgn(directionAngle)){
                directionAngle = prevDirectionAngle + fDist*(directionAngle - prevDirectionAngle);
            //    qDebug()<<"B2 directionAngle:"<< directionAngle;
            //}else{
            //    qDebug()<<"B sgn changed!";
            //}



        //}else{
        //    qDebug()<<"B smoothCorner = false";

        }
    }

    return pos;

}


bool VectorShape::isRectangle()
{

    if(mCPoints.size() != 4) return false;

    if(mCPoints[0].x==mCPoints[1].x && mCPoints[2].x==mCPoints[3].x && mCPoints[1].y==mCPoints[2].y && mCPoints[3].y==mCPoints[0].y){
        return true;
    }
    if(mCPoints[1].x==mCPoints[2].x && mCPoints[0].x==mCPoints[3].x && mCPoints[0].y==mCPoints[1].y && mCPoints[2].y==mCPoints[3].y){
        return true;
    }

    return false;
}


void VectorShape::setSinglePoint(Vec2i p)
{

    if(mCPoints.empty()){
        mCPoints.push_back(ShapePoint(p.x, p.y));
    }else{
        mCPoints[0].set(p.x, p.y);
    }

    updatePathPoints();
}


void VectorShape::setSinglePoint(int _x, int _y)
{
    if(mCPoints.empty()){
        mCPoints.push_back(ShapePoint(_x,_y));
    }else{
        mCPoints[0].set(_x,_y);
    }

    updatePathPoints();
}


Vec2i VectorShape::singlePoint()
{
    assert(mCPoints.size()==1);
    return Vec2i(mCPoints[0].x, mCPoints[0].y);
}


Vec2f VectorShape::singlePointF()
{
    assert(mCPoints.size()==1);
    return Vec2f(mCPoints[0].x, mCPoints[0].y);
}



//================================================================================


void VectorShape::UpdateRectanglePointsFromControlPoints()
{

    mPoints.clear();

    float angle = 0.0;
    float distance = 0.0;

    for(int i=0; i<mCPoints.size(); i++){
        if(i>0){
            distance += DistanceTwoPoints(mCPoints[i-1],mCPoints[i]);
        }
        mPoints.push_back(PathPoint(Vec2f(mCPoints[i].x, mCPoints[i].y), distance));

    }
    if(mPoints.empty()==false){
        distance += DistanceTwoPoints(mCPoints.back(), mCPoints.front());
        mPoints.push_back(PathPoint(Vec2f(mCPoints.front().x, mCPoints.front().y), distance));
        mPathLength = mPoints.back().distance;
    }

    setPathPointsAngles();
    updateBoundingBox();

}


void VectorShape::UpdatePolylinePointsFromControlPoints()
{

    mPoints.clear();
    float distance = 0.0;

    for(int i=0; i<mCPoints.size(); i++){
        if(i>0){
            distance += DistanceTwoPoints(mCPoints[i-1],mCPoints[i]);
        }
        mPoints.push_back(PathPoint(Vec2f(mCPoints[i].x, mCPoints[i].y), distance));
    }
    if(mPoints.empty()==false){
        if(mClosed){
            distance += DistanceTwoPoints(mCPoints.back(), mCPoints.front());
            mPoints.push_back(PathPoint(Vec2f(mCPoints.front().x, mCPoints.front().y), distance));
        }
        mPathLength = mPoints.back().distance;
    }

    //qDebug()<<"path length:"<<pathLength;

    setPathPointsAngles();
    updateBoundingBox();

}


void VectorShape::UpdateBezierPolylinePointsFromControlPoints()
{

    mPoints.clear();

    if(mCPoints.empty()==false){

        for(int i=0; i<mCPoints.size()-1; i++){

            int nPointsStart = mPoints.size();

            Vec2f P(mCPoints[i].x, mCPoints[i].y);
            Vec2f Pnext(mCPoints[i+1].x, mCPoints[i+1].y);

            Vec2f BP1 = P;
            Vec2f BP2 = mCPoints[i].BPnext;
            Vec2f BP3 = mCPoints[i+1].BPprev;
            Vec2f BP4 = Pnext;

            GetShapePointsFromBezierSegment(mPoints, BP1, BP2, BP3, BP4, 1);

            //qDebug()<<"i:"<<i<<"  Points.size:"<<Points.size()<<" PointsCPnextIndex.size:"<<PointsCPnextIndex.size()<<" nAddedPoints:"<<nAddedPoints;
        }

        if(mClosed){

            int nPointsStart = mPoints.size();

            Vec2f P(mCPoints.back().x, mCPoints.back().y);
            Vec2f Pnext(mCPoints.front().x, mCPoints.front().y);

            Vec2f BP1 = P;
            Vec2f BP2 = mCPoints.back().BPnext;
            Vec2f BP3 = mCPoints.front().BPprev;
            Vec2f BP4 = Pnext;

            GetShapePointsFromBezierSegment(mPoints, BP1, BP2, BP3, BP4, 1);

            if(mPoints.front().smoothCorner==false || mPoints.back().smoothCorner==false){
                mPoints.front().smoothCorner = mPoints.back().smoothCorner = false;
            }

        }

        if(mPoints.empty()==false){
            mPathLength = mPoints.back().distance;
        }

        //qDebug()<<"path length:"<<pathLength;


    }

    setPathPointsAngles();
    updateBoundingBox();

}


void VectorShape::UpdateEllipsePointsFromControlPoints()
{

    mPoints.clear();


    ma = std::abs(mCPoints[RIGHT].x - mCPoints[CENTER].x);
    mb = std::abs(mCPoints[TOP].y - mCPoints[CENTER].y);
    mCenter = Vec2f(mCPoints[CENTER].x, mCPoints[CENTER].y);

    //int n = std::max(a+b, 12 ) & (~3) ;

    float ra = (ma+mb) / 2.0;
    float da = std::acos(ra / (ra + 0.125 /1.0)) * 2;
    int n = std::round(2*mathPI / da + 0.5);

    //n *= 4;
   // n = std::max(n,12 );
    //n = std::min(n,1000 );

    float distance = 0.0;
    Vec2f pPrev;

    for(int i=0; i<n; i++){
        float th = (i+.5) * 2*mathPI/n;
        Vec2f p(mCenter.x + std::cos( th ) * ma, mCenter.y + std::sin( th ) * mb);

        if(i>0){
            distance += DistanceTwoPoints(pPrev,p);
        }
        mPoints.push_back(PathPoint(p,distance, true));

        pPrev = p;
    }

    //close ellipse
    distance +=  DistanceTwoPoints(mPoints.back(), mPoints.front());
    mPoints.push_back(PathPoint(Vec2f(mPoints.front().x, mPoints.front().y), distance, true));   // close ellipse

    if(mPoints.empty()==false){
        mPathLength = mPoints.back().distance;
    }

    //qDebug()<<"path length:"<<pathLength;

    setPathPointsAngles();
    updateBoundingBox();


}


void VectorShape::UpdateSinglePointFromControlPoints()
{

    mPoints.clear();
    mPoints.push_back(PathPoint(Vec2f(mCPoints[0].x, mCPoints[0].y)));

    mPathLength = 0;
    updateBoundingBox();

}



}
