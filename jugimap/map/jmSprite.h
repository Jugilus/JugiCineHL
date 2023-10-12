#ifndef JMSPRITE_H
#define JMSPRITE_H


#ifdef JUGI_EDITOR

#include <list>
#include "jugi/eFramework/map/mapEVarious.h"
#include "jugi/globals.h"
#endif

#include <list>
#include <ncine/SceneNode.h>
#include "jmSourceSprite.h"
#include "jmCommon.h"
#include "jmParameter.h"
#include "jmColor.h"
#include "jmGlobal.h"


namespace jugimap {



class MapLoader_bin;
class SourceImage;
//class Layer;
class SpriteLayer;
class SourceSprite;
class CompositeSprite;
//class StandardSprite;
//class TextSprite;
class Collider;
struct AnimatedProperties;
class Font;


//struct EAnimatedProperties;
class AnimationInstance;
//class SpriteModifiers;
class Map;



///\ingroup MapElements
///\brief The Sprite is the base sprite class.
class Sprite
{
public:
    friend class MapLoader_bin;
    friend class SourceObjectsLoader_xml;
    friend class SpriteLayer;
    friend class DummySourceObjects;


    /// The Property enumerator provides flags for sprite properties.
    enum Property
    {
        NONE =      0,
        POSITION =  1 << 0,
        FLIP =      1 << 1,
        SCALE =     1 << 2,
        ROTATION =  1 << 3,
        HANDLE =    1 << 4,

        ALPHA =     1 << 5,
        BLEND =     1 << 6,
        OVERLAY_COLOR = 1 << 7,
        COLOR =     1 << 8,

        FRAME =     1 << 12,    //temporary
        TEXTURE =     1 << 10,
        TEXT_STRING = 1 << 11,

        VISIBILITY =  1 << 13,

        ID =        1 << 15,
        TAG =       1 << 16,
        DATA =      1 << 17,
        LINK =      1 << 18,

        TRANSFORMATION = POSITION | FLIP | SCALE |  ROTATION | HANDLE,
        APPEARANCE = ALPHA | BLEND | OVERLAY_COLOR | COLOR,
        ALL = TRANSFORMATION | APPEARANCE | TEXTURE | TEXT_STRING | VISIBILITY |  ID | TAG | DATA | LINK
    };

    enum IncludedPositions
    {
        BASE =              1 << 0,
        ANI_TRANSLATION =         1 << 1,
        PARRALAX_OFFSET =   1 << 2
    };


    ///\brief Create a new sprite which is a passive copy of this sprite.
    ///
    /// Sprites created as passive copies does not have initialized their engine sprite and they are not added to the layer.
    /// To display a passive sprite on screen you must turn it into an active copy:
    /// \code
    /// sprite->GetLayer()->AddSprite(sprite);
    /// sprite->InitEngineSprite();
    /// \endcode
    ///
    /// You can also create an active sprite from a passive sprite by using MakeActiveCopy.
    /// \see MakeActiveCopy
    Sprite* makePassiveCopy();


    static Sprite* makePassiveSprite(SourceSprite *_sourceSprite);


    ///\brief Create a new sprite which is an active copy of this sprite.
    ///
    /// Sprites created as an active copies are displayed on screen immediately.
    /// \see MakeActiveCopy
    Sprite* makeActiveCopy();


    static Sprite* makeSpriteByKind(SpriteType _kind);



    ///\brief Copy properties from the given **_sprite**.
    ///
    /// The given **copyFlags** must consists of flags defined in the Property enumerator.
    /// **Important:** This function does not copy core sprite properties:
    /// *source Sprite*, *layer*, *parentComposedSprite*;
    virtual void copyProperties(Sprite *_sprite, int copyFlags);


    virtual void updatePropertiesViaExtraParameters(){}


    //------ MANAGE ENGINE OBJECTS


