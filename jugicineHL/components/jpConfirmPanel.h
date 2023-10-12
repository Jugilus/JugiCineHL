#ifndef PLAYER__COMPONENTS__CONFIRM_PANEL__H
#define PLAYER__COMPONENTS__CONFIRM_PANEL__H


#include <memory>
#include "jpComponentsCommon.h"
#include "jpVariables.h"


namespace pugi{
class xml_node;
}

namespace jugimap{

class GTextSprite;
class GuiButton;
class GuiTextField;
class PlayedScene;


class ConfirmPanel : public OverlayComponent
{
public:

    enum class Mode
    {
        NOT_DEFINED,
        DELETE_USER
    };


    ConfirmPanel(const pugi::xml_node &_node);
    bool initConnections(PlayedScene *_scene) override;

    void start() override;
    void update(UpdateMode _updateMode) override;
    void onStateEnded() override;

private:

    GuiTextField *mTextField = nullptr;                 // LINK
    GuiButton *mYesButton = nullptr;                     // LINK
    GuiButton *mNoButton = nullptr;                 // LINK

    TriggerVariable *mResult_Yes;
    TriggerVariable *mResult_No;


    Mode mMode = Mode::NOT_DEFINED;


    //--- cfg
    struct Cfg
    {
        std::string mTextField;
        std::string mYesButton;
        std::string mNoButton;
    };
    std::unique_ptr<Cfg>mCfg;

};




}






#endif
