#ifndef PLAYER__ACTION_TRIGGERS_LOGIC_H
#define PLAYER__ACTION_TRIGGERS_LOGIC_H


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



class EGameStateStarted : public ActionTrigger
{
public:
    static std::string type;

    EGameStateStarted(Action *_parentGameAction, const pugi::xml_node &_node);

    bool initConnections(PlayedScene *scene) override;

    //---
    bool isTriggered() override;


private:
    LogicState *mGameState = nullptr;            // LINK

    //--- cfg parameters
    std::string mStatePath;

};


//-------------------------------------------------------------------------------


class EActionFinished: public ActionTrigger
{
public:
    static std::string type;

    EActionFinished(Action*_parentGameAction, const pugi::xml_node &_node);

    bool initConnections(PlayedScene *scene) override;

    //---
    bool isTriggered() override;


private:
    Action* mGameAction = nullptr;          // LINK

    //--- cfg parameters
    std::string mActionName;

};


//-------------------------------------------------------------------------------


class TOnTrigger : public ActionTrigger
{
public:
    static std::string type;

    TOnTrigger(Action*_parentGameAction, const pugi::xml_node &_node);

    bool initConnections(PlayedScene *scene) override;

    //---
    bool isTriggered() override;


private:
    TriggerVariable* mTrigger = nullptr;          // LINK

    //--- cfg parameters
    std::string mTriggerName;

};



//-------------------------------------------------------------------------------

/*

class TOnVariableChanged : public ActionTrigger
{
public:
    static std::string type;

    TOnVariableChanged(Action*_parentGameAction, const pugi::xml_node &_node);

    bool initConnections(PlayerScene *scene) override;

    //---
    bool isTriggered() override;


private:
    Variable* mVariable = nullptr;          // LINK

    //Variable* mStored = nullptr;
    int mIntValue = 0;
    float mFloatValue = 0.0f;
    bool mBoolValue = false;
    std::string mStringValue;


    //--- cfg parameters
    std::string mVariableName;

};


*/


//-------------------------------------------------------------------------------


class EDoAction : public ActionTrigger
{
public:
    static std::string type;

    EDoAction(Action*_parentGameAction, const pugi::xml_node &_node);

    bool initConnections(PlayedScene *scene) override;

    //----
    bool isTriggered() override;

};


}

#endif // PLAYER__ACTION_TRIGGERS_LOGIC_H
