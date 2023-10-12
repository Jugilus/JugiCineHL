#ifndef PLAYER__ACTION_CONDITIONS_H
#define PLAYER__ACTION_CONDITIONS_H

#include <memory>
#include <string>
#include <vector>

#include <jmTween.h>
#include <jmInput.h>
#include "jpSettings.h"
#include "actions/jpActionsCommon.h"


namespace pugi{
class xml_node;
}

namespace jugimap{


class PlayedScene;
class TriggerVariable;





class IfCondition : public ActionCondition
{
public:
    static std::string type;

    IfCondition(Action*_parentGameAction, const pugi::xml_node &_node);

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





}

#endif // PLAYER__ACTION_TRIGGERS_LOGIC_H
