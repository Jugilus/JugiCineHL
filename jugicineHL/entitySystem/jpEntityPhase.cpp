#include <chrono>
#include <assert.h>
#include <sstream>
#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmMap.h"
#include "jmCamera.h"
#include "jmSceneLayout.h"
#include "jmCommonFunctions.h"
#include "jmStandardSprite.h"
#include "jmSpriteLayer.h"
#include "jmUtilities.h"
#include "items/jpItemsCommon.h"

#include "data/jpItem.h"
#include "jpPlayedApp.h"
#include "jpPlayedScene.h"
#include "jpUtilities.h"
#include "jpGfxObjectsCommon.h"
//#include "jpActionsCommon.h"

#include "movements/jpMovementBase.h"
#include "jpEntityLogicState.h"
#include "jpB2Body.h"
#include "jpEntitySystem.h"
#include "jpSourceEntity.h"
#include "jpEntity.h"
#include "jpEntityPhase.h"



namespace jugimap{


/*

bool EntityPhaseCfg::initCfg(const pugi::xml_node &_node)
{

    name = std::string(_node.name());
    dbgSystem.addMessage("Loading entity phase '" + name + "' ...");


    for(pugi::xml_attribute a = _node.first_attribute(); a; a = a.next_attribute()){
        std::string attributeName =std::string(a.name());

        if(attributeName=="aniSpriteObjects"){
            std::string sValue = a.as_string();
            mAniSpriteObjects = StdString::splitString(sValue, ",");

        }else if(attributeName=="alphaFadeDurationS"){
            std::string sValue = a.as_string();
            if(StdString::floatNumber(sValue, alphaFadeDurationS)==false){
                return false;
            }

        }else{

            dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + std::string(_node.name()) +"' !");
            return false;
        }
    }


    dbgSystem.removeLastMessage();
    return true;
}


//====================================================================================================


EntityPhase::~EntityPhase()
{

    for(AniSpriteObject *aso : mAniSpriteObjects){
        delete aso;
    }

}


bool EntityPhase::build(PlayedScene *_scene, EntityPhaseCfg *_cfg, EntityPhasesHandler *_parent)
{

    mCfg = _cfg;
    mParentObject = _parent;

    dbgSystem.addMessage("Building entity phase '" + mCfg->name + "' ...");


    for(const std::string &s : mCfg->mAniSpriteObjects){
        GfxObjectCfg *cfg = _scene->gfxObjectCfgStorage().getObject(s);
        if(cfg==nullptr){
            return false;
        }
        if(cfg->type()!=GfxObjectType::ANIMATED_SPRITE){
            dbgSystem.addMessage("Gfx object '" + s +"' is not an 'animated sprite'!");
            return false;
        }

        mAniSpriteObjects.push_back(new AniSpriteObject(cfg, this));
    }


    if(mCfg->alphaFadeDurationS>0.0f){
        mTween.Init(0.0f, 1.0f, mCfg->alphaFadeDurationS, EasingKind::LINEAR);
    }


    //----
    dbgSystem.removeLastMessage();
    return true;

}


bool EntityPhase::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("Initializing entity phase '" + mCfg->name + "' ...");


    for(AniSpriteObject *aso : mAniSpriteObjects){
        if(aso->initConnections(_scene)==false){
            return false;
        }
    }

    //----
    dbgSystem.removeLastMessage();
    return true;

}


void EntityPhase::start(Vec2f _pos)
{

    for(AniSpriteObject *aso : mAniSpriteObjects){
        aso->start(_pos);
    }

    if(mTween.state()==TweenState::IDLE){
        mTween.Play();
    }

}


void EntityPhase::update()
{

    for(AniSpriteObject *aso : mAniSpriteObjects){
        aso->update();
    }

    if(mTween.state()!=TweenState::NEVER_INITIALIZED){
        mTween.Update();
    }

}


bool EntityPhase::areAnimationsStalled()
{

    for(AniSpriteObject *aso : mAniSpriteObjects){
        if(aso->isAnimationStalled()==false){
            return false;
        }
    }

    return true;
}

bool EntityPhase::isFinished()
{

    for(AniSpriteObject *aso : mAniSpriteObjects){
        if(aso->isAnimationStopped()==false){
            return false;
        }
    }

    if(mTween.state()!=TweenState::NEVER_INITIALIZED){
        if(mTween.state()!=TweenState::IDLE){
            return false;
        }
    }

    return true;
}


ItemData * EntityPhase::findData(const std::string &_name, bool _setErrorMessage)
{



    if(_setErrorMessage){
        dbgSystem.addMessage("Data '" + _name + "' not found!");
    }

    return nullptr;
}


//====================================================================================================


bool EntityPhasesHandlerCfg::initCfg(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading entity phase handler '" + mName +"' ...");

    //-----
    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());

        if(nodeName=="endPhase"){

            mPhaseCfgs.push_back(EntityPhaseCfg(EntityPhaseType::END));
            EntityPhaseCfg &phaseCfg = mPhaseCfgs.back();

            if(phaseCfg.initCfg(n)==false){
                return false;
            }

        }else{

            dbgSystem.addMessage("Unknown node '" + nodeName + "' !");
            return false;
        }
    }


    dbgSystem.removeLastMessage();
    return true;

}


//-------------------------------------------------------------------------------------------------


bool EntityPhasesHandler::build(PlayedScene *_scene, EntityPhasesHandlerCfg *_cfg)
{

    mCfg = _cfg;
    dbgSystem.addMessage("Building entity phase handler '" + mCfg->mName +"' ...");


    mEntityPhases.resize(mCfg->mPhaseCfgs.size());

    for(unsigned int i=0; i<mEntityPhases.size(); i++){
        EntityPhaseCfg &epCfg = mCfg->mPhaseCfgs[i];
        EntityPhase &ep = mEntityPhases[i];
        if(ep.build(_scene, &epCfg, this)==false){
            return false;
        }

    }

    dbgSystem.removeLastMessage();
    return true;

}



bool EntityPhasesHandler::initConnections(PlayedScene *_scene)
{
    dbgSystem.addMessage("Initializing entity phase handler '" + mCfg->mName +"' ...");


    for(EntityPhase &ep : mEntityPhases){
        if(ep.initConnections(_scene)==false){
            return false;
        }
    }

    //----
    dbgSystem.removeLastMessage();
    return true;

}


EntityPhase * EntityPhasesHandler::entityPhase(EntityPhaseType _type)
{

    for(EntityPhase &ep : mEntityPhases){
        if(ep.type()== _type){
            return &ep;
        }
    }

    return nullptr;
}


//========================================================================================================

*/

void EntityLifespanHandler::addEntityForErase(Entity *_entity)
{

    mEntitiesForErase.push_back(_entity);

}


void EntityLifespanHandler::addSourceEntityForSpawning(SpawnerAndSpawned * _sas)
{
    mSourceEntitiesForSpawning.push_back(_sas);
}


/*
void EntityLifespanHandler::update()
{

    for(Entity *e : mEntitiesForErase){
        EntityPhasesHandler *eph = e->entityPhasesHandler();
        EntityPhase *endPhase = nullptr;

        if(eph){
            endPhase = eph->entityPhase(EntityPhaseType::END);
        }
        if(endPhase){
            endPhase->start(e->mapElement().position());

            if(endPhase->tween().state()==TweenState::PLAYING){
                mEntitiesWithEndPhase.push_back({e, endPhase});

            }else{
                eraseEntity(e);
                mEntitiesWithEndPhase.push_back({nullptr, endPhase});
            }

        }else{
            eraseEntity(e);
        }
    }
    mEntitiesForErase.clear();


    //---
    for(int i = int(mEntitiesWithEndPhase.size())-1; i>=0; i-- ){
        Entity *e = mEntitiesWithEndPhase[i].first;
        EntityPhase *endPhase = mEntitiesWithEndPhase[i].second;

        endPhase->update();

        if(e){
            assert(endPhase->tween().state()!=TweenState::NEVER_INITIALIZED);
            float alpha = 1.0f - endPhase->tween().GetValue();      // tween is running from 0.0f - 1.0f
            e->mapElement().setAlpha(alpha);
        }

        if(endPhase->isFinished()){
            if(e){
                eraseEntity(e);
            }
            mEntitiesWithEndPhase.erase(mEntitiesWithEndPhase.begin()+i);
        }
    }

}
*/


void EntityLifespanHandler::update()
{
    update_erasingEntities();
    update_spawningEntities();
}


void EntityLifespanHandler::update_erasingEntities()
{

    for(Entity *e : mEntitiesForErase){

        //--- check if entity is being spawned (items)
        if(e->itemPickableSignal() && e->itemPickableSignal()->value()==false){
           continue;
        }

        if(e->sourceEntity()->name()=="iAmigaBall"){
            DummyFunction();
        }

        removeEntityFromWorld(e);

        bool vanishEffect = false;
        bool entityAlphaFade = false;

        Item *item = e->item()->findChildItem("endPhase", false);
        if(item){
            for(ItemData *d : item->dataStorage()){
                if(d->type()==ItemDataType::ANI_SPRITE_OBJECT){
                    AniSpriteObjectData *asod = static_cast<AniSpriteObjectData*>(d);
                    //AniSpriteObject *aso = asod->aniSpriteObject();
                    asod->start(e->mapElement().position());
                    vanishEffect = true;

                }else if(d->type()==ItemDataType::TWEEN){
                    TweenData *td = static_cast<TweenData*>(d);
                    Tween &tween = td->tween();
                    tween.Play(1.0f, 0.0f);
                    entityAlphaFade = true;
                    vanishEffect = true;
                }
            }
        }

        if(vanishEffect){
            if(entityAlphaFade){
                mEntitiesWithVanishEffect.push_back({e, item});
            }else{
                mEntitiesWithVanishEffect.push_back({nullptr, item});
            }
        }

        if(entityAlphaFade==false){
            releaseEntityFromPool(e);
        }
    }

    mEntitiesForErase.clear();


    //----
    for(int i = int(mEntitiesWithVanishEffect.size())-1; i>=0; i-- ){
        Entity *e = mEntitiesWithVanishEffect[i].first;
        Item *item = mEntitiesWithVanishEffect[i].second;

        bool activeVanishEffect = false;

        for(ItemData *d : item->dataStorage()){
            if(d->type()==ItemDataType::ANI_SPRITE_OBJECT){
                AniSpriteObjectData *asod = static_cast<AniSpriteObjectData*>(d);
                //AniSpriteObject *aso = asod->aniSpriteObject();
                asod->update();
                if(asod->isAnimationStopped()==false){
                    activeVanishEffect = true;
                }

            }else if(d->type()==ItemDataType::TWEEN){
                TweenData *td = static_cast<TweenData*>(d);
                Tween &tween = td->tween();
                if(tween.state()!=TweenState::IDLE){
                    float alpha = tween.Update();
                    e->mapElement().setAlpha(alpha);
                    activeVanishEffect = true;
                }
            }
        }

        if(activeVanishEffect==false){
            mEntitiesWithVanishEffect.erase(mEntitiesWithVanishEffect.begin()+i);
            if(e){
                releaseEntityFromPool(e);
            }
        }
    }

}


void EntityLifespanHandler::update_spawningEntities()
{

    for(SpawnerAndSpawned *sas : mSourceEntitiesForSpawning){
        Entity* spawnerEntity = sas->spawnerEntity;
        SourceEntity *se = sas->spawnedSourceEntity;
        //print("Spawning '" + se->name() + "'!");

        Entity *e = se->entityPool().acquireObject();

        EntitySystem *entitySystem = se->sourceEntityCfg()->entitySystem;
        entitySystem->entities().push_back(e);

        Sprite *sprite = nullptr;

        Vec2f offset{0.0f, 0.0f};
        SpawnerCfg *spawnerCfg = spawnerEntity->sourceEntity()->sourceEntityCfg()->mSpawnerCfg.get();
        if(spawnerCfg){
            MovementEngineCfg *engineCfg = spawnerEntity->currentEngine()->currentCfg();
            offset = spawnerCfg->getOffsetForSpawnedEntity(se, engineCfg, spawnerEntity->direction());
        }

        if(e->initialized()==false){

            PlayedScene *scene = entitySystem->parentPlayerScene();
            sprite = Sprite::makePassiveSprite(se->sourceSprite());
            sprite->_setParentLayer(se->spriteLayer());
            sprite->setPosition(spawnerEntity->mapElement().sprite()->position() + offset);
            sprite->initEngineObjects();
            se->spriteLayer()->addSprite(sprite);

            if(e->build(scene, se, sprite, nullptr, nullptr)==false){
                DummyFunction();
            }
            if(e->initConnections1(scene)==false){
                DummyFunction();
            }
            if(e->initConnections2(scene)==false){
                DummyFunction();
            }
            if(e->initConnections(scene)==false){
                DummyFunction();
            }

        }else{
            sprite = e->mapElement().sprite();
            sprite->setPosition(spawnerEntity->mapElement().sprite()->position() + offset);
            sprite->setVisible(true);
        }


        if(e->relativeMover()){
            e->relativeMover()->start(sprite->position(), spawnerEntity->direction(), e);
            if(e->itemPickableSignal()){
                e->itemPickableSignal()->setValue(false);
            }
            if(e->relativeMover()->currentAnimationInstance()){
                e->setCurrentAnimationInstance(e->relativeMover()->currentAnimationInstance());
            }
        }

    }


    mSourceEntitiesForSpawning.clear();


}


void EntityLifespanHandler::releaseEntityFromPool(Entity *_entity)
{

    _entity->animationPlayer().Stop();
    _entity->mapElement().removeFromMap();
    _entity->sourceEntity()->entityPool().releaseObject(_entity);

}


void EntityLifespanHandler::removeEntityFromWorld(Entity *_entity)
{

    EntitySystem *entitySystem = _entity->sourceEntity()->sourceEntityCfg()->entitySystem;
    StdVector::removeElement(entitySystem->entities(), _entity);
    _entity->removeFromWorld();
    _entity->contactTrigger().clearContacts();

}


}



