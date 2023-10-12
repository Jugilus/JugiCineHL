#ifndef JUGIMAP_MAP_H
#define JUGIMAP_MAP_H

#include "ncine/SceneNode.h"
#include "jmCommon.h"
#include "jmParameter.h"
#include "jmGlobal.h"
#include "jmUsingDeclarations.h"
#include "jmLayerElement.h"


#ifdef JUGI_EDITOR
#include "general/tcolor.h"
#include "general/tpixmap.h"
#endif


namespace jugimap {

class Scene;
class Layer;
class SpriteLayer;
class VectorLayer;
class DrawingLayer;
//class Sprite;
//class VectorShape;
class MapLoader_bin;
class Camera;
class ShapeDrawer;
class GuiWidget;
class Map;
class LayerNode;
class SourceSprite;
class SpriteLayersGroup;
class VectorLayersGroup;
//class FontStyle;
class Font;
struct TextLabel;
class SourceGroup;

struct SM_ChangeChildText;
struct SM_ChangeChildParameter;

/*

class MapNode : public ncine::SceneNode
{
public:

    MapNode(Map *_m);
    virtual ~MapNode() override;

    nctl::Array<SceneNode *> &getChildren(){ return children_; }

    Map * m = nullptr;               // OWNED
};

*/



//--------------------------------------------------------------------------------------


/// \ingroup MapElements
/// \brief The Map class defines the map element from JugiMap Editor.
///
/// A map object stores Layer objects and is their owner.




class Map : public LayerElement
{
public:
    friend class MapWriter_bin;
    friend class MapLoader_bin;
    friend class SourceObjectsLoader_xml;
    friend class GameObjectsBuilder;




    Map(const std::string &_nameID);

    virtual void deleteContent() override;


    ///\brief Delete the map content.
    ///
    /// This function deletes layers and their content (sprites, vector shapes, ...).
    /// SourceGraphics objects (source sprites, textures, ...) assigned to map objects are **not** deleted!
    //void deleteWidgets();


    //virtual void Init(MapType _mapType);


    ///\brief Initialize this map as a word map.
    virtual void initAsWorldMap();


    ///\brief Initialize this map as a parallax map linked to a world map with the given size *_worldMapSize*.
    virtual void initAsParallaxMap(Vec2i _worldMapSize);


    ///\brief Initialize this map as a screen map with the given design size *_screenMapDesignSize*.
    virtual void initAsScreenMap(Vec2i _screenMapDesignSize);


    ///\brief Initialize the engine map objects.
    ///
    /// This function initialize the engine objects of all map elements. It should be called after a map was initialized by type.
    virtual void init(MapType _mapType);


    virtual void clearExternalDataLinks();


    ///\brief Update the engine map objects.
    ///
    /// This function must be called before the engine begin with drawing (rendering).
    //virtual void UpdateEngineObjects();


    virtual void updateCameraDependentObjects();

    void updateGeometry(int flags=0) override;
    void setEngineNodesEnabled(bool _enableAll) override;

    void updateBoundingBox(int flags = 0) override;


    void rebuildQuickAccessContainers() override;

    //virtual void UpdateColliders();


    ///\brief Draw the engine map objects.
    ///
    /// This function is needed only for the engines which do not handle drawing (rendering) internally.
    //virtual void DrawEngineObjects();


    ///\brief Assign a camera to this map.
    ///
    /// A camera must be assigned before calling the UpdateEngineMap() function.
    /// The type of an assigned camera depends of the map type:
    /// - For a world map use a WorldMapCamera camera.
    /// - For a parallax map use the same camera as is used for the world map.
    /// - For a screen map use a ScreenMapCamera.
    virtual void setCamera(Camera *_camera){ mCamera = _camera; }

    ///\brief Returns the camera assigned to this map.
    Camera* camera(){ return mCamera; }


    //std::vector<Camera*> & cameras(){ return mCameras; }

    //-----




    ///\brief Returns the type of this map.
    MapType type(){ return mType; }


    ///\brief Returns the tile size of this map.
    //Vec2i tileSize() {return mTileSize; }


    ///\brief Returns the number of tile rows and columns.
    //Vec2i tilesCount() {return mNTiles; }


    ///\brief Returns a reference to the layers of this map.
    //std::vector<Layer*>& GetLayers() {return layers; }


    ///\brief Returns a reference to the parameters of this map.
    //CParameters& parameters() {return mParameters; }


