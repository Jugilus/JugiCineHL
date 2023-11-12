#ifndef PLAYER__SCENE_LOGIC_STATE_H
#define PLAYER__SCENE_LOGIC_STATE_H

#include <memory>
#include <string>
#include <vector>

#include "jmSignal.h"

#include "jpGlobal.h"
#include "jpLogicState.h"
#include "jpLogicStateCfg.h"
#include "jpSettings.h"


namespace pugi{
class xml_node;
}

namespace jugimap{



struct SceneLogicStateCfg : public LogicStateCfg
{

    SceneLogicStateCfg(const std::string &_name) : LogicStateCfg(_name){}

    bool initCfg(const pugi::xml_node &_node) override;

    LogicStateCfg* createLogicStateCfg(const std::string &_name) override;
    CustomLogicState* createCustomState(LogicState *_parentState) override;


    std::string compound;
    std::string component;


};



class SceneCustomLogicState : public CustomLogicState
{
public:

    SceneCustomLogicState(LogicState *_parentState, SceneLogicStateCfg *_cfg);
    ~SceneCustomLogicState() override;


    bool initConnections(PlayedScene *_scene) override;

    bool startingPhaseUpdate() override;
    void start() override;
    void preUpdate(UpdateMode &_updateMode) override;
    void update(UpdateMode &_updateMode) override;
    void postUpdate(UpdateMode &_updateMode) override;
    void finish() override;
    void initTransitionFrom(CustomLogicState* _prevStateData) override;
    void drawShapes(jugimap::ShapeDrawer &_drawer, int _flags = 0) override;

    std::vector<Component*> &usedComponents(){ return mUsedComponents; }
    Compound * compound(){ return mCompound; }
    CompoundTransition *compoundTransition(){ return mCompoundTransition.get(); }


private:
    SceneLogicStateCfg *mCfg = nullptr;

    std::vector<Component*>mUsedComponents;             // LINKS
    Compound *mCompound = nullptr;                      // LINK
    std::unique_ptr<CompoundTransition>mCompoundTransition;



};


}

#endif // PLAYER__ACTION_COMMON_H
