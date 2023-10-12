#include <memory>
#include <ncine/FileSystem.h>
#include "pugixml/pugixml.hpp"

#ifdef JUGI_EDITOR
#include "../jugi/general/tstream.h"
#include "../jugi/eFramework/map/eVectorShape.h"
//#include "../jugi/tools/spineWrapper.h"
//#include "../jugi/tools/spriterWrapper.h"
#endif

#include "jmSystem.h"
#include "jmCommon.h"
#include "jmSourceContainers.h"
#include "jmSourceSprite.h"
#include "jmCommonFunctions.h"
#include "jmGlobal.h"
#include "jmSystemSourceObjects.h"
#include "jmSourceItem.h"
#include "jmSpriteLayer.h"
#include "jmVectorLayer.h"
#include "jmSprite.h"
#include "jmStandardSprite.h"
#include "jmCompositeSprite.h"
#include "jmTextSprite.h"
#include "jmMap.h"
#include "jmVectorShape.h"
#include "jmFrameAnimation.h"
#include "jmTimelineAnimation.h"
#include "jmStreams.h"
#include "jmFont.h"
#include "jmColor.h"
#include "jmGuiText.h"
#include "jmSourceObjectsLoader_xml.h"



namespace jugimap {



void SourceObjectsLoader_xml::loadSourceGroups()
{


    //yOrientation = Yorientation::DOWN;          // starting state
    yOrientation = Yorientation::UP;          // starting state

    SourceLibrary &dbgSourceLibrary = sourceLibrary;


    std::string dirPath = CurrentProject.sourceCfgDir;

    if(ncine::FileSystem::isDirectory(dirPath.c_str())==false){
        assert(true);
    }

    ncine::FileSystem::Directory dir(dirPath.c_str());

    //QDir dir(QString::fromStdString(dirPath));
    //assert(dir.exists());

    //if(systemSourceObjectsCreated==false){
    //    systemSourceObjects.create();
    //    systemSourceObjectsCreated = true;
    //}


#ifdef JUGI_EDITOR
    CurrentSourceGroup = nullptr;
    CurrentSourceSet = nullptr;
#endif

    //if(globalSourceObjectsCreated==false){
    //    loadGlobalSourceObjects();
    //    globalSourceObjectsCreated = true;
    //}



    //bool first = false;

    //_LoadSetsFromCfgDir(CurrentProject.sourceCfgDir, ComposedSprites);
    //for(const QFileInfo &info : dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot)){


    while(true){
        const char *_fileName = dir.readNext();
        if(_fileName==nullptr){
            break;
        }
        std::string fileName(_fileName);
        if(fileName.empty()==false && fileName.substr(0, 1)=="."){
             continue;
        }
        std::string filePath = dirPath + "/" + fileName;
        if(ncine::FileSystem::isDirectory(filePath.c_str())){
            std::string groupName = StdString::baseNameFromFilePath(filePath);

//#ifdef JUGI_EDITOR
//            if(first==false){
//                SourceSets.mName = groupName;
//                SourceGroup * group = &SourceSets;
//                sourceLibrary.sourceGroups().push_back(group);
//                loadSourceGroup(group);
//                first = true;
//
//            }else{
                SourceGroup * group = new SourceGroup(groupName);
                sourceLibrary.sourceGroups().push_back(group);
                loadSourceGroup(group);
//            }
//
//#else
//            SourceGroup * group = new SourceGroup(groupName);
//            sourceLibrary.sourceGroups().push_back(group);
//            loadSourceGroup(group);
//#endif

        }
    }



#ifdef JUGI_EDITOR

    // select current group and set
    if(sourceLibrary.sourceGroups().empty()==false){

        int nSetsMax = 0;
        for(SourceGroup* sg : sourceLibrary.sourceGroups()){
            if(sg->sourceSets().size()>=nSetsMax){
                nSetsMax = sg->sourceSets().size();
                CurrentSourceGroup = sg;
            }
        }

        int nObjectsMax = 0;
        for(SourceSet* ss : CurrentSourceGroup->sourceSets()){
            if(ss->sourceSprites().size()>=nObjectsMax){
                nObjectsMax = ss->sourceSprites().size();
                CurrentSourceSet = ss;
            }
        }

    }else{
        CurrentSourceGroup = nullptr;
        CurrentSourceSet = nullptr;
    }
#endif

}


void SourceObjectsLoader_xml::loadGlobalAndSystemSourceObjects()
{

    systemSourceObjects.create();
    loadGlobalSourceObjects();

}


void SourceObjectsLoader_xml::loadGlobalSourceObjects()
{

    std::string filePath = CurrentProject.sourceCfgDir + "/glb_common.xml";
    //QString filePath = QString("%1/glb_common.%2").arg(CurrentProject.sourceCfgDir).arg(cfgFileSuffix);

    FontsLibrary &fontLibraryTMP = fontsLibrary;

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filePath.c_str());


    if(result.status==pugi::xml_parse_status::status_ok){

        pugi::xml_node rootNode = doc.child("commonData");
        if(rootNode.empty()==false){
            for (pugi::xml_node n = rootNode.first_child(); n; n = n.next_sibling()){

                if(std::string(n.name())=="fontStyles"){
                    /*
                    for (pugi::xml_node nFont = n.first_child(); nFont; nFont = nFont.next_sibling()){

                        if(std::string(nFont.name())=="fontStyle"){

                            FontStyle *ts = new FontStyle();

                            ts->id = nFont.attribute("id").as_int(0);
                            ts->nameID = nFont.attribute("name").as_string("Font style");
                            ts->familyName = nFont.attribute("font").value();
                            ts->size = nFont.attribute("size").as_int(16);
                            ts->italic = nFont.attribute("italic").as_bool(false);
                            ts->bold = nFont.attribute("bold").as_bool(false);

                            fontStyles.styles.push_back(ts);
                        }
                    }
                    */

               }else if(std::string(n.name())=="fonts"){

                        for (pugi::xml_node nFont = n.first_child(); nFont; nFont = nFont.next_sibling()){

                            if(std::string(nFont.name())=="font"){

                                Font *f = new Font();
                                f->mName = nFont.attribute("name").as_string("font");
                                f->mRelativeFilePath = nFont.attribute("file").value();
                                fontsLibrary.fonts().push_back(f);
                            }
                        }

                }else if(std::string(n.name())=="colorStyles"){

                    /*
                    for (pugi::xml_node nColor = n.first_child(); nColor; nColor = nColor.next_sibling()){

                        if(std::string(nColor.name())=="colorStyle"){

                            ColorStyle *tc = new ColorStyle();

                            tc->id = nColor.attribute("id").as_int(0);
                            std::string sColor = nColor.attribute("color").value();
                            tc->color = ColorRGBA::ParseFromHexString(sColor);
                            colorStyles.colors.push_back(tc);
                        }
                    }
                    */

                }else if(std::string(n.name())=="colors"){

                    for (pugi::xml_node nColor = n.first_child(); nColor; nColor = nColor.next_sibling()){

                        if(std::string(nColor.name())=="color"){

                            Color *c = new Color();
                            c->mName = nColor.attribute("name").as_string("color");
                            std::string sColor = nColor.attribute("color").value();
                            c->mColorRGBA = ColorRGBA::ParseFromHexString(sColor);

                            textColorsLibrary.Add(c);
                        }
                    }
                }else if(std::string(n.name())=="texts"){

                    for (pugi::xml_node nText = n.first_child(); nText; nText = nText.next_sibling()){

                        if(std::string(nText.name())=="textFile"){
                            TextBook *tb = new TextBook();
                            tb->mRelativeFilePath = nText.attribute("file").value();
                            tb->mName = StdString::baseNameFromFilePath(tb->mRelativeFilePath);
                            textLibrary.books().push_back(tb);
                        }
                    }

                }else if(std::string(n.name())=="settings"){
                    yOrientation = GetYorientationFromString(n.attribute("yOrientation").as_string("Down"));
                    //if(yOrientationOverrided != Yorientation::NOT_DEFINED){
                    //    yOrientation = yOrientationOverrided;
                    //}
                    dummySourceObjects.imageSize = n.attribute("dummiesTextureSize").as_int(dummySourceObjects.imageSize);
                }
            }
        }
    }

    //globalSourceObjectsCreated = true;

}