    ///\brief Returns the world map size.
    ///
    /// The returned value depends of the map type:
    /// - For a world map it is the size of the map.
    /// - For a parallax map it is the size of the linked world map.
    /// - For a screen map it is not defined (zero vector).
    /// This function should not be called before the map initalization (calling InitWorldMap, InitParallaxMap or InitScreenMap)!
    Vec2i worldMapSize() { return mWorldMapSize; }


    ///\brief Returns the screen map design size.
    ///
    /// The returned value depends of the map type:
    /// - For world maps and parallax maps it is not defined.
    /// - For a screen map it is the design size set at initialization.
    /// This function should not be called before the map initalization (calling InitWorldMap, InitParallaxMap or InitScreenMap)!
    Vec2i screenMapDesignSize() { return mScreenMapDesignSize; }


    ///\brief Returns the size of this map.
    Vec2i size() { return Vec2i(mWorldBoundingBox.GetWidth(), mWorldBoundingBox.GetHeight());}


    BoundingBoxFloat boundingBox(){ return mWorldBoundingBox; }


    void modifyXParallaxFactorsForFixedMapWidth(Vec2i _designViewport, Vec2i _newViewport);


    void modifyYParallaxFactorsForFixedMapHeight(Vec2i _designViewport, Vec2i _newViewport);


    ///\brief Capture the sprite properties, usually the position only,  required for the *lerp drawing*.
    ///
    /// \see Globals::SetUseLerpDrawing
    //void CaptureForLerpDrawing();


    ///\brief Set the visibility of this map.
    ///
    /// \see IsVisible
    //virtual void setVisible(bool _visible){ mVisible = _visible; mNode->setEnabled(_visible); }


    ///\brief Returns *true* if this map is visible; otherwise returns false.
    ///
    /// \see IsVisible
    //bool isVisible(){ return mVisible; }


    ///\brief Adds the given *_layer* to this map.
    ///
    /// If the *_index* is specified the layer will be inserted at that position in the layers vector; otherwise it will be added at the end.
    /// This function is available for the cases where we manually create maps or layers.


    SpriteLayer* addSpriteLayer(const std::string &_name);


    VectorLayer* addVectorLayer(const std::string &_name);



    //void AddDrawingLayer(jugimap::DrawingLayer* _drawingLayer);



    ///\brief Sets the name of this map to the given *_name*.
    ///
    /// This function is available for the cases where we manually create maps.
    //void _setName(const std::string &_name){ mName = _name; }




    ///\brief Sets the starting z-order.
    ///
    /// This function should be used before loading the map to set starting z-order value for layers.
    /// \see GetZOrderStart
    //void _setZOrderStart(int _zOrderStart){ mZOrderStart = _zOrderStart; }


    ///\brief Returns the starting z-order value.
    ///
    /// \see _SetZOrderStart
    //int zOrderStart(){ return mZOrderStart; }


    ///\brief Returns the next z-order to be used with a next (manually) added layer to the map.
    //int GetNextZOrder();


    virtual void drawShapes(ShapeDrawer &drawer);


    //-------
    void setBoundingBoxUpdateRequired(bool _value)
    {
        mBoundingBoxUpdateRequired = _value;
    }

    bool boundingBoxUpdateRequired(){ return mBoundingBoxUpdateRequired; }


    //MapNode* node(){ return mNode; }
    //LayerNode* node(){ return mNode; }

    //nctl::Array<ncine::SceneNode *> &nodeChildren(){ return mNode->getChildren(); }

    //void UpdateLayersVectors();

    std::vector<Layer*>& layers(){ return mLayers; }

    std::vector<SpriteLayer*>& spriteLayers() { return mSpriteLayers; }

    std::vector<VectorLayer*>& vectorLayers() { return mVectorLayers; }

    std::vector<std::string>& usedSourceGroupsNames(){ return mUsedSourceGroupsNames; }


    //----

    void eUpdateTextSprites_SetTexts(const SM_ChangeChildText &_modifier, int &_numUpdated);

    void updateSprites_SetParameters(const SM_ChangeChildParameter &_modifier, int &_numUpdated);

    void changeEllipsesToBezierCurves(int _dataFlag);


