#ifndef PLAYER__ENTITIES__BOX2D_DEBUG_DRAW__H
#define PLAYER__ENTITIES__BOX2D_DEBUG_DRAW__H

#include <memory>
#include <string>
#include <vector>
#include <ncine/imgui.h>
#include <ncine/Matrix4x4.h>
#include "box2d.h"



namespace jugimap{



class DebugDraw : public b2Draw
{
public:

    void Begin();

    void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;

    void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;

    void DrawCircle(const b2Vec2& center, float radius, const b2Color& color) override;

    void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) override;

    void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;

    void DrawTransform(const b2Transform& xf) override;

    void DrawPoint(const b2Vec2& p, float size, const b2Color& color) override;

    void DrawString(int x, int y, const char* string, ...);

    void DrawString(const b2Vec2& p, const char* string, ...);

    void DrawAABB(b2AABB* aabb, const b2Color& color);

    void Flush();

    bool m_showUI = false;
    //GLRenderPoints* m_points;
    //GLRenderLines* m_lines;
    //GLRenderTriangles* m_triangles;

    inline b2Vec2 transformPoint(b2Vec2 v, const ncine::Matrix4x4f &m)
    {
        return b2Vec2(m[0][0] * v.x + m[1][0] * v.y + m[3][0],
                        m[0][1] * v.x + m[1][1] * v.y + m[3][1]);
    }

    inline void drawLine(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
    {

        float height = ImGui::GetIO().DisplaySize.y;
        mDrawList->AddLine(ImVec2(p1.x, ImGui::GetIO().DisplaySize.y-p1.y), ImVec2(p2.x, ImGui::GetIO().DisplaySize.y-p2.y), ImColor(color.r, color.g, color.b, color.a));

    }


    ncine::Matrix4x4f mMCamera;
    float mCameraScale = 1.0;
    ImDrawList* mDrawList = nullptr;
    ImColor mOutlineColor = ImColor(0, 255, 0, 255);
    float mDotRadius = 2;
    std::vector<ImVec2>mVertices;
    //void drawDot(ncine::Vector2f pos);
};




}






#endif
