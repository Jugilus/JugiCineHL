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
#include "jpUtilities.h"
#include "jpObjectParser.h"

#include "jpEntitySystem.h"
#include "jpB2Fixture.h"
#include "jpB2Body.h"
#include "jpEntity.h"
#include "jpEntityUtilities.h"
#include "jpB2World.h"
#include "jpMovementCommon.h"
#include "jpGroundMovement.h"



namespace jugimap{




GroundMovementState GetGroundMovementStateFromString(const std::string &state)
{

    if(state=="STANDING"){
        return GroundMovementState::STANDING;

    }else if(state=="IDLE"){
        return GroundMovementState::IDLE;

    }else if(state=="TURNING_AROUND"){
        return GroundMovementState::TURNING_AROUND;

    }else if(state=="MOVING"){
        return GroundMovementState::MOVING;
    }

    return GroundMovementState::UNKNOWN;

}



std::string GetGroundMovementStateString(GroundMovementState state)
{

    if(state==GroundMovementState::STANDING){
        return "STANDING";

    }else if(state==GroundMovementState::IDLE){
        return "IDLE";

    }else if(state==GroundMovementState::TURNING_AROUND){
        return "TURNING_AROUND";

    }else if(state==GroundMovementState::MOVING){
        return "MOVING";

    }else if(state==GroundMovementState::STEEP_SLOPE_STUMBLING){
        return "STEEP_SLOPE_STUMBLING";

    }

    return "UNKNOWN";

}


std::vector<std::pair<std::string, int>>gGroundMovementStateNamedValues
{
    {"STANDING", static_cast<int>(GroundMovementState::STANDING) },
    {"IDLE", static_cast<int>(GroundMovementState::IDLE) },
    {"TURNING_AROUND", static_cast<int>(GroundMovementState::TURNING_AROUND) },
    {"MOVING", static_cast<int>(GroundMovementState::MOVING) },
    {"STEEP_SLOPE_STUMBLING", static_cast<int>(GroundMovementState::STEEP_SLOPE_STUMBLING) }

};


//----------------------------------------------------------------------------




const float GroundMovementCfg::minMaxSlope = 0.0f;
const float GroundMovementCfg::maxMaxSlope = 90.0f;


bool GroundMovementCfg::initCfg(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading movement engine of type '" + factory->typeSignature() + "' and name '" + name + "'...");


    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());

        if(nodeName=="movement"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="maxSpeedP"){
                    maxSpeed = gWorldInfo.pixelsToMeters(a.as_float(maxSpeed));

                }else if(attributeName=="maxSpeedM"){
                    maxSpeed = a.as_float(maxSpeed);

                }else if(attributeName=="accDistanceP"){
                    accDistance = gWorldInfo.pixelsToMeters(a.as_float(accDistance));

                }else if(attributeName=="accDistanceM"){
                    accDistance = a.as_float(accDistance);

                }else if(attributeName=="steepSlopeSpeedP"){
                    steepSlopeSpeed = gWorldInfo.pixelsToMeters(a.as_float(steepSlopeSpeed));

                }else if(attributeName=="steepSlopeSpeedM"){
                    steepSlopeSpeed = a.as_float(steepSlopeSpeed);

                }else if(attributeName=="maxSlope"){
                    maxSlope = ClampValue(a.as_float(maxSlope), minMaxSlope, maxMaxSlope);

                }else if(attributeName=="animationSpeedFactor"){
                    animationSpeedFactor = a.as_float(animationSpeedFactor);

                }else if(attributeName=="jumpXSpeedInheritFactor"){
                    jumpXSpeedInheritFactor = ClampValue(a.as_float(jumpXSpeedInheritFactor), 0.0f, 1.0f);

                }else{

                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                    return false;
                }
            }


        }else if(nodeName=="triggers"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="moveLeft"){
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

                if(childNodeName=="standing"){

                    for(pugi::xml_attribute a = nChild.first_attribute(); a; a = a.next_attribute()){
                        std::string attributeName =std::string(a.name());

                        if(attributeName=="animationLeft"){
                            aniStandingLeft = a.as_string("");

                        }else if(attributeName=="animationRight"){
                            aniStandingRight = a.as_string("");

                        }else if(attributeName=="bodyOrientationLeft"){
                            bodyOrientationStandingLeft = GetDirectionFromString(a.as_string("LEFT"));

                        }else if(attributeName=="bodyOrientationRight"){
                            bodyOrientationStandingRight = GetDirectionFromString(a.as_string("RIGHT"));

                        }else{

                            dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + childNodeName +"' !");
                            return false;
                        }

                    }

                }else if(childNodeName=="turning"){

                    for(pugi::xml_attribute a = nChild.first_attribute(); a; a = a.next_attribute()){
                        std::string attributeName =std::string(a.name());

                        if(attributeName=="animationLeft"){
                            aniTurningToLeft = a.as_string("");

                        }else if(attributeName=="animationRight"){
                            aniTurningToRight = a.as_string("");

                        }else{

                            dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + childNodeName +"' !");
                            return false;
                        }
                    }


                }else if(childNodeName=="idle"){

                    for(pugi::xml_attribute a = nChild.first_attribute(); a; a = a.next_attribute()){
                        std::string attributeName =std::string(a.name());

                        if(attributeName=="animationLeft"){
                            aniIdleLeft = a.as_string("");

                        }else if(attributeName=="animationRight"){
                            aniIdleRight = a.as_string("");

                        }else if(attributeName=="delay"){
                            idleAnimationDelay = a.as_float(idleAnimationDelay);

                        }else{

                            dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + childNodeName +"' !");
                            return false;
                        }
                    }


                }else if(childNodeName=="moving"){

                    for(pugi::xml_attribute a = nChild.first_attribute(); a; a = a.next_attribute()){
                        std::string attributeName =std::string(a.name());

                        if(attributeName=="animationLeft"){
                            aniMovingLeft = a.as_string("");

                        }else if(attributeName=="animationRight"){
                            aniMovingRight = a.as_string("");

                        }else if(attributeName=="bodyOrientationLeft"){
                            bodyOrientationMovingLeft = GetDirectionFromString(a.as_string("LEFT"));

                        }else if(attributeName=="bodyOrientationRight"){
                            bodyOrientationMovingRight = GetDirectionFromString(a.as_string("RIGHT"));

                        }else{

                            dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + childNodeName +"' !");
                            return false;
                        }

                    }


                }else if(childNodeName=="steepSlopeStaggering"){

                    for(pugi::xml_attribute a = nChild.first_attribute(); a; a = a.next_attribute()){
                        std::string attributeName =std::string(a.name());

                        if(attributeName=="animationLeft"){
                            aniSteepSlopeStaggeringLeft = a.as_string("");

                        }else if(attributeName=="animationRight"){
                            aniSteepSlopeStaggeringRight = a.as_string("");

                        }else{

                            dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + childNodeName +"' !");
                            return false;
                        }

                    }

                }else{

                    dbgSystem.addMessage("Unknown node '" + childNodeName + "' !");
                    return false;
                }
            }

        }else if(nodeName=="customState"){

            customStates.push_back(CustomStateCfg());
            CustomStateCfg &cs = customStates.back();

            if(cs.initCfg(n, gGroundMovementStateNamedValues)==false){
                return false;
            }

        }else{

            dbgSystem.addMessage("Unknown node '" + nodeName + "' !");
            return false;
        }

    }


    //b2Vec2 _maxSlope{ std::cos(maxSlope*b2_pi/180.0f), std::sin(maxSlope*b2_pi/180.0f)};
    //maxSlopeNormal.Set(-_maxSlope.y, _maxSlope.x );

    yMinNormalForMaxSlope = std::abs(std::cos(maxSlope*b2_pi/180.0f));


    dbgSystem.removeLastMessage();
    return true;
}



