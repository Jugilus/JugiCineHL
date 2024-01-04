#ifndef JUGIMAP_LAYERS_H
#define JUGIMAP_LAYERS_H

#include "ncine/SceneNode.h"

#include "jmCommon.h"
#include "jmParameter.h"
#include "jmColor.h"
#include "jmGlobal.h"
#include "jmUsingDeclarations.h"
#include "jmLayerElement.h"



namespace jugimap {


class Map;
//class ESprite;
//class StandardSprite;
//class ESourceObject;
//class TextSprite;
//class Font;
//class ComposedSprite;
//class JugiMapBinaryLoader;
//class VectorShape;
//class ESpriteLayer;
//class EVectorLayer;
//class Layer;




struct ParallaxAndScreenLayerParameters
{
    static int typeLastSet;

    static const int typePARALLAX_LAYER = 0;
    static const int typePARALLAX_STRETCHING_SINGLE_SPRITE = 1;
    static const int typeSCREEN_LAYER = 2;
    static const int typeSCREEN_STRETCHING_SINGLE_SPRITE = 3;
    int type = typePARALLAX_LAYER;


    //Vec2i tiling;                                     // boolean flags
    Vec2i tilingCount = Vec2i(1,1);                     // how many time the laye content is tiles
    Vec2i tilingCountAutoSpread;                        // boolean flags
    Vec2i tilingSpacing;
    Vec2i tilingSpacingDelta;
    Vec2i alignPosition;                                 // relative to world size in % 0 - 100 :  0=left,top   100 =right,bottom
    Vec2i alignOffset;                                   // offset from alignPosition in pixels
    Vec2i alignOffset_ObtainFromMap = Vec2i(1,1);        // boolean flags
    Vec2f parallaxFactor;                               // speed factor

    std::string attachTolayer;

    //---
    int stretch = stretchXYToWorld;
    static const int stretchXYToWorld = 0;
    static const int stretchXYToView = 1;

};



///\ingroup MapElements
///\brief The Layer class is the base class for layers.
class Layer : public LayerElement
{
public:
    friend class MapLoader_bin;
    friend class LayerNode;
    friend class CompositeSprite;
    friend class Map;
    friend class MapLoader_bin;
    friend class MapWriter_binNEW;
    friend class SourceObjectsLoader_xml;


    /// Constructor.
    Layer();
    Layer(Map *_map);
    Layer(const std::string &_name, Map *_map);
    Layer(const Layer &_SrcLayer);
    Layer& operator=(const Layer &l) = delete;



    ///\brief  Initialize layer properties from the *parameters* set in the editor.
    ///
    /// This function should usually not be used manually as it gets called in the Map::InitMapParameters function.
    virtual void updatePropertiesViaExtraParameters(){}


    ///\brief  Initialize all engine objects related to this layer and its content.
    ///
    /// This function should usually not be used manually as it gets called in the Map::InitEngineMap function.
    virtual void init() {}


    virtual void clearExternalDataLinks(){}


    ///\brief  Update all engine objects related to this layer and its content.
    ///
    /// This function should usually not be used manually as it gets called in the Map::UpdateEngineMap function.
    //virtual void UpdateEngineObjects() {}


    virtual void updateCameraDependentObjects() {}


    //virtual void UpdateColliders(){}




    ///\brief  Draw all engine objects related to this layer and its content.
    ///
    /// This function should usually not be used manually as it gets called in the Map::DrawEngineMap function.
    /// It is needed only for the engines which do not handle drawing (rendering) internally.
    //virtual void DrawEngineObjects() {}


    ///\brief Returns a Map object where this layer is stored.
    ///
    /// If this layer is stored in a ComposedSprite it will returns the map where this composed sprite is located.
    Map* map() {return mMap;}


    ///\brief Returns a ComposedSprite where this layer is stored or *nullptr* if it is stored in a map.
    CompositeSprite* parentCompositeSprite() {return mParentCompositeSprite;}

    CompositeSprite* rootCompositeSprite();

    Map* rootMap();


    ///\brief Returns true if this layer is stored in a Map; if it's stored in a ComposedSprite returns false.
    //bool isMapLayer(){ return (mParentComposedSprite==nullptr); }


     ///\brief Returns the kind of this layer.
    //LayerKind kind() {return mKind;}


    ///\brief Returns the name of this layer.
    //std::string name() {return mName;}


    ///\brief Returns a reference to the vector of stored paramters in this layer.
    //CParameters& parameters() {return mParameters;}


    ///\brief Returns the zOrder of this layer.
    ///
    /// The zOrder is usable in engines where we set drawing order via zOrder factor.
    //int zOrder() {return mZOrder;}




    /// \brief Returns the map bounding box of this layer.
    BoundingBoxFloat worldBoundingBox(){ return mWorldBoundingBox;}


    ///\brief Returns the LayerType factor of this layer.
    ///
    /// This factor is used for layers stored in parallax maps.
    LayerType layerType(){ return mLayerType;}


    ///\brief Returns the AlignX factor of this layer.
    ///
    /// This factor is used for layers stored in parallax maps and screen maps.
    //AlignX GetAlignX() {return alignX;}


