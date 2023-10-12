#ifndef JMSPRITELAYER_H
#define JMSPRITELAYER_H

#ifdef JUGI_EDITOR
#include "../jugi/general/tgraphics.h"
#endif

#include <list>
#include "jmLayer.h"



namespace jugimap {

class Sprite;
class StandardSprite;
class TextSprite;
class SourceSprite;
class TObjectList;
class Font;
struct TextLabel;
class SourceGroup;
struct SM_ChangeChildText;
struct SM_ChangeChildParameter;



class SpriteLayersGroup : public LayerElement
{
public:





#ifndef JUGI_EDITOR
    SpriteLayersGroup();
    SpriteLayersGroup(const std::string& _nameID);
    void rebuildQuickAccessContainers() override;

#else
    void InitLayersGroup();
    SpriteLayersGroup();
    SpriteLayersGroup(const std::string& _nameID);
    SpriteLayersGroup(const SpriteLayersGroup& _SrcLayersGroup);

    void SetLayersMapPointers();

    void Draw(jugi::base::TGraphics& Graphics, int flags=0);
    void MoveContent(int dxt, int dyt) override;
    void rebuildQuickAccessContainers() override;


    //void UpdateBoundingBox(int flags=0) override;
#endif

};




///\ingroup MapElements
///\brief The SpriteLayer class defines the sprite and tile layers from JugiMap Editor.
///
/// A sprite layer stores Sprite objects and is their owner.
class SpriteLayer : public Layer
{
public:
    friend class MapLoader_bin;
    friend class MapWriter_bin;
    friend class SourceObjectsLoader_xml;
    friend class SpriteLayerNode;
    friend class CompositeSprite;
    friend class Map;



    SpriteLayer();
    SpriteLayer(Map *_map);
    SpriteLayer(const std::string &_name, Map *_map);
    SpriteLayer(const SpriteLayer &_SrcLayer);
    ~SpriteLayer();

    virtual void deleteContent() override;

    //----
    virtual void init() override;
    virtual void clearExternalDataLinks() override;
    virtual void updatePropertiesViaExtraParameters() override;
    virtual void updateCameraDependentObjects() override;
    virtual void updateBoundingBox(int flags=0) override;


    std::list<Sprite*>& sprites(){ return mSprites; }


    //void setVisible(bool _visible);
    //bool isVisible(){ return mVisible; }

    /// Sets the alpha factor of this sprite layer to the given *_alpha*. The alpha (opacity) factor is in the scale of [0.0 - 1.0].
    void setAlpha(float _alpha);
    /// Returns the alpha factor of this sprite layer.
    float alpha(bool _includingParentAlpha = true);
    /// Sets the blend factor of this sprite layer to the given *_blend*.
    void setBlend(Blend _blend);
    /// Returns the blend factor of this sprite layer.
    Blend blend() {return mBlend;}
    /// Adds a new sprite to this sprite layer. The layer will take over the ownership of the added sprite.
    void addSprite(Sprite* _sprite);
    void insertSprite(Sprite* _sprite, int _pos);

    StandardSprite * addStandardSprite(SourceSprite *_sourceSprite, Vec2f _position);

    TextSprite * addTextSprite(const std::string &_textString, Font* _font, ColorRGBA _color, Vec2f _position, TextHandleVariant _textHandle);
    /// Remove and delete the given sprite *_sprite* from this sprite layer. The sprite must belong to this layer otherwise it will not be deleted.
    void deleteSprite(Sprite* _sprite);

    void takeSprite(Sprite* _sprite);



    void deleteAllSprites();

    //int findLastZOrder();




    //-------
    void setBoundingBoxUpdateRequired(bool _value);

    bool boundingBoxUpdateRequired(){ return mBoundingBoxUpdateRequired; }

    SpriteLayerContent spriteLayerContent(){ return mContent; }


    //-----
    void eUpdateTextSprites_SetTexts(const SM_ChangeChildText &_modifier, int &_numUpdated);
    void updateSprites_SetParameters(const SM_ChangeChildParameter &_modifier, int &_numUpdated);

    void _setContent(SpriteLayerContent _content) { mContent = _content; }

    void changeEllipsesToBezierCurves(int _dataFlag);

    //void setEngineNodesEnabled(bool _forceEnabled) override;

private:
    //bool mVisible = true;
    float mAlpha = 1.0;                                  // Transparency for sprites: 0 - 1.0
    Blend mBlend = Blend::ALPHA;                         // Blending mode for sprites: SOLID, ALPHA, ADDITIVE
    std::list<Sprite*>mSprites;

    SpriteLayerContent mContent = SpriteLayerContent::GRID_SPRITES;



#ifdef JUGI_EDITOR

public:
    friend struct jugi::TTiledImporter;
    friend struct jugi::TTiledExporter;