void SourceObjectsLoader_xml::loadSourceGroup(SourceGroup *sourceGroup)
{


    std::string dirPath = CurrentProject.sourceCfgDir + "/" + sourceGroup->mName;
    //if(sourceGroup->mName==""){
    //    dirPath = CurrentProject.sourceCfgDir;
    //}

    //QDir dir(QString::fromStdString(dirPath));
    //if(!dir.exists()) return;
    if(ncine::FileSystem::isDirectory(dirPath.c_str())==false){
        assert(true);
    }
    ncine::FileSystem::Directory dir(dirPath.c_str());


    //sourceGroup->DeleteSets();
    //if(sourceGroup->mName==""){
    //    loadGlobalData();
    //}


    std::vector<CompositeSprite*>ComposedSprites;

    //for(const QFileInfo &info : dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot)){

    //    if(info.suffix()==glob::cfgFileSuffix){
            //QString rootName = GetRootElementNameFromCfgXml(info.absoluteFilePath());
    //        std::string rootName = rootNodeName(info.absoluteFilePath().toStdString());

    while(true){

        const char *_fileName = dir.readNext();
        if(_fileName==nullptr){
            break;
        }
        std::string fileName(_fileName);
        if(fileName.empty()==false && fileName.substr(0, 1)=="."){
             continue;
        }
        std::string filePath = dirPath + "/" + fileName;

        if(ncine::FileSystem::isFile(filePath.c_str())){

            std::string rootName = rootNodeName(filePath);
            if(rootName=="sourceGraphicSet"){

                std::string setName = StdString::baseNameFromFilePath(filePath);
                SourceSet * Set = new SourceSet(setName);
                Set->mParentSourceGroup = sourceGroup;
                if(loadSourceSet(Set)){
                    sourceGroup->sourceSets().push_back(Set);

                }else{
                    delete Set;
                    //QMessageBox::information(MainWindow, "Load graphic set error!", glob::loadErrorMessage);
                }

            }else if(rootName=="composedSprite"){

                //TEComposedSprite *ComposedSprite = new TEComposedSprite(info.baseName().toStdString());
                CompositeSprite *ComposedSprite = new CompositeSprite();
                //ComposedSprite->mMap->tileWidth = CurrentProject.tileWidth;
                //ComposedSprite->mMap->tileHeight = CurrentProject.tileHeight;
                ComposedSprite->mSourceSprite = new SourceSprite(SpriteType::COMPOSITE);                    // ! takoj naredimo ker uporabljamo Parameters od TESourceObject
                ComposedSprite->mSourceSprite->mSourceCompositeSprite = ComposedSprite;
#ifdef JUGI_EDITOR
                ComposedSprite->mSourceSprite->mMarkerImage = dummySourceObjects.SIcompositeSpriteSourceImage;
                ComposedSprite->mSourceSprite->sourceImages().push_back(ComposedSprite->mSourceSprite->mMarkerImage);
#endif

                //if(ComposedSprite->LoadFromCfgXml_pugi(info.absoluteFilePath().toStdString())){
                //if(loadCompositeSprite(ComposedSprite, info.absoluteFilePath().toStdString())){
                if(loadSourceCompositeSprite(ComposedSprite, filePath)){
                    ComposedSprites.push_back(ComposedSprite);

                }else{
                    //delete ComposedSprite;
                    delete ComposedSprite->mSourceSprite;    // owner je SourceObject !

                    //QMessageBox::information(MainWindow, "Load composed sprite error!", glob::loadErrorMessage);
                }
            }
        }
    }


    //sourceGroup->RebuildImagesForSourceObjecIfNeeded();

//#ifdef JUGI_EDITOR
    finalizeSourceCompositeSprites(sourceGroup, ComposedSprites);
//#else
//    finalizeComposedSprites_ReadFromCfgFiles(sourceGroup, ComposedSprites);
//#endif

    //---
    //sourceGroup->OrderSets();

#ifdef JUGI_EDITOR
    OrderSets(sourceGroup);
    //---
    if(sourceGroup->sourceSets().empty()==false){
        CurrentSourceSet = sourceGroup->sourceSets().front();
    }else{
        CurrentSourceSet = nullptr;
    }
#endif

}


bool SourceObjectsLoader_xml::loadSourceSet(SourceSet *sourceSet)
{

    //std::string filePath;

    //assert(sourceSet->mParentSourceGroup);

    //if(sourceSet->mParentSourceGroup && sourceSet->mParentSourceGroup->mName!=""){
        std::string filePath = CurrentProject.sourceCfgDir + "/" + sourceSet->mParentSourceGroup->mName + "/" + sourceSet->mName + ".xml";
    //}else{
    //    filePath = CurrentProject.sourceCfgDir + "/" + sourceSet->mName + ".xml";
    //}


    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filePath.c_str());

    if(result.status==pugi::xml_parse_status::status_ok){

        pugi::xml_node rootNode = doc.child("sourceGraphicSet");
        if(rootNode.empty()==false){
            pugi::xml_attribute a = rootNode.attribute("name");
            assert(a.empty()==false);
            sourceSet->mName = a.value();

            //for(pugi::xml_node n : rootNode.children()){
            for (pugi::xml_node n = rootNode.first_child(); n; n = n.next_sibling()){

                if(std::string(n.name())=="images"){
                    loadSourceSet_images(sourceSet, n);

                }else if(std::string(n.name())=="sprites"){
                    loadSourceSet_sprites(sourceSet, n);

                }else if(std::string(n.name())=="textSprites"){
                    loadSourceSet_texts(sourceSet, n);

                }
            }
        }
    }

    return true;
}


bool SourceObjectsLoader_xml::loadSourceSet_images(SourceSet *sourceSet, pugi::xml_node &node)
{

    for(pugi::xml_node n = node.first_child(); n; n = n.next_sibling()){
        if(std::string(n.name())=="spriteSheetImage"){

            SourceFile * sif = new SourceFile(FileKind::SPRITE_SHEET_IMAGE);
            sif->mRelativeFilePath = n.attribute("file").value();

            readAndMakeSubimages(sif);
            sourceSet->sourceFiles().push_back(sif);
            readSourceImagesExtraData(sif);


        }else if(std::string(n.name())=="tileSheetImage"){

            SourceFile * sif = new SourceFile(FileKind::TILE_SHEET_IMAGE);
            sif->mRelativeFilePath =  n.attribute("file").value();

            readAndMakeSubimages(sif);
            sourceSet->sourceFiles().push_back(sif);
            readSourceImagesExtraData(sif);



        }else if(std::string(n.name())=="singleImage"){

            SourceFile *sif = new SourceFile(FileKind::SINGLE_IMAGE);
            sif->mRelativeFilePath =  n.attribute("file").value();

            readAndMakeSubimages(sif);
            sourceSet->sourceFiles().push_back(sif);
            readSourceImagesExtraData(sif);

#ifdef JUGI_EDITOR

        /*
        }else if(std::string(n.name())=="spineObject"){

            return false;

            SourceFile *SourceImageFile = new SourceFile(FileKind::SPINE_FILE);
            SourceImageFile->mRelativeFilePath =  n.attribute("file").value();
            SourceImageFile->mSpineAtlasRelativeFilePath = n.attribute("atlasFile").value();

            SourceImageFile->mSpineSourceSprite = new TSpineSourceSprite();
            SourceImageFile->mSpineSourceSprite->skeletonFile = CurrentProject.gameRootDir + "/" + SourceImageFile->mRelativeFilePath;
            SourceImageFile->mSpineSourceSprite->atlasFile = CurrentProject.gameRootDir + "/" + SourceImageFile->mSpineAtlasRelativeFilePath;


            for(pugi::xml_node nChild = n.first_child(); nChild; nChild=nChild.next_sibling()){

                if(std::string(nChild.name())=="skin"){

                    SourceImage* si = new SourceImage();
                    si->mSourceFile = SourceImageFile;
                    si->mName = nChild.attribute("name").value();
                    si->mSpineScale = nChild.attribute("scale").as_float(1.0f);
                    if(si->LoadPixmap()){
                        SourceImageFile->mSourceImages.push_back(si);
                    }else{
                        delete si;
                    }

                }else{
                    qDebug()<<"skipped element:"<<nChild.name();
                }
            }

               //---
            if(SourceImageFile->mSourceImages.size()>0){
                sourceSet->sourceFiles().push_back(SourceImageFile);
                SourceImageFile->eDeletePixmap();

            }else{

                print("Error loading spineObject: " + SourceImageFile->mRelativeFilePath);
                delete SourceImageFile;
            }

        }else if(std::string(n.name())=="spriterObject"){

            return false;

            SourceFile *SourceImageFile = new SourceFile(FileKind::SPRITER_FILE);
            SourceImageFile->mRelativeFilePath =  n.attribute("file").value();

            SourceImageFile->mSpriterSourceSprite = new TSpriterSourceSprite();
            SourceImageFile->mSpriterSourceSprite->filePath = CurrentProject.gameRootDir + "/" + SourceImageFile->mRelativeFilePath;

            for(pugi::xml_node nChild = n.first_child(); nChild; nChild=nChild.next_sibling()){

                if(std::string(nChild.name())=="entity"){

                    SourceImage* si = new SourceImage();
                    si->mSourceFile = SourceImageFile;
                    si->mName = nChild.attribute("name").value();
                    si->mSpineScale = nChild.attribute("scale").as_float(1.0f);
                    if(si->LoadPixmap()){
                        SourceImageFile->mSourceImages.push_back(si);
                    }else{
                        delete si;
                    }

                }else{
                        qDebug()<<"skipped element:" << nChild.name();
                }
            }

            //---
            if(SourceImageFile->mSourceImages.size()>0){
                sourceSet->sourceFiles().push_back(SourceImageFile);

                SourceImageFile->eDeletePixmap();
            }else{

                print("Error loading spriterObject: " + SourceImageFile->mRelativeFilePath);
                delete SourceImageFile;
            }
        */

#endif
        }
    }

    return true;
}


