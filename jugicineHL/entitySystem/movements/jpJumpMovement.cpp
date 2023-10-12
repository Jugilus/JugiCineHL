#include <chrono>
#include <assert.h>
#include <sstream>
#include "pugixml/pugixml.hpp"
#include "jmAnimationCommon.h"
#include "jmCommonFunctions.h"
#include "jmStandardSprite.h"
#include "jmSystem.h"

#include "jpUtilities.h"
#include "jpPlayedApp.h"

#include "jpB2Fixture.h"
#include "jpSourceEntity.h"
#include "jpEntity.h"
#include "jpEntityUtilities.h"
#include "jpB2World.h"
#include "jpJumpMovement.h"



namespace jugimap{



//----------------------------------------------------------------------------

JumpMovementState GetJumpMovementStateFromString(const std::string &state)
{

    if(state=="TAKING_OFF"){
        return JumpMovementState::TAKING_OFF;

    }else if(state=="ASCENDING"){
        return JumpMovementState::ASCENDING;

    }else if(state=="DESCENDING"){
        return JumpMovementState::DESCENDING;

    }else if(state=="LANDING"){
        return JumpMovementState::LANDING;

    }else if(state=="LANDED"){
        return JumpMovementState::LANDED;
    }

    return JumpMovementState::UNKNOWN;
}



//------------------------------------------------------------------------------------------

const float JumpMovementCfg::minIntermediateHeightRatio = 0.5f;
const float JumpMovementCfg::maxIntermediateHeightRatio = 1.0f;
const float JumpMovementCfg::minIntermediateSpeedRatio = 0.25f;
const float JumpMovementCfg::maxIntermediateSpeedRatio = 1.0f;


bool JumpMovementCfg::initCfg(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading movement engine of type '" + factory->typeSignature() + "' and name '" + name + "'...");


    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());

