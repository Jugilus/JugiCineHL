#ifndef PLAYER__ENTITIES__BOX2D_UTILITIES__H
#define PLAYER__ENTITIES__BOX2D_UTILITIES__H


#include "string"
#include "vector"
#include "box2d/box2d.h"
#include "jmCommonFunctions.h"
#include "jpGlobal.h"




namespace jugimap{

class Sprite;
class StandardSprite;
struct SourceFixture;
class TileGrid;



struct GhostVertexData
{
    bool found = false;
    Vec2f ghostVertex;
    StandardSprite *sprite = nullptr;

};



void createB2ShapeVerticesForTileSprite(SourceFixture &sf, Vec2f spriteRelPos, TileGrid *tileGrid=nullptr);


GhostVertexData FindGhostVertex(Vec2f p, Vec2f spritePos, const TileGrid &tileGrid, bool frontGhost);



//---------------------------------------------------------


class TileGrid
{
public:

    TileGrid(){}
    TileGrid(Vec2i _nTiles, Vec2i _tileSize);
    ~TileGrid();

    void Rebuild(Vec2i _nTiles, Vec2i _tileSize);
    void DeleteGrid();

    Vec2i GetNumTiles() const {return mNumTiles;}
    Vec2i GetTileSize() const {return mTileSize;}

    void AddSprite(StandardSprite  *sprite);
    StandardSprite *GetSprite(Vec2f worldPosition) const;

    void ClearSprites();


private:
    StandardSprite  ***mTiles = nullptr;               // SPRITE LINKS in OWNED array of arrays
    Vec2i mNumTiles;
    Vec2i mTileSize;
};





}


#endif
