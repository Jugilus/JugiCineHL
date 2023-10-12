#include <chrono>
#include <assert.h>
#include <sstream>
#include "ncine/Application.h"
#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmMap.h"
#include "jmCamera.h"
#include "jmSceneLayout.h"
#include "jmCommonFunctions.h"
#include "jmSourceContainers.h"
#include "jmVectorShape.h"
#include "jmTextSprite.h"
#include "jmUtilities.h"

#include "items/jpItemsCommon.h"
#include "jpPlayedApp.h"
#include "jpPlayedScene.h"
#include "jpItemsTable.h"
#include "jpUtilities.h"
#include "jpActionsCommon.h"

#include "scripting/jpBehavior.h"
#include "jpB2World.h"
#include "jpB2DebugDraw.h"



namespace jugimap{



void DebugDraw::Begin()
{

    //ncine::Application &app = ncine::theApplication();

    ncine::Application &ncApp = ncine::theApplication();

    float width = app->designResolution().width;
    float height = app->designResolution().height;

    width = ncApp.width();
    height = ncApp.height();

    //ImGui::SetNextWindowPos(ImVec2(-4, -1));        // moram zamakniti ker drugače ne riše ob robu - ???
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(width, height));
    ImGui::SetNextWindowContentSize(ImVec2(width, height));


    ImGui::Begin("My shapes", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs);
    //ImGui::Begin("My shapes", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar);

    mDrawList = ImGui::GetWindowDrawList();

}


//
void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{

    b2Vec2 p1 = vertices[vertexCount - 1];
    p1 = gWorldInfo.metersToPixels<b2Vec2>(p1);
    p1 = transformPoint(p1, mMCamera);

    for (int32 i = 0; i < vertexCount; ++i)
    {
        b2Vec2 p2 = vertices[i];
        p2 = gWorldInfo.metersToPixels<b2Vec2>(p2);
        p2 = transformPoint(p2, mMCamera);

        drawLine(p1, p2, color);
        p1 = p2;
    }
}


//
void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{

    b2Color fillColor(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);

    if(mVertices.size() < vertexCount){
        mVertices.resize(vertexCount+1);
    }
    for(int i=0; i<vertexCount; i++){
        b2Vec2 p = vertices[i];
        p = gWorldInfo.metersToPixels<b2Vec2>(p);
        p = transformPoint(p, mMCamera);

        mVertices[i].x = p.x;
        mVertices[i].y = ImGui::GetIO().DisplaySize.y - p.y;
    }

    for (int32 i = 1; i < vertexCount - 1; ++i){
        mDrawList->AddTriangleFilled(mVertices[0], mVertices[i], mVertices[i+1], ImColor(fillColor.r, fillColor.g, fillColor.b, fillColor.a));
    }

    //---
    ImVec2 p1 = mVertices[vertexCount - 1];
    for (int32 i = 0; i < vertexCount; ++i){
        ImVec2 p2 = mVertices[i];
        mDrawList->AddLine(p1,p2, ImColor(color.r, color.g, color.b, color.a));
        p1 = p2;
    }

    //----
    //mDrawList->AddPolyline(mVertices.data(), vertexCount, ImColor(color.r, color.g, color.b, color.a), ImDrawFlags_None, 1.0);
}


//
void DebugDraw::DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
{
    b2Vec2 c = gWorldInfo.metersToPixels<b2Vec2>(center);
    c = transformPoint(c, mMCamera);
    float r = gWorldInfo.metersToPixels(radius);
    r *= mCameraScale;

    mDrawList->AddCircle(ImVec2(c.x, ImGui::GetIO().DisplaySize.y-c.y), r, ImColor(color.r, color.g, color.b, color.a));
}