    void SpriteLayerInit();
    void onDeconstruct();





    void Clear();
    //int DeleteObjectsWithoutSourceObjectOrSourceImages();
    int DeleteObjectsWithFlagedSourceObjects(int _flagMask);

    void RebuildGrid(int _nTilesX, int _nTilesY);
    void AddObjectsToGrid(int flags=0);
    void DeleteGrid();
    void ClearGrid();

    void MoveContent(int dxt, int dyt) override;
    //void UpdateBoundingBox(int flags = 0) override;
    //void updatePropertiesViaExtraParameters() override;

    void NormalizeZero(int _flags=0);

    //void UpdateTransformMatrices();
    //void UpdateTransformMatrices(SourceSprite *_SourceObject);

    void SyncParameters(SourceSprite *_SourceObject, int &_syncCount, bool _seekInComposedSprites=false);
    void SyncParameters_mapCompositeSprites(SourceSprite *_SourceObject, int &_syncCount);

    void CheckAndSaveComposedSpritesCfgFilesIfNeeded(int &_actionflag);

    //jugimap::BoundingBoxInt GetPixelBoundingBox();


    bool AddObjectToBrushLayer(Sprite* _object);
    Sprite* AddObject(Sprite* inobject, int _xt, int _yt, bool _flags);
    Sprite* AddObject(Sprite* inobject, int _xt, int _yt, int _dxInCell, int _dyInCell, bool _flags);
    void DeleteObject(Sprite* _Object, bool _flags);
    bool DeleteGridCellObjects(int _xt, int _yt, bool _flags);
    void InsertMapObjectToList(Sprite *_object);
    void syncChildNodesWithSprites();

    void StoreObjectsForUndo();
    Sprite* GetObjectByIdMapCopy(unsigned long long _idMapCopy);
    bool TilePointInGrid(int _xt, int _yt){ return _xt>=0 && _xt<mNumTilesX && _yt>=0 && _yt<mNumTilesY;}
    bool TilePointInGrid(const Vec2i &_PT){ return TilePointInGrid(_PT.x, _PT.y);}
    bool HasObjectAtPosition(int _xt, int _yt, int _xInCell, int _yInCell);

    void Draw(jugi::base::TGraphics& Graphics, int flags=0) override;
    void DrawCollisionShapes(jugi::base::TGraphics& Graphics);
    void DrawBoundingBox(jugi::base::TGraphics& Graphics) override;

    //void GatherObjectsOfSourceObject(std::vector<Sprite*>&_Sprites, SourceSprite *_SourceSprite);
    void eUpdateTextSpritesWithChangedTextLabel(int &_numUpdated);
    void eVerifyTextSpritesBookIsAvailable(int &_numUpdated);
    bool eUpdateCompositeSpritesWithChangedSourceCompositeSprite(CompositeSprite* _changedSourceCompositeSprite, int &_numUpdated, int _flags=0);
    //void GatherTextLabelsWithNullPixmap(std::vector<TextLabel*> &_textLabels);
    void eObtainUsedSourceGroups(std::vector<SourceGroup*>& _usedSourceGroups);
    void eUpdateStandardSpriteActiveTextureOnSourceMarkerChange(int &_numUpdated);
    bool eUpdateSpritesHandlePoint(int &_numUpdated);

    void eReplaceSpritesOfSourceSpritesFlagedForDeleteWithDummySprites(int &_numReplaced);
    void eDeleteSpritesWithSourceSpritesMarkedForDelete(int &_numDeleted);




    //---
    bool mIncludedInMultiLayerBrush = false;
    //int mBlending = jugi::base::TGraphics::ALPHABLEND;
    TObjectList ***mGridObjects = nullptr;
    int mNumTilesX=0, mNumTilesY=0;
    // int mTileWidth=0, mTileHeight=0;
    jugimap::BoundingBoxInt mTilesBoundingBox;

    SpriteLayer* mBrushLayer = nullptr;                   // OWNED pripadajoči brush layer iz BrushMap

    //--- tmp
    std::list<Sprite*> *mStoredObjectsForUndo = nullptr;   // pomozna lista za undo
    bool sourceSetLayerForWithTemporarySourceObjects = false;    //pomozni flag


#else

    void SpriteLayerInit();

    /// Constructor.

    SpriteLayer(const std::string &_name, int _flags=0);
    /// Destructor
    //virtual ~SpriteLayer() override;


#endif



private:

    //---
    bool mEditorTileLayer = false;
    //bool mSpritesChanged = false;
    //int mSpritesChangeFlag = 0;
    bool mBoundingBoxUpdateRequired = false;


    bool updateStretchingSingleSpriteLayer();

};





}




#endif // JMSPRITELAYER_H
