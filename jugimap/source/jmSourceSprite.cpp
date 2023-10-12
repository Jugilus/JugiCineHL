#include "ncine/Texture.h"

#ifdef JUGI_EDITOR

//#include "jugi/eFramework/map/eCompositeSprite.h"
#include "jugi/general/openGL/image.h"
#include "jugi/general/openGL/texture.h"
#endif


#include "jmSystem.h"
#include "jmCompositeSprite.h"
#include "jmVectorShape.h"
#include "jmVectorShapeUtilities.h"
#include "jmFrameAnimation.h"
#include "jmTimelineAnimation.h"
#include "jmSourceItem.h"
#include "jmSourceFile.h"
#include "jmSourceSprite.h"




namespace jugimap {





//================================================================================================================

//ESourceObject ESourceObject::dummy_missingSourceSprite;


SourceSprite* ChildSourceObjectReplacement::FindChildSourceSprite(std::vector<ChildSourceObjectReplacement>&_replacements, const std::string &_childSpriteName)
{

    for(ChildSourceObjectReplacement &cssr : _replacements){
        if(cssr.childName == _childSpriteName){
            return cssr.sourceObjectReplacement;
        }
    }

    return nullptr;

}


//================================================================================================================



SourceSprite::~SourceSprite()
{


//#ifdef JUGI_EDITOR

    //for(EFrameAnimation* fa : mEFrameAnimations){
    //    delete fa;
    //}
    //for(ETimelineAnimation* ta : mETimelineAnimations){
    //    delete ta;
    //}

    if(mSourceCompositeSprite){         // laready delete
        //delete mSourceCompositeSprite->node();
#ifdef JUGI_EDITOR
        //delete MarkerImage;         // LINK to dummy
#endif
    }



//#else

    // 'mSourceCompositeSprite' was deleted in advance from the group destructor!

    for(FrameAnimation* fa : mFrameAnimations){
        delete fa;
    }
    for(TimelineAnimation* ta : mTimelineAnimations){
        delete ta;
    }


//#endif

}


void SourceSprite::init()
{

    //if(mInitialized) return;

    //for(SourceImage *im : mSourceImages){
    //    im->init();
    //}


    if(mName=="tilesA,3"){
        DummyFunction();
    }

    //--- status flags
    mStatusFlags = 0;


    if(settings.SpriteCollidersEnabled()){


        if(mSourceImages.size()>1){

            for(int i=0; i<int(mSourceImages.size())-1; i++){
                SourceImage * gi = mSourceImages[i];
                SourceImage * gi2 = mSourceImages[i+1];

                if( !(gi->size() == gi2->size()) ){
                    mStatusFlags |= Status::HAS_IMAGES_OF_DIFFERENT_SIZE;
                }

                if(gi->spriteShapes().empty() && gi2->spriteShapes().empty()){
                    //empty

                }else if(gi->spriteShapes().empty()==false && gi2->spriteShapes().empty()==false){
                    mStatusFlags |= Status::HAS_IMAGES_WITH_SPRITE_SHAPES;

                    if(SameGeometricShapes(gi->spriteShapes(), gi2->spriteShapes())==false){
                        mStatusFlags |= Status::HAS_IMAGES_WITH_DIFFERENT_SPRITE_SHAPES;
                    }

                }else{

                    mStatusFlags = mStatusFlags | Status::HAS_IMAGES_WITH_SPRITE_SHAPES | Status::HAS_IMAGES_WITH_DIFFERENT_SPRITE_SHAPES;

                }

                if(gi->spriteShapes().size()>1){
                    mStatusFlags |= Status::HAS_MULTIPLE_SPRITE_SHAPES_IN_IMAGE;
                }
            }


        }else if(mSourceImages.size()==1){

            SourceImage * gi = mSourceImages[0];
            if(gi->spriteShapes().empty()==false){
                mStatusFlags |= Status::HAS_IMAGES_WITH_SPRITE_SHAPES;

                if(gi->spriteShapes().size()>1){
                    mStatusFlags |= Status::HAS_MULTIPLE_SPRITE_SHAPES_IN_IMAGE;
                }
            }

        }
    }

    mInitialized = true;
}



}