bool SourceObjectsLoader_xml::loadSourceSet_sprites(SourceSet *sourceSet, pugi::xml_node &node)
{

    for(pugi::xml_node n = node.first_child(); n; n = n.next_sibling()){
        if(std::string(n.name())=="sprite"){

            SourceSprite *so = new SourceSprite(SpriteType::STANDARD);
            so->mParentSourceSet = sourceSet;
            //o->id = Objects.size();
            so->mName = n.attribute("name").value();

            std::string imageName;
            if(n.attribute("imageName").empty()==false) imageName = n.attribute("imageName").value();
            std::string imageFilePath;
            if(n.attribute("imageFile").empty()==false) imageFilePath = n.attribute("imageFile").value();

            if(imageName.empty()==false && imageFilePath.empty()==false){   // alternativna varianta s krajšim zapisom če ima sprite 1 image
                SourceImage *si = sourceSet->getSourceImage(imageFilePath, imageName);
                if(si){
                    so->mSourceImages.push_back(si);
                }else{
                    DummyFunction();
                }
            }
            for(pugi::xml_node nChild=n.first_child(); nChild; nChild=nChild.next_sibling()){
                if(std::string(nChild.name())=="image"){
                    std::string name = nChild.attribute("name").value();
                    std::string filePath = nChild.attribute("file").value();
                    SourceImage *si = sourceSet->getSourceImage(filePath, name);
                    if(si){
                        so->mSourceImages.push_back(si);
#ifdef JUGI_EDITOR
                        if(nChild.attribute("marker").as_bool()==true) so->mMarkerImage = si;
#endif
                    }

                }else if(std::string(nChild.name())=="parameter"){
                    so->mParameters.push_back(CParameter());
                    CParameter &pv = so->mParameters.back();
                    readParameter(pv, nChild);

                }else if(std::string(nChild.name())=="constantParameter"){
                    so->mConstantParameters.push_back(CParameter());
                    CParameter &pv = so->mConstantParameters.back();
                    readParameter(pv, nChild);


                }else if(std::string(nChild.name())=="frameAnimation"){
                    FrameAnimation *fa = new FrameAnimation(so,"");
                    loadFrameAnimation(fa, nChild);
                    if(fa->GetFrames().empty()){
                        delete fa;
                    }else{
                        so->mFrameAnimations.push_back(fa);
                    }

                }else if(std::string(nChild.name())=="timelineAnimation"){
                    TimelineAnimation *ta = new TimelineAnimation(so);
                    loadTimelineAnimation(ta, nChild);
#ifdef JUGI_EDITOR
                    ta->DeleteEmptyChildAnimationMembers();
                    if(ta->HasNoAnimationTracks()){
                        delete ta;
                    }else{
                        so->mTimelineAnimations.push_back(ta);
                    }
#else
                    so->mTimelineAnimations.push_back(ta);
#endif
                }
            }

            //-----
#ifdef JUGI_EDITOR
            FrameAnimation::AssignMirrorAnimations(so);
#endif

            //-----
            if(so->mSourceImages.empty()==false){
#ifdef JUGI_EDITOR
                if(so->mMarkerImage==nullptr){
                    so->mMarkerImage = so->mSourceImages.front();
                }
#endif
                so->mKind = GetSpriteTypeFromFileKind(so->mSourceImages.front()->parentSourceFile()->kind());
                sourceSet->sourceSprites().push_back(so);

            }else{

                delete so;
            }
        }
    }

    return true;

}


bool SourceObjectsLoader_xml::loadSourceSet_texts(SourceSet *sourceSet, pugi::xml_node &node)
{

    //FontStyles &dbgFontStyles = fontStyles;
    //ColorStyles &dbgColorStyles = colorStyles;


    for(pugi::xml_node n = node.first_child(); n; n = n.next_sibling()){
        if(std::string(n.name())=="textSprite"){

            SourceSprite *so = new SourceSprite(SpriteType::TEXT);
            so->mParentSourceSet = sourceSet;
            so->mName = n.attribute("name").value();

            //--- source image
            SourceFile *textsDummySIF = sourceSet->getTextsDummySourceFile();
            if(textsDummySIF==nullptr){
                textsDummySIF = new SourceFile(FileKind::SPRITE_TEXT_CONTAINER);
                textsDummySIF->mRelativeFilePath = "text items dummy file";
                sourceSet->sourceFiles().push_back(textsDummySIF);
            }

            SourceImage *si = new SourceImage();
            si->mSourceFile = textsDummySIF;
            //si->mName = "textSourceItem";


            //--- text label
            si->mTextLabel = new TextLabel();
            //si->mTextLabel->text = n.attribute("string").value();
            si->mName = so->mName;

            std::string fontStyle = n.attribute("fontStyle").value();
            if(fontStyle.empty()==false){
                assert(false);   // MUST REPLACE fontStyle WITH fontName
            }
            std::string fontName = n.attribute("fontName").as_string("");
            si->mTextLabel->font = fontsLibrary.getFont(fontName);



            //if(tl->font==nullptr){
            //    tl->font = fontLibrary.GetSystemFont();
            //}

            //ColorRGBA c;
            if(n.attribute("colorStyle")){
                assert(false);
                //std::string hexColor = n.attribute("colorStyle").value();
                //ColorRGBA c = ColorRGBA::ParseFromHexString(hexColor);
                //tl->sourceColor = textColorsLibrary.FindColor(c);
            }

            std::string colorName = n.attribute("colorName").as_string("");
            si->mTextLabel->color = textColorsLibrary.getColor(colorName);

            //if(n.attribute("colorName")){
            //    std::string hexColor = n.attribute("colorStyle").value();
            //    ColorRGBA c = ColorRGBA::ParseFromHexString(hexColor);
            //    tl->sourceColor = textColorsLibrary.FindColor(c);
            //}

            //if(tl->sourceColor==nullptr){
            //    tl->sourceColor = textColorsLibrary.defaultSourceColor();
            //}

            //tl->handle = TextLabel::GetHandleFromString(n.attribute("handle").value());
            si->mTextLabel->handle = GetTextHandleVariantFromString(n.attribute("handle").value());




            //si->mLabel = n.attribute("string").as_string("text");
            //JFont *jFont = fontLibrary.FindStyle(n.attribute("fontStyle").value());
            //if(jFont){
             //   si->mFontStyleId = jFont->id;
            //}
            //si->mColorRGBA = ColorRGBA::ParseFromHexString(n.attribute("colorStyle").as_string(""));
            //si->mTextHandleVariant = GetTextHandleVariantFromString(n.attribute("handle").value());




//#ifdef JUGI_EDITOR
            //si->textLabel = tl;
//#else
            //si->mLabel = tl->text;
            //si->mFontStyleId = fontStyles.indexOfStyle(fontStyleName);
            //si->mColorStyleId = tl->colorStyle->id;
            //si->mTextHandleVariant =tl->handle;
            //delete tl;
//#endif


            //if(si->LoadPixmap()){
            //if(si->mFontStyleId != -1){
                so->mSourceImages.push_back(si);
                textsDummySIF->mSourceImages.push_back(si);
            //}

            //}else{
            //    delete si;
            //    si = nullptr;
            //}


            for(pugi::xml_node nChild = n.first_child(); nChild; nChild = nChild.next_sibling()){

                std::string childName = nChild.name();

                if(childName=="parameter"){
                    so->mParameters.push_back(CParameter());
                    CParameter &pv = so->mParameters.back();
                    readParameter(pv, nChild);

                }else if(childName=="constantParameter"){
                    so->mConstantParameters.push_back(CParameter());
                    CParameter &pv = so->mConstantParameters.back();
                    readParameter(pv, nChild);

                }else if(childName=="timelineAnimation"){
                    TimelineAnimation *ta = new TimelineAnimation(so);
                    loadTimelineAnimation(ta, nChild);
#ifdef JUGI_EDITOR
                    ta->DeleteEmptyChildAnimationMembers();
                    if(ta->HasNoAnimationTracks()){
                        delete ta;
                    }else{
                        so->mTimelineAnimations.push_back(ta);
                    }
#else
                    so->mTimelineAnimations.push_back(ta);
#endif
                }
            }

            //-----
            if(so->mSourceImages.empty()==false){
#ifdef JUGI_EDITOR
                if(so->mMarkerImage==nullptr){
                    so->mMarkerImage = so->mSourceImages.front();
                }
#endif
                sourceSet->sourceSprites().push_back(so);

            }else{

                delete so;
            }
        }
    }

    return true;
}