        if(nodeName=="yMovement"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="maxHeightP"){
                    maxHeight = gWorldInfo.pixelsToMeters(a.as_float(maxHeight));

                }else if(attributeName=="maxHeightM"){
                    maxHeight = a.as_float(maxHeight);

                }else if(attributeName=="pushOffSpeedP"){
                    pushOffSpeed = gWorldInfo.pixelsToMeters(a.as_float(pushOffSpeed));

                }else if(attributeName=="pushOffSpeedM"){
                    pushOffSpeed = a.as_float(pushOffSpeed);

                }else if(attributeName=="intermediateHeightRatio"){
                    intermediateHeightRatio = ClampValue(a.as_float(intermediateHeightRatio), JumpMovementCfg::minIntermediateHeightRatio, JumpMovementCfg::maxIntermediateHeightRatio);

                }else if(attributeName=="intermediateSpeedRatio"){
                    intermediateSpeedRatio = ClampValue(a.as_float(intermediateSpeedRatio), JumpMovementCfg::minIntermediateSpeedRatio, JumpMovementCfg::maxIntermediateSpeedRatio);

                }else{

                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                    return false;
                }
            }


        }else if(nodeName=="xMovement"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="xMaxSpeedP"){
                    xMaxSpeed = gWorldInfo.pixelsToMeters(a.as_float(xMaxSpeed));

                }else if(attributeName=="xMaxSpeedM"){
                    xMaxSpeed = a.as_float(xMaxSpeed);

                }else if(attributeName=="xAccDistanceP"){
                    xAccDistance = gWorldInfo.pixelsToMeters(a.as_float(xAccDistance));

                }else if(attributeName=="xAccDistanceM"){
                    xAccDistance = a.as_float(xAccDistance);

                }else if(attributeName=="xGroundSpeedInheritFactor"){
                    xGroundSpeedInheritFactor = a.as_float(xGroundSpeedInheritFactor);

                }else{

                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                    return false;
                }
            }


        }else if(nodeName=="otherParameters"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="maxSlope"){
                    maxSlope = a.as_float(maxSlope);
                    b2Vec2 _maxSlope{ std::cos(maxSlope*b2_pi/180.0f), std::sin(maxSlope*b2_pi/180.0f)};
                    maxSlopeNormalY = _maxSlope.x;

                }else if(attributeName=="pushImpulseSpeedFactor"){
                    pushImpulseSpeedFactor = a.as_float(pushImpulseSpeedFactor);

                }else if(attributeName=="addCeilingSensor"){
                    addCeilingSensor = a.as_bool(false);

                }else{

                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                    return false;
                }
            }

        }else if(nodeName=="triggers"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="startJump"){
                    sigStartJump = a.as_string("");

                }else if(attributeName=="moveLeft"){
                    sigMoveLeft = a.as_string("");

                }else if(attributeName=="moveRight"){
                    sigMoveRight = a.as_string("");

                }else{

                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                    return false;
                }
            }


        }else if(nodeName=="baseStates"){


            for(pugi::xml_node nChild = n.first_child(); nChild; nChild = nChild.next_sibling()){
                std::string childNodeName = std::string(nChild.name());

                if(childNodeName=="takeOff"){

                    for(pugi::xml_attribute a = nChild.first_attribute(); a; a = a.next_attribute()){
                        std::string attributeName =std::string(a.name());

                        if(attributeName=="animationLeft"){
                            aniTakeOffLeft = a.as_string("");

                        }else if(attributeName=="animationRight"){
                            aniTakeOffRight = a.as_string("");

                        }else{

                            dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + childNodeName +"' !");
                            return false;
                        }
                    }


                }else if(childNodeName=="ascending"){

                    for(pugi::xml_attribute a = nChild.first_attribute(); a; a = a.next_attribute()){
                        std::string attributeName =std::string(a.name());

                        if(attributeName=="animationLeft"){
                            aniAscendingLeft = a.as_string("");

                        }else if(attributeName=="animationRight"){
                            aniAscendingRight = a.as_string("");

                        }else{

                            dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + childNodeName +"' !");
                            return false;
                        }
                    }


                }else if(childNodeName=="atMaxHeight"){

                    for(pugi::xml_attribute a = nChild.first_attribute(); a; a = a.next_attribute()){
                        std::string attributeName =std::string(a.name());

                        if(attributeName=="animationLeft"){
                            aniAtMaxHeightLeft = a.as_string("");

                        }else if(attributeName=="animationRight"){
                            aniAtMaxHeightRight = a.as_string("");

                        }else{

                            dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + childNodeName +"' !");
                            return false;
                        }
                    }


                }else if(childNodeName=="descending"){

                    for(pugi::xml_attribute a = nChild.first_attribute(); a; a = a.next_attribute()){
                        std::string attributeName =std::string(a.name());

                        if(attributeName=="animationLeft"){
                           aniDescendingLeft = a.as_string("");

                        }else if(attributeName=="animationRight"){
                            aniDescendingRight = a.as_string("");

                        }else{

                            dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + childNodeName +"' !");
                            return false;
                        }

                    }

                }else if(childNodeName=="landing"){

                    for(pugi::xml_attribute a = nChild.first_attribute(); a; a = a.next_attribute()){
                        std::string attributeName =std::string(a.name());

                        if(attributeName=="animationLeft"){
                           aniLandingLeft = a.as_string("");

                        }else if(attributeName=="animationRight"){
                            aniLandingRight = a.as_string("");

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

        }else{

            dbgSystem.addMessage("Unknown node '" + nodeName + "' !");
            return false;

        }
    }

    dbgSystem.removeLastMessage();
    return true;

}



JumpMovementData::~JumpMovementData()
{

    if(aniTakeOffLeft){
        delete aniTakeOffLeft;
    }
    if(aniTakeOffRight){
        delete aniTakeOffRight;
    }

    if(aniAscendingLeft){
        delete aniAscendingLeft;
    }
    if(aniAscendingRight){
        delete aniAscendingRight;
    }

    if(aniAtMaxHeightLeft){
        delete aniAtMaxHeightLeft;
    }
    if(aniAtMaxHeightRight){
        delete aniAtMaxHeightRight;
    }

    if(aniDescendingLeft){
        delete aniDescendingLeft;
    }
    if(aniDescendingRight){
        delete aniDescendingRight;
    }

    if(aniLandingLeft){
        delete aniLandingLeft;
    }
    if(aniLandingRight){
        delete aniLandingRight;
    }

}


