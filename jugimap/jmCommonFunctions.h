#ifndef JUGIMAP_COMMON_FUNCTIONS_H
#define JUGIMAP_COMMON_FUNCTIONS_H

#include <assert.h>
#include <algorithm>
#include <cmath>
#include <vector>
#include <string>
#include "jmCommon.h"


namespace ncine {
class SceneNode;
}


namespace jugimap {



/// \addtogroup CommonFunctions
/// @{


//const double pi = 3.14159265358979323846;


const double mathPI = 3.14159265;




/// \brief Print ASCII text in IDE output window - MS Windows only.
//void DbgOutput(const std::string &_text);


void print(const std::string &text);



/// \brief Returns true if the given **a** and **b** are equal; otherwise returns false.
///
/// The compared floating-point numbers are considered equal if their difference is smaller then the given **epsilon**.
inline bool AreEqual(double a, double b, double epsilon=0.0001)
{
    return fabs(a - b) < epsilon;
}


/// \brief Returns 1 if the sign of the given **value** is positive number; returns -1 if it's negative; returns 0 if it's 0.
template <typename T>
inline int Sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

inline int sign(float value)
{
    return value>0 ? 1 : -1 ;
}


/// \brief Returns a random float number between (and including) the given **fMin** and **fMax**.
double fRand(double fMin, double fMax);


/// \brief Returns a random integer number between (and including) the given **iMin** and **iMax**.
int iRand(int iMin, int iMax);


/// \brief Returns a **value** clamped to the **valueMin** and **valueMax**.
template<typename T>
inline T ClampValue(T value, T valueMin, T valueMax)
{
    if(value<=valueMin)
        return valueMin;
    else if(value>=valueMax)
        return valueMax;
    else
        return value;

}


template<typename T>
inline void Clamp(T &value, T valueMin, T valueMax)
{
    if(value<valueMin)
        value = valueMin;
    else if(value>valueMax)
        value = valueMax;

}

//std::string ExtractNameFromPath(std::string path);


//std::vector<std::string>SplitString(std::string text, const std::string &delimiter);


//void RemoveStringWhiteSpacesOnStartAndEnd(std::string &s);





inline void DummyFunction()
{
}



/// Returns a Vec2f object formed by converting Vec2i to Vec2f.
inline Vec2f Vec2iToVec2f(const Vec2i &v)
{
    return Vec2f(v.x, v.y);
}


/// Returns a Vec2i object formed by converting Vec2f to Vec2i.
inline Vec2i Vec2fToVec2i(const Vec2f &v)
{
    return Vec2i(std::round(v.x), std::round(v.y));
}


inline BoundingBoxFloat BoundingBoxIntToFloat(BoundingBoxInt bb)
{
    return BoundingBoxFloat(Vec2iToVec2f(bb.min), Vec2iToVec2f(bb.max));
}


inline BoundingBoxInt BoundingBoxFloatToInt(BoundingBoxFloat bb)
{
    return BoundingBoxInt(Vec2fToVec2i(bb.min), Vec2fToVec2i(bb.max));
}



/// Returns a Rectf object formed by converting the given Recti object **r** to Rectf.
inline BoundingBoxFloat RectiToRectf(const BoundingBoxInt &r)
{
    return BoundingBoxFloat(Vec2iToVec2f(r.min), Vec2iToVec2f(r.max));
}

/// Returns a Recti object formed by converting the given Rectf object **r** to Recti.
inline BoundingBoxInt RectfToRecti(const BoundingBoxFloat &r)
{
    return BoundingBoxInt(Vec2fToVec2i(r.min), Vec2fToVec2i(r.max));
}



/// Returns an AffineMat3f matrix for the given transformations **pos**, **scale**, **flip**, **rotation** and **handle**.
AffineMat3f MakeTransformationMatrix(Vec2f pos, Vec2f scale, Vec2i flip, float rotation, Vec2f handle);

/// Returns an AffineMat3f matrix for the given transformations **pos**, **scale**, **flip** and **rotation**.
AffineMat3f MakeTransformationMatrix(Vec2f pos, Vec2f scale, Vec2i flip, float rotation);


/// Returns a Rectf bounding of the given rectangle **rect** transformed by **matrix**.
BoundingBoxFloat TransformRectf(const BoundingBoxFloat &rect, const AffineMat3f &matrix);



/// Returns an AffineMat3f matrix for the given transformations **pos**, **scale**, **flip**, **rotation** and **handle**.
ncine::Matrix4x4f MakeTransformationMatrix(Vec2f pos, Vec2f scale, float rotation, Vec2f handle, Vec2i flip={0,0});

/// Returns an AffineMat3f matrix for the given transformations **pos**, **scale**, **flip** and **rotation**.
ncine::Matrix4x4f MakeTransformationMatrix(Vec2f pos, Vec2f scale, float rotation, Vec2i flip={0,0});


/// Returns a Rectf bounding of the given rectangle **rect** transformed by **matrix**.
BoundingBoxFloat TransformRectf(const BoundingBoxFloat &rect, const ncine::Matrix4x4f &matrix);



// GEOMETRY
//------------------------------------------------------------------------------------


template<typename T>
inline bool IsEqual(const T &v1 , const T &v2, float epsilon=0.0001f)
{
    //return  fabs(v1.x - v2.x)<epsilon && fabs(v1.y - v2.y)<epsilon ;

    float dx = (v1.x > v2.x)? v1.x-v2.x : v2.x - v1.x;
    float dy = (v1.y > v2.y)? v1.y-v2.y : v2.y - v1.y;

    return dx<epsilon && dy<epsilon;
}


/*
template<typename T>
inline float Distance(const Vec2<T>&v1 , const Vec2<T>&v2)
{
    Vec2<T> v = v1 - v2;
    return std::sqrt(v.x*v.x + v.y*v.y);
}

template<typename T1, typename T2>
inline float Distance(const Vec2<T1>&v1 , const Vec2<T2>&v2)
{
    Vec2f v = {v1.x - v2.x, v1.y - v2.y};
    return std::sqrt(v.x*v.x + v.y*v.y);
}


inline float Distance(float x1, float y1, float x2, float y2)
{
    return std::sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}


template<typename T>
inline float LengthSqr(const Vec2<T>&v)
{
    return v.x*v.x + v.y*v.y;
}


template<typename T>
inline float DistanceSqr(const Vec2<T>&v1 , const Vec2<T>&v2)
{
    return LengthSqr(v1 - v2);
}

*/



template<typename T>
inline float DistanceTwoPoints(const T& P1, const T& P2)
{
    return std::sqrt((P2.x-P1.x)*(P2.x-P1.x) + (P2.y-P1.y)*(P2.y-P1.y));
}

template<typename T1, typename T2>
inline float DistanceTwoPoints(const T1& P1, const T2& P2)
{
    return std::sqrt((P2.x-P1.x)*(P2.x-P1.x) + (P2.y-P1.y)*(P2.y-P1.y));
}

//template<typename T>
//inline float DistanceTwoPoints(T x1, T y1, T x2, T y2)
//{
//    return std::sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
//}
inline float DistanceTwoPoints(float x1, float y1, float x2, float y2)
{
    return std::sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}


template<typename T>
inline float SquareDistanceTwoPoints(const T& P1, const T& P2)
{
    return (P1.x - P2.x)*(P1.x - P2.x) + (P1.y - P2.y)*(P1.y - P2.y);
}



/// Returns a Vec2f at the given **distance** from **P1** to **P2** using linear interpolation.
template<typename T>
Vec2f MakeVec2fAtDistanceFromP1(const T& p1, const T& p2, double dist)
{
    //double fDistance = distance/p1.Distance(p2);
    double fDistance = dist/DistanceTwoPoints(p1, p2);
    return Vec2f(p1.x + fDistance* (p2.x-p1.x), p1.y + fDistance* (p2.y-p1.y));
}


///\brief Returns a Vec2f at the given **fDistance** from **P1** to **P2** using linear interpolation.
/// **fDistance** is defined as 'distanceP1toP/distanceP1toP2'.
template<typename T>
Vec2f MakeVec2fAtDistanceFactorFromP1(const T& p1, const T& p2, double fDistance)
{
    return Vec2f(p1.x + fDistance* (p2.x-p1.x), p1.y + fDistance* (p2.y-p1.y));
}




template<typename T>
inline double distToSegment(const T& P, const T& P1, const T& P2, T& ProjectedPoint)
{
  double l2 = SquareDistanceTwoPoints(P1, P2);
  if (l2 == 0){
      ProjectedPoint.set(P1.x, P1.y);
      return std::sqrt(SquareDistanceTwoPoints(P, P1));
  }
  double t = ((P.x - P1.x) * (P2.x - P1.x) + (P.y - P1.y) * (P2.y - P1.y)) / l2;
  if (t < 0){
      ProjectedPoint.set(P1.x, P1.y);
      return std::sqrt(SquareDistanceTwoPoints(P, P1));
  }
  if (t > 1){
      ProjectedPoint.set(P2.x, P2.y);
      return std::sqrt(SquareDistanceTwoPoints(P, P2));
  }
  ProjectedPoint.set(P1.x +  t* ( P2.x - P1.x ), P1.y +  t* ( P2.y - P1.y ) );
  return std::sqrt(SquareDistanceTwoPoints(P, ProjectedPoint));
}


template<typename T>
inline double distToSegment(const T& P, const T& P1, const T& P2)
{

    T projectedPoint;
    return distToSegment(P, P1, P2 , projectedPoint);

    /*
    double l2 = SquareDistanceTwoPoints(P1, P2);
    if (l2 == 0) return std::sqrt(SquareDistanceTwoPoints(P, P1));
    double t = ((P.x - P1.x) * (P2.x - P1.x) + (P.y - P1.y) * (P2.y - P1.y)) / l2;
    if (t < 0){
        return std::sqrt(SquareDistanceTwoPoints(P, P1));
    }
    if (t > 1){
        return std::sqrt(SquareDistanceTwoPoints(P, P2));
    }
    T ProjectedPoint(P1.x +  t* ( P2.x - P1.x ), P1.y +  t* ( P2.y - P1.y ) );
    return std::sqrt(SquareDistanceTwoPoints(P, ProjectedPoint));
    */
}


template<typename T>
inline double distToLine(const T& P, const T& P1, const T& P2, T& ProjectedPoint)
{
  double l2 = SquareDistanceTwoPoints(P1, P2);
  if (l2 == 0) return std::sqrt(SquareDistanceTwoPoints(P, P1));
  double t = ((P.x - P1.x) * (P2.x - P1.x) + (P.y - P1.y) * (P2.y - P1.y)) / l2;

  ProjectedPoint.set(P1.x +  t* ( P2.x - P1.x ), P1.y +  t* ( P2.y - P1.y ) );
  return std::sqrt(SquareDistanceTwoPoints(P, ProjectedPoint));
}


template<typename T>
inline double distToLine(const T& P, const T& P1, const T& P2)
{
    T projectedPoint;
    return distToLine(P, P1, P2 , projectedPoint);
}


template<typename T>
inline double AngleBetweenTwoPoints(const T& P1, const T& P2)
{
    return std::atan2(P2.y-P1.y, P2.x-P1.x) * 180.0/mathPI;
}


bool Point_in_polygon(double tx, double ty, const std::vector<Vec2f> &Poly);




template<typename T>
bool Intersect(const T& PA1, const T& PA2, const T& PB1, const T& PB2, T& PResult)
{
    static const double intersection_epsilon = 1.0e-30;

    double num = (PA1.y-PB1.y) * (PB2.x-PB1.x) - (PA1.x-PB1.x) * (PB2.y-PB1.y);
    double den = (PA2.x-PA1.x) * (PB2.y-PB1.y) - (PA2.y-PA1.y) * (PB2.x-PB1.x);
    if(fabs(den) < intersection_epsilon) return false;
    double r = num / den;
    PResult.x = PA1.x + r * (PA2.x-PA1.x);
    PResult.y = PA1.y + r * (PA2.y-PA1.y);
    return true;

}



// VECTOR
//------------------------------------------------------------------------------------


struct StdVector
{