bool SourceObjectsLoader_xml::readAndMakeSubimages(SourceFile *sourceFile)
{

    assert(sourceFile->mSourceImages.empty());

    std::string path = StdString::pathFromFilePath(sourceFile->mRelativeFilePath);
    std::string baseName = StdString::baseNameFromFilePath(sourceFile->mRelativeFilePath);
    std::string cfgFilePath = CurrentProject.gameRootDir + "/" + path + "/" + baseName +"_jugi_subimages.xml";


#ifdef JUGI_EDITOR
    if(sourceFile->mAbsoluteFilePath.empty()==false){
        path = StdString::pathFromFilePath(sourceFile->mAbsoluteFilePath);
        baseName = StdString::baseNameFromFilePath(sourceFile->mAbsoluteFilePath);
        cfgFilePath = path + "/" + baseName +"_jugi_subimages.xml";
    }
#endif


    pugi::xml_document doc;

    pugi::xml_parse_result result = doc.load_file(cfgFilePath.c_str());

    if(result.status != pugi::xml_parse_status::status_ok){
        return false;
    }


    for(pugi::xml_node n = doc.first_child(); n ; n=n.next_sibling()){

        if(std::string(n.name())=="tileSheetImage"){
        //---------------------------------------------------------------------------------

            if(n.attribute("width")) sourceFile->mWidth = n.attribute("width").as_int(-1);
            if(n.attribute("height")) sourceFile->mHeight = n.attribute("height").as_int(-1);
            sourceFile->mTileWidth = n.attribute("tileWidth").as_int(-1);
            sourceFile->mTileHeight = n.attribute("tileHeight").as_int(-1);
            sourceFile->mTilesHorizontalSpacing = n.attribute("horizontalSpacing").as_int(0);
            sourceFile->mTilesVerticalSpacing = n.attribute("verticalSpacing").as_int(0);
            sourceFile->mTilesLeftOffset = n.attribute("leftOffset").as_int(0);
            sourceFile->mTilesTopOffset = n.attribute("topOffset").as_int(0);

            sourceFile->CreateTileSheetSourceImages();


        }else if(std::string(n.name())=="spriteSheetImage"){
        //---------------------------------------------------------------------------------

            if(sourceFile->kind()!=FileKind::SPRITE_SHEET_IMAGE){
                return false;
            }

            if(n.attribute("width")) sourceFile->mWidth = n.attribute("width").as_int(-1);
            if(n.attribute("height")) sourceFile->mHeight = n.attribute("height").as_int(-1);
            sourceFile->mSheetGridSpacingX = n.attribute("gridHorizontalSpacing").as_int(0);
            sourceFile->mSheetGridSpacingY = n.attribute("gridVerticalSpacing").as_int(0);

            for(pugi::xml_node nChild = n.first_child(); nChild; nChild=nChild.next_sibling()){

                if(std::string(nChild.name())=="subimage"){

                    //ESourceImage* SourceImage = new ESourceImage();
                    //SourceImage->nameID = nChild.attribute("name").value();
                    //SourceImage->xInSheetImage = nChild.attribute("x").as_int();
                    //SourceImage->yInSheetImage = nChild.attribute("y").as_int();
                    //SourceImage->width = nChild.attribute("width").as_int();
                    //SourceImage->height = nChild.attribute("height").as_int();
                    //SourceImage->xHandle = SourceImage->width/2;
                    //SourceImage->yHandle = SourceImage->height/2;
                    //sourceFile->SourceImages.push_back(SourceImage);

                    std::string name = nChild.attribute("name").value();
                    int x = nChild.attribute("x").as_int();
                    int y = nChild.attribute("y").as_int();
                    int width = nChild.attribute("width").as_int();
                    int height = nChild.attribute("height").as_int();

                    sourceFile->AddSourceImage(name, x, y, width, height);

                }
            }

            if(sourceFile->mSourceImages.empty()==false){

                /*
                std::string frameKeyWord = "-jfr";
                std::vector<std::string>baseNames;

                for(SourceImage* si : sourceFile->mSourceImages){

                    std::string baseName = si->mName;
                    int index = si->mName.find(frameKeyWord);
                    if(index != std::string::npos){
                        baseName = si->mName.substr(0, index);
                    }

                    //if(baseNames.contains(baseName)==false){
                    int pos = GetIndexOfVectorElement(baseNames, baseName);
                    if(pos==-1){
                        baseNames.push_back(baseName);
                        si->listOrder = (baseNames.size()-1) * 1000;
                    }else{
                        si->listOrder = pos * 1000;
                    }

                    if(index!=-1){
                        //QString frameIndex = qNameID.mid(index+4);
                        std::string frameIndex = si->mName.substr(index+4);
                        si->listOrder += std::stoi(frameIndex);
                    }
                }

                //qSort(SourceImages.begin(), SourceImages.end(), [](const TESourceImage* si1, const TESourceImage* si2) -> bool { return si1->listOrder < si2->listOrder;});
                std::sort(sourceFile->mSourceImages.begin(), sourceFile->mSourceImages.end(), [](const SourceImage* si1, const SourceImage* si2) -> bool { return si1->listOrder < si2->listOrder;});
                */

                std::vector<std::vector<std::string>>storedNameParts;

                for(SourceImage* si : sourceFile->mSourceImages){

                    std::vector<std::string>nameParts = StdString::splitString(si->mName, "__");
                    int frameIndex = 0;
                    std::string &lastPart = nameParts.back();

                    int index = lastPart.find("-jfr");
                    if(index != std::string::npos){
                        std::string frameIndexS = lastPart.substr(index+4);
                        frameIndex = StdString::stringToInt(frameIndexS, 0);
                        lastPart = lastPart.substr(0, index);

                    }
                    /*
                    else{
                        if(nameParts.size()>1 && lastPart.substr(0,2)=="jfr"){
                            std::string frameIndexS = lastPart.substr(2);
                            frameIndex = StdString::stringToInt(frameIndexS, 0);

                            nameParts.pop_back();   // no name part (only frame index) so we remove it from the vector
                        }
                    }
                    */

                    if(nameParts.size() > storedNameParts.size()){
                        storedNameParts.resize(nameParts.size());
                    }

                    si->listOrder = 0;

                    long long int step = 1000LL * 1000LL * 1000LL * 1000LL * 1000LL;
                    for(int i=0; i<nameParts.size(); i++){

                        const std::string &namePart = nameParts.at(i);

                        int pos = StdVector::indexOfElement(storedNameParts[i], nameParts[i]);
                        if(pos==-1){
                            storedNameParts[i].push_back(namePart);

                            si->listOrder += (storedNameParts[i].size()-1) * step;

                        }else{
                            si->listOrder += pos * step;
                        }
                        step /= 1000;       // first part = 10000000, second part = 10000, third part = 10
                    }

                    si->listOrder += frameIndex;
                }

                std::sort(sourceFile->mSourceImages.begin(), sourceFile->mSourceImages.end(), [](const SourceImage* si1, const SourceImage* si2) -> bool { return si1->listOrder < si2->listOrder;});

            }

        }else if(std::string(n.name())=="singleImage"){
        //---------------------------------------------------------------------------------

            if(sourceFile->kind()!=FileKind::SINGLE_IMAGE){
                return false;
            }

            sourceFile->mWidth = n.attribute("width").as_int(-1);
            sourceFile->mHeight = n.attribute("height").as_int(-1);

            assert(sourceFile->mWidth!=-1 && sourceFile->mHeight!=-1);

            //ESourceImage* SourceImage = new ESourceImage(sourceFile);
            //SourceImage->nameID = baseNameFromFilePath(sourceFile->relativeFilePath);
            //SourceImage->width = sourceFile->width;
            //SourceImage->height = sourceFile->height;
            //SourceImage->xHandle = SourceImage->width/2;
            //SourceImage->yHandle = SourceImage->height/2;
            //sourceFile->SourceImages.push_back(SourceImage);

            sourceFile->AddSourceImage(StdString::baseNameFromFilePath(sourceFile->mRelativeFilePath),0,0, sourceFile->mWidth, sourceFile->mHeight);

        }
    }

    return true;

}


bool SourceObjectsLoader_xml::readSourceImagesExtraData(SourceFile *sourceFile)
{

    std::string path = StdString::pathFromFilePath(sourceFile->mRelativeFilePath);
    std::string baseName = StdString::baseNameFromFilePath(sourceFile->mRelativeFilePath);
    std::string cfgFilePath = CurrentProject.gameRootDir + "/" + path + "/" + baseName +"_jugi_extradata.xml";


    pugi::xml_document doc;

    pugi::xml_parse_result result = doc.load_file(cfgFilePath.c_str());

    if(result.status != pugi::xml_parse_status::status_ok){
        return false;
    }

    pugi::xml_node rootNode = doc.child("imageExtraData");

    for(pugi::xml_node n = rootNode.first_child(); n ; n=n.next_sibling()){

        if(std::string(n.name())=="subimage"){

            std::string imageName = n.attribute("name").value();
            SourceImage *sourceImage = nullptr;
            for(SourceImage *si : sourceFile->mSourceImages){
                if(si->mName==imageName){
                    sourceImage = si;
                    break;
                }
            }

            if(sourceImage){
                if(n.attribute("handlePoint")){
                    std::string handlePointString = n.attribute("handlePoint").value();
                    std::vector<std::string>handlePointList = StdString::splitString(handlePointString, ",");
                    assert(handlePointList.size()==2);
                    if(handlePointList.size()==2){
                        //SourceImage->HandlePoint.Set(handlePointList[0].toInt(), handlePointList[1].toInt());
                        sourceImage->mHandle.x = std::stoi(handlePointList[0]);
                        sourceImage->mHandle.y = std::stoi(handlePointList[1]);
                        //if(settings.IsYCoordinateUp()){
                        if(yOrientation != settings.yOrientation()){
                            sourceImage->mHandle.y = sourceImage->height() - sourceImage->mHandle.y;
                        }
                    }
                }
            }


            for(pugi::xml_node nChild = n.first_child(); nChild; nChild=nChild.next_sibling()){

                ShapeKind kind = GetShapeKindFromFileString(nChild.name());

#ifdef JUGI_EDITOR
                VectorShape* shape = VectorShape::Create(kind);
#else
                VectorShape* shape = new VectorShape(kind);
#endif

                if(sourceImage && shape){

                    if(sourceImage->name()=="buttonA1"){
                        DummyFunction();
                    }

                    Vec2f ncineAnchor = Vec2f(sourceImage->width() * 0.5f, sourceImage->height() * 0.5f);

                    std::string PointsString = nChild.attribute("points").value();
                    std::vector<std::string>PointsStringList = StdString::splitString(PointsString, ";");

                    for(const std::string &ps : PointsStringList){

                        std::string pointString = ps.substr(1, ps.length()-2);  // odrezemo oklepaj spredaj in zadaj
                        std::vector<std::string>pointStringList = StdString::splitString(pointString, ",");

                        if(shape->mKind==ShapeKind::BEZIER_POLYCURVE){
                            assert(pointStringList.size()==6);
                            if(pointStringList.size()==6){
                                ShapePoint CP;
                                CP.x = std::stoi(pointStringList[0]);
                                CP.y = std::stoi(pointStringList[1]);
                                CP.BPprev.x = std::stof(pointStringList[2]);
                                CP.BPprev.y = std::stof(pointStringList[3]);
                                CP.BPnext.x = std::stof(pointStringList[4]);
                                CP.BPnext.y = std::stof(pointStringList[5]);
                                shape->mCPoints.push_back(CP);
                            }
                        }else{
                            assert(pointStringList.size()==2);
                            if(pointStringList.size()==2){
                                ShapePoint CP;
                                CP.x = std::stoi(pointStringList[0]);
                                CP.y = std::stoi(pointStringList[1]);

                                //if(settings.IsYCoordinateUp()){
                                if(yOrientation != settings.yOrientation()){
                                    CP.y = sourceImage->height()-CP.y;

                                    //---
                                    CP.x -=  ncineAnchor.x;
                                    CP.y -=  ncineAnchor.y;
                                    CP.BPnext.x -=  ncineAnchor.x;
                                    CP.BPnext.y -=  ncineAnchor.y;
                                    CP.BPprev.x -=  ncineAnchor.x;
                                    CP.BPprev.y -=  ncineAnchor.y;
                                }

                                shape->mCPoints.push_back(CP);
                            }
                        }
                    }

                    shape->mClosed = nChild.attribute("closed").as_bool(false);   // convert "true" or "yes" to bool true
                    if(kind==ShapeKind::RECTANGLE) shape->mClosed = true;
                    shape->mDataFlags = nChild.attribute("dataFlags").as_int(0);
                    shape->mExtra = nChild.attribute("probe").as_bool(false);   // convert "true" or "yes" to bool true

                    if(shape->mCPoints.empty()==false){
                        shape->updatePathPoints();
                        //sourceImage->AddLoadedShape(Shape);
                        sourceImage->mShapes.push_back(shape);
                        //SourceImage->Shapes.push_back
                        shape = nullptr;
                    }
                }

                if(shape){
                    delete shape;
                }
            }

            if(sourceImage){
                sourceImage->rebuildQuickAccessShapeContainers();
            }


        }else{
            print("Error reading cfg file! Wrong name: " + std::string(n.name()));
            return false;
        }
    }

    return true;
}