void GroundMovementCfg::collectCustomSensors(std::vector<CustomSensorCfg> &_customSensors)
{

    for(CustomStateCfg & csCfg : customStates){
        if(csCfg.customSensorCfg.customSensorCategory.empty()==false){
            _customSensors.push_back(csCfg.customSensorCfg);
        }
    }
}


//------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------


bool GroundMovementData::initConnections(PlayedScene *_scene, Entity *_actor)
{

    dbgSystem.addMessage("Initializing connections for the data object of '" + cfg->name + "' ...");


    // triggers
    if(cfg->sigMoveLeft.empty()){        // computer controlled actors
        //moveLeft = &moveLeftObj;
        moveLeft.init(&sigMoveLeftObj, SignalQuery::Style::ACTIVE);

    }else{
        //ObtainSignalFromPath(cfg->sigMoveLeft, _scene, _actor, nullptr, moveLeft);
        app->signalParserManager()->parseSignalAccessor(_scene, cfg->sigMoveLeft, moveLeft, _actor);

        if(moveLeft.isValid()==false){
            return false;
        }
    }


    if(cfg->sigMoveRight.empty()){
        //moveRight = &moveRightObj;
        moveRight.init(&sigMoveRightObj, SignalQuery::Style::ACTIVE);

    }else{
        //ObtainSignalFromPath(cfg->sigMoveRight, _scene, _actor, nullptr, moveRight);
        app->signalParserManager()->parseSignalAccessor(_scene, cfg->sigMoveRight, moveRight, _actor);
        if(moveRight.isValid()==false){
            return false;
        }
    }


    // animations

    Sprite *sprite = _actor->sprite();
    //StandardSprite *stdSprite = static_cast<StandardSprite*>(_actor);

    if(cfg->aniStandingRight.empty()==false){
        aniStandingRight = ObtainAnimationInstance(sprite, cfg->aniStandingRight);
        if(aniStandingRight==nullptr){
            return false;
        }
    }
    if(cfg->aniStandingLeft.empty()==false){
        aniStandingLeft = ObtainAnimationInstance(sprite, cfg->aniStandingLeft);
        if(aniStandingLeft==nullptr){
            return false;
        }
    }

    if(cfg->aniTurningToRight.empty()==false){
        aniTurningToRight = ObtainAnimationInstance(sprite, cfg->aniTurningToRight);
        if(aniTurningToRight==nullptr){
            return false;
        }
    }
    if(cfg->aniTurningToLeft.empty()==false){
        aniTurningToLeft = ObtainAnimationInstance(sprite, cfg->aniTurningToLeft);
        if(aniTurningToLeft==nullptr){
            return false;
        }
    }

    if(cfg->aniIdleRight.empty()==false){
        aniIdleRight = ObtainAnimationInstance(sprite, cfg->aniIdleRight);
        if(aniIdleRight==nullptr){
            return false;
        }
    }
    if(cfg->aniIdleLeft.empty()==false){
        aniIdleLeft = ObtainAnimationInstance(sprite, cfg->aniIdleLeft);
        if(aniIdleLeft==nullptr){
            return false;
        }
    }

    if(cfg->aniMovingRight.empty()==false){
        aniMovingRight = ObtainAnimationInstance(sprite, cfg->aniMovingRight);
        if(aniMovingRight==nullptr){
            return false;
        }
    }
    if(cfg->aniMovingLeft.empty()==false){
        aniMovingLeft = ObtainAnimationInstance(sprite, cfg->aniMovingLeft);
        if(aniMovingLeft==nullptr){
            return false;
        }
    }

    if(cfg->aniSteepSlopeStaggeringRight.empty()==false){
        aniSteepSlopeStaggeringRight = ObtainAnimationInstance(sprite, cfg->aniSteepSlopeStaggeringRight);
        if(aniSteepSlopeStaggeringRight==nullptr){
            return false;
        }
    }
    if(cfg->aniSteepSlopeStaggeringLeft.empty()==false){
        aniSteepSlopeStaggeringLeft = ObtainAnimationInstance(sprite, cfg->aniSteepSlopeStaggeringLeft);
        if(aniSteepSlopeStaggeringLeft==nullptr){
            return false;
        }
    }


    //---
    /*
    for(CustomStateCfg & csc : cfg->customStates){
        GroundMovementCustomStateData *cs = nullptr;
        if(csc.baseState==CustomStateBaseState::STANDING){
            customStandingStates.emplace_back();
            cs = &customStandingStates.back();
        }else if(csc.baseState==CustomStateBaseState::MOVING){
            customMovingStates.emplace_back();
            cs = &customMovingStates.back();
        }
        cs->cfg = &csc;

        cs->customActSignalId = _actor->addCustomActName(cs->cfg->name);
    }

    // separating init from construction phase, to avoid copy constructing animation instances
    for(GroundMovementCustomStateData &cs : customStandingStates){
        if(cs.initConnections(_scene, _actor)==false){
            return false;
        }
    }

    for(GroundMovementCustomStateData &cs : customMovingStates){
        if(cs.initConnections(_scene, _actor)==false){
            return false;
        }
    }
    */

    //---
    customStates.resize(cfg->customStates.size());
    for(unsigned int i=0; i<cfg->customStates.size(); i++){
        CustomStateCfg &csCfg = cfg->customStates[i];
        CustomStateData &cs = customStates[i];
        cs.cfg = &csCfg;
        cs.customActSignalId = _actor->addCustomActName(cs.cfg->name);

        if(cs.initConnections(_scene, _actor)==false){
            return false;
        }
    }

    //---
    yMinNormalForMaxSlope = cfg->yMinNormalForMaxSlope;


    //---
    dbgSystem.removeLastMessage();
    return true;
}


