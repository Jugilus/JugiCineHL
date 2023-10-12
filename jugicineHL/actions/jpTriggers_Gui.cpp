#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmGuiCommon.h"
#include "jmGuiBar.h"
#include "jmGuiButton.h"
#include "jmGuiSlider.h"
#include "jmGuiTextInput.h"
#include "jmAnimationCommon.h"
#include "map/jmSpriteLayer.h"
#include "jmSystem.h"
#include "jmGlobal.h"
#include "jpUtilities.h"
#include "jpTriggers_Gui.h"


namespace jugimap{



std::string TOnButtonPressed::type = "onButtonPressed";

TOnButtonPressed::TOnButtonPressed(Action *_parentGameAction, const pugi::xml_node &_node)
{
    mParentGameAction = _parentGameAction;
    mButtonName = _node.attribute("button").as_string("");
}


bool TOnButtonPressed::initConnections(PlayedScene *scene)
{
    dbgSystem.addMessage("init '" + type + "'");

    mButton = dynamic_cast<GuiButton*>(ObtainGuiWidget(scene, mButtonName, GuiWidgetKind::BUTTON));

    if(mButton==nullptr){
        return false;
    }

    dbgSystem.removeLastMessage();

    return true;
}


bool TOnButtonPressed::isTriggered()
{

    //return mButton->IsPressed();

   // bool result = (GuiWidget::GetInteracted_Pressed(mButtonName) != nullptr);

    bool pressed = mButton->IsPressed();

    //if(result==true){
    //    DummyFunction();
    //}

    return pressed;

}


//-------------------------------------------------------------------------------------------


std::string TOnSliderChanged::type = "onSliderChanged";


TOnSliderChanged::TOnSliderChanged(Action *_parentGameAction, const pugi::xml_node &_node)
{
    mParentGameAction = _parentGameAction;
    mSliderName = _node.attribute("slider").as_string("");
}


bool TOnSliderChanged::initConnections(PlayedScene *scene)
{
    dbgSystem.addMessage("init '" + type + "'");

    mSlider = dynamic_cast<GuiSlider*>(ObtainGuiWidget(scene, mSliderName, GuiWidgetKind::SLIDER));

    if(mSlider==nullptr){
        return false;
    }

    dbgSystem.removeLastMessage();

    return true;
}


bool TOnSliderChanged::isTriggered()
{

    //return mButton->IsPressed();

   // bool result = (GuiWidget::GetInteracted_Pressed(mButtonName) != nullptr);

    bool changed = mSlider->IsValueChanged();

    //if(result==true){
    //    DummyFunction();
    //}

    return changed;

}


//-------------------------------------------------------------------------------------------



std::string TOnTextInput::type = "onTextInputDone";

TOnTextInput::TOnTextInput(Action *_parentGameAction, const pugi::xml_node &_node)
{
    mParentGameAction = _parentGameAction;
    mTextInputName = _node.attribute("textInput").as_string("");
}


bool TOnTextInput::initConnections(PlayedScene *scene)
{
    dbgSystem.addMessage("init '" + type + "'");


    mTextInput = dynamic_cast<GuiTextInput*>(ObtainGuiWidget(scene, mTextInputName, GuiWidgetKind::TEXT_INPUT));
    if(mTextInput==nullptr){
        return false;
    }

    dbgSystem.removeLastMessage();

    return true;
}


bool TOnTextInput::isTriggered()
{

    bool interacted = GuiWidget::GetInteracted()==mTextInput;   // when text input is finished ( by pressing enter)

    if(interacted){
        print("textInput: " + mTextInput->text());
    }

    return interacted;

}


}
