#ifndef PLAYER__ACTION_TRIGGERS_GUI_H
#define PLAYER__ACTION_TRIGGERS_GUI_H


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

class GuiTextInput;
class GuiButton;
class GuiSlider;
class PlayedScene;





class TOnButtonPressed : public ActionTrigger
{
public:
    static std::string type;

    TOnButtonPressed(Action*_parentGameAction, const pugi::xml_node &_node);

    bool initConnections(PlayedScene *scene) override;

    //---
    bool isTriggered() override;

private:
    jugimap::GuiButton* mButton = nullptr;      // LINK

    //--- cfg parameters
    std::string mButtonName;

};


//------------------------------------------------------------------------------


class TOnSliderChanged : public ActionTrigger
{
public:
    static std::string type;

    TOnSliderChanged(Action*_parentGameAction, const pugi::xml_node &_node);

    bool initConnections(PlayedScene *scene) override;

    //---
    bool isTriggered() override;

private:
    jugimap::GuiSlider* mSlider = nullptr;      // LINK

    //--- cfg parameters
    std::string mSliderName;

};


//------------------------------------------------------------------------------


class TOnTextInput : public ActionTrigger
{
public:
    static std::string type;

    TOnTextInput(Action*_parentGameAction, const pugi::xml_node &_node);

    bool initConnections(PlayedScene *scene) override;

    //---
    bool isTriggered() override;

private:
    GuiTextInput *mTextInput = nullptr;           // link

    //--- cfg parameters
    std::string mTextInputName;

};




}

#endif // PLAYER__ACTION_TRIGGERS_GUI_H