GroundMovementData::~GroundMovementData()
{

    if(aniStandingLeft){
        delete aniStandingLeft;
    }
    if(aniStandingRight){
        delete aniStandingRight;
    }

    if(aniIdleLeft){
        delete aniIdleLeft;
    }
    if(aniIdleRight){
        delete aniIdleRight;
    }

    if(aniTurningToLeft){
        delete aniTurningToLeft;
    }
    if(aniTurningToRight){
        delete aniTurningToRight;
    }

    if(aniMovingLeft){
        delete aniMovingLeft;
    }
    if(aniMovingRight){
        delete aniMovingRight;
    }

    if(aniSteepSlopeStaggeringLeft){
        delete aniSteepSlopeStaggeringLeft;
    }
    if(aniSteepSlopeStaggeringRight){
        delete aniSteepSlopeStaggeringRight;
    }

}


//==========================================================================================================


void VGGroundMovement::startMovement(GroundMovementCfg &cfg, Direction _direction, float _startSpeed, SlidingGroundCfg *_slidingCfg)
{

    //mSpeedGenerator.dbgName = "GroundSpeedGenerator";
    float accDistance = (_slidingCfg == nullptr)? cfg.accDistance : _slidingCfg->accDistance;
    mSpeedGenerator.setMovement(cfg.maxSpeed, accDistance, _startSpeed);
    //mSpeedGenerator.setMovement(cfg.maxSpeed, cfg.accDistance, _startSpeed);

    mDirection = _direction;

    if(_direction==Direction::LEFT){
        mInputUnitVelocity.Set(-1.0f, 0.0f);

    }else if(_direction==Direction::RIGHT){
        mInputUnitVelocity.Set(1.0f, 0.0f);

    }else if(_direction==Direction::UP){
        mInputUnitVelocity.Set(0.0f, 1.0f);

    }else if(_direction==Direction::DOWN){
        mInputUnitVelocity.Set(0.0f, -1.0f);
    }

}


b2Vec2 VGGroundMovement::updateVelocity(float _timeStep, Direction _inputDirection, int _blockedDirections, b2Vec2 _slopeNormal)
{

    //if(mDirection == Direction::NONE){
    //    return {0.0f, 0.0f};

    //}

    if(static_cast<int>(mDirection) & _blockedDirections){
        //mDirection = Direction::NONE;
        mSpeedGenerator.stop();
        //mSpeed = 0.0f;
        mVelocity.SetZero();
        return mVelocity;
    }

    //float dt = 1.0f / 60.0f;
    //---

    //manage slopes

    //float dot = b2Dot(mUnitVelocity,mSlopeNormal);
    // dot = 0 -> velocity is perpendicular to slope normal, which menas in direction of the slope -> moving allowed
    // dot = 1 -> velocity is parrallel with slope normal -> moving is blocked
    // threshold is 45° (dot = diagonalUnitSize), if dot is larger moving is blocked, if smaller moving is allowed but velocity is adjusted to the slope tangent

    //dot = std::abs(dot);    // needed for platforming style of movement

    //if(dot > diagonalUnitSize+0.05){
    //    print("MOVEMENT BLOCKED  dot=" + std::to_string(dot));
    //    mState = SpeedState::STANDSTILL;
    //    mVelocity.SetZero();
    //    return mVelocity;
   // }


    mUnitVelocity = mInputUnitVelocity;
    if(_slopeNormal.y != 0.0f){

       //print("dot=" + std::to_string(dot) + "   uv x=" + std::to_string(mUnitVelocity.x) + " y=" + std::to_string(mUnitVelocity.y)+
       //      "   n x=" + std::to_string(mSlopeNormal.x) + " y=" + std::to_string(mSlopeNormal.y));

        if(mUnitVelocity.x * _slopeNormal.x > 0){
            mUnitVelocity.y = - std::abs(_slopeNormal.x);
        }else{
            mUnitVelocity.y = std::abs(_slopeNormal.x);
        }
        //mUnitVelocity.x = sign(mInputUnitVelocity.x) * std::abs(mSlopeNormal.y);
        mUnitVelocity.x = Sgn(mInputUnitVelocity.x) * std::abs(_slopeNormal.y);
    }

    //unitVelocity += mExtraVelocity;

    bool generateSpeed = (_inputDirection == mDirection);
    float speed = mSpeedGenerator.updateSpeed(_timeStep, generateSpeed);

    //if(mSpeedGenerator.state()==SpeedGenerator::State::STANDING_STILL){
    //    if(_inputDirection != mDirection){
    //       setDirection(_inputDirection);
    //    }
    //}


    mVelocity = speed * mUnitVelocity;

    return mVelocity;

}


void VGGroundMovement::stop()
{

    mSpeedGenerator.stop();
    mVelocity.SetZero();

}



//==========================================================================================================


/*
bool ESlopeSliding::init(GroundMovementCfg &cfg)
{

    mSpeedAt45deg = cfg.iceSlidingSpeedAt45deg;
    mAccDistanceAt45degSpeed = cfg.iceSlidingAccDistance;

    mDecelerationDistance = 0.0f;
    mSlopeNormal.SetZero();
    mDirection = Direction::NONE;

    return true;
}
*/



void ESlopeSliding::setActiveSlidingGroundCfg(SlidingGroundCfg *_cfg)
{

    if(_cfg == mActiveSlidingGroundCfg){
        return;
    }

    mActiveSlidingGroundCfg = _cfg;
    if(_cfg){
        mSpeedAt45deg = _cfg->speedOn45degSlope;
        mAccDistanceAt45degSpeed = _cfg->accDistance;
    }

    mDecelerationDistance = 0.0f;
    mSlopeNormal.SetZero();
    mSpeed = 0.0f;
    mDirection = Direction::NONE;

}


void ESlopeSliding::stop()
{
    mSlopeNormal.SetZero();
    mSpeed = 0.0f;
    mVelocity.SetZero();

}


