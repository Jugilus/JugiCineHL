#ifndef JUGIMAP_SOURCE_GRAPHICS_H
#define JUGIMAP_SOURCE_GRAPHICS_H


#include "jmCommon.h"
#include "jmParameter.h"
#include "jmGlobal.h"
#include "jmUsingDeclarations.h"

#include "jmSourceItem.h"
#include "jmSourceFile.h"


namespace ncine{
class Texture;
}


namespace jugimap {



/// \ingroup MapElements
/// \brief The SourceSprite class defines the source sprite from JugiMap Editor.
///
/// A source sprite stores common sprite data and parameters which can be shared by all Sprite objects of the same source sprite.
/// For StandardSprite objects these data includes textures, shapes, animations etc.
class SourceSprite
{
public:
    friend class MapLoader_bin;
    friend class SourceObjectsLoader_xml;
    friend class SourceDataSaveHelper;
    friend class DummySourceObjects;
    //friend class StandardSprite;
    //friend class SkeletonSprite;




    //static ESourceObject dummy_missingSourceSprite;


    /// The status flags for standard source sprites (SpriteKind::STANDARD).
    enum Status{
        NONE = 0,
        HAS_IMAGES_OF_DIFFERENT_SIZE = 1,                       ///< Has images of different size.
        HAS_IMAGES_WITH_SPRITE_SHAPES = 2,                      ///< Has image(s) with defined sprite shapes.
        HAS_IMAGES_WITH_DIFFERENT_SPRITE_SHAPES = 4,         ///< Has more images and they do not have all the same sprite shapes.
        HAS_MULTIPLE_SPRITE_SHAPES_IN_IMAGE = 8                 ///< The source sprite has image(s) with multiple sprite shapes.
    };


    /// \brief Initialize this source sprite.
    ///
    /// This function is called by Sprite::InitEngineSprite and should not be called manually.
    void init();



    SpriteType kind() {return mKind;}

    SourceSet* parentSourceSet(){ return mParentSourceSet; }

    /// \brief Returns the name of this source sprite.
    const std::string & name() {return mName;}


    /// \brief Returns a reference to the vector of stored constant parameters in this source sprite.
    CParameters& constantParameters(){return mConstantParameters;}

    CParameters& parameters(){return mParameters;}

    /// Returns a reference to the vector of source images in this source sprite.
    std::vector<SourceImage*>& sourceImages(){return mSourceImages;}


    /// Returns the status flags.
    int statusFlags() {return mStatusFlags;}


    /// Returns a reference to the vector of stored frame animations in this source sprite.
    std::vector<FrameAnimation*>& frameAnimations(){return mFrameAnimations;}


    /// Returns a reference to the vector of stored timeline animations in this source sprite.
    std::vector<TimelineAnimation*>& timelineAnimations(){return mTimelineAnimations;}


    /// Returns a pointer to the owned source composed sprite or nullptr if there is none.
    CompositeSprite* sourceCompositeSprite(){return mSourceCompositeSprite;}


    bool isInitialized(){ return mInitialized; }


    //void* mUserData = nullptr;


private:
    SpriteType mKind = SpriteType::NOT_DEFINED;
    SourceSet * mParentSourceSet = nullptr;          // LINK na sourceset

    std::string mName;
    std::vector<SourceImage*>mSourceImages;                     // LINKs

    std::vector<FrameAnimation*>mFrameAnimations;               // OWNED
    std::vector<TimelineAnimation*>mTimelineAnimations;         // OWNED
    CompositeSprite * mSourceCompositeSprite = nullptr;           // OWNED

    CParameters mConstantParameters;
    CParameters mParameters;



    int mStatusFlags = Status::NONE;
    bool mInitialized = false;

    int saveID = -1;


#ifdef JUGI_EDITOR

public:
    friend class MapWriter_bin;
    friend class SourceObjectsWriter_xml;
    friend class TCompositeSpritesManager;
    friend class jugi::TSpriteSetupEngine_MultiFrameSprites;
    friend class jugi::gui::TDockContent_TextLabel;
    friend void oldStuff::_MakeSets_StandardSprites_LegacyLSDformat(std::vector<oldStuff::TObjectData *> &, const std::string &);
    friend void oldStuff::_MakeSets_ComposedSprites_LegacyLSDformat(std::vector<oldStuff::TObjectData *> &, const std::string &);
    friend bool jugi::Load_LegacyLSDfile(const std::string &);
    friend void MakeSourceSetImagesLayer(int);
    friend SpriteLayer *MakeSourceSpriteLayer(SourceSprite *);


    SourceImage * markerImage(){ return mMarkerImage; }


    SourceImage *mMarkerImage = nullptr;       // LINK
    int actionFlags = 0;
    //QRectF Rect;

    //---
    int tiledGID=0;                     // za tiled mape
    int setID = -1;                     // za blitzmax legacy mape


    int width();
    int height();
    //FileKind sourceFileKind();
    //void RebuildImagesIfNeeded();
    //void SetImagesTextureFlags(int _textureFlags);
    //void RebuildTextLabel();


#endif

 // --------------------------------------------------------------------------------------
public:

    /// Constructor.
    SourceSprite(SpriteType _kind) : mKind(_kind){}
    SourceSprite(SpriteType _kind, SourceSet* _SourceSet, const std::string &_nameID) : mKind(_kind),mParentSourceSet(_SourceSet),mName(_nameID){}


    SourceSprite(const SourceSprite &ss) = delete;
    SourceSprite & operator=(const SourceSprite &ss) = delete;

    /// Destructor.
    ~SourceSprite();

};





struct ChildSourceObjectReplacement
{
    std::string childName;
    SourceSprite *sourceObjectReplacement = nullptr;        // LINK
    SourceSprite *originalSourceObject = nullptr;           // LINK
    //int sourceObjectReplacement_sourceGroupSaveID = -1;
    //int sourceObjectReplacement_sourceSetSaveID = -1;
    //int sourceObjectReplacement_sourceObjectSaveID = -1;

    ChildSourceObjectReplacement(){}
    ChildSourceObjectReplacement(const std::string &_childName, SourceSprite * _sourceObjectReplacement, SourceSprite* _originalSourceObject)
                                    : childName(_childName),
                                      sourceObjectReplacement(_sourceObjectReplacement),
                                      originalSourceObject(_originalSourceObject)
    {}


    static SourceSprite* FindChildSourceSprite(std::vector<ChildSourceObjectReplacement>&_replacements, const std::string &_childSpriteName);

};




/*
/// \ingroup MapElements
/// \class SourceGraphics
/// \brief The graphics storage.
///
/// The SourceGraphics class represents source graphics elements from JugiMap Editor.
/// It stores and owns SourceSprite objects and GraphicFile objects.
///
/// The following code will create a SourceGraphics object and load data from a file into it.
/// \code
///    SourceGraphics* sourceGraphics = new SourceGraphics();
///    if(JugiMapBinaryLoader::LoadSourceGraphics(sourceGraphicsFilePath, sourceGraphics)){
///        // loading successful !
///    }
/// \endcode
///
/// **Important:** The JugiMapBinaryLoader::LoadSourceGraphics function does **not** load any actual images or other files.
/// It only loads from the source graphics file various parameters and smaller bits of data. Actual loading of graphic data is
/// carried out at the initialization of engine objects during map initialization.
*/

}






#endif
