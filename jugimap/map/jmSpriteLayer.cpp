#include "jmSystem.h"
#include "jmCommonFunctions.h"
#include "jmSystemSourceObjects.h"
#include "jmSourceSprite.h"
#include "jmSourceItem.h"
#include "jmCamera.h"
#include "jmMap.h"
#include "jmTextSprite.h"
#include "jmCompositeSprite.h"
#include "jmSpriteModifiers.h"
#include "jmStandardSprite.h"
#include "jmSpriteLayer.h"



namespace jugimap {





#ifndef JUGI_EDITOR

SpriteLayersGroup::SpriteLayersGroup()
{

    mKind = LayerKind::SPRITE_LAYERS_GROUP;
    mNode = new LayerNode(this);

}

SpriteLayersGroup::SpriteLayersGroup(const std::string& _nameID) : LayerElement(_nameID)
{
    mKind = LayerKind::SPRITE_LAYERS_GROUP;
    mNode = new LayerNode(this);

}


void SpriteLayersGroup::rebuildQuickAccessContainers()
{
    assert(parent() && parent()->kind()==LayerKind::MAP);
    Map *m = static_cast<Map*>(parent());
    m->rebuildQuickAccessContainers();
}

#endif





//--------------------------------------------------------------------------------------

SpriteLayer::SpriteLayer() : Layer()
{
    SpriteLayerInit();
}

SpriteLayer::SpriteLayer(Map *_map) : Layer(_map)
{
    SpriteLayerInit();

}


SpriteLayer::SpriteLayer(const std::string &_name, jugimap::Map *_map) : Layer(_name, _map)
{
    SpriteLayerInit();
}


SpriteLayer::SpriteLayer(const SpriteLayer &_SrcLayer) : Layer(_SrcLayer)
{

    SpriteLayerInit();

    //---
    mAlpha = _SrcLayer.mAlpha;
    mBlend = _SrcLayer.mBlend;

    //mMap = _SrcLayer.mMap;

    for(Sprite *o : _SrcLayer.mSprites){
        //Sprite *oCopy = new Sprite(*o);
        Sprite *oCopy = o->makePassiveCopy();
        addSprite(oCopy);
        //mSprites.push_back(oCopy);
        //oCopy->iter = mSprites.end();
        //oCopy->iter--;
        //oCopy->mParentLayer = this;
    }


    mContent = _SrcLayer.mContent;

#ifdef JUGI_EDITOR
    //mBlending = _SrcLayer.mBlending;
    mIncludedInMultiLayerBrush = _SrcLayer.mIncludedInMultiLayerBrush;
    mTilesBoundingBox = _SrcLayer.mTilesBoundingBox;

    RebuildGrid(_SrcLayer.mNumTilesX, _SrcLayer.mNumTilesY);
    AddObjectsToGrid();

#endif

    //---
    //mBrushLayer = new SpriteLayer(&BrushMap);
    //mBrushLayer->mContent = mContent;
    //mBrushLayer->nameID = _SrcLayer.nameID + " brush layer";
    //mBrushLayer->RebuildGrid(mNumTilesX, mNumTilesY);
}


SpriteLayer::~SpriteLayer()
{
#ifndef JUGI_EDITOR
    deleteAllSprites();
#else
    onDeconstruct();
#endif
}


void SpriteLayer::deleteContent()
{
    deleteAllSprites();

    Layer::deleteContent();
}


void SpriteLayer::init()
{

    updatePropertiesViaExtraParameters();

    //---
    for(Sprite* s : mSprites){
        s->initEngineObjects();
    }

    //---
    if(hidden){
        node()->setDrawEnabled(false);
    }
}


void SpriteLayer::clearExternalDataLinks()
{

    for(Sprite* s : mSprites){
        s->clearExternalDataLinks();
    }

}


void SpriteLayer::updatePropertiesViaExtraParameters()
{

    Layer::updatePropertiesViaExtraParameters();


    bool alphaSet = false;
    bool blendSet = false;

    for(CParameter &PV : parameters()){
        if(PV.active == false) continue;

        if(PV.name=="alpha"){

            //mAlpha = StdString::stringToFloat(PV.value, 1.0);
            setAlpha(StdString::stringToFloat(PV.value, 1.0));

#ifdef JUGI_EDITOR
            if(mBrushLayer){
                mBrushLayer->setAlpha(mAlpha);
            }
#endif
            alphaSet = true;

        }else if(PV.name=="blending"){

            if(PV.value=="ALPHA"){
                //mBlend = Blend::ALPHA;
                setBlend(Blend::ALPHA);

            }else if(PV.value=="MULTIPLY"){
                //mBlend = Blend::MULTIPLY;
                setBlend(Blend::MULTIPLY);

            }else if(PV.value=="LINEAR DODGE"){
                //mBlend = Blend::ADDITIVE;
                setBlend(Blend::ADDITIVE);

            }else if(PV.value=="SOLID COVER"){
                //mBlend = Blend::SOLID;
                setBlend(Blend::SOLID);
            }

#ifdef JUGI_EDITOR
            if(mBrushLayer){
                mBrushLayer->setBlend(mBlend);
            }
#endif

            blendSet = true;
        }
    }


    if(alphaSet==false && AreEqual(mAlpha, 1.0f)==false){
        setAlpha(1.0f);
#ifdef JUGI_EDITOR
        if(mBrushLayer){
            mBrushLayer->setAlpha(1.0f);
        }
#endif
    }

    if(blendSet==false && mBlend!= Blend::ALPHA){
        setBlend(Blend::ALPHA);
#ifdef JUGI_EDITOR
        if(mBrushLayer){
            mBrushLayer->setBlend(Blend::ALPHA);
        }
#endif
    }



}


void SpriteLayer::updateCameraDependentObjects()
{

    if(node()->isEnabled()==false) return;

    if(map()){

        if(map()->type()==MapType::PARALLAX){

            if(layerType()==LayerType::PARALLAX){
                if(UpdateParallaxOffset()){

                    for(Sprite *s : mSprites){
                        s->setPosition(s->position( Sprite::IncludedPositions::ANI_TRANSLATION ));
                    }

                }

            }else if(layerType()==LayerType::PARALLAX_STRETCHING_SINGLE_SPRITE){
                if(updateStretchingSingleSpriteLayer()){
                    //AppendToSpritesChangeFlag(Sprite::Property::TRANSFORMATION);
                }
            }


        }else if(map()->type()==MapType::SCREEN){

            if(layerType()==LayerType::SCREEN_STRETCHING_SINGLE_SPRITE){
                if(updateStretchingSingleSpriteLayer()){
                    //AppendToSpritesChangeFlag(Sprite::Property::TRANSFORMATION);
                }
            }
        }
    }
}


bool SpriteLayer::updateStretchingSingleSpriteLayer()
{

    //if(stretchSet) return false;

    if(mNode->children().isEmpty()) return false;

    //StandardSprite  *s = dynamic_cast<StandardSprite *>(castNodeToSprite(mNode->children().front()));
    StandardSprite  *s = dynamic_cast<StandardSprite *>(mSprites.front());
    if(s==nullptr || s->activeImage()==nullptr) return false;


    Vec2f spriteSize = Vec2iToVec2f(s->activeImage()->size());
    Vec2f spriteHandle(spriteSize.x/2.0, spriteSize.y/2.0);
    Vec2f spritePosition;

    //---
    Vec2f rectSize;

    if(map()->type()==MapType::PARALLAX){

        WorldMapCamera *camera = dynamic_cast<WorldMapCamera*>(map()->camera());
        assert(camera);

        //if(GetStretchingVariant()==StretchingVariant::XY_TO_WORLD_SIZE){
        if(mPP.stretch==ParallaxAndScreenLayerParameters::stretchXYToWorld){
            rectSize = Vec2f(camera->GetWorldSize().x, camera->GetWorldSize().y);
            spritePosition = rectSize/2.0;

        //}else if(GetStretchingVariant()==StretchingVariant::XY_TO_VIEWPORT_SIZE){
        }else if(mPP.stretch==ParallaxAndScreenLayerParameters::stretchXYToView){
            rectSize = camera->GetViewport().GetSize();
            spritePosition = camera->GetPointedPosition();
        }

    }else if(map()->type()==MapType::SCREEN){
        rectSize = Vec2f(map()->screenMapDesignSize().x, map()->screenMapDesignSize().y);
        spritePosition = rectSize/2.0;
    }

    Vec2f spriteScale = rectSize / spriteSize;



    if(IsEqual(spriteHandle, s->handle())==false || IsEqual(spritePosition, s->position())==false || IsEqual(spriteScale, s->scale())==false){

        s->setHandle(spriteHandle);
        s->setPosition(spritePosition);
        s->setScale(spriteScale);

        dbgNcSprite = static_cast<StandardSpriteNode*>(s->node());

        //stretchSet = true;
        return true;
    }

    return false;
}

/*
void SpriteLayer::setVisible(bool _visible)
{

    mVisible = _visible;

    mNode->setEnabled(mVisible);

}
*/


void SpriteLayer::setAlpha(float _alpha)
{

    mAlpha = _alpha;

    for(Sprite* s : mSprites){
        s->setAlpha(s->alpha());         // layer alpha will be multiplied inside the function
    }

}


void SpriteLayer::setBlend(Blend _blend)
{
    mBlend = _blend;


    for(Sprite* s : mSprites){
        if(s->ownBlend()==Blend::NOT_DEFINED){
            s->setOwnBlend(Blend::NOT_DEFINED);      // in this case the blend of the parent layer will be used
        }
    }

}


float SpriteLayer::alpha(bool _includingParentAlpha)
{
    if(_includingParentAlpha && parentCompositeSprite()){
        return mAlpha * parentCompositeSprite()->alpha();
    }else{
        return mAlpha;
    }
}


void SpriteLayer::addSprite(Sprite* _sprite)
{

    mSprites.push_back(_sprite);
    _sprite->iter = mSprites.end(); _sprite->iter--;

    mNode->addChildNode(_sprite->node());

    _sprite->_setParentLayer(this);

}


void SpriteLayer::insertSprite(Sprite* _sprite, int _pos)
{

    if(_pos>=0 && _pos<mSprites.size()){



    }else{
        addSprite(_sprite);
    }

}




StandardSprite * SpriteLayer::addStandardSprite(SourceSprite *_sourceSprite, Vec2f _position)
{
    assert(_sourceSprite->kind()==SpriteType::STANDARD);

    StandardSprite *s = new StandardSprite();
    s->_setSourceSprite(_sourceSprite);
    s->_setParentLayer(this);
    s->setPosition(_position);
    s->initEngineObjects();

    addSprite(s);
    return s;
}


TextSprite * SpriteLayer::addTextSprite(const std::string &_textString, Font* _font, ColorRGBA _color, Vec2f _position, TextHandleVariant _textHandle)
{

    TextSprite* ts = new TextSprite();
    ts->_setSourceSprite(dummySourceObjects.SStextSourceSprite);
    ts->_setParentLayer(this);
    ts->setFont(_font);
    ts->setColor(_color);
    ts->setText(_textString);
    ts->setRelativeHandle(GetRelativeHandleFromTextHandleVariant(_textHandle, settings.yOrientation()));
    ts->setPosition(_position);
    ts->_setInitialized(true);
    //ts->setChangeFlags(Sprite::Property::ALL);

    addSprite(ts);

    return ts;
}


void SpriteLayer::deleteSprite(Sprite *_sprite)
{

    //ncine::SceneNode *n = castSpriteToSceneNode(_sprite);
    //delete n;

    //delete _sprite->node();

    if(_sprite->parentLayer()!=this) return;

    mNode->removeChildNode(_sprite->node());
    mSprites.erase(_sprite->iter);

    delete _sprite;

}


bool SpriteLayer::takeSprite(Sprite *_sprite)
{

    if(_sprite->parentLayer()!=this) return false;

    mNode->removeChildNode(_sprite->node());
    mSprites.erase(_sprite->iter);

    return true;

}




void SpriteLayer::deleteAllSprites()
{

    node()->removeAllChildrenNodes();

    for(Sprite* s :mSprites){
        delete s;
    }
    mSprites.clear();

}


void SpriteLayer::setBoundingBoxUpdateRequired(bool _value)
{

    mBoundingBoxUpdateRequired = _value;
    if(map()){
        map()->setBoundingBoxUpdateRequired(_value);
    }
    if(parentCompositeSprite()){
        parentCompositeSprite()->setParentBoundingBoxUpdateRequired(_value);
    }

}


void SpriteLayer::updateBoundingBox(int flags)
{

    if(mBoundingBoxUpdateRequired == false) return;

    mWorldBoundingBox.setInitialSize();

    for(Sprite *s : mSprites){
        if(s->type()==SpriteType::COMPOSITE){
            s->updateBoundingBox();
        }
        mWorldBoundingBox.Unite(s->boundingBox());
    }


#ifdef JUGI_EDITOR

    //if(mSprites.size()==0){
    //    mTilesBoundingBox.setEmptySize();
    //}else{
        mTilesBoundingBox.setInitialSize();
        for(Sprite *o : mSprites){
            mTilesBoundingBox.Expand(Vec2i(o->xt, o->yt));
        }
    //}
#endif

    mBoundingBoxUpdateRequired = false;
}


/*
void SpriteLayer::setEngineNodesEnabled(bool _forceEnabled)
{

    if(_forceEnabled){
        mNode->setEnabled(true);
    }else{
        mNode->setEnabled(!hidden);
    }

    for(Sprite* s : mSprites){
        s->setEngineNodesEnabled(_forceEnabled);
    }
}
*/


void SpriteLayer::eUpdateTextSprites_SetTexts(const SM_ChangeChildText &_modifier, int &_numUpdated)
{


    for(Sprite *s : mSprites){

        if(s->type()==SpriteType::TEXT){
            if(s->name()==_modifier.childSpriteTag){
                TextSprite *ts = static_cast<TextSprite*>(s);
                if(_modifier.text.empty()==false){
                    ts->setText(_modifier.text);
                }else{
                    ts->setTextFromBook(_modifier.bookName, _modifier.textSegmentID);
                }
                _numUpdated++;
            }

        }else if(s->type()==SpriteType::COMPOSITE){
            CompositeSprite *cs = static_cast<CompositeSprite*>(s);
            cs->map()->eUpdateTextSprites_SetTexts(_modifier, _numUpdated);
        }
    }
}



void SpriteLayer::updateSprites_SetParameters(const SM_ChangeChildParameter &_modifier, int &_numUpdated)
{
    for(Sprite *s : mSprites){
        if(s->name()==_modifier.childSpriteTag){
            for(CParameter &p : s->parameters().list()){
                if(p.name==_modifier.parameterName){
                    p.value = _modifier.parameterValue;
                    _numUpdated++;
                    break;
                }
            }

        }else if(s->type()==SpriteType::COMPOSITE){
            CompositeSprite *cs = static_cast<CompositeSprite*>(s);
            cs->map()->updateSprites_SetParameters(_modifier, _numUpdated);
        }
    }

}


void SpriteLayer::changeEllipsesToBezierCurves(int _dataFlag)
{

    for(Sprite *s : mSprites){
        if(s->type()==SpriteType::COMPOSITE){
            CompositeSprite *cs = static_cast<CompositeSprite*>(s);
            cs->map()->changeEllipsesToBezierCurves(_dataFlag);
        }
    }
}


/*
int SpriteLayer::findLastZOrder()
{

    if(mNode->children().isEmpty()) return zOrder();

    //int zOrder = zOrder();
    int zOrder = mZOrder;

    for(ncine::SceneNode* n : nodeChildren()){
        Sprite* s = castNodeToSprite(n);
        if(s->type()==SpriteType::COMPOSED){
            CompositeSprite *cs = static_cast<CompositeSprite*>(s);

            for(ncine::SceneNode *n : cs->mapNodeChildren()){
                LayerElement *l = static_cast<LayerNode*>(n)->layerElement;
                if(l->kind()==LayerKind::SPRITE_LAYER){
                    int z = static_cast<SpriteLayer*>(l)->findLastZOrder();
                    if(settings.GetZOrderStep()>0){
                        zOrder = std::max(zOrder, z);
                    }else{
                        zOrder = std::min(zOrder, z);
                    }
                }
            }
        }
    }

    return zOrder;
}
*/


#ifndef JUGI_EDITOR

void SpriteLayer::SpriteLayerInit()
{
    mKind = LayerKind::SPRITE_LAYER;
    mNode = new LayerNode(this);

    //---
    mWorldBoundingBox.setInitialSize();
}





#endif



}
