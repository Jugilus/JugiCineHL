#include <cmath>
#include <assert.h>
#include "pugixml/pugixml.hpp"
#include "jmUtilities.h"
#include "jmTimelineAnimation.h"
#include "jmFrameAnimation.h"
#include "jmStandardSprite.h"
#include "jmCommonFunctions.h"
#include "jmSystem.h"

#include "jpPlayedApp.h"
#include "logic/jpLogicStateCfg.h"
#include "logic/jpLogicAction.h"
#include "jpUtilities.h"
#include "jpObjectParser.h"

#include "jpEntitySystem.h"
#include "jpB2Fixture.h"
#include "jpB2Body.h"
#include "jpEntity.h"
#include "jpEntityUtilities.h"
#include "jpB2World.h"
#include "jpMovementCommon.h"
#include "jpCustomState.h"



namespace jugimap{






bool CustomStateCfg::initCfg(const pugi::xml_node &_node, std::vector<std::pair<std::string, int> > &_engineStateNamedValues)
{


    for(pugi::xml_attribute a = _node.first_attribute(); a; a = a.next_attribute()){
        std::string attributeName =std::string(a.name());

        if(attributeName=="name"){
            name = a.as_string("");

        }else if(attributeName=="baseStates"){
            std::string sBaseStates = a.as_string("");
            std::vector<std::string>parts = StdString::splitString(sBaseStates, ",");

            for(const std::string &s : parts){
                for(auto &p : _engineStateNamedValues){
                    if(p.first==s){
                        //engineStateValue = p;
                        engineStateValues.push_back(p);
                        break;
                    }
                }
            }

            if(engineStateValues.empty()){
                dbgSystem.addMessage("Wrong attribute value '" + sBaseStates + "'");
                return false;
            }

        }else if(attributeName=="movement"){
            std::string sValue = a.as_string();
            if(StdString::boolValue(sValue, engineMovementAllowed)==false){
                return false;
            }

        //}else if(attributeName=="trigger"){
        //    sigTrigger = a.as_string("");

        }else if(attributeName=="animationLeft"){
            aniLeft = a.as_string("");

        }else if(attributeName=="animationRight"){
            aniRight = a.as_string("");

        }else{

            dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + std::string(_node.name()) +"' !");
            return false;
        }
    }


    for(pugi::xml_node nChild = _node.first_child(); nChild; nChild = nChild.next_sibling()){
        std::string childNodeName = std::string(nChild.name());

        /*
        if(childNodeName=="context"){

            for(pugi::xml_attribute a = nChild.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="onSignal"){
                    contextTrigger = a.as_string("");

                }else{

                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + childNodeName +"' !");
                    return false;
                }
            }

        }else
        */

        if(childNodeName=="trigger"){
            if(actionTriggerCfg.initCfg(nChild)==false){
                return false;
            }


        }else if(childNodeName=="animations"){

            for(pugi::xml_attribute a = nChild.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="animationLeft"){
                    aniLeft = a.as_string("");

                }else if(attributeName=="animationRight"){
                    aniRight = a.as_string("");

                }else{
                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + childNodeName +"' !");
                    return false;
                }
            }


        }else if(childNodeName=="customSensor"){

            for(pugi::xml_attribute a = nChild.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="extraShapeCategory"){
                    customSensorCfg.customSensorCategory = a.as_string("");

                }else if(attributeName=="vectorShapeData"){
                    customSensorCfg.customSensorExtraShapeID = a.as_int(1);

                }else if(attributeName=="activeOnAnimationKeyData"){
                    customSensorCfg.extraShapeActiveOnAnimationKeyData = a.as_int(1);

                }else{

                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + childNodeName +"' !");
                    return false;
                }
            }

        }else if(childNodeName=="activePhase"){

            for(pugi::xml_attribute a = nChild.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="onFrameData"){
                    std::string sValue = a.as_string("");
                    if(StdString::integerNumber(sValue, activePhaseFrameIntData)==false){
                        return false;
                    }

                }else{

                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + childNodeName +"' !");
                    return false;
                }
            }

        }else{

            dbgSystem.addMessage("Unknown node '" + childNodeName +"' !");
            return false;
        }
    }

    return true;

}


//---------------------------------------------------------------------------------------------




CustomStateData::~CustomStateData()
{
    if(aniLeft){
        delete aniLeft;
    }
    if(aniRight){
        delete aniRight;
    }
    if(actionTrigger){
        delete actionTrigger;
    }

}