b2Vec2 ESlopeSliding::updateVelocity(float _timeStep, b2Vec2 _prevEngineVelocity, b2Vec2 _slopeNormal)
{

    float dt = _timeStep;
    mVelocity.SetZero();

    if(mSlopeNormal.x != _slopeNormal.x || mSlopeNormal.y != _slopeNormal.y){

        //mUnitVelocity.SetZero();

        float slopAngle = 90.0f - 180.0f * atan2(std::abs(_slopeNormal.y), std::abs(_slopeNormal.x))/b2_pi;
        assert(slopAngle >= 0.0f && slopAngle <=90.0f);
        float f = slopAngle/45.0;
        mSpeedMax = mSpeedAt45deg * f;

        if(_slopeNormal.x > 0.0f){
            mUnitVelocity.Set(_slopeNormal.y, -_slopeNormal.x);
            mDirection = Direction::RIGHT;
            mDecelerationDistance = mAccDistanceAt45degSpeed * f;

            if(mDecelerationDistance>0.01){
                mDeceleration = (mSpeedMax*mSpeedMax)/(2.0f * mDecelerationDistance);
            }else{
                mDeceleration = 0.0f;
            }


        }else if(_slopeNormal.x < 0.0f){
            mUnitVelocity.Set(-_slopeNormal.y, _slopeNormal.x);
            mDirection = Direction::LEFT;
            mDecelerationDistance = mAccDistanceAt45degSpeed * f;

            if(mDecelerationDistance>0.01){
                mDeceleration = (mSpeedMax*mSpeedMax)/(2.0f * mDecelerationDistance);
            }else{
                mDeceleration = 0.0f;
            }

        }else{
            // plains

            if(mDirection == Direction::RIGHT && _prevEngineVelocity.x > 0.0f){         //
                mUnitVelocity.Set(1.0f, 0.0f);

            }else if(mDirection == Direction::LEFT && _prevEngineVelocity.x < 0.0f){
                mUnitVelocity.Set(-1.0f, 0.0f);

            }else{
                mUnitVelocity.SetZero();
            }

        }

        mSlopeNormal = _slopeNormal;
    }

    /*
    if(static_cast<int>(mDirection) & _blockedDirections){
        mSpeed = 0.0f;
        mVelocity.SetZero();
        print("SLIDING blocked!");

        return mVelocity;
    }
    */


    if(mSpeed < mSpeedMax){

        // no acceleration, full speed immediately !

        //mSpeed += mAcc * dt;

        //if(mSpeed > speedMax){
            mSpeed = mSpeedMax;
        //}

    }else if(mSpeed > mSpeedMax){

        if(mDeceleration > 0.0f){
            mSpeed -= mDeceleration * dt;

        }else{
            mSpeed = mSpeedMax;
        }

        if(mSpeed<mSpeedMax){
            mSpeed = mSpeedMax;
        }
    }


    print("SLIDING mSpeed="+ std::to_string(mSpeed) + " speedMax=" + std::to_string(mSpeedMax) + " mDecelerationDistance="  + std::to_string(mDecelerationDistance));


    mVelocity.x = mUnitVelocity.x * mSpeed;
    mVelocity.y = mUnitVelocity.y * mSpeed;


    return mVelocity;

}


//==========================================================================================================



void EGroundConveyor::setActiveConveyorCfg(ConveyorGroundCfg *_cfg)
{

    if(_cfg == mActiveConveyorCfg){
        return;
    }

    mActiveConveyorCfg = _cfg;
    if(_cfg){
        mSpeed = _cfg->speed;
        mDirection = _cfg->direction;

        if(mDirection==Direction::LEFT){
            mDirectionUnitVelocity.Set(-1.0f, 0.0f);

        }else if(mDirection==Direction::RIGHT){
            mDirectionUnitVelocity.Set(1.0f, 0.0f);
        }

    }else{
        mSpeed = 0.0f;
        mDirection = Direction::NONE;
        mDirectionUnitVelocity.SetZero();
    }


}


void EGroundConveyor::stop()
{
    mVelocity.SetZero();
}


b2Vec2 EGroundConveyor::updateVelocity(float _timeStep, int _blockedDirections, b2Vec2 _slopeNormal)
{

    b2Vec2 unitVelocity = mDirectionUnitVelocity;
    if(_slopeNormal.y != 0.0f){

       //print("dot=" + std::to_string(dot) + "   uv x=" + std::to_string(mUnitVelocity.x) + " y=" + std::to_string(mUnitVelocity.y)+
       //      "   n x=" + std::to_string(mSlopeNormal.x) + " y=" + std::to_string(mSlopeNormal.y));

        if(unitVelocity.x * _slopeNormal.x > 0){
            unitVelocity.y = - std::abs(_slopeNormal.x);
        }else{
            unitVelocity.y = std::abs(_slopeNormal.x);
        }
        //mUnitVelocity.x = sign(mInputUnitVelocity.x) * std::abs(mSlopeNormal.y);
        unitVelocity.x = Sgn(mDirectionUnitVelocity.x) * std::abs(_slopeNormal.y);
    }

    //print("CONVEYOR mSpeed="+ std::to_string(mSpeed));

    mVelocity.x = unitVelocity.x * mSpeed;
    mVelocity.y = unitVelocity.y * mSpeed;


    return mVelocity;

}


//----------------------------------------------------------------------------------


bool GroundMovementEngine::init(Entity *_entity)
{


    mParentEntity = _entity;
    mSignals.push_back(&mSigState);

    for(GroundMovementData& d : mGroundMovementDatas){
        if(d.moveLeft.signal() == &d.sigMoveLeftObj){
            mSignals.push_back(&d.sigMoveLeftObj);
        }
        if(d.moveRight.signal() == &d.sigMoveRightObj){
            mSignals.push_back(&d.sigMoveRightObj);
        }
        mSignals.push_back(&d.sigDisabled);

    }

    return true;
}


