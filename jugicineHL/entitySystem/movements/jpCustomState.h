#ifndef PLAYER__MOVEMENT__CUSTOM_STATE__H
#define PLAYER__MOVEMENT__CUSTOM_STATE__H

#include <memory>
#include <string>
#include <vector>

#include "logic/jpLogicStateCfg.h"
#include "jpQueries.h"
#include "jpMovementBase.h"



namespace pugi{
class xml_node;
}

namespace jugimap{

class ActionTrigger;
class Entity;
class AnimationInstance;
class PlayedScene;
struct SourceFixture;




struct CustomStateCfg
{

    bool initCfg(const pugi::xml_node &_node, std::vector<std::pair<std::string, int>> &_engineStateNamedValues);


    std::string name;
    std::vector<std::pair<std::string, int>>engineStateValues;


    //std::string sigTrigger;
    std::string aniRight;
    std::string aniLeft;

    //---
    //std::string contextTrigger;

    ActionTriggerCfg actionTriggerCfg;

    //---
    bool engineMovementAllowed = false;
    int activePhaseFrameIntData = -1;

    //---
    CustomSensorCfg customSensorCfg;
};



struct CustomStateData
{

    ~CustomStateData();
    bool initConnections(PlayedScene *_scene, Entity *_actor);


    CustomStateCfg *cfg = nullptr;          //LINK
    //SignalQuery sqTrigger;
    AnimationInstance * aniLeft = nullptr;                  // OWNED
    AnimationInstance * aniRight = nullptr;                 // OWNED

    //---
    //SignalQuery contextTrigger;

    ActionTrigger * actionTrigger = nullptr;                 // OWNED

    //---
    SourceFixture *triggeringExtraShapeSourceFixture = nullptr;
    bool extraShapeFixtureCreated = false;

    //---
    int customActSignalId = 0;

};


enum class CustomStateStatus
{
    NO_CUSTOM_STATE,
    CUSTOM_STATE_STARTED,
    CUSTOM_STATE_ACTIVE,
    CUSTOM_STATE_ENDED
};


class CustomStateEngine
{
public:

    void init(MovementEngine* _engine,  std::vector<CustomStateData> *_customStates);

    void reset();
    CustomStateStatus update(EngineUpdateParameters &eup, int _baseState);
    CustomStateData *activeCustomState(){ return mCustomState; }

private:
    CustomStateStatus mStatus = CustomStateStatus::NO_CUSTOM_STATE;

    MovementEngine* mEngine = nullptr;                              // LINK
    std::vector<CustomStateData> * mCustomStates = nullptr;         // LINK
    CustomStateData *mCustomState = nullptr;                        // LINK
    AnimationInstance * mActiveAnimationInstance = nullptr;         // LINK


    CustomStateData *obtainActiveCustomState(int _baseState);
    CustomStateData* _checkCustomState( CustomStateData * _customState);


};





}






#endif