bool SourceObjectsLoader_xml::loadSourceCompositeSprite(CompositeSprite *cs, const std::string &filePath)
{

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filePath.c_str());

    if(result.status != pugi::xml_parse_status::status_ok){
        print("Error reading file: " + filePath);
        return false;
    }

    pugi::xml_node rootNode = doc.child("composedSprite");
    if(rootNode.empty()){
        return false;
    }

    cs->mMap->_setName(rootNode.attribute("name").value());
    cs->mSourceSprite->mName = cs->mMap->name();
    cs->mSourceSetName = rootNode.attribute("set").value();

    if(cs->mMap->name()=="userSelector"){
        DummyFunction();
    }

    //cs->mSize.x = rootNode.attribute("width").as_int(-1);
    //cs->mSize.y = rootNode.attribute("height").as_int(-1);

    cs->mMap->mNumTilesX = rootNode.attribute("nTilesX").as_int(-1);
    cs->mMap->mNumTilesY = rootNode.attribute("nTilesY").as_int(-1);
    cs->mMap->mTileWidth = rootNode.attribute("tileWidth").as_int(64);
    cs->mMap->mTileHeight = rootNode.attribute("tileHeight").as_int(64);
    //cs->mHandle.x = rootNode.attribute("xHandle").as_int(0);
    //cs->mHandle.y = rootNode.attribute("yHandle").as_int(0);

#ifdef JUGI_EDITOR
    //cs->mSourceSprite->MarkerImage->mHandle.x = cs->mHandle.x;
    //cs->mSourceSprite->MarkerImage->mHandle.y = cs->mHandle.y;
#endif
    //if(yOrientation != settings.yOrientation()){
        //cs->mHandle.y = cs->mSize.y - cs->mHandle.y;

#ifdef JUGI_EDITOR
        //cs->mSourceSprite->MarkerImage->mHandle.y = cs->mHandle.y;
#endif
    //}


    if(cs->mMap->name()=="button_RunScene"){
        DummyFunction();
    }



    for(pugi::xml_node n=rootNode.first_child(); n; n=n.next_sibling()){

        std::string nodeName(n.name());

        //--- PARAMETERS
        if(nodeName=="parameter"){
            cs->mSourceSprite->mParameters.push_back(CParameter());
            CParameter &pv = cs->mSourceSprite->mParameters.back();
            //pv.ReadFromXML_pugi(n);
            readParameter(pv, n);

        }else if(nodeName=="constantParameter"){
            cs->mSourceSprite->mConstantParameters.push_back(CParameter());
            CParameter &pv = cs->mSourceSprite->mConstantParameters.back();
            //pv.ReadFromXML_pugi(n);
            readParameter(pv, n);

        }else if(nodeName=="timelineAnimation"){
            TimelineAnimation *ta = new TimelineAnimation(cs->mSourceSprite);
            loadTimelineAnimation(ta, n);

#ifdef JUGI_EDITOR
            ta->DeleteEmptyChildAnimationMembers();
            if(ta->HasNoAnimationTracks()){
                delete ta;
            }else{
                cs->mSourceSprite->mTimelineAnimations.push_back(ta);
            }
#else
            cs->mSourceSprite->mTimelineAnimations.push_back(ta);
#endif



        //--- LAYERS
        }else if(nodeName == "layer"){
            std::string layerName = n.attribute("name").value();

            SpriteLayer *Layer = new SpriteLayer(layerName, cs->mMap);
            Layer->mParentCompositeSprite = cs;

            Layer->mContent = GetSpriteLayerContentFromString(n.attribute("content").as_string("Free sprites"));
            Layer->hidden = n.attribute("hidden").as_bool(false);

            cs->mMap->addChildLayer(Layer);

            /*
            Layer->mWorldBoundingBox.min.x = n.attribute("xMin").as_int(0);
            Layer->mWorldBoundingBox.min.y = n.attribute("yMin").as_int(0);
            Layer->mWorldBoundingBox.max.x = n.attribute("xMax").as_int(0);
            Layer->mWorldBoundingBox.max.y = n.attribute("yMax").as_int(0);

            if(yOrientation != settings.yOrientation()){
                int yMin = cs->mSize.y - Layer->mWorldBoundingBox.max.y;
                int yMax = cs->mSize.y - Layer->mWorldBoundingBox.min.y;
                Layer->mWorldBoundingBox.min.y = yMin;
                Layer->mWorldBoundingBox.max.y = yMax;
            }
            */


            for(pugi::xml_node n2=n.first_child(); n2; n2=n2.next_sibling()){

                //--- LAYER PARAMETERS
                if(std::string(n2.name()) == "parameter"){
                    Layer->mParameters.push_back(CParameter());
                    CParameter &pv = Layer->mParameters.back();
                    //pv.ReadFromXML_pugi(n2);
                    readParameter(pv, n2);

                //--- SPRITES
                }else if(std::string(n2.name()) == "sprite"){

                    std::string sourceSpriteName = n2.attribute("name").value();
                    SpriteType spriteType = GetSpriteTypeFromString(n2.attribute("type").as_string("Standard"));

                    Sprite *o  = Sprite::makeSpriteByKind(spriteType);

#ifdef JUGI_EDITOR
                    o->idMapCopy = Sprite::idMapCopyCounter++;
#endif

                    o->mParentLayer = Layer;    // !

                    int x =  n2.attribute("x").as_int(0);
                    int y =  n2.attribute("y").as_int(0);
                    int xt = x / cs->mMap->mTileWidth;
                    int yt = y / cs->mMap->mTileHeight;
                    int xInCell = x % cs->mMap->mTileWidth;
                    int yInCell = y % cs->mMap->mTileHeight;

                    if(yOrientation != settings.yOrientation()){
                        yt = cs->mMap->mNumTilesY - yt - 1 ;

                        if(Layer->mContent==SpriteLayerContent::FREE_SPRITES){
                            yInCell = cs->mMap->mTileHeight - yInCell - 1;
                            yt += yInCell/cs->mMap->mTileHeight;
                            yInCell = yInCell % cs->mMap->mTileHeight;

                        }else{
                            yInCell = 0;
                        }
                    }

                    o->setPositionFromGridPosition(Vec2i(xt, yt), Vec2i(xInCell,yInCell), Vec2i(cs->mMap->mTileWidth, cs->mMap->mTileHeight));


#ifdef JUGI_EDITOR
                    o->xt = xt;
                    o->yt = yt;
                    o->xInCell = xInCell;
                    o->yInCell = yInCell;
#endif


                    o->mName = n2.attribute("nameID").value();
                    if(n2.attribute("scale")){
                        o->mScale.x = o->mScale.y = n2.attribute("scale").as_float(1.0f);
                    }else{
                        o->mScale.x = n2.attribute("xScale").as_float(1.0f);
                        o->mScale.y = n2.attribute("yScale").as_float(1.0f);
                    }

                    o->mRotation = n2.attribute("rotation").as_float(0.0f);
                    if(yOrientation != settings.yOrientation()){
                        o->mRotation = -o->mRotation;
                    }

                    o->mFlip.x = n2.attribute("xFlip").as_bool(false);
                    o->mFlip.y = n2.attribute("yFlip").as_bool(false);
                    o->mAlpha = n2.attribute("alpha").as_float(1.0f);

                    if(n2.attribute("extraProperty")){

                        /*
                        o->mExtraProperties.SetKind(EShaderBasedProperties::GetKindFromString(n2.attribute("extraProperty").value()));
                        if(o->mExtraProperties.GetKind()==EShaderBasedProperties::kindOVERLAY_COLOR){
                            if(n2.attribute("overlayColorBlend")){
                                o->mExtraProperties.SetBlend(GetColorOverlayBlendFromString(n2.attribute("overlayColorBlend").value()));
                            }
                            if(n2.attribute("overlayColorRGB")){
                                //int rgba = base::StringToRGBA(n2.attribute("overlayColorRGB").value());
                                //ColorRGBA c(rgba);
                                std::string hexString = n2.attribute("overlayColorRGB").value();
                                ColorRGBA c = ColorRGBA::ParseFromHexString(hexString);
                                 o->mExtraProperties.SetRGB(c.r, c.g, c.b);
                            }
                            if(n2.attribute("overlayColorAlpha")){
                                o->mExtraProperties.SetAlpha(n2.attribute("overlayColorAlpha").as_float(1.0f));
                            }
                        }
                        */
                    }

                    o->mHandleForCompositeSprite = n2.attribute("handleSource").as_bool(false);
                    if(o->mHandleForCompositeSprite){
                        cs->mHandleForMapCompositeSprites = o->position();
                    }


                    if(spriteType==SpriteType::TEXT){
                        TextSprite *ts = static_cast<TextSprite*>(o);
                        ts->mText = n2.attribute("text").as_string("text");
                        ts->mBookName = n2.attribute("book").as_string("");
                        ts->mBookSegmentId = n2.attribute("segment").as_int(0);
                    }

                    o->mExtraPtr = new std::string(sourceSpriteName);


                    for(pugi::xml_node n3=n2.first_child(); n3; n3=n3.next_sibling()){

                        //--- SPRITE PARAMETERS
                        if(std::string(n3.name())=="parameter"){
                            o->mParameters.push_back(CParameter());
                            CParameter &pv = o->mParameters.back();
                            readParameter(pv, n3);
                        }
                    }

                    //----
                    Layer->addSprite(o);

                }
            }

        }else if(nodeName == "vectorLayer"){
            std::string layerName = n.attribute("name").value();

            VectorLayer *Layer = new VectorLayer(layerName, cs->mMap);
            Layer->hidden = n.attribute("hidden").as_bool(false);
            Layer->mParentCompositeSprite = cs;

            //--- vector layer must be in the vector layers group !
            VectorLayersGroup *vlg = nullptr;
            for(ncine::SceneNode* n: cs->mMap->nodeChildren()){
                LayerElement *l = static_cast<LayerNode*>(n)->layerElement;
                if(l->kind()==LayerKind::VECTOR_LAYERS_GROUP){
                    vlg = static_cast<VectorLayersGroup*>(l);
                }
            }

            if(vlg==nullptr){
                vlg = new VectorLayersGroup("Vector layers");
                cs->mMap->addChildLayer(vlg);
            }

            //---
            vlg->addChildLayer(Layer);


            for(pugi::xml_node n2=n.first_child(); n2; n2=n2.next_sibling()){

                //--- LAYER PARAMETERS
                if(std::string(n2.name()) == "parameter"){
                    Layer->mParameters.push_back(CParameter());
                    CParameter &pv = Layer->mParameters.back();
                    //pv.ReadFromXML_pugi(n2);
                    readParameter(pv, n2);

                }else{

                    //--- SHAPES
                    ShapeKind kind = GetShapeKindFromFileString(n2.name());

#ifdef JUGI_EDITOR
                    VectorShape* shape = VectorShape::Create(kind);
#else
                    VectorShape* shape = new VectorShape(kind);
#endif

                    if(shape){

                        std::string PointsString = n2.attribute("points").value();
                        std::vector<std::string>PointsStringList = StdString::splitString(PointsString, ";");

                        for(const std::string &ps : PointsStringList){

                            std::string pointString = ps.substr(1, ps.length()-2);  // odrezemo oklepaj spredaj in zadaj
                            std::vector<std::string>pointStringList = StdString::splitString(pointString, ",");

                            ShapePoint CP;

                            if(shape->mKind==ShapeKind::BEZIER_POLYCURVE){
                                assert(pointStringList.size()==6);
                                if(pointStringList.size()==6){
                                    CP.x = std::stoi(pointStringList[0]);
                                    CP.y = std::stoi(pointStringList[1]);
                                    CP.BPprev.x = std::stof(pointStringList[2]);
                                    CP.BPprev.y = std::stof(pointStringList[3]);
                                    CP.BPnext.x = std::stof(pointStringList[4]);
                                    CP.BPnext.y = std::stof(pointStringList[5]);

                                    if(yOrientation != settings.yOrientation()){
                                        int mapHeight = cs->mMap->mTileHeight * cs->map()->mNumTilesY;
                                        CP.y = mapHeight - CP.y;
                                        CP.BPnext.y = mapHeight - CP.BPnext.y;
                                        CP.BPprev.y = mapHeight - CP.BPprev.y;
                                    }
                                }
                            }else{
                                assert(pointStringList.size()==2);
                                if(pointStringList.size()==2){
                                    CP.x = std::stoi(pointStringList[0]);
                                    CP.y = std::stoi(pointStringList[1]);

                                    if(yOrientation != settings.yOrientation()){
                                        int mapHeight = cs->mMap->mTileHeight * cs->map()->mNumTilesY;
                                        CP.y = mapHeight - CP.y;
                                    }

                                }
                            }

                            shape->mCPoints.push_back(CP);
                        }

                        shape->mName = n2.attribute("nameID").as_string("");
                        shape->mId = n2.attribute("id").as_int(0);
                        shape->mClosed = n2.attribute("closed").as_bool(false);         // convert "true" or "yes" to bool true
                        shape->mDataFlags = n2.attribute("dataFlags").as_int(0);
                        shape->mExtra = n2.attribute("probe").as_bool(false);           // convert "true" or "yes" to bool true

                        if(kind==ShapeKind::RECTANGLE || kind==ShapeKind::ELLIPSE){
                            shape->mClosed = true;
                        }

                        for(pugi::xml_node n3=n2.first_child(); n3; n3=n3.next_sibling()){

                            //--- SPRITE PARAMETERS
                            if(std::string(n3.name())=="parameter"){
                                shape->mParameters.push_back(CParameter());
                                CParameter &pv = shape->mParameters.back();
                                readParameter(pv, n3);
                            }
                        }

                        shape->_setParentLayer(Layer);

                        if(shape->mCPoints.empty()==false){
                            shape->updatePathPoints();
                            Layer->shapes().push_back(shape);
                            shape = nullptr;
                        }
                    }

                    if(shape){
                        delete shape;
                    }
                }
            }
        }
    }


    return true;
}