    template<typename T>
    static int indexOfElement(std::vector<T*>&v, T* element)
    {
        for(int i=0; i<v.size(); i++){
            if(v[i]==element){
                return i;
            }
        }
        return -1;
    }


    template<typename T>
    static int indexOfElement(std::vector<T>&v, const T &element)
    {
        for(int i=0; i<v.size(); i++){
            if(v[i]==element){
                return i;
            }
        }
        return -1;
    }


    template<typename T>
    static bool removeElement(std::vector<T*>&v, T* element)
    {
        for(int i = int(v.size())-1; i>=0; i-- ){
            if( v[i]==element ){
                v.erase(v.begin()+i);
                return true;
            }
        }
        return false;
    }

    template<typename T>
    static bool removeElement(std::vector<T>&v, T& element)
    {
        for(int i = int(v.size())-1; i>=0; i-- ){
            if( v[i]==element ){
                v.erase(v.begin()+i);
                return true;
            }
        }
        return false;
    }


    template<typename T>
    static bool addElementIfNotInVector(std::vector<T*>&v, T* element)
    {

        if(std::find(v.begin(), v.end(), element) == v.end()) {
            v.push_back(element);
            return true;
        }

        return false;
    }


    template<typename T>
    static bool contains(std::vector<T*>&v, T* element)
    {
        for(int i=0; i<v.size(); i++){
            if(v[i]==element){
                return true;
            }
        }
        return false;
    }


