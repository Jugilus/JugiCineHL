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
#include "jmUtilities.h"
#include "items/jpItemsCommon.h"
#include "jpPlayedApp.h"
#include "jpPlayedScene.h"
#include "jpUtilities.h"
//#include "jpActionsCommon.h"


#include "jpEntityLogicState.h"
#include "movements/jpMovementBase.h"
#include "jpB2Body.h"
#include "jpEntitySystem.h"
#include "jpSourceEntity.h"
#include "jpEntity.h"
#include "jpEntityGroups.h"


namespace jugimap{





int GetMovableObjectFromString_signalQuery(const std::string &_type)
{


    if(_type=="MOVED"){
        return MovableObject::MOVED;

    }else if(_type=="ON_LEFT"){
        return MovableObject::ON_LEFT;

    }else if(_type=="ON_RIGHT"){
        return MovableObject::ON_RIGHT;

    }else if(_type=="ON_TOP"){
        return MovableObject::ON_TOP;

    }else if(_type=="ON_BOTTOM"){
        return MovableObject::ON_BOTTOM;

    }

    return MovableObject::UNKNOWN;

}



int GetMovableObjectFromString_signalSetter(const std::string &_type)
{

    if(_type=="MOVED"){
        return MovableObject::MOVED;
    }

    return MovableObject::UNKNOWN;

}


std::vector<NamedValue>gMovableObjectNamedValues_query
{
    {"MOVED", MovableObject::MOVED },
    {"ON_LEFT", MovableObject::ON_LEFT },
    {"ON_RIGHT", MovableObject::ON_RIGHT},
    {"ON_TOP", MovableObject::ON_TOP},
    {"ON_BOTTOM", MovableObject::ON_BOTTOM}

};


std::vector<NamedValue>gMovableObjectNamedValues_setter
{
    {"MOVED", MovableObject::MOVED }

};



//-----------------------------------------------------------------------------------------------



void MovingMovableObjectGroup::initBehavior(BitsetSignal *_movableObjectSignal, Direction _movableObjectPosition)
{
    mMovableObjectSignal = _movableObjectSignal;
    mMovableObjectPosition = _movableObjectPosition;
}




void MovingMovableObjectGroup::update_Movement(UpdateMode &_updateMode)
{


    if(mMovableObjectSignal->activeEnded(MovableObject::MOVED)){
        mParentGroupsManager->deleteEntityGroup(this);     // this also sets all pointers to nullptr
        return;
    }
    for(Entity *e : mMembers){
        if(e->currentEngine()->type()!=MovementEngineType::GROUND_MOVEMENT){
            mMovableObjectSignal->setFlags_onNextFrame(MovableObject::MOVED, false);
            return;
        }
    }


    //--- add blcked direction from moved object to the actor pushing the movable objects
    Direction direction = mLeader->direction();

    if(direction==Direction::LEFT){
        DummyFunction();
    }else{
        DummyFunction();
    }

    if(direction==Direction::RIGHT || direction==Direction::LEFT){
        mLeader->blockedDirectionsRef() &= ~static_cast<int>(direction);
        for(Entity* e : mMembers){
            if(e==mLeader) continue;

            if(mMovableObjectPosition==direction){
                if(e->blockedDirectionsRef() & static_cast<int>(direction)){
                    mLeader->blockedDirectionsRef() |= static_cast<int>(direction);
                }
            }

        }
    }


    //---
    mLeader->update_Movement(_updateMode);




    for(Entity* e : mMembers){
        if(e==mLeader) continue;
        //e->update(_updateMode);

        e->update_Movement(_updateMode);

        /*
        if(mLeader->statusFlagsRef() & EntityStatusFlags::MOVING_ON_GROUND){
            e->statusFlagsRef() |= EntityStatusFlags::MOVING_ON_GROUND;
        }else{
            e->statusFlagsRef() &= ~EntityStatusFlags::MOVING_ON_GROUND;
        }

        e->_setVelocity(mLeader->velocity());
        e->_setAngularVelocity(mLeader->angularVelocity());
        e->body()->B2Body()->SetLinearVelocity(e->velocity());
        e->body()->B2Body()->SetAngularVelocity(e->angularVelocity());
        */
    }

}



}



