//#include "jpTriggers_Logic.h"
//#include "jpTriggers_Gui.h"
//#include "jpTriggers_Input.h"
#include "jpConditions.h"
#include "jpCommands.h"
#include "jpTransitionOperations.h"

#include "jpInputCustomizer.h"
#include "jpUserSelector.h"
#include "jpLanguageSelector.h"
#include "jpNewUserPanel.h"
#include "jpConfirmPanel.h"
#include "jpMessagePanel.h"
#include "jpObjectFactory.h"



namespace jugimap {


/*
ActionTrigger* ActionFactory::createActionTrigger(const std::string &type, Action* parentAction, const pugi::xml_node &node)
{

    //if(type==EGameStateStarted::type){
    //    return new EGameStateStarted(parentAction, node);

    //}else if(type==TOnButtonPressed::type){
    //    return new TOnButtonPressed(parentAction, node);

    //}else if(type==TOnSliderChanged::type){
    //    return new TOnSliderChanged(parentAction, node);

    //}else if(type==TOnTextInput::type){
    //    return new TOnTextInput(parentAction, node);

    //}else if(type==EkeyPressed::type){
    //    return new EkeyPressed(parentAction, node);

    //}else if(type==EActionFinished::type){
    //    return new EActionFinished(parentAction, node);

    //}else if(type==TOnTrigger::type){
    //    return new TOnTrigger(parentAction, node);

    //}else if(type==EDoAction::type){
    //    return new EDoAction(parentAction, node);

    //}else if(type==EDoActionNew::type){
    //    return new EDoAction(parentAction, node);

    //}


    return nullptr;
}
*/


ActionCondition* ActionFactory::createActionCondition(const std::string &type, Action *parentAction, const std::string &value)
{

    //if(type==IfCondition::type){
    //    return new IfCondition(node);

    //}else
    if(type==ACOnSignal::type){
        return new ACOnSignal(parentAction, value);

    }else if(type==ACOnSignals::type){
        return new ACOnSignals(parentAction, value);

    }

    return nullptr;
}


/*
ActionCommand* ActionFactory::createActionCommands(const std::string &type, Action* parentAction, const pugi::xml_node &node)
{


    //if(type==SetButtonState::type){
    //    return new SetButtonState(parentAction, node);

    //}else if(type==SetTextInput::type){
    //    return new SetTextInput(parentAction, node);

    //}else
    //if(type==SetTableItemsGroup::type){
    //    return new SetTableItemsGroup(parentAction, node);

    //}else if(type==SetActiveState::type){
    //    return new SetActiveState(parentAction, node);

    //}else if(type==SetNextState::type){
    //    return new SetNextState(parentAction, node);

    //}else if(type==AResetAction::type){
    //    return new AResetAction(parentAction, node);

    //}else if(type==AShowOverlayComponent::type){
    //    return new AShowOverlayComponent(parentAction, node);

    //}else if(type==AShowOverlayCompound::type){
    //    return new AShowOverlayCompound(parentAction, node);

    //}else if(type==AQuitGame::type){
    //    return new AQuitGame(parentAction, node);

    //}else if(type==ASetGlobalVariable::typeGlobal){
    //    return  new ASetGlobalVariable(parentAction, node, ASetGlobalVariable::VariableOrigin::GLOBAL);

    //}else
    //if(type==ASetGlobalVariable::typeScene){
    //    return  new ASetGlobalVariable(parentAction, node, ASetGlobalVariable::VariableOrigin::SCENE);

    //}else if(type==ASetGlobalVariable::typeEntity){
    //    return  new ASetGlobalVariable(parentAction, node, ASetGlobalVariable::VariableOrigin::ENTITY);

    //}else if(type==CActivateTrigger::type){
     //   return  new CActivateTrigger(parentAction, node);

    //}else if(type==ADoTransition::type){
    //    return  new ADoTransition(parentAction, node);

    //}else if(type==ASetGfxObject::type){
    //    return  new ASetGfxObject(parentAction, node);

    //}else if(type==CSetSystemParameter::type){
    //    return  new CSetSystemParameter(parentAction, node);

    //}else if(type==APlayScene::type){
    //    return  new APlayScene(parentAction, node);

    //}

    return nullptr;
}
*/


ActionCommand* ActionFactory::createActionCommands(const std::string &type, Action* parentAction, const std::string &value)
{


    if(type==SetNextState::doType){
        return new SetNextState(parentAction, value);

    //if(type==SetChildState::doType){
    //    return new SetChildState(parentAction, value);

    }else if(type==DoSetSignalB::doType){
        return new DoSetSignalB(parentAction, value);

    }else if(type==AShowOverlayCompound::doType){
        return new AShowOverlayCompound(parentAction, value);

    }else if(type==SetTableItemsGroup::doType){
        return new SetTableItemsGroup(parentAction, value);

    }else if(type==AQuitGame::doType){
        return new AQuitGame(parentAction, value);

    }else if(type==ADbgPrint::doType){
        return new ADbgPrint(parentAction, value);
    }

    return nullptr;
}



/*
ActionCondition* ActionFactory::createActionConditionForEntityAction(const std::string &type, BehaviorAction *parentAction, const std::string &value)
{

    if(type==ACOnSignal::type){
        return new ACOnSignal(parentAction, value);

    }else if(type==ACOnSignals::type){
        return new ACOnSignals(parentAction, value);

    //}else if(type==OnGroundMovementState::type){
    //    return new OnGroundMovementState(parentAction, value);

    //}else if(type==OnJumpMovementState::type){
    //    return new OnJumpMovementState(parentAction, value);

    //}else if(type==OnGroundDetected::type){
    //    return new OnGroundDetected(parentAction, value);

    //}else if(type==OnPatwayDetected::type){
    //    return new OnPatwayDetected(parentAction, value);

    //}else if(type==OnPatwayAccessed::type){
    //    return new OnPatwayAccessed(parentAction, value);

    //}else if(type==OnPatwayEnd::type){
    //    return new OnPatwayEnd(parentAction, value);

    //}else if(type==OnPathwayToGroundMovement::type){
    //    return new OnPathwayToGroundMovement(parentAction, value);

    }


    return nullptr;
}

*/


/*
ActionCommand* ActionFactory::createActionCommandsForEntityAction(const std::string &type, BehaviorAction *parentAction, const std::string& value)
{

    if(type==DoPlayAnimation::type){
        return new DoPlayAnimation(parentAction, value);

    //}else if(type==DoGroundMovement::type){
    //    return new DoGroundMovement(parentAction, value);

    //}else if(type==DoJumpMovement::type){
    //    return new DoJumpMovement(parentAction, value);

    //}else if(type==DoPointToPointPathMovement::type){
    //    return new DoPointToPointPathMovement(parentAction, value);

    //}else if(type==DoPointToPointMovementTask::type){
    //    return new DoPointToPointMovementTask(parentAction, value);

    //}else if(type==DoPathMovement::type){
    //    return new DoPathMovement(parentAction, value);

    //}else if(type==DoAnimatedBehavior::type){
    //    return new DoAnimatedBehavior(parentAction, value);

    }else if(type==DoSetState::type){
        return new DoSetState(parentAction, value);

    }else if(type==DoSetSignal::type){
        return new DoSetSignal(parentAction, value);

   // }else if(type==DoTransition::type){
    //    return new DoTransition(parentAction, value);

    }

    return nullptr;

}
*/


/*

ActionCondition* ActionFactory::createActionConditionDirectionCopy(ActionCondition* source, BehaviorAction *parentAction, const std::string& value)
{

    if(dynamic_cast<ACOnSignal*>(source)){
        return new ACOnSignal(parentAction, value);

    //}else if(dynamic_cast<OnGroundMovementState*>(source)){
    //    return new OnGroundMovementState(parentAction, value);

    //}else if(dynamic_cast<OnGroundDetected*>(source)){
    //    return new OnGroundDetected(parentAction, value);

    //}else if(dynamic_cast<OnPatwayDetected*>(source)){
    //    return new OnPatwayDetected(parentAction, value);

    }

    return nullptr;
}


ActionCommand* ActionFactory::createActionCommandsDirectionCopy(ActionCommand* source, BehaviorAction *parentAction, const std::string& value)
{

    if(dynamic_cast<DoPlayAnimation*>(source)){
        return new DoPlayAnimation(parentAction, value);

    //}else if(dynamic_cast<DoSetGroundMovement*>(source)){
    //    return new DoSetGroundMovement(parentAction, value);

    }else if(dynamic_cast<DoSetState*>(source)){
        return new DoSetState(parentAction, value);

    //}else if(dynamic_cast<DoTransition*>(source)){
    //    return new DoTransition(parentAction, value);

    }

    return nullptr;
}

*/



TransitionOp* ActionFactory::createTransitionInOut(const std::string &type, TransitionMember *transitionedObject, const pugi::xml_node &node)
{

    if(type==TranslateInOutOfScreen::typeOut){
        return new TranslateInOutOfScreen(transitionedObject, node, TransitionType::OUT);

    }else if(type==TranslateInOutOfScreen::typeIn){
        return new TranslateInOutOfScreen(transitionedObject, node, TransitionType::IN);

    }else if(type==AlphaFadeInOut::typeOut){
        return new AlphaFadeInOut(transitionedObject, node, TransitionType::OUT);

    }else if(type==AlphaFadeInOut::typeIn){
        return new AlphaFadeInOut(transitionedObject, node, TransitionType::IN);

    }else if(type==ScaleInOut::typeOut){
        return new ScaleInOut(transitionedObject, node, TransitionType::OUT);

    }else if(type==ScaleInOut::typeIn){
        return new ScaleInOut(transitionedObject, node, TransitionType::IN);
    }

    return nullptr;
}


Component* ActionFactory::createComponent(const std::string &type, const pugi::xml_node& node)
{

    if(type=="userSelector"){
        return new UserSelector(node);

    }else if(type=="messagePanel"){
        return new MessagePanel(node);

    }else if(type=="confirmPanel"){
        return new ConfirmPanel(node);

    }else if(type=="inputCustomizer"){
        return new InputCustomizer(node);

    }else if(type=="newUserPanel"){
        return new NewUserPanel(node);

    }else if(type=="languageSelector"){
        return new LanguageSelector(node);

    //}else if(type=="entitySystemUpdater"){
    //    return new EntitySystemUpdater(node);

    }

    return nullptr;
}





ActionFactory* actionFactory = nullptr;

}
