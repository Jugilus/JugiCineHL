#ifndef PLAYER__ACTION_FACTORY_H
#define PLAYER__ACTION_FACTORY_H


#include <string>


namespace pugi{
class xml_node;
}

namespace jugimap {


class Action;
class ActionTrigger;
//class BehaviorAction;
class ActionCommand;
//class ActionTrigger;
class ActionCondition;
class TransitionOp;
class TransitionMember;
class Component;
//class BehaviorState;




class ActionFactory
{
public:

    virtual ~ActionFactory(){}

    //virtual ActionTrigger* createActionTrigger(const std::string &type, Action* parentAction, const pugi::xml_node& node);
    virtual ActionCondition* createActionCondition(const std::string &type, Action* parentAction, const std::string &value);
    virtual ActionCondition* createActionTriggerCondition(const std::string &type, ActionTrigger* parentAction, const std::string &value);

    //virtual ActionCommand* createActionCommands(const std::string &type, Action* parentAction, const pugi::xml_node& node);
    virtual ActionCommand* createActionCommands(const std::string &type, Action* parentAction, const std::string &value);


   // virtual ActionCondition* createActionConditionForEntityAction(const std::string &type, BehaviorAction* parentAction, const std::string& value);

    //virtual ActionCommand* createActionCommandsForEntityAction(const std::string &type, BehaviorAction* parentAction, const std::string& value);
    //virtual ActionCondition* createActionConditionDirectionCopy(ActionCondition* source, BehaviorAction *parentAction, const std::string& value);
    //virtual ActionCommand* createActionCommandsDirectionCopy(ActionCommand* source, jugimap::BehaviorAction *parentAction, const std::string& value);


    virtual TransitionOp* createTransitionInOut(const std::string &type, TransitionMember* transitionedObject, const pugi::xml_node& node);

    virtual Component* createComponent(const std::string &type, const pugi::xml_node& node);



};


extern ActionFactory* actionFactory;


}




#endif // PLAYER__ACTION_FACTORY_H
