#include <assert.h>
#include <ncine/FileSystem.h>
#include <ncine/Texture.h>
#include "box2d/box2d.h"

#include "jmVectorShape.h"
#include "jmSprite.h"
#include "jmStandardSprite.h"
#include "jmSourceItem.h"
#include "jmCommonFunctions.h"

#include "jpEntityCommon.h"
#include "jpB2Body.h"
#include "jpB2Fixture.h"
#include "jpB2World.h"
#include "jpB2Utilities.h"




namespace jugimap{








b2Vec2 GetUnitNormal(b2Vec2 p1, b2Vec2 p2)
{

    float dist = DistanceTwoPoints(p1, p2);

    return b2Vec2((p2.y-p1.y)/dist, -(p2.x-p1.x)/dist);

}

/*
TerrainSurfaceDirection GetTerrainSurfaceDirection(b2Vec2 normal)
{

    float angle = 180 * std::atan2f(normal.y, normal.x)/b2_pi;
    if(angle<0.0f) angle += 360.0f;

    if(angle>44.0f && angle <136.0f){
        return TerrainSurfaceDirection::GROUND;

    }else if (angle>136.0f && angle <224.0f){
        return TerrainSurfaceDirection::RIGHT_WALL;

    }else if (angle>224.0f && angle <314.0f){
        return TerrainSurfaceDirection::CEILING;
    }

    return TerrainSurfaceDirection::LEFT_WALL;

}
*/




/*
b2Fixture* createB2Shape(VectorShape *_vs, b2FixtureDef &_fixtureDef, b2Body *_body, ncine::Vector2f _pos)
{

    FixtureUserData *fud = reinterpret_cast<FixtureUserData*>(_fixtureDef.userData.pointer);


    static std::vector<b2Vec2>vertices;

    if(_vs->kind()==ShapeKind::POLYLINE || _vs->kind()==ShapeKind::RECTANGLE){

        std::vector<PathPoint>& mPoints = _vs->pathPoints();

        if(_vs->isClosed()){

            b2PolygonShape shape;
            //b2Vec2 *vertices = new b2Vec2[mPoints.size()];
            if(vertices.size()<mPoints.size()){
                vertices.resize(mPoints.size()+1);
            }

            for(unsigned int i=0; i<mPoints.size(); i++){
                vertices[i].Set(gWorldInfo.pixelsToMeters(_pos.x + mPoints[i].x), gWorldInfo.pixelsToMeters(_pos.y + mPoints[i].y));
                if(i>0){
                    b2Vec2 norm = GetUnitNormal(vertices[i-1],vertices[i]);
                    DirectionAndAngle dir = GetBlockedDirectionFromSurfaceNormal(norm);
                    fud->normals.push_back(std::pair<b2Vec2,Direction>(norm, dir.mDirection));
                    //fud->surfaceDirection |= static_cast<int>(GetTerrainSurfaceDirection(norm));
                    fud->blockedDir |= static_cast<int>(dir.mDirection);
                }
            }
            shape.Set(vertices.data(), mPoints.size());
            _fixtureDef.shape = &shape;
            b2Fixture *f = _body->CreateFixture(&_fixtureDef);
            return f;

        }else{

            if(mPoints.size()==2){
                b2EdgeShape shape;
                b2Vec2 v1(gWorldInfo.pixelsToMeters(_pos.x + mPoints[0].x), gWorldInfo.pixelsToMeters(_pos.y + mPoints[0].y));
                b2Vec2 v2(gWorldInfo.pixelsToMeters(_pos.x + mPoints[1].x), gWorldInfo.pixelsToMeters(_pos.y + mPoints[1].y));
                b2Vec2 norm = GetUnitNormal(v1,v2);
                //TerrainSurfaceDirection tsd = GetTerrainSurfaceDirection(norm);
                //fud->normals.push_back(std::pair<b2Vec2,TerrainSurfaceDirection>(norm, tsd));
                //fud->surfaceDirection |= static_cast<int>(GetTerrainSurfaceDirection(norm));
                DirectionAndAngle dir = GetBlockedDirectionFromSurfaceNormal(norm);
                fud->normals.push_back(std::pair<b2Vec2,Direction>(norm, dir.mDirection));
                fud->blockedDir |= static_cast<int>(dir.mDirection);

                b2Vec2 v0(v1.x -1.0f*(v2.x-v1.x), v1.y -1.0f*(v2.y-v1.y));      // ghost point before p1
                b2Vec2 v3(v1.x +2.0f*(v2.x-v1.x), v1.y +2.0f*(v2.y-v1.y));      // ghost point after p2

                //if(dir.mAngle>50 && dir.mAngle<90){
                //    dummyFunction();
                //    v3.y = v2.y;
                //}

                shape.SetOneSided(v0,v1,v2,v3);
                _fixtureDef.shape = &shape;
                b2Fixture *f = _body->CreateFixture(&_fixtureDef);
                return f;

            }else if(mPoints.size()>2){
                b2ChainShape shape;
                if(vertices.size()<mPoints.size()){
                    vertices.resize(mPoints.size()+1);
                }
                for(unsigned int i=0; i<mPoints.size(); i++){
                    vertices[i].Set(gWorldInfo.pixelsToMeters(_pos.x + mPoints[i].x), gWorldInfo.pixelsToMeters(_pos.y + mPoints[i].y));
                    if(i>0){
                        b2Vec2 norm = GetUnitNormal(vertices[i-1],vertices[i]);
                        //TerrainSurfaceDirection tsd = GetTerrainSurfaceDirection(norm);
                        //fud->normals.push_back(std::pair<b2Vec2,TerrainSurfaceDirection>(norm, tsd));
                        //fud->surfaceDirection |= static_cast<int>(GetTerrainSurfaceDirection(norm));
                        DirectionAndAngle dir = GetBlockedDirectionFromSurfaceNormal(norm);
                        fud->normals.push_back(std::pair<b2Vec2,Direction>(norm, dir.mDirection));
                        fud->blockedDir |= static_cast<int>(dir.mDirection);
                    }
                }
                b2Vec2 vPrev(vertices[0].x -1.0f*(vertices[1].x-vertices[0].x), vertices[0].y -1.0f*(vertices[1].y-vertices[0].y));      // ghost point before frist point
                b2Vec2 vNext(vertices[mPoints.size()-2].x +2.0f*(vertices[mPoints.size()-1].x-vertices[mPoints.size()-2].x),
                            vertices[mPoints.size()-2].y +2.0f*(vertices[mPoints.size()-1].y-vertices[mPoints.size()-2].y));            // ghost point after last point

                shape.CreateChain(vertices.data(), mPoints.size(), vPrev, vNext);
                _fixtureDef.shape = &shape;
                b2Fixture *f = _body->CreateFixture(&_fixtureDef);
                return f;
            }
        }


    }else if(_vs->kind()==ShapeKind::ELLIPSE){

        b2CircleShape shape;
        shape.m_p.Set(gWorldInfo.pixelsToMeters(_vs->center().x), gWorldInfo.pixelsToMeters(_vs->center().y));
        shape.m_radius = gWorldInfo.pixelsToMeters(_vs->a());
        _fixtureDef.shape = &shape;
        b2Fixture *f = _body->CreateFixture(&_fixtureDef);
        return f;
    }

    return nullptr;

}
*/


/*
void createB2ShapeForTileSprite(FixtureSprite *fSprite, Vec2f _bodyOrigin, TileGrid &tileGrid, b2Body *_body, FixtureRole _fixtureRole, bool _isSensor)
{

    //FixtureUserData *fud = reinterpret_cast<FixtureUserData*>(_fixtureDef.userData.pointer);



    static std::vector<b2Vec2>vertices;

    StandardSprite *sprite = fSprite->sprite;
    Vec2f spritePos = sprite->position();
    spritePos -= _bodyOrigin;


    for(VectorShape *vs : sprite->activeImage()->spriteShapes()){

        if(vs->kind()==ShapeKind::POLYLINE || vs->kind()==ShapeKind::RECTANGLE){

            b2FixtureDef fixtureDef;
            fixtureDef.friction = 1.0;
            fixtureDef.isSensor = _isSensor;
            fSprite->fixtureUserData.sourceSprite = sprite->sourceSprite();
            fSprite->fixtureUserData.id = FixtureUserData::idCounter ++ ;
            //sprite->mFixtureUserData.role = FixtureRole::BLOCKING_ENVIRONMENT;
            fSprite->fixtureUserData.role = _fixtureRole;
            fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(&fSprite->fixtureUserData);

            FixtureUserData *fud = reinterpret_cast<FixtureUserData*>(fixtureDef.userData.pointer);

            const std::vector<ShapePoint>& mPoints = vs->shapePoints();

            if(vs->isClosed()){

                b2PolygonShape shape;
                //b2Vec2 *vertices = new b2Vec2[mPoints.size()];
                if(vertices.size()<mPoints.size()){
                    vertices.resize(mPoints.size()+1);
                }

                for(unsigned int i=0; i<mPoints.size(); i++){
                    vertices[i].Set(gWorldInfo.pixelsToMeters(spritePos.x + mPoints[i].x), gWorldInfo.pixelsToMeters(spritePos.y + mPoints[i].y));
                    if(i>0){
                        b2Vec2 norm = GetUnitNormal(vertices[i-1],vertices[i]);
                        DirectionAndAngle dir = GetBlockedDirectionFromSurfaceNormal(norm);
                        fud->normals.push_back(std::pair<b2Vec2,Direction>(norm, dir.mDirection));
                        //fud->surfaceDirection |= static_cast<int>(GetTerrainSurfaceDirection(norm));
                        fud->blockedDir |= static_cast<int>(dir.mDirection);
                    }
                }
                shape.Set(vertices.data(), mPoints.size());
                fixtureDef.shape = &shape;
                b2Fixture *f = _body->CreateFixture(&fixtureDef);
                //return f;

            }else{

                if(mPoints.size()==2){
                    b2EdgeShape shape;
                    b2Vec2 v1(gWorldInfo.pixelsToMeters(spritePos.x + mPoints[0].x), gWorldInfo.pixelsToMeters(spritePos.y + mPoints[0].y));
                    b2Vec2 v2(gWorldInfo.pixelsToMeters(spritePos.x + mPoints[1].x), gWorldInfo.pixelsToMeters(spritePos.y + mPoints[1].y));
                    b2Vec2 norm = GetUnitNormal(v1,v2);
                    //TerrainSurfaceDirection tsd = GetTerrainSurfaceDirection(norm);
                    //fud->normals.push_back(std::pair<b2Vec2,TerrainSurfaceDirection>(norm, tsd));
                    //fud->surfaceDirection |= static_cast<int>(GetTerrainSurfaceDirection(norm));
                    DirectionAndAngle dir = GetBlockedDirectionFromSurfaceNormal(norm);
                    fud->normals.push_back(std::pair<b2Vec2,Direction>(norm, dir.mDirection));
                    fud->blockedDir |= static_cast<int>(dir.mDirection);

                    // ghost point before p1
                    b2Vec2 v0;

                    if(spritePos.x==175.0f && spritePos.y==325.0f){
                        DummyFunction();
                    }

                    GhostVertexData gvdPrev = FindGhostVertex(mPoints[0].asVec2f(), spritePos, tileGrid, true);
                    if(gvdPrev.found){
                        v0 = gWorldInfo.pixelsToMeters(gvdPrev.sprite->position() + gvdPrev.ghostVertex);
                    }else{
                        v0 = {v1.x -1.0f*(v2.x-v1.x), v1.y -1.0f*(v2.y-v1.y)};      // extrapolate
                    }

                    // ghost point after p2
                    b2Vec2 v3;

                    GhostVertexData gvdNext = FindGhostVertex(mPoints[1].asVec2f(), spritePos, tileGrid, false);
                    if(gvdNext.found){
                        v3 = gWorldInfo.pixelsToMeters(gvdNext.sprite->position() + gvdNext.ghostVertex);
                    }else{
                        v3 = {v1.x +2.0f*(v2.x-v1.x), v1.y +2.0f*(v2.y-v1.y)};       // extrapolate
                    }


                    //if(dir.mAngle>50 && dir.mAngle<90){
                    //    dummyFunction();
                    //    v3.y = v2.y;
                    //}

                    shape.SetOneSided(v0,v1,v2,v3);
                    fixtureDef.shape = &shape;
                    b2Fixture *f = _body->CreateFixture(&fixtureDef);
                    //return f;

                }else if(mPoints.size()>2){
                    b2ChainShape shape;
                    if(vertices.size()<mPoints.size()){
                        vertices.resize(mPoints.size()+1);
                    }
                    for(unsigned int i=0; i<mPoints.size(); i++){
                        vertices[i].Set(gWorldInfo.pixelsToMeters(spritePos.x + mPoints[i].x), gWorldInfo.pixelsToMeters(spritePos.y + mPoints[i].y));
                        if(i>0){
                            b2Vec2 norm = GetUnitNormal(vertices[i-1],vertices[i]);
                            DirectionAndAngle dir = GetBlockedDirectionFromSurfaceNormal(norm);
                            fud->normals.push_back(std::pair<b2Vec2,Direction>(norm, dir.mDirection));
                            fud->blockedDir |= static_cast<int>(dir.mDirection);
                        }
                    }


                    // ghost point before the first point
                    b2Vec2 vPrev;
                    //Vec2f pPrev;

                    GhostVertexData gvdPrev = FindGhostVertex(mPoints[0].asVec2f(), spritePos, tileGrid, true);
                    if(gvdPrev.found){
                        vPrev = gWorldInfo.pixelsToMeters(gvdPrev.sprite->position() + gvdPrev.ghostVertex);
                    }else{
                        vPrev = {vertices[0].x -1.0f*(vertices[1].x-vertices[0].x), vertices[0].y -1.0f*(vertices[1].y-vertices[0].y)};      // extrapolate
                    }

                    // ghost point after the last point
                    b2Vec2 vNext;

                    GhostVertexData gvdNext = FindGhostVertex(mPoints[mPoints.size()-1].asVec2f(), spritePos, tileGrid, false);
                    if(gvdNext.found){
                        vNext = gWorldInfo.pixelsToMeters(gvdNext.sprite->position() + gvdNext.ghostVertex);
                    }else{
                        vNext = {vertices[mPoints.size()-2].x +2.0f*(vertices[mPoints.size()-1].x-vertices[mPoints.size()-2].x),
                                 vertices[mPoints.size()-2].y +2.0f*(vertices[mPoints.size()-1].y-vertices[mPoints.size()-2].y)};       // extrapolate
                    }


                    //b2Vec2 vPrev(vertices[0].x -1.0f*(vertices[1].x-vertices[0].x), vertices[0].y -1.0f*(vertices[1].y-vertices[0].y));      // ghost point before frist point
                    //b2Vec2 vNext(vertices[mPoints.size()-2].x +2.0f*(vertices[mPoints.size()-1].x-vertices[mPoints.size()-2].x),
                    //            vertices[mPoints.size()-2].y +2.0f*(vertices[mPoints.size()-1].y-vertices[mPoints.size()-2].y));            // ghost point after last point

                    shape.CreateChain(vertices.data(), mPoints.size(), vPrev, vNext);
                    fixtureDef.shape = &shape;
                    b2Fixture *f = _body->CreateFixture(&fixtureDef);
                    //return f;
                }
            }
        }
    }

}
*/



void createB2ShapeVerticesForTileSprite(SourceFixture &sf, Vec2f spriteRelPos, TileGrid *tileGrid)
{


    //Vec2f spritePos = _spr->position();
    //spritePos -= _bodyOrigin;                // ??????????????????????????????????


    VectorShape *vs = sf.fud->vectorShape;


    if(vs->kind()==ShapeKind::POLYLINE || vs->kind()==ShapeKind::RECTANGLE){

        const std::vector<ShapePoint>& mPoints = vs->shapePoints();

        if(vs->isClosed() && sf.createChainLoop==false){

            sf.mShapeType = b2Shape::Type::e_polygon;
            sf.mVertices.resize(mPoints.size());

            for(unsigned int i=0; i<mPoints.size(); i++){
                sf.mVertices[i].Set(gWorldInfo.pixelsToMeters(spriteRelPos.x + mPoints[i].x), gWorldInfo.pixelsToMeters(spriteRelPos.y + mPoints[i].y));
            }

        }else{

            if(mPoints.size()==2){

                sf.mShapeType = b2Shape::Type::e_edge;
                sf.mVertices.resize(2);

                b2Vec2 v1(gWorldInfo.pixelsToMeters(spriteRelPos.x + mPoints[0].x), gWorldInfo.pixelsToMeters(spriteRelPos.y + mPoints[0].y));
                b2Vec2 v2(gWorldInfo.pixelsToMeters(spriteRelPos.x + mPoints[1].x), gWorldInfo.pixelsToMeters(spriteRelPos.y + mPoints[1].y));

                if(spriteRelPos.x==175.0f && spriteRelPos.y==325.0f){
                    DummyFunction();
                }

                // ghost points before v1 and after v2
                b2Vec2 v0 {v1.x -1.0f*(v2.x-v1.x), v1.y -1.0f*(v2.y-v1.y)};       // extrapolate is 'just in case' method and can cause problems
                b2Vec2 v3 {v1.x +2.0f*(v2.x-v1.x), v1.y +2.0f*(v2.y-v1.y)};       // extrapolate is 'just in case' method and can cause problems


                // for tiles a tileGrid is used to obtain correct ghost vertices
                if(tileGrid){
                    GhostVertexData gvdPrev = FindGhostVertex(mPoints[0].asVec2f(), spriteRelPos, *tileGrid, true);
                    if(gvdPrev.found){
                        v0 = gWorldInfo.pixelsToMeters(gvdPrev.sprite->position() + gvdPrev.ghostVertex);
                    }else{
                        DummyFunction();
                    }

                    GhostVertexData gvdNext = FindGhostVertex(mPoints[1].asVec2f(), spriteRelPos, *tileGrid, false);
                    if(gvdNext.found){
                        v3 = gWorldInfo.pixelsToMeters(gvdNext.sprite->position() + gvdNext.ghostVertex);
                    }else{
                        DummyFunction();
                    }
                }

                sf.mVertices[0] = v1;
                sf.mVertices[1] = v2;
                sf.mPrevVertex = v0;
                sf.mNextVertex = v3;

                //if(dir.mAngle>50 && dir.mAngle<90){
                //    dummyFunction();
                //    v3.y = v2.y;
                //}

            }else if(mPoints.size()>2){

                sf.mShapeType = b2Shape::Type::e_chain;
                sf.mVertices.resize(mPoints.size());

                for(unsigned int i=0; i<mPoints.size(); i++){
                    sf.mVertices[i].Set(gWorldInfo.pixelsToMeters(spriteRelPos.x + mPoints[i].x), gWorldInfo.pixelsToMeters(spriteRelPos.y + mPoints[i].y));
                }

                if(vs->isClosed() && sf.createChainLoop){
                   //ghost vertices are not needed
                   return;
                }


                // ghost vertices
                b2Vec2 vPrev {sf.mVertices[0].x -1.0f*(sf.mVertices[1].x-sf.mVertices[0].x), sf.mVertices[0].y -1.0f*(sf.mVertices[1].y-sf.mVertices[0].y)};      // extrapolate
                b2Vec2 vNext {sf.mVertices[mPoints.size()-2].x +2.0f*(sf.mVertices[mPoints.size()-1].x-sf.mVertices[mPoints.size()-2].x),
                             sf.mVertices[mPoints.size()-2].y +2.0f*(sf.mVertices[mPoints.size()-1].y-sf.mVertices[mPoints.size()-2].y)};       // extrapolate


                if(tileGrid){
                    GhostVertexData gvdPrev = FindGhostVertex(mPoints[0].asVec2f(), spriteRelPos, *tileGrid, true);
                    if(gvdPrev.found){
                        vPrev = gWorldInfo.pixelsToMeters(gvdPrev.sprite->position() + gvdPrev.ghostVertex);
                    }else{
                        DummyFunction();
                    }

                    GhostVertexData gvdNext = FindGhostVertex(mPoints[mPoints.size()-1].asVec2f(), spriteRelPos, *tileGrid, false);
                    if(gvdNext.found){
                        vNext = gWorldInfo.pixelsToMeters(gvdNext.sprite->position() + gvdNext.ghostVertex);
                    }else{
                        DummyFunction();
                    }
                }

                sf.mPrevVertex = vPrev;
                sf.mNextVertex = vNext;
            }
        }
    }


}





GhostVertexData FindGhostVertexInSprite(StandardSprite * sprite, Vec2f p, bool frontGhost)
{

    GhostVertexData gvd;

    if(sprite->type()!= SpriteType::STANDARD){
        return gvd;
    }

    SourceImage *si = sprite->sourceSprite()->sourceImages().front();

    //for(VectorShape *vs : sprite->activeImage()->spriteShapes()){
    for(VectorShape *vs : si->spriteShapes()){
        if(vs->kind()==ShapeKind::POLYLINE && vs->isClosed()==false){

            std::vector<ShapePoint> & points = vs->shapePoints();
            if(points.size()<2) continue;                           // safety, should not be possible

            if(frontGhost){
                // front ghost of an edge shape is located at the end of a touching neighbour shape
                if(p.x==points.back().x && p.y==points.back().y){
                    gvd.sprite = sprite;
                    gvd.ghostVertex = points[points.size()-2].asVec2f();
                    gvd.found = true;
                    break;
                }

            }else{
                // back ghost of an edge shape is located at the front of a touching neighbour shape
                if(p.x==points.front().x && p.y==points.front().y){
                    gvd.sprite = sprite;
                    gvd.ghostVertex = points[1].asVec2f();
                    gvd.found = true;
                     break;
                }
            }
        }
    }

    return gvd;
}



GhostVertexData FindGhostVertex(Vec2f p, Vec2f spritePos, const TileGrid &tileGrid, bool frontGhost)
{

    // shape point p is in local coordinates relative to the center of the tile
    // we check for ghost vertex in surrounding tiles if the point is in the border of the tile

    Vec2f tileSize(tileGrid.GetTileSize().x, tileGrid.GetTileSize().y);
    float wHalf = tileSize.x / 2.0f;
    float hHalf = tileSize.y / 2.0f;



    if(p.x == wHalf){

        // right border

        if(p.y == hHalf){
            // check top right tile
            StandardSprite * s = tileGrid.GetSprite({spritePos.x + tileSize.x, spritePos.y + tileSize.y});
            if(s){
                Vec2f pTouching{-p.x, -p.y};
                GhostVertexData gvd = FindGhostVertexInSprite(s, pTouching, frontGhost);
                if(gvd.found){
                    return gvd;
                }
            }

        }else if(p.y == -hHalf){
            // check bottom right tile
            StandardSprite * s = tileGrid.GetSprite({spritePos.x + tileSize.x, spritePos.y - tileSize.y});
            if(s){
                Vec2f pTouching{-p.x, -p.y};
                GhostVertexData gvd = FindGhostVertexInSprite(s, pTouching, frontGhost);
                if(gvd.found){
                    return gvd;
                }
            }

        }

        // check right tile
        StandardSprite * s = tileGrid.GetSprite({spritePos.x + tileSize.x, spritePos.y});
        if(s){
            Vec2f pTouching{-p.x, p.y};
            GhostVertexData gvd = FindGhostVertexInSprite(s, pTouching, frontGhost);
            if(gvd.found){
                return gvd;
            }
        }


    }else if(p.x == -wHalf){

        // left border

        if(p.y == hHalf){
            // check top left tile
            StandardSprite * s = tileGrid.GetSprite({spritePos.x - tileSize.x, spritePos.y + tileSize.y});
            if(s){
                Vec2f pTouching{-p.x, -p.y};
                GhostVertexData gvd = FindGhostVertexInSprite(s, pTouching, frontGhost);
                if(gvd.found){
                    return gvd;
                }
            }

        }else if(p.y == -hHalf){
            // check bottom left tile
            StandardSprite * s = tileGrid.GetSprite({spritePos.x - tileSize.x, spritePos.y - tileSize.y});
            if(s){
                Vec2f pTouching{-p.x, -p.y};
                GhostVertexData gvd = FindGhostVertexInSprite(s, pTouching, frontGhost);
                if(gvd.found){
                    return gvd;
                }
            }

        }

        // check left tile
        StandardSprite * s = tileGrid.GetSprite({spritePos.x - tileSize.x, spritePos.y});
        if(s){
            Vec2f pTouching{-p.x, p.y};
            GhostVertexData gvd = FindGhostVertexInSprite(s, pTouching, frontGhost);
            if(gvd.found){
                return gvd;
            }
        }
    }

    if(p.y == hHalf){

        // top border - check top tile

        StandardSprite * s = tileGrid.GetSprite({spritePos.x, spritePos.y + tileSize.y});
        if(s){
            Vec2f pTouching{p.x, -p.y};
            GhostVertexData gvd = FindGhostVertexInSprite(s, pTouching, frontGhost);
            if(gvd.found){
                return gvd;
            }
        }


    }else if(p.y == -hHalf){

        // bottom border - check bottom tile

        StandardSprite * s = tileGrid.GetSprite({spritePos.x, spritePos.y - tileSize.y});
        if(s){
            Vec2f pTouching{p.x, -p.y};
            GhostVertexData gvd = FindGhostVertexInSprite(s, pTouching, frontGhost);
            if(gvd.found){
                return gvd;
            }
        }

    }

    return GhostVertexData();

}




//--------------------------------------------------------------------------------------



TileGrid::TileGrid(Vec2i _nTiles, Vec2i _tileSize)
{
    Rebuild(_nTiles,_tileSize);

}


TileGrid::~TileGrid()
{
    DeleteGrid();
}



void TileGrid::Rebuild(Vec2i _numTiles, Vec2i _tileSize)
{

    //if(nTiles != _nTiles){
    if( !(mNumTiles == _numTiles)){
        DeleteGrid();

        mNumTiles = _numTiles;

        mTiles = new StandardSprite **[mNumTiles.x];
        for(int ix=0; ix<mNumTiles.x; ix++){
            mTiles[ix] = new StandardSprite *[mNumTiles.y];
        }
    }

    mTileSize = _tileSize;
    ClearSprites();

}


void TileGrid::DeleteGrid()
{
    if(mTiles==nullptr) return;

    for(int xt=0; xt<mNumTiles.x; xt++){
        delete[] mTiles[xt];
    }
    delete[] mTiles;

}


void TileGrid::AddSprite(StandardSprite *sprite)
{

    int xt = sprite->position().x/mTileSize.x;
    int yt = sprite->position().y/mTileSize.y;

    if(xt<0 || xt>=mNumTiles.x || yt<0 || yt>=mNumTiles.y){
        assert(false);
        return;
    }

    mTiles[xt][yt] = sprite;
}


StandardSprite *TileGrid::GetSprite(Vec2f worldPosition) const
{

    int xt = worldPosition.x/mTileSize.x;
    int yt = worldPosition.y/mTileSize.y;

    if(xt<0 || xt>=mNumTiles.x || yt<0 || yt>=mNumTiles.y) return nullptr;


    StandardSprite *s = mTiles[xt][yt];

    return s;

}



void TileGrid::ClearSprites()
{
    for(int xt=0; xt<mNumTiles.x; xt++){
        for(int yt=0; yt<mNumTiles.y; yt++){
            mTiles[xt][yt] = nullptr;
        }
    }

}





}