    //---- WIDGETS --------------------------------------------------------------
    /*

    void rebuildWidgets();

    void setWidgetsToInitialState();

    void updateWidgets();

    void addWidget(GuiWidget* _widget){ mWidgets.push_back(_widget); }

    GuiWidget* findWidget(const std::string &_name);

    GuiWidget* findWidget(int _tag);

    std::vector<GuiWidget*>& widgets(){ return mWidgets; }

    void setWidgetsActive(bool _widgetsActive){ mWidgetsActive = _widgetsActive; }

    bool areWidgetsActive(){ return mWidgetsActive; }

    */


//protected:
    virtual void initCustomParameters();


    int numTilesX(){ return mNumTilesX; }
    int numTilesY(){ return mNumTilesY; }
    int tileWidth(){ return mTileWidth; }
    int tileHeight(){ return mTileHeight; }

    void _setTileWidth(int _tileWidth) { mTileWidth = _tileWidth; }
    void _setTileHeight(int _tileHeight) { mTileHeight = _tileHeight; }

    void _setNumTilesX(int _numTilesX) { mNumTilesX = _numTilesX; }
    void _setNumTilesY(int _numTilesY) { mNumTilesY = _numTilesY; }


protected:



    //Vec2i mTileSize;
    //Vec2i mNTiles;
    //----
    //TEProjectInfo mProjectInfo;

    //CParameters mParameters;

    //---
    MapType mType = MapType::WORLD;
    Vec2i mWorldMapSize;                                 // used for WORLD and PARALLAX maps
    Vec2i mScreenMapDesignSize;                          // used for SCREEN maps
    BoundingBoxFloat mWorldBoundingBox;

    //---

    std::vector<Layer*>mLayers;                          // LINKs to layers (without groups) for more convinient access
    std::vector<SpriteLayer*>mSpriteLayers;              // LINKs
    std::vector<VectorLayer*>mVectorLayers;              // LINKs

    //---
    Camera* mCamera = nullptr;                           // LINK
    //std::vector<Camera*>mCameras;                        // LINKs

    //bool mVisible = true;
    //int mZOrderStart = 1000;

    std::vector<std::string>mUsedSourceGroupsNames;


    //---
    std::vector<GuiWidget*>mWidgets;                     // OWNER
    bool mWidgetsActive = true;

    bool mBoundingBoxUpdateRequired = false;

    void setLayersPlanes();
    void normalize();



    int mNumTilesX = 0;
    int mNumTilesY = 0;                    // velikost mape je skupna velikost tile layers bounding box
    int mTileWidth = 64;
    int mTileHeight = 64;




#ifndef JUGI_EDITOR

public:


    void onConstruct();

    virtual ~Map();


    Scene *mScene = nullptr;                // LINK

private:
    Map(const Map &l) = delete;
    Map& operator=(const Map &l) = delete;


#else

public:

    friend class jugi::gui::TNewMapDialog;

    static bool showCurrentLayerOnly;



    void onConstruct();
    Map(const Map &_Map);
    virtual ~Map() override;

    bool isMapOfCompositeSprite(){ return node()->compositeSprite!=nullptr; }

    //int DeleteObjectsWithFlagedSourceObjects(int _flagMask);

    //void UpdateTileLayerList();
    bool ChangeGridIfNeeded(const jugimap::BoundingBoxInt &_NewTilesBoundingBox, int &dxGridLeft, int &dyGridTop);
    //void UpdateLayersBoundingBoxes();
    void MoveContent(int dxt, int dyt) override;
    virtual void RebuildLayersGrids(int _nTilesX, int _nTilesY);
    void AddObjectsToLayersGrids(int flags=0);
    void ClearLayerGrids();
    //void UpdateBoundingBox(int flags = 0);
    //void UpdateTransformMatrices();
    //void UpdateTransformMatrices(SourceSprite *_SourceObject);
    void SyncParameters(SourceSprite *_SourceObject, int &_syncCount, bool _seekInComposedSprites);
    void SyncParameters_mapCompositeSprites(SourceSprite *_SourceObject, int &_syncCount);

    void CheckAndSaveComposedSpritesCfgFilesIfNeeded(int &_actionflag);

    void ChangeCurrentLayer(LayerElement *_Layer);


    bool TilePointInGrid(int _xt, int _yt){ return _xt>=0 && _xt<mNumTilesX && _yt>=0 && _yt<mNumTilesY;}
    bool TilePointInGrid(const Vec2i &_PT){ return TilePointInGrid(_PT.x, _PT.y);}
    bool HasObjectAtGridPosition(int _xt, int _yt);
    bool HasGridObjectAtPosition(int _xt, int _yt);
    bool ContainsSourceSprite(SourceSprite * _SourceObject);


