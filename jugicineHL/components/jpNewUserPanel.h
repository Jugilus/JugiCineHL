#ifndef PLAYER__COMPONENTS__NEW_USER_PANEL__H
#define PLAYER__COMPONENTS__NEW_USER_PANEL__H


#include <memory>
#include "jpComponentsCommon.h"
#include "jpVariables.h"


namespace pugi{
class xml_node;
}

namespace jugimap{

class GTextSprite;
class GuiButton;
class GuiTextInput;
class PlayedScene;


class NewUserPanel : public OverlayComponent
{
public:

    NewUserPanel(const pugi::xml_node &_node);
    bool initConnections(PlayedScene *_scene) override;

    void start() override;
    void update(UpdateMode _updateMode) override;
    void onStateEnded() override;

private:

    GuiTextInput *mTextInput = nullptr;                 // LINK
    GuiButton *mOkButton = nullptr;                     // LINK
    GuiButton *mCancelButton = nullptr;                 // LINK
    StringVar *mVarNewUserName = nullptr;               // LINK
    GTextSprite *mTSActiveUserName = nullptr;           // LINK

    bool mAddingFirstUser = true;

    //--- cfg
    struct Cfg
    {
        std::string mTextInput;
        std::string mOkButton;
        std::string mCancelButton;
    };
    std::unique_ptr<Cfg>mCfg;

    void finishNewUserPanel();
};




}






#endif
