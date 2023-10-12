#ifdef JUGI_EDITOR


#include "jugi/general/openGL/image.h"
//#include "jugi/tools/spineWrapper.h"
//#include "jugi/tools/spriterWrapper.h"
#include "jugi/eFramework/map/eVectorShape.h"
#include "jugi/globals.h"

using namespace jugi;
using namespace jugi::base;
using namespace jugi::openGL;

#endif

#include "jmVectorShape.h"
#include "jmCommonFunctions.h"
#include "jmSourceFile.h"
#include "jmSourceItem.h"


namespace jugimap {





SourceImage::~SourceImage()
{

    //for(EShape *vs : mSpriteShapes){
    //    delete vs;
    //}
    //for(EShape *vs : mExtraShapes){
    //    delete vs;
    //}
    for(VectorShape *vs : mShapes){
        delete vs;
    }

    if(mTextLabel) delete mTextLabel;


#ifdef JUGI_EDITOR
    if(Pixmap) delete Pixmap;
    if(BBShape) delete BBShape;
#endif

}



void SourceImage::rebuildQuickAccessShapeContainers()
{

    mSpriteShapes.clear();
    mExtraShapes.clear();

    for(VectorShape* s : mShapes){
        if(s->isExtra()){
            mExtraShapes.push_back(s);
        }else{
            mSpriteShapes.push_back(s);
        }
    }
}



void SourceImage::revertPositioningFromNcineAncher(Vec2f &p)
{
    p.x+=width()*0.5;
    p.y+=height()*0.5;

    p.x-=handle().x;
    p.y-=handle().y;

}




#ifndef JUGI_EDITOR


bool SourceImage::LoadExternalData()
{


    if(IsFileImage(mSourceFile->kind())){

        // nothing to load

    }else if(mSourceFile->kind()==FileKind::SPINE_FILE){


    }else if(mSourceFile->kind()==FileKind::SPRITER_FILE){


    }else if(mSourceFile->kind()==FileKind::SPRITE_TEXT_CONTAINER){


    }

    return true;
}


void SourceImage::DeleteExternalData()
{


}

#endif


/*
bool TextLabel::operator==(const TextLabel &_tl)
{
    return (text==_tl.text && font==_tl.font && sourceColor==_tl.sourceColor && handle==_tl.handle);
}

bool TextLabel::operator!=(const TextLabel &_tl)
{
    return !(operator==(_tl));
}
*/







//ESourceImage *SIEmptyComposedSpriteDummy = nullptr;
//ESourceImage *SIEmptyFrame = nullptr;
//ESourceImage *SIMissingSourceObject = nullptr;
//ESourceImage *SIMissingTexture = nullptr;




}