bool CustomStateData::initConnections(PlayedScene *_scene, Entity *_actor)
{

    Sprite *sprite = _actor->sprite();

    //ObtainSignalFromPath(cfg->sigTrigger, _scene, _actor, nullptr,trigger);
    /*
    if(cfg->sigTrigger.empty()==false){
        app->signalParserManager()->parseSignalAccessor(_scene, cfg->sigTrigger, sqTrigger, _actor);
        if(sqTrigger.isValid()==false){
            return false;
        }
    }
    */

    if(cfg->aniRight.empty()==false){
        aniRight = ObtainAnimationInstance(sprite, cfg->aniRight);
        if(aniRight==nullptr){
            return false;
        }
    }
    if(cfg->aniLeft.empty()==false){
        aniLeft = ObtainAnimationInstance(sprite, cfg->aniLeft);
        if(aniLeft==nullptr){
            return false;
        }
    }

    /*
    if(cfg->contextTrigger.empty()==false){
        //ObtainSignalFromPath(cfg->contextTrigger, _scene, _actor, nullptr, contextTrigger);
        app->signalParserManager()->parseSignalAccessor(_scene, cfg->contextTrigger, contextTrigger, _actor);
        if(contextTrigger.isValid()==false){
            return false;
        }
    }
    */

    if(cfg->actionTriggerCfg.mConditions.empty()==false){
        actionTrigger = new ActionTrigger(_actor);

        if(actionTrigger->build(cfg->actionTriggerCfg)==false){
            return false;
        }
        if(actionTrigger->initConnections(_scene)==false){
            return false;
        }
    }



    //---
    if(cfg->customSensorCfg.customSensorCategory.empty()==false){

        EntityCategory * extraShapeCategory = _actor->parentEntitySystem()->entityCategoriesGroup().getEntityCategory(cfg->customSensorCfg.customSensorCategory);
        if(extraShapeCategory==nullptr){
            return false;
        }

        triggeringExtraShapeSourceFixture = _actor->body()->sourceBody()->getSourceFixture(extraShapeCategory);
        if(triggeringExtraShapeSourceFixture==nullptr){
            return false;
        }

        extraShapeFixtureCreated = (triggeringExtraShapeSourceFixture->temporaryFixture==true)? false : true;

    }


    return true;

}


//---------------------------------------------------------------------------------------------


void CustomStateEngine::init(MovementEngine *_engine, std::vector<CustomStateData> *_customStates)
{
    mEngine = _engine;
    mCustomStates = _customStates;
    if(mCustomStates && mCustomStates->empty()){
        mCustomStates = nullptr;
    }

}


void CustomStateEngine::reset()
{

    if(mStatus==CustomStateStatus::NO_CUSTOM_STATE || mStatus==CustomStateStatus::CUSTOM_STATE_ENDED){
        return;
    }


    Entity *entity = mEngine->parentEntity();

    mEngine->setStateEllapsedTimeS(0.0f);
    mCustomState = nullptr;
    mActiveAnimationInstance = nullptr;
    entity->statusFlagsRef() &= ~EntityStatusFlags::FORCED_ENTITY_TRANSFORMATION;
    entity->customActSignal().setValue_onNextFrame(-1);
    mStatus = CustomStateStatus::CUSTOM_STATE_ENDED;

}