    //ESpriteLayer* AddTileLayer(const QString &_name, int _flags);
    SpriteLayer* AddTileLayer(const std::string &_name, int _flags);
    void AddTileLayer(SpriteLayer * _Layer, int _flags);
    SpriteLayersGroup* AddLayersGroup(const std::string &_name, int _flags=0);
    void AddLayersGroup(SpriteLayersGroup* _LG, int _flags=0);
    void InsertLayerElement(LayerElement* _baseLayer, int _flags=0);
    VectorLayer* AddVectorLayer(const std::string &_name, int _flags=0);
    VectorLayer* AddVectorLayer(VectorLayer *_vectorLayer, int _flags=0);
    void AddVectorLayersGroup(VectorLayersGroup* _vectorLayersGroup, int _flags=0);

    LayerElement* currentLayer(){ return mCurrentLayer; }

    //jugi::base::TPixmap* RenderMapToPixmap(int _sizeMax, bool _drawGrid);
    jugi::base::TPixmap& RenderMapToBufferPixmap(int _sizeMax);
    void UpdateThumbnailPixmap();

    void PrepareAfterLoadNEW();

    void Draw(jugi::base::TGraphics& Graphics, int flags=0) override;
    void DrawBoundingBox(jugi::base::TGraphics& Graphics) override;
    void DrawCollisionShapes(jugi::base::TGraphics& Graphics);

    void updatePropertiesViaExtraParameters() override;

    //void GatherObjectsOfSourceObject(std::vector<Sprite*>&_Objects, SourceSprite *_SourceObject);
    VectorShape* FindVectorShapeOfNameID(const std::string &_nameID);

    void eUpdateTextSpritesWithChangedTextLabel(int &_numUpdated);
    void eVerifyTextSpritesBookIsAvailable(int &_numUpdated);
    bool eUpdateCompositeSpritesWithChangedSourceCompositeSprite(CompositeSprite* _changedSourceCompositeSprite, int &_numUpdated, int _flags=0);
    void eObtainUsedSourceGroups(std::vector<SourceGroup*>& _usedSourceGroups);
    void eUpdateStandardSpriteActiveTextureOnSourceMarkerChange(int &_numUpdated);
    bool eUpdateSpritesHandlePoint(int &_numUpdated);

    void eReplaceSpritesOfSourceSpritesFlagedForDeleteWithDummySprites(int &_numReplaced);
    void eDeleteSpritesWithSourceSpritesMarkedForDelete(int &_numDeleted);


    const std::string relativeFilePath(){ return mRelativeFilePath; }


    //void ChangeTextLabelStyles(Font *_currentFont, Font *_newFont,  bool rebuildPixmap = true);

    //void GatherTextLabelsWithNullPixmap(std::vector<TextLabel*> &_textLabels);



    void OnUpdatedBoundingBox(int flags = 0);



//private:




    //----
    std::string mfilePath;
    std::string mRelativeFilePath;
    //std::string mAutosaveFilePath;
    //std::string mDirExportMap;
    //std::string mDirExportSourceGraphics;
    std::string mDirExportImage;




    //size
    //int mNumTilesX = 0;
    //int mNumTilesY = 0;                    // velikost mape je skupna velikost tile layers bounding box
    jugimap::BoundingBoxInt mTilesBoundingBox;
    jugimap::BoundingBoxInt mTilesBoundingBoxOLD;
    jugi::base::TColor mBackgroundColor = jugi::base::TColor(0,0,0,0);
    //jugimap::BoundingBoxInt mBoundingBox;               // abolutna velikost - upoštevamo velikosti sprajtov ki lahko degajo izven območja TilesBoundingBox


    //---
    int mNumSprites=0;

    //std::vector<SpriteLayer*>mSpriteLayers;              // links na child items za enostavnejši dostop
    LayerElement *mCurrentLayer = nullptr;

    jugi::base::TPixmap *mThumbnailPixmap = nullptr;
    int mAutoSaveTimerOnPaused = -1;

    bool mMapEditedSaveNeeded = false;