bool GroundMovementEngine::start(MovementEngineData *_data)
{

    assert(_data->factory==mFactory);


    if(mParentEntity->statusFlagsRef() & EntityStatusFlags::FORCED_ENTITY_TRANSFORMATION){
        return false;
    }

    //-----
    float inheritedSpeed = 0.0f;

    MovementEngine *currentEngine = mParentEntity->currentEngine();
    Direction entityDirection = mParentEntity->direction();

    if(currentEngine){
        if(currentEngine->type() == MovementEngineType::JUMP_MOVEMENT){

            //JumpMovementEngine * jumpEngine = static_cast<JumpMovementEngine*>(mCurrentEngine);
            //float xJumpVelocity = jumpEngine->jumpMovement().velocity().x;

            float xVelocity = currentEngine->forwardVelocity().x;

            if(( xVelocity > 0.0f && entityDirection == Direction::LEFT ) ||        // the speed is inherited only if it goes in the current direction
               ( xVelocity < 0.0f && entityDirection == Direction::RIGHT ))
            {
                xVelocity = 0.0f;
            }
            inheritedSpeed = std::abs(xVelocity)* mCurrentData->cfg->jumpXSpeedInheritFactor;

            //---
            currentEngine->stop();


        //}else if(currentEngine->type() == BehaviorEngineType::GROUND_MOVEMENT){
        }else if(currentEngine == this){
            //GroundMovementEngine * engine = static_cast<GroundMovementEngine*>(currentEngine);
            Direction bodyOrientation = getBodyOrientation();
            if(bodyOrientation != Direction::NONE){
                entityDirection = bodyOrientation;
                //_entity->_setDirection(entityDirection);
            }

        }
    }

    //GroundMovementEngine *groundEngine = groundMovementEngine();
    //BehaviorEngine *nextEngine = getMovementEngine(BehaviorEngineType::GROUND_MOVEMENT);
    //GroundMovementEngineBase * groundEngine = static_cast<GroundMovementEngineBase*>(nextEngine);

    if(entityDirection!=Direction::LEFT && entityDirection!=Direction::RIGHT){
        entityDirection = mDirection;
        //_entity->_setDirection(entityDirection);
    }


    mCurrentData = static_cast<GroundMovementData*>(_data);
    mDirection = entityDirection;
    mAnimationPlayer = &mParentEntity->animationPlayer();
   // mCurrentData->_startSpeed = inheritedSpeed;


    //mDirection = _direction;
    //mAnimationPlayer = _animationPlayer;

    //mDirection = mCurrentData->_direction;
    //mAnimationPlayer = mCurrentData->_animationPlayer;
    //float _startSpeed = mCurrentData->_startSpeed;

    mStateEllapsedTimeS = 0.0f;
    mForwardVelocity.SetZero();
    resetAnimationPlayer();


    //----
    if(inheritedSpeed > 0.0f){
        mState = GroundMovementState::MOVING;
        mGroundMovement.startMovement(*mCurrentData->cfg, mDirection, inheritedSpeed, mSlopeSliding.slidingGroundCfg());


        if(mDirection==Direction::LEFT){
            mActiveAnimationInstance = mCurrentData->aniMovingLeft;

        }else if(mDirection==Direction::RIGHT){
            mActiveAnimationInstance = mCurrentData->aniMovingRight;

        }

    }else{
        mState = GroundMovementState::STANDING;

        if(mDirection==Direction::LEFT){
            mActiveAnimationInstance = mCurrentData->aniStandingLeft;

        }else if(mDirection==Direction::RIGHT){
            mActiveAnimationInstance = mCurrentData->aniStandingRight;
        }

    }

    mCustomStateEngine.init(this, &(mCurrentData->customStates));

    //---
    //mSlopeSliding.init(*mCurrentData->cfg);

    //---
    updateAnimationPlayer();


    return true;

}


b2Vec2 GroundMovementEngine::update(EngineUpdateParameters &eup)
{

    mGroundNormal = eup.groundNormal;
    mStateEllapsedTimeS += eup.timeStep;

    mVelocity.SetZero();
    mVelocity = mExtraVelocity;


    if(mState==GroundMovementState::UNKNOWN){
        DummyFunction();
    }

    //if(mGroundNormal.y <= mCurrentData->cfg->maxSlopeNormal.y && eup.contactsWithDifferentGroundNormals==false){
    if(mGroundNormal.y < mCurrentData->cfg->yMinNormalForMaxSlope && eup.contactsWithDifferentGroundNormals==false){

        updateSteepSlopeSliding(eup);

        //---
        mGroundMovement.stop();
        setState(GroundMovementState::STEEP_SLOPE_STUMBLING);
        mStateEllapsedTimeS = 0.0f;

    }else{

        if(mState==GroundMovementState::STEEP_SLOPE_STUMBLING){
            if(mDirection==Direction::LEFT){
                mActiveAnimationInstance = mCurrentData->aniStandingLeft;

            }else if(mDirection==Direction::RIGHT){
                mActiveAnimationInstance = mCurrentData->aniStandingRight;
            }
            setState(GroundMovementState::STANDING);
        }

        //----
        mInputDirection = Direction::NONE;
        if((eup.entity->statusFlagsRef() & EntityStatusFlags::DISABLED_USER_CONTROLLS)==0){

            if(mCurrentData->moveLeft.active()){
                mInputDirection = Direction::LEFT;

            }else if(mCurrentData->moveRight.active()){
                mInputDirection = Direction::RIGHT;
            }
        }

        //----
        manageCustomStates(eup);

        //----
        if(mState==GroundMovementState::STANDING || mState==GroundMovementState::IDLE){
            update_stateSTANDING(eup);

        }else if(mState==GroundMovementState::TURNING_AROUND){
            update_stateTURNING_AROUND(eup);

        }else if(mState==GroundMovementState::MOVING){
            update_stateMOVING(eup);

        }

        //----
        if(mSlidingGroundCfg != mSlopeSliding.slidingGroundCfg()){
            mSlopeSliding.setActiveSlidingGroundCfg(mSlidingGroundCfg);
        }
        if(mSlopeSliding.isActive()){
            updateSliding(eup);
        }

        //----
        if(mConveyorGroundCfg != mGroundConveyor.conveyorGroundCfg()){
            mGroundConveyor.setActiveConveyorCfg(mConveyorGroundCfg);
        }
        if(mGroundConveyor.isActive()){
            updateConveyor(eup);
        }else{
            DummyFunction();
        }

        //if(mGroundNormal.y <= mCurrentData->cfg->maxSlopeNormal.y  && eup.contactsWithDifferentGroundNormals==false){
        //    mGroundMovement.stop();
        //    updateSteepSlopeSliding(eup);
       // }

    }

       /*
    if(mVelocity.x > 0.0f){
        if(eup.blockedDirections & static_cast<int>(Direction::RIGHT)){
            mVelocity.SetZero();
        }
    }else if(mVelocity.x < 0.0f){
        if(eup.blockedDirections & static_cast<int>(Direction::LEFT)){
            mVelocity.SetZero();
        }
    }
    */

   // print("normal x="+std::to_string(eup.groundNormal.x) + " y=" + std::to_string(eup.groundNormal.y));



    mDirectionSignal.setValue_onNextFrame(static_cast<int>(mDirection));

    //---
    updateAnimationPlayer();


    //if(mState==GroundMovementState::UNKNOWN){
    //    DummyFunction();
    //}

    //if(mCurrentData->cfg->name=="movingMovableObjectOnRight"){
    //    print(GetGroundMovementStateString(mState));
    //    print("B movingMovableObjectOnRight velocity x=" + std::to_string(mVelocity.x));
    //}

    mForwardVelocity = mVelocity;



    return mVelocity;
}


