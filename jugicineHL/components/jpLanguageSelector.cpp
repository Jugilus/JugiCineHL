#include <chrono>
#include <assert.h>
#include <sstream>
#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmCommonFunctions.h"
#include "jmTextSprite.h"
#include "jmUtilities.h"
#include "jmText.h"

#include "gui/jpGuiSystem.h"
#include "gui/widgets/jpGuiButton.h"
#include "gui/widgets/jpGuiTable.h"

#include "items/jpItemsCommon.h"
#include "jpPlayedApp.h"
#include "jpPlayedScene.h"

#include "jpUtilities.h"
#include "jpLanguageSelector.h"



namespace jugimap{



using namespace jugimap;



LanguageSelector::LanguageSelector(const pugi::xml_node &_node)
{

    mName = "languageSelector";
    mCfg.reset(new Cfg());

    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());

        if(nodeName=="useWidgets"){
            mCfg->mLanguageButton = n.attribute("languageButton").as_string("");
            mCfg->mLanguageTextSprite = n.attribute("languageTextSprite").as_string("");
            mSpacingFactor = n.attribute("spacingFactor").as_float(1.0f);
        }
    }

}


LanguageSelector::~LanguageSelector()
{

    for(GuiButton *b : mExtraButtons){
        delete b;
    }

}


bool LanguageSelector::initConnections(PlayedScene *_scene)
{

    if(mInitialized) return true;


    dbgSystem.addMessage("init component '" + mName + "'");

    mParentPlayerScene = _scene;

    //---
    GuiButton* mFirstButton = dynamic_cast<GuiButton*>(ObtainGuiWidget(_scene, mCfg->mLanguageButton, WidgetType::BUTTON));
    if(mFirstButton==nullptr){
        return false;
    }
    mUsedWidgets.push_back(mFirstButton);

    //---
    std::vector<Sprite*>sprites;
    CollectSpritesWithName(_scene, sprites, mCfg->mLanguageTextSprite);
    if(sprites.empty() || sprites.front()->type()!=SpriteType::TEXT){
        dbgSystem.addMessage("Text sprite with name '" +  mCfg->mLanguageTextSprite +  "' not found!" );
        return false;
    }
    TextSprite *mLanguageTextSprite = static_cast<TextSprite*>(sprites.front());


    int spacing = mFirstButton->GetSprite()->boundingBox().GetHeight();
    spacing *= mSpacingFactor;

    //--- set languages
    const std::string &activeLanguage = textLibrary.activeLanguage();

    for(unsigned int i=0; i<textLibrary.languages().size(); i++){
        const std::string &language = textLibrary.languages().at(i);
        GuiButton *button = nullptr;

        if(i==0){       // use already created object for the first language
            button = mFirstButton;
            mLanguageTextSprite->setText(language);

        }else{          // for other languages create new objects

            Sprite *buttonSprite = mFirstButton->GetSprite()->makeActiveCopy();
            buttonSprite->setPosition(Vec2f(mFirstButton->GetSprite()->position().x,
                                            mFirstButton->GetSprite()->position().y - i*spacing));
            button = new GuiButton(mFirstButton->cfg(), buttonSprite);
            if(button->initConnections(_scene->guiSystem())==false){
                assert(true);
            }
            mExtraButtons.push_back(button);
            mUsedWidgets.push_back(button);

            //_--
            TextSprite *ts = static_cast<TextSprite*>(mLanguageTextSprite->makeActiveCopy());
            ts->setText(language);
            ts->setPosition(Vec2f(mLanguageTextSprite->position().x, mLanguageTextSprite->position().y - i*spacing));
        }

        mLanguageButtons.emplace_back(std::pair<std::string, GuiButton*>(language, button));

        if(language==activeLanguage){
            button->SetChecked(true);
        }
    }

    //--- inform system settings that 'language' should also be stored in the .ini file
    app->systemSettings()->appendToUsedSystemParameters(SystemParameter::LANGUAGE);


    //---
    mInitialized = true;

    //---
    dbgSystem.removeLastMessage();
    return true;
}


void LanguageSelector::start()
{
    mParentPlayerScene->guiSystem()->appendToUsedWidgets(mUsedWidgets);
    //GuiSystem *guiSystem = dynamic_cast<GuiSystem *>(mParentPlayerScene->componentsGroup()->getComponent("guiSystem", false));  assert(guiSystem);
    //guiSystem->appendToUsedWidgets(mUsedWidgets);

}


void LanguageSelector::update(UpdateMode &_updateMode)
{

    if(_updateMode.modalOverlay){
        return;
    }

    for(const auto &lb : mLanguageButtons){
        const std::string &language = lb.first;
        GuiButton *button = lb.second;

        if(button->isPressedStarted()){
            app->systemSettings()->setActiveLanguage(language);
            app->systemSettings()->saveSettingsIniFile();
        }
    }
}


void LanguageSelector::onStateEnded()
{
    mParentPlayerScene->guiSystem()->removeFromUsedWidgets(mUsedWidgets);
    //GuiSystem *guiSystem = dynamic_cast<GuiSystem *>(mParentPlayerScene->componentsGroup()->getComponent("guiSystem", false));  assert(guiSystem);
    //guiSystem->removeFromUsedWidgets(mUsedWidgets);

}


}
