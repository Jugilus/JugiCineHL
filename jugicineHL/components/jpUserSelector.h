#ifndef PLAYER__COMPONENTS__USER_SELECTOR__H
#define PLAYER__COMPONENTS__USER_SELECTOR__H


#include <memory>
#include "jpComponentsCommon.h"
#include "jpVariables.h"


namespace pugi{
class xml_node;
}

namespace jugimap{


class GTextSprite;
class GuiTable;
class GuiButton;
class ItemsGroup;
class PlayedScene;
class ADoTransition;
class Compound;


class UserSelector : public Component
{
public:

    UserSelector(const pugi::xml_node &_node);

    bool initConnections(PlayedScene *_scene) override;

    void start() override;
    void update(UpdateMode &_updateMode) override;
    void onStateEnded() override;


private:

    enum class State
    {
        SELECTOR,
        NEW_USER_PANEL,
        CONFIRM_DELETE_PANEL
    };

    //PlayedScene *mParentPlayerScene = nullptr;
    State mState = State::SELECTOR;

    GuiTable *mUsersTable = nullptr;                        // LINK
    GuiButton *mAddNewUserButton = nullptr;                 // LINK
    GuiButton *mBDeleteUserButton = nullptr;                  // LINK
    //std::vector<GuiWidget*>mSelectorWidgets;

    //---
    //OverlayComponent* mNewUserPanelComponent = nullptr;         // LINK
    //std::unique_ptr<ADoTransition>mAOpenNewUserPanel;
    //std::unique_ptr<ADoTransition>mACloseNewUserPanel;

    Compound* mNewUserPanelCompound = nullptr;

    //---
    //OverlayComponent* mConfirmDeleteUserComponent = nullptr;         // LINK
    Compound* mConfirmDeleteUserCompound = nullptr;
    std::unique_ptr<ComponentData> mConfirmDeleteUserComponentData;
    //std::unique_ptr<ADoTransition>mAOpenConfirmDeletePanel;
    //std::unique_ptr<ADoTransition>mACloseConfirmDeletePanel;

    TriggerVariable *mConfirmPanel_resultYes = nullptr;             // LINK
    TriggerVariable *mConfirmPanel_resultNo = nullptr;             // LINK


    //---
    //StringVar *mVarNewUserName = nullptr;                       // LINK
    GTextSprite *mTSActiveUserName = nullptr;               //LINK
    ItemsGroup* mUsersNamesItems = nullptr;                 // LINK


    //--- cfg
    struct Cfg
    {
        std::string mUsersTable;
        std::string mAddNewUserButton;
        std::string mBDeleteUserButton;

        //std::string mNewUserPanelComponent;
        //std::string mNewUserPanelComponentTransition;
        //std::string mNewUserPanelComponentTransitionShow;
        std::string mNewUserPanelCompound;

        //std::string mConfirmDeleteUserComponent;
        //std::string mConfirmDeleteUserComponentTransition;
        //std::string mConfirmDeleteUserComponentTransitionShow;
        std::string mConfirmDeleteUserCompound;
        //---data for mConfirmDeleteUserComponent
        std::string mConfirmDeleteTextSegment;
    };

    std::unique_ptr<Cfg>mCfg;


    void CheckConfirmPanelTriggers();


};




}






#endif