    ///\brief Initialize the engine sprite.
    ///
    /// This function creates and initialize the engine sprite.
    /// It gets called in the SpriteLayer::InitEngineLayer at the map engine initialization but we can use it also
    /// to initilaze sprites created with MakePassiveCopy.
    virtual void initEngineObjects() = 0;


    virtual void clearExternalDataLinks() = 0;


    ///\brief Update the engine sprite.
    ///
    /// This function updates the engine sprite when properties of the jugimap sprite change. Usually you don't need to use it as other functions call it when required.
    //virtual void updateEngineObjects(){}


    ///\brief Set the visibility of this sprite.
    ///
    /// \see IsVisible
    virtual void setVisible(bool _visible);


    ///\brief Returns *true* if this sprite is visible; otherwise returns false.
    ///
    /// \see IsVisible
    bool isVisible(){ return mVisible; }


    //------
    ///\brief Update the map bounding box of this sprite.
    ///
    /// The dimensions of bounding box are based on the size of the active texture. If a sprite is not scaled or rotated the bounding box and the texture have the same size.
    /// Usually you don't need to use this function as other functions call it when required.
    //virtual void updateBoundingBox(bool force=false) {mWorldBoundingBox = BoundingBoxFloat(mPosition, mPosition);}
    virtual void updateBoundingBox() {}

    /// \brief Returns the world bounding rectangle of this sprite.
    ///
    /// \see UpdateBoundingBox
    virtual BoundingBoxFloat boundingBox(){ return mWorldBoundingBox;}
    virtual BoundingBoxInt boundingBoxInt(){ return BoundingBoxInt(mWorldBoundingBox.min.x, mWorldBoundingBox.min.y, mWorldBoundingBox.max.x, mWorldBoundingBox.max.y); }



    //---- LINK OBJECTS


    ///\brief Returns a sprite layer where this sprite is stored.
    SpriteLayer* parentLayer() {return mParentLayer;}


    ///\brief Returns the source sprite of this sprite.
    SourceSprite* sourceSprite() {return mSourceSprite;}


    ///\brief Returns the parent composed sprite of this sprite.
    ///
    /// If this sprite is not inside a composed sprite returns nullptr.
    CompositeSprite* parentCompositeSprite();


    ///\brief Returns the kind of this sprite.
    ///
    /// The kind factor is the same as the kind of assigned SourceSprite object.
    SpriteType type();


    //------
    ///\brief Set the id of this sprite to the given _id.
    ///
    /// An id is an arbitrary integer number which can be assigned to a Sprite object in the editor.
    /// \see GetId
    void setId(int _id){ mId = _id; }


    ///\brief Returns the id of this sprite.
    ///
    /// \see SetId
    int id(){ return mId;}


    ///\brief Set the name of this sprite to the given _nameID.
    ///
    /// A name is an arbitrary string which can be assigned to a Sprite object in the editor.
    /// \see GetName
    void setName(const std::string& _nameID){ mName = _nameID; }


    ///\brief Returns the name of this sprite.
    ///
    /// \see SetName
    const std::string & name(){ return mName;}



    ///\brief Set the tag of this sprite to the given _tag.
    ///
    /// A tag is an arbitrary integer number which can be assigned to a Sprite object.
    /// \see GetTag
    void setTag(int _tag){ mTag = _tag; }


    ///\brief Returns the tag of this sprite.
    ///
    /// \see SetTag
    int tag(){ return mTag;}


    //------

    ///\brief Assign a custom object of this sprite to the given _customObject.
    ///
    /// A custom object is an arbitrary object which can be assigned to a sprite.
    /// **Important!** The sprite takes ownership over the custom object.
    /// \see GetCustomObject
    void setCustomObject(CustomObject* _customObject){ mCustomObject = _customObject; }


    ///\brief Returns the custom object of this sprite if exists; otherwise returns nullptr.
    ///
    /// \see SetCustomObject
    CustomObject* getCustomObject(){ return mCustomObject; }


    //------


    ///\brief Returns a reference to the vector of stored parameters in this sprite.
    ///
    /// These are the parameters defined in Jugimap Editor.
    CParameters &parameters(){return mParameters;}