bool JumpMovementData::initConnections(PlayedScene *_scene, Entity *_actor)
{
    dbgSystem.addMessage("Init connections for the data object of '" + cfg->name + "' ...");



    //--- signals
    if(cfg->sigStartJump.empty()){        // computer controlled actors
        startJump.init(&sigStartJumpObj, SignalQuery::Style::ACTIVE);

    }else{
        //ObtainSignalFromPath(cfg->sigStartJump, _scene, _actor, nullptr, startJump);
        app->signalParserManager()->parseSignalAccessor(_scene, cfg->sigStartJump, startJump, _actor);
        if(startJump.isValid()==false){
            return false;
        }
    }


    if(cfg->sigMoveLeft.empty()){        // computer controlled actors
        moveLeft.init(&sigMoveLeftObj, SignalQuery::Style::ACTIVE);

    }else{
        //ObtainSignalFromPath(cfg->sigMoveLeft, _scene, _actor, nullptr, moveLeft);
        app->signalParserManager()->parseSignalAccessor(_scene, cfg->sigMoveLeft, moveLeft, _actor);
        if(moveLeft.isValid()==false){
            return false;
        }
    }


    if(cfg->sigMoveRight.empty()){        // computer controlled actors
        moveRight.init(&sigMoveRightObj, SignalQuery::Style::ACTIVE);

    }else{
        //ObtainSignalFromPath(cfg->sigMoveRight, _scene, _actor, nullptr, moveRight);
        app->signalParserManager()->parseSignalAccessor(_scene, cfg->sigMoveRight, moveRight, _actor);
        if(moveRight.isValid()==false){
            return false;
        }
    }

    //


    // animations

    Sprite *sprite = _actor->sprite();

    if(cfg->aniTakeOffRight.empty()==false){
        aniTakeOffRight = ObtainAnimationInstance(sprite, cfg->aniTakeOffRight);
        if(aniTakeOffRight==nullptr){
            return false;
        }
    }
    if(cfg->aniTakeOffLeft.empty()==false){
        aniTakeOffLeft = ObtainAnimationInstance(sprite, cfg->aniTakeOffLeft);
        if(aniTakeOffLeft==nullptr){
            return false;
        }
    }

    if(cfg->aniAscendingRight.empty()==false){
        aniAscendingRight = ObtainAnimationInstance(sprite, cfg->aniAscendingRight);
        if(aniAscendingRight==nullptr){
            return false;
        }
    }
    if(cfg->aniAscendingLeft.empty()==false){
        aniAscendingLeft = ObtainAnimationInstance(sprite, cfg->aniAscendingLeft);
        if(aniAscendingLeft==nullptr){
            return false;
        }
    }

    if(cfg->aniAtMaxHeightRight.empty()==false){
        aniAtMaxHeightRight = ObtainAnimationInstance(sprite, cfg->aniAtMaxHeightRight);
        if(aniAtMaxHeightRight==nullptr){
            return false;
        }
    }
    if(cfg->aniAtMaxHeightLeft.empty()==false){
        aniAtMaxHeightLeft = ObtainAnimationInstance(sprite, cfg->aniAtMaxHeightLeft);
        if(aniAtMaxHeightLeft==nullptr){
            return false;
        }
    }

    if(cfg->aniDescendingRight.empty()==false){
        aniDescendingRight = ObtainAnimationInstance(sprite, cfg->aniDescendingRight);
        if(aniDescendingRight==nullptr){
            return false;
        }
    }
    if(cfg->aniDescendingLeft.empty()==false){
        aniDescendingLeft = ObtainAnimationInstance(sprite, cfg->aniDescendingLeft);
        if(aniDescendingLeft==nullptr){
            return false;
        }
    }

    if(cfg->aniLandingRight.empty()==false){
        aniLandingRight = ObtainAnimationInstance(sprite, cfg->aniLandingRight);
        if(aniLandingRight==nullptr){
            return false;
        }
    }
    if(cfg->aniLandingLeft.empty()==false){
        aniLandingLeft = ObtainAnimationInstance(sprite, cfg->aniLandingLeft);
        if(aniLandingLeft==nullptr){
            return false;
        }
    }

    //---
    dbgSystem.removeLastMessage();
    return true;
}



