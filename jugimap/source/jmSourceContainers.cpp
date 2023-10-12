#ifdef JUGI_EDITOR
#include "eFramework/projectMaintenance.h"
#endif

#include "jmSystem.h"
#include "jmCommonFunctions.h"
#include "jmMap.h"
#include "jmCompositeSprite.h"
#include "jmGuiText.h"
#include "jmLayer.h"
#include "jmFont.h"
#include "jmSourceItem.h"
#include "jmSourceFile.h"
#include "jmSourceSprite.h"
#include "jmSourceContainers.h"



namespace jugimap {



SourceSet::SourceSet(const std::string &_name)
{
    mName = _name;
}


SourceSet::~SourceSet()
{

    for(SourceFile* SIF : mSourceFiles){
        delete SIF;
    }

    for(SourceSprite *o : mSourceSprites){
        delete o;
    }


#ifdef JUGI_EDITOR
    if(this==CurrentSourceSet){
        CurrentSourceSet = nullptr;
    }
#endif
}


SourceFile* SourceSet::getSourceFile(const std::string &_relativeFilePath)
{
    for(SourceFile* sf : mSourceFiles){
        if(sf->relativeFilePath() == _relativeFilePath){
            return sf;
        }
    }
    return nullptr;
}


SourceImage* SourceSet::getSourceImage(const std::string &_relativeFilePath, const std::string &_imageName)
{
    SourceFile* SIF = getSourceFile(_relativeFilePath);
    if(SIF){
        return SIF->GetSourceImage(_imageName);
    }
    return nullptr;
}


SourceSprite *SourceSet::getSourceSprite(const std::string &_name)
{
    for(SourceSprite* o : mSourceSprites){
        if(o->name() == _name) return o;
    }
    return nullptr;
}


SourceFile* SourceSet::getTextsDummySourceFile()
{

    for(SourceFile *sif : mSourceFiles){
        if(sif->kind()==FileKind::SPRITE_TEXT_CONTAINER){
            return sif;
        }
    }

    return nullptr;
}


//========================================================================================================



SourceGroup::SourceGroup(const std::string &_nameID)
{
    mName = _nameID;

}


SourceGroup::~SourceGroup()
{

    // First delete all composite sprites, which can contain sprites from different sets within the group!
    std::vector<CompositeSprite*>compositeSprites;
    for(SourceSet* Set : mSourceSets){
        for(SourceSprite *ss : Set->sourceSprites()){
            if(ss->sourceCompositeSprite()){
                //ss->sourceCompositeSprite()->GetComposedSpritesDepth();
                compositeSprites.push_back(ss->sourceCompositeSprite());
            }
        }
    }
    //--- sort from bigger depth to lower depth
    //sort(compositeSprites.begin(), compositeSprites.end(),
    //      [](const CompositeSprite* cs1, const CompositeSprite* cs2) -> bool { return cs1->mNestingDepth > cs2->mNestingDepth;});

    //---
    for(CompositeSprite* cs : compositeSprites){
        delete cs;
    }
    compositeSprites.clear();


    //---
    for(SourceSet* Set : mSourceSets){
        delete Set;
    }
    mSourceSets.clear();


#ifdef JUGI_EDITOR
    if(CurrentSourceGroup==this){
        CurrentSourceGroup = nullptr;
    }
#endif
}



void SourceGroup::initSourceObjects()
{

    for(SourceSet* Set : mSourceSets){
        for(SourceFile *sif : Set->sourceFiles()){
            sif->initSourceObjects();
        }
    }

    for(SourceSet* set : mSourceSets){
        for(SourceSprite *ss : set->sourceSprites()){
            ss->init();
        }
    }

}




bool SourceGroup::loadExternalData()
{


    dbgSystem.addMessage("loading source group '" + mName +"' external data");


#ifdef JUGI_EDITOR

    // make pixmaps for the gui selector where needed
    //for(SourceSet* Set : mSourceSets){
    //     for(SourceFile *sif : Set->sourceFiles()){
    //         sif->MakeSourceSpritesPixmaps();
    //    }
    //}

    // initilialize composite source sprites
    std::vector<CompositeSprite*>compositeSprites;
    for(SourceSet* Set : mSourceSets){
        for(SourceSprite *ss : Set->sourceSprites()){
            if(ss->sourceCompositeSprite()){
                compositeSprites.push_back(ss->sourceCompositeSprite());
            }
        }
    }
    //--- sortiramo po nesting globini - od composite sprajtov brez composed childov do tistih z najbolj globokim "nestingom" !
    sort(compositeSprites.begin(), compositeSprites.end(),
          [](const CompositeSprite* cs1, const CompositeSprite* cs2) -> bool { return cs1->mNestingDepth < cs2->mNestingDepth;});


    //---
    std::vector<SourceImage*>sourceItemsForChildTextPixmapsCreation;           // not needed in this case
    for(CompositeSprite* cs : compositeSprites){
        cs->eSetChildTextSpritePixmaps(sourceItemsForChildTextPixmapsCreation);               // must call this before SourceFile LoadExternalData();
    }
#endif


    for(SourceSet* Set : mSourceSets){
        for(SourceFile *sif : Set->sourceFiles()){
            if(sif->LoadExternalData()==false){
                return false;
            }
        }
    }



#ifdef JUGI_EDITOR

    for(CompositeSprite* cs : compositeSprites){

        if(cs->sourceSprite()->name()=="csBlueDiamond"){
            DummyFunction();
        }

        cs->eInitEngineObjects();
    }

#endif




/*
#ifndef JUGI_EDITOR
    for(SourceSet* set : mSourceSets){
        for(SourceSprite *ss : set->sourceSprites()){
            ss->init();
        }
    }
#endif
*/

    dbgSystem.removeLastMessage();
    return true;

}


void SourceGroup::deleteExternalData()
{

    //--- clear sprite links to external data
    std::vector<CompositeSprite*>compositeSprites;
    for(SourceSet* Set : mSourceSets){
        for(SourceSprite *ss : Set->sourceSprites()){
            if(ss->sourceCompositeSprite()){
                compositeSprites.push_back(ss->sourceCompositeSprite());
            }
        }
    }

    for(CompositeSprite* cs : compositeSprites){
        cs->clearExternalDataLinks();
    }

    //--- delete external data
    for(SourceSet* Set : mSourceSets){
        for(SourceFile *sif : Set->sourceFiles()){
            sif->DeleteExternalData();
        }
    }

}




/*
void SourceGroup::DeleteSets()
{

    for(SourceSet* Set : mSourceSets){
        delete Set;
    }
    mSourceSets.clear();

#ifdef JUGI_EDITOR
    if(this==&SourceSets){
        CurrentSourceSet = nullptr;
    }
#endif

    //textStyles.DeleteStyles();
    //textColors.DeleteColors();
}
*/

SourceSet* SourceGroup::getSourceSet(const std::string &_nameID)
{
    for(SourceSet* Set : mSourceSets){
        if(Set->name()==_nameID) return Set;
    }
    return nullptr;
}



SourceSprite* SourceGroup::getSourceSprite(const std::string &_nameID)
{

    for(SourceSet* Set : mSourceSets){
        for(SourceSprite *so : Set->sourceSprites()){
            if(so->name() == _nameID) return so;
        }
    }

    return nullptr;
}



//========================================================================================================


void SourceLibrary::initSourceObjects()
{

    for(SourceGroup* sg : sourceLibrary.sourceGroups()){
        sg->initSourceObjects();
    }
}



bool SourceLibrary::loadExternalData()
{

    dbgSystem.addMessage("loading source library external data");


    //if(fontsLibrary.loadExternalData()==false){
    //    return false;
    //}

    //if(textLibrary.loadExternalData()==false){
    //    return false;
    //}


    for(SourceGroup* sg : sourceLibrary.sourceGroups()){
        if(sg->loadExternalData()==false){
            return false;
        }
    }

    dbgSystem.removeLastMessage();
    return true;

}


bool SourceLibrary::loadGlobalExternalData()
{

    dbgSystem.addMessage("Loading source library external data...");


    if(fontsLibrary.loadExternalData()==false){
        return false;
    }

    if(textLibrary.loadExternalData()==false){
        return false;
    }

    dbgSystem.removeLastMessage();
    return true;

}





void SourceLibrary::deleteContent()
{

    for(SourceGroup* sg : mGroups){
        delete sg;
    }

    mGroups.clear();

}


SourceSprite* SourceLibrary::getSourceSprite(const std::string &_name)
{

    for(SourceGroup* sg : mGroups){
        for(SourceSet* set : sg->sourceSets()){
            for(SourceSprite *so : set->sourceSprites()){
                if(so->name() == _name) return so;
            }
        }
    }

    return nullptr;
}


SourceGroup* SourceLibrary::getSourceGroup(const std::string& _name)
{

    for(SourceGroup* sg : mGroups){
        if(sg->name()==_name){
            return sg;
        }
    }

    return nullptr;
}


SourceSprite* SourceLibrary::getSourceObjectViaSaveHelperIDs(int sourceGroupSaveID, int sourceSetSaveID, int sourceObjectSaveID)
{

    SourceGroup *sourceGroup = getSourceGroup(saveHelper.sourceGroupName(sourceGroupSaveID));
    if(sourceGroup){
        //sourceGroup->countUsedSets ++;
        SourceSet *sourceSet = sourceGroup->getSourceSet(saveHelper.sourceSetName(sourceSetSaveID));
        if(sourceSet){
            //sourceSet->countUsedObjects ++;
            return sourceSet->getSourceSprite(saveHelper.sourceSpriteName(sourceObjectSaveID));
        }
    }

    return nullptr;
}





//===================================================================================


void SourceDataSaveHelper::clear()
{

//#ifdef JUGI_EDITOR
//    SourceSets.saveID = -1;
//#endif
    for(SourceGroup* g : sourceLibrary.sourceGroups()){
        g->saveID = -1;
        for(SourceSet* ss : g->sourceSets()){
            ss->saveID = -1;
            for(SourceSprite* so : ss->sourceSprites()){
                so->saveID = -1;
            }
        }
    }

    groupNames.clear();
    setNames.clear();
    sourceSpriteNames.clear();

}


int SourceDataSaveHelper::indexForSourceSpriteName(const std::string &name)
{

    for(int i=0; i<sourceSpriteNames.size(); i++){
        if(name==sourceSpriteNames[i]){
            return i;
        }
    }

    sourceSpriteNames.push_back(name);
    return sourceSpriteNames.size()-1;
}


int SourceDataSaveHelper::indexForSetName(const std::string &name)
{

    for(int i=0; i<setNames.size(); i++){
        if(name==setNames[i]){
            return i;
        }
    }

    setNames.push_back(name);
    return setNames.size()-1;
}


int SourceDataSaveHelper::indexForGroupName(const std::string &name)
{

    for(int i=0; i<groupNames.size(); i++){
        if(name==groupNames[i]){
            return i;
        }
    }

    groupNames.push_back(name);
    return groupNames.size()-1;
}


std::string SourceDataSaveHelper::sourceSpriteName(int index)
{
    assert(index>=0 && index<sourceSpriteNames.size());
    return sourceSpriteNames[index];
}

std::string SourceDataSaveHelper::sourceSetName(int index)
{
    assert(index>=0 && index<setNames.size());
    return setNames[index];

}

std::string SourceDataSaveHelper::sourceGroupName(int index)
{
    assert(index>=0 && index<groupNames.size());
    return groupNames[index];

}



SourceLibrary sourceLibrary;
SourceDataSaveHelper saveHelper;



}
