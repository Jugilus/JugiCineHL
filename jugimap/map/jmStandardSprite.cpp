#include <algorithm>
#include <assert.h>
#include "jmCommon.h"
#include "jmCommonFunctions.h"
#include "jmSystem.h"
#include "jmSourceSprite.h"
#include "jmVectorShape.h"
#include "jmUtilities.h"
#include "jmSystemSourceObjects.h"
#include "jmFrameAnimation.h"
#include "jmAnimationCommon.h"
#include "jmCollision.h"
#include "jmMap.h"
#include "jmLayer.h"
#include "jmSpriteLayer.h"
#include "jmVectorLayer.h"
#include "jmFont.h"
#include "jmCompositeSprite.h"
#include "jmStandardSprite.h"



namespace jugimap {




StandardSpriteNode::StandardSpriteNode(StandardSprite* _s): s(_s)
{
    type_ = ncine::Object::ObjectType::SPRITE;
    setDeleteChildrenOnDestruction(false);
}


StandardSpriteNode::~StandardSpriteNode()
{
    //delete s;
}


void StandardSpriteNode::transform()
{

    if(s->sourceSprite()->name()=="menu_background"){
    }

    ncine::Sprite::transform();


    if (dirtyBits_.test(DirtyBitPositions::TransformationBit) != false){
        s->onTransform();
    }


    /*
    if(s->activeImage){
        s->mWorldBoundingBox = TransformRectf(Rectf(-s->activeImage->GetWidth()*0.5f, -s->activeImage->GetHeight()*0.5, s->activeImage->GetWidth()*0.5f, s->activeImage->GetHeight()*0.5f), worldMatrix());
        s->setBoundingBoxUpdateRequired(true);

        // solution bellow would be also OK!
        //updateAabb();
        //mWorldBoundingBox.set(Vec2f(ncine::Sprite::aabb().x, ncine::Sprite::aabb().y), Vec2f(ncine::Sprite::aabb().x+ncine::Sprite::aabb().w, ncine::Sprite::aabb().y+ncine::Sprite::aabb().h));

        if(s->collider){
            s->collider->UpdateShapes(worldMatrix());
        }
    }
    */

}


//---------------------------------------------------------------------------------------


StandardSprite::StandardSprite ()
{

    mNode = new StandardSpriteNode(this);

}


StandardSprite::~StandardSprite ()
{

    if(mCollider){
        delete mCollider;
    }

    delete mNode;
}


void StandardSprite::initEngineObjects()
{

    if(sourceSprite()->name()=="menu_background"){
        DummyFunction();
    }


    if(isInitialized()) return;

    //sourceSprite()->init();
    //assert(sourceSprite()->isInitialized());

    if(sourceSprite()->isInitialized()==false){     // situation for new created source sprites
        sourceSprite()->init();
    }


    //---
#ifdef JUGI_EDITOR
    SourceImage *si = sourceSprite()->mMarkerImage;
    assert(si);
#else
    SourceImage *si = sourceSprite()->sourceImages().front();    // Create sprite with the first image.
#endif

    SourceFile *gf = si->parentSourceFile();

    static_cast<StandardSpriteNode*>(mNode)->setTexture(gf->texture());
    //static_cast<StandardSpriteNode*>(mNode)->setLayer(parentLayer()->zOrder());

    if(sourceSprite()->name()=="Fella"){
        dbgNcSprite = static_cast<StandardSpriteNode*>(mNode);
    }

    if(settings.SpriteCollidersEnabled()){
        if(sourceSprite()->statusFlags() & SourceSprite::Status::HAS_IMAGES_WITH_SPRITE_SHAPES){
            if(sourceSprite()->statusFlags() & SourceSprite::Status::HAS_MULTIPLE_SPRITE_SHAPES_IN_IMAGE){
                setCollider(new MultiShapesCollider(this));
            }else{
                setCollider(new SingleShapeCollider(this));
            }
        }
    }

    if(parentCompositeSprite() && sourceSprite()->name()=="Fella")
    {
        DummyFunction();
    }
    //---
    setActiveImage(si);

    mInitialized = true;

    updateAllNodeParameters();


    //---
    //setChangeFlags(Property::ALL);
}


void StandardSprite::clearExternalDataLinks()
{

    static_cast<StandardSpriteNode*>(mNode)->setTexture(nullptr);

    mInitialized = false;

}



void StandardSprite::setActiveImage(SourceImage *_image)
{

    if(mActiveImage==_image) return;

    mActiveImage = _image;
    if(mActiveImage==dummySourceObjects.SIemptyFrame){
        mActiveImage = nullptr;
    }

    if(mActiveImage){

        //---- set texture (before setting the handle)
        SourceFile * gf = mActiveImage->parentSourceFile();
        static_cast<StandardSpriteNode*>(mNode)->setTexture(gf->texture());
        //ncine::Recti rect(activeImage()->subimagePosition().x, activeImage()->subimagePosition().y, activeImage()->width(), activeImage()->height());
        ncine::Recti rect(activeImage()->subimagePosition().x, activeImage()->subimagePosition().y, activeImage()->width(), activeImage()->height());
        static_cast<StandardSpriteNode*>(mNode)->setTexRect(rect);

        setHandle(Vec2f(mActiveImage->handle().x, mActiveImage->handle().y));

        //---- update world bounding box
        mWorldBoundingBox = TransformRectf(BoundingBoxFloat(-mActiveImage->width()*0.5f, -mActiveImage->height()*0.5, mActiveImage->width()*0.5f, mActiveImage->height()*0.5f), mNode->worldMatrix());
        setParentBoundingBoxUpdateRequired(true);
    }


    //----
    if(mActiveImage==nullptr){                   // image can also be nullptr!
        if(mNode->isDrawEnabled()){                    // update for new ncine version
            mNode->setDrawEnabled(false);
        }
        return;

    }else{
        if(mNode->isDrawEnabled()==false && isVisible()){
            mNode->setDrawEnabled(true);
        }
    }


    setColliderShapes();
}


void StandardSprite::setHandle(Vec2f _handle)
{

    mHandle = _handle;

    if(mActiveImage){
        ncine::Vector2f anchorPoint(mHandle.x - mActiveImage->width() * 0.5f, mHandle.y - mActiveImage->height()*0.5f);
        mNode->setAbsAnchorPoint(anchorPoint);
    }
}


void StandardSprite::setVisible(bool _visible)
{

    mVisible = _visible;

    bool realVisible = mVisible;
    if(mAP){
        if(!mAP->hidden != realVisible){
            realVisible = !mAP->hidden;
        }
    }

    if(realVisible){
        if(activeImage()){
            mNode->setDrawEnabled(true);
        }

    }else{
        mNode->setDrawEnabled(false);
    }

}


/*
void StandardSprite::updateEngineObjects()
{

    return;

    if(changeFlags()==0) return;

    int flags = changeFlags();


    if(flags & Property::TRANSFORMATION){


        //Vec2f h = handle();
        Vec2f localPos = position();
        localPos += parentLayer()->GetParallaxOffset();

        Vec2f localScale = scale();
        float localRotation = rotation();
        Vec2i localFlip = flip();


        if(flags & Property::HANDLE){
            //if(activeImage()){
            //    if(sourceSprite()->GetName()=="Fella"){
            //        DummyFunction();
            //    }
            //    ncine::Vector2f anchorPoint(handle().x - activeImage()->GetWidth() * 0.5f, handle().y - activeImage()->GetHeight()*0.5f);
             //   mNode->setAbsAnchorPoint(anchorPoint);
            //}
        }



        if(flags & Property::POSITION){
            mNode->setPosition(localPos);
        }
        if(flags & Property::SCALE){
            mNode->setScale((localFlip.x==1)? -localScale.x : localScale.x, (localFlip.y==1)? -localScale.y : localScale.y);
        }
        if(flags & Property::ROTATION){
            mNode->setRotation(-localRotation);
        }
        if(flags & Property::FLIP){

            if(sourceSprite()->GetName()=="Fella"){
                DummyFunction();
            }

            mNode->setScale((localFlip.x==1)? -localScale.x : localScale.x, (localFlip.y==1)? -localScale.y : localScale.y);

        }

    }


    if(flags & Property::APPEARANCE){


        if(flags & Property::OVERLAY_COLOR){
            manageShaders_OverlayColor();
        }

        if(flags & Property::ALPHA){
            mNode->setAlpha(alpha()*parentLayer()->GetAlpha()*255);
        }

        if(flags & Property::BLEND){
            Blend blend = ownBlend();
            if(blend==Blend::NOT_DEFINED) blend = parentLayer()->GetBlend();

            if(blend==Blend::ALPHA){
                static_cast<StandardSpriteNode*>(mNode)->setBlendingFactors(ncine::DrawableNode::BlendingFactor::SRC_ALPHA,  ncine::DrawableNode::BlendingFactor::ONE_MINUS_SRC_ALPHA);

            }else if(blend==Blend::MULTIPLY){
                static_cast<StandardSpriteNode*>(mNode)->setBlendingFactors(ncine::DrawableNode::BlendingFactor::DST_COLOR,  ncine::DrawableNode::BlendingFactor::ZERO);

            }else if(blend==Blend::ADDITIVE){
                static_cast<StandardSpriteNode*>(mNode)->setBlendingFactors(ncine::DrawableNode::BlendingFactor::SRC_ALPHA,  ncine::DrawableNode::BlendingFactor::ONE);

            }else if(blend==Blend::SOLID){
                static_cast<StandardSpriteNode*>(mNode)->setBlendingFactors(ncine::DrawableNode::BlendingFactor::ONE,  ncine::DrawableNode::BlendingFactor::ZERO);

            }
        }

    }

    //---
    setChangeFlags(0);
}
*/



void StandardSprite::onTransform()
{

    if(mActiveImage){

        if(sourceSprite()->name()=="Blue diamond"){
            DummyFunction();
        }

#ifdef JUGI_EDITOR

        ncine::Matrix4x4f m = mNode->worldMatrix();
        BoundingBoxFloat rect(-mActiveImage->width()*0.5f, -mActiveImage->height()*0.5, mActiveImage->width()*0.5f, mActiveImage->height()*0.5f);

        mBox[0] = jugimap::transform(Vec2f( rect.min.x, rect.min.y ), m);
        mBox[1] = jugimap::transform(Vec2f( rect.max.x, rect.min.y ), m);
        mBox[2] = jugimap::transform(Vec2f( rect.max.x, rect.max.y ), m);
        mBox[3] = jugimap::transform(Vec2f( rect.min.x, rect.max.y ), m);
        //----
        mWorldBoundingBox.setInitialSize();
        for(int i=0; i<mBox.size(); i++){
            if(mWorldBoundingBox.min.x > mBox[i].x) mWorldBoundingBox.min.x = mBox[i].x;
            if(mWorldBoundingBox.min.y > mBox[i].y) mWorldBoundingBox.min.y = mBox[i].y;
            if(mWorldBoundingBox.max.x < mBox[i].x) mWorldBoundingBox.max.x = mBox[i].x;
            if(mWorldBoundingBox.max.y < mBox[i].y) mWorldBoundingBox.max.y = mBox[i].y;
        }


#else

        mWorldBoundingBox = TransformRectf(BoundingBoxFloat(-mActiveImage->width()*0.5f, -mActiveImage->height()*0.5, mActiveImage->width()*0.5f, mActiveImage->height()*0.5f), mNode->worldMatrix());

        // solution bellow would be also OK!
        //updateAabb();
        //mWorldBoundingBox.set(Vec2f(ncine::Sprite::aabb().x, ncine::Sprite::aabb().y), Vec2f(ncine::Sprite::aabb().x+ncine::Sprite::aabb().w, ncine::Sprite::aabb().y+ncine::Sprite::aabb().h));

#endif

        if(mCollider){
            mCollider->UpdateShapes(worldMatrix());
        }

        setParentBoundingBoxUpdateRequired(true);
    }

}


int StandardSprite::activeImageFrameIndex()
{


    for(int i=0; i<sourceSprite()->sourceImages().size(); i++){

        if(sourceSprite()->sourceImages().at(i) == mActiveImage){
            return i;
        }
    }

    return -1;
}


void StandardSprite::setActiveImageByFrameIndex(int _frameIndex)
{

    if(_frameIndex==-1) return;

    assert(_frameIndex>=0 && _frameIndex<sourceSprite()->sourceImages().size());

    setActiveImage(sourceSprite()->sourceImages().at(_frameIndex));

}


void StandardSprite::setColliderShapes()
{

    if(mCollider==nullptr) return;

    if(mActiveImage==nullptr){

        // Possible situation when there is an empty frame image in frame animation!
        if(mPreserveShapesOnNullActiveImage==false){
            mCollider->ClearShapes();
        }
        return;
    }

    if(mActiveImage->spriteShapes().empty()==false){

        if(sourceSprite()->statusFlags() & SourceSprite::Status::HAS_IMAGES_WITH_DIFFERENT_SPRITE_SHAPES){
            mCollider->ClearShapes();
        }

        if(mCollider->GetShapesCount()==0){              // empty collider
            mCollider->AddShapes();
        }

    }else{
        mCollider->ClearShapes();
    }
}


bool StandardSprite::hasCollider()
{
    return mCollider!=nullptr;
}


bool StandardSprite::pointInside(Vec2f _pos)
{

    if(mSourceSprite->name()=="buttonA"){
        DummyFunction();
    }

    //if(changeFlags() & jugimap::Sprite::Property::TRANSFORMATION){
        //updateEngineObjects();                   // need to update engine sprite which also updates collider
    //}

    if(mCollider){
        return mCollider->PointInside(_pos);
    }else{
        return mWorldBoundingBox.Contains(_pos);
    }


}


bool StandardSprite::overlaps(jugimap::Sprite *_sprite)
{
    if(mCollider==nullptr) return false;
    if(_sprite->hasCollider()==false) return false;

    //if(changeFlags() & jugimap::Sprite::Property::TRANSFORMATION){
    //    updateEngineObjects();
    //}
    //if(_sprite->changeFlags() & jugimap::Sprite::Property::TRANSFORMATION){
    //    _sprite->updateEngineObjects();
    //}

    if(_sprite->type()==SpriteType::STANDARD){
        StandardSprite *s = static_cast<StandardSprite*>(_sprite);

        return mCollider->Overlaps(s->collider());

    }else if(_sprite->type()==SpriteType::COMPOSITE){
        CompositeSprite *s = static_cast<CompositeSprite*>(_sprite);

        return s->overlaps(this);
    }

    return false;
}


bool StandardSprite::raycastHit(Vec2f _rayStart, Vec2f _rayEnd, Vec2f &_hitPos)
{
    if(mCollider==nullptr) return false;
    //if(changeFlags() & jugimap::Sprite::Property::TRANSFORMATION){
    //    updateEngineObjects();
    //}

    return mCollider->RaycastHit(_rayStart, _rayEnd, _hitPos);

}


void StandardSprite::applyAnimatedProperties(AnimatedProperties &_ap)
{

    if(mSourceSprite->name() == "Shine"){
        DummyFunction();
    }

    jugimap::Sprite::applyAnimatedProperties(_ap);

    if(mNode==nullptr) return;  //temporary

    if(mAP->changeFlags & Property::TEXTURE){
        if(mActiveImage != mAP->sourceImage){
            setActiveImage(mAP->sourceImage);
        }
    }

    //if(GetNameID()=="batTest" && _ap.changeFlags & Property::TEXTURE){
        //DbgOutput("spiderA texture:" + GetActiveImage()->GetName());
    //    DbgOutput("batTest texture:"+ GetActiveImage()->GetName() + "  pos x:" + std::to_string(_ap.translation.x)+" y:"+ std::to_string(_ap.translation.y));

    //}

}

/*
void StandardSprite::setEngineNodesEnabled(bool _forceEnabled)
{

    if(_forceEnabled){
        node()->setEnabled(true);

    }else{
        node()->setEnabled(mVisible);
    }

}
*/


ncine::Sprite *dbgNcSprite = nullptr;


}