    int mMapXstored = 0;
    int mMapYstored = 0;
    int mZoomStored = 100;

#endif

};


#ifdef JUGI_EDITOR

void ManageItemsVisibility(jugi::TItem *_rootItem, jugi::TItem *_soloVisibleItem);

#endif


//bool ContainChildWidthNameID_dupl(Map *map, const std::string &nameID);




/*

/// \ingroup MapElements
/// \class jugimap::Map
/// \brief The root map element.
///
/// The Map class represents a map created in JugiMap Editor.
/// It stores and owns Layer objects.
///
/// There are three types of maps:
/// 1. World map
/// 2. Parallax map
/// 3. Screen map
///
/// The type defines how a map will be displayed in game. A map loaded from a file has no defined type - we define its type by initializing it
/// according to its purpose. Of course, a map need to be designed according to its purpose as well.
///
/// An object which controls how a map is displayed is the Camera. Every map must have assigned a camera according to its type: world and parallax maps
/// must have assigned the same WorldMapCamera object, a screen map must have assigned a ScreenMapCamera object.
///
/// Another required step of the map setup is initialization of engine objects. Jugimap extended classes wrap
/// engine (game development framework) objects and these objects need to be created and initialized before they can be used.
///
/// The code bellow shows how to load and initialize the maps:
///
/// \code
///
///    // Load source graphics used by the maps
///
///    SourceGraphics* sourceGraphics = new SourceGraphics();
///    if(JugiMapBinaryLoader::LoadSourceGraphics(sourceGraphicsFilePath, sourceGraphics)==false){
///        // load error !
///    }
///
///    SourceGraphics* guiSourceGraphics = new SourceGraphics();
///    if(JugiMapBinaryLoader::LoadSourceGraphics(guiSourceGraphicsFilePath, guiSourceGraphics)==false){
///        // load error !
///    }
///
///
///    // Load maps
///
///    Map* levelMap = glob::objectFactory->NewMap();
///    if(JugiMapBinaryLoader::LoadMap(levelMapFilePath, levelMap, sourceGraphics)==false){
///        // load error !
///    }
///
///    Map* backgroundMap = glob::objectFactory->NewMap();
///    if(JugiMapBinaryLoader::LoadMap(backgroundMapFilePath, backgroundMap, sourceGraphics)==false){
///        // load error !
///    }
///
///    Map* guiMap = glob::objectFactory->NewMap();
///    if(JugiMapBinaryLoader::LoadMap(guiMapFilePath, guiMap, guiSourceGraphics)==false){
///        // load error !
///    }
///
///
///    // Initilaize maps by type.
///
///    // levelMap is a world map and we initialize it as such.
///    // We must initialize world maps before parallax maps as these require world map size as a parameter.
///    levelMap->InitWorldMap();
///
///    // backgroundMap is a parallax map and we initialize it as such.
///    backgroundMap->InitParallaxMap(levelMap->GetWorldMapSize());
///
///    // guiMap is a screen map and we initialize it as such.
///    // We need to specify a box "guiMapDesignSize" inside which the gui map layers will be aligned.
///    Vec2i guiMapDesignSize(270, glob::screenSize.y);
///    guiMap->InitScreenMap(guiMapDesignSize);
///
///
///    // Initilaize engine map objects.
///
///    levelMap->InitEngineMap();
///    backgroundMap->InitEngineMap();
///    guiMap->InitEngineMap();
///
///
///     // Set cameras
///
///    // levelMap requires a WorldMapCamera which will transform the world map into screen coordinates relative to specified viewport.
///    WorldMapCamera* levelCamera = new WorldMapCamera();
///    levelCamera->Init(Rectf(270,0, glob::screenSize.x, glob::screenSize.y), levelMap->GetWorldMapSize());
///    levelMap->SetCamera(levelCamera);            // A world map must have assigned a WorldMapCamera object !
///
///    // backgroundMap requires the same WorldMapCamera object.
///    backgroundMap->SetCamera(levelCamera);
///
///    // guiMap requires ScreenMapCamera which will transform the screen map guiMapDesignSize into screen coordinates.
///    ScreenMapCamera* guiCamera = new ScreenMapCamera();
///    Vec2f guiMapPosition = Vec2f(0,0);
///    guiCamera->Init(guiMapDesignSize, guiMapPosition);
///    guiMap->SetCamera(guiCamera);
///
/// \endcode
///
/// The maps were created via objectFactory->NewMap(). We must **always use object factory** to create objects of the engine extended classes!
///
/// See \ref Camera for more information about cameras.
///

*/



}



#endif