void SourceObjectsLoader_xml::finalizeSourceCompositeSprites(SourceGroup *sourceGroup, std::vector<CompositeSprite*> &_sourceCompositeSprites)
{


    //---- SOURCE SPRITES
    for(CompositeSprite* cs : _sourceCompositeSprites){

        SourceSet *Set = sourceGroup->getSourceSet(cs->mSourceSetName);
        if(Set==nullptr) Set = sourceGroup->sourceSets().front();
        assert(Set);

        cs->mSourceSprite->mName = cs->mMap->mName;
        cs->mSourceSprite->mParentSourceSet = Set;

        Set->sourceSprites().push_back(cs->mSourceSprite);
    }



    //---- LAYERS and SPRITES
    for(CompositeSprite* cs : _sourceCompositeSprites){

#ifdef JUGI_EDITOR
        cs->mMap->mNumSprites = 0;
#endif

        if(cs->mMap->name()=="button_RunScene"){
            DummyFunction();
        }



        for(SpriteLayer *Layer : cs->map()->spriteLayers()){

            bool spritesChanged = false;

            std::list<Sprite*>::iterator i = Layer->sprites().begin();
            while (i != Layer->sprites().end())
            {
                Sprite *o = *i;

                assert(o->mExtraPtr);
                std::string *sourceSpriteName = (std::string*)o->mExtraPtr;

                SourceSprite *so = nullptr;
                if(*sourceSpriteName=="emptyCompositeSpriteDummy"){
                    so = dummySourceObjects.SSdummyChildForEmptyCompositeSprite;
                }else{
                    so = sourceGroup->getSourceSprite(*sourceSpriteName);
                }

                delete sourceSpriteName;
                o->mExtraPtr = nullptr;

                if(so && so->sourceCompositeSprite()==cs){      // should never happen, probably not needed
                    so = nullptr;
                }

                if(so==nullptr){
                    so = dummySourceObjects.SSmissingSourceSprite;
                }

                //---
                if(so==dummySourceObjects.SSmissingSourceSprite){

                    // If sprite is not standard sprite we must replace it with standard sprite (we don't have dummies for other sprite types)
                    if(dynamic_cast<StandardSprite*>(o)==nullptr){

                        //--- create new standard sprite
                        StandardSprite *newSprite = new StandardSprite();
#ifdef JUGI_EDITOR
                        newSprite->idMapCopy = o->idMapCopy;
#endif
                        newSprite->mParentLayer = Layer;            // set layer right awway so we can set position
                        newSprite->setPosition(o->position());

                        //--- delete originalni sprite
                        Layer->node()->removeChildNode(o->node());
                        i =  Layer->sprites().erase(o->iter);
                        delete o;

                        // after erase the iterator points to the next sprite
                        // if we insert a new sprite at this iterator it will be at the same position like deleted sprite
                        newSprite->iter = Layer->sprites().insert(i, newSprite);
                        i--;        // decrease iterator to point to newly inserted sprite as we increase it at the end of loop

                        o = newSprite;

                        spritesChanged = true;
                    }
                }

                o->mParentLayer = Layer;
                o->mSourceSprite = so;

                if(o->mSourceSprite->mSourceCompositeSprite){
                    o->mHandle = o->mSourceSprite->mSourceCompositeSprite->mHandleForMapCompositeSprites;
                }

                //--- zagotovimo da se struktura parametrov ujema
                if(o->mParameters.size()>0 || o->mSourceSprite->mParameters.size()>0){
                    o->mParameters.syncParameters(o->mSourceSprite->mParameters);
                }
                //o->UpdatePredefinedVariablesFromParameters(true);

                //----
                i++;
            }

            //---
#ifdef JUGI_EDITOR
            cs->mMap->mCurrentLayer = Layer;
            cs->mMap->mNumSprites += Layer->sprites().size();

            // sync child nodes if sprites in the layer changed
            if(spritesChanged){
                Layer->node()->removeAllChildrenNodes();
                for(Sprite* s : Layer->sprites()){
                    Layer->node()->addChildNode(s->node());
                }
            }
#endif
        }




        //--- Composite sprite must not be empty!
#ifdef JUGI_EDITOR
        if(cs->mMap->mNumSprites==0){
            cs->eAddEmptyCompositeSpriteDummyChild();
        }
#endif

    }

    // order sprites by depth of nested children composite sprites
    for(CompositeSprite* cs : _sourceCompositeSprites){
        if(cs->map()->name()=="rotatorA"){
            DummyFunction();
        }
        cs->ObtainNestingDepth();
    }

    //qSort(ComposedSprites.begin(), ComposedSprites.end(),
    //      [](const CompositeSprite* cs1, const CompositeSprite* cs2) -> bool { return cs1->mNestingDepth < cs2->mNestingDepth;});




    // we must copy layers structure for children composite sprites ( we  couldn't do it at sprite creation as source composite sprites were not yet defined)
    for(CompositeSprite* cs : _sourceCompositeSprites){
        if(cs->sourceSprite()->name()=="comSPrite3"){
            DummyFunction();
        }

        for(SpriteLayer* sl : cs->map()->spriteLayers()){
            for(Sprite* s : sl->sprites()){
                if(s->type()==SpriteType::COMPOSITE){
                    CompositeSprite* csChild = static_cast<CompositeSprite*>(s);
                    csChild->map()->_setName(cs->map()->name()+"_spr");
                    CompositeSprite::copyLayers(csChild->sourceSprite()->sourceCompositeSprite(), csChild);
                }
            }
        }
    }


}