void GroundMovementEngine::updateGroundMovement(EngineUpdateParameters &eup)
{



}


void GroundMovementEngine::update_stateSTANDING(EngineUpdateParameters &eup)
{

    if(mCustomStateEngine.activeCustomState()){
        return;
    }


    if(mInputDirection == mDirection){

        if((static_cast<int>(mDirection) & eup.blockedDirections) == 0){

            //if(mCurrentData->cfg->name=="pushingMovableObjectRightMovement"){
            //    DummyFunction();
            //}

            mGroundMovement.startMovement(*mCurrentData->cfg, mInputDirection, 0.0f, mSlopeSliding.slidingGroundCfg());
            //mSlopeSliding.init(*mCurrentData->cfg);
            setState(GroundMovementState::MOVING);
            mStateEllapsedTimeS = 0.0f;

            if(mDirection==Direction::LEFT){
                mActiveAnimationInstance = mCurrentData->aniMovingLeft;

            }else if(mDirection==Direction::RIGHT){
                mActiveAnimationInstance = mCurrentData->aniMovingRight;
            }
        }

    }else if(mInputDirection != Direction::NONE){


        mDirection = mInputDirection;
        if(mDirection==Direction::LEFT){
            //if(mCurrentData->cfg->name=="pushingMovableObjectRightMovement"){
            //    DummyFunction();
            //}
            if(mCurrentData->aniTurningToLeft){
                 mActiveAnimationInstance = mCurrentData->aniTurningToLeft;
                 setState(GroundMovementState::TURNING_AROUND);

            }else{
                mActiveAnimationInstance = mCurrentData->aniStandingLeft;
            }


        }else if(mDirection==Direction::RIGHT){
            //if(mCurrentData->cfg->name=="pushingMovableObjectRightMovement"){
            //    DummyFunction();
            //}
             if(mCurrentData->aniTurningToRight){
                 mActiveAnimationInstance = mCurrentData->aniTurningToRight;
                 setState(GroundMovementState::TURNING_AROUND);

             }else{
                 mActiveAnimationInstance = mCurrentData->aniStandingRight;
             }
        }
        mStateEllapsedTimeS = 0.0f;


    }else{

        if(mState==GroundMovementState::STANDING){
            if(mCurrentData->aniIdleRight && mCurrentData->aniIdleLeft){
                if(mStateEllapsedTimeS > mCurrentData->cfg->idleAnimationDelay){
                    if(mDirection==Direction::LEFT){
                        mActiveAnimationInstance = mCurrentData->aniIdleLeft;

                    }else if(mDirection==Direction::RIGHT){
                        mActiveAnimationInstance = mCurrentData->aniIdleRight;

                    }
                    setState(GroundMovementState::IDLE);
                    mStateEllapsedTimeS = 0.0f;
                }
            }

        }else if(mState==GroundMovementState::IDLE){
            if(mAnimationPlayer && mAnimationPlayer->GetState()==AnimationPlayerState::STALLED){
                if(mDirection==Direction::LEFT){
                    mActiveAnimationInstance = mCurrentData->aniStandingLeft;

                }else if(mDirection==Direction::RIGHT){
                    mActiveAnimationInstance = mCurrentData->aniStandingRight;
                }
                setState(GroundMovementState::STANDING);
                mStateEllapsedTimeS = 0.0f;
            }
        }
    }

}


void GroundMovementEngine::update_stateMOVING(EngineUpdateParameters &eup)
{


    if(mCustomStateEngine.activeCustomState()){
        if(mCustomStateEngine.activeCustomState()->cfg->engineMovementAllowed){
            mVelocity = mGroundMovement.updateVelocity(eup.timeStep, mInputDirection, eup.blockedDirections, mGroundNormal);

        }else{
            mInputDirection = Direction::NONE;
            setState(GroundMovementState::STANDING);
        }
        return;
    }


    mVelocity = mGroundMovement.updateVelocity(eup.timeStep, mInputDirection, eup.blockedDirections, mGroundNormal);


    if(mSlopeSliding.isActive()){

        if(mGroundMovement.speedGenerator().state()==SpeedGenerator::State::DECELERATING){
            if(mDirection==Direction::LEFT){
                mActiveAnimationInstance = mCurrentData->aniStandingLeft;

            }else if(mDirection==Direction::RIGHT){
                mActiveAnimationInstance = mCurrentData->aniStandingRight;
            }

            //--- when moving UP the slope there is no decelerating on stopping
            if(mVelocity.y > 0.0f){
                mGroundMovement.speedGenerator().stop();
            }

        }else if(mGroundMovement.speedGenerator().state()==SpeedGenerator::State::ACCELERATING){
            if(mDirection==Direction::LEFT){
                mActiveAnimationInstance = mCurrentData->aniMovingLeft;

            }else if(mDirection==Direction::RIGHT){
                mActiveAnimationInstance = mCurrentData->aniMovingRight;
            }

            //--- when moving down the slope there is no accelerating on start
            if(mVelocity.y < 0.0f){
                mGroundMovement.speedGenerator().setFullSpeed();
            }

        }else if(mGroundMovement.speedGenerator().state()==SpeedGenerator::State::STANDING_STILL){
            setState(GroundMovementState::STANDING);
            mStateEllapsedTimeS = 0.0f;

        }

    }else{

        if(mGroundMovement.speed()==0.0f){

            if(mCurrentData->cfg->name=="pushingMovableObjectRightMovement"){
                DummyFunction();
            }

            setState(GroundMovementState::STANDING);
            mStateEllapsedTimeS = 0.0f;

            if(mDirection==Direction::LEFT){
                mActiveAnimationInstance = mCurrentData->aniStandingLeft;

            }else if(mDirection==Direction::RIGHT){
                mActiveAnimationInstance = mCurrentData->aniStandingRight;
            }
        }
    }

    //if(mCurrentData->cfg->name=="movingMovableObjectOnRight"){
    //    print("update_stateMOVING movingMovableObjectOnRight velcoity x=" + std::to_string(mVelocity.x));
    //}

}


