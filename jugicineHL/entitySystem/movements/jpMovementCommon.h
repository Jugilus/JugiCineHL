#ifndef PLAYER__MOVEMENT_SPEED_CFG__H
#define PLAYER__MOVEMENT_SPEED_CFG__H

#include <memory>
#include <string>
#include <vector>
#include "jpEntityGlobal.h"



namespace pugi{
class xml_node;
}

namespace jugimap{






struct SlidingGroundCfg
{

    SlidingGroundCfg(const std::string &_name): name(_name){}

    bool initCfg(const pugi::xml_node &_node);

    std::string name;

    float speedOn45degSlope = 5.0f;    // meters
    float accDistance = 5.0f;           // meters,seconds
};


struct ConveyorGroundCfg
{

    ConveyorGroundCfg(const std::string &_name): name(_name){}

    bool initCfg(const pugi::xml_node &_node);

    std::string name;

    float speed = 5.0f;
    Direction direction = Direction::RIGHT;
};






class ExtraGroundTypesCfgsGroup
{
public:

    SlidingGroundCfg* getSlidingGroundCfg(const std::string &_name, bool _setErrorMessage = true);
    ConveyorGroundCfg* getConveyorGroundCfg(const std::string &_name, bool _setErrorMessage = true);


    std::vector<SlidingGroundCfg*> & slidingGroundCfgs(){ return mSlidingGroundCfgs; }
    std::vector<ConveyorGroundCfg*> & conveyorGroundCfgs(){ return mConveyorGroundCfgs; }

private:
    std::vector<SlidingGroundCfg*>mSlidingGroundCfgs;
    std::vector<ConveyorGroundCfg*>mConveyorGroundCfgs;

};


}






#endif
