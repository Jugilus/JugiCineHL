#ifndef JUGIMAP_SPRITES_H
#define JUGIMAP_SPRITES_H

#include "ncine/Sprite.h"
#include "ncine/TextNode.h"
#include "ncine/Font.h"

#ifdef JUGI_EDITOR
#include "../jugi/general/tpixmap.h"
#endif

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
class TextBook;
class TextModifier;
//class Collider;
//struct AnimatedProperties;
//class Font;
//struct SourceSpriteReplacements;






//=======================================================================================


class TextSpriteNode : public ncine::TextNode
{
public:

   TextSprite * s = nullptr;                    // OWNED

   TextSpriteNode(TextSprite *_s);
   virtual ~TextSpriteNode() override;
   void transform() override;
   //void setTransformationBitsDirty();

};


///\ingroup MapElements
///\brief The TextSprite class defines the text sprite from JugiMap Editor.
class TextSprite : public Sprite
{
public:
    friend class TextSpriteNode;
    friend class MapLoader_bin;
    friend class SourceObjectsLoader_xml;

    ///\brief Constructor
    TextSprite();

    ~TextSprite();


    virtual void initEngineObjects() override;
    virtual void clearExternalDataLinks() override;
    void updateAllNodeParameters() override;
    //virtual void updateEngineObjects() override;
    virtual void copyProperties(Sprite *_sprite, int copyFlags) override;
    virtual bool pointInside(Vec2f _pos) override;


    void setHandle(Vec2f _handle) override;

    //------

    void setFont(Font *_font);


    /// Returns the font of this text sprite.
    Font* font(){ return mFont; }


    ///\brief Set the text string of this text sprite to the given _textString.
    ///
    /// \see text
    void setText(const std::string &_textString);


    ///\brief Returns the string of this text sprite.
    ///
    /// \see SetTextString
    std::string text(){ return mText; }


    ///\brief Set the color of this text sprite to the given _color.
    ///
    /// Only red, green and blue componenet will be used for the text color. Alpha component is set with *SetAlpha*.
    /// \see GetColor, SetAlpha
    void setColor(ColorRGBA _color);


    ///\brief Returns the color of this text sprite.
    ///
    /// \see SetColor
    ColorRGBA color(){ return mColor; }


    ///\brief Returns the relative handle of this text sprite.
    ///
    /// The relative handle has coordinates in range from 0.0 to 1.0.
    Vec2f relativeHandle(){ return mRelativeHandle; }


    ///\brief Returns the width of this text sprite.
    float width(){ return static_cast<TextSpriteNode*>(mNode)->width();}


    ///\brief Returns the height of this text sprite.
    float height(){ return static_cast<TextSpriteNode*>(mNode)->height();}


    /*
    ///\brief Set the relative handle of this text sprite.
    ///
    /// The relative handle has coordinates in range from 0.0 to 1.0. The (absolute) handle is obtained
    /// from relative handle by multiplying it with the sprite size. For text sprites we always specify relative handle as this ensure
    /// that text preserve its allignement if the text string changes.
    /// This function is called by other functions and should not be called directly.
    /// \see GetRelativeHandle
    */
    void setRelativeHandle(Vec2f _relHandle, bool adjustToPixelCoordinates=true);


    std::string bookName(){ return mBookName; }
    int textSegmentId(){ return mBookSegmentId; }


    bool setTextFromBook(const std::string _bookName, int _bookSegmentId);

    //virtual void setEngineNodesEnabled(bool _forceEnabled) override;
    //bool setFromSourceItemData();


protected:


private:
    Font* mFont = nullptr;               // LINK pointer
    std::string mText;
    ColorRGBA mColor{255,255,255,255};
    Vec2f mRelativeHandle{0.5, 0.5};
    bool mEmptyString = false;

    std::string mBookName;
    int mBookSegmentId = 0;

    //TextModifier *mTextModifier = nullptr;


    void onTransform();


//#ifdef JUGI_EDITOR

    // pixmap for selector - used for source composite sprites where we changed text string
//    jugi::base::TPixmap *mTextPixmap = nullptr;             // OWNED
//    std::string mTextPixmapString;
//    BoundingBoxInt mTextPixmapRect;                         // used when creating pixmap

//#endif

};


//=======================================================================================

///\ingroup MapElements
///\brief The SkeletonSprite class defines the skeleton sprite from JugiMap Editor.
class SkeletonSprite : public Sprite
{
public:
    friend class MapLoader_bin;

    /// Constructor.
    SkeletonSprite(){}

    virtual ~SkeletonSprite() override {}

    virtual void initEngineObjects() override;

};


extern ncine::TextNode *errorMessageLabel;     // LINK


}




#endif  // JUGIMAP_SPRITES_H
