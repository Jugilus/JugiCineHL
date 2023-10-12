#ifndef JM_SOURCE_ITEM_H
#define JM_SOURCE_ITEM_H

#ifdef JUGI_EDITOR
#include "../jugi/general/tpixmap.h"
#include "../jugi/general/baseGlobals.h"
#endif

#include <ncine/Texture.h>
#include "jmCommon.h"
#include "jmParameter.h"
#include "jmGlobal.h"
#include "jmColor.h"
#include "jmUsingDeclarations.h"




namespace jugi{
namespace base {
//class TPixmap;
}
namespace openGL {
class Image;
}
}




namespace jugimap {


class Font;
class SourceFile;
class VectorShape;
struct TextLabel;
struct Color;
struct CustomTextPixmap;


/// \ingroup MapElements
/// \brief The GraphicItem class defines the equivalent object from JugiMap Editor.
///
/// A graphics item stores parameters related to graphic items (images, texts, skeleton sprites).
class SourceImage
{
public:
    friend class MapLoader_bin;
    friend class SourceObjectsLoader_xml;
    friend class DummySourceObjects;
    friend class SourceFile;
    //friend class SourceSprite;
    //friend class SourceFile;
    //friend class StandardSprite;
    //friend class SkeletonSprite;

    //static SourceImage dummy_missingSourceSprite;


    /// \brief Initialize this graphic item.
    ///
    /// This function is called by SourceSprite::Init and should not be called manually.
    //virtual void init();


    /// Returns the graphic file of this graphic item.
    SourceFile* parentSourceFile() {return mSourceFile;}

    /// Returns the name of this graphic item.
    const std::string & name() {return mName;}

    /// Returns the position of sub-image if this graphic item belong to an image file.
    Vec2i subimagePosition() {return mRect.min;}

    /// Returns the width of sub-image if this graphic item belongs to an image file.
    int width() {return mRect.GetWidth();}

    /// Returns the height of sub-image if this graphic item belongs to an image file.
    int height() {return mRect.GetHeight();}

    /// Returns the size of sub-image if this graphic item belongs to an image file.
    Vec2i size() {return mRect.GetSize();}

    BoundingBoxInt subimageRect(){ return mRect; }

    /// Returns the handle of sub-image if this graphic item belongs to an image file.
    Vec2i handle(){ return mHandle; }

    /// Returns the spine scale if this graphic item belongs to a Spine file.
    float spineScale() {return mSpineScale;}

    /// Returns a reference to sprite shapes if this graphic item belong to an image file.
    std::vector<VectorShape*>& spriteShapes() {return mSpriteShapes;}

    /// Returns a reference to probe shapes if this graphic item belong to an image file.
    std::vector<VectorShape*>& extraShapes() {return mExtraShapes;}

    /// Returns a reference to probe shapes if this graphic item belong to an image file.
    std::vector<VectorShape*>& shapes() {return mShapes;}


    TextLabel * textLabel(){ return mTextLabel; }

    virtual bool LoadExternalData();

    virtual void DeleteExternalData();

    void rebuildQuickAccessShapeContainers();

    void revertPositioningFromNcineAncher(Vec2f &p);


    /// Constructor.
    SourceImage(){}

    SourceImage(const SourceImage &gi) = delete;
    SourceImage & operator=(const SourceImage &gi) = delete;

    /// Destructor.
    virtual ~SourceImage();


private:
    SourceFile *mSourceFile = nullptr;             //LINK pointer to parent file.
    std::string mName;

    // (sub)image rectangle
    BoundingBoxInt mRect;


    Vec2i mHandle;

    std::vector<VectorShape*>mSpriteShapes;          // LINKs from mShapes
    std::vector<VectorShape*>mExtraShapes;           // LINKs from mShapes
    std::vector<VectorShape*>mShapes;                // OWNED

    // text
    TextLabel *mTextLabel = nullptr;

    // spine
    float mSpineScale = 1.0;

    long long int listOrder = 0;

    int saveID = -1;

//-----------------------------------------------------------------------------------------
#ifdef JUGI_EDITOR

public:
    friend class MapWriter_bin;
    friend class SourceObjectsWriter_xml;
    friend class ProjectMaintenance;
    friend class jugi::TSpriteSetupEngine_ImagesCollisionShapes;
    friend class jugi::TSpriteSetupEngine_ImagesHandlePoint;
    friend class jugi::TSpriteSheetSubimagesAppEngine;
    friend class jugi::gui::TDockContent_TextLabel;
    friend class jugi::gui::TAdjustTilesetDialog;
    friend class jugi::gui::TSpriteSheetSubimagesDialog;
    friend class jugi::gui::TPropertiesDock;
    friend class jugi::gui::TSourceSpritesLoad;
    friend struct jugi::gui::TSourceImageFileLoader;
    friend void oldStuff::_MakeSets_StandardSprites_LegacyLSDformat(std::vector<oldStuff::TObjectData *> &, const std::string &);
    friend void oldStuff::_MakeSets_ComposedSprites_LegacyLSDformat(std::vector<oldStuff::TObjectData *> &, const std::string &);
    //friend class CompositeSprite;


    static jugi::base::TPixmap BufferPixmap;
    //---
    std::string spriterAnimationName;               // spriter nima default poze -> snapshot pixmap izvedemo za izbrano animacijo

    VectorShape *BBShape = nullptr;        // rabimo za sprite sheet editor
    int actionFlags = 0;


    //----
    jugi::base::TPixmap * GetPixmap();
    jugi::base::TPixmap * GetTextPixmap(const std::string &_text);
    void SetPixmap(jugi::base::TPixmap *_Pixmap);

    void DeletePixmap();
    bool LoadPixmap();
    void RebuildImage();

    bool loadPixmapNew();

    void MakeEmptyBBShape();
    void MakeBBShape();
    void DeleteBBShape();
    void VerifyAndUpdateDimensionsFromBBShape();

    //void SetGeneratedPixmap(jugi::base::TPixmap *_Pixmap);









private:

    jugi::base::TPixmap *Pixmap = nullptr;            // OWNED




#endif

};



#ifdef JUGI_EDITOR


struct CustomTextPixmap
{

    jugi::base::TPixmap *pixmap = nullptr;
    std::string pixmapText;
    BoundingBoxInt mRect;

    ~CustomTextPixmap();



};


#endif



struct TextLabel
{
public:


    TextLabel(){}
    TextLabel(TextLabel &src);



    bool operator==(const TextLabel &_tl)
    {
        return (font==_tl.font && color==_tl.color && handle==_tl.handle);
    }

    bool operator!=(const TextLabel &_tl)
    {
        return !(operator==(_tl));
    }


    //void GetHandle(int width, int height, int &xHandle, int &yHandle);




    //std::string text;
    Font* font = nullptr;                                 // LINK
    Color* color = nullptr;                               // LINK
    TextHandleVariant handle = TextHandleVariant::CENTER;


#ifdef JUGI_EDITOR

    ~TextLabel();

    std::vector<CustomTextPixmap*>childTextPixmaps;     // OWNED

    CustomTextPixmap* obtainChildTextPixmap(const std::string &_pixmapText);
    jugi::base::TPixmap* getChildTextPixmap(const std::string &_pixmapText);
    bool isChildTextPixmapUpdateRequired();
;
#endif

};


//extern ESourceImage *SIEmptyComposedSpriteDummy;
//extern ESourceImage *SIEmptyFrame;
//extern ESourceImage *SIMissingSourceObject;
//extern ESourceImage *SIMissingTexture;



}


#endif // JMSOURCEITEM_H