    ///\brief Returns the dataFlags of this sprite.
    ///
    /// This is an integer defined in JugiMap Editor.
    int dataFlags() {return mDataFlags;}


    void setDataFlags(int _dataFlags){ mDataFlags = _dataFlags; }


    // TRANSFORM PROPERTIES
    //-------------------------------------------------------------------------------

    ///\brief Set the position of this sprite to the given **_position**.
    ///
    /// \see GetPosition, GetGlobalPosition, GetFullPosition, GetFullGlobalPosition
    virtual void setPosition(Vec2f _position);


    void updatePosition();



    ///\brief Returns the position of this sprite.
    ///
    /// This is a local position which is for sprites inside the map layers also a global position.
    /// For sprites in layers of a composed sprite this position is relative to the composed sprite coordinate system.
    /// If the given *_includeAnimatedProperties* is *true*, the animated properties are included in the returned value.
    ///
    /// \see GetGlobalPosition, GetFullPosition, GetFullGlobalPosition, SetPosition
    Vec2f position(IncludedPositions _included = IncludedPositions::BASE);


    Vec2f absPosition() { return mNode->absPosition(); }

    //------


    ///\brief Set the horizontal and vertical scale factor of this sprite to the given **_scale**.
    ///
    ///\see GetScale, GetGlobalScale
    void setScale(Vec2f _scale);


    ///\brief Returns the horizontal and vertical scale factor of this sprite.
    ///
    /// This is a local scale factor which is for sprites inside the map layers also a global scale factor.
    /// If the given *_includeAnimatedProperties* is *true*, the animated properties are included in the returned value.
    ///
    /// \see GetGlobalScale, SetScale
    Vec2f scale(bool _includeAnimatedProperties = true);


    Vec2f absScale(){ return mNode->absScale(); }

    //------


    ///\brief Set the rotation of this sprite to the given **_rotation**.
    ///
    /// The rotation is in degrees.
    ///\see GetRotation, GetGlobalRotation
    void setRotation(float _rotation);


    ///\brief Returns the rotation of this sprite in degrees.
    ///
    /// This is a local rotation which is for sprites inside the map layers also a global rotation.
    /// If the given *_includeAnimatedProperties* is *true*, the animated properties are included in the returned value.
    ///
    /// \see GetGlobalRotation, SetRotation
    float rotation(bool _includeAnimatedProperties = true);


    float absRotation() const { return mNode->absRotation(); }

    //------


    ///\brief Set the horizontal and vertical flip factor of this sprite to the given **_flip**.
    ///
    ///\see GetFlip, GetGlobalFlip
    void setFlip(Vec2i _flip);


    ///\brief Returns the horizontal and vertical flip factor of this sprite.
    ///
    /// The flip value 1 means that a sprite is *flipped*, the value 0 means that it is not.
    /// This is a local flip factor which is for sprites inside the map layers also a global flip factor.
    /// If the given *_includeAnimatedProperties* is *true*, the animated properties are included in the returned value.
    ///
    /// \see GetGlobalFlip, SetFlip
    Vec2i flip(bool _includeAnimatedProperties = true);

    //------

    const ncine::Matrix4x4f &worldMatrix() { return mNode->worldMatrix(); }


    const ncine::Matrix4x4f &localMatrix() { return mNode->localMatrix(); }


    //------

    ///\brief Set the handle point of this sprite to the given **_handle** point.
    ///
    /// The handle point is a point in the sprite local coordinates, relative to which the sprite transformations are being applied.
    /// This point is automatically assigned to the handle point of the active image (for the standard sprites).
    /// You can manually use this function to set an arbitrary handle point but be aware that changing
    /// the active image will set the sprite handle point again to that of the image. The sprite shapes
    /// are also relative to the image handle point and will no longer be correct if you set a different handle point.
    virtual void setHandle(Vec2f _handle){};


