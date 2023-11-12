#ifndef PLAYER__ACTION_TRIGGERS_INPUT_H
#define PLAYER__ACTION_TRIGGERS_INPUT_H


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



/*
class EkeyPressed : public ActionTrigger
{
public:
    static std::string type;


    EkeyPressed(Action*_parentGameAction, const pugi::xml_node &_node);

    bool initConnections(PlayedScene *scene) override;

    //---
    bool isTriggered() override;

private:
    KeyCode mKeyCode;

    //--- cfg parameters
    std::string mKeyName;

};
*/


}

#endif // PLAYER__ACTION_TRIGGERS_INPUT_H