void VGJumpMovement::start(JumpMovementCfg &_cfg, Direction _xDirection, b2Vec2 _inheritedVelocity, bool _startWithDescending)
{

    mXDirection = _xDirection;

    float maxHeight = _cfg.maxHeight;
    float interHeight = maxHeight * _cfg.intermediateHeightRatio;
    float pushOffSpeed = _cfg.pushOffSpeed;
    float interSpeed = pushOffSpeed * _cfg.intermediateSpeedRatio;

    mJumpSpeedGenerator.start(maxHeight, interHeight, pushOffSpeed, interSpeed, _inheritedVelocity.y, _startWithDescending);

    //mXSpeedGenerator.stop();    // in case there is registered speed from previous jump;

    /*
    float xStartVelocity = _initialVelocity.x;
    if(( xStartVelocity > 0.0f && mXDirection == Direction::LEFT ) ||        // the speed is inherited only if it goes in the current direction
       ( xStartVelocity < 0.0f && mXDirection == Direction::RIGHT ))
    {
        xStartVelocity = 0.0f;
    }
    float startSpeed = _cfg.xGroundSpeedInheritFactor * std::abs(xStartVelocity);
    */

    mXSpeedGenerator.setMovement(_cfg.xMaxSpeed, _cfg.xAccDistance, _inheritedVelocity.x);
    //mXSpeedGenerator.dbgName = "JumpXSpeedGenerator";



}


b2Vec2 VGJumpMovement::updateVelocity(float _timeStep, Direction _inputXDirection, int _blockedDirections)
{

    //--- X direction

    if(_blockedDirections & static_cast<int>(mXDirection)){
        mXSpeedGenerator.stop();
        mVelocity.x = 0.0f;

    }else{

        if(_inputXDirection == mXDirection){
            mVelocity.x = mXSpeedGenerator.updateSpeed(_timeStep, true);

        }else{
            mVelocity.x = mXSpeedGenerator.updateSpeed(_timeStep, false);
            //if(mXSpeedGenerator.state()==SpeedGenerator::State::STANDING_STILL){
            if(mXSpeedGenerator.speed()==0.0f){
                if(_inputXDirection==Direction::LEFT || _inputXDirection==Direction::RIGHT){
                    mXDirection = _inputXDirection;
                }
            }
        }
    }
    assert(mXDirection==Direction::LEFT || mXDirection==Direction::RIGHT);

    if(mXDirection==Direction::LEFT){
        mVelocity.x = -mVelocity.x;
    }


    //--- Y direction
    if(_blockedDirections & static_cast<int>(Direction::UP)){

        mJumpSpeedGenerator.setOnBlockingCeiling();

        /*
        if(mJumpSpeedGenerator.speed() >= mJumpSpeedGenerator.interSpeed()){
            mJumpSpeedGenerator.setState(JumpSpeedGenerator::State::DESCENDING_HIGH_SPEED);

        }else{

            mJumpSpeedGenerator.setState(JumpSpeedGenerator::State::DESCENDING_LOW_SPEED);

        }
        */


    }else if(_blockedDirections & static_cast<int>(Direction::DOWN)){
        mJumpSpeedGenerator.setState(JumpSpeedGenerator::State::ON_GROUND);
        //mXSpeedGenerator.stop();
    }

    mVelocity.y = mJumpSpeedGenerator.updateVelocity(_timeStep);


    return mVelocity;

}


void VGJumpMovement::stop()
{

    mXSpeedGenerator.stop();
    mJumpSpeedGenerator.stop();
    mVelocity.SetZero();
}


//---------------------------------------------------------------------------------------------------

JumpMovementEngine::JumpMovementEngine(MovementEngineFactory *_factory) : MovementEngine(_factory)
{
    mDirection = Direction::LEFT;
}



