#include <string>
#include <sstream>
#include "ncine/SceneNode.h"
#include "jmGlobal.h"
#include "jmParameter.h"
#include "jmSourceContainers.h"
#include "jmSourceSprite.h"
#include "jmFrameAnimation.h"
#include "jmTimelineAnimation.h"
#include "jmSpriteLayer.h"
#include "jmVectorLayer.h"
#include "jmMap.h"
#include "jmScene.h"
#include "jmSceneLayout.h"
#include "jmCompositeSprite.h"
#include "jmStandardSprite.h"
#include "jmVectorShape.h"
#include "jmGuiCommon.h"
#include "jmUtilities.h"


namespace jugimap {


//--- COLLECT LAYERS

void CollectLayersWithParameter(Map *map, std::vector<Layer*>&collectedLayers,  const std::string &pName, const std::string &pValue, LayerKind layerKind)
{

    for(Layer* l : map->layers()){
        if(layerKind==LayerKind::NOT_DEFINED || layerKind==l->kind()){
            if(l->parameters().exists(pName, pValue)){
                collectedLayers.push_back(l);
            }
            if(l->kind()==LayerKind::SPRITE_LAYER){
                SpriteLayer *sl = static_cast<SpriteLayer*>(l);

                for(Sprite *s : sl->sprites()){
                    if(s->type()==SpriteType::COMPOSITE){
                        CompositeSprite *cs = static_cast<CompositeSprite*>(s);
                        CollectLayersWithParameter(cs->map(), collectedLayers, pName, pValue, layerKind);
                    }
                }
            }
        }
    }
}


Layer* FindLayerWithName(Map *map, const std::string &name, LayerKind layerKind)
{

    for(Layer* l : map->layers()){
        if(layerKind==LayerKind::NOT_DEFINED || layerKind==l->kind()){
            if(l->name()==name){
                return l;
            }
            if(l->kind()==LayerKind::SPRITE_LAYER){
                SpriteLayer *sl = static_cast<SpriteLayer*>(l);

                for(Sprite *s : sl->sprites()){
                    if(s->type()==SpriteType::COMPOSITE){
                        CompositeSprite *cs = static_cast<CompositeSprite*>(s);
                        Layer *l = FindLayerWithName(cs->map(), name, layerKind);
                        if(l){
                            return l;
                        }
                    }
                }
            }
        }
    }

    return nullptr;
}


void CollectLayersWithName(Map *map, std::vector<Layer*>&collectedLayers,  const std::string &name, LayerKind layerKind)
{

    for(Layer* l : map->layers()){
        if(layerKind==LayerKind::NOT_DEFINED || layerKind==l->kind()){

            if(l->name()==name){
                collectedLayers.push_back(l);
            }
            if(l->kind()==LayerKind::SPRITE_LAYER){
                SpriteLayer *sl = static_cast<SpriteLayer*>(l);

                for(Sprite *s : sl->sprites()){

                    if(s->type()==SpriteType::COMPOSITE){
                        CompositeSprite *cs = static_cast<CompositeSprite*>(s);
                        CollectLayersWithName( cs->map(), collectedLayers, name, layerKind);
                    }
                }
            }
        }
    }

}




//--- COLLECT SPRITES


void CollectSpritesWithId(Map *map, std::vector<Sprite*>&collectedSprites,  int id)
{
    //for(ncine::SceneNode* n : map->nodeChildren()){
    //    BaseLayer *l = static_cast<LayerNode*>(n)->baseLayer;
    for(Layer* l : map->layers()){

        if(l->kind()==LayerKind::SPRITE_LAYER){
            SpriteLayer * sl = static_cast<SpriteLayer*>(l);
            CollectSpritesWithId(sl, collectedSprites, id);
        }
    }
}


void CollectSpritesWithId(CompositeSprite *composedSprite, std::vector<Sprite*>&collectedSprites,  int id)
{
    for(ncine::SceneNode* n : composedSprite->mapNodeChildren()){
        LayerElement *l = static_cast<LayerNode*>(n)->layerElement;

        if(l->kind()==LayerKind::SPRITE_LAYER){
            SpriteLayer * sl = static_cast<SpriteLayer*>(l);
            CollectSpritesWithId(sl, collectedSprites, id);
        }
    }
}


void CollectSpritesWithName(Scene *scene, std::vector<Sprite*>&collectedSprites,  const std::string &name)
{

    for(SceneMap *sm : scene->sceneMaps()){
        CollectSpritesWithName(sm->map(), collectedSprites, name);
    }
}


void CollectSpritesWithName(Map *map, std::vector<Sprite*>&collectedSprites,  const std::string &name)
{

    //for(ncine::SceneNode* n : map->nodeChildren()){
    //    Layer *l = static_cast<LayerNode*>(n)->baseLayer;
    for(Layer* l : map->layers()){
        if(l->kind()==LayerKind::SPRITE_LAYER){
            SpriteLayer * sl = static_cast<SpriteLayer*>(l);
            CollectSpritesWithName(sl, collectedSprites, name);
        }
    }
}


void CollectSpritesWithName(CompositeSprite *composedSprite, std::vector<Sprite*>&collectedSprites,  const std::string &name)
{

    for(ncine::SceneNode* n : composedSprite->mapNodeChildren()){
        LayerElement *l = static_cast<LayerNode*>(n)->layerElement;

        if(l->kind()==LayerKind::SPRITE_LAYER){
            SpriteLayer * sl = static_cast<SpriteLayer*>(l);
            CollectSpritesWithName(sl, collectedSprites, name);
        }
    }
}


void CollectSpritesOfKind(Map *map, std::vector<Sprite*>&collectedSprites,  SpriteType kind)
{

    //for(ncine::SceneNode* n : map->nodeChildren()){
    //    Layer *l = static_cast<LayerNode*>(n)->baseLayer;
    for(Layer* l : map->layers()){
        if(l->kind()==LayerKind::SPRITE_LAYER){
            SpriteLayer * sl = static_cast<SpriteLayer*>(l);
            CollectSpritesOfKind(sl, collectedSprites, kind);
        }
    }
}


void CollectSpritesOfKind(CompositeSprite *composedSprite, std::vector<Sprite*>&collectedSprites,  SpriteType kind)
{

    for(ncine::SceneNode* n : composedSprite->mapNodeChildren()){
        LayerElement *l = static_cast<LayerNode*>(n)->layerElement;

        if(l->kind()==LayerKind::SPRITE_LAYER){
            SpriteLayer * sl = static_cast<SpriteLayer*>(l);
            CollectSpritesOfKind(sl, collectedSprites, kind);
        }
    }
}


void CollectSpritesWithParameter(Map *map, std::vector<Sprite *> &collectedSprites,  const std::string &pName, const std::string &pValue)
{
    //for(ncine::SceneNode* n : map->nodeChildren()){
    //    Layer *l = static_cast<LayerNode*>(n)->baseLayer;
    for(Layer* l : map->layers()){
        if(l->kind()==LayerKind::SPRITE_LAYER){
            SpriteLayer * sl = static_cast<SpriteLayer*>(l);
            CollectSpritesWithParameter(sl, collectedSprites, pName, pValue);
        }
    }
}


void CollectSpritesWithParameter(CompositeSprite *composedSprite, std::vector<Sprite *> &collectedSprites,  const std::string &pName, const std::string &pValue)
{

    for(ncine::SceneNode* n : composedSprite->mapNodeChildren()){
        LayerElement *l = static_cast<LayerNode*>(n)->layerElement;

        if(l->kind()==LayerKind::SPRITE_LAYER){
            SpriteLayer * sl = static_cast<SpriteLayer*>(l);
            CollectSpritesWithParameter(sl, collectedSprites, pName, pValue);
        }
    }
}


void CollectSpritesWithConstParameter(Map *map, std::vector<Sprite *> &collectedSprites,  const std::string &pName, const std::string &pValue)
{

    //for(ncine::SceneNode* n : map->nodeChildren()){
    //    Layer *l = static_cast<LayerNode*>(n)->baseLayer;
    for(Layer* l : map->layers()){
        if(l->kind()==LayerKind::SPRITE_LAYER){
            SpriteLayer * sl = static_cast<SpriteLayer*>(l);
            CollectSpritesWithConstParameter(sl, collectedSprites, pName, pValue);
        }
    }
}


void CollectSpritesWithConstParameter(CompositeSprite *composedSprite, std::vector<Sprite *> &collectedSprites,  const std::string &pName, const std::string &pValue)
{

    for(ncine::SceneNode* n : composedSprite->mapNodeChildren()){
        LayerElement *l = static_cast<LayerNode*>(n)->layerElement;

        if(l->kind()==LayerKind::SPRITE_LAYER){
            SpriteLayer * sl = static_cast<SpriteLayer*>(l);
            CollectSpritesWithConstParameter(sl, collectedSprites, pName, pValue);
        }
    }
}

/*
void CollectSpritesWithOrigConstParameter(Map *map, std::vector<Sprite *> &collectedSprites,  const std::string &pName, const std::string &pValue)
{

    //for(ncine::SceneNode* n : map->nodeChildren()){
    //    Layer *l = static_cast<LayerNode*>(n)->baseLayer;
    for(Layer* l : map->layers()){
        if(l->kind()==LayerKind::SPRITE_LAYER){
            SpriteLayer * sl = static_cast<SpriteLayer*>(l);
            CollectSpritesWithOrigConstParameter(sl, collectedSprites, pName, pValue);
        }
    }
}


void CollectSpritesWithOrigConstParameter(CompositeSprite *composedSprite, std::vector<Sprite *> &collectedSprites,  const std::string &pName, const std::string &pValue)
{

    for(ncine::SceneNode* n : composedSprite->mapNodeChildren()){
        LayerElement *l = static_cast<LayerNode*>(n)->layerElement;

        if(l->kind()==LayerKind::SPRITE_LAYER){
            SpriteLayer * sl = static_cast<SpriteLayer*>(l);
            CollectSpritesWithOrigConstParameter(sl, collectedSprites, pName, pValue);
        }
    }
}
*/

void CollectSpritesWithDataFlags(Map *map, std::vector<Sprite*>&collectedSprites,  int dataFlags, bool compareDataFlagsAsBitmask)
{

    //for(ncine::SceneNode* n : map->nodeChildren()){
    //    Layer *l = static_cast<LayerNode*>(n)->baseLayer;
    for(Layer* l : map->layers()){
        if(l->kind()==LayerKind::SPRITE_LAYER){
            SpriteLayer * sl = static_cast<SpriteLayer*>(l);
            CollectSpritesWithDataFlags(sl, collectedSprites, dataFlags, compareDataFlagsAsBitmask);
        }
    }
}


void CollectSpritesWithDataFlags(CompositeSprite *composedSprite, std::vector<Sprite*>&collectedSprites,  int dataFlags, bool compareDataFlagsAsBitmask)
{

    for(ncine::SceneNode* n : composedSprite->mapNodeChildren()){
        LayerElement *l = static_cast<LayerNode*>(n)->layerElement;

        if(l->kind()==LayerKind::SPRITE_LAYER){
            SpriteLayer * sl = static_cast<SpriteLayer*>(l);
            CollectSpritesWithDataFlags(sl, collectedSprites, dataFlags, compareDataFlagsAsBitmask);
        }
    }
}


void CollectSpritesWithSourceSpriteName(Map *map, std::vector<Sprite*>&collectedSprites,  const std::string &name)
{

    for(SpriteLayer * sl : map->spriteLayers()){
        CollectSpritesWithSourceSpriteName(sl, collectedSprites, name);
    }

}


void CollectSpritesWithSourceSpriteName(CompositeSprite *compositeSprite, std::vector<Sprite*>&collectedSprites,  const std::string &name)
{

    for(SpriteLayer * sl : compositeSprite->map()->spriteLayers()){
        CollectSpritesWithSourceSpriteName(sl, collectedSprites, name);
    }

}


//---------------------------------------------------------------------------



void CollectSpritesWithId(SpriteLayer *spriteLayer, std::vector<Sprite*>&collectedSprites,  int id)
{

    for(Sprite *s : spriteLayer->sprites()){

        if(s->id()==id){
            collectedSprites.push_back(s);
        }
        if(s->type()==SpriteType::COMPOSITE){
            CompositeSprite *cs = static_cast<CompositeSprite*>(s);
            for(SpriteLayer * sl : cs->map()->spriteLayers()){
                CollectSpritesWithId(sl, collectedSprites, id);
            }
        }
    }

}


void CollectSpritesWithName(SpriteLayer *spriteLayer, std::vector<Sprite*>&collectedSprites,  const std::string &name)
{

    for(Sprite *s : spriteLayer->sprites()){

        if(s->name()==name){
            collectedSprites.push_back(s);
        }
        if(s->type()==SpriteType::COMPOSITE){
            CompositeSprite *cs = static_cast<CompositeSprite*>(s);
            for(SpriteLayer * sl : cs->map()->spriteLayers()){
                CollectSpritesWithName(sl, collectedSprites, name);
            }
        }
    }
}



void CollectSpritesOfKind(SpriteLayer *spriteLayer, std::vector<Sprite*>&collectedSprites,  SpriteType kind)
{

    for(Sprite *s : spriteLayer->sprites()){

        if(s->type()==kind){
            collectedSprites.push_back(s);
        }
        if(s->type()==SpriteType::COMPOSITE){
            CompositeSprite *cs = static_cast<CompositeSprite*>(s);
            for(SpriteLayer * sl : cs->map()->spriteLayers()){
                CollectSpritesOfKind(sl, collectedSprites, kind);
            }
        }
    }
}



void CollectSpritesWithParameter(SpriteLayer *spriteLayer, std::vector<Sprite*>&collectedSprites, const std::string &pName, const std::string &pValue)
{

    for(Sprite *s : spriteLayer->sprites()){

        if(s->sourceSprite()->name()=="pendulumA"){
            DummyFunction();
        }

        //if(Parameter::Exists(s->parameters(), pName, pValue)){
        if(s->parameters().exists(pName, pValue)){
            collectedSprites.push_back(s);
        }
        if(s->type()==SpriteType::COMPOSITE){
            CompositeSprite *cs = static_cast<CompositeSprite*>(s);
            for(SpriteLayer * sl : cs->map()->spriteLayers()){
                CollectSpritesWithParameter(sl, collectedSprites, pName, pValue);
            }
        }
    }
}


void CollectSpritesWithConstParameter(SpriteLayer *spriteLayer, std::vector<Sprite*>&collectedSprites, const std::string &pName, const std::string &pValue)
{

    for(Sprite *s : spriteLayer->sprites()){
        if(s->sourceSprite()->constantParameters().exists(pName, pValue)){
            collectedSprites.push_back(s);
        }

        if(s->type()==SpriteType::COMPOSITE){
            CompositeSprite *cs = static_cast<CompositeSprite*>(s);
            for(SpriteLayer * sl : cs->map()->spriteLayers()){
                CollectSpritesWithConstParameter(sl, collectedSprites, pName, pValue);
            }
        }
    }
}


/*
void CollectSpritesWithOrigConstParameter(SpriteLayer *layer, std::vector<Sprite*>&collectedSprites, const std::string &pName, const std::string &pValue)
{

    for(ncine::SceneNode *n : layer->node()->children()){
        Sprite * s = castNodeToSprite(n);

        //if(Parameter::Exists(s->originalSourceSprite()->constantParameters(), pName, pValue)){
        if(s->originalSourceSprite()->constantParameters().exists(pName, pValue)){
            collectedSprites.push_back(s);
        }
        if(s->type()==SpriteType::COMPOSITE){
            CompositeSprite *composedSprite = static_cast<CompositeSprite*>(s);

            for(ncine::SceneNode* n : composedSprite->mapNodeChildren()){
                LayerElement *l = static_cast<LayerNode*>(n)->layerElement;
                if(l->kind()==LayerKind::SPRITE_LAYER){
                    SpriteLayer * sl = static_cast<SpriteLayer*>(l);
                    CollectSpritesWithOrigConstParameter(sl, collectedSprites, pName, pValue);
                }
            }
        }
    }
}
*/


void CollectSpritesWithDataFlags(SpriteLayer *spriteLayer, std::vector<Sprite*>&collectedSprites, int dataFlags, bool compareDataFlagsAsBitmask)
{

    for(Sprite *s : spriteLayer->sprites()){

        if(compareDataFlagsAsBitmask){
            if(s->dataFlags() & dataFlags){
                collectedSprites.push_back(s);
            }

        }else{
            if(s->dataFlags()==dataFlags){
                collectedSprites.push_back(s);
            }
        }

        if(s->type()==SpriteType::COMPOSITE){
            CompositeSprite *cs = static_cast<CompositeSprite*>(s);
            for(SpriteLayer * sl : cs->map()->spriteLayers()){
                CollectSpritesWithDataFlags(sl, collectedSprites, dataFlags, compareDataFlagsAsBitmask);
            }
        }
    }
}


void CollectSpritesWithSourceSpriteName(SpriteLayer *spriteLayer, std::vector<Sprite*>&collectedSprites,  const std::string &name)
{

    for(Sprite *s : spriteLayer->sprites()){

        if(s->sourceSprite()->name()==name){
            collectedSprites.push_back(s);
        }

        if(s->type()==SpriteType::COMPOSITE){
            CompositeSprite *cs = static_cast<CompositeSprite*>(s);
            for(SpriteLayer * sl : cs->map()->spriteLayers()){
                CollectSpritesWithSourceSpriteName(sl, collectedSprites, name);
            }
        }
    }

}



//--- FIND VECTOR SHAPE

VectorShape* FindVectorShapeWithName(Map *map, const std::string &name)
{

    for(Layer *l : map->layers()){
        if(l->kind()==LayerKind::VECTOR_LAYER){
            VectorLayer * vl = static_cast<VectorLayer*>(l);
            VectorShape * vs = FindVectorShapeWithName(vl, name);
            if(vs) return vs;

        }else if(l->kind()==LayerKind::SPRITE_LAYER){
            SpriteLayer * sl = static_cast<SpriteLayer*>(l);
            VectorShape * vs = FindVectorShapeWithName(sl, name);
            if(vs) return vs;
        }
    }
    return nullptr;
}


VectorShape* FindVectorShapeWithName(SpriteLayer *spriteLayer, const std::string &name)
{

    for(Sprite *s : spriteLayer->sprites()){
        if(s->type()==SpriteType::COMPOSITE){
            CompositeSprite *cs = static_cast<CompositeSprite*>(s);
            VectorShape * vs = FindVectorShapeWithName(cs->map(), name);
            if(vs) return vs;
        }
    }
    return nullptr;
}



VectorShape* FindVectorShapeWithParameter(Map *map, const std::string &pName, const std::string &pValue, ShapeKind kind)
{

    for(Layer *l : map->layers()){
        if(l->kind()==LayerKind::VECTOR_LAYER){
            VectorLayer * vl = static_cast<VectorLayer*>(l);
            VectorShape * vs = FindVectorShapeWithParameter(vl, pName, pValue, kind);
            if(vs) return vs;

        }else if(l->kind()==LayerKind::SPRITE_LAYER){
            SpriteLayer * sl = static_cast<SpriteLayer*>(l);
            VectorShape * vs = FindVectorShapeWithParameter(sl, pName, pValue, kind);
            if(vs) return vs;
        }
    }
    return nullptr;
}


VectorShape* FindVectorShapeWithParameter(SpriteLayer *spriteLayer, const std::string &pName, const std::string &pValue, ShapeKind kind)
{
    for(Sprite *s : spriteLayer->sprites()){
        if(s->type()==SpriteType::COMPOSITE){
            CompositeSprite *cs = static_cast<CompositeSprite*>(s);
            VectorShape * vs = FindVectorShapeWithParameter(cs->map(), pName, pValue, kind);
            if(vs) return vs;
        }
    }
    return nullptr;
}



void CollectVectorShapesWithParameter(Map *map, std::vector<VectorShape*>&collectedShapes, const std::string &pName, const std::string &pValue, ShapeKind kind)
{

    for(Layer *l : map->layers()){
        if(l->kind()==LayerKind::VECTOR_LAYER){
            VectorLayer * vl = static_cast<VectorLayer*>(l);
            CollectVectorShapesWithParameter(vl->shapes(), collectedShapes, pName, pValue, kind);

        }else if(l->kind()==LayerKind::SPRITE_LAYER){
            SpriteLayer * sl = static_cast<SpriteLayer*>(l);
            CollectVectorShapesWithParameter(sl, collectedShapes, pName, pValue, kind);
        }
    }
}


void CollectVectorShapesWithParameter(SpriteLayer *spriteLayer, std::vector<VectorShape*>&collectedShapes, const std::string &pName, const std::string &pValue, ShapeKind kind)
{

    for(Sprite *s : spriteLayer->sprites()){
        if(s->type()==SpriteType::COMPOSITE){
            CompositeSprite *cs = static_cast<CompositeSprite*>(s);
            CollectVectorShapesWithParameter(cs->map(), collectedShapes, pName, pValue, kind);
        }
    }
}



VectorShape* FindVectorShapeWithProperties(Map *map, int dataFlags, bool compareDataFlagsAsBitmask, ShapeKind kind)
{

    for(Layer *l : map->layers()){
        if(l->kind()==LayerKind::VECTOR_LAYER){
            VectorLayer * vl = static_cast<VectorLayer*>(l);
            VectorShape * vs = FindVectorShapeWithProperties(vl, dataFlags, compareDataFlagsAsBitmask, kind);
            if(vs) return vs;
        }
    }
    return nullptr;
}


VectorShape* FindVectorShapeWithName(VectorLayer * vectorLayer, const std::string &name)
{
    return FindVectorShapeWithName(vectorLayer->shapes(), name);
}


VectorShape* FindVectorShapeWithParameter(VectorLayer * vectorLayer, const std::string &pName, const std::string &pValue, ShapeKind kind)
{
    return FindVectorShapeWithParameter(vectorLayer->shapes(), pName, pValue, kind);
}


VectorShape* FindVectorShapeWithProperties(VectorLayer * vectorLayer, int dataFlags, bool compareDataFlagsAsBitmask, ShapeKind kind)
{
    return FindVectorShapeWithProperties(vectorLayer->shapes(), dataFlags, compareDataFlagsAsBitmask, kind);
}


VectorShape* FindVectorShapeWithName(std::vector<VectorShape *> &vectorShapes, const std::string &name)
{
    for(VectorShape * vs : vectorShapes){
        if(vs->name()==name){
            return vs;
        }
    }
    return nullptr;
}


VectorShape* FindVectorShapeWithParameter(std::vector<VectorShape *> &vectorShapes, const std::string &pName, const std::string &pValue, ShapeKind kind)
{
    for(VectorShape * vs : vectorShapes){

        if(kind!=ShapeKind::NOT_DEFINED && vs->kind()!=kind){
            continue;
        }
        //if(Parameter::Exists(vs->GetParameters(), pName, pValue)){
        if(vs->parameters().exists(pName, pValue)){
            return vs;
        }
    }
    return nullptr;
}


void CollectVectorShapesWithParameter(std::vector<VectorShape*>&vectorShapes, std::vector<VectorShape*>&collectedShapes,  const std::string &pName, const std::string &pValue, ShapeKind kind)
{
    for(VectorShape * vs : vectorShapes){

        if(kind!=ShapeKind::NOT_DEFINED && vs->kind()!=kind){
            continue;
        }
        if(vs->parameters().exists(pName, pValue)){
            collectedShapes.push_back(vs);
        }
    }

}


VectorShape* FindVectorShapeWithProperties(std::vector<VectorShape*>&vectorShapes, int dataFlags, bool compareDataFlagsAsBitmask, ShapeKind kind)
{

    for(VectorShape *vs : vectorShapes){

        if(kind!=ShapeKind::NOT_DEFINED && vs->kind()!=kind){
            continue;
        }
        //if(probe!=-1 && vs->IsProbe()!=bool(probe)){
        //    continue;
        //}
        if(dataFlags!=-1){
            if(compareDataFlagsAsBitmask){
                if((vs->dataFlags() & dataFlags)==0){
                    continue;
                }
            }else{
                if(vs->dataFlags() != dataFlags){
                    continue;
                }
            }
        }
        return vs;
    }

    return nullptr;
}


/*
VectorShape* FindSpriteExtraShapeWithProperties(StandardSprite * standardSprite, int dataFlags, bool compareDataFlagsAsBitmask, ShapeKind kind)
{
    if(standardSprite->GetActiveImage()==nullptr) return nullptr;

    return FindVectorShapeWithProperties(standardSprite->GetActiveImage()->GetExtraShapes(), 1, dataFlags, compareDataFlagsAsBitmask, kind);
}
*/


VectorShape* FindSpriteExtraShapeWithProperties(SourceImage * graphicItem, int dataFlags, bool compareDataFlagsAsBitmask, ShapeKind kind)
{

    if(graphicItem==nullptr) return nullptr;

    return FindVectorShapeWithProperties(graphicItem->extraShapes(), dataFlags, compareDataFlagsAsBitmask, kind);

}


VectorShape* FindSpriteCollisionShapeWithProperties(SourceImage * graphicItem, int dataFlags, bool compareDataFlagsAsBitmask, ShapeKind kind)
{

    if(graphicItem==nullptr) return nullptr;

    return FindVectorShapeWithProperties(graphicItem->spriteShapes(), dataFlags, compareDataFlagsAsBitmask, kind);

}



//--- FIND SOURCE SPRITE


void CollectSourceSpritesWithConstParameter(std::vector<SourceSprite*>&collectedSourceSprites, const std::string &pName, const std::string &pValue)
{
    for(SourceGroup * sg : sourceLibrary.sourceGroups()){
        CollectSourceSpritesWithConstParameter(sg, collectedSourceSprites, pName, pValue);
    }

}


void CollectSourceSpritesWithConstParameter(SourceGroup *sourceGroup, std::vector<SourceSprite*>&collectedSourceSprites, const std::string &pName, const std::string &pValue)
{

    for(SourceSet* set : sourceGroup->sourceSets()){
        for(SourceSprite* ss : set->sourceSprites()){
            if(ss->constantParameters().exists(pName, pValue)){
                collectedSourceSprites.push_back(ss);
            }
        }
    }

}


SourceSprite* FindSourceSpriteWithName(const std::string &name)
{
    for(SourceGroup * sg : sourceLibrary.sourceGroups()){
        SourceSprite* ss = FindSourceSpriteWithName(sg, name);
        if(ss){
            return ss;
        }
    }

    return nullptr;
}


SourceSprite* FindSourceSpriteWithName(SourceGroup *sourceGroup, const std::string &name)
{

    for(SourceSet* set : sourceGroup->sourceSets()){
        for(SourceSprite* ss : set->sourceSprites()){
            if(ss->name()==name){
                return ss;
            }
        }
    }

    return nullptr;
}



SourceSprite* FindSourceSpriteByGraphicFilePath(const std::string &filePath)
{
    for(SourceGroup * sg : sourceLibrary.sourceGroups()){
        SourceSprite* ss = FindSourceSpriteByGraphicFilePath(sg, filePath);
        if(ss){
            return ss;
        }
    }

    return nullptr;
}


SourceSprite* FindSourceSpriteByGraphicFilePath(SourceGroup *sourceGroup, const std::string &filePath)
{
    for(SourceSet* set : sourceGroup->sourceSets()){
        for(SourceSprite* ss : set->sourceSprites()){
            for(SourceImage *gi : ss->sourceImages()){
                if(gi->parentSourceFile()->relativeFilePath()==filePath){
                    return ss;
                }
            }
        }
    }

    return nullptr;
}




//--- FIND ANIMATIONS


FrameAnimation* FindFrameAnimationWithName(StandardSprite * standardSprite, const std::string &name)
{
    return FindFrameAnimationWithName(standardSprite->sourceSprite()->frameAnimations(), name);
}


FrameAnimation* FindFrameAnimationWithName(std::vector<FrameAnimation*>&animations, const std::string &name)
{
    for(FrameAnimation * fa : animations){
        if(fa->GetName()==name){
            return fa;
        }
    }
    return nullptr;
}


FrameAnimation* FindFrameAnimationWithParameter(std::vector<FrameAnimation*>&animations, const std::string &pName, const std::string &pValue)
{
    for(FrameAnimation * fa : animations){
        //if(Parameter::Exists(fa->GetParameters(), pName, pValue)){
        if(fa->GetParameters().exists(pName, pValue)){
            return fa;
        }
    }
    return nullptr;
}


//-----

TimelineAnimation* FindTimelineAnimationWithName(Sprite *sprite, const std::string &name)
{
    return FindTimelineAnimationWithName(sprite->sourceSprite()->timelineAnimations(), name);
}


TimelineAnimation* FindTimelineAnimationWithName(std::vector<TimelineAnimation*>&animations, const std::string &name)
{
    for(TimelineAnimation * ta : animations){
        if(ta->GetName()==name){
            return ta;
        }
    }
    return nullptr;
}


TimelineAnimation* FindTimelineAnimationWithParameter(std::vector<TimelineAnimation*>&animations, const std::string &pName, const std::string &pValue)
{
    for(TimelineAnimation * ta : animations){
        //if(Parameter::Exists(ta->GetParameters(), pName, pValue)){
        if(ta->GetParameters().exists(pName, pValue)){
            return ta;
        }
    }
    return nullptr;
}


//---



void GatherSpritesWithSetNameID(CompositeSprite *_compositSprite, std::vector<std::vector<Sprite*>>&spritesPerNameID)
{

    /*
    for(ncine::SceneNode* n : _composedSprite->mapNodeChildren()){
        LayerElement *l = static_cast<LayerNode*>(n)->layerElement;

        if(l->kind()==LayerKind::SPRITE_LAYER){
            SpriteLayer* sl = static_cast<SpriteLayer*>(l);

            for(ncine::SceneNode *n : sl->nodeChildren()){
                Sprite * o = castNodeToSprite(n);

                if(o->name() != ""){

                    int index = -1;
                    for(int i=0; i<spritesPerNameID.size(); i++){
                        if(o->name()==spritesPerNameID[i].front()->name()){
                            index = i;
                            break;
                        }
                    }

                    if(index==-1){
                        spritesPerNameID.push_back(std::vector<Sprite*>());
                        index = spritesPerNameID.size()-1;
                    }

                    bool objectExist = false;
                    for(Sprite* o2 : spritesPerNameID[index]){
                        if(o2==o){
                            objectExist = true;
                            break;
                        }
                    }
                    if(objectExist==false){
                        spritesPerNameID[index].push_back(o);
                    }

                }
                if(o->type()==SpriteType::COMPOSITE){
                    GatherSpritesWithSetNameID(static_cast<CompositeSprite*>(o), spritesPerNameID);
                }
            }
        }
    }
    */


    for(SpriteLayer* sl : _compositSprite->map()->spriteLayers()){
        for(Sprite *o : sl->sprites()){

            if(o->name() != ""){

                int index = -1;
                for(int i=0; i<spritesPerNameID.size(); i++){
                    if(o->name()==spritesPerNameID[i].front()->name()){
                        index = i;
                        break;
                    }
                }

                if(index==-1){
                    spritesPerNameID.push_back(std::vector<Sprite*>());
                    index = spritesPerNameID.size()-1;
                }

                bool objectExist = false;
                for(Sprite* o2 : spritesPerNameID[index]){
                    if(o2==o){
                        objectExist = true;
                        break;
                    }
                }
                if(objectExist==false){
                    spritesPerNameID[index].push_back(o);
                }

            }
            if(o->type()==SpriteType::COMPOSITE){
                GatherSpritesWithSetNameID(static_cast<CompositeSprite*>(o), spritesPerNameID);
            }
        }
    }

}



//


GuiWidget* FindGuiWidgetWithName(std::vector<GuiWidget*> &widgets, const std::string &name,  GuiWidgetKind widgetKind)
{

    for(GuiWidget * w : widgets){
        if(w->GetName()==name && w->GetKind()==widgetKind){
            return w;
        }
    }

    return nullptr;

}


}
