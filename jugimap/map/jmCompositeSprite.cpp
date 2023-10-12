#include <algorithm>
#include <assert.h>
#include "jmCommon.h"
#include "jmCommonFunctions.h"
#include "jmSourceSprite.h"
#include "jmVectorShape.h"
#include "jmUtilities.h"
#include "jmFrameAnimation.h"
#include "jmAnimationCommon.h"
#include "jmCollision.h"
#include "jmMap.h"
#include "jmLayer.h"
#include "jmSpriteLayer.h"
#include "jmVectorLayer.h"
#include "jmFont.h"
#include "jmSpriteModifiers.h"
#include "jmCompositeSprite.h"



namespace jugimap {


/*

ComposedSpriteNode::ComposedSpriteNode(EComposedSprite *_s): s(_s)
{
    setDeleteChildrenOnDestruction(true);
    type_ = ncine::Object::ObjectType::SCENENODE;
}


ComposedSpriteNode::~ComposedSpriteNode()
{
    delete s;
}


void ComposedSpriteNode::transform()
{

    ncine::SceneNode::transform();

    //s->transform();

    s->setParentBoundingBoxUpdateRequired(true);


}

*/



//--------------------------------------------------------------------------------------------


CompositeSprite::CompositeSprite()
{
    //mNode = new ComposedSpriteNode(this);

    mMap = new Map("");
    mMap->node()->compositeSprite = this;
    mNode = mMap->node();       // LINK to map's layerNode

}


CompositeSprite::~CompositeSprite()
{
    //for(Layer* l : layers){
    //    delete l;
    //}
    //layers.clear();

    //delete mMap;
    //mMap->_delete();  // no! -> layerNode deletes map and this composite sprite!


    delete mMap;
}


nctl::Array<ncine::SceneNode*>& CompositeSprite::mapNodeChildren()
{
    return mMap->node()->getChildren();
}


void CompositeSprite::initEngineObjects()
{

    if(isInitialized()) return;


    for(Layer *l : mMap->layers()){
        l->init();
    }
    updatePropertiesViaExtraParameters();


    updateAllNodeParameters();

    mInitialized = true;

}


void CompositeSprite::clearExternalDataLinks()
{

    for(Layer *l : mMap->layers()){
        l->clearExternalDataLinks();
    }

    mInitialized = false;

}


void CompositeSprite::updatePropertiesViaExtraParameters()
{

    int numUpdated = 0;


    for(CParameter &p : mParameters){
        if(p.active==false) continue;

        if(p.name == SM_ChangeChildText::name){
            SM_ChangeChildText sm(p.value);
            if(sm.isValid()){
                mMap->eUpdateTextSprites_SetTexts(sm, numUpdated);
            }

        //}else if(p.name == SM_ChangeChildParameter::name){
        }else if(p.name.substr(0, SM_ChangeChildParameter::name.size()) == SM_ChangeChildParameter::name){
            SM_ChangeChildParameter sm(p.value);
            if(sm.isValid()){
                mMap->updateSprites_SetParameters(sm, numUpdated);
            }
        }
    }


}




void CompositeSprite::setHandle(Vec2f _handle)
{

    mHandle = _handle;
    mNode->setAbsAnchorPoint(mHandle);
}

/*
void ComposedSprite::updateEngineObjects()
{

    return;

    int flags = changeFlags();

    if(flags & Property::TRANSFORMATION){

        Vec2f localPos = position();
        localPos += parentLayer()->GetParallaxOffset();

        Vec2f localScale = scale();
        float localRotation = rotation();
        Vec2i localFlip = flip();


        if(flags & Property::HANDLE){
            //Vec2f h = handle();
            //Vec2i size = GetSourceSprite()->GetSourceComposedSprite()->size;
            //if(GetActiveImage()){
                //ncine::Vector2f anchorPoint(GetHandle().x - GetActiveImage()->GetWidth() * 0.5f, GetHandle().y - GetActiveImage()->GetHeight()*0.5f);
            //}
            //ncine::Vector2f anchorPoint(handle.x - size.x * 0.5f, handle.y - size.y*0.5f);
            //setAbsAnchorPoint(anchorPoint);
            //mNode->setAbsAnchorPoint(handle());

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
            mNode->setScale((localFlip.x==1)? -localScale.x : localScale.x, (localFlip.y==1)? -localScale.y : localScale.y);
        }


        setBoundingBoxUpdateRequired(true);



    }

    //---
    setChangeFlags(0);


    for(ncine::SceneNode* n : mNode->children()){
        Layer *l = static_cast<LayerNode*>(n)->l;

        if(l->GetKind()==LayerKind::SPRITE){
            SpriteLayer * sl = static_cast<SpriteLayer*>(l);
            for(ncine::SceneNode *n : sl->node()->children()){
                jugimap::Sprite *s = castNodeToSprite(n);
                s->updateEngineObjects();
            }
        }
    }


}

*/


/*
void ComposedSprite::setVisible(bool _visible)
{

    if(isVisible()==_visible) return;


    for(ncine::SceneNode* n : mNode->children()){
        Layer *l = static_cast<LayerNode*>(n)->l;

        if(l->GetKind()==LayerKind::SPRITE){
            SpriteLayer * sl = static_cast<SpriteLayer*>(l);
            for(ncine::SceneNode *n : sl->node()->children()){
                jugimap::Sprite *s = castNodeToSprite(n);
                s->setVisible(_visible);
            }
        }
    }
    mVisible = _visible;
    //setVisible(_visible);
    if(mVisible){
        setChangeFlags(Property::TRANSFORMATION | Property::APPEARANCE);
    }
}
*/


bool CompositeSprite::hasCollider()
{

    for(SpriteLayer *sl : mMap->spriteLayers()){
        for(Sprite *s : sl->sprites()){
            bool hasShape = s->hasCollider();
            if(hasShape) return true;
        }
    }
    return false;
}


bool CompositeSprite::pointInside(Vec2f _pos)
{

    //if(changeFlags() & Sprite::Property::TRANSFORMATION){
    //    updateEngineObjects();
    //}

    for(SpriteLayer *sl : mMap->spriteLayers()){
        for(Sprite *s : sl->sprites()){
            bool inside = s->pointInside(_pos);
            if(inside){
                return true;
            }
        }
    }
    return false;
}


bool CompositeSprite::overlaps(Sprite *_sprite)
{

    //if(changeFlags() & Sprite::Property::TRANSFORMATION){
    //    updateEngineObjects();
    //}
    //if(_sprite->changeFlags() & Sprite::Property::TRANSFORMATION){
    //    _sprite->updateEngineObjects();
    //}


    for(SpriteLayer *sl : mMap->spriteLayers()){
        for(Sprite *s : sl->sprites()){
            if(s==_sprite) continue;
            bool overlaps = s->overlaps(_sprite);
            if(overlaps) return true;
        }
    }
    return false;
}


bool CompositeSprite::raycastHit(Vec2f _rayStart, Vec2f _rayEnd, Vec2f &_hitPos)
{

    //if(changeFlags() & Sprite::Property::TRANSFORMATION){
    //    updateEngineObjects();                   // need to update engine sprite so that collider is in sysnc with the sprite
    //}

    //for(Layer *l : layers){
    for(SpriteLayer *sl : mMap->spriteLayers()){
        for(Sprite *s : sl->sprites()){
            bool hit = s->raycastHit(_rayStart, _rayEnd, _hitPos);
            if(hit) return true;
        }
    }
    return false;
}


BoundingBoxFloat CompositeSprite::boundingBox()
{

    if(boundingBoxUpdateRequired()){
        updateBoundingBox();
    }

    return mWorldBoundingBox;

}


BoundingBoxInt CompositeSprite::boundingBoxInt()
{

    if(boundingBoxUpdateRequired()){
        updateBoundingBox();
    }

    return BoundingBoxInt(mWorldBoundingBox.min.x, mWorldBoundingBox.min.y, mWorldBoundingBox.max.x, mWorldBoundingBox.max.y);

}


void CompositeSprite::updateBoundingBox()
{

    if(mBoundingBoxUpdateRequired==false) return;

    if(sourceSprite()->name()=="sprComp1"){
        DummyFunction();
    }

   // Sprite::ResetAnimatedProperties();        // ???

    mWorldBoundingBox.setInitialSize();
    for(SpriteLayer *sl : mMap->spriteLayers()){
        sl->updateBoundingBox();
        mWorldBoundingBox.Unite(sl->worldBoundingBox());
    }
    //if(mWorldBoundingBox.IsInitialSize()){
    //    mWorldBoundingBox.setEmptySize();
    //}


#ifdef JUGI_EDITOR

    ncine::Matrix4x4f m = mNode->worldMatrix();
    //BoundingBoxFloat rect(map()->mBoundingBox.min.x, map()->mBoundingBox.min.y, map()->mBoundingBox.max.x, map()->mBoundingBox.max.y);
    //BoundingBoxFloat rect(0,0, mWorldBoundingBox.GetWidth(), mWorldBoundingBox.GetHeight());
    //BoundingBoxFloat srcRect = sourceSprite()->sourceComposedSprite()->map()->boundingBox();
    //BoundingBoxFloat rect(srcRect.min.x, srcRect.min.y, srcRect.max.x, srcRect.max.y);

    //BoundingBoxFloat scsBB = sourceSprite()->sourceCompositeSprite()->map()->boundingBox();
    BoundingBoxFloat rect = sourceSprite()->sourceCompositeSprite()->map()->boundingBox();
    //BoundingBoxFloat rect(0.0f, 0.0f, scsBB.GetWidth(), scsBB.GetHeight());

    mBox[0] = jugimap::transform(Vec2f( rect.min.x, rect.min.y ), m);
    mBox[1] = jugimap::transform(Vec2f( rect.max.x, rect.min.y ), m);
    mBox[2] = jugimap::transform(Vec2f( rect.max.x, rect.max.y ), m);
    mBox[3] = jugimap::transform(Vec2f( rect.min.x, rect.max.y ), m);

    //----
    /*
    BoundingBoxInt mOuterBoundingBox;
    mOuterBoundingBox.setInitialSize();
    for(int i=0; i<mBox.size(); i++){
        if(mOuterBoundingBox.min.x > mBox[i].x) mOuterBoundingBox.min.x = mBox[i].x;
        if(mOuterBoundingBox.min.y > mBox[i].y) mOuterBoundingBox.min.y = mBox[i].y;
        if(mOuterBoundingBox.max.x < mBox[i].x) mOuterBoundingBox.max.x = mBox[i].x;
        if(mOuterBoundingBox.max.y < mBox[i].y) mOuterBoundingBox.max.y = mBox[i].y;
    }
    */

#endif

    mBoundingBoxUpdateRequired = false;

}


void CompositeSprite::resetAnimatedProperties()
{

    Sprite::resetAnimatedProperties();

    for(SpriteLayer *sl : mMap->spriteLayers()){
        for(Sprite *s : sl->sprites()){
            s->resetAnimatedProperties();
        }
    }
}


Vec2f CompositeSprite::positionFromAbsPosition(const Vec2f &_absPos)
{
    return jugimap::transform(_absPos, mNode->worldMatrix().inverse());
}



/*
void CompositeSprite::copyLayers(CompositeSprite *csSource, CompositeSprite *csDest, int &zOrder)
{

    //SourceSpriteReplacements * ssrChildren = csDest->mSSRChildren;
    std::vector<ChildSourceObjectReplacement> &CSSreplacements = csDest->mChildrenSourceSpriteReplacements;


    //--- SPRITE LAYERS
    for(Layer* le : csSource->mMap->layers()){

        if(le->kind()==LayerKind::SPRITE_LAYER){

            SpriteLayer * sl = static_cast<SpriteLayer*>(le);

            SpriteLayer *lCopy = new SpriteLayer(sl->name(), csSource->mMap);
            lCopy->mMap = csDest->mParentLayer->mMap;                // Using csDest because csSource can be SourceComposedSprite where the map is null.
            lCopy->mParentComposedSprite = csDest;
            lCopy->mKind = sl->mKind;
            lCopy->mName = sl->mName;
            lCopy->mParameters = sl->mParameters;

            //--- set depth when building
            lCopy->mZOrder = zOrder;
            zOrder += settings.GetZOrderStep();

            //DbgOutput("CS Layer:"+lCopy->GetName() + " zOrder:" + std::to_string(lCopy->zOrder));

            //----
            for(Sprite* s: sl->sprites()){

                Sprite *sCopy = jugimap::Sprite::makeSpriteByKind(s->type());
                sCopy->_setSourceSprite(s->sourceSprite());
                sCopy->_setLayer(lCopy);
                //sCopy->mParentComposedSprite = lCopy->parentComposedSprite;
                //sCopy->_setParentComposedSprite(lCopy->parentComposedSprite());
                sCopy->copyProperties(s, Property::ALL);

                //if(ssrChildren && sCopy->name()!=""){
                //    ESourceObject *ss = ssrChildren->FindSourceSpriteForChild(sCopy->name());
                //    if(ss){
                //        sCopy->_setSourceSpriteReplacement(new SourceSpriteReplacements(ss, "", sCopy->sourceSprite()));
                //    }
                //}
                if(CSSreplacements.empty()==false && sCopy->name()!=""){
                    SourceSprite *ss = ChildSourceObjectReplacement::FindChildSourceSprite(CSSreplacements, sCopy->name());
                    if(ss){
                        //sCopy->_setSourceSpriteReplacement(new SourceSpriteReplacements(ss, "", sCopy->sourceSprite()));
                        sCopy->_setSourceSpriteReplacement(ss);
                    }
                }

                if(s->type()==SpriteType::COMPOSED){
                    CompositeSprite *cs = static_cast<CompositeSprite*>(s);
                    CompositeSprite *csCopy = static_cast<CompositeSprite*>(sCopy);
                    csCopy->mMap->_setName(cs->mMap->name());
                    csCopy->mHandle = cs->sourceSprite()->sourceComposedSprite()->handle();
                    copyLayers(cs->sourceSprite()->sourceComposedSprite(), csCopy, zOrder);
                    //copyLayers(cs, csCopy, zOrder);
                }

                //---- store sprite
                //ncine::SceneNode* nCopy = castSpriteToSceneNode(sCopy);
                //lCopy->node()->addChildNode(nCopy);
                lCopy->addSprite(sCopy);
            }

            //csDest->mNode->addChildNode(lCopy->node());
            csDest->map()->addChildLayer(lCopy);


        }else if(le->kind()==LayerKind::VECTOR_LAYER){

            VectorLayer * vl = static_cast<VectorLayer*>(le);

            //VectorLayer *vlCopy = objectFactory->NewVectorLayer();  // new VectorLayer();
            VectorLayer *vlCopy = new VectorLayer(vl->name(), csSource->mMap);
            vlCopy->mParentComposedSprite = csDest;
            vlCopy->mKind = vl->mKind;
            vlCopy->mName = vl->mName;
            vlCopy->mParameters = vl->mParameters;

            vlCopy->mZOrder = zOrder;
            zOrder += settings.GetZOrderStep();


            for(VectorShape *vs : vl->shapes()){
                //VectorShape *vsCopy = new VectorShape(*vs);
                //VectorShape *vsCopy = VectorShape::Copy(vs);
                VectorShape *vsCopy = new VectorShape(*vs);
                vlCopy->AddVectorShape(vsCopy);
            }
            //csDest->layers.push_back(vlCopy);

            csDest->mNode->addChildNode(vlCopy->node());

        }
    }
}
*/



void CompositeSprite::copyLayers(CompositeSprite *csSource, CompositeSprite *csDest)
{

    //std::vector<ChildSourceObjectReplacement> &CSSreplacements = csDest->mChildrenSourceSpriteReplacements;

    csDest->map()->_setTileWidth(csSource->map()->tileWidth());
    csDest->map()->_setTileHeight(csSource->map()->tileHeight());
    csDest->map()->_setNumTilesX(csSource->map()->numTilesX());
    csDest->map()->_setNumTilesY(csSource->map()->numTilesY());


    //--- SPRITE LAYERS
    for(Layer* le : csSource->mMap->layers()){

        if(le->kind()==LayerKind::SPRITE_LAYER){

            SpriteLayer * sl = static_cast<SpriteLayer*>(le);

            SpriteLayer *lCopy = new SpriteLayer(sl->name(), csSource->mMap);
            //lCopy->mMap = csDest->mParentLayer->mMap;                // Using csDest because csSource can be SourceComposedSprite where the map is null.
            lCopy->mMap = csDest->mMap;                                // Using csDest because csSource can be SourceComposedSprite where the map is null.
            lCopy->mParentCompositeSprite = csDest;
            lCopy->mKind = sl->mKind;
            lCopy->mName = sl->mName;
            lCopy->mParameters = sl->mParameters;
            lCopy->hidden = sl->hidden;
            //---
            lCopy->mWorldBoundingBox = sl->mWorldBoundingBox;
            lCopy->mLayerType = sl->mLayerType;
            lCopy->mLayersPlaneSize = sl->mLayersPlaneSize;
            lCopy->mParallaxOffset = sl->mParallaxOffset;
            lCopy->mPP = sl->mPP;
            //---
            lCopy->mAlpha = sl->mAlpha;                                  // Transparency for sprites: 0 - 1.0
            lCopy->mBlend = sl->mBlend;                                 // Blending mode for sprites: SOLID, ALPHA, ADDITIVE
            lCopy->mContent = sl->mContent;

            //----
            for(Sprite* s: sl->sprites()){

                Sprite *sCopy = jugimap::Sprite::makeSpriteByKind(s->type());
                sCopy->_setSourceSprite(s->sourceSprite());
                sCopy->_setParentLayer(lCopy);
                sCopy->copyProperties(s, Property::ALL);

                //if(CSSreplacements.empty()==false && sCopy->name()!=""){
                //    SourceSprite *ss = ChildSourceObjectReplacement::FindChildSourceSprite(CSSreplacements, sCopy->name());
                //    if(ss){
                //        sCopy->_setSourceSpriteReplacement(ss);
                //     }
                //}
                if(s->type()==SpriteType::COMPOSITE){
                    CompositeSprite *cs = static_cast<CompositeSprite*>(s);
                    CompositeSprite *csCopy = static_cast<CompositeSprite*>(sCopy);
                    csCopy->mMap->_setName(cs->mMap->name());
                    //csCopy->mHandle = cs->sourceSprite()->sourceCompositeSprite()->handle();
                    csCopy->mHandle = cs->sourceSprite()->sourceCompositeSprite()->mHandleForMapCompositeSprites;
                    copyLayers(cs->sourceSprite()->sourceCompositeSprite(), csCopy);
                }
                lCopy->addSprite(sCopy);

            }

            csDest->map()->addChildLayer(lCopy);


        }else if(le->kind()==LayerKind::VECTOR_LAYER){

            VectorLayer * vl = static_cast<VectorLayer*>(le);
            VectorLayer *vlCopy = new VectorLayer(vl->name(), csSource->mMap);
            vlCopy->mMap = csDest->mMap;                                // Using csDest because csSource can be SourceComposedSprite where the map is null.
            vlCopy->mParentCompositeSprite = csDest;
            vlCopy->mKind = vl->mKind;
            vlCopy->mName = vl->mName;
            vlCopy->mParameters = vl->mParameters;
            vlCopy->hidden = vl->hidden;

            for(VectorShape *vs : vl->shapes()){
                VectorShape *vsCopy = new VectorShape(*vs);
                vsCopy->_setParentLayer(vlCopy);
                vlCopy->AddVectorShape(vsCopy);
            }

            //csDest->mNode->addChildNode(vlCopy->node());

            //--- vector layer must be put into the vector layers group !
            VectorLayersGroup *vlg = nullptr;
            for(ncine::SceneNode* n: csDest->mMap->nodeChildren()){
                LayerElement *l = static_cast<LayerNode*>(n)->layerElement;
                if(l->kind()==LayerKind::VECTOR_LAYERS_GROUP){
                    vlg = static_cast<VectorLayersGroup*>(l);
                }
            }
            if(vlg==nullptr){
                vlg = new VectorLayersGroup("Vector layers");
                csDest->mMap->addChildLayer(vlg);
            }

            vlg->addChildLayer(vlCopy);

        }


    }
}



void CompositeSprite::copyProperties(Sprite *_sprite, int copyFlags)
{
    Sprite::copyProperties(_sprite, copyFlags);

    CompositeSprite * cs = dynamic_cast<CompositeSprite*>(_sprite);
    if(cs==nullptr) return;


    mHandleForMapCompositeSprites = cs->mHandleForMapCompositeSprites;
    mNestingDepth = cs->mNestingDepth;


}


/*
void CompositeSprite::setEngineNodesEnabled(bool _forceEnabled)
{

    if(_forceEnabled){
        node()->setEnabled(true);
    }else{
        node()->setEnabled(mVisible);
    }

    for(SpriteLayer *sl : mMap->spriteLayers()){
        sl->setEngineNodesEnabled(_forceEnabled);
    }

}
*/


//-----

void _GetComposedSpritesDepth(CompositeSprite* cs, int currentDepth, int &maxDepth)
{

    maxDepth = std::max(maxDepth, currentDepth);

    for(SpriteLayer* sl : cs->map()->spriteLayers()){
        for(Sprite *s : sl->sprites()){
            if(s->sourceSprite()->sourceCompositeSprite()){
                _GetComposedSpritesDepth(s->sourceSprite()->sourceCompositeSprite(), currentDepth+1, maxDepth);
            }
        }
    }
}


void CompositeSprite::ObtainNestingDepth()
{
    int currentDepth = 0;
    mNestingDepth = 0;
    _GetComposedSpritesDepth(this, currentDepth, mNestingDepth);

}



}