void JumpMovementEngine::createDataObjects(std::vector<MovementEngineCfg *> &_cfgs)
{
    mJumpMovementDatas.resize(_cfgs.size());
    for(unsigned int i=0; i<_cfgs.size(); i++){
        assert(_cfgs[i]->factory == mFactory);
        mJumpMovementDatas.at(i).factory = mFactory;
        mJumpMovementDatas.at(i).cfg = static_cast<JumpMovementCfg*>(_cfgs[i]);
    }

}


bool JumpMovementEngine::initDataObjectsConnections(PlayedScene *_scene, Entity *_actor)
{
    for(JumpMovementData& d : mJumpMovementDatas){
        if(d.initConnections(_scene, _actor)==false){
            return false;
        }
    }

    return true;
}



bool JumpMovementEngine::init(Entity *_entity)
{

    mParentEntity = _entity;

    // store all signals for preUpdate and postUpdate
    mSignals.push_back(&mSigState);

    for(JumpMovementData& d : mJumpMovementDatas){
        if(d.startJump.mSignal == &d.sigStartJumpObj){
            mSignals.push_back(&d.sigStartJumpObj);
        }
        if(d.moveLeft.mSignal == &d.sigMoveLeftObj){
            mSignals.push_back(&d.sigMoveLeftObj);
        }
        if(d.moveRight.mSignal == &d.sigMoveRightObj){
            mSignals.push_back(&d.sigMoveRightObj);
        }
        mSignals.push_back(&d.sigDisabled);
    }

    return true;
}


bool JumpMovementEngine::start(MovementEngineData *_data)
{
    assert(mFactory==_data->factory);

    mCurrentData = static_cast<JumpMovementData*>(_data);

    if(mCurrentData->sigDisabled.active()){
        return false;
    }

    if(mParentEntity->statusFlagsRef() & EntityStatusFlags::FORCED_ENTITY_TRANSFORMATION){
        return false;
    }

    //---

    b2Vec2 inheritedVelocity{0.0f,0.0f};

    MovementEngine *currentEngine = mParentEntity->currentEngine();
    Direction entityDirection = mParentEntity->direction();

    if(currentEngine){
        if(currentEngine->type() == MovementEngineType::GROUND_MOVEMENT){

            //---
            b2Vec2 groundNormal = static_cast<GroundMovementEngineBase*>(currentEngine)->groundNormal();
            if(groundNormal.y < mCurrentData->cfg->maxSlopeNormalY){
                return false;
            }

            //---
            float xVelocity = currentEngine->forwardVelocity().x;
            if(( xVelocity > 0.0f && entityDirection == Direction::LEFT ) ||        // the speed is inherited only if it goes in the current direction
               ( xVelocity < 0.0f && entityDirection == Direction::RIGHT ))
            {
                xVelocity = 0.0f;
            }
            inheritedVelocity.x = std::abs(xVelocity) * mCurrentData->cfg->xGroundSpeedInheritFactor;

            //---
            currentEngine->stop();
        }
    }

    bool startWithDescending = (mParentEntity->blockedDirectionsRef() & static_cast<int>(Direction::DOWN)) == 0 ;      // hole bellow the actor

    if(startWithDescending==false){
        DummyFunction();
    }
    //print("startWithDescending=" + std::to_string(startWithDescending));
    //mJumpMovementEngine->start(_jumpMovementData, mDirection, inheritedVelocity, startWithDescending);
    //mMovementEngineType = MovementEngineType::JUMP_MOVEMENT;

    //JumpMovementEngine *jumpEngine = jumpMovementEngine();
    if(entityDirection!=Direction::LEFT && entityDirection!=Direction::RIGHT){
        entityDirection = mDirection;
    }


    //-----
    mDirection = entityDirection;
    mAnimationPlayer = &mParentEntity->animationPlayer();
    mStateEllapsedTimeS = 0.0f;
    mForwardVelocity.SetZero();

    mJumpMovement.start(*mCurrentData->cfg, mDirection, inheritedVelocity, startWithDescending);


    //---- MANAGE ANIMATIONS
    resetAnimationPlayer();

    if(startWithDescending){
        setState(JumpMovementState::DESCENDING);

        if(mDirection==Direction::LEFT){
            mActiveAnimationInstance = mCurrentData->aniDescendingLeft;

        }else if(mDirection==Direction::RIGHT){
            mActiveAnimationInstance = mCurrentData->aniDescendingRight;
        }

    }else{

        if(mDirection==Direction::LEFT){

            if(mCurrentData->aniTakeOffLeft){
                mActiveAnimationInstance = mCurrentData->aniTakeOffLeft;
                setState(JumpMovementState::TAKING_OFF);

            }else{
                mActiveAnimationInstance = mCurrentData->aniAscendingLeft;
                setState(JumpMovementState::ASCENDING);
            }

        }else if(mDirection==Direction::RIGHT){

            if(mCurrentData->aniTakeOffRight){
                mActiveAnimationInstance = mCurrentData->aniTakeOffRight;
                setState(JumpMovementState::TAKING_OFF);

            }else{
                mActiveAnimationInstance = mCurrentData->aniAscendingRight;
                setState(JumpMovementState::ASCENDING);
            }

        }
    }

    updateAnimationPlayer();

    return true;
}