    template<typename T>
    static bool contains(std::vector<T>&v, const T &element)
    {
        for(int i=0; i<v.size(); i++){
            if(v[i]==element){
                return true;
            }
        }
        return false;
    }


    template<typename T>
    static void RemoveAllElements(std::vector<T*>&v, T* element)
    {
        v.erase(std::remove(v.begin(), v.end(), element), v.end());
    }


};



template<typename T>
int GetIndexOfVectorElement(std::vector<T*>&v, T* element)
{
    int index = -1;

    if(v.empty()==false){
        for(int i=0; i<v.size(); i++){
            if(v[i]==element){
                index = i;
            }
        }
    }

    return index;
}


template<typename T>
bool RemoveElementFromVector(std::vector<T*>&v, T* element)
{

    for(int i = int(v.size())-1; i>=0; i-- ){
        if( v[i]==element ){
            v.erase(v.begin()+i);
            return true;
        }
    }

    return false;

}


template<typename T>
bool AddElementToVectorIfNotIn(std::vector<T*>&v, T* element)
{

    if(std::find(v.begin(), v.end(), element) == v.end()) {
        v.push_back(element);
        return true;
    }

    return false;
}


template<typename T>
bool vectorContains(std::vector<T*>&v, T* element)
{

    for(int i=0; i<v.size(); i++){
        if(v[i]==element){
            return true;
        }
    }

    return false;
}



template<typename T>
int GetIndexOfVectorElement(std::vector<T>&v, const T &element)
{
    int index = -1;

    if(v.empty()==false){
        for(int i=0; i<v.size(); i++){
            if(v[i]==element){
                index = i;
            }
        }
    }

    return index;
}


template<typename T>
void RemoveAllFromVector(std::vector<T*>&v, T* element)
{
    v.erase(std::remove(v.begin(), v.end(), element), v.end());
}



// STRING
//------------------------------------------------------------------------------------




inline bool isWhiteSpaceCharacter(char c) { return (c==' ' || c== '\n' || c=='\r' || c=='\t' || c=='\v' || c== '\f'); }


struct StdString
{

    static float stringToFloat(const std::string &_text, float defNumber);

    static int stringToInt(const std::string &_text, int defNumber);

    static bool stringToBool(const std::string &_text);

    static std::string baseNameFromFilePath(const std::string &path);

    static std::string fileNameFromFilePath(const std::string &path);

    static std::string pathFromFilePath(const std::string &path);

    static std::string suffixFromFilePath(const std::string &path);

    std::string removeSuffixFromFilePath(const std::string &path);

    static std::vector<std::string> splitString(std::string text, const std::string &delimiter, bool removeWhiteSpacesOnStartAndEnd = true);

    static std::string joinStrings(std::vector<std::string> &texts, const std::string &delimiter);

    static void appendNonEmptyString(std::string &source, const std::string &added, const std::string &delimiter);

    static void removeStringWhiteSpacesOnStartAndEnd(std::string &s);

    static void removeEndSlash(std::string &s);

};



struct Misc
{

    static inline int GetTileIndex(double value){ return int(std::round(std::floor(value))); }

};






//------------------------------------------------------------------------------------



/// @}      //end addtogroup CommonFunctions



}




#endif


