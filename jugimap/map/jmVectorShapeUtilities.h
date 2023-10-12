#ifndef JUGIMAP_VECTOR_SHAPES_UTILITIES_H
#define JUGIMAP_VECTOR_SHAPES_UTILITIES_H


//#include "jmVectorShapes.h"
//#include "jmColor.h"
//#include "jmUsingDeclarations.h"
#include "jmCommon.h"



namespace jugimap{

class ShapeDrawer;
class VectorShape;


bool IsPolygonConvex(std::vector<Vec2f> &vertices, bool &cw);
bool SameGeometricShapes(std::vector<VectorShape *>&shapes1, std::vector<VectorShape *>&shapes2);
bool GeometricShapesEqual(VectorShape * gs1, VectorShape *gs2);
//void BezierPolycurveToPolyline(std::vector<BezierVertex>&bezierVertices,  std::vector<Vec2f>&vertices);

void MoveGeometricShape(VectorShape * gs, Vec2f dPos);


//void DrawTransformedGeometricShape(Drawer* drawer, GeometricShape *vs, AffineMat3f m, float scale);
//void DrawGeometricShape(Drawer* drawer, GeometricShape *vs, Vec2f offset=Vec2f());

void GetShapePointsFromBezierSegment(std::vector<PathPoint> &Points, Vec2f BP1, Vec2f BP2, Vec2f BP3, Vec2f BP4, int i);


VectorShape* ConvertEllipseToBezierCurve(VectorShape *ellipse);


extern float bezierShapeToPolyThreshold;






}




#endif