b2Vec2 JumpMovementEngine::update(EngineUpdateParameters &eup)
{

    mVelocity.SetZero();

    Direction inputXDirection = Direction::NONE;
    if((mParentEntity->statusFlagsRef() & EntityStatusFlags::DISABLED_USER_CONTROLLS)==0){
        if(mCurrentData->moveLeft.active()){
            inputXDirection = Direction::LEFT;

        }else if(mCurrentData->moveRight.active()){
            inputXDirection = Direction::RIGHT;
        }
    }

    if(mCurrentData->cfg->addCeilingSensor){
        manageApproachingCeiling();
    }


    if(mState==JumpMovementState::TAKING_OFF){

        // manage takeoff animation and set state to ASCENDING when the animation finishes
        if(mAnimationPlayer && mAnimationPlayer->GetState()==AnimationPlayerState::STALLED){
            setState(JumpMovementState::ASCENDING);
            mStateEllapsedTimeS = 0.0f;

            if(mDirection==Direction::LEFT){
                mActiveAnimationInstance = mCurrentData->aniAscendingLeft;

            }else if(mDirection==Direction::RIGHT){
                mActiveAnimationInstance = mCurrentData->aniAscendingRight;
            }
        }


    }else if(mState==JumpMovementState::ASCENDING){


        int blockedDirections = eup.blockedDirections;

        //if(mJumpMovement.jumpSpeedGenerator().distance()==0.0){
        if(mJumpMovement.velocity().y >= 0.0f){
            blockedDirections = blockedDirections & (~static_cast<int>(Direction::DOWN));   // ! clear blocked down direction when on ground so the jumping can start
        }

        mVelocity = mJumpMovement.updateVelocity(eup.timeStep, inputXDirection, blockedDirections);

        /*
        if(mJumpMovement.state() == JumpSpeedGenerator::State::AT_MAX_HEIGHT ||
           mJumpMovement.state() == JumpSpeedGenerator::State::DESCENDING_HIGH_SPEED        // set on blocked ascending
           )
        {

            setState(JumpMovementState::DESCENDING;

           if(mJumpMovement.xDirection() != mDirection){
               mDirection = mJumpMovement.xDirection();

               if(mDirection==Direction::RIGHT && mCurrentData->aniDescendingRight){
                   // start aniDescendingRight

               }else if(mDirection==Direction::LEFT && mCurrentData->aniDescendingLeft){
                   // start aniDescendingLeft
               }
            }
        */

        if(mJumpMovement.state() == JumpSpeedGenerator::State::AT_MAX_HEIGHT){

            setState(JumpMovementState::DESCENDING);

           if(mJumpMovement.xDirection() != mDirection){
               mDirection = mJumpMovement.xDirection();
           }

           if(mDirection==Direction::RIGHT){

               if(mCurrentData->aniAtMaxHeightRight){
                   mActiveAnimationInstance = mCurrentData->aniAtMaxHeightRight;

               }else{
                  mActiveAnimationInstance = mCurrentData->aniDescendingRight;
               }

           }else if(mDirection==Direction::LEFT){

               if(mCurrentData->aniAtMaxHeightLeft){
                   mActiveAnimationInstance = mCurrentData->aniAtMaxHeightLeft;

               }else{
                  mActiveAnimationInstance = mCurrentData->aniDescendingLeft;
               }
           }


        }else if(mJumpMovement.state() == JumpSpeedGenerator::State::DESCENDING_HIGH_SPEED ||
                 mJumpMovement.state() == JumpSpeedGenerator::State::DESCENDING_LOW_SPEED){        // Set on blocked ascending !

            setState(JumpMovementState::DESCENDING);

            if(mJumpMovement.xDirection() != mDirection){
                mDirection = mJumpMovement.xDirection();
            }

            if(mDirection==Direction::RIGHT){
                mActiveAnimationInstance = mCurrentData->aniDescendingRight;

            }else if(mDirection==Direction::LEFT){
                mActiveAnimationInstance = mCurrentData->aniDescendingLeft;
            }


        }else if(mJumpMovement.xDirection() != mDirection){
            mDirection = mJumpMovement.xDirection();

            if(mDirection==Direction::RIGHT){
                mActiveAnimationInstance = mCurrentData->aniAscendingRight;

            }else if(mDirection==Direction::LEFT){
                 mActiveAnimationInstance = mCurrentData->aniAscendingLeft;
            }
        }



    }else if(mState==JumpMovementState::DESCENDING){

        // manage blocking in Y Direction
        //if(_blockedDirections & static_cast<int>(Direction::DOWN)){
        //    mJumpMovement.setState(JumpMovement::State::ON_GROUND);
        //}

        mVelocity = mJumpMovement.updateVelocity(eup.timeStep, inputXDirection, eup.blockedDirections);

        if(mJumpMovement.state() == JumpSpeedGenerator::State::ON_GROUND){

            if(mDirection==Direction::RIGHT){
                if(mCurrentData->aniLandingRight){
                    setState(JumpMovementState::LANDING);
                    mActiveAnimationInstance = mCurrentData->aniLandingRight;

                }else{
                    setState(JumpMovementState::LANDED);
                    //mActiveAnimationInstance = mCurrentData->aniLandingRight;

                    mForwardVelocity = mVelocity;
                }

            }else if(mDirection==Direction::LEFT){
                if(mCurrentData->aniLandingLeft){
                    setState(JumpMovementState::LANDING);
                    mActiveAnimationInstance = mCurrentData->aniLandingLeft;

                }else{
                    setState(JumpMovementState::LANDED);
                    //mActiveAnimationInstance = mCurrentData->aniLandingLeft;
                    mForwardVelocity = mVelocity;
                }

            }
        }


    }else if(mState==JumpMovementState::LANDING){

        // manage landing animation and set state to LANDED when the animation finishes

        if(mAnimationPlayer && mAnimationPlayer->GetState()==AnimationPlayerState::STALLED){
            setState(JumpMovementState::LANDED);
            mStateEllapsedTimeS = 0.0f;
        }


    }

    mDirectionSignal._setOnNextFrame(static_cast<int>(mDirection));

    //---
    updateAnimationPlayer();

    return mVelocity;

}


