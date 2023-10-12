#ifndef JUGIMAP_COMPOSED_SPRITE_H
#define JUGIMAP_COMPOSED_SPRITE_H

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


class MapLoader_bin;
class SourceImage;
class Layer;
class SpriteLayer;
class SourceSprite;
class CompositeSprite;
//class StandardSprite;
class TextSprite;
class Collider;
struct AnimatedProperties;
class Font;
//struct SourceSpriteReplacements;




/*
class ComposedSpriteNode : public ncine::SceneNode
{
public:

   EComposedSprite * s = nullptr;

   ComposedSpriteNode(EComposedSprite *_s);
   virtual ~ComposedSpriteNode() override;
   nctl::Array<ncine::SceneNode*> &getChildren(){ return children_; }
   void transform() override;

};
*/

///\ingroup MapElements
///\brief The ComposedSprite class defines the composed sprite from JugiMap Editor.
///
/// A composed sprite stores Layer objects and is their owner.
class CompositeSprite : public Sprite
{
public:
    //friend class TComposedSpritesManager;
    friend class SourceObjectsLoader_xml;
    friend class MapLoader_bin;
    //friend class Sprite;
    //friend class StandardSprite;
    //friend class SkeletonSprite;
    //friend class ComposedSpriteNode;

    /// Constructor.
    CompositeSprite();

    //ComposedSpriteNode* node();
    /// Destructor.
    virtual ~CompositeSprite() override;

    virtual void initEngineObjects() override;
    virtual void clearExternalDataLinks() override;
    //virtual void updateEngineObjects() override;

    virtual void updatePropertiesViaExtraParameters() override;


    void setHandle(Vec2f _handle) override;

    virtual void updateBoundingBox() override;
    BoundingBoxFloat boundingBox() override;
    BoundingBoxInt boundingBoxInt() override;
    //virtual void setVisible(bool _visible) override;
    virtual bool hasCollider() override;
    virtual bool pointInside(Vec2f _pos) override;
    virtual bool overlaps(Sprite *_sprite) override;
    virtual bool raycastHit(Vec2f _rayStart, Vec2f _rayEnd, Vec2f &_hitPos) override;

    virtual void resetAnimatedProperties() override;

    Vec2f positionFromAbsPosition(const Vec2f &_pos);

    nctl::Array<ncine::SceneNode*> &mapNodeChildren();

//{ return static_cast<ComposedSpriteNode*>(mNode)->getChildren();}


    void ObtainNestingDepth();

    Map *map(){ return mMap; }

    //ESourceObject *mSourceSprite = nullptr;         // link na pripadajoči source objekt





    static void copyLayers(CompositeSprite *csSource, CompositeSprite *csDest);

    void copyProperties(Sprite *_sprite, int copyFlags) override;

    //void setEngineNodesEnabled(bool _enableAll) override;

    //ComposedSprite();
    //TEComposedSprite(const std::string &_name);

    //CompositeSprite(const CompositeSprite &_cs);

    //~ComposedSprite();

    Vec2f handleForMapCompositeSprites(){ return mHandleForMapCompositeSprites; }


    int mNestingDepth = 0;
    Vec2f mHandleForMapCompositeSprites;


#ifdef JUGI_EDITOR

public:

    //void eInitDummySourceImage();
    //bool IsEmpty();



    void eInitEngineObjects();
    void eRebuildOnChangedSourceCompositeSprite();

    //jugi::base::TPixmap * RenderComposedSpriteToPixmap();
    void eNormalizeToZero();
    void eObtainHandleForMapCompositeSprites();
    void eAddEmptyCompositeSpriteDummyChild();

    void eSetChildTextSpritePixmaps(std::vector<SourceImage*>&_sourceItemsForChildTextPixmapsCreation);
    void eGatherSourceItemsWithCustomChildTextPixmaps(std::vector<SourceImage*>&_sourceItemsWithCustomChildTextPixmaps);
    bool eContainChildTextWithCustomString(TextLabel *_textLabel, const std::string &_childTextString);


#endif






protected:

    //static void copyLayers(CompositeSprite *csSource, CompositeSprite *csDest, int &zOrder);


private:

    // misc - for construction purposes
    //Vec2i mTileSize;
    //Vec2i mNumTiles;
    Map *mMap = nullptr;

    //----

    //std::string mapmName;



    //---
    //Vec2i mSize;
    std::string mSourceSetName;


};





}




#endif  // JUGIMAP_SPRITES_H