bool SourceObjectsLoader_xml::loadTimelineAnimation(TimelineAnimation *ta, pugi::xml_node &node)
{

    ta->name = node.attribute("name").value();

    if(ta->name == "aniRotatorA"){
        DummyFunction();
    }

    ta->bp.startDelay = node.attribute("startDelay").as_int(0);
    std::string stringLoopCount = node.attribute("loopCount").value();
    if(stringLoopCount=="forever"){
        ta->bp.loopForever = true;
        ta->bp.loopCount = 1;
    }else{
        ta->bp.loopCount = std::stoi(stringLoopCount);
        ta->bp.loopForever = false;
    }

#ifdef JUGI_EDITOR
    ta->scaleDuration = node.attribute("scaleDuration").as_float(1.0f);
#endif

    ta->dataFlags = node.attribute("dataFlags").as_int();

    if(node.attribute("repeatStartTime")){
        ta->bp.repeat_PeriodStart = node.attribute("repeatStartTime").as_int(0);
        ta->bp.repeat = true;
        ta->bp.repeat_PeriodEnd = node.attribute("repeatEndTime").as_int(0);
        if(ta->bp.repeat_PeriodEnd < ta->bp.repeat_PeriodStart) ta->bp.repeat_PeriodEnd = ta->bp.repeat_PeriodStart;
        ta->bp.startAtRepeatTime = node.attribute("startAtRepeatTime").as_int();

    }else{
        ta->bp.repeat = false;
        ta->bp.repeat_PeriodStart = ta->bp.repeat_PeriodEnd = 0;
        ta->bp.startAtRepeatTime = true;
   }


    //--- KEY ANIMATIONS
   for(pugi::xml_node n=node.first_child(); n; n=n.next_sibling()){

       if(std::string(n.name())=="animationMember"){

           AnimationMember *mas = new AnimationMember();
           mas->nameID = n.attribute("childNameID").value();
           mas->disabled = n.attribute("disabled").as_bool(false);

           for(pugi::xml_node n2=n.first_child(); n2; n2=n2.next_sibling()){


               if(std::string(n2.name())=="animationTrack"){

                    //EAnimationTrack *ka = _ReadKeyAnimation_pugi(n2, mas);
                   AnimationTrack *ka = readKeyAnimation(ta, mas, n2);
                    if(ka==nullptr){
                        continue;
                    }

                }else{
                    print("skipped element: " + std::string(n2.name()));
                }
           }

           ta->animationMembers.push_back(mas);


       }else if(std::string(n.name())=="metaAnimationTrack"){

            AnimationTrack *ka = readKeyAnimation(ta, nullptr, n);
            if(ka==nullptr){
                continue;
            }
            ka->countKindAni = 1;
            ta->metaAnimationTrack = ka;


       }else if(std::string(n.name())=="parameter"){
           ta->parameters.push_back(CParameter());
           CParameter &pv = ta->parameters.back();
           readParameter(pv, n);
       }
   }

//#ifdef JUGI_EDITOR
   ta->ObtainDuration();
//#endif

   return true;
}


AnimationTrack* SourceObjectsLoader_xml::readKeyAnimation(TimelineAnimation *ta, AnimationMember *mas, pugi::xml_node &node)
{


    AnimationTrack *ka = new AnimationTrack();

    std::string kindS = node.attribute("kind").value();
    ka->kind = GetAnimationTrackKindFromString(kindS);
    if(ka->kind == AnimationTrackKind::NOT_DEFINED){
        delete ka;
        return nullptr;
    }

    ka->animation = ta;
    ka->listOrder = GetOrderOfAnimationTrackKind(ka->kind);
    ka->name = GetAnimationTrackKindString(ka->kind);
    ka->disabled = node.attribute("disabled").as_bool(false);

    std::string colorBlendS = node.attribute("colorBlend").value();
    if(colorBlendS != ""){
        ka->tp.colorBlend = GetColorOverlayBlendFromString(colorBlendS);
    }
    ka->tp.pathNameID = node.attribute("pathNameId").value();
    ka->tp.pathDirectionOrientation = node.attribute("directionOrientation").as_bool(false);
    ka->tp.pathRelDistanceOffset = node.attribute("posOffset").as_float(0.0f);
    ka->tp.reverseDirectionOnClosedShape = node.attribute("reverseOnClosedShape").as_bool();


    for(pugi::xml_node n=node.first_child(); n; n=n.next_sibling()){
        if(std::string(n.name())=="key"){

            AnimationKey *ak = AnimationKey::Create(ka);

            ak->time = n.attribute("time").as_int(0);
#ifdef JUGI_EDITOR
            ak->timeUnscaled = ak->time/ta->scaleDuration;
#endif
            if(n.attribute("easing")) ak->easing.kind = GetEasingKindFromString(n.attribute("easing").value());

             if(ka->kind==AnimationTrackKind::TRANSLATION){
                 AKTranslation* k = static_cast<AKTranslation*>(ak);
                 k->position.x = n.attribute("dx").as_float(0.0f);
                 k->position.y = n.attribute("dy").as_float(0.0f);

                 //if(settings.IsYCoordinateUp()){
                 if(yOrientation != settings.yOrientation()){
                     k->position.y = - k->position.y;
                 }

             }else if(ka->kind==AnimationTrackKind::SCALING){
                 AKScaling* k = static_cast<AKScaling*>(ak);
                 if(n.attribute("scale")){
                     k->scale.x = n.attribute("scale").as_float(1.0f);
                     k->scale.y = k->scale.x;
                     k->uniform = true;                                     // uniform scale

                 }else{
                     k->scale.x = n.attribute("xScale").as_float(1.0f);
                     k->scale.y = n.attribute("yScale").as_float(1.0f);
                     k->uniform = false;                                    // uniform scale
                 }

             }else if(ka->kind==AnimationTrackKind::FLIP_HIDE){
                 AKFlipHide* k = static_cast<AKFlipHide*>(ak);
                 k->flip.x = n.attribute("xFlip").as_int(0);
                 k->flip.y = n.attribute("yFlip").as_int(0);

             }else if(ka->kind==AnimationTrackKind::ROTATION){
                 AKRotation* k = static_cast<AKRotation*>(ak);
                 k->rotation = n.attribute("rotation").as_float(0.0f);

                 //if(settings.IsYCoordinateUp()){
                 if(yOrientation != settings.yOrientation()){
                     k->rotation = - k->rotation;
                 }

             }else if(ka->kind==AnimationTrackKind::ALPHA_CHANGE){
                 AKAlphaChange* k = static_cast<AKAlphaChange*>(ak);
                 k->alpha = n.attribute("alpha").as_float(1.0f);

             }else if(ka->kind==AnimationTrackKind::OVERLAY_COLOR_CHANGE){
                 AKOverlayColorChange* k = static_cast<AKOverlayColorChange*>(ak);
                 k->color.r = n.attribute("red").as_float(0.0f);
                 k->color.g = n.attribute("green").as_float(0.0f);
                 k->color.b = n.attribute("blue").as_float(0.0f);
                 k->color.a = n.attribute("opacity").as_float(1.0f);

             }else if(ka->kind==AnimationTrackKind::PATH_MOVEMENT){
                 AKPathMovement* k = static_cast<AKPathMovement*>(ak);
                 k->relDistance = n.attribute("pos").as_float(0.0f);

             }else if(ka->kind==AnimationTrackKind::FRAME_CHANGE){
                 AKFrameChange* k = static_cast<AKFrameChange*>(ak);
                 k->frameImageIndex = n.attribute("index").as_int(0);
                 k->animationFrame.offset.x = n.attribute("xOffset").as_int(0);
                 k->animationFrame.offset.y = n.attribute("yOffset").as_int(0);
                 k->animationFrame.flip.x = n.attribute("xFlip").as_int(0);
                 k->animationFrame.flip.y = n.attribute("yFlip").as_int(0);

                 //if(settings.IsYCoordinateUp()){
                 if(yOrientation != settings.yOrientation()){
                     k->animationFrame.offset.y = - k->animationFrame.offset.y;
                 }

             }else if(ka->kind==AnimationTrackKind::FRAME_ANIMATION){
                 AKFrameAnimation* k = static_cast<AKFrameAnimation*>(ak);
                 k->animationName = n.attribute("frameAnimationName").value();
                 k->completeLoops = n.attribute("completeLoops").as_bool(true);
                 k->discardAnimationsQueue = n.attribute("discardAnimationsQueue").as_bool(false);
                 k->fSpeed = n.attribute("fSpeed").as_float(1.0f);

             }else if(ka->kind==AnimationTrackKind::TIMELINE_ANIMATION){
                 AKTimelineAnimation* k = static_cast<AKTimelineAnimation*>(ak);
                 k->animationName = n.attribute("timelineAnimationName").value();
                 k->completeLoops = n.attribute("completeLoops").as_bool(true);
                 k->discardAnimationsQueue = n.attribute("discardAnimationsQueue").as_bool(false);
                 k->fSpeed = n.attribute("fSpeed").as_float(1.0f);

             }else if(ka->kind==AnimationTrackKind::META){
                 AKMeta* k = static_cast<AKMeta*>(ak);
                 k->paused = n.attribute("pauseAnimation").as_bool(false);
                 k->label = n.attribute("label").value();
                 k->dataFlags = n.attribute("dataFlags").as_int(0);

                 for(pugi::xml_node nChild=n.first_child(); nChild; nChild=nChild.next_sibling()){
                    if(std::string(nChild.name())=="parameter"){
                         k->parameters.push_back(CParameter());
                         CParameter &pv = k->parameters.back();
                         readParameter(pv, nChild);

                     }else{
                         print("skipped element: " + std::string(nChild.name()));
                     }
                 }
             }

             ka->animationKeys.push_back(ak);

        }else{
            print("skipped element: " + std::string(n.name()));
        }
     }

#ifdef JUGI_EDITOR
     ka->SortAnimationKeys();
#endif

     if(mas){

         mas->animationTracks.push_back(ka);
         for(AnimationTrack *a : mas->animationTracks){
             if(ka->kind==a->kind){
                 ka->countKindAni++;
             }
         }
         if(ka->countKindAni>1){
             //ka->name = QString("%1 (%2)").arg(ka->name).arg(QString::number(ka->countKindAni));
              ka->name = ka->name + " (" + std::to_string(ka->countKindAni) + ")";
         }

         sort(mas->animationTracks.begin(), mas->animationTracks.end(), [](const AnimationTrack* ka1, const AnimationTrack* ka2)
               -> bool { return (ka1->listOrder*100 + ka1->countKindAni < ka2->listOrder*100 + ka2->countKindAni);});

     }

     return ka;
 }


