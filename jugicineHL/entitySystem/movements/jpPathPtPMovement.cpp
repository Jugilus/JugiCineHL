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
#include "jpObjectParser.h"

#include "jpEntity.h"
#include "jpEntityUtilities.h"
#include "jpSourceEntity.h"
#include "jpB2World.h"
#include "jpPathPtPMovement.h"



namespace jugimap{





PathPTPMovementState GetPathPTPMovementStateFromString(const std::string &state)
{


    if(state=="STANDING"){
        return PathPTPMovementState::STANDING;

    }else if(state=="IDLE"){
        return PathPTPMovementState::IDLE;

    }else if(state=="TURNING_AROUND"){
        return PathPTPMovementState::TURNING_AROUND;

    }else if(state=="MOVING"){
        return PathPTPMovementState::MOVING;
    }

    return PathPTPMovementState::UNKNOWN;

}


PathPTPRotationState GetPathPTPRotationStateFromString(const std::string &state)
{

    if(state=="STANDING_STILL"){
        return PathPTPRotationState::STANDING_STILL;

    }else if(state=="MOVING"){
        return PathPTPRotationState::MOVING;
    }

    return PathPTPRotationState::UNKNOWN;

}


//-----------------------------------------------------------------------------------------



bool PathPTPMovementCfg::initCfg(const pugi::xml_node &_node)
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

                }else if(attributeName=="angularSpeed"){
                    maxAngularSpeed = a.as_float(maxAngularSpeed);

                }else if(attributeName=="angularAccDistance"){
                     angularAccDistance = a.as_float(angularAccDistance);

                }else{

                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                    return false;
                }
            }


        }else if(nodeName=="triggers"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="moveForward"){
                    moveForward = a.as_string("");

                }else if(attributeName=="moveBackward"){
                    moveBackward = a.as_string("");

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

//===================================================================================


PathPTPMovementData::~PathPTPMovementData()
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


bool PathPTPMovementData::initConnections(PlayedScene *_scene, Entity *_actor)
{

    dbgSystem.addMessage("Init connections for the data object of '" + cfg->name + "' ...");


    // triggers
    if(cfg->moveForward.empty()){      // computer controlled entity !
        moveForward.init(&sigMoveForwardObj, SignalQuery::Style::ACTIVE);

    }else{
        //ObtainSignalFromPath(cfg->moveForward, _scene, _actor, nullptr, moveForward);
        app->signalParserManager()->parseSignalAccessor(_scene, cfg->moveForward, moveForward, _actor);
        if(moveForward.isValid()==false){
            return false;
        }

    }

    if(cfg->moveBackward.empty()){      // computer controlled entity !
        moveBackward.init(&sigMoveBackwardObj, SignalQuery::Style::ACTIVE);

    }else{
        //ObtainSignalFromPath(cfg->moveBackward, _scene, _actor, nullptr, moveBackward);
        app->signalParserManager()->parseSignalAccessor(_scene, cfg->moveBackward, moveBackward, _actor);
        if(moveBackward.isValid()==false){
            return false;
        }
        if(moveForward.signal()->type()!=SignalType::BOOL){
            dbgSystem.addMessage("'move forward' trigger is not a boolean! ");
            return false;
        }
    }



    // triggers
    if(cfg->rotateCW.empty()){      // computer controlled entity !
        rotateCW.init(&sigRotateCWObj, SignalQuery::Style::ACTIVE);

    }else{
        //ObtainSignalFromPath(cfg->rotateCW, _scene, _actor, nullptr, rotateCW);
        app->signalParserManager()->parseSignalAccessor(_scene, cfg->rotateCW, rotateCW, _actor);
        if(rotateCW.isValid()==false){
            return false;
        }

    }

    if(cfg->rotateCCW.empty()){      // computer controlled entity !
        rotateCCW.init(&sigRotateCCWObj, SignalQuery::Style::ACTIVE);

    }else{
       // ObtainSignalFromPath(cfg->rotateCCW, _scene, _actor, nullptr, rotateCCW);
        app->signalParserManager()->parseSignalAccessor(_scene, cfg->rotateCCW, rotateCCW, _actor);
        if(rotateCCW.isValid()==false){
            return false;
        }
    }


    //--- find a vector shape which has the actor placed at its shape point ans use it as path
    Sprite *actorSprite = _actor->sprite();
    Vec2f sprPos = actorSprite->position();
    float distMin = 999999.0f;
    int pathPointIndex = -1;
    for(SceneMap* sm : _scene->sceneMaps()){
        if(sm->info.mapType != MapType::WORLD) continue;

        VectorShape *vs = ObtainVectorShapeWithAPathPointPosition(sm->map(), sprPos, distMin, pathPointIndex);
        if(vs && vs->kind() == ShapeKind::ELLIPSE){
            DummyFunction();
        }
        if(vs){
            vectorShape = vs;
            currentPathPointIndex = pathPointIndex;
        }
    }
    if(distMin!=0.0f){
        dbgSystem.addMessage("Path for an entity of type '" + _actor->sourceEntity()->name() + "' not found! The entity sprite must be positioned at a path point!");
        return false;

    }



    continousCircularMovement = vectorShape->parameters().boolValue("eContinousCircularMovement", false);
    duration = vectorShape->parameters().floatValue("eDuration", 0.0f);

    endShapePointIndexOffset = _actor->sprite()->parameters().intValue("ePathSegmentEndPointOffset", -1);
    if(endShapePointIndexOffset != -1){
        // if endShapePointOffset is defined we obtain startingShapePointIndex

        distMin = 999999.0f;
        startingShapePointIndex = -1;

        for(unsigned int i=0; i<vectorShape->shapePoints().size(); i++){
            float dist = DistanceTwoPoints(sprPos, vectorShape->shapePoints()[i]);
            if(dist<distMin){
                distMin = dist;
                startingShapePointIndex = i;
            }
        }

        assert(startingShapePointIndex != -1);

    }

    //---- set angular velocity
    if(cfg->maxAngularSpeed > 0.0f){

        //--- start and end angle are defined with sprite parameters
        std::string sParameters = actorSprite->parameters().value("eBehRotation");

        if(sParameters.empty()==false){

            std::vector<std::string> parameters = StdString::splitString(sParameters, ",");

            if(parameters.size() == 1){
                startRotationDirection = GetRotationDirectionFromString(parameters[0]);
                if(startRotationDirection==RotationDirection::NONE){
                    dbgSystem.addMessage("Wrong value '" + parameters[0] + "' in the 'eBehRotation' parameter!");
                    return false;
                }
                continousCircularRotation = true;

            }else if(parameters.size()==3){
                startAngle = StdString::stringToFloat(parameters[0], 0);
                endAngle = StdString::stringToFloat(parameters[1], 0);
                startRotationDirection = GetRotationDirectionFromString(parameters[2]);
                if(startRotationDirection==RotationDirection::NONE){
                    dbgSystem.addMessage("Wrong value '" + parameters[2] + "' in the 'eBehRotation' parameter!");
                    return false;
                }

            }else{

                dbgSystem.addMessage("Wrong value '" + sParameters + "' for the 'eBehRotation' parameter!");
                return false;
            }

        }
    }




    //---
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


    //---
    dbgSystem.removeLastMessage();
    return true;
}





//===================================================================================


void VGPathPTPMovement::init(PathPTPMovementData &data, Direction _direction)
{

    mActive = false;

    if(mPath!=data.vectorShape){
        mPath = data.vectorShape;
       // mStartIndex = -1;
        mCurrentIndex = data.currentPathPointIndex;

        mStartIndex = 0;
        mEndIndex = mPath->pathPoints().size()-1;
        if(data.endShapePointIndexOffset != -1){
            mStartIndex = mCurrentIndex;

            mStep = (_direction==Direction::FORWARD)?  1 : -1;
            int mEndShapePointIndex = data.startingShapePointIndex + data.endShapePointIndexOffset * mStep;
            Clamp(mEndShapePointIndex, 0, int(mPath->shapePoints().size())-1);
            ShapePoint &endShapePoint = mPath->shapePoints().at(mEndShapePointIndex);

            //--- obtain path point index for the endShapePoint
            float distMin = 999999.0f;
            for(unsigned int i=0; i<mPath->pathPoints().size(); i++){
                float dist = DistanceTwoPoints(endShapePoint, mPath->pathPoints()[i]);
                if(dist<distMin){
                    distMin = dist;
                    mEndIndex = i;
                }
            }

            if(distMin !=0.0f){
                DummyFunction();
            }

            mUsingPathSegment = true;

            mPathSegmentLength = 0.0f;
            int minIndex = std::min(mStartIndex, mEndIndex);
            int maxIndex = std::max(mStartIndex, mEndIndex);
            for( int i=minIndex+1; i<= maxIndex; i++){
                float dist = DistanceTwoPoints(mPath->pathPoints()[i], mPath->pathPoints()[i-1]);
                mPathSegmentLength += dist;
            }

        }


        mContinousCircularMovement = false;
        if(mUsingPathSegment == false){
            if(mPath->isClosed()){

                // if closed path has no sharp corners it has continous circular movement
                mContinousCircularMovement= true;
                for(int i=0; i<mPath->pathPoints().size(); i++){
                    if(mPath->pathPoints().at(i).smoothCorner==false){
                        mContinousCircularMovement = false;
                        break;
                    }
                }
                // if closed path has sharp corners, the continous circular movement is enabled by 'continousCircularMovement' parameter
                if(data.continousCircularMovement){
                    mContinousCircularMovement = true;
                }
            }
        }

        mActive = (mPath->pathLength() > 0.0f) ;

    }
}


void VGPathPTPMovement::initMovement(PathPTPMovementData &data, Direction _direction, bool _continueFullSpeed)
{

    // smooth closed path - special case with continous mvement around the shape
    mStep = (_direction==Direction::FORWARD)?  1 : -1;



    mSegment.index1 = mCurrentIndex;
    mSegment.index2 = mSegment.index3 = -1;
    mSegment.distance12 = mSegment.distance23 = 0.0f;

    //---
    if(mContinousCircularMovement){
        mSegment.index2 = mSegment.index3 = mCurrentIndex;
        mSegment.distance12 = mPath->pathLength();

    }else{

        mSegment.index2 = obtainNextIndex(mSegment.index1, mSegment.distance12);

        if(mSegment.index2 != -1){
            mCurrentPosP = mPath->pathPoints().at(mSegment.index1);
            mCurrentDistanceP = 0.0f;

            //--- obtain also the index after the next index as it may be useful in some situations.
            mSegment.index3 = obtainNextIndex(mSegment.index2, mSegment.distance23);
        }
    }


    if(mSegment.index2 != -1){

        float distanceM = gWorldInfo.pixelsToMeters(mSegment.distance12);

       if(_continueFullSpeed){
           //mSpeedGenerator.continueFullSpeed(distanceM);

           float overDistanceM = mSpeedGenerator.passedDistance() - mSpeedGenerator.distance();     // obtain how much did passed distance went over the distance
           mSpeedGenerator.continueFullSpeed(distanceM, overDistanceM);

       }else{
           float accDistance = (mContinousCircularMovement==false)? data.cfg->accDistance : 0.0f;
           float maxSpeed = data.cfg->maxSpeed;
           if(data.duration > 0.0f){
               float lengthP = (mUsingPathSegment==true)? mPathSegmentLength : mPath->pathLength();
               float lengthM = gWorldInfo.pixelsToMeters(lengthP);
               maxSpeed = lengthM/data.duration;
               accDistance = 0.0f;                  // This mode is used to sync with animations and speeds must be constants !
           }
           mSpeedGenerator.initMovement(maxSpeed, accDistance, distanceM);
       }
   }

}


b2Vec2 VGPathPTPMovement::updateVelocity(float _timeStep, bool _finishFullSpeed)
{


    float speed = 0.0f;


    if(mContinousCircularMovement){
        speed = mSpeedGenerator.updateSpeed(_timeStep, true);
        float passedDistanceM = mSpeedGenerator.passedDistance();
        float passedDistanceP = gWorldInfo.metersToPixels(mSpeedGenerator.passedDistance());

        if(mSpeedGenerator.state()==PointToPointSpeedGenerator::State::FINISHED_FULL_SPEED){
            /*
            float mOverDistanceP = passedDistanceP - segment().distance12;

            float distanceM = gWorldInfo.pixelsToMeters(mSegment.distance12 - mOverDistanceP);
            mSpeedGenerator.continueFullSpeed(distanceM, true);
            passedDistanceP = mOverDistanceP;

            mCurrentIndex = mSegment.index1;
            mCurrentDistanceP = 0.0f;
            */

            float mOverDistanceP = passedDistanceP - segment().distance12;

            float distanceM = gWorldInfo.pixelsToMeters(mSegment.distance12);
            float mOverDistanceM = passedDistanceM - distanceM;
            mSpeedGenerator.continueFullSpeed(distanceM, mOverDistanceM);

            passedDistanceP = mOverDistanceP;

            mCurrentIndex = mSegment.index1;
            mCurrentDistanceP = 0.0f;
            //mCurrentDistanceP = passedDistanceP;
        }

        obtainUnitVelocity(passedDistanceP);


    }else{

        speed = mSpeedGenerator.updateSpeed(_timeStep, _finishFullSpeed);
        float passedDistanceP = gWorldInfo.metersToPixels(mSpeedGenerator.passedDistance());

        obtainUnitVelocity(passedDistanceP);

        if(mSpeedGenerator.state()==PointToPointSpeedGenerator::State::FINISHED_FULL_SPEED ||
            mSpeedGenerator.state()==PointToPointSpeedGenerator::State::STANDING_STILL)
        {
            mCurrentIndex = mSegment.index2;
        }

    }


    //----
    //if(mPath->isClosed()){
    //    obtainUnitVelocity_Slow(passedDistanceP);

    //}else{

        //obtainUnitVelocity_OpenPath_Quick(passedDistanceP);
        //obtainUnitVelocity_Slow(passedDistanceP);            // also works
    //}





    mVelocity = speed * mUnitVelocity;

    /*
    float speedP = gWorldInfo.metersToPixels(speed);
    Vec2f velocityP(mUnitVelocity.x*speedP, mUnitVelocity.y*speedP);
    mDistPDbg += speedP * _timeStep;

    mCPviaSpeedP.x += _timeStep * velocityP.x;
    mCPviaSpeedP.y += _timeStep * velocityP.y;

    print("*** cpViaSpped x=" + std::to_string(mCPviaSpeedP.x) + " y=" + std::to_string(mCPviaSpeedP.y) +
          " mCPviaDistance x=" + std::to_string(mCPviaDistanceP.x) + " y=" + std::to_string(mCPviaDistanceP.y));

    */


    return mVelocity;

}


void VGPathPTPMovement::obtainUnitVelocity(float passedDistanceP)
{

    std::vector<PathPoint> & pathPoints = mPath->pathPoints();

    float dist = mCurrentDistanceP;
    int nextIndex = mCurrentIndex;

    while(true){

        nextIndex += mStep;

        /*
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
        */

        if(nextIndex > mEndIndex){
            if(mUsingPathSegment==false && mPath->isClosed()){
                //index = 0;
                nextIndex = 1;  // the last point of the closed shape is at the same position as the first point so we set to the second point
            }else{
                nextIndex = mEndIndex;
                break;
            }

        }else if(nextIndex < mStartIndex){
            if(mUsingPathSegment==false && mPath->isClosed()){
                //index = pathPoints.size()-1;
                nextIndex = pathPoints.size()-2;    // the last point of the closed shape is at the same position as the first point

            }else{
                nextIndex = mStartIndex;
                break;
            }
        }


        PathPoint p1 = pathPoints[mCurrentIndex];
        PathPoint p2 = pathPoints[nextIndex];
        float d = DistanceTwoPoints(p1, p2);
        dist += d;

        if(nextIndex == mSegment.index2){
            break;
        }
        if(passedDistanceP < dist){
            break;
        }else{
            mCurrentIndex = nextIndex;
            mCurrentDistanceP += d;
        }
    }


    //---
    if(mCurrentIndex!=nextIndex){
        PathPoint p1 = pathPoints[mCurrentIndex];
        PathPoint p2 = pathPoints[nextIndex];
        //float d = DistanceTwoPoints(p1, p2);
        //mUnitVelocity.Set((p1.x-p2.x)/d, (p1.y-p2.y)/d);

        Vec2f newCurrentPosP = MakeVec2fAtDistanceFromP1(p2, p1, dist - passedDistanceP);

        float d = DistanceTwoPoints(newCurrentPosP, mCurrentPosP);
        mUnitVelocity.Set((newCurrentPosP.x-mCurrentPosP.x)/d, (newCurrentPosP.y-mCurrentPosP.y)/d);

        mCurrentPosP = newCurrentPosP;
        mCurrentPos = gWorldInfo.pixelsToMeters(mCurrentPosP);

    }else{
        mUnitVelocity.SetZero();
    }

}


/*
void VGPointToPointPathMovement::obtainUnitVelocity_Slow(float passedDistanceP)
{


    std::vector<PathPoint> & pathPoints = mPath->pathPoints();

    float dist = 0;
    int index = mSegment.index1;
    int prevIndex = index;

    while(true){

        prevIndex = index;
        index += mStep;

        if(index >= int(pathPoints.size())){
            if(mPath->isClosed()){
                //index = 0;
                index = 1;  // the last point of the closed shape is at the same position as the first point so we set to the second point
            }else{
                index = pathPoints.size()-1;
                break;
            }

        }else if(index < 0){
            if(mPath->isClosed()){
                //index = pathPoints.size()-1;
                index = pathPoints.size()-2;    // the last point of the closed shape is at the same position as the first point

            }else{
                index = 0;
                break;
            }
        }

        PathPoint p1 = pathPoints[index];
        PathPoint p2 = pathPoints[prevIndex];
        dist += DistanceTwoPoints(p1, p2);

        if(dist>=passedDistanceP){
            break;
        }
        if(index == mSegment.index2){
            break;
        }
    }


    //---
    if(index!=prevIndex){
        PathPoint p1 = pathPoints[index];
        PathPoint p2 = pathPoints[prevIndex];
        //float d = DistanceTwoPoints(p1, p2);
        //mUnitVelocity.Set((p1.x-p2.x)/d, (p1.y-p2.y)/d);

        Vec2f newCurrentPosP = MakeVec2fAtDistanceFromP1(p1, p2, dist - passedDistanceP);

        float d = DistanceTwoPoints(newCurrentPosP, mCurrentPosP);
        mUnitVelocity.Set((newCurrentPosP.x-mCurrentPosP.x)/d, (newCurrentPosP.y-mCurrentPosP.y)/d);

        mCurrentPosP = newCurrentPosP;
        mCurrentPos = gWorldInfo.pixelsToMeters(mCurrentPosP);

    }else{
        mUnitVelocity.SetZero();
    }

}



void VGPointToPointPathMovement::obtainUnitVelocity_OpenPath_Quick(float passedDistanceP)
{


    //float passedDistanceM = mSpeedGenerator.passedDistance();
    //float passedDistanceP = gWorldInfo.metersToPixels(passedDistanceM);

    std::vector<PathPoint> & pathPoints = mPath->pathPoints();


    float dist = 0;
    int index = mSegment.index1;
    int prevIndex = index;

    int indexP1 = mSegment.index1;
    int indexP2 = mSegment.index2;

    if(indexP2 > indexP1){

        float distStart = pathPoints[indexP1].distance;

        if(passedDistanceP > pathPoints[indexP2].distance - distStart){
            indexP1 = indexP2;
        }


        while(indexP2 - indexP1 > 1){

            int indexCheck = (indexP1+indexP2)/2;
            dist = pathPoints[indexCheck].distance - distStart;

            if(dist > passedDistanceP){
                indexP2 = indexCheck;

            }else if(dist< passedDistanceP){
                indexP1 = indexCheck;
            }
        }

        dist = pathPoints[indexP2].distance - distStart;

    }else{

        float distStart = pathPoints[indexP1].distance;

        if(passedDistanceP > distStart - pathPoints[indexP2].distance){
            indexP1 = indexP2;
        }

        while(indexP1 - indexP2 > 1){

            int indexCheck = (indexP1+indexP2)/2;
            dist = distStart - pathPoints[indexCheck].distance;

            if(dist > passedDistanceP){
                indexP2 = indexCheck;

            }else if(dist< passedDistanceP){
                indexP1 = indexCheck;
            }
        }

        dist = distStart - pathPoints[indexP2].distance;

    }


    //---
    if(indexP1!=indexP2){
        PathPoint p1 = pathPoints[indexP1];
        PathPoint p2 = pathPoints[indexP2];
        Vec2f newCurrentPosP = MakeVec2fAtDistanceFromP1(p2, p1, dist - passedDistanceP);       // ?

        float d = DistanceTwoPoints(newCurrentPosP, mCurrentPosP);
        mUnitVelocity.Set((newCurrentPosP.x-mCurrentPosP.x)/d, (newCurrentPosP.y-mCurrentPosP.y)/d);

        mCurrentPosP = newCurrentPosP;
        mCurrentPos = gWorldInfo.pixelsToMeters(mCurrentPosP);

    }else{
        mUnitVelocity.SetZero();        // ?
    }

}

*/


int VGPathPTPMovement::obtainNextIndex(int _startIndex, float &_distance)
{

    std::vector<PathPoint> & pathPoints = mPath->pathPoints();

    if(mPath->name()=="test"){
        DummyFunction();
    }


    int index = _startIndex;

    while(true){

        int prevIndex = index;
        index += mStep;

        /*
        if(index >= int(pathPoints.size())){
            if(mPath->isClosed()){
                //index = 0;
                index = 1;  // the last point of the closed shape is at the same position as the first point
            }else{
                index = pathPoints.size()-1;
                break;
            }

        }else if(index < 0){
            if(mPath->isClosed()){
                //index = pathPoints.size()-1;
                index = pathPoints.size()-2;    // the last point of the closed shape is at the same position as the first point
            }else{
                index = 0;
                break;
            }
        }
        */

        if(index > mEndIndex){
            if(mUsingPathSegment==false && mPath->isClosed()){
                //index = 0;
                index = 1;  // the last point of the closed shape is at the same position as the first point
            }else{
                index = mEndIndex;
                break;
            }

        }else if(index < mStartIndex){
            if(mUsingPathSegment==false && mPath->isClosed()){
                //index = pathPoints.size()-1;
                index = pathPoints.size()-2;    // the last point of the closed shape is at the same position as the first point
            }else{
                index = mStartIndex;
                break;
            }
        }


        //b2Vec2 p1 = gWorldInfo.pixelsToMeters(pathPoints[index]);
        //b2Vec2 p2 = gWorldInfo.pixelsToMeters(pathPoints[prevIndex]);
        PathPoint p1 = pathPoints[index];
        PathPoint p2 = pathPoints[prevIndex];
        _distance += DistanceTwoPoints(p1, p2);

        //_distance += DistanceTwoPoints(pathPoints[index], pathPoints[prevIndex]);

        if(pathPoints[index].smoothCorner==false){      // charp corner points separate moving segments
            break;
        }

    }

    if(index==_startIndex){     // no next index found
        index = -1;
    }


    return index;

}


b2Vec2 VGPathPTPMovement::currentPoint()
{

    if(mPath==nullptr){
        return b2Vec2();
    }

    return gWorldInfo.pixelsToMeters(mPath->pathPoints().at(mCurrentIndex));

}


void VGPathPTPMovement::stop()
{
    mSpeedGenerator.stop();
    mVelocity.SetZero();

}



//============================================================================================


PathPTPMovementEngine::PathPTPMovementEngine(MovementEngineFactory *_factory) : MovementEngine(_factory)
{
    mDirection = Direction::FORWARD;
}


void PathPTPMovementEngine::createDataObjects(std::vector<MovementEngineCfg *> &_cfgs)
{

    mPathMovementDatas.resize(_cfgs.size());
    for(unsigned int i=0; i<_cfgs.size(); i++){
        assert(_cfgs[i]->factory == mFactory);
        mPathMovementDatas.at(i).factory = mFactory;
        mPathMovementDatas.at(i).cfg = static_cast<PathPTPMovementCfg*>(_cfgs[i]);
    }

}


bool PathPTPMovementEngine::initDataObjectsConnections(PlayedScene *_scene, Entity *_actor)
{

    for(PathPTPMovementData& d : mPathMovementDatas){
        if(d.initConnections(_scene, _actor)==false){
            return false;
        }
    }

    return true;
}


void PathPTPMovementEngine::collectSignalsForLUT(SignalStorage &_storage)
{

    for(PathPTPMovementData& d : mPathMovementDatas){

    }

}


/*
void PathPointToPointMovementEngine::start(PathMovementData *_data, Direction _direction, Entity *_entity)
{

    mCurrentData = _data;
    mDirection = _direction;
    mStateEllapsedTimeS = 0.0f;
    mVelocityGenerator.init(*mCurrentData, _direction);
    mAngularVelocityGenerator.init(_data->startAngle, _data->endAngle, mCurrentData->continousCircularRotation, mCurrentData->startRotationDirection);

    setState(PathMovementState::STANDING;
    if(mDirection==Direction::FORWARD){
        mActiveAnimationInstance = mCurrentData->aniStandingForward;

    }else if(mDirection==Direction::BACKWARD){
        mActiveAnimationInstance = mCurrentData->aniStandingBackward;
    }

}
*/


bool PathPTPMovementEngine::init(Entity *_entity)
{
    mParentEntity = _entity;

    // store all signals for their preUpdate and postUpdate
    mSignals.push_back(&mSigState);

    for(PathPTPMovementData& d : mPathMovementDatas){
        if(d.moveForward.signal() == &d.sigMoveForwardObj){
            mSignals.push_back(&d.sigMoveForwardObj);
        }
        if(d.moveBackward.signal() == &d.sigMoveBackwardObj){
            mSignals.push_back(&d.sigMoveBackwardObj);
        }
        mSignals.push_back(&d.sigDisabled);

        if(d.rotateCW.signal() == &d.sigRotateCWObj){
            mSignals.push_back(&d.sigRotateCWObj);
        }
        if(d.rotateCCW.signal() == &d.sigRotateCCWObj){
            mSignals.push_back(&d.sigRotateCCWObj);
        }
        mSignals.push_back(&d.sigRotateDisabled);

    }

    return true;
}


bool PathPTPMovementEngine::start(MovementEngineData *_data)
{
    assert(mFactory==_data->factory);


    if(mParentEntity->statusFlagsRef() & EntityStatusFlags::FORCED_ENTITY_TRANSFORMATION){
        return false;
    }

    mCurrentData = static_cast<PathPTPMovementData*>(_data);

    if(mCurrentData->sigDisabled.active(true)){
        return false;
    }

    Direction entityDirection = mParentEntity->direction();

    if(entityDirection!=Direction::FORWARD && entityDirection!=Direction::BACKWARD){
        entityDirection = mDirection;
    }

    mDirection = entityDirection;
    mStateEllapsedTimeS = 0.0f;
    mVelocityGenerator.init(*mCurrentData, mDirection);
    mAngularVelocityGenerator.init(mCurrentData->startAngle, mCurrentData->endAngle, mCurrentData->continousCircularRotation, mCurrentData->startRotationDirection);

    setState(PathPTPMovementState::STANDING);
    if(mDirection==Direction::FORWARD){
        mActiveAnimationInstance = mCurrentData->aniStandingForward;

    }else if(mDirection==Direction::BACKWARD){
        mActiveAnimationInstance = mCurrentData->aniStandingBackward;
    }

    return true;

}


b2Vec2 PathPTPMovementEngine::update(EngineUpdateParameters &eup)
{

    mVelocity.SetZero();
    mFixedPositionSet = false;
    mStateEllapsedTimeS += eup.timeStep;


    if(mVelocityGenerator.isActive()){
        updateTranslation(eup);
    }

    if(mAngularVelocityGenerator.isActive()){
        updateRotation(eup);
    }


    mDirectionSignal.setValue_onNextFrame(static_cast<int>(mDirection));

    return mVelocity;

}


void PathPTPMovementEngine::updateTranslation(EngineUpdateParameters &eup)
{


    mInputDirection = Direction::NONE;
    if(mCurrentData->moveForward.active()){
        mInputDirection = Direction::FORWARD;

    }else if(mCurrentData->moveBackward.active()){
        mInputDirection = Direction::BACKWARD;
    }


    //---
    if(mState==PathPTPMovementState::STANDING || mState==PathPTPMovementState::IDLE){
        update_stateSTANDING(eup);

    }else if(mState==PathPTPMovementState::TURNING_AROUND){
        update_stateTURNING_AROUND(eup);

    }else if(mState==PathPTPMovementState::MOVING){
        update_stateMOVING(eup);

    }

}


void PathPTPMovementEngine::update_stateSTANDING(EngineUpdateParameters &eup)
{

    if(mDirection == mInputDirection){

        mVelocityGenerator.initMovement(*mCurrentData, mDirection, false);
        if(mVelocityGenerator.segment().index2 != -1){
            setState(PathPTPMovementState::MOVING);
            mStateEllapsedTimeS = 0.0f;
        }

        if(mDirection==Direction::FORWARD){
            mActiveAnimationInstance = mCurrentData->aniMovingForward;

        }else if(mDirection==Direction::BACKWARD){
            mActiveAnimationInstance = mCurrentData->aniMovingBackward;
        }


    }else if(mInputDirection != Direction::NONE){

        mDirection = mInputDirection;

        if(mDirection==Direction::FORWARD){
            if(mCurrentData->aniTurningToForward){
                 mActiveAnimationInstance = mCurrentData->aniTurningToForward;
                 setState(PathPTPMovementState::TURNING_AROUND);

            }else{
                mActiveAnimationInstance = mCurrentData->aniStandingForward;
            }


        }else if(mDirection==Direction::BACKWARD){
             if(mCurrentData->aniTurningToBackward){
                 mActiveAnimationInstance = mCurrentData->aniTurningToBackward;
                 setState(PathPTPMovementState::TURNING_AROUND);

             }else{
                 mActiveAnimationInstance = mCurrentData->aniStandingBackward;
             }
        }
        mStateEllapsedTimeS = 0.0f;

    }else{

        // manage IDLE state

        if(mState==PathPTPMovementState::STANDING){
            if(mCurrentData->aniIdleForward && mCurrentData->aniIdleBackward){
                if(mStateEllapsedTimeS > mCurrentData->cfg->idleAnimationDelay){
                    if(mDirection==Direction::FORWARD){
                        mActiveAnimationInstance = mCurrentData->aniIdleForward;

                    }else if(mDirection==Direction::BACKWARD){
                        mActiveAnimationInstance = mCurrentData->aniIdleBackward;

                    }
                    setState(PathPTPMovementState::IDLE);
                    mStateEllapsedTimeS = 0.0f;
                }
            }

        }else if(mState==PathPTPMovementState::IDLE){
            if(mAnimationPlayer && mAnimationPlayer->GetState()==AnimationPlayerState::STALLED){
                if(mDirection==Direction::FORWARD){
                    mActiveAnimationInstance = mCurrentData->aniStandingForward;

                }else if(mDirection==Direction::BACKWARD){
                    mActiveAnimationInstance = mCurrentData->aniStandingBackward;
                }
                setState(PathPTPMovementState::STANDING);
                mStateEllapsedTimeS = 0.0f;
            }
        }
    }

}


void PathPTPMovementEngine::update_stateMOVING(EngineUpdateParameters &eup)
{

    bool finishFullSpeed = (mInputDirection==mDirection && mVelocityGenerator.segment().index3 != -1);

    mVelocity = mVelocityGenerator.updateVelocity(eup.timeStep, finishFullSpeed);

    if(mVelocityGenerator.state()==PointToPointSpeedGenerator::State::STANDING_STILL){
        setState(PathPTPMovementState::STANDING);
        mStateEllapsedTimeS = 0.0f;

        mFixedPositionSet = true;
        mFixedPosition = mVelocityGenerator.currentPoint();

        if(mDirection==Direction::FORWARD){
            mActiveAnimationInstance = mCurrentData->aniStandingForward;

        }else if(mDirection==Direction::BACKWARD){
            mActiveAnimationInstance = mCurrentData->aniStandingBackward;
        }


    }else if(mVelocityGenerator.state()==PointToPointSpeedGenerator::State::FINISHED_FULL_SPEED){

        if(mVelocityGenerator.segment().index3 != -1){       // should always be true
            mVelocityGenerator.initMovement(*mCurrentData, mDirection, false);

        }else{      // safety

            setState(PathPTPMovementState::STANDING);
            mStateEllapsedTimeS = 0.0f;
            if(mDirection==Direction::FORWARD){
                mActiveAnimationInstance = mCurrentData->aniStandingForward;

            }else if(mDirection==Direction::BACKWARD){
                mActiveAnimationInstance = mCurrentData->aniStandingBackward;
            }
        }
    }


}


void PathPTPMovementEngine::update_stateTURNING_AROUND(EngineUpdateParameters &eup)
{

    if(mAnimationPlayer && mAnimationPlayer->GetState()==AnimationPlayerState::STALLED){
        setState(PathPTPMovementState::STANDING);
        mStateEllapsedTimeS = 0.0f;

        if(mDirection==Direction::FORWARD){
            mActiveAnimationInstance = mCurrentData->aniStandingForward;

        }else if(mDirection==Direction::BACKWARD){
            mActiveAnimationInstance = mCurrentData->aniStandingBackward;
        }
    }
}


void PathPTPMovementEngine::updateRotation(EngineUpdateParameters &eup)
{


    mRotationInputDirection = RotationDirection::NONE;
    if(mCurrentData->rotateCW.active()){
        mRotationInputDirection = RotationDirection::CW;

    }else if(mCurrentData->rotateCCW.active()){
        mRotationInputDirection = RotationDirection::CCW;
    }


    if(mRotationState==PathPTPRotationState::STANDING_STILL){

        //if(mRotationInputDirection == mRotationDirection){
        if(mCurrentData->rotateCW.active()){

            mAngularVelocityGenerator.initMovement(mCurrentData->cfg->maxAngularSpeed, mCurrentData->cfg->angularAccDistance);
            mRotationState = PathPTPRotationState::MOVING;

        }


        //}else if(mRotationInputDirection != RotationDirection::NONE){

        //    mRotationDirection = mRotationInputDirection;

        //}

    }else if(mRotationState==PathPTPRotationState::MOVING){

        mAngularVelocity = mAngularVelocityGenerator.updateAngularVelocity(eup.timeStep);

        if(mAngularVelocity == 0.0f){
            mRotationState = PathPTPRotationState::STANDING_STILL;
        }

    }

}



void PathPTPMovementEngine::stop()
{
    mVelocityGenerator.stop();
}


MovementEngineData* PathPTPMovementEngine::getMovementEngineData(const std::string &_name, bool _setErrorMessage)
{

    for(PathPTPMovementData &d : mPathMovementDatas){
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
PathPTPMovementData *PathPointToPointMovementEngine::getPathMovementData(const std::string &_name, bool _setErrorMessage)
{

    for(PathPTPMovementData &d : mPathMovementDatas){
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


PathPTPMovementCfg* PathPTPMovementFactory::createMovementEngineCfg(const std::string &_name)
{
    return new PathPTPMovementCfg(this, _name);
}


PathPTPMovementEngine* PathPTPMovementFactory::createMovementEngine()
{
    return new PathPTPMovementEngine(this);
}




}