//
void DebugDraw::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color)
{

    b2Vec2 c = gWorldInfo.metersToPixels<b2Vec2>(center);
    c = transformPoint(c, mMCamera);
    float r = gWorldInfo.metersToPixels(radius);
    r *= mCameraScale;
    b2Vec2 _axis = gWorldInfo.metersToPixels<b2Vec2>(axis);
    _axis = transformPoint(_axis, mMCamera);

    mDrawList->AddCircle(ImVec2(c.x, ImGui::GetIO().DisplaySize.y-c.y), r, ImColor(color.r, color.g, color.b, color.a));

    b2Color fillColor(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);
    mDrawList->AddCircleFilled(ImVec2(c.x, ImGui::GetIO().DisplaySize.y-c.y), r, ImColor(fillColor.r, fillColor.g, fillColor.b, fillColor.a));

    // Draw a line fixed in the circle to animate rotation.
    b2Vec2 p = c + r * axis;
    drawLine(c, p, color);
}

//
void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{

    b2Vec2 _p1 = gWorldInfo.metersToPixels<b2Vec2>(p1);
    _p1 = transformPoint(_p1, mMCamera);
    b2Vec2 _p2 = gWorldInfo.metersToPixels<b2Vec2>(p2);
    _p2 = transformPoint(_p2, mMCamera);

    drawLine(_p1,_p2, color);
}


//
void DebugDraw::DrawTransform(const b2Transform& xf)
{

    const float k_axisScale = 0.4f;
    b2Color red(1.0f, 0.0f, 0.0f);
    b2Color green(0.0f, 1.0f, 0.0f);
    b2Vec2 p1 = xf.p;
    b2Vec2 p2 = p1 + k_axisScale * xf.q.GetXAxis();

    DrawSegment(p1,p2,red);

    p2 = p1 + k_axisScale * xf.q.GetYAxis();

    DrawSegment(p1,p2,green);

}

//
void DebugDraw::DrawPoint(const b2Vec2& p, float size, const b2Color& color)
{

    b2Vec2 c = gWorldInfo.metersToPixels<b2Vec2>(p);
    c = transformPoint(c, mMCamera);

    mDrawList->AddCircleFilled(ImVec2(c.x, ImGui::GetIO().DisplaySize.y-c.y), size, ImColor(color.r, color.g, color.b, color.a));

}


//
void DebugDraw::DrawString(int x, int y, const char* string, ...)
{

    if (m_showUI == false)
    {
        return;
    }

    va_list arg;
    va_start(arg, string);
    ImGui::Begin("Overlay", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar);
    ImGui::SetCursorPos(ImVec2(float(x), float(y)));
    ImGui::TextColoredV(ImColor(230, 153, 153, 255), string, arg);
    ImGui::End();
    va_end(arg);

}

//
void DebugDraw::DrawString(const b2Vec2& pw, const char* string, ...)
{

    //b2Vec2 ps = g_camera.ConvertWorldToScreen(pw);

    b2Vec2 ps = transformPoint(pw, mMCamera);

    va_list arg;
    va_start(arg, string);
    ImGui::Begin("Overlay", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar);
    ImGui::SetCursorPos(ImVec2(ps.x, ps.y));
    ImGui::TextColoredV(ImColor(230, 153, 153, 255), string, arg);
    ImGui::End();
    va_end(arg);
}

//
void DebugDraw::DrawAABB(b2AABB* aabb, const b2Color& c)
{

    b2Vec2 p1 = aabb->lowerBound;
    p1 = gWorldInfo.metersToPixels<b2Vec2>(p1);
    p1 = transformPoint(p1, mMCamera);
    b2Vec2 p2 = b2Vec2(aabb->upperBound.x, aabb->lowerBound.y);
    p2 = gWorldInfo.metersToPixels<b2Vec2>(p2);
    p2 = transformPoint(p2, mMCamera);
    b2Vec2 p3 = aabb->upperBound;
    p3 = gWorldInfo.metersToPixels<b2Vec2>(p3);
    p3 = transformPoint(p3, mMCamera);
    b2Vec2 p4 = b2Vec2(aabb->lowerBound.x, aabb->upperBound.y);
    p4 = gWorldInfo.metersToPixels<b2Vec2>(p4);
    p4 = transformPoint(p4, mMCamera);

    drawLine(p1,p2, c);
    drawLine(p2,p3, c);
    drawLine(p3,p4, c);
    drawLine(p4,p1, c);
}


//
void DebugDraw::Flush()
{

    ImGui::End();

}


}
