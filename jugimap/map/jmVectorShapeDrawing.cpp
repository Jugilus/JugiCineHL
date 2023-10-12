#include <algorithm>
#include "jmSystem.h"
#include "jmCommonFunctions.h"
#include "jmInput.h"
#include "jmVectorShapeDrawing.h"
#include "jmCamera.h"
#include "jmMap.h"
#include "jmVectorLayer.h"
#include "jmSprite.h"
#include "jmCompositeSprite.h"
#include "jmStandardSprite.h"
#include "jmSourceSprite.h"
#include "jmVectorShape.h"
#include "jmVectorShapeUtilities.h"


namespace jugimap {




ShapeDrawer::ShapeDrawer()
{
}

ShapeDrawer::~ShapeDrawer()
{
}


void ShapeDrawer::begin()
{

    ImGui::SetNextWindowPos(ImVec2(-4, -1));        // moram zamakniti ker drugače ne riše ob robu - ???
    ImGui::SetNextWindowSize(ImVec2(jugimap::settings.GetScreenSize().x+8, jugimap::settings.GetScreenSize().y+2));
    ImGui::SetNextWindowContentSize(ImVec2(jugimap::settings.GetScreenSize().x, jugimap::settings.GetScreenSize().y));


    ImGui::Begin("My shapes", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs);
    mDrawList = ImGui::GetWindowDrawList();

}


void ShapeDrawer::end()
{
    ImGui::End();
}


void ShapeDrawer::drawShape(VectorShape *_shape, const ncine::Matrix4x4f &_mLocalToWorld, Vec2f _scaleLocalToWorld)
{

    drawTransformedShape(_shape, _mLocalToWorld, _scaleLocalToWorld);

}


void ShapeDrawer::drawShape_withCamera(VectorShape *_shape, const ncine::Matrix4x4f &_mLocalToWorld, Vec2f _scaleLocalToWorld)
{


    //Vec2f cameraScale = {mCamera->GetScale(), mCamera->GetScale()};
    //float cameraRot  = mCamera->GetRotation();
    //Vec2f cameraPosition = mCamera->GetProjectedMapPosition();

    //ncine::Matrix4x4f mCamera = ncine::Matrix4x4f::translation(cameraPosition.x, cameraPosition.y, 0.0f);
    //mCamera.rotateZ(-cameraRot);
    //mCamera.scale(cameraScale.x, cameraScale.y, 1.0f);


    assert(mCamera);

    const ncine::Matrix4x4f & cameraMatrix = mCamera->GetMatrix();
    const ncine::Matrix4x4f m = cameraMatrix * _mLocalToWorld;
    Vec2f scale = _scaleLocalToWorld * mCamera->GetScale() * settings.appScale();

    drawTransformedShape(_shape, m, scale);

}


void ShapeDrawer::drawLine(jugimap::Vec2f p1, jugimap::Vec2f p2)
{

    mDrawList->AddLine(ImVec2(p1.x, ImGui::GetIO().DisplaySize.y-p1.y), ImVec2(p2.x, ImGui::GetIO().DisplaySize.y-p2.y), mOutlineColor);

}


void ShapeDrawer::drawLine_withCamera(Vec2f p1, Vec2f p2)
{
    //VectorShape s(ShapeKind::POLYLINE);
    //s.pathPoints().push_back(p1);
    //s.pathPoints().push_back(p2);
    //drawTransformedShape(&s, _m);

    assert(mCamera);

    const ncine::Matrix4x4f & cameraMatrix = mCamera->GetMatrix();
    p1 = transform(p1, cameraMatrix);
    p1 = transform(p2, cameraMatrix);

    mDrawList->AddLine(ImVec2(p1.x, ImGui::GetIO().DisplaySize.y-p1.y), ImVec2(p2.x, ImGui::GetIO().DisplaySize.y-p2.y), mOutlineColor);
}


void ShapeDrawer::drawRectangle(jugimap::BoundingBoxFloat r)
{

    mDrawList->AddRect(ImVec2(r.min.x, ImGui::GetIO().DisplaySize.y-r.min.y), ImVec2(r.max.x, ImGui::GetIO().DisplaySize.y-r.max.y), mOutlineColor);

}


void ShapeDrawer::drawRectangle_withCamera(BoundingBoxFloat r)
{

    //VectorShape s(ShapeKind::RECTANGLE);
    //s.pathPoints().push_back(r.min);
    //s.pathPoints().push_back(Vec2f(r.min.x, r.max.y));
    //s.pathPoints().push_back(r.max);
    //s.pathPoints().push_back(Vec2f(r.max.x, r.min.y));
    //s._setClosed(true);
    //drawTransformedShape(&s, _m);


    assert(mCamera);

    const ncine::Matrix4x4f &cameraMatrix = mCamera->GetMatrix();

    Vec2f p1 = transform(r.min, cameraMatrix);
    Vec2f p2 = transform(Vec2f(r.min.x, r.max.y), cameraMatrix);
    Vec2f p3 = transform(r.max, cameraMatrix);
    Vec2f p4 = transform(Vec2f(r.max.x, r.min.y), cameraMatrix);

    mDrawList->AddLine(ImVec2(p1.x, ImGui::GetIO().DisplaySize.y-p1.y), ImVec2(p2.x, ImGui::GetIO().DisplaySize.y-p2.y), mOutlineColor);
    mDrawList->AddLine(ImVec2(p2.x, ImGui::GetIO().DisplaySize.y-p2.y), ImVec2(p3.x, ImGui::GetIO().DisplaySize.y-p3.y), mOutlineColor);
    mDrawList->AddLine(ImVec2(p3.x, ImGui::GetIO().DisplaySize.y-p3.y), ImVec2(p4.x, ImGui::GetIO().DisplaySize.y-p4.y), mOutlineColor);
    mDrawList->AddLine(ImVec2(p4.x, ImGui::GetIO().DisplaySize.y-p4.y), ImVec2(p1.x, ImGui::GetIO().DisplaySize.y-p1.y), mOutlineColor);

    //drawWithCameraTransform(&s);

    //if(mCamera){
        //const ncine::Matrix4x4f & cameraMatrix = mCamera->GetMatrix();

        //m = cameraMatrix * m;

    //    drawTransformedShape(&s, mCamera->GetMatrix(), {mCamera->GetScale(), mCamera->GetScale()});
    //}


}


void ShapeDrawer::drawRectFilled(jugimap::BoundingBoxFloat r)
{

    mDrawList->AddRectFilled(ImVec2(r.min.x, ImGui::GetIO().DisplaySize.y-r.min.y), ImVec2(r.max.x, ImGui::GetIO().DisplaySize.y-r.max.y), mOutlineColor);

}


void ShapeDrawer::drawDot(jugimap::Vec2f pos)
{

    mDrawList->AddCircleFilled(ImVec2(pos.x, ImGui::GetIO().DisplaySize.y-pos.y), mDotRadius, mOutlineColor, 6);

}


void ShapeDrawer::drawDot_withCamera(Vec2f pos)
{

    assert(mCamera);

    const ncine::Matrix4x4f &cameraMatrix = mCamera->GetMatrix();
    pos = transform(pos, cameraMatrix);
    mDrawList->AddCircleFilled(ImVec2(pos.x, ImGui::GetIO().DisplaySize.y-pos.y), mDotRadius, mOutlineColor, 6);

}


void ShapeDrawer::drawEllipse(Vec2f _center, Vec2f _radius)
{

    //c.y = ImGui::GetIO().DisplaySize.y - c.y;

    float ra = (std::fabs(_radius.x) + std::fabs(_radius.y)) / 2;
    float da = std::acos(ra / (ra + 0.125 /1.0)) * 2;
    int n = std::round(2*mathPI / da + 0.5);

    float pxPrev = _center.x + std::cos(0) * _radius.x;
    float pyPrev = _center.y + std::sin(0) * _radius.y;

    for(int i=1; i<=n; i++){
        float angle = i * 2*mathPI/n;
        float px = _center.x + std::cos( angle ) * _radius.x;
        float py = _center.y + std::sin( angle ) * _radius.y;
        drawLine(Vec2f(pxPrev,pyPrev), Vec2f(px,py));
        pxPrev = px;
        pyPrev = py;

    }
}


void ShapeDrawer::drawEllipse_withCamera(jugimap::Vec2f _center, jugimap::Vec2f _radius)
{

    assert(mCamera);

    const ncine::Matrix4x4f &cameraMatrix = mCamera->GetMatrix();
    _center = transform(_center, cameraMatrix);
    _radius *= mCamera->GetScale() * settings.appScale();

    drawEllipse(_center, _radius);

}


void ShapeDrawer::drawTexture(ncine::Texture* _texture, jugimap::Vec2f dstMin, jugimap::Vec2f dstMax, jugimap::Vec2f uvMin, jugimap::Vec2f uvMax)
{


    //mDrawList->AddImage(_texture->guiTexId(), ImVec2(dstMin.x, dstMin.y), ImVec2(dstMax.x, dstMax.y),
    //                    ImVec2(uvMin.x, uvMin.y), ImVec2(uvMax.x, uvMax.y), mOutlineColor);

    mDrawList->AddImage(_texture->guiTexId(), ImVec2(dstMin.x, ImGui::GetIO().DisplaySize.y - dstMax.y), ImVec2(dstMax.x, ImGui::GetIO().DisplaySize.y - dstMin.y),
                        ImVec2(uvMin.x, uvMin.y), ImVec2(uvMax.x, uvMax.y), mOutlineColor);

}


void ShapeDrawer::drawText(const std::string &_text, Vec2f pos)
{

    mDrawList->AddText(ImVec2(pos.x, ImGui::GetIO().DisplaySize.y-pos.y), mOutlineColor, _text.c_str());

}


void ShapeDrawer::drawTransformedShape(VectorShape *vs, const ncine::Matrix4x4f &_m, Vec2f _scale)
{

    Vec2f v, v1, v2;

    std::vector<PathPoint>&Points = vs->pathPoints();


    if(vs->kind()==ShapeKind::POLYLINE || vs->kind()==ShapeKind::RECTANGLE){

        //PolyLineShape *poly = static_cast<PolyLineShape*>(vs);
        for(int i=0; i<int(Points.size())-1; i++){
            v1.set(Points[i].x, Points[i].y);
            v2.set(Points[i+1].x, Points[i+1].y);

            v1 = transform(v1, _m);
            v2 = transform(v2, _m);

            drawLine( Vec2f(v1.x, v1.y), Vec2f(v2.x,v2.y) );

        }
        if(vs->isClosed()){
            v1.set(Points[Points.size()-1].x, Points[Points.size()-1].y);
            v2.set(Points[0].x, Points[0].y);

            v1 = transform(v1, _m);
            v2 = transform(v2, _m);

            drawLine( Vec2f(v1.x, v1.y), Vec2f(v2.x,v2.y) );
        }

    }else if(vs->kind()==ShapeKind::BEZIER_POLYCURVE){

        //BezierShape *bezpoly = static_cast<BezierShape*>(vs);
        for(int i=0; i<int(vs->pathPoints().size())-1; i++){
            v1.set(Points[i].x, Points[i].y);
            v2.set(Points[i+1].x, Points[i+1].y);

            v1 = transform(v1, _m);
            v2 = transform(v2, _m);

            drawLine( Vec2f(v1.x, v1.y), Vec2f(v2.x,v2.y) );
        }

        /*
        if(bezpoly->IsClosed()){
            v1 = bezpoly->polylineVertices[bezpoly->polylineVertices.size()-1];
            v2 = bezpoly->polylineVertices[0];

            v1 = m.Transform(v1);
            v2 = m.Transform(v2);

            drawer->Line(v1,v2);
        }
        */


    }else if(vs->kind()==ShapeKind::ELLIPSE){

        //EllipseShape *e = static_cast<EllipseShape*>(vs);
        //v.set(e->center.x, e->center.y);
        v = vs->center();

        v = transform(v, _m);

        drawEllipse(Vec2f(v.x, v.y), {_scale.x * vs->a(), _scale.y * vs->b()});


    }else if(vs->kind()==ShapeKind::SINGLE_POINT){

        //SinglePointShape *p = static_cast<SinglePointShape*>(vs);
        //v = p->point;
        //v.set(p->get().x, p->get().y);
        v = vs->singlePointF();

        v = transform(v, _m);
        drawDot(Vec2f(v.x, v.y));

    }

}





//====================================================================================================



void DrawVectorShapes(jugimap::ShapeDrawer& drawer, Map *map)
{

    //for(ncine::SceneNode *n : map->nodeChildren()){
    //    if(n->type()==ncine::Object::ObjectType::SCENENODE){
    //        jugimap::Layer* layer = static_cast<LayerNode*>(n)->baseLayer;
    for(Layer *l : map->layers()){

        if(l->kind()==LayerKind::VECTOR_LAYER){
            jugimap::VectorLayer* vl = static_cast<VectorLayer*>(l);

            for(VectorShape *vs : vl->shapes()){
                drawer.drawShape_withCamera(vs);
            }
        }
    }
}


void DrawSpriteCollisionShapes(jugimap::ShapeDrawer& drawer, ncine::SceneNode* node)
{


    static jugimap::ColorRGBA colorDrawMouseOverSpriteShape(200,0,0,255);

    Camera *camera = drawer.camera();

    for(ncine::SceneNode *n : node->children()){
        if(n->isEnabled()==false) continue;

        if(n->type()==ncine::Object::ObjectType::SCENENODE){
            //if(dynamic_cast<ComposedSpriteNode*>(n)){
            //    EComposedSprite* cs = static_cast<ComposedSpriteNode*>(n)->s;
            //    DummyFunction();
            //}
            if(dynamic_cast<LayerNode*>(n)){
                CompositeSprite* cs = static_cast<LayerNode*>(n)->compositeSprite;
                DummyFunction();
            }

            DrawSpriteCollisionShapes(drawer, n);

        }else if(n->type()==ncine::Object::ObjectType::SPRITE){
            jugimap::StandardSprite* s = static_cast<jugimap::StandardSpriteNode*>(n)->s;
            assert(s);

            //---- check all situations where drawing shapes is not needed
            if(s->isVisible()==false) continue;
            if(s->activeImage()==nullptr)  continue;
            if(camera->GetMapVisibleRect().Intersects(s->boundingBox())==false){
                continue;
            }

            std::vector<VectorShape*>allShapes = s->activeImage()->spriteShapes();
            for(VectorShape * vs : s->activeImage()->extraShapes()){
                allShapes.push_back(vs);
            }

            //----
            bool mouseOver = false;
            if(s->collider() && s->pointInside(camera->MapPointFromScreenPoint(Vec2iToVec2f(mouse.pos())))){
                mouseOver = true;
                print("mouseOver sprite: " + s->sourceSprite()->name());
            }

            //--- draw bounding boxes
            ColorRGBA storedColor = drawer.color();
            drawer.setOutlineColor({0,128,0,255});
            drawer.drawRectangle_withCamera(s->boundingBox());
            drawer.setOutlineColor(storedColor);
            if(mouseOver && allShapes.empty()){
                ColorRGBA storedColor = drawer.color();
                drawer.setOutlineColor(colorDrawMouseOverSpriteShape);
                drawer.drawRectangle_withCamera(s->boundingBox());
                drawer.setOutlineColor(storedColor);
            }

            if(allShapes.empty()) continue;


            //--- transform vector shapes to world positions of their sprites
            ncine::Matrix4x4f m = s->node()->worldMatrix();
            Vec2f scale = {s->node()->absScale().x, s->node()->absScale().y};

            for(VectorShape *vs : allShapes){
                if(mouseOver){
                    ColorRGBA storedColor = drawer.color();
                    drawer.setOutlineColor(colorDrawMouseOverSpriteShape);
                    drawer.drawShape_withCamera(vs, m, scale);
                    drawer.setOutlineColor(storedColor);

                }else{

                    drawer.drawShape_withCamera(vs, m, scale);
                }
            }
        }
    }
}



}



