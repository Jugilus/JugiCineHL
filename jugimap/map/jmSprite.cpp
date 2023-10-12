#include "jmAnimationCommon.h"
#include "jmSpriteLayer.h"
#include "jmTextSprite.h"
#include "jmSpriteModifiers.h"
#include "jmCompositeSprite.h"
#include "jmStandardSprite.h"
#include "jmSprite.h"


namespace jugimap{



Sprite *dbgSprite=nullptr;
Sprite *dbgCompodedSprite=nullptr;
Sprite *dbgChildSprite = nullptr;




/*
Sprite* castNodeToSprite(ncine::SceneNode *_node)
{

    jugimap::Sprite *s = nullptr;
    if(_node->type()==ncine::Object::ObjectType::SPRITE){
        s = static_cast<jugimap::StandardSpriteNode*>(_node)->s;

    }else if(_node->type()==ncine::Object::ObjectType::TEXTNODE){
        s = static_cast<jugimap::TextSpriteNode*>(_node)->s;

    }else if(_node->type()==ncine::Object::ObjectType::SCENENODE){
        s = static_cast<jugimap::LayerNode*>(_node)->compositeSprite;

    }
    //assert(s);

    return s;
}
*/

/*
ncine::SceneNode* castSpriteToSceneNode(jugimap::Sprite* _sprite)
{
    ncine::SceneNode *n = nullptr;
    if(_sprite->type()==SpriteType::STANDARD){
        n = static_cast<StandardSprite*>(_sprite)->node();

    }else if(_sprite->type()==SpriteType::COMPOSITE){
        n = static_cast<CompositeSprite*>(_sprite)->node();

    }else if(_sprite->type()==SpriteType::TEXT){
        n = static_cast<TextSprite*>(_sprite)->node();
    }
   // assert(n);

    return n;
}
*/


Sprite* Sprite::makeSpriteByKind(SpriteType _kind)
{
    if(_kind==SpriteType::STANDARD){
        return new StandardSprite();

    }else if(_kind==SpriteType::COMPOSITE){
        return new CompositeSprite();

    }else if(_kind==SpriteType::TEXT){
        return new TextSprite();

    }

    assert(false);

    return nullptr;
}



Sprite* Sprite::makePassiveSprite(SourceSprite *_sourceSprite)
{

    Sprite *sprite = makeSpriteByKind(_sourceSprite->kind());
    sprite->mSourceSprite = _sourceSprite;

    if(sprite->type()==SpriteType::COMPOSITE){
        CompositeSprite *cs = static_cast<CompositeSprite*>(sprite);

        cs->mHandle = _sourceSprite->sourceCompositeSprite()->mHandleForMapCompositeSprites;

        CompositeSprite::copyLayers(_sourceSprite->sourceCompositeSprite(), cs);


    }

    return sprite;

}



Sprite* Sprite::makePassiveCopy()
{

    Sprite *s = makeSpriteByKind(type());
    s->mSourceSprite = mSourceSprite;
    s->mParentLayer = mParentLayer;               // Set layer link but do not insert sprite into layer yet!
    //s->mParentComposedSprite = mParentComposedSprite;
    s->copyProperties(this, Property::ALL);
    if(type()==SpriteType::COMPOSITE){
        //int zOrder = s->mParentLayer->zOrder();
        //CompositeSprite::copyLayers(static_cast<CompositeSprite*>(this), static_cast<CompositeSprite*>(s), zOrder);
        CompositeSprite::copyLayers(static_cast<CompositeSprite*>(this), static_cast<CompositeSprite*>(s));
    }

    return s;
}





Sprite* Sprite::makeActiveCopy()
{
    Sprite *s = makePassiveCopy();
    s->mParentLayer->addSprite(s);
    s->initEngineObjects();
    //s->updateEngineObjects();
    return s;
}


void Sprite::copyProperties(Sprite *_sprite, int copyFlags)
{

    if(copyFlags & Property::POSITION){
        mPosition = _sprite->mPosition;
    }

    if(copyFlags & Property::SCALE){
        mScale = _sprite->mScale;
    }

    if(copyFlags & Property::FLIP){
        mFlip = _sprite->mFlip;
    }

    if(copyFlags & Property::ROTATION){
        mRotation = _sprite->mRotation;
    }

    if(copyFlags & Property::HANDLE){
        mHandle = _sprite->mHandle;
    }

    if(copyFlags & Property::ALPHA){
        mAlpha = _sprite->mAlpha;
    }

    if(copyFlags & Property::BLEND){
        mBlend = _sprite->mBlend;
    }

    if(copyFlags & Property::OVERLAY_COLOR){
        mColorOverlayActive = _sprite->mColorOverlayActive;
        mColorOverlayRGBA = _sprite->mColorOverlayRGBA;
        mColorOverlayBlend = _sprite->mColorOverlayBlend;
    }

    if(copyFlags & Property::ID){
        mName = _sprite->mName;
        mId = _sprite->mId;
    }

    if(copyFlags & Property::TAG){
        mTag = _sprite->mTag;
    }

    if(copyFlags & Property::DATA){
        mParameters = _sprite->mParameters;
        mDataFlags = _sprite->mDataFlags;
    }

    if(copyFlags & Property::LINK){
        mCustomObject = _sprite->mCustomObject;
    }

    //mSourceSpriteReplacement = _sprite->mSourceSpriteReplacement;
    //mChildrenSourceSpriteReplacements = _sprite->mChildrenSourceSpriteReplacements;

    //other
    mHandleForCompositeSprite = _sprite->mHandleForCompositeSprite;


#ifdef JUGI_EDITOR

    xt = _sprite->xt;
    yt = _sprite->yt;
    xInCell = _sprite->xInCell;
    yInCell = _sprite->yInCell;
    xInCellFixed = _sprite->xInCellFixed;
    yInCellFixed = _sprite->yInCellFixed;
    xInCellFixedSet = _sprite->xInCellFixedSet;
    yInCellFixedSet = _sprite->yInCellFixedSet;
    mUniformScale = _sprite->mUniformScale;

    mExtraProperties = _sprite->mExtraProperties;

    //mTransform = _sprite->mTransform;
    mBox = _sprite->mBox;
    //mOuterBoundingBox = _sprite->mOuterBoundingBox;

#endif



    //mChangeFlags |= copyFlags;
    //mParentLayer->SetSpritesChanged(true);
    //if(parentComposedSprite){
    //    parentComposedSprite->SetChildrenSpritesChanged(true);
    //}
}



SpriteType Sprite::type()
{
    return mSourceSprite->kind();
}


//ESourceObject* Sprite::originalSourceSprite()
//{

//    return (mSSR!=nullptr)? mSSR->originalSourceSprite : mSourceSprite;

//}


CompositeSprite* Sprite::parentCompositeSprite()
{
    if(mParentLayer==nullptr) return nullptr;

    return mParentLayer->parentCompositeSprite();
}



///\brief Set the visibility of this text.
///
/// \see IsVisible
void Sprite::setVisible(bool _visible)
{
    //if(mVisible==_visible) return;

    mVisible = _visible;
    //mChangeFlags |= Property::VISIBILITY;
    //mParentLayer->SetSpritesChanged(true);

    bool realVisible = mVisible;
    if(mAP){
        if(!mAP->hidden != realVisible){
            realVisible = !mAP->hidden;
        }
    }

    mNode->setDrawEnabled(realVisible);
}


void Sprite::setPosition(Vec2f _position)
{
    //if(mPosition==_position) return;

    mPosition = _position;
    //mChangeFlags |= Property::POSITION;
    //mParentLayer->SetSpritesChanged(true);

    Vec2f fullPosition = mPosition + parentLayer()->parallaxOffset();
    if(mAP){
        fullPosition += mAP->translation + mAP->aniFrameOffset;
    }
    mNode->setPosition(fullPosition);

#ifdef JUGI_EDITOR
    setGridPositionFromPosition();
#endif

}



void Sprite::updatePosition()
{
    Vec2f fullPosition = mPosition + parentLayer()->parallaxOffset();
    if(mAP){
        fullPosition += mAP->translation + mAP->aniFrameOffset;
    }
    mNode->setPosition(fullPosition);
}



void Sprite::setScale(Vec2f _scale)
{
    //if(mScale==_scale) return;

    mScale = _scale;
    //mChangeFlags |= Property::SCALE;
   // mParentLayer->SetSpritesChanged(true);

    Vec2f fullScale = mScale;
    if(mAP){
        fullScale *= mAP->scale;
    }
    Vec2i fullFlip = flip();

    mNode->setScale((fullFlip.x==1)? -fullScale.x : fullScale.x, (fullFlip.y==1)? -fullScale.y : fullScale.y);

}


void Sprite::setRotation(float _rotation)
{
    mRotation = _rotation;

    float fullRotation = mRotation;
    if(mAP){
        fullRotation += mAP->rotation;
    }

    //mNode->setRotation(-fullRotation);
    mNode->setRotation(fullRotation);
}




void Sprite::setFlip(Vec2i _flip)
{

    mFlip = _flip;

    Vec2i fullFlip = mFlip;
    if(mAP){
        if(mAP->flip.x) fullFlip.x = 1 - fullFlip.x;
        if(mAP->flip.y) fullFlip.y = 1 - fullFlip.y;
    }
    Vec2f fullScale = scale();

    mNode->setScale((fullFlip.x==1)? -fullScale.x : fullScale.x, (fullFlip.y==1)? -fullScale.y : fullScale.y);

}


/*
void Sprite::setHandle(Vec2f _handle)
{
    if(mHandle==_handle) return;

    mHandle = _handle;
    mChangeFlags |= Property::HANDLE;
    mParentLayer->SetSpritesChanged(true);
}
*/

void Sprite::setAlpha(float _alpha)
{
    //if(mAlpha==_alpha) return;

    mAlpha = _alpha;
    //mChangeFlags |= Property::ALPHA;
    //mParentLayer->SetSpritesChanged(true);

    float fullAlpha = mAlpha;
    if(mAP){
        fullAlpha *= mAP->alpha;
    }
    fullAlpha *= parentLayer()->alpha();

    mNode->setAlpha(fullAlpha*255);

}


void Sprite::setOwnBlend(Blend _blend)
{
    //if(mOwnBlend==_blend) return;

    if(dynamic_cast<ncine::DrawableNode*>(mNode)==nullptr) return;

    mBlend = _blend;
    //mChangeFlags |= Property::BLEND;
    //mParentLayer->SetSpritesChanged(true);

    Blend blend = mBlend;

    if(blend==Blend::NOT_DEFINED) blend = parentLayer()->blend();

    if(blend==Blend::ALPHA){
        static_cast<ncine::DrawableNode*>(mNode)->setBlendingFactors(ncine::DrawableNode::BlendingFactor::SRC_ALPHA,  ncine::DrawableNode::BlendingFactor::ONE_MINUS_SRC_ALPHA);

    }else if(blend==Blend::MULTIPLY){
        static_cast<ncine::DrawableNode*>(mNode)->setBlendingFactors(ncine::DrawableNode::BlendingFactor::DST_COLOR,  ncine::DrawableNode::BlendingFactor::ZERO);

    }else if(blend==Blend::ADDITIVE){
        static_cast<ncine::DrawableNode*>(mNode)->setBlendingFactors(ncine::DrawableNode::BlendingFactor::SRC_ALPHA,  ncine::DrawableNode::BlendingFactor::ONE);

    }else if(blend==Blend::SOLID){
        static_cast<ncine::DrawableNode*>(mNode)->setBlendingFactors(ncine::DrawableNode::BlendingFactor::ONE,  ncine::DrawableNode::BlendingFactor::ZERO);

    }

}


void Sprite::setColorOverlayActive(bool _colorOverlayActive)
{
    //if(mColorOverlayActive==_colorOverlayActive) return;

    mColorOverlayActive = _colorOverlayActive;

    //mChangeFlags |= Property::OVERLAY_COLOR;
    //mParentLayer->SetSpritesChanged(true);

    manageShaders_OverlayColor();
}


void Sprite::setColorOverlayRGBA(ColorRGBA _colorOverlayRGBA)
{
    //if(mColorOverlayRGBA==_colorOverlayRGBA) return;

    mColorOverlayRGBA = _colorOverlayRGBA;
    //mChangeFlags |= Property::OVERLAY_COLOR;
    //mParentLayer->SetSpritesChanged(true);

    manageShaders_OverlayColor();
}


void Sprite::setColorOverlayBlend(ColorOverlayBlend _colorOverlayBlend)
{
    //if(mColorOverlayBlend==_colorOverlayBlend) return;

    mColorOverlayBlend = _colorOverlayBlend;
    //mChangeFlags |= Property::OVERLAY_COLOR;
    //mParentLayer->SetSpritesChanged(true);

    manageShaders_OverlayColor();
}


/*
void Sprite::setChangeFlags(int _changeFlags)
{
    mChangeFlags = _changeFlags;
    if(mChangeFlags !=0){
        mParentLayer->SetSpritesChanged(true);
    }
}


void Sprite::appendToChangeFlags(int _changeFlags)
{
    mChangeFlags |= _changeFlags;
    if(mChangeFlags !=0 ){
        mParentLayer->SetSpritesChanged(true);
    }
}
*/


Vec2f Sprite::position(IncludedPositions _included)
{

    Vec2f pos = mPosition;
    if(_included & IncludedPositions::ANI_TRANSLATION  && mAP){
        pos +=  mAP->translation;
    }
    if(_included & IncludedPositions::PARRALAX_OFFSET){
        pos += mParentLayer->parallaxOffset();
    }
    return pos;
}

//-----


Vec2f Sprite::scale(bool _includeAnimatedProperties)
{
    if(_includeAnimatedProperties && mAP){
        return mScale * mAP->scale;
    }
    return mScale;
}

//-----

float Sprite::rotation(bool _includeAnimatedProperties)
{
    if(_includeAnimatedProperties && mAP){
        return mRotation + mAP->rotation;
    }
    return mRotation;
}

//-----


Vec2i Sprite::flip(bool _includeAnimatedProperties)
{
    if(_includeAnimatedProperties && mAP){
        Vec2i f = mFlip;
        if(mAP->flip.x) f.x = 1 - f.x;
        if(mAP->flip.y) f.y = 1 - f.y;
        return f;

    }
    return mFlip;
}

//-----

float Sprite::alpha(bool _includeAnimatedProperties)
{
    if(_includeAnimatedProperties && mAP){
        return mAlpha * mAP->alpha;
    }
    return mAlpha;
}


//-----


bool Sprite::isOverlayColorActive()
{
    if(mAP && mAP->changeFlags & Property::OVERLAY_COLOR){
        return true;
    }
    return mColorOverlayActive;
}


ColorRGBA Sprite::overlayColorRGBA(bool _includeAnimatedProperties)
{
    if(_includeAnimatedProperties && mAP && mAP->changeFlags & Property::OVERLAY_COLOR){
        return mAP->colorOverlayRGBA;
    }
    return mColorOverlayRGBA;
}


ColorOverlayBlend Sprite::overlayColorBlend(bool _includeAnimatedProperties)
{
    if(_includeAnimatedProperties && mAP && mAP->changeFlags & Property::OVERLAY_COLOR){
        return mAP->colorOverlayBlend;
    }
    return mColorOverlayBlend;
}


void Sprite::createAnimatedPropertiesIfNone()
{

    if(mAP==nullptr){
        mAP = new AnimatedProperties();
    }

}


void Sprite::resetAnimatedProperties()
{

//#ifdef JUGI_EDITOR

    //ResetAnimatedProperties();

//#else

    if(mAP){
        mAP->Reset();
    }

//#endif


}


void Sprite::applyAnimatedProperties(AnimatedProperties& _ap)
{

    if(_ap.changeFlags==0) return;

    int flags = _ap.changeFlags;

    mAP->Append(_ap);


    if(mNode==nullptr) return;  // temporary


    //appendToChangeFlags(_ap.changeFlags);

    if(flags & Property::VISIBILITY){
        setVisible(mVisible);
    }

    if(flags & Property::POSITION){
        setPosition(mPosition);
    }

    if(flags & Property::SCALE){
        setScale(mScale);
    }

    if(flags & Property::ROTATION){
        setRotation(mRotation);
    }

    if(flags & Property::FLIP){
        setFlip(mFlip);
    }

    if(flags & Property::OVERLAY_COLOR){
        manageShaders_OverlayColor();
    }

    if(flags & Property::ALPHA){
        setAlpha(mAlpha);
    }

    //if(sourceSprite->GetName()=="rotatorA"){
    //    DbgOutput("SetAnimatedProperties _ap pos x:" + std::to_string(_ap.translation.x)+" y:" + std::to_string(_ap.translation.y));
    //    DbgOutput("SetAnimatedProperties rotatorA pos x:" + std::to_string(GetGlobalPosition().x)+" y:" + std::to_string(GetGlobalPosition().y));

    //}

    //if(sourceSprite()->name()=="ramA"){
    //    print( sourceSprite()->name() +" pos.y: " + std::to_string(mPosition.y));
    //}
    //    DbgOutput("SetAnimatedProperties _ap pos x:" + std::to_string(_ap.translation.x)+" y:" + std::to_string(_ap.translation.y));
    //    DbgOutput("SetAnimatedProperties rotatorA pos x:" + std::to_string(GetGlobalPosition().x)+" y:" + std::to_string(GetGlobalPosition().y));

}


//------


/*
void Sprite::createSpriteModifiersIfNone()
{

    if(mSM==nullptr){
        mSM = new SpriteModifiers();
    }

}
*/

//------


void Sprite::setParentBoundingBoxUpdateRequired(bool _value)
{

    if(type()==SpriteType::COMPOSITE){
        mBoundingBoxUpdateRequired = _value;
    }
    if(parentLayer()){          // it can be null if this Sprite is a source Composite sprite
        parentLayer()->setBoundingBoxUpdateRequired(_value);
    }
    if(parentCompositeSprite()){
        parentCompositeSprite()->setParentBoundingBoxUpdateRequired(_value);
    }

}


//------


void Sprite::manageShaders_OverlayColor()
{


    if(isOverlayColorActive()==false){

        mNode->setColor(255,255,255,255);


    }else{

        ColorRGBA colorRGBA = overlayColorRGBA();
        ColorRGBAf c(colorRGBA);


        switch (overlayColorBlend()) {


        case ColorOverlayBlend::SIMPLE_MULTIPLY:

            mNode->setColor(colorRGBA.r, colorRGBA.g, colorRGBA.b, colorRGBA.a);
            break;

        case ColorOverlayBlend::NORMAL:

            break;

        case ColorOverlayBlend::MULTIPLY:

            break;

        case ColorOverlayBlend::LINEAR_DODGE:

            break;

        case ColorOverlayBlend::COLOR:

            break;

        case ColorOverlayBlend::NOT_DEFINED:

            break;

        }

    }


}


void Sprite::setPositionFromGridPosition(Vec2i cellPos, Vec2i inCellPos, Vec2i cellSize)
{

    Vec2f pos;

    if(mParentLayer->spriteLayerContent()==SpriteLayerContent::FREE_SPRITES){
        pos.x = cellPos.x * cellSize.x + inCellPos.x;
        pos.y = cellPos.y * cellSize.y + inCellPos.y;

    }else if(mParentLayer->spriteLayerContent()==SpriteLayerContent::GRID_SPRITES){
        pos.x = cellPos.x * cellSize.x + cellSize.x/2.0;
        pos.y = cellPos.y * cellSize.y + cellSize.y/2.0;
    }

    if(mNode){
        setPosition(pos);
    }

}



void Sprite::updateAllNodeParameters()
{
    //if(changeFlags()==0) return;

    //int flags = changeFlags();


    //if(flags & Property::HANDLE){
        setHandle(mHandle);
    //}

    //if(flags & Property::VISIBILITY){
        setVisible(mVisible);
    //}


    //if(flags & Property::POSITION){
        setPosition(mPosition);
    //}

    //if(flags & Property::SCALE){
        setScale(mScale);
    //}

    //if(flags & Property::ROTATION){
        setRotation(mRotation);
    //}

    //if(flags & Property::FLIP){
        setFlip(mFlip);
    //}

    //if(flags & Property::OVERLAY_COLOR){
        manageShaders_OverlayColor();
    //}

    //if(flags & Property::ALPHA){
        setAlpha(mAlpha);
    //}

    //if(flags & Property::BLEND){
        setOwnBlend(mBlend);
    //}


}

#ifndef JUGI_EDITOR


Sprite::~Sprite()
{

    if(mAP){
        delete mAP;
    }
    //if(mSSR){
    //    delete mSSR;
    //}
    //if(mSSRChildren){
    //    delete mSSRChildren;
    //}
    if(mCustomObject){
        delete mCustomObject;
    }

    //delete mNode;

}


#endif





}
