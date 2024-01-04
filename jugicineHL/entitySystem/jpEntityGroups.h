#ifndef PLAYER__ENTITY__ENTITY_GROUPS__H
#define PLAYER__ENTITY__ENTITY_GROUPS__H

#include <memory>
#include <string>
#include <vector>
#include <limits>
#include "box2d.h"
#include "jmCommon.h"
#include "jmSignal.h"
#include "jpGlobal.h"
#include "jpEntityCommon.h"


namespace jugimap{


class PlayedScene;
class Entity;







struct MovableObject
{
    static const int UNKNOWN =      0;
    static const int MOVED =                        1 << 0;
    static const int ON_RIGHT =                     1 << 1;
    static const int ON_LEFT =                      1 << 2;
    static const int ON_TOP =                       1 << 3;
    static const int ON_BOTTOM =                    1 << 4;

};

int GetMovableObjectFromString_signalQuery(const std::string &_type);

int GetMovableObjectFromString_signalSetter(const std::string &_type);

extern std::vector<std::pair<std::string, unsigned int>>gMovableObjectNamedValues_query;
extern std::vector<std::pair<std::string, unsigned int>>gMovableObjectNamedValues_setter;


//------------------------------------------------------------------------------------



class MovingMovableObjectGroup : public EntityMovingGroup
{
public:
    MovingMovableObjectGroup() : EntityMovingGroup(EntityGroupType::MOVING_MOVABLE_OBJECT){}


    void initBehavior(BitsetSignal *_movableObjectSignal, Direction _movableObjectPosition);
    void update_Movement(UpdateMode &_updateMode) override;


private:
    BitsetSignal *mMovableObjectSignal = nullptr;      // LINK

    Direction mMovableObjectPosition = Direction::NONE;


};






}






#endif