void JumpMovementEngine::manageApproachingCeiling()
{

    EntityContactSignal *ect = mParentEntity->contactTrigger().findEntityContactSignalWithEntityRoleA(EntityRole::CEILING_SENSOR);

    if(ect && ect->activeStarted()){
        if(mJumpMovement.jumpSpeedGenerator().distToCeiling()==0.0f){    // not yet set

            bool notBlockableCeiling = false;
            Entity * otherEntity = ect->mTwoEntitiesContact.entityShapeB().entity;

            if(otherEntity->sourceEntity()->sourceEntityCfg()->passableFromBellow){
                notBlockableCeiling = true;
            }
            if(notBlockableCeiling==false){
                for(TwoEntitiesContact::DualFixture &df : ect->mTwoEntitiesContact.dualFixtures()){
                    b2Fixture *otherFixture = df.fixtureB;
                    FixtureUserData *fud = reinterpret_cast<FixtureUserData *>(otherFixture->GetUserData().pointer);
                    if(fud->properties & FixtureProperties::PASSTHROUGH_FROM_BELLOW){
                        notBlockableCeiling = true;
                        break;
                    }
                }
            }
            if(notBlockableCeiling==false){
                mJumpMovement.jumpSpeedGenerator().setDistToCeiling(gWorldInfo.ceilingSensorHeightMeters());
            }
        }
    }

}