CustomStateStatus CustomStateEngine::update(EngineUpdateParameters &eup, int _baseState)
{


    mStatus = CustomStateStatus::NO_CUSTOM_STATE;

    //if(mEngine->type()==MovementEngineType::JUMP_MOVEMENT){
    //    DummyFunction();
    //}

    if(mCustomStates == nullptr){
        return mStatus;
    }

    if(mCustomState==nullptr){
        mCustomState = obtainActiveCustomState(_baseState);
        if(mCustomState==nullptr){
            return mStatus;
        }
        mStatus = CustomStateStatus::CUSTOM_STATE_STARTED;

    }else{
        mStatus = CustomStateStatus::CUSTOM_STATE_ACTIVE;
    }


    //----
    AnimationPlayer *animationPlayer = mEngine->animationPlayer();     assert(animationPlayer);
    Entity *entity = mEngine->parentEntity();

    if(animationPlayer->GetAnimationInstance() != mActiveAnimationInstance){
        DummyFunction();    // safety, should not be allowed that the base engine change animation instance once the custom state is active
        reset();
        return mStatus;
    }


    if(mCustomState->triggeringExtraShapeSourceFixture){

        bool extraShapeActive = false;

        if(animationPlayer->GetAnimationInstance()->GetKind()==AnimationKind::FRAME_ANIMATION){

        }else if(animationPlayer->GetAnimationInstance()->GetKind()==AnimationKind::TIMELINE_ANIMATION){

            //if(mAnimationPlayer->GetLatestUpdateReturnFlag() & AnimationPlayerFlags::META_KEY_CHANGED){
                AKMeta* metaKey = animationPlayer->GetActiveMetaKey();
                if(metaKey && metaKey->dataFlags==mCustomState->cfg->customSensorCfg.extraShapeActiveOnAnimationKeyData){
                    extraShapeActive = true;
                }
            //}
        }

        if(extraShapeActive && mCustomState->extraShapeFixtureCreated==false){
            eup.entity->body()->createFixture(*mCustomState->triggeringExtraShapeSourceFixture);
            mCustomState->extraShapeFixtureCreated = true;

        }else if(extraShapeActive==false && mCustomState->extraShapeFixtureCreated){
            eup.entity->body()->destroyFixture(mCustomState->triggeringExtraShapeSourceFixture->fud->category);
            mCustomState->extraShapeFixtureCreated = false;
        }
    }

    //----
    if(animationPlayer->GetAnimationInstance()->GetKind()==AnimationKind::FRAME_ANIMATION){

        // manage active phase of the custom state
        if(mCustomState->cfg->activePhaseFrameIntData != -1){
            FrameAnimationInstance *fai = static_cast<FrameAnimationInstance*>(animationPlayer->GetAnimationInstance());
            int currentFrameIntData = fai->currentFrameIntData();
            IntSignal &customActSignal = entity->customActSignal();
            if(mCustomState->cfg->activePhaseFrameIntData == currentFrameIntData){
                if(customActSignal.value() != mCustomState->customActSignalId){
                    customActSignal.setValue_onNextFrame(mCustomState->customActSignalId);
                }
            }else{
                if(customActSignal.value() == mCustomState->customActSignalId){
                    customActSignal.setValue_onNextFrame(-1);
                }
            }
        }
    }


    if(animationPlayer->GetState()==AnimationPlayerState::STALLED){

        mCustomState = obtainActiveCustomState(_baseState);
        if(mCustomState){

            // immediately start a new (or existing) custom state
            //if(customState==mCustomState){
                //mStateEllapsedTimeS = 0.0f;
                animationPlayer->Stop();
                animationPlayer->Play(mActiveAnimationInstance);

                entity->customActSignal().reset(-1);
                entity->customActSignal().setValue_onNextFrame(mCustomState->customActSignalId);
            //}
            DummyFunction();

        }else{

            reset();
        }
    }

    return mStatus;

}


CustomStateData *CustomStateEngine::obtainActiveCustomState(int _baseState)
{

    CustomStateData *customState = nullptr;

    for(CustomStateData &cs : *mCustomStates){
        for(auto &p :cs.cfg->engineStateValues){
            if(p.second != _baseState){
                continue;
            }
        }

        customState = _checkCustomState(&cs);
        if(customState){
            return customState;
        }
    }

    return nullptr;

}


CustomStateData *CustomStateEngine::_checkCustomState(CustomStateData *_customState)
{


    if(_customState->actionTrigger){
        if(_customState->actionTrigger->isTriggered()){

            Direction direction = mEngine->direction();
            Entity *entity = mEngine->parentEntity();

            //---
            //mBaseStateAnimationInstance = mEngine->activeAnimationInstance();

            if(direction==Direction::LEFT){
                mActiveAnimationInstance = _customState->aniLeft;

            }else if(direction==Direction::RIGHT){
                mActiveAnimationInstance = _customState->aniRight;
            }
            mEngine->setActiveAnimationInstance(mActiveAnimationInstance);
            mEngine->updateAnimationPlayer();

            //---
            if(_customState->cfg->activePhaseFrameIntData == -1){
                entity->customActSignal().setValue_onNextFrame(_customState->customActSignalId);
            }

            mEngine->setStateEllapsedTimeS(0.0f);

            return _customState;

        }else{
            return nullptr;

        }
    }

    /*
    if(_customState->contextTrigger.signal()){
        if(_customState->contextTrigger.active()==false){
            return nullptr;
        }
    }

    if(_customState->sqTrigger.active()){

        Direction direction = mEngine->direction();
        Entity *entity = mEngine->parentEntity();

        //---
        //mBaseStateAnimationInstance = mEngine->activeAnimationInstance();

        if(direction==Direction::LEFT){
            mActiveAnimationInstance = _customState->aniLeft;

        }else if(direction==Direction::RIGHT){
            mActiveAnimationInstance = _customState->aniRight;
        }
        mEngine->setActiveAnimationInstance(mActiveAnimationInstance);
        mEngine->updateAnimationPlayer();

        //---
        if(_customState->cfg->activePhaseFrameIntData == -1){
            entity->customActSignal().setValue_onNextFrame(_customState->customActSignalId);
        }

        mEngine->setStateEllapsedTimeS(0.0f);

        return _customState;
    }
    */

    return nullptr;
}





}
