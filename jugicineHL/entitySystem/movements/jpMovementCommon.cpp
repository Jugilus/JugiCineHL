#include <chrono>
#include <assert.h>
#include <sstream>
#include "pugixml/pugixml.hpp"
#include "jmCommonFunctions.h"
#include "jmSystem.h"

#include "jpB2World.h"
#include "jpMovementCommon.h"



namespace jugimap{





bool SlidingGroundCfg::initCfg(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading sliding ground ...");

    for(pugi::xml_attribute a = _node.first_attribute(); a; a = a.next_attribute()){
        std::string attributeName =std::string(a.name());

        if(attributeName=="name"){
            // already set

        }else if(attributeName=="speedOn45degSlopeP"){
            speedOn45degSlope = gWorldInfo.pixelsToMeters(a.as_float(speedOn45degSlope));

        }else if(attributeName=="speedOn45degSlopeM"){
            speedOn45degSlope = a.as_float(speedOn45degSlope);

        }else if(attributeName=="accDistanceP"){
            accDistance = gWorldInfo.pixelsToMeters(a.as_float(accDistance));

        }else if(attributeName=="accDistanceM"){
            accDistance = a.as_float(accDistance);

        }else{

            dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + std::string(_node.name()) +"' !");
            return false;

        }
    }


    dbgSystem.removeLastMessage();
    return true;
}



bool ConveyorGroundCfg::initCfg(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading conveyor ground ...");


    for(pugi::xml_attribute a = _node.first_attribute(); a; a = a.next_attribute()){
        std::string attributeName =std::string(a.name());

        if(attributeName=="name"){
            // already set

        }else if(attributeName=="speedP"){
            speed = gWorldInfo.pixelsToMeters(a.as_float(speed));

        }else if(attributeName=="speedM"){
            speed = a.as_float(speed);

        }else if(attributeName=="direction"){
            direction = GetDirectionFromString(a.as_string(""));

            if(direction != Direction::RIGHT && direction !=  Direction::LEFT){
                direction = Direction::RIGHT;
            }

        }else{

            dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + std::string(_node.name()) +"' !");
            return false;

        }
    }


    dbgSystem.removeLastMessage();
    return true;
}




//-----------------------------------------------------------------------------------------------


SlidingGroundCfg * ExtraGroundTypesCfgsGroup::getSlidingGroundCfg(const std::string &_name, bool _setErrorMessage)
{

    for(SlidingGroundCfg* g : mSlidingGroundCfgs){
        if(g->name==_name){
            return g;
        }
    }

    if(_setErrorMessage){
        dbgSystem.addMessage("A 'SlidingGroundCfg' object with name '" + _name + "' not found!");
    }

    return nullptr;
}



ConveyorGroundCfg * ExtraGroundTypesCfgsGroup::getConveyorGroundCfg(const std::string &_name, bool _setErrorMessage)
{

    for(ConveyorGroundCfg* g : mConveyorGroundCfgs){
        if(g->name==_name){
            return g;
        }
    }

    if(_setErrorMessage){
        dbgSystem.addMessage("A 'ConveyorGroundCfg' object with name '" + _name + "' not found!");
    }

    return nullptr;
}




}