bool SourceObjectsLoader_xml::loadFrameAnimation(FrameAnimation *fa, pugi::xml_node &node)
{

    fa->name = node.attribute("name").value();
    std::string stringLoopCount = node.attribute("loopCount").value();
    if(stringLoopCount=="forever"){
        fa->bp.loopForever = true;
        fa->bp.loopCount = 1;
    }else{
        fa->bp.loopCount = std::stoi(stringLoopCount);
        fa->bp.loopForever = false;
    }

    fa->scaleDuration = node.attribute("scaleDuration").as_float(1.0);
    fa->dataFlags = node.attribute("dataFlags").as_int(0);

    if(node.attribute("repeatStartTime")){
        fa->bp.repeat_PeriodStart = node.attribute("repeatStartTime").as_int(0);
        fa->bp.repeat = true;
        fa->bp.repeat_PeriodEnd = node.attribute("repeatEndTime").as_int(0);
        if(fa->bp.repeat_PeriodEnd < fa->bp.repeat_PeriodStart) fa->bp.repeat_PeriodEnd = fa->bp.repeat_PeriodStart;
        fa->bp.startAtRepeatTime = node.attribute("startAtRepeatTime").as_bool(true);

    }else{
        fa->bp.repeat = false;
        fa->bp.repeat_PeriodStart = fa->bp.repeat_PeriodEnd = 0;
        fa->bp.startAtRepeatTime = true;
    }

#ifdef JUGI_EDITOR
    fa->horMirror = node.attribute("horMirror").as_string("");
    fa->verMirror = node.attribute("verMirror").as_string("");
#endif

    //---FRAMES
    for(pugi::xml_node n=node.first_child(); n; n=n.next_sibling()){

        if(std::string(n.name())=="frame"){

            AnimationFrame *Frame = new AnimationFrame();
            Frame->SetDuration(n.attribute("duration").as_int());
            Frame->durationUnscaled = Frame->duration/fa->scaleDuration;
            Frame->offset.x = n.attribute("xOffset").as_int(0);
            Frame->offset.y = n.attribute("yOffset").as_int(0);
            Frame->flip.x = n.attribute("xFlip").as_int(0);
            Frame->flip.y = n.attribute("yFlip").as_int(0);
            Frame->dataFlags = n.attribute("dataFlags").as_int(0);

            //if(settings.IsYCoordinateUp()){
            if(yOrientation != settings.yOrientation()){
                Frame->offset.y = - Frame->offset.y;
            }

            std::string imageName = n.attribute("image").value();
            if(imageName==dummySourceObjects.SIemptyFrame->mName){
                Frame->sourceImage = dummySourceObjects.SIemptyFrame;

            }else{
                for(SourceImage *si : fa->sourceObject->mSourceImages){
                    if(si->mName==imageName){
                        Frame->sourceImage = si;
                        break;
                    }
                }
            }

            if(Frame->sourceImage){
                fa->frames.push_back(Frame);
            }else{
                delete Frame;
            }

        }else if(std::string(n.name())=="parameter"){
            fa->parameters.push_back(CParameter());
            CParameter &pv = fa->parameters.back();
            //pv.ReadFromXML_pugi(n);
            readParameter(pv, n);
        }
    }

//#ifdef JUGI_EDITOR
    fa->ObtainDuration();
//#endif

    return true;

}


void SourceObjectsLoader_xml::readParameter(CParameter &pv, pugi::xml_node &node)
{

    pv.name = node.attribute("name").value();
    pv.value = node.attribute("value").value();

    pv.type = CParameter::typeFromString(node.attribute("type").value());
    if(pv.type==CParameter::typeBOOLEAN){
        //pv.active = (StdString::stringToInt(pv.value,1)==0)? false : true;
        //pv.value = "";

        if(pv.value=="1" || pv.value=="true"){
             pv.value = "true";

        }else{
            pv.value = "false";
        }
    }

    pv.valueMin = node.attribute("min").value();
    pv.valueMax = node.attribute("max").value();

    std::string _valuesSet = node.attribute("valuesSet").value();
    if(_valuesSet.empty()==false){
        pv.valuesSet = StdString::splitString(_valuesSet, ",");
    }

    //if(pv.type!=CParameter::typeBOOLEAN){
        if(node.attribute("active").empty()==false){
            pv.active = node.attribute("active").as_bool(true);
        }
    //}

}


std::string SourceObjectsLoader_xml::rootNodeName(const std::string &filePath)
{

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filePath.c_str());

    if(result.status==pugi::xml_parse_status::status_ok){
        for(pugi::xml_node n = doc.first_child(); n; n = n.next_sibling()){
            if(n.type()==pugi::node_element){
                return std::string(n.name());
            }
        }
    }

    return "";
}


void SourceObjectsLoader_xml::OrderSets(SourceGroup *sourceGroup)
{


    std::string filePath = CurrentProject.sourceCfgDir + "/" + sourceGroup->mName + "/order.txt";
    if(sourceGroup->mName.empty()){
        filePath = CurrentProject.sourceCfgDir + "/order.txt";;
    }


    std::unique_ptr<TextStreamReader>Stream(NewTextStreamReader(filePath));
    if(Stream->IsOpen()==false){
        return;
    }


    if(Stream->Eof()==false){

        std::string line;
        Stream->ReadLine(line);
        if(line.substr(0,6)=="order:"){
            line = line.substr(6);
        }
        std::vector<std::string> names = StdString::splitString(line, ",");

        std::vector<SourceSet*>SetsNEW;
        for(std::string &setName : names){
            SourceSet* Set =  sourceGroup->getSourceSet(setName);
            if(Set){
                SetsNEW.push_back(Set);
                RemoveElementFromVector(sourceGroup->sourceSets(), Set);
            }
        }
        // ce je kakšen element se ostal v "SourceSets" ga skopiramo
        for(SourceSet* Set : sourceGroup->sourceSets()){
            SetsNEW.push_back(Set);
        }
        sourceGroup->sourceSets().clear();
        sourceGroup->sourceSets() = SetsNEW;
    }



    /*
    TTextStream s(filePath, QIODevice::ReadOnly, TStream::qFILE, false);
    if(s.IODevice==nullptr){
        return;
    }

    if(s.AtEnd()==false){

        std::string line = s.ReadLine().toStdString();
        if(line.substr(0,6)=="order:"){
            line = line.substr(6);
        }
        std::vector<std::string> names = splitString(line, ",");

        std::vector<ESourceSet*>SetsNEW;
        for(std::string &setName : names){
            ESourceSet* Set =  sourceGroup->GetSetByName(setName);
            if(Set){
                SetsNEW.push_back(Set);
                RemoveElementFromVector(sourceGroup->sourceSets(), Set);
            }
        }
        // ce je kakšen element se ostal v "SourceSets" ga skopiramo
        for(ESourceSet* Set : sourceGroup->sourceSets()){
            SetsNEW.push_back(Set);
        }
        sourceGroup->sourceSets().clear();
        sourceGroup->sourceSets() = SetsNEW;
    }

    s.Close();
    */

}




//SourceObjectsLoader_xml SourceObjectsLoader;
SourceObjectsLoader_xml sourceObjectsLoader_xml;




}
