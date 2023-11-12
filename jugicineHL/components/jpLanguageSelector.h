#ifndef PLAYER__COMPONENTS__LANGUAGE_SELECTOR__H
#define PLAYER__COMPONENTS__LANGUAGE_SELECTOR__H


#include <memory>
#include "jpComponentsCommon.h"
#include "jpVariables.h"


namespace pugi{
class xml_node;
}

namespace jugimap{


class TextSprite;
class GTextSprite;
class GuiTable;
class GuiButton;
class ItemsGroup;
class PlayedScene;
class ADoTransition;


class LanguageSelector : public Component
{
public:

    LanguageSelector(const pugi::xml_node &_node);
    ~LanguageSelector();

    bool initConnections(PlayedScene *_scene) override;

    void start() override;
    void update(UpdateMode &_updateMode) override;
    void onStateEnded() override;


private:

    //PlayedScene *mParentPlayerScene = nullptr;
    //std::vector<GuiWidget*>mUsedWidgets;            // LINKS

    //GuiButton* mFirstButton = nullptr;              // LINK
    std::vector<GuiButton*>mExtraButtons;           // OWNED  ( only one button for one language is created in the editor, other are copies stored here)
    //TextSprite* mLanguageTextSprite = nullptr;      // LINK

    std::vector<std::pair<std::string, GuiButton*>>mLanguageButtons;

    float mSpacingFactor = 1.0;

    //--- cfg
    struct Cfg
    {
        std::string mLanguageButton;
        std::string mLanguageTextSprite;
    };

    std::unique_ptr<Cfg>mCfg;

};




}






#endif