void JumpMovementEngine::stop()
{

    mJumpMovement.stop();

}



MovementEngineData* JumpMovementEngine::getMovementEngineData(const std::string &_name, bool _setErrorMessage)
{
    for(JumpMovementData &d : mJumpMovementDatas){
        if(d.cfg->name == _name){
            return &d;
        }
    }

    if(_setErrorMessage){
        dbgSystem.addMessage("A data instance of 'jumpMovementCfg' object with name '" + _name + "' not found!");
    }

    return nullptr;

}


void JumpMovementEngine::obtainSignal_signalQuery(SignalQuery &_signalQuery, const std::string &_data, const std::string &_signalName, const std::string &_signalValue, bool _setErrorMessage)
{

    if(_data.empty()==false){
        for(JumpMovementData &d : mJumpMovementDatas){
            if(d.cfg->name == _data){

                if(_signalName=="START_JUMP"){
                    _signalQuery.mSignal = d.startJump.mSignal;

                }else if(_signalName=="MOVE_LEFT"){
                    _signalQuery.mSignal = d.moveLeft.mSignal;

                }else if(_signalName=="MOVE_RIGHT"){
                    _signalQuery.mSignal = d.moveRight.mSignal;

                }else if(_signalName=="DISABLED"){
                    _signalQuery.mSignal = &d.sigDisabled;
                }

                //if(_setErrorMessage){
                //    dbgSystem.addMessage("Get signal '" + _signalName + "' error! The signal is unknown!");
                //}

                //return;
                break;
            }
        }

        if(_signalQuery.mSignal){
            return;
        }

        //if(_setErrorMessage){
        //    dbgSystem.addMessage("Get signal '" + _signalName + "' error!  The jump engine data with name '" + _data + "' not found !");
        //}
        //return;

    }

    if(_signalName=="STATE"){
        _signalQuery.mSignal = &mSigState;
        _signalQuery.mIntValue =  static_cast<int>(GetJumpMovementStateFromString(_signalValue));

        if(_signalQuery.mIntValue == static_cast<int>(JumpMovementState::UNKNOWN)){
            dbgSystem.addMessage("Unknown signal value '" + _signalValue + " ' !");
        }
        return;
    }

    if(_setErrorMessage){
        dbgSystem.addMessage("Get signal '" + _signalName + "' error! The signal is unknown!");
    }

}


void JumpMovementEngine::obtainSignal_signalSetter(SignalSetter &_signalSetter, const std::string &_data, const std::string &_signalName, const std::string &_signalValue, bool _setErrorMessage )
{

    if(_data.empty()==false){
        for(JumpMovementData &d : mJumpMovementDatas){
            if(d.cfg->name == _data){
                if(_signalName=="DISABLED"){
                    _signalSetter.mSignal = &d.sigDisabled;
                }

                //if(_setErrorMessage){
                //    dbgSystem.addMessage("Get signal '" + _signalName + "' error! The signal is unknown or not available for setting it!");
                //}
                //return;
                break;
            }
        }

        //if(_setErrorMessage){
        //    dbgSystem.addMessage("Get signal '" + _signalName + "' error!  The jump engine data with name '" + _data + "' not found !");
        //}
        //return;

        if(_signalSetter.mSignal){
            return;
        }

    }

    if(_setErrorMessage){
        dbgSystem.addMessage("Get signal '" + _signalName + "' error! The signal is unknown or not available for setting it!");
    }

}



//=============================================================================================================


JumpMovementCfg* JumpMovementFactory::createMovementEngineCfg(const std::string &_name)
{
    return new JumpMovementCfg(this, _name);
}


JumpMovementEngine* JumpMovementFactory::createMovementEngine()
{
    return new JumpMovementEngine(this);
}


}