    ///\brief Returns the handle point of this sprite.
    Vec2f handle() {return mHandle;}


    // APPEARANCE PROPERTIES
    //-------------------------------------------------------------------------------

    ///\brief Set the alpha of this sprite to the given **_alpha**.
    ///
    /// The alpha is an opacity factor in the range of [0.0 - 1.0].
    ///\see GetAlpha
    void setAlpha(float _alpha);


    ///\brief Returns the alpha of this sprite.
    ///
    /// If the given *_includeAnimatedProperties* is *true*, the animated properties are included in the returned value.
    ///
    /// \see SetAlpha
    float alpha(bool _includeAnimatedProperties = true);


    //------

    ///\brief Set the own blend of this sprite to the given **_blend**.
    ///
    /// The own blend is a Blend factor assigned to a sprite. If the own blend is not defined (Blend::NOT_DEFINED)
    /// then the Blend factor of the sprite layer will be used.
    ///\see GetOwnBlend
    void setOwnBlend(Blend _blend);


    ///\brief Returns the own blend of this sprite.
    ///
    ///\see SetOwnBlend
    Blend ownBlend(){return mBlend;}


    //------

    ///\brief Set to *true* to make the color overlay active; set to *false* to disable the color overlay.
    ///
    /// The color overlay is a color overlayed over a sprite.
    ///\see IsOverlayColorActive, SetColorOverlayRGBA, SetColorOverlayBlend
    void setColorOverlayActive(bool _colorOverlayActive);


    ///\brief Returns true if the overlay color of this sprite is active; otherwise returns false.
    ///
    ///\see SetColorOverlayActive, SetColorOverlayRGBA, SetColorOverlayBlend
    bool isOverlayColorActive();



    ///\brief Set the color overlay of this sprite to the given  **_colorOverlayRGBA**.
    ///
    ///\see GetColorOverlayRGBA, SetColorOverlayActive, SetColorOverlayBlend
    void setColorOverlayRGBA(ColorRGBA _colorOverlayRGBA);


    ///\brief Returns the overlay color of this sprite.
    ///
    /// If the given *_includeAnimatedProperties* is *true*, the animated properties are included in the returned value.
    ///
    ///\see SetColorOverlayRGBA,
    ColorRGBA overlayColorRGBA(bool _includeAnimatedProperties = true);


    ///\brief Set the color overlay blend of this sprite to the given  **_colorOverlayBlend**.
    ///
    /// This blending works the same as blending in painting applications when you overlay layers.
    ///\see GetColorOverlayBlend, SetColorOverlayActive, SetColorOverlayRGBA
    void setColorOverlayBlend(ColorOverlayBlend _colorOverlayBlend);


    ///\brief Returns the overlay color blend of this sprite.
    ///
    /// If the given *_includeAnimatedProperties* is *true*, the animated properties are included in the returned value.
    ///
    /// \see SetColorOverlayBlend
    ColorOverlayBlend overlayColorBlend(bool _includeAnimatedProperties = true);


    //------ ANIMATED PROPERTIES


    ///\brief Returns the pre-animation properties.
    AnimatedProperties* animatedProperties(){ return mAP; }


    ///\brief Store current properties for animation purposes.
    ///
    /// This function is usually called from animation objects.
    ///\see Collider
    virtual void createAnimatedPropertiesIfNone();


    ///\brief Set sprite properties to pre-animation state.
    ///
    /// This function is usually called from animation objects.
    ///\see Collider
    virtual void resetAnimatedProperties();


    ///\brief Set sprite properties from the given **_ap** object.
    ///
    /// This function is usually called from animation objects.
    ///\see Collider
    virtual void applyAnimatedProperties(AnimatedProperties& _ap);



    //------ SPRITE MODIFIERS

    //SpriteModifiers* spriteModifiers(){ return mSM; }
    //void createSpriteModifiersIfNone();


    //------ COLLIDER


