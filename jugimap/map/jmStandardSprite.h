#ifndef JUGIMAP_STANDARD_SPRITE_H
#define JUGIMAP_STANDARD_SPRITE_H

#include "ncine/Sprite.h"
#include "ncine/TextNode.h"
#include "ncine/Font.h"

//#include "jmSourceSprite.h"
#include "jmCommon.h"
#include "jmParameter.h"
#include "jmColor.h"
#include "jmGlobal.h"
#include "jmSprite.h"



namespace jugi {
class TEObject;
}


namespace jugimap {


//class JugiMapBinaryLoader;
//class ESourceImage;
//class Layer;
//class ESpriteLayer;
//class ESourceObject;
//class ComposedSprite;
class StandardSprite;
class TextSprite;
//class Collider;
//struct AnimatedProperties;
//class Font;
//struct SourceSpriteReplacements;



class StandardSpriteNode : public ncine::Sprite
{
public:

   StandardSprite * s = nullptr;                    // OWNED

   StandardSpriteNode(StandardSprite *_s);
   virtual ~StandardSpriteNode() override;
   void transform() override;

};


///\ingroup MapElements
///\brief The StandardSprite class defines the sprite from JugiMap Editor.
class StandardSprite  : public Sprite
{
public:
    friend class StandardSpriteNode;
    friend class MapLoader_bin;
    friend class SourceObjectsLoader_xml;

    /// The physics mode of the standard sprite.
    //enum PhysicsMode{
    //    NO_PHYSICS=0,       ///< Not included in the physics simulation.
    //     STATIC=1,           ///< Included in the physics simulation as a static object.
    //     DYNAMIC=2,          ///< Included in the physics simulation as a dynamic object.
    //     KINEMATIC=3         ///< Included in the physics simulation as a kinematic object.
    // };


    /// Constructor.
    StandardSprite();


    virtual void initEngineObjects() override;
    //virtual void updateEngineObjects() override;
    virtual void clearExternalDataLinks() override;



    virtual bool hasCollider() override;
    virtual bool pointInside(Vec2f _pos) override;
    virtual bool overlaps(jugimap::Sprite *_sprite) override;
    virtual bool raycastHit(Vec2f _rayStart, Vec2f _rayEnd, Vec2f &_hitPos) override;


    void setHandle(Vec2f _handle) override;

    void setVisible(bool _visible) override;


    /// \brief Returns the active image.
    SourceImage * activeImage() {return mActiveImage;}


    /// \brief Set the active image to the given *_image*.
    ///
    /// **Important:** The *_image* must be one of the images within the source sprite's graphic items.
    /// The *_image* can also be nullptr in which case the sprite will not be visible.
    virtual void setActiveImage(SourceImage *_image);


    /// \brief Returns the index of the active image in the source sprite's graphic items vector.
    ///
    /// If the active image is nullptr it returns -1.
    int activeImageFrameIndex();


    /// \brief Set the active image to the image at the given *_frameIndex* in the source sprite's graphic items vector.
    ///
    /// The index must be inside the vector's bounds!
    void setActiveImageByFrameIndex(int _frameIndex);



    /// \brief Set collider to the given *_collider*.
    ///
    /// This function is called at the sprite initialization.
    void setCollider(Collider *_collider){ mCollider = _collider; }


    /// \brief Returns the collider or nullptr if none.
    Collider * collider(){ return mCollider; }


    /// \brief Returns the physics mode.
    ///
    /// This function can be used with engines which provides a physics simulation engine.
    //virtual PhysicsMode GetPhysicsMode(){ return physicsMode; }


    /// \brief Set the physics mode to the given *_physicsMode*.
    ///
    /// This function can be used with engines which provides a physics simulation engine.
    //virtual void SetPhysicsMode(PhysicsMode _physicsMode){ physicsMode = _physicsMode;}


    virtual void applyAnimatedProperties(AnimatedProperties& _ap) override;


    //virtual void setEngineNodesEnabled(bool _forceEnabled) override;


protected:

    /// Destructor.
    virtual ~StandardSprite();

    void setColliderShapes();


private:
    SourceImage *mActiveImage = nullptr;                     // LINK to currently active (displayed) image from the source sprite 'graphicItems' list.
    Collider *mCollider = nullptr;                           // OWNED
    bool mPreserveShapesOnNullActiveImage = true;
    //PhysicsMode physicsMode = PhysicsMode::NO_PHYSICS;



    void onTransform();

};


//=======================================================================================

extern ncine::Sprite *dbgNcSprite;



}




#endif  // JUGIMAP_SPRITES_H
