#ifndef PLAYER__LOGIC_STATE_CONDITIONS_H
#define PLAYER__LOGIC_STATE_CONDITIONS_H

#include <memory>
#include <string>
#include <vector>

#include <jmTween.h>
#include <jmInput.h>

#include "jpQueries.h"
#include "jpSettings.h"
#include "jpLogicState.h"


namespace pugi{
class xml_node;
}

namespace jugimap{


class PlayedScene;
class TriggerVariable;




/*
class IfCondition : public ActionCondition
{
public:
    static std::string type;

    IfCondition(const pugi::xml_node &_node);

    bool initConnections(PlayedScene *scene) override;

    //---
    bool isConditionTrue() override;

private:
    std::vector<Variable*>mVariables;
    std::vector<Variable*>mVariableValues;

    //--- cfg parameters
    struct Cfg
    {
        std::string mVariables;
    };
    std::unique_ptr<Cfg>mCfg;

};
*/


//==============================================================


class ACOnSignal : public ActionCondition
{
public:
    static std::string type;

    ACOnSignal(Action *_parentAction, const std::string &_value);

    bool initConnections(PlayedScene *scene) override;

    //---
    bool isTrue() override;


private:
    SignalQuery mSignalQuery;


};

//==============================================================




class ACOnSignals : public ActionCondition
{
public:
    static std::string type;


    ACOnSignals(Action *_parentAction, const std::string &_value);

    bool initConnections(PlayedScene *_scene) override;

    //---
    bool isTrue() override;


private:

    CompositeQuery mCompositeQuery;

};





}

#endif // PLAYER__ACTION_TRIGGERS_LOGIC_H