    ///\brief Returns true if this sprite has assigned a collider; otherwise returns false.
    ///
    /// A collider is assigned to sprites which have SourceSprite with defined sprite shapes.
    ///\see Collider
    virtual bool hasCollider() { return false; }


    ///\brief Returns true if the given point **_pos** is inside the shape(s) of this sprite; otherwise returns false.
    ///
    /// The given point must be in global coordinate system. This function uses a sprite collider to obtain the result. It returns *false* if this sprite has no collider assigned.
    ///\see Collider
    virtual bool pointInside(Vec2f _pos){ return false; }


    ///\brief Returns true if the shapes of this and the given **_sprite** overelaps; otherwise returns false.
    ///
    /// This function uses sprite colliders to obtain the result. It returns *false* if one or both of sprites is without a collider.
    ///\see Collider
    virtual bool overlaps(Sprite *_sprite){  return false; }


    ///\brief Returns true if a ray (line segment) from **_rayStart** to **_rayEnd** 'hits' at the shape(s) of this sprite; otherwise return false.
    ///
    /// If the 'hit' occurs its position get stored in the given **_hitPos**.
    /// This function uses sprite colliders to obtain the result. It returns *false* if this sprite has no collider assigned.
    ///\see Collider
    virtual bool raycastHit(Vec2f _rayStart, Vec2f _rayEnd, Vec2f &_hitPos){  return false; }



    //-------- CHANGE FLAGS


    ///\brief Set the change flags of this sprite to the given *_changeFlags*.
    ///
    /// The *_changeFlags* must be a value or a bitwise combination of values defined in the Property enumerator.
    /// \see AppendToChangeFlags, GetChangeFlags
    //void setChangeFlags(int _changeFlags);


    ///\brief Append the given *_changeFlags* to the change flags of this sprite with a biwise operation.
    /// ///
    /// The *_changeFlags* must be a value or a bitwise combination of values defined in the Property enumerator.
    /// \see SetChangeFlags, GetChangeFlags
    //void appendToChangeFlags(int _changeFlags);


    ///\brief Returns the *change flags* of this sprite.
    ///
    /// \see SetChangeFlags, AppendToChangeFlags
    //int changeFlags() {return mChangeFlags;}


    //--------


    ///\brief Set the source sprite of this sprite to the given **_sourceSprite**.
    ///
    /// This function must never be used on already created sprites.
    /// It should be used only for purposes of manual sprite creation.
    void _setSourceSprite(SourceSprite *_sourceSprite){ mSourceSprite = _sourceSprite; }


    ///\brief Set the layer of this sprite to the given **_spriteLayer**.
    ///
    /// This function must never be used on already created sprites.
    /// It should be used only for purposes of manual sprite creation.
    void _setParentLayer(SpriteLayer *_spriteLayer){ mParentLayer = _spriteLayer; }



    //void _setParentComposedSprite(EComposedSprite *_cs){ mParentComposedSprite = _cs; }

    //void _setSourceSpriteReplacement(SourceSpriteReplacements *_ssr){ mSSR = _ssr; }
    //void _setSourceSpriteReplacement(SourceSprite * _sourceSprite)
    //{
    //    if(mOriginalSourceSprite==nullptr){
    //        mOriginalSourceSprite = mSourceSprite;
    //    }
    //    mSourceSprite= _sourceSprite;
    //}


    //SourceSpriteReplacements* sourceSpriteReplacements(){ return mSSR;}


    //SourceSprite* originalSourceSprite(){ return mOriginalSourceSprite? mOriginalSourceSprite : mSourceSprite; }


    bool isInitialized(){ return mInitialized; }

    void _setInitialized(bool _initialized){ mInitialized = _initialized; }

    //-------

    void setParentBoundingBoxUpdateRequired(bool _value);

    bool boundingBoxUpdateRequired(){ return mBoundingBoxUpdateRequired; }


    ncine::SceneNode* node(){ return mNode; }

    virtual void updateAllNodeParameters();


    void setPositionFromGridPosition(Vec2i cellPos, Vec2i inCellPos, Vec2i cellSize);