    ///\brief Returns the AlignY factor of this layer.
    ///
    /// This factor is used for layers stored in parallax maps and screen maps.
    //AlignY GetAlignY() {return alignY;}

    //StretchingVariant GetStretchingVariant(){ return mStretchingVariant;}


    ///\brief Returns the allign position factor of this layer.
    ///
    /// This position is used for layers stored in parallax maps and screen maps.
    //Vec2i GetAlignPosition(){ return alignPosition; }


    ///\brief Returns the allign offset factor of this layer.
    ///
    /// This factor is used for layers stored in parallax maps and screen maps.
    //Vec2f GetAlignOffset(){ return alignOffset; }


    ///\brief Returns the layers plane size of this layer.
    ///
    /// This factor is used for layers stored in parallax maps and screen maps.
    //Vec2f GetLayersPlaneSize(){return layersPlaneSize;}


    ///\brief Returns the ParallaxLayerMode factor of this layer.
    ///
    /// This factor is used for layers stored in parallax maps.
    //ParallaxLayerMode GetParallaxLayerMode(){ return parallaxLayerMode;}





    ///\brief Returns the parallax factor of this layer.
    ///
    /// This factor is used for layers stored in parallax maps.
    //Vec2f parallaxFactor() {return mParallaxFactor;}


    ///\brief Returns the parallax offset of this layer.
    ///
    /// The parallax offset is an extra offset added to the sprites position.
    /// This factor is used for layers stored in parallax maps.
    Vec2f parallaxOffset(){return mParallaxOffset;}


    ///\brief Returns the ScreenLayerMode factor of this layer.
    ///
    /// This factor is used for layers stored in screen maps.
    //ScreenLayerMode GetScreenLayerMode(){ return screenLayerMode;}


    //-------

    ///\brief Sets the name of this layer to the given *_name*.
    ///
    /// This function is available for the cases where we manually create layers.
    //void _setName(const std::string &_name){ mName = _name; }


    ///\brief Sets the kind of this layer to the given *_kind*.
    ///
    /// This function is available for the cases where we manually create layers.
    void _setKind(LayerKind _kind){ mKind = _kind; }


    ///\brief Sets the layer mode of this layer to the given *_layerType*.
    ///
    /// This function is available for the cases where we manually create layers.
    void _setLayerType(LayerType _layerType){ mLayerType = _layerType; }


    void _setMap(Map *_map) { mMap = _map; }


    void _setParentCompositeSprite(CompositeSprite *_compositeSprite) { mParentCompositeSprite = _compositeSprite; }

    ///\brief Sets the parallax factor of this layer to the given *_parallaxFactor*.
    ///
    /// The parallax factor is used only for layers of LayerType::PARALLAX
    /// This function is available for the cases where we manually create layers.
    //void _setParallaxFactor(Vec2f _parallaxFactor){ mParallaxFactor = _parallaxFactor; }


    ///\brief Sets the align position of this layer to the given *_alignPosition*.
    ///
    /// The align position is used only for layers of LayerType::PARALLAX and LayerType::SCREEN
    /// This function is available for the cases where we manually create layers.
    //void _setAlignPosition(Vec2i _alignPosition){ mAlignPosition = _alignPosition; }


    ///\brief Sets the align offset of this layer to the given *_alignOffset*.
    ///
    /// The align offset is used only for layers of LayerType::PARALLAX and LayerType::SCREEN
    /// This function is available for the cases where we manually create layers.
    //void _setAlignOffset(Vec2f _alignOffset){ mAlignOffset = _alignOffset; }


    ///\brief Sets the 'attachToLayer'  of this layer to the given *_attachToLayer*.
    ///
    /// The 'attachToLayer' is used only for layers of LayerType::PARALLAX and LayerType::SCREEN
    /// This function is available for the cases where we manually create layers.
    //void _setAttachToLayer(std::string _attachToLayer){ mAttachToLayer = _attachToLayer; }


    ///\brief Sets the stretching variant of this layer to the given *_stretchingVariant*.
    ///
    /// The stretching variant is used only for layer of LayerType::PARALLAX_STRETCHING_SINGLE_SPRITE.
    /// This function is available for the cases where we manually create layers.
    //void _setStretchingVariant(StretchingVariant _stretchingVariant){ mStretchingVariant = _stretchingVariant;}


    ///\brief Sets the z-order factor of this layer to the given *_zOrder*.
    ///
    /// This function is available for the cases where we manually create layers.
    //void _setZOrder(int _zOrder){ mZOrder = _zOrder; }



    ParallaxAndScreenLayerParameters &pp(){ return mPP; }


protected:

    /// Destructor.
    virtual ~Layer(){}

    bool UpdateParallaxOffset();


    Map *mMap = nullptr;                               // LINK to map
    CompositeSprite *mParentCompositeSprite = nullptr;   // LINK to parent composed sprite (or nullptr if parent is map).

    //---
    //int mZOrder = 0;                                      // Drawing order of layer sprites (used by some engines)
    //---
    BoundingBoxFloat mWorldBoundingBox;


    ParallaxAndScreenLayerParameters mPP;
    //---- properties of layers of the parallax maps
    LayerType mLayerType = LayerType::WORLD;
    Vec2f mLayersPlaneSize;
    Vec2f mParallaxOffset;


};



}


#endif
