#ifdef JUGI_EDITOR
#include <QFont>
#include <QFontMetrics>
#include <QImage>
#include <QMessageBox>
#include "jugi/general/tpixmap.h"
#include "jugi/general/openGL/image.h"
#include "jugi/general/openGL/texture.h"
#include "jugi/gui/mainwindow.h"
#include "jugi/eFramework/map/eVectorShape.h"
#include "jugi/globals.h"

using namespace jugi;
using namespace jugi::base;
using namespace jugi::openGL;

#endif

#include "jmSystem.h"
#include "ncine/Texture.h"
#include "jmSourceItem.h"
#include "jmSourceFile.h"


namespace jugimap {





SourceFile::~SourceFile()
{
    if(mTexture) delete mTexture;

    for(SourceImage *i : mSourceImages){
        delete i;
    }

#ifdef JUGI_EDITOR

    if(mPixmap) delete mPixmap;


#endif

}


/*
void SourceFile::init()
{
    if(mTexture) return;            // already loaded


    if(kind()==FileKind::SPINE_FILE){

        // spine atlas file: GetSpineAtlasRelativeFilePath()
        // spine skeleton file: GetRelativeFilePath()

    }else if(kind()==FileKind::SPRITER_FILE){

        // spriter model file: GetRelativeFilePath()

    }else{      // image

        mTexture = new ncine::Texture(relativeFilePath().c_str());

        assert(mTexture);
    }
}
*/

SourceImage* SourceFile::AddSourceImage(const std::string &_name, int _x, int _y, int _width, int _height)
{


    SourceImage* si = new SourceImage();
    si->mSourceFile = this;
    si->mName = _name;
    si->mRect.min.x = _x;
    si->mRect.min.y = _y;
    si->mRect.max.x = si->mRect.min.x + _width;
    si->mRect.max.y = si->mRect.min.y + _height;
    si->mHandle.set(_width/2, _height/2);
    mSourceImages.push_back(si);

    return si;

    /*
    SourceItem* si = new SourceItem();
    si->mSourceImageFile = this;
    si->nameID = _nameID;
    si->xInSheetImage = _x;
    si->yInSheetImage = _y;
    si->width = _width;
    si->height = _height;
    si->xHandle = _width/2;
    si->yHandle = _height/2;
    mItems.push_back(si);

    return si;
    */

}


SourceImage* SourceFile::GetSourceImage(const std::string &_name)
{

    for(SourceImage* si : mSourceImages){
        if(si->mName==_name) return si;
    }
    return nullptr;
}


bool SourceFile::CreateTileSheetSourceImages()
{

    assert(mWidth!=-1 && mHeight!=-1);


    for(SourceImage *si: mSourceImages){
        delete si;
    }
    mSourceImages.clear();


    //---
    int i = 0;
    mNumTilesY = 0;
    int yPos = mTilesTopOffset;
    while(yPos+mTileHeight <= mHeight){

        mNumTilesX = 0;
        int xPos = mTilesLeftOffset;
        while(xPos+mTileWidth <= mWidth){

            SourceImage* si = AddSourceImage(std::to_string(i),xPos,yPos,mTileWidth,mTileHeight);

            //---
            xPos += mTileWidth + mTilesHorizontalSpacing;
            i++;
            mNumTilesX++;
        }

        yPos += mTileHeight + mTilesVerticalSpacing;
        mNumTilesY++;
    }

    return true;
}



void SourceFile::initSourceObjects()
{

    for(SourceImage *si : mSourceImages){
        si->rebuildQuickAccessShapeContainers();
    }

}


#ifndef JUGI_EDITOR


bool SourceFile::LoadExternalData()
{

    if(mExternalDataLoaded) return true;


    if(IsFileImage(mKind)){

        if(mTexture) return true;

        std::string filePath = CurrentProject.gameRootDir + "/" + mRelativeFilePath;

        mTexture = new ncine::Texture(filePath.c_str());
        if(mTexture->dataSize()==0){
            delete mTexture;
            dbgSystem.addMessage("Error loading image '" + filePath + "'!");
            return false;
        }

        //assert(mTexture->dataSize()>0);
        //mTexture->setMagFiltering(ncine::Texture::Filtering::NEAREST);
        //mTexture->setMinFiltering(ncine::Texture::Filtering::LINEAR);

    }else if(mKind==FileKind::SPINE_FILE || mKind==FileKind::SPRITER_FILE){


    }else if(mKind==FileKind::SPRITE_TEXT_CONTAINER){


    }

    mExternalDataLoaded = true;

    return true;

}


void SourceFile::DeleteExternalData()
{

    for(SourceImage* si : mSourceImages){
        si->DeleteExternalData();
    }

    //---
    if(mTexture){
        delete mTexture;
        mTexture = nullptr;
    }

    //---
    mExternalDataLoaded = false;

}


#endif



}