    bool isHandleForCompositeSprite(){ return mHandleForCompositeSprite; }
    void _setHandleForCompositeSprite(bool _handleForCompositeSprite){ mHandleForCompositeSprite = _handleForCompositeSprite; }


    virtual void setEngineNodesEnabled(bool _enableAll){}



    std::list<Sprite*>::iterator iter;        // position in the parent sprite layers list


protected:

    //--- LINKS TO CONNECTED OBJECTS
    ncine::SceneNode* mNode = nullptr;                      // LINK pointer to an extended ncine node
    SpriteLayer *mParentLayer = nullptr;                    // LINK pointer to a layer where the sprite is stored.
    SourceSprite * mSourceSprite= nullptr;                  // LINK pointer to the belonging SourceSprite.
    //EComposedSprite * mParentComposedSprite = nullptr;       // LINK pointer to parent composed sprite if sprite is child of a composed sprite.


    //--- IDENTIFICATION AND CUSTOM DATA
    int mId = 0;
    std::string mName;
    int mTag = 0;
    CustomObject* mCustomObject = nullptr;
    CParameters mParameters;
    int mDataFlags = 0;

    //--- TRANSFORMATION PROPERTIES
    Vec2f mHandle;
    Vec2f mPosition;
    Vec2f mScale = Vec2f(1.0, 1.0);
    float mRotation = 0.0;
    Vec2i mFlip;

    //--- APPEARANCE PROPERTIES
    bool mVisible = true;
    float mAlpha = 1.0;                                      // in scale [0 - 1.0]
    Blend mBlend = Blend::NOT_DEFINED;
    bool mColorOverlayActive = false;
    ColorRGBA mColorOverlayRGBA = 0;
    ColorOverlayBlend mColorOverlayBlend = ColorOverlayBlend::NORMAL;

    //--- ANIMATION
    AnimatedProperties* mAP = nullptr;                      // OWNED


    //SpriteModifiers* mSM = nullptr;

    //--- source sprite replacement
    //SourceSprite* mSourceSpriteReplacement = nullptr;               // LINK
    //SourceSprite* mOriginalSourceSprite = nullptr;                  // LINK
    //std::vector<ChildSourceObjectReplacement>mChildrenSourceSpriteReplacements;

    //---
    BoundingBoxFloat mWorldBoundingBox;

    bool mInitialized = false;
    bool mBoundingBoxUpdateRequired = false;void *mExtraPtr = nullptr;
    bool mHandleForCompositeSprite = false;


    virtual void manageShaders_OverlayColor();




#ifdef JUGI_EDITOR

public:

    friend class MapWriter_bin;
    friend class SourceObjectsWriter_xml;
    friend class jugi::gui::TPropertiesDock;
    friend class jugi::TTiledImporter;


    static unsigned long long idMapCopyCounter;
    static unsigned long long mapObjectReplaceLimitID;


    //ESpriteLayer * mParentLayer = nullptr;                  // LINK ; layer kopije imajo link na layer
    //ESourceObject* mSourceSprite = nullptr;     // LINK ; layer kopije imajo link na izvorni objekt iz sourceset-a

    unsigned long long idMapCopy = 0;           //unikatni id za vsak objekt, pomozna variabla za undo

    //--- properties
    int xt=0,yt=0;                              // koordinate za mapo v tajlih
    int xInCell=0,yInCell=0;                    // zamik znotraj tile slota

    //--- suported parameters in editor
    int xInCellFixed=0,yInCellFixed=0;          // predefinirani zamik znotraj tile slota
    bool xInCellFixedSet = false, yInCellFixedSet = false;

    //--- transform
    bool mUniformScale = true;

    //---
    EShaderBasedProperties mExtraProperties;
    //jugi::openGL::UniformBlock *mImageVariableUniforms = nullptr;              // OWNED


    //---
    //AffineMat3f mTransform;
    std::array<Vec2f,4>mBox;
    //jugimap::BoundingBoxInt mOuterBoundingBox;

