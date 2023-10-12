#include <chrono>
#include <assert.h>
#include <sstream>
#include "pugixml/pugixml.hpp"
#include "jmCommonFunctions.h"
#include "jmSystem.h"
#include "jmVectorShape.h"
#include "jmStandardSprite.h"
#include "jpUtilities.h"
#include "jmSceneLayout.h"
#include "jmUtilities.h"

#include "jpPlayedScene.h"
#include "jpPlayedApp.h"

#include "scripting/jpBehavior.h"
#include "jpSourceEntity.h"
#include "jpEntity.h"
#include "jpEntityUtilities.h"
#include "jpB2Body.h"
#include "jpB2World.h"
#include "jpEntitySystem.h"
#include "jpPathMovement.h"



namespace jugimap{





PathMovementState GetPathMovementStateFromString(const std::string &state)
{


    if(state=="STANDING"){
        return PathMovementState::STANDING;

    }else if(state=="IDLE"){
        return PathMovementState::IDLE;

    }else if(state=="TURNING_AROUND"){
        return PathMovementState::TURNING_AROUND;

    }else if(state=="MOVING"){
        return PathMovementState::MOVING;
    }

    return PathMovementState::UNKNOWN;

}


//------------------------------------------------------------------------------



bool PathMovementCfg::initCfg(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading movement engine of type '" + factory->typeSignature() + "' and name '" + name + "'...");


    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());