void GroundMovementEngine::update_stateTURNING_AROUND(EngineUpdateParameters &eup)
{

    if(mCustomStateEngine.activeCustomState()){
        return;
    }


    if(mAnimationPlayer && mAnimationPlayer->GetState()==AnimationPlayerState::STALLED){
        setState(GroundMovementState::STANDING);
        mStateEllapsedTimeS = 0.0f;

        if(mDirection==Direction::LEFT){
            mActiveAnimationInstance = mCurrentData->aniStandingLeft;

        }else if(mDirection==Direction::RIGHT){
            mActiveAnimationInstance = mCurrentData->aniStandingRight;
        }
    }

}


/*
GroundMovementCustomStateData *GroundMovementEngine::checkForCustomState()
{

    GroundMovementCustomStateData *customState = nullptr;

    for(GroundMovementCustomStateData &cs : mCurrentData->customStandingStates){

        if(cs.contextTrigger.signal()){
            if(cs.contextTrigger.active()==false){
                continue;
            }
        }

        if(cs.sqTrigger.active()){
            customState = &cs;
            if(mDirection==Direction::LEFT){
                mActiveAnimationInstance = customState->aniLeft;

            }else if(mDirection==Direction::RIGHT){
                mActiveAnimationInstance = customState->aniRight;

            }
            mStateEllapsedTimeS = 0.0f;

            if(cs.cfg->activePhaseFrameIntData == -1){
                mParentEntity->customActSignal().setValue_onNextFrame(cs.customActSignalId);
            }
            break;
        }
    }


    return customState;
}


void GroundMovementEngine::updateCustomState(EngineUpdateParameters &eup)
{

    assert(mCustomState);



    if(mAnimationPlayer){

        if(mCustomState->triggeringExtraShapeSourceFixture){

            bool extraShapeActive = false;

            if(mAnimationPlayer->GetAnimationInstance()->GetKind()==AnimationKind::FRAME_ANIMATION){

            }else if(mAnimationPlayer->GetAnimationInstance()->GetKind()==AnimationKind::TIMELINE_ANIMATION){

                //if(mAnimationPlayer->GetLatestUpdateReturnFlag() & AnimationPlayerFlags::META_KEY_CHANGED){
                    AKMeta* metaKey = mAnimationPlayer->GetActiveMetaKey();
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
        if(mAnimationPlayer->GetAnimationInstance()->GetKind()==AnimationKind::FRAME_ANIMATION){
            if(mCustomState->cfg->activePhaseFrameIntData != -1){
                FrameAnimationInstance *fai = static_cast<FrameAnimationInstance*>(mAnimationPlayer->GetAnimationInstance());
                int currentFrameIntData = fai->currentFrameIntData();
                IntSignal &customActSignal = mParentEntity->customActSignal();
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

        if(mAnimationPlayer->GetState()==AnimationPlayerState::STALLED){

            GroundMovementCustomStateData *customState = checkForCustomState();
            if(customState==nullptr){
                if(mDirection==Direction::LEFT){
                    mActiveAnimationInstance = mCurrentData->aniStandingLeft;

                }else if(mDirection==Direction::RIGHT){
                    mActiveAnimationInstance = mCurrentData->aniStandingRight;
                }
                mStateEllapsedTimeS = 0.0f;
                mCustomState = nullptr;
                eup.entity->statusFlagsRef() &= ~EntityStatusFlags::FORCED_ENTITY_TRANSFORMATION;
                mParentEntity->customActSignal().setValue_onNextFrame(-1);

            }else{
                // immediately start a new (or existing) custom state
                //if(customState==mCustomState){
                    mStateEllapsedTimeS = 0.0f;
                    mAnimationPlayer->Stop();
                    mAnimationPlayer->Play(mActiveAnimationInstance);

                    mParentEntity->customActSignal().reset(-1);
                    mParentEntity->customActSignal().setValue_onNextFrame(mCustomState->customActSignalId);
                //}
                DummyFunction();
            }

        }
    }

}
*/

void GroundMovementEngine::updateSteepSlopeSliding(EngineUpdateParameters &eup)
{


    b2Vec2 unitVelocity{0.0f, 0.0f};

    if(mGroundNormal.x > 0.0f){
        unitVelocity.Set(mGroundNormal.y, -mGroundNormal.x);

    }else if(mGroundNormal.x < 0.0f){
        unitVelocity.Set(-mGroundNormal.y, mGroundNormal.x);
    }

    unitVelocity.x = -unitVelocity.x * 1;     // this will push toward the ground; needed to avoid jittering when actor try t


    mVelocity.x += unitVelocity.x * mCurrentData->cfg->steepSlopeSpeed;
    mVelocity.y += unitVelocity.y * mCurrentData->cfg->steepSlopeSpeed;


    //----
    if(mDirection==Direction::LEFT){
        mActiveAnimationInstance = (mCurrentData->aniSteepSlopeStaggeringLeft != nullptr)? mCurrentData->aniSteepSlopeStaggeringLeft : mCurrentData->aniStandingLeft;

    }else if(mDirection==Direction::RIGHT){
        mActiveAnimationInstance = (mCurrentData->aniSteepSlopeStaggeringRight != nullptr)? mCurrentData->aniSteepSlopeStaggeringRight : mCurrentData->aniStandingRight;
    }


}


void GroundMovementEngine::updateSliding(EngineUpdateParameters &eup)
{


    b2Vec2 slidingVelocity = mSlopeSliding.updateVelocity(eup.timeStep, eup.prevEngineVelocity, mGroundNormal);

    if(mGroundMovement.velocity().y<0.0f)   // movement down the slope
    {
        // it looks the best if we adjust mVelocity and set sliding velocity to zero
        float speedMax = std::max(mGroundMovement.speed(), mSlopeSliding.speed());
        mVelocity.x = mGroundMovement.unitVelocity().x * speedMax;
        mVelocity.y = mGroundMovement.unitVelocity().y * speedMax;
        slidingVelocity.SetZero();

    }else{

        mVelocity = mGroundMovement.velocity() + slidingVelocity;

    }

    // For situations where sliding movement is stronger and in opposite direction than the normal ground movement
    // we check out for movement blocking ( there is some jittering if we check inside 'mSlopeSliding.updateVelocity' )
    if(mVelocity.x > 0.0f){
        if(eup.blockedDirections & static_cast<int>(Direction::RIGHT)){
            mSlopeSliding.stop();
        }
    }else if(mVelocity.x < 0.0f){
        if(eup.blockedDirections & static_cast<int>(Direction::LEFT)){
            mSlopeSliding.stop();
        }
    }


}