    bool mOnDeleteDebugCheck = true;

    //----
    //std::vector<AnimationInstance*>mEanimations;
    //bool mAnimated = false;

    //bool mInitializedForEditor = false;


    //int zOrder = 0;


    //--------------------------------------------------------------


    static Sprite* CreateMapCopy(Sprite *inobject, int _xt, int _yt, SpriteLayer *_Layer);
    static Sprite* CreateMapCopy(Sprite *inobject, int _xt, int _yt, int _xInCell, int _yInCell, SpriteLayer *_Layer);
    static Sprite* CreateMapCopy(SourceSprite *_SourceObject, int _xt, int _yt, SpriteLayer *_Layer);
    static Sprite* CreateFromSourceObject(SourceSprite *_SourceObject);

    //virtual void InitEngineObjectsForEditor(){}

    //void UpdateTransformMatrix();

    //void UpdatePredefinedVariablesFromParameters(bool _skipTransformUpdate = false);

    bool SetPosition(int _x, int _y, TGridChangeInfo &_GridChangeInfo, Map *_Map);   // transform operacija  _x,_y = pixel coordinata za FREE_PLACEMENT, tiles coordinata za GRID_PLACEMENT
    TSpriteProperties eSpriteProperties();
    void eSetProperties(const TSpriteProperties &_spriteProperties);

    //int GetX(){return (xt*CurrentProject.tileWidth + xInCell);}
    //int GetY(){return (yt*CurrentProject.tileHeight + yInCell);}
    int GetX();
    int GetY();
    void setPositionFromGridPosition(int flags=0);
    void setGridPositionFromPosition();

    Vec2f GetDrawnPosition(const ncine::Matrix4x4f &mParentTransform);
    jugimap::BoundingBoxInt GetDrawnOuterBoundingBox(const ncine::Matrix4x4f &mParentTransform);
    bool PointInBox(int _x, int _y);

    //void Draw(jugi::base::TGraphics &Graphics, const AffineMat3f &mParentTransform, int _depth = 0);
    //void ClearAnimatedMarks();
    //void ResetAnimatedProperties();

    //AffineMat3f GetAnimatedSpriteMatrixAndDraw2(jugi::base::TGraphics &Graphics, const AffineMat3f &mParentTransform);

    void DrawHandle(jugi::base::TGraphics &Graphics, const ncine::Matrix4x4f &mParentTransform);
    void DrawBoundingBox(jugi::base::TGraphics &Graphics, const ncine::Matrix4x4f &mParentTransform, int flags=0);
    void DrawOuterBoundingBox(jugi::base::TGraphics &Graphics, const ncine::Matrix4x4f &mParentTransform, int flags=0);
    void DrawCollisionShapes(jugi::base::TGraphics &Graphics, const ncine::Matrix4x4f &mParentTransform);
    BoundingBoxInt DrawParameters(jugi::base::TGraphics &Graphics, const ncine::Matrix4x4f &mParentTransform);

    //void CS_SetChildReplacementSourceObject(const std::string &_childName, SourceSprite *_sourceObjectReplacement);

    //-----



    Sprite();
    Sprite(const Sprite &_Object);
    virtual ~Sprite();


#else


    /// Constructor.
    Sprite(){}
    Sprite(const Sprite &s) = delete;
    Sprite& operator=(const Sprite &s) = delete;

public:

    /// \brief Destructor.
    ///
    /// **Important:** Do not delete sprites directly. Use SpriteLayer::RemoveAndDeleteSprite command:
    /// \code
    /// sprite->GetLayer()->RemoveAndDeleteSprite(sprite);
    /// \endcode
    virtual ~Sprite();

#endif








private:



};


//Sprite* castNodeToSprite(ncine::SceneNode *_node);
//ncine::SceneNode* castSpriteToSceneNode(jugimap::Sprite* _sprite);





//================================================================================



}





#endif // JMSPRITE_H
