#ifndef JMSOURCEFILE_H
#define JMSOURCEFILE_H


#include "jmCommon.h"
#include "jmParameter.h"
#include "jmGlobal.h"
#include "jmUsingDeclarations.h"


namespace ncine{
class Texture;
class Font;
}

namespace jugi{
namespace base {
class TPixmap;
}
namespace openGL {
class Image;
}
}


namespace jugimap {

class TSpineSourceSprite;
class TSpriterSourceSprite;
class Font;

class SourceImage;





/// \ingroup MapElements
/// \brief The GraphicFile class defines the equivalent object from JugiMap Editor.
///
/// A graphic file stores GraphicItem objects and is their owner.
class SourceFile
{
public:
    friend class MapLoader_bin;
    friend class SourceObjectsLoader_xml;
    friend class DummySourceObjects;
    //friend class SourceImage;
    //friend class StandardSprite;
    //friend class SkeletonSprite;
    //friend class SourceGraphics;

    //static std::string pathPrefix;          ///< An optional string attached to stored file paths. It must end with slash '/'!

    //static ESourceFile dummyFile;


    /// \brief Initialize this GraphicFile.
    ///
    /// This function is called by GraphicItem::Init and should not be called manually.
    //virtual void init();


    /// \brief Returns the kind of this file.
    FileKind kind() {return mKind;}


    /// \brief Returns the relative file path of this file.
    //std::string relativeFilePath() {return pathPrefix + mRelativeFilePath;}
    const std::string & relativeFilePath() {return mRelativeFilePath;}

    /// \brief Returns the size of image if this file is an image.
    //Vec2i size() {return mSize;}


    /// \brief Returns a reference to the vector of stored graphic items in this file.
    std::vector<SourceImage*>& sourceItems() {return mSourceImages;}


    /// \brief Returns the relative file path of the spine atlas if this file is a Spine file.
    //std::string spineAtlasRelativeFilePath() {return pathPrefix + mSpineAtlasRelativeFilePath;}


    /// \brief Returns the nCine texture object.
    ncine::Texture *texture() {return mTexture;}


    bool isExternalDataLoaded(){ return mExternalDataLoaded; }


    SourceImage* AddSourceImage(const std::string &_name, int _x, int _y, int _width, int _height);

    SourceImage* GetSourceImage(const std::string &_name);

    bool CreateTileSheetSourceImages();

    void initSourceObjects();
    bool LoadExternalData();

    void DeleteExternalData();

    //void SetImagesTextureFlags(int flags);



//protected:

    /// Constructor.
    //SourceFile(){}
    SourceFile(FileKind _kind): mKind(_kind){}

    SourceFile(const SourceFile &gf) = delete;
    SourceFile & operator=(const SourceFile &gf) = delete;

    /// Destructor.
    virtual ~SourceFile();



private:
    FileKind mKind = FileKind::NOT_DEFINED;
    std::string mRelativeFilePath;
    std::vector<SourceImage*>mSourceImages;                 // OWNED objects


    //  IMAGE

    //--- size
    int mWidth = -1;
    int mHeight = -1;

    //--- spriteSheet
    int mSheetGridSpacingX = -1;
    int mSheetGridSpacingY = -1;

    //--- tileSheet
    int mTileWidth = -1;
    int mTileHeight = -1;
    int mTilesHorizontalSpacing = 0;
    int mTilesVerticalSpacing = 0;
    int mTilesLeftOffset = 0;
    int mTilesTopOffset = 0;
    int mNumTilesX = -1;
    int mNumTilesY = -1;

    ncine::Texture *mTexture = nullptr;    // OWNED

    bool mExternalDataLoaded = false;


    //--- za spine image
    std::string mSpineAtlasRelativeFilePath;

    //--- spriter

    //---
    int saveID = -1;


#ifdef JUGI_EDITOR
public:
    friend class SourceObjectsWriter_xml;
    friend class jugi::gui::TAdjustTilesetDialog;
    friend class jugi::gui::TSpriteSheetSubimagesDialog;
    friend class jugi::gui::TDockContent_TextLabel;
    friend class jugi::gui::TSourceSpritesLoad;
    friend struct jugi::gui::TSourceImageFileLoader;
    friend class jugi::gui::TTilesetDisplayWidget;
    friend void oldStuff::MakeGfxSets(oldStuff::TObjectData *);
    friend void oldStuff::_MakeSets_StandardSprites_LegacyLSDformat(std::vector<oldStuff::TObjectData *> &, const std::string &);
    friend void oldStuff::_MakeSets_ComposedSprites_LegacyLSDformat(std::vector<oldStuff::TObjectData *> &, const std::string &);
    friend struct jugi::TTiledImporter;
    friend struct jugi::TTiledExporter;


    int actionFlags = 0;


    void eResetActionFlagsAndTemporaryObjects();

    int width(){ return mWidth; }
    int height(){ return mHeight; }
    int tileWidth(){ return mTileWidth; }
    int tileHeight(){ return mTileHeight; }
    int numTilesX(){ return mNumTilesX; }
    int numTilesY(){ return mNumTilesY; }
    jugi::base::TPixmap * pixmap(){ return mPixmap; }
    TSpineSourceSprite *spineSourceSprite(){ return mSpineSourceSprite;}
    TSpriterSourceSprite* spriterSourceSprite(){ return mSpriterSourceSprite; }


private:
    TSpineSourceSprite *mSpineSourceSprite = nullptr;
    TSpriterSourceSprite *mSpriterSourceSprite = nullptr;
    jugi::base::TPixmap *mPixmap = nullptr;
    std::string mAbsoluteFilePath;


    bool eLoadPixmap();
    //void MakeSourceSpritesPixmaps();
    void eDeletePixmap();
    //bool MakeTileSheetSourceImages(bool _makeTilePixmaps);
    //int DeleteFlagedSourceImages(int _flagMask);


#endif

};



//extern ESourceFile *SIFDummy;


}







#endif // JMSOURCEFILE_H