void GroundMovementEngine::updateConveyor(EngineUpdateParameters &eup)
{

    b2Vec2 conveyorVelocity = mGroundConveyor.updateVelocity(eup.timeStep, eup.blockedDirections, mGroundNormal);

    //print("conveyorVelocity x=" + std::to_string(conveyorVelocity.x) + " y=" + std::to_string(conveyorVelocity.y));

    mVelocity += conveyorVelocity;

    // For situations where conveyor movement is stronger and in opposite direction than the normal ground movement
    // we check out for movement blocking ( there is some jittering if we check inside 'mGroundConveyor.updateVelocity' )
    if(mVelocity.x > 0.0f){
        if(eup.blockedDirections & static_cast<int>(Direction::RIGHT)){
            mGroundConveyor.stop();
        }
    }else if(mVelocity.x < 0.0f){
        if(eup.blockedDirections & static_cast<int>(Direction::LEFT)){
            mGroundConveyor.stop();
        }
    }
}


void GroundMovementEngine::manageCustomStates(EngineUpdateParameters &eup)
{


    CustomStateStatus status = mCustomStateEngine.update(eup, static_cast<int>(mState));

    if(status==CustomStateStatus::CUSTOM_STATE_ENDED){

        if(mState==GroundMovementState::STANDING){
            if(mDirection==Direction::LEFT){
                mActiveAnimationInstance = mCurrentData->aniStandingLeft;
            }else if(mDirection==Direction::RIGHT){
                mActiveAnimationInstance = mCurrentData->aniStandingRight;
            }

        }else if(mState==GroundMovementState::MOVING){
            if(mDirection==Direction::LEFT){
                mActiveAnimationInstance = mCurrentData->aniMovingLeft;
            }else if(mDirection==Direction::RIGHT){
                mActiveAnimationInstance = mCurrentData->aniMovingRight;
            }
        }
    }

}




void GroundMovementEngine::stop()
{
    mGroundMovement.stop();
    resetAnimationPlayer();

}


Direction GroundMovementEngine::getBodyOrientation()
{

    if(mState==GroundMovementState::STANDING){
        if(mDirection==Direction::LEFT){
            return mCurrentData->cfg->bodyOrientationStandingLeft;
        }else if(mDirection==Direction::RIGHT){
            return mCurrentData->cfg->bodyOrientationStandingRight;
        }

    }else if(mState==GroundMovementState::MOVING){
        if(mDirection==Direction::LEFT){
            return mCurrentData->cfg->bodyOrientationMovingLeft;
        }else if(mDirection==Direction::RIGHT){
            return mCurrentData->cfg->bodyOrientationMovingRight;
        }

    }

    return Direction::NONE;

}


void GroundMovementEngine::createDataObjects(std::vector<MovementEngineCfg *> &_cfgs)
{

    mGroundMovementDatas.resize(_cfgs.size());
    for(unsigned int i=0; i<_cfgs.size(); i++){
        assert(_cfgs[i]->factory == mFactory);
        mGroundMovementDatas.at(i).factory = mFactory;
        mGroundMovementDatas.at(i).cfg = static_cast<GroundMovementCfg*>(_cfgs[i]);
    }

}


bool GroundMovementEngine::initDataObjectsConnections(PlayedScene *_scene, Entity *_actor)
{

    for(GroundMovementData& d : mGroundMovementDatas){
        if(d.initConnections(_scene, _actor)==false){
            return false;
        }

        //---
        /*
        std::string signalParsingPrefix = d.cfg->getSignalParsingPrefix();
        _actor->signalStorage()->addSignal_query(d.moveLeft.signal(), signalParsingPrefix + "MOVE_LEFT");
        _actor->signalStorage()->addSignal_query(d.moveRight.signal(), signalParsingPrefix + "MOVE_RIGHT");
        _actor->signalStorage()->addSignal_query(&d.sigDisabled, signalParsingPrefix + "DISABLED");
        SignalIdentifier *si = _actor->signalStorage()->addSignal_query(&mSigState, signalParsingPrefix + "STATE");
        if(si) si->intValuesIdentifier = &gGroundMovementStateNamedValues;

        // setter signals
        _actor->signalStorage()->addSignal_setter(&d.sigDisabled, signalParsingPrefix +"DISABLED");
        */
    }

    return true;
}


void GroundMovementEngine::collectSignalsForLUT(SignalStorage &_storage)
{

    for(GroundMovementData& d : mGroundMovementDatas){

        std::string signalParsingPrefix = d.cfg->getSignalParsingPrefix();
        _storage.addSignal_query(d.moveLeft.signal(), signalParsingPrefix + "MOVE_LEFT");
        _storage.addSignal_query(d.moveRight.signal(), signalParsingPrefix + "MOVE_RIGHT");
        _storage.addSignal_query(&d.sigDisabled, signalParsingPrefix + "DISABLED");
        SignalIdentifier *si = _storage.addSignal_query(&mSigState, signalParsingPrefix + "STATE");
        if(si) si->intValuesIdentifier = &gGroundMovementStateNamedValues;

        // setter signals
        _storage.addSignal_setter(&d.sigDisabled, signalParsingPrefix +"DISABLED");
    }

}



MovementEngineData* GroundMovementEngine::getMovementEngineData(const std::string &_name, bool _setErrorMessage)
{
    for(GroundMovementData &d : mGroundMovementDatas){
        if(d.cfg->name == _name){
            return &d;
        }
    }

    if(_setErrorMessage){
        dbgSystem.addMessage("A data instance of 'groundMovementCfg' object with name '" + _name + "' not found!");
    }

    return nullptr;
}

/*
GroundMovementData* GroundMovementEngine::getGroundMovementData(const std::string &_name, bool _setErrorMessage)
{

    for(GroundMovementData &d : mGroundMovementDatas){
        if(d.cfg->name == _name){
            return &d;
        }
    }

    if(_setErrorMessage){
        dbgSystem.addMessage("A data instance of 'groundMovementCfg' object with name '" + _name + "' not found!");
    }

    return nullptr;

}
*/

//===========================================================================================================


MovementEngineCfg* GroundMovementFactory::createMovementEngineCfg(const std::string &_name)
{
    return new GroundMovementCfg(this, _name);
}


GroundMovementEngine* GroundMovementFactory::createMovementEngine()
{
    return new GroundMovementEngine(this);
}


}