        if(nodeName=="parameters"){

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

                //}else if(attributeName=="angularSpeed"){
                //    maxAngularSpeed = a.as_float(maxAngularSpeed);

                //}else if(attributeName=="angularAccDistance"){
                //    angularAccDistance = a.as_float(angularAccDistance);

                }else if(attributeName=="pathways"){
                    pathwayCategories = StdString::splitString(std::string(a.as_string("")), ",");

                }else{

                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                    return false;
                }
            }


        }else if(nodeName=="triggers"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="moveForward"){
                    sigMoveForward = a.as_string("");

                }else if(attributeName=="moveBackward"){
                    sigMoveBackward = a.as_string("");

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

                        if(attributeName=="animationForward"){
                            aniStandingForward = a.as_string("");

                        }else if(attributeName=="animationBackward"){
                            aniStandingBackward = a.as_string("");

                        }else{

                            dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + childNodeName +"' !");
                            return false;
                        }

                    }

                }else if(childNodeName=="turning"){

                    for(pugi::xml_attribute a = nChild.first_attribute(); a; a = a.next_attribute()){
                        std::string attributeName =std::string(a.name());

                        if(attributeName=="animationForward"){
                            aniTurningToForward = a.as_string("");

                        }else if(attributeName=="animationBackward"){
                            aniTurningToBackward = a.as_string("");

                        }else{

                            dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + childNodeName +"' !");
                            return false;
                        }

                    }

                }else if(childNodeName=="idle"){

                    for(pugi::xml_attribute a = nChild.first_attribute(); a; a = a.next_attribute()){
                        std::string attributeName =std::string(a.name());

                        if(attributeName=="animationForward"){
                            aniIdleForward = a.as_string("");

                        }else if(attributeName=="animationBackward"){
                            aniIdleBackward = a.as_string("");

                        }else if(attributeName=="idleAnimationDelay"){
                            idleAnimationDelay = a.as_float(idleAnimationDelay);

                        }else{

                            dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + childNodeName +"' !");
                            return false;
                        }

                    }

                }else if(childNodeName=="moving"){

                    for(pugi::xml_attribute a = nChild.first_attribute(); a; a = a.next_attribute()){
                        std::string attributeName =std::string(a.name());

                        if(attributeName=="animationForward"){
                            aniMovingForward = a.as_string("");

                        }else if(attributeName=="animationBackward"){
                            aniMovingBackward = a.as_string("");

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


PathMovementData::~PathMovementData()
{

    if(aniStandingForward){
        delete aniStandingForward;
    }
    if(aniStandingBackward){
        delete aniStandingBackward;
    }

    if(aniIdleForward){
        delete aniIdleForward;
    }
    if(aniIdleBackward){
        delete aniIdleBackward;
    }

    if(aniTurningToForward){
        delete aniTurningToForward;
    }
    if(aniTurningToBackward){
        delete aniTurningToBackward;
    }

    if(aniMovingForward){
        delete aniMovingForward;
    }
    if(aniMovingBackward){
        delete aniMovingBackward;
    }

}


bool PathMovementData::initConnections(PlayedScene *_scene, Entity *_actor)
{

    dbgSystem.addMessage("Init connections for pathways for the data object of '" + cfg->name + "' ...");


    // triggers
    if(cfg->sigMoveForward.empty()){      // computer controlled entity !
        moveForward.init(&sigMoveForwardObj, SignalQuery::Style::ACTIVE);

    }else{
        //ObtainSignalFromPath(cfg->sigMoveForward, _scene, _actor, nullptr, moveForward);
        app->signalParserManager()->parseSignalAccessor(_scene, cfg->sigMoveForward, moveForward, _actor);
        if(moveForward.isValid()==false){
            return false;
        }
    }

    if(cfg->sigMoveBackward.empty()){      // computer controlled entity !
        moveBackward.init(&sigMoveBackwardObj, SignalQuery::Style::ACTIVE);

    }else{
        //ObtainSignalFromPath(cfg->sigMoveBackward, _scene, _actor, nullptr, moveBackward);
        app->signalParserManager()->parseSignalAccessor(_scene, cfg->sigMoveBackward, moveBackward, _actor);
        if(moveBackward.isValid()==false){
            return false;
        }
    }


    for(const std::string &s : cfg->pathwayCategories){
        EntityCategory *ec = _actor->parentEntitySystem()->entityCategoriesGroup().getEntityCategory(s);
        if(ec==nullptr){
            return false;
        }
        pathwayCategories.push_back(ec);
    }

    if(pathwayCategories.empty()){
        dbgSystem.addMessage("No patways category has been defined!");
        return false;
    }


    // animations
    StandardSprite *stdSprite = static_cast<StandardSprite*>(_actor->sprite());

    if(cfg->aniStandingForward.empty()==false){
        aniStandingForward = ObtainFrameAnimationInstance(stdSprite, cfg->aniStandingForward);
        if(aniStandingForward==nullptr){
            return false;
        }
    }
    if(cfg->aniStandingBackward.empty()==false){
        aniStandingBackward = ObtainFrameAnimationInstance(stdSprite, cfg->aniStandingBackward);
        if(aniStandingBackward==nullptr){
            return false;
        }
    }

    if(cfg->aniMovingForward.empty()==false){
        aniMovingForward = ObtainFrameAnimationInstance(stdSprite, cfg->aniMovingForward);
        if(aniMovingForward==nullptr){
            return false;
        }
    }
    if(cfg->aniMovingBackward.empty()==false){
        aniMovingBackward = ObtainFrameAnimationInstance(stdSprite, cfg->aniMovingBackward);
        if(aniMovingBackward==nullptr){
            return false;
        }
    }

    if(cfg->aniIdleForward.empty()==false){
        aniIdleForward = ObtainFrameAnimationInstance(stdSprite, cfg->aniIdleForward);
        if(aniIdleForward==nullptr){
            return false;
        }
    }
    if(cfg->aniIdleBackward.empty()==false){
        aniIdleBackward = ObtainFrameAnimationInstance(stdSprite, cfg->aniIdleBackward);
        if(aniIdleBackward==nullptr){
            return false;
        }
    }

    if(cfg->aniTurningToForward.empty()==false){
        aniTurningToForward = ObtainFrameAnimationInstance(stdSprite, cfg->aniTurningToForward);
        if(aniTurningToForward==nullptr){
            return false;
        }
    }
    if(cfg->aniTurningToBackward.empty()==false){
        aniTurningToBackward = ObtainFrameAnimationInstance(stdSprite, cfg->aniTurningToBackward);
        if(aniTurningToBackward==nullptr){
            return false;
        }
    }


    dbgSystem.removeLastMessage();
    return true;
}



//==========================================================================


PathwayAccessedSignal::PathwayAccessedSignal(Entity *_actor, PathMovementData *_pathMovementData, PathMovementEngine *_pathMovementEngine) : CustomEntityBoolSignal(),
    mActor(_actor),
    mPathMovementData(_pathMovementData),
    mPathMovementEngine(_pathMovementEngine)
{
}


void PathwayAccessedSignal::update()
{

    // check commands for accessing pathway
    if(mPathMovementData->moveForward.active()==false && mPathMovementData->moveBackward.active()==false){
        _unsetOnNextFrame();
        return;
    }

     Direction direction = (mPathMovementData->moveForward.active()==true)? Direction::FORWARD : Direction::BACKWARD;

    for(EntityContactSignal& ect : mActor->contactTrigger().contactedEntitiesTriggers()){
        assert(mActor == ect.mTwoEntitiesContact.entityShapeA().entity);
        Entity* pathway = ect.mTwoEntitiesContact.entityShapeB().entity;

        if(pathway->mainShapeRole()!=EntityRole::PATHWAY) continue;
        if(StdVector::contains(mPathMovementData->pathwayCategories, pathway->sourceEntity()->sourceEntityCfg()->category)==false){
            continue;
        }

        PathwaySourceEntityCfg * pathwayCfg = pathway->sourceEntity()->sourceEntityCfg()->mPathwaySourceEntityCfg.get();

        mPathMovementData->vectorShape = pathway->vectorShape();
        mPathMovementData->pathwayOrientation = pathwayCfg->orientation;

        if(mPathMovementData->pathwayOrientation==PathwayOrientation::VERTICAL){
            if(mPathMovementData->vectorShape->pathPoints().front().y < mPathMovementData->vectorShape->pathPoints().back().y){
                mPathMovementData->verticalPathwayDirection = VerticalPathwayDirection::UP;
            }else{
                mPathMovementData->verticalPathwayDirection = VerticalPathwayDirection::DOWN;
            }
        }else if(mPathMovementData->pathwayOrientation==PathwayOrientation::HORIZONTAL){
            if(mPathMovementData->vectorShape->pathPoints().front().x < mPathMovementData->vectorShape->pathPoints().back().x){
                mPathMovementData->horizontalPathwayDirection = HorizontalPathwayDirection::RIGHT;
            }else{
                mPathMovementData->horizontalPathwayDirection = HorizontalPathwayDirection::LEFT;
            }
        }

        //mPathMovementData->centeringMarginM = pathwayCfg->centeringMarginM;
        mPathMovementData->centeringMarginM = pathway->sourceEntity()->sourceEntityCfg()->centeringMargin;
        mPathMovementData->centeringStyle = pathwayCfg->centeringStyle;
        mPathMovementData->actorPathwayContactOffsetM = pathwayCfg->getContactPointOffsetForActor(mActor->sourceEntity()->name());
        mPathMovementData->pathway = pathway;

        //b2Vec2 actorPosM = mActor->body()->B2Body()->GetPosition();


        if(mPathMovementEngine->preStartIfPathwayAccessed(mPathMovementData, direction, &mActor->animationPlayer())){
            _setOnNextFrame();
            return;
        }
    }

    mPathMovementData->vectorShape = nullptr;    // ! ('vectorShape' is used as flag that pathwayMovementEngine is ready to be used)
    mPathMovementData->pathway = nullptr;

    _unsetOnNextFrame();
    return;

}



//==========================================================================


PathwayLeftSignal::PathwayLeftSignal(Entity *_actor, PathMovementData *_pathMovementData, PathMovementEngine *_pathwayEngine, MovementEngineData *_targetEngineData) : CustomEntityBoolSignal(),
    mActor(_actor),
    mPathMovementData(_pathMovementData),
    mPathwayEngine(_pathwayEngine),
    mTargetEngineData(_targetEngineData)
{

}


void PathwayLeftSignal::update()
{

    assert(mPathwayEngine == mActor->currentEngine());

    if(mTargetEngineData->factory->type()==MovementEngineType::GROUND_MOVEMENT){

        GroundMovementDataBase *groundMovementDataBase = static_cast<GroundMovementDataBase*>(mTargetEngineData);

        bool activeGroundMoveCommand = (groundMovementDataBase->moveLeft.active() || groundMovementDataBase->moveRight.active());

        if(mPathwayEngine->isPathwayFinished(activeGroundMoveCommand)){
            _setOnNextFrame();
            return;
        }

        if(mPathMovementData->moveBackward.active() && (mActor->blockedDirectionsRef() & static_cast<int>(Direction::DOWN))){
            _setOnNextFrame();
            return;
        }

    }


   _unsetOnNextFrame();

}




//===================================================================================



bool VGPathMovement::init(PathMovementData &_data, Direction _direction, b2Vec2 _actorContactPosM)
{

    mPath = _data.vectorShape;
    mPathwayOrientation = _data.pathwayOrientation;

    mDirection = _direction;
    mStep = (mDirection==Direction::FORWARD)?  1 : -1;


    Vec2f actorContactPosP = gWorldInfo.metersToPixels<Vec2f>(_actorContactPosM);
    bool positionFound = obtainCurrentPositionOnPath(actorContactPosP);

    return positionFound;

    //mCurrentDistanceP = _actorPosM.distanceP;
    //mCurrentPosP = _actorPosM.pointP;
    //mCurrentDistanceM = gWorldInfo.pixelsToMeters(mCurrentDistanceP);

}


bool VGPathMovement::initMovement(PathMovementData &_data, Direction _direction, b2Vec2 _actorContactPosM)
{

    mDirection = _direction;
    mStep = (mDirection==Direction::FORWARD)?  1 : -1;


    Vec2f actorContactPosP = gWorldInfo.metersToPixels<Vec2f>(_actorContactPosM);
    obtainCurrentPositionOnPath(actorContactPosP);


    //if(mCurrentDistanceP==0.0f && mStep==-1){
    //    return false;
    //}
    //if(mCurrentDistanceP==mPath->pathLength() && mStep==1){
    //    return false;
    //}

    mSpeedGenerator.setMovement(_data.cfg->maxSpeed, _data.cfg->accDistance, 0.0f);
    return true;

}



b2Vec2 VGPathMovement::updateVelocity(float _timeStep, Direction _inputDirection, int _blockedDirections, b2Vec2 _actorContactPosM)
{


    if(static_cast<int>(mDirection) & _blockedDirections){
        //mDirection = Direction::NONE;
        mSpeedGenerator.stop();
        //mSpeed = 0.0f;
        mVelocity.SetZero();
        return mVelocity;
    }


    bool generateSpeed = (_inputDirection == mDirection);
    float speed = mSpeedGenerator.updateSpeed(_timeStep, generateSpeed);

    if(speed==0.0f){
        mVelocity.SetZero();
        return mVelocity;
    }


    //Vec2f actorContactPosP = gWorldInfo.metersToPixels<Vec2f>(_actorContactPosM);
    //obtainCurrentPositionOnPath(actorContactPosP);


    float d = speed * _timeStep;
    float newDistanceM = mCurrentDistanceM + mStep*d;
    float newDistanceP = gWorldInfo.metersToPixels(newDistanceM);

    if(newDistanceP <= 0){
        speed = 0.0f;
        mCurrentDistanceP = 0;
        mCurrentDistanceM = 0.0f;
        mCurrentPosP.set(mPath->pathPoints().front().x, mPath->pathPoints().front().y);
        mCurrentPosM = gWorldInfo.pixelsToMeters(mCurrentPosP);
        print ("VGPathMovement::updateVelocity mCurrentDistanceP = 0.0f !");

    }else if(newDistanceP >= mPath->pathLength()){
        speed = 0.0f;
        mCurrentDistanceP = mPath->pathLength();
        mCurrentDistanceM = gWorldInfo.pixelsToMeters(mCurrentDistanceP);
        mCurrentPosP.set(mPath->pathPoints().back().x, mPath->pathPoints().back().y);
        mCurrentPosM = gWorldInfo.pixelsToMeters(mCurrentPosP);

        print ("VGPathMovement::updateVelocity mCurrentDistanceP = pathLength !");

    }else{

        obtainUnitVelocity(newDistanceP);
        mCurrentDistanceM = newDistanceM;
        mCurrentDistanceP = newDistanceP;
    }

    mVelocity = speed * mUnitVelocity;

    print("mCurrentDistanceP=" + std::to_string(mCurrentDistanceP) + " mCurrentIndex=" + std::to_string(mCurrentIndex));
    //print("mCurrentPosM x=" +std::to_string(mCurrentPosM.x) + " y=" +std::to_string(mCurrentPosM.y) + " mVelocity x=" + std::to_string(mVelocity.x) + " y="+ std::to_string(mVelocity.y));

    return mVelocity;
}


void VGPathMovement::obtainUnitVelocity(float newDistanceP)
{

    std::vector<PathPoint> & pathPoints = mPath->pathPoints();

    float dist = mCurrentDistanceP;
    int nextIndex = mCurrentIndex;



    while(true){

        nextIndex += mStep;

        if(nextIndex >= int(pathPoints.size())){
            if(mPath->isClosed()){
                //index = 0;
                nextIndex = 1;  // the last point of the closed shape is at the same position as the first point so we set to the second point
            }else{
                nextIndex = pathPoints.size()-1;
                break;
            }

        }else if(nextIndex < 0){
            if(mPath->isClosed()){
                //index = pathPoints.size()-1;
                nextIndex = pathPoints.size()-2;    // the last point of the closed shape is at the same position as the first point

            }else{
                nextIndex = 0;
                break;
            }
        }

        PathPoint p1 = pathPoints[mCurrentIndex];
        PathPoint p2 = pathPoints[nextIndex];

        if(mStep==1){

            if(newDistanceP>=p1.distance && newDistanceP<=p2.distance){
                //Vec2f newCurrentPosP = MakeVec2fAtDistanceFromP1(p1, p2, newDistanceP - p1.distance);
                break;
            }

        }else{
            if(newDistanceP>=p2.distance && newDistanceP<=p1.distance){
                //Vec2f newCurrentPosP = MakeVec2fAtDistanceFromP1(p1, p2, newDistanceP - p1.distance);
                break;
            }
        }

        mCurrentIndex = nextIndex;

        //float d = DistanceTwoPoints(p1, p2);
        //dist += d;

        //if(nextIndex == mSegment.index2){
        //    break;
        //}
        //if(newDistanceP < dist){
        //    break;
        //}else{
        //    mCurrentIndex = nextIndex;
        //    mCurrentDistanceP += d;
        //}
    }


    //---
    if(mCurrentIndex!=nextIndex){
        PathPoint p1 = pathPoints[mCurrentIndex];
        PathPoint p2 = pathPoints[nextIndex];
        //float d = DistanceTwoPoints(p1, p2);
        //mUnitVelocity.Set((p1.x-p2.x)/d, (p1.y-p2.y)/d);

        Vec2f newCurrentPosP;   // = MakeVec2fAtDistanceFromP1(p2, p1, dist - newDistanceP);
        if(mStep==1){
            newCurrentPosP = MakeVec2fAtDistanceFromP1(p1, p2, newDistanceP - p1.distance);
        }else{
            newCurrentPosP = MakeVec2fAtDistanceFromP1(p2, p1, newDistanceP - p2.distance);
        }

        float d = DistanceTwoPoints(newCurrentPosP, mCurrentPosP);
        if(d>0.01f){
            mUnitVelocity.Set((newCurrentPosP.x-mCurrentPosP.x)/d, (newCurrentPosP.y-mCurrentPosP.y)/d);
        }else{
            mUnitVelocity.SetZero();
        }

        mCurrentPosP = newCurrentPosP;
        mCurrentPosM = gWorldInfo.pixelsToMeters(mCurrentPosP);

        //mCurrentDistanceP = newDistanceP;
        //mCurrentDistanceM = gWorldInfo.pixelsToMeters(mCurrentDistanceP);

    }else{
        mUnitVelocity.SetZero();
    }

}


bool VGPathMovement::obtainCurrentPositionOnPath(Vec2f _actorContactPosP)
{

    //vertical

    if(mPath->pathPoints().size() < 2){
        return false;
    }


    bool positionFound = false;


    //Vec2f actorP1(_actorPosP.x, _actorPosP.y - 37.5);
    Vec2f actorP1(_actorContactPosP.x, _actorContactPosP.y);

    Vec2f actorP2 = actorP1;
    Vec2f pIntersected;
    float distMin = 999999.0f;

    if(mPathwayOrientation==PathwayOrientation::VERTICAL){
        actorP2.x += 100;

        // there is no threshold how much can actor be outside the path

        if(mPath->pathPoints().front().y < mPath->pathPoints().back().y){
            if(actorP1.y <= mPath->pathPoints().front().y){
                mCurrentPosP = mPath->pathPoints().front();
                mCurrentDistanceP = 0.0f;
                mCurrentIndex = (mStep==1)? 0 : 1;
                positionFound = true;

            }else if(actorP1.y >= mPath->pathPoints().back().y){
                mCurrentPosP = mPath->pathPoints().back();
                mCurrentDistanceP = mPath->pathLength();
                mCurrentIndex = (mStep==1)? mPath->pathPoints().size()-2 : mPath->pathPoints().size()-1;
                positionFound = true;
            }

        }else if(mPath->pathPoints().front().y > mPath->pathPoints().back().y){
            if(actorP1.y >= mPath->pathPoints().front().y){
                mCurrentPosP = mPath->pathPoints().front();
                mCurrentDistanceP = 0.0f;
                mCurrentIndex = (mStep==1)? 0 : 1;
                positionFound = true;
            }else if(actorP1.y <= mPath->pathPoints().back().y){
                mCurrentPosP = mPath->pathPoints().back();
                mCurrentDistanceP = mPath->pathLength();
                mCurrentIndex = (mStep==1)? mPath->pathPoints().size()-2 : mPath->pathPoints().size()-1;
                positionFound = true;
            }

        }

        if(positionFound==false){
            for(int i=0; i<int(mPath->pathPoints().size())-1; i++){

                PathPoint &p1 = mPath->pathPoints()[i];
                PathPoint &p2 = mPath->pathPoints()[i+1];
                float yMin = std::min(p1.y, p2.y);
                float yMax = std::max(p1.y, p2.y);

                if(actorP1.y >= yMin && actorP1.y <= yMax){

                    if(Intersect<Vec2f>(p1, p2, actorP1, actorP2, pIntersected)){
                        float dist = DistanceTwoPoints(actorP1, pIntersected);
                        if(dist<distMin){
                            distMin = dist;
                            mCurrentPosP = pIntersected;
                            mCurrentDistanceP= p1.distance + DistanceTwoPoints(p1, pIntersected);
                            mCurrentIndex = (mStep==1)? i : i+1;
                            positionFound = true;
                        }

                    }
                }
            }
        }


    }else if(mPathwayOrientation==PathwayOrientation::HORIZONTAL){
        actorP2.y += 100;


        for(int i=0; i<int(mPath->pathPoints().size())-1; i++){

            PathPoint &p1 = mPath->pathPoints()[i];
            PathPoint &p2 = mPath->pathPoints()[i+1];
            float xMin = std::min(p1.x, p2.x);
            float xMax = std::max(p1.x, p2.x);

            if(actorP1.x >= xMin && actorP1.x <= xMax){

                if(Intersect<Vec2f>(p1, p2, actorP1, actorP2, pIntersected)){
                    float dist = DistanceTwoPoints(actorP1, pIntersected);
                    if(dist<distMin){
                        distMin = dist;
                        mCurrentPosP = pIntersected;
                        mCurrentDistanceP= p1.distance + DistanceTwoPoints(p1, pIntersected);
                        mCurrentIndex = (mStep==1)? i : i+1;
                        positionFound = true;
                    }
                }
            }
        }
    }

    mCurrentDistanceM = gWorldInfo.pixelsToMeters(mCurrentDistanceP);
    mCurrentPosM = gWorldInfo.pixelsToMeters(mCurrentPosP);





    return positionFound;

}


bool VGPathMovement::isPathwayStart()
{

    float threshold = gWorldInfo.minGeometryBlockPixels() * 0.5;
    return mCurrentDistanceP < threshold ;                          // must be threshold

}


bool VGPathMovement::isPathwayEnd()
{

    float threshold = gWorldInfo.minGeometryBlockPixels() * 0.5;
    return mCurrentDistanceP > mPath->pathLength() - threshold;

}


b2Vec2 VGPathMovement::currentPoint()
{

    if(mPath==nullptr){
        return b2Vec2();
    }

    return gWorldInfo.pixelsToMeters(mPath->pathPoints().at(mCurrentIndex));

}


void VGPathMovement::stop()
{
    mSpeedGenerator.stop();
    mVelocity.SetZero();

}



//============================================================================================



PathMovementEngine::PathMovementEngine(MovementEngineFactory *_factory) : MovementEngine(_factory)
{
    mDirection = Direction::FORWARD;
}


void PathMovementEngine::createDataObjects(std::vector<MovementEngineCfg *> &_cfgs)
{

    mPathMovementDatas.resize(_cfgs.size());
    for(unsigned int i=0; i<_cfgs.size(); i++){
        assert(_cfgs[i]->factory == mFactory);
        mPathMovementDatas.at(i).factory = mFactory;
        mPathMovementDatas.at(i).cfg = static_cast<PathMovementCfg*>(_cfgs[i]);
    }

}


bool PathMovementEngine::initDataObjectsConnections(PlayedScene *_scene, Entity *_actor)
{

    for(PathMovementData& d : mPathMovementDatas){
        if(d.initConnections(_scene, _actor)==false){
            return false;
        }
    }

    return true;
}


bool PathMovementEngine::preStartIfPathwayAccessed(PathMovementData *_data, Direction _direction, AnimationPlayer *_animationPlayer)
{

    assert(mFactory==_data->factory);

    mPrestarted = false;
    mCurrentData = _data;
    mDirection = _direction;
    mAnimationPlayer = _animationPlayer;


    if(mCurrentData->sigDisabled.active()){
        return false;
    }

    //--- blocked directions are set for cardinal directions and we need to set corresponding blocked pathway direction

    b2Vec2 actorPosM = mParentEntity->body()->B2Body()->GetPosition();
    b2Vec2 actorContactPosM = actorPosM + _data->actorPathwayContactOffsetM;


    bool positionOnPathwayFound = mVelocityGenerator.init(*_data, mDirection, actorContactPosM);
    if(positionOnPathwayFound==false){
        return false;
    }


    /*

    if(mDirection == Direction::FORWARD){

        //if(mVelocityGenerator.isPathwayStart()==false){
        //    return false;
        //}
        if(mVelocityGenerator.isPathwayEnd()){
            return false;
        }


        if(_data->pathwayOrientation==PathwayOrientation::VERTICAL){


            //if(mCurrentData->vectorShape->pathPoints().front().y > mCurrentData->vectorShape->pathPoints().back().y){
            if(_data->verticalPathwayDirection==VerticalPathwayDirection::DOWN){
                if(mParentEntity->blockedDirectionsRef() & static_cast<int>(Direction::DOWN)){
                    return false;
                }
            }

            if(_data->centeringMarginM.xMarginActive){
                if((actorContactPosM.x <  mVelocityGenerator.currentPositionM().x - _data->centeringMarginM.left)  ||
                    (actorContactPosM.x >  mVelocityGenerator.currentPositionM().x + _data->centeringMarginM.right))
                {
                    print("Actor not within centering margin!");
                    return false;
                }
            }

        }

        //if(DistanceTwoPoints(actorContactPosM, mVelocityGenerator.currentPositionM()) > _data->centeringMarginM){
        //    print("Actor not within centering margin!");
        //    return false;
        //}

    }else if(mDirection == Direction::BACKWARD){
        if(mVelocityGenerator.isPathwayStart()){
            return false;
        }

        if(_data->pathwayOrientation==PathwayOrientation::VERTICAL){

            //if(mCurrentData->vectorShape->pathPoints().front().y < mCurrentData->vectorShape->pathPoints().back().y){
            if(_data->verticalPathwayDirection==VerticalPathwayDirection::UP){
                if(mParentEntity->blockedDirectionsRef() & static_cast<int>(Direction::DOWN)){
                    return false;
                }
            }

            if(_data->centeringMarginM.xMarginActive){
                if((actorContactPosM.x <  mVelocityGenerator.currentPositionM().x - _data->centeringMarginM.left)  ||
                    (actorContactPosM.x >  mVelocityGenerator.currentPositionM().x + _data->centeringMarginM.right))
                {
                    print("Actor not within centering margin!");
                    return false;
                }
            }
        }
    }

    */



    if(mDirection == Direction::FORWARD){

        if(mVelocityGenerator.isPathwayEnd()){
            return false;
        }

    }else if(mDirection == Direction::BACKWARD){

        if(mVelocityGenerator.isPathwayStart()){
            return false;
        }
    }

    //---
    if(_data->pathwayOrientation==PathwayOrientation::VERTICAL){

        if(_data->centeringMarginM.xMarginActive){
            if((actorContactPosM.x <  mVelocityGenerator.currentPositionM().x - _data->centeringMarginM.left)  ||
                (actorContactPosM.x >  mVelocityGenerator.currentPositionM().x + _data->centeringMarginM.right))
            {
                print("Actor not within centering margin!");
                return false;
            }
        }

        if(mParentEntity->blockedDirectionsRef() & static_cast<int>(Direction::DOWN)){

            if(_data->verticalPathwayDirection==VerticalPathwayDirection::DOWN && mDirection == Direction::FORWARD){
                if(mParentEntity->statusFlagsRef() & EntityStatusFlags::MOVABLE_OBJECT_BELLOW){
                    return false;
                }
            }else if(_data->verticalPathwayDirection==VerticalPathwayDirection::UP && mDirection == Direction::BACKWARD){
                if(mParentEntity->statusFlagsRef() & EntityStatusFlags::MOVABLE_OBJECT_BELLOW){
                    return false;
                }
            }
        }

    }


    //----

    mCurrentData->exitPathPointsIndexes.clear();

    std::string pathwayExitPoints = _data->vectorShape->parameters().value("ePathwayExitPoints");
    if(pathwayExitPoints.empty()){
        mCurrentData->exitPathPointsIndexes.push_back(0);
        mCurrentData->exitPathPointsIndexes.push_back(_data->vectorShape->pathPoints().size()-1);

    }else{
        std::vector<std::string>strIndexes = StdString::splitString(pathwayExitPoints,",");
        for(const std::string &s : strIndexes){
            int index = StdString::stringToInt(s,0);
            if(index>=0 && index<_data->vectorShape->pathPoints().size()){
               mCurrentData->exitPathPointsIndexes.push_back(index);
            }
        }
    }


    //----

    //bool positionOnPathwayFound = mVelocityGenerator.init(*mCurrentData, mDirection, _actorPosM + mCurrentData->actorPathwayContactOffsetM);
    //assert(positionOnPathwayFound);     // should be always true as we ensured the actor accessed pathway (via 'isPathwayAccessed')

    // mVelocityGenerator was already initialized by condition 'isPathwayAccessed'


    mStateEllapsedTimeS = 0.0f;
    mState = PathMovementState::STANDING;

    //---
    resetAnimationPlayer();

    if(mDirection==Direction::FORWARD){
        mActiveAnimationInstance = mCurrentData->aniStandingForward;

    }else if(mDirection==Direction::BACKWARD){
        mActiveAnimationInstance = mCurrentData->aniStandingBackward;
    }

    updateAnimationPlayer();


    //---
    mPrestarted = true;

    return true;

    // SET ACTIVE ANIMATION IF AVAILABLE

}


bool PathMovementEngine::init(Entity *_entity)
{

    mParentEntity = _entity;

    // store all signals for their preUpdate and postUpdate
    mSignals.push_back(&mSigState);

    for(PathMovementData& d : mPathMovementDatas){
        if(d.moveForward.mSignal == &d.sigMoveForwardObj){
            mSignals.push_back(&d.sigMoveForwardObj);
        }
        if(d.moveBackward.mSignal == &d.sigMoveBackwardObj){
            mSignals.push_back(&d.sigMoveBackwardObj);
        }
        mSignals.push_back(&d.sigDisabled);
    }

    return true;
}


bool PathMovementEngine::start(MovementEngineData *_data)
{

    if(mPrestarted == false){
        return false;
    }

    CenteringStyle centeringStyle = mCurrentData->centeringStyle;

    if(centeringStyle==CenteringStyle::SNAP_TO_POSITION){

        b2Vec2 pathStartPosM = currentPositionM();
        b2Vec2 actorPosM = mParentEntity->body()->B2Body()->GetPosition();

        if(mCurrentData->pathwayOrientation==PathwayOrientation::VERTICAL){
            actorPosM.x = pathStartPosM.x;

        }else if(mCurrentData->pathwayOrientation==PathwayOrientation::HORIZONTAL){
            actorPosM.y = pathStartPosM.y;
        }

        mParentEntity->body()->B2Body()->SetTransform(actorPosM, 0.0f);


    }else if(centeringStyle==CenteringStyle::MOVE_TO_POSITION){

        // ? need to decide how to handle centering from other engine
        assert(false);
        return false;

    }


    return true;
}


b2Vec2 PathMovementEngine::update(EngineUpdateParameters &eup)
{

    mStateEllapsedTimeS += eup.timeStep;
    mVelocity.SetZero();

    mInputDirection = Direction::NONE;
    if(mCurrentData->moveForward.active()){
        mInputDirection = Direction::FORWARD;

    }else if(mCurrentData->moveBackward.active()){
        mInputDirection = Direction::BACKWARD;
    }


    //--- blockedDirections are set for cardinal directions; the top end of the vertical pathway can be blocked so we also set blocked direction in corresponding pathway direction
    if(mCurrentData->pathwayOrientation==PathwayOrientation::VERTICAL){

        if(eup.blockedDirections & static_cast<int>(Direction::UP)){
             if(mCurrentData->verticalPathwayDirection==VerticalPathwayDirection::UP && mDirection==Direction::FORWARD){
                 eup.blockedDirections |= static_cast<int>(Direction::FORWARD);

             }else if(mCurrentData->verticalPathwayDirection==VerticalPathwayDirection::DOWN && mDirection==Direction::BACKWARD){
                 eup.blockedDirections |= static_cast<int>(Direction::BACKWARD);
             }
        }

    }



    //---
    if(mState==PathMovementState::STANDING || mState==PathMovementState::IDLE){
        update_stateSTANDING(eup);


    }else if(mState==PathMovementState::TURNING_AROUND){
        update_stateTURNING_AROUND(eup);


    }else if(mState==PathMovementState::MOVING){
        update_stateMOVING(eup);

    }


    //---
    mDirectionSignal._setOnNextFrame(static_cast<int>(mDirection));

    //---
    updateAnimationPlayer();


    return mVelocity;

}


void PathMovementEngine::update_stateSTANDING(EngineUpdateParameters &eup)
{


    if(mInputDirection == mDirection){

        //ObtainPoints();
        //if(mPoints.p2Exists){
            //mVelocityGenerator.initMovement(*mCurrentData->cfg, mPoints.p1, mPoints.p2, false);

        if((static_cast<int>(mDirection) & eup.blockedDirections) == 0){

            if(mVelocityGenerator.initMovement(*mCurrentData, mDirection, eup.actorPosM + mCurrentData->actorPathwayContactOffsetM)){
                setState(PathMovementState::MOVING);

                mStateEllapsedTimeS = 0.0f;

                if(mDirection==Direction::FORWARD){
                    mActiveAnimationInstance = mCurrentData->aniMovingForward;

                }else if(mDirection==Direction::BACKWARD){
                    mActiveAnimationInstance = mCurrentData->aniMovingBackward;

                }
            }
        }

    }else if(mInputDirection != Direction::NONE){

        mDirection = mInputDirection;
        if(mDirection==Direction::FORWARD){
            if(mCurrentData->aniTurningToForward){
                 mActiveAnimationInstance = mCurrentData->aniTurningToForward;
                 setState(PathMovementState::TURNING_AROUND);

            }else{
                mActiveAnimationInstance = mCurrentData->aniStandingForward;
            }


        }else if(mDirection==Direction::BACKWARD){
             if(mCurrentData->aniTurningToBackward){
                 mActiveAnimationInstance = mCurrentData->aniTurningToBackward;
                 setState(PathMovementState::TURNING_AROUND);

             }else{
                 mActiveAnimationInstance = mCurrentData->aniStandingBackward;
             }
        }
        mStateEllapsedTimeS = 0.0f;


    }else{

        if(mState==PathMovementState::STANDING){
            if(mCurrentData->aniIdleForward && mCurrentData->aniIdleBackward){
                if(mStateEllapsedTimeS > mCurrentData->cfg->idleAnimationDelay){
                    if(mDirection==Direction::FORWARD){
                        mActiveAnimationInstance = mCurrentData->aniIdleForward;

                    }else if(mDirection==Direction::BACKWARD){
                        mActiveAnimationInstance = mCurrentData->aniIdleBackward;

                    }
                    setState(PathMovementState::IDLE);
                    mStateEllapsedTimeS = 0.0f;
                }
            }

        }else if(mState==PathMovementState::IDLE){
            if(mAnimationPlayer && mAnimationPlayer->GetState()==AnimationPlayerState::STALLED){
                if(mDirection==Direction::FORWARD){
                    mActiveAnimationInstance = mCurrentData->aniStandingForward;

                }else if(mDirection==Direction::BACKWARD){
                    mActiveAnimationInstance = mCurrentData->aniStandingBackward;
                }
                setState(PathMovementState::STANDING);
                mStateEllapsedTimeS = 0.0f;
            }
        }
    }


}


void PathMovementEngine::update_stateMOVING(EngineUpdateParameters &eup)
{

    //bool finishFullSpeed = (inputDirection==mDirection && mPoints.pNextExists);
    //bool finishFullSpeed = (inputDirection==mDirection && mVelocityGenerator.segment().index3 != -1);

    mVelocity = mVelocityGenerator.updateVelocity(eup.timeStep, mInputDirection, eup.blockedDirections, eup.actorPosM + mCurrentData->actorPathwayContactOffsetM);

    //b2Vec2 p = mPoints.p1 + mVelocityGenerator.passedDistance()*mVelocityGenerator.unitVelocity();

    if(mVelocityGenerator.speed()==0.0f){
        setState(PathMovementState::STANDING);
        mStateEllapsedTimeS = 0.0f;

        if(mDirection==Direction::FORWARD){
            mActiveAnimationInstance = mCurrentData->aniStandingForward;

        }else if(mDirection==Direction::BACKWARD){
            mActiveAnimationInstance = mCurrentData->aniStandingBackward;
        }
    }

}


void PathMovementEngine::update_stateTURNING_AROUND(EngineUpdateParameters &eup)
{

    if(mAnimationPlayer && mAnimationPlayer->GetState()==AnimationPlayerState::STALLED){
        setState(PathMovementState::STANDING);
        mStateEllapsedTimeS = 0.0f;

        if(mDirection==Direction::FORWARD){
            mActiveAnimationInstance = mCurrentData->aniStandingForward;

        }else if(mDirection==Direction::BACKWARD){
            mActiveAnimationInstance = mCurrentData->aniStandingBackward;
        }
    }
}


void PathMovementEngine::stop()
{
    mVelocityGenerator.stop();
}


bool PathMovementEngine::isPathwayFinished(bool _activeGroundMoveCommand)
{

    assert(mCurrentData->pathway);

    //EPathway * pathwayNext = mCurrentData->pathway->connectedPathwayNext();
    //EPathway * pathwayPrev = mCurrentData->pathway->connectedPathwayPrev();


    //int exitPointIndex = mVelocityGenerator.currentIndex();


    // automatic exit at the path start and end
    if(mDirection==Direction::FORWARD){
        if(mVelocityGenerator.isPathwayEnd()){
            return true;
        }

    }else if(mDirection==Direction::BACKWARD){
        if(mVelocityGenerator.isPathwayStart()){
            return true;
        }

        //exitPointIndex--;
        //if(exitPointIndex<0){
        //    assert(false);
        //    exitPointIndex = 0;
        //}
    }


    //---
    if(_activeGroundMoveCommand == false){
        return false;
    }

    //---
    //if(StdVector::contains(mCurrentData->exitPathPointsIndexes, exitPointIndex)==false){
    //    return false;
    //}


    //PathPoint &currentIndexPoint = mCurrentData->vectorShape->pathPoints().at(exitPointIndex);
    Vec2f currentPosP = mVelocityGenerator.currentPositionP();
    float maxFromGroundDistance = gWorldInfo.minGeometryBlockPixels() * 0.5;

    if(mCurrentData->pathwayOrientation==PathwayOrientation::VERTICAL){


        //if(currentPosP.y < mCurrentData->vectorShape->pathPoints().at(exitPointIndex).y  + maxFromGroundDistance ){
        //    print("Actor within maxGroundDistance!");
        //    return true;
        //}

        for(int index : mCurrentData->exitPathPointsIndexes){
            int yExit = mCurrentData->vectorShape->pathPoints().at(index).y;
            if(currentPosP.y > yExit - maxFromGroundDistance  &&  currentPosP.y < yExit + maxFromGroundDistance) {
                print("Actor within maxGroundDistance!");
                return true;
            }
        }



        /*
        if(mCurrentData->vectorShape->pathPoints().front().y < mCurrentData->vectorShape->pathPoints().back().y){

            if(currentPosP.y < mCurrentData->vectorShape->pathPoints().front().y  + maxFromGroundDistance ){
                print("Actor within maxGroundDistance!");
                return true;
            }

        }else{
            if(currentPosP.y < mCurrentData->vectorShape->pathPoints().back().y + maxFromGroundDistance){
                print("Actor within maxGroundDistance!");
                return true;
            }

        }
        */

    }



    return false;
}


/*
bool PathMovementEngine::isPathwayAccessed(PathMovementData *_data, b2Vec2 _actorPosM)
{


    //Vec2f actorPosP = gWorldInfo.metersToPixels<Vec2f>(_actorPosM);
    //Vec2f actorPos(_actorPosM.x, _actorPosM.y - gWorldInfo.pixelsToMeters(37.5));

    mCurrentData = _data;

    b2Vec2 actorContactPosM = _actorPosM + _data->actorPathwayContactOffsetM;

    //---
    if(_data->moveForward->active()){

        mDirection = Direction::FORWARD;

        bool positionOnPathwayFound = mVelocityGenerator.init(*_data, Direction::FORWARD, actorContactPosM);
        if(positionOnPathwayFound==false){
            return false;
        }
        if(mVelocityGenerator.isPathwayStart()){

            if(DistanceTwoPoints(actorContactPosM, mVelocityGenerator.currentPosition()) > _data->centeringMarginM){
                print("Actor not within centering margin!");
                return false;
            }

            return true;
        }

    }else if(_data->moveBackward->active()){

        mDirection = Direction::BACKWARD;

        bool positionOnPathwayFound = mVelocityGenerator.init(*_data, Direction::BACKWARD, actorContactPosM);
        if(positionOnPathwayFound==false){
            return false;
        }
        if(mVelocityGenerator.isPathwayEnd()){
            if(DistanceTwoPoints(actorContactPosM, mVelocityGenerator.currentPosition()) > _data->centeringMarginM){
                return false;
            }

            return true;
        }
    }

    return false;

}
*/



void PathMovementEngine::obtainSignal_signalQuery(SignalQuery &_signalQuery, const std::string &_dataName, const std::string &_signalName, const std::string &_signalValue, bool _setErrorMessage)
{

    PathMovementData *engineData = nullptr;

    if(_dataName.empty()==false){
        for(PathMovementData &d : mPathMovementDatas){
            if(d.cfg->name == _dataName){

                engineData = &d;

                if(_signalName=="MOVE_FORWARD"){
                    _signalQuery.mSignal = d.moveForward.mSignal;

                }else if(_signalName=="MOVE_BACKWARD"){
                    _signalQuery.mSignal = d.moveBackward.mSignal;
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
        _signalQuery.mIntValue =  static_cast<int>(GetPathMovementStateFromString(_signalValue));

        if(_signalQuery.mIntValue == static_cast<int>(PathMovementState::UNKNOWN)){
            dbgSystem.addMessage("Unknown signal value '" + _signalValue + " ' !");
        }
        return;
    }


    //---- CUSTOM ENTITY SIGNALS (Created signals will be stored in controller states !)

    if(_signalName=="PATWAY_ACCESSED"){
        assert(engineData); assert(mParentEntity);
        _signalQuery.mSignal = new PathwayAccessedSignal(mParentEntity, engineData, this);

    }else if(_signalName=="PATHWAY_LEFT"){

        assert(engineData); assert(mParentEntity);

        //target engine name
        MovementEngineData *targetEngineData = mParentEntity->getMovementEngineData(_signalValue);
        if(targetEngineData==nullptr){
            dbgSystem.addMessage("Movement engine '" + _signalValue + " ' not found!");
            return;
        }
        _signalQuery.mSignal = new PathwayLeftSignal(mParentEntity, engineData, this, targetEngineData);
    }


    if(_setErrorMessage){
        dbgSystem.addMessage("Get signal '" + _signalName + "' error! The signal is unknown!");
    }

}


void PathMovementEngine::obtainSignal_signalSetter(SignalSetter &_signalSetter, const std::string &_dataName, const std::string &_signalName, const std::string &_signalValue, bool _setErrorMessage )
{

    PathMovementData *data = nullptr;

    if(_dataName.empty()==false){
        for(PathMovementData &d : mPathMovementDatas){
            if(d.cfg->name == _dataName){

                data = &d;

                //if(_setErrorMessage){
                //    dbgSystem.addMessage("Get signal '" + _signalName + "' error! The signal is unknown or not available for setting it!");
                //}
                //return;
            }
        }

        //if(_setErrorMessage){
        //    dbgSystem.addMessage("Get signal '" + _signalName + "' error!  The jump engine data with name '" + _dataName + "' not found !");
        //}
        //return;
        if(_signalSetter.mSignal){
            return;
        }

    }

    //---- CUSTOM ENTITY SIGNALS (Signals will be stored in controller states !)
    mFactory->obtainCustomSignal_signalSetter(_signalSetter, _signalName, _signalValue);
    if(_signalSetter.mSignal){
        return;
    }


    if(_setErrorMessage){
        dbgSystem.addMessage("Get signal '" + _signalName + "' error! The signal is unknown or not available for setting it!");
    }

}





MovementEngineData* PathMovementEngine::getMovementEngineData(const std::string &_name, bool _setErrorMessage)
{

    for(PathMovementData &d : mPathMovementDatas){
        if(d.cfg->name == _name){
            return &d;
        }
    }

    if(_setErrorMessage){
        dbgSystem.addMessage("A data instance of 'pathMovementCfg' object with name '" + _name + "' not found!");
    }

    return nullptr;

}


/*
PathMovementData* PathMovementEngine::getPathMovementData(const std::string &_name, bool _setErrorMessage)
{

    for(PathMovementData &d : mPathMovementDatas){
        if(d.cfg->name == _name){
            return &d;
        }
    }

    if(_setErrorMessage){
        dbgSystem.addMessage("A data instance of 'pathMovementCfg' object with name '" + _name + "' not found!");
    }

    return nullptr;
}
*/


//=============================================================================================================


PathMovementCfg* PathMovementFactory::createMovementEngineCfg(const std::string &_name)
{
    return new PathMovementCfg(this, _name);
}


PathMovementEngine* PathMovementFactory::createMovementEngine()
{
    return new PathMovementEngine(this);
}



void PathMovementFactory::obtainCustomSignal_signalQuery(SignalQuery &_signalQuery, const std::string &_signalName, const std::string &_signalValue)
{

    //if(_signalName=="PATWAY_ACCESSED"){
     //   _signalQuery.mSignal = new PathwayAccessedSignal();
    //}

}


}
