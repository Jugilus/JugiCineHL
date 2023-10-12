#ifndef JM_SOURCE_CONTAINERS_H
#define JM_SOURCE_CONTAINERS_H

#include <string>
#include <vector>
#include "jmGlobal.h"


namespace jugimap {


class SourceFile;
class SourceImage;
class SourceSprite;
class CompositeSprite;
class SourceGroup;
struct TextLabel;
class Font;



class SourceSet
{
public:
    friend class SourceObjectsLoader_xml;
    friend class SourceObjectsWriter_xml;
    //friend void MakeGfxSets(oldStuff::TObjectData *LevelCfgData);


    //---------------------------------------------------------
    SourceSet(){}
    SourceSet(const std::string &_name);
    ~SourceSet();

    const std::string & name(){ return mName; }
    SourceGroup * parentSourceGroup(){ return mParentSourceGroup; }
    std::vector<SourceSprite*>& sourceSprites(){ return mSourceSprites; }
    std::vector<SourceFile*>& sourceFiles(){ return mSourceFiles; }

    SourceFile* getSourceFile(const std::string &_relativeFilePath);
    SourceImage* getSourceImage(const std::string &_relativeFilePath, const std::string &_imageName);
    SourceSprite* getSourceSprite(const std::string &_name);
    SourceFile* getTextsDummySourceFile();


    int saveID = 0;

#ifdef JUGI_EDITOR

    int zoom = 100;
    int setID = -1;     // za blitzmax legacy mape

    int actionFlags = 0;

    SourceSprite* eAddNewSourceObject(SpriteType _type, const std::string &_nameID);
    SourceSprite* eAddNewSourceObject(SpriteType _type, SourceImage *_SourceImage, const std::string &_nameID);
    SourceSprite* eInsertNewSourceObject(SpriteType _type, SourceImage *_SourceImage, const std::string &_nameID, int _index);

    //int DeleteSourceObjectsWithoutSourceImages();
    //int DeleteFlagedSourceObjects(int _flagMask);
    //int DeleteFlagedSourceImageFiles(int _flagMask);

    int eDeleteSourceSpritesMarkedForDelete();



    SourceFile* eGetSingleTileSheetSourceImageFile();

    //void GatherTextLabelsWithFont(std::vector<TextLabel*> &_textLabels, Font *_font);


    //void SetSourceImagesNotUsedFlag();

    //void SetObjectImagesTextureFlags(int _textureFlags);
    //void RebuildObjectImagesIfNeeded();
    void eResetActionFlagsAndTemporaryObjects();

    void _setParentSourceGroup(SourceGroup *_sourceGroup){ mParentSourceGroup = _sourceGroup;}
    void _setName(const std::string &_name) { mName = _name;}

#endif



private:
    std::string mName;
    SourceGroup * mParentSourceGroup = nullptr;                   // LINK to parent group

    std::vector<SourceSprite*>mSourceSprites;            //OWNED
    std::vector<SourceFile*>mSourceFiles;      //OWNED

};


//=================================================================================


class SourceGroup
{
public:
    friend class SourceObjectsLoader_xml;
    friend class SourceObjectsWriter_xml;

    //----
    SourceGroup(){}
    SourceGroup(const std::string &_nameID);
    ~SourceGroup();
    //void DeleteSets();

    void initSourceObjects();
    bool loadExternalData();
    void deleteExternalData();

    const std::string & name(){ return mName; }
    std::vector<SourceSet*> &sourceSets(){ return mSourceSets; }

    SourceSet* getSourceSet(const std::string &_nameID);
    SourceSprite* getSourceSprite(const std::string &_nameID);


    int saveID = -1;


#ifdef JUGI_EDITOR

    int actionFlags = 0;


    //bool loadTextures();

    //void DeleteSet(SourceSet* _SourceSet);



    SourceFile* eGetSourceFile(const std::string &_relativeFilePath);
    //SourceImage* GetSourceImage(const std::string &_relativeFilePath, const std::string &_imageName);

    bool eIsSourceFileUsedInSourceSprites(SourceFile* _sourceFile);
    void eSetSourceImagesUsedInSourceSpritesFlag();
    void eResetActionFlagsAndTemporaryObjects();
    //void SetSourceImagesTextureFlags(int textureFlags);
    //void RebuildImagesForSourceObjecIfNeeded();

    //int GetNextVersionNumberForSourceObjectName(const std::string &_baseName);

    //void eGatherSourceCompositeSprites(std::vector<CompositeSprite*> &_compositeSprites);

    void _setName(const std::string &_name) { mName = _name; }
    //void GatherTextLabelsWithFont(std::vector<TextLabel*> &_textLabels, Font *_font);

#endif


private:
    std::string mName;
    std::vector<SourceSet*>mSourceSets;          //OWNED

};


//=================================================================================


class SourceLibrary
{
public:
    friend class SourceObjectsLoader_xml;
    friend class SourceObjectsWriter_xml;


    std::vector<SourceGroup*>& sourceGroups(){ return mGroups; }
    void initSourceObjects();
    bool loadExternalData();
    bool loadGlobalExternalData();

    void deleteContent();
    SourceSprite* getSourceSprite(const std::string &_name);
    SourceGroup* getSourceGroup(const std::string &_name);
    SourceSprite *getSourceObjectViaSaveHelperIDs(int sourceGroupSaveID, int sourceSetSaveID, int sourceObjectSaveID);


#ifdef JUGI_EDITOR
    SourceFile* eGetSourceFile(const std::string &_relativeFilePath);
    bool eIsSourceFileUsedInSourceSprites(SourceFile* _sourceFile);
    void eSetSourceImagesUsedInSourceSpritesFlag();
    void eResetActionFlagsAndTemporaryObjects();
#endif


private:
    std::vector<SourceGroup*>mGroups;          //OWNED
};



struct SourceDataSaveHelper
{
    std::vector<std::string>groupNames;
    std::vector<std::string>setNames;
    std::vector<std::string>sourceSpriteNames;

    void clear();

    int indexForSourceSpriteName(const std::string &name);
    int indexForSetName(const std::string &name);
    int indexForGroupName(const std::string &name);

    std::string sourceSpriteName(int index);
    std::string sourceSetName(int index);
    std::string sourceGroupName(int index);

};


extern SourceDataSaveHelper saveHelper;
extern SourceLibrary sourceLibrary;


#ifdef JUGI_EDITOR

//--- globals

extern SourceSet* CurrentSourceSet;
//extern SourceGroup SourceSets;
//extern SourceGroup LoadedSourceSets;
extern SourceGroup* CurrentSourceGroup;


//--- functions
std::string GenerateUniqueSpriteNameFromImageName(SourceFile * _SourceImageFile, SourceImage *_SourceImage);


#endif

//----constants









}


#endif // JMSOURCECONTAINERS_H
