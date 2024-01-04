#include <chrono>
#include <assert.h>
#include <sstream>
#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmMap.h"
#include "jmCamera.h"
#include "jmSceneLayout.h"
#include "jmCommonFunctions.h"
//#include "jmText.h"
#include "jmSpriteLayer.h"
#include "jmStandardSprite.h"
#include "jmCompositeSprite.h"
#include "jmVectorLayer.h"
#include "jmVectorShape.h"
#include "jmSprite.h"
#include "jmSourceSprite.h"
#include "jmUtilities.h"

#include "items/jpItemsCommon.h"
#include "jpPlayedApp.h"
#include "jpPlayedScene.h"
//#include "jpItemsTable.h"
#include "jpUtilities.h"
//#include "jpActionsCommon.h"
#include "jpGlobal.h"
#include "jpQueries.h"
#include "gfxObjects/jpAnimationObjects.h"

#include "jpGuiCommon.h"
#include "jpGuiUtilities.h"
#include "widgets/jpGuiButton.h"
#include "widgets/jpGuiSlider.h"
#include "widgets/jpGuiSlot.h"
#include "widgets/jpGuiTable.h"
#include "widgets/jpGuiTableNew.h"
#include "widgets/jpGuiTextField.h"
#include "widgets/jpGuiTextInput.h"
#include "widgets/jpGuiBar.h"
#include "jpGuiSystem.h"



namespace jugimap{





GuiSystem::GuiSystem()
{

    mName = "guiSystem";

    mWidgetsManager.reset(new WidgetsManager());

    mWidgetsManager->addWidgetFactory(new ButtonFactory("button", WidgetType::BUTTON));
    mWidgetsManager->addWidgetFactory(new SliderFactory("slider", WidgetType::SLIDER));
    mWidgetsManager->addWidgetFactory(new GuiSlotNewFactory("slotNew", WidgetType::SLOT));
    mWidgetsManager->addWidgetFactory(new TableFactory("table", WidgetType::TABLE));
    mWidgetsManager->addWidgetFactory(new TableNewFactory("tableNew", WidgetType::TABLE));
    mWidgetsManager->addWidgetFactory(new TextFieldFactory("textField", WidgetType::TEXT_FIELD));
    mWidgetsManager->addWidgetFactory(new TextInputFactory("textInput", WidgetType::TEXT_INPUT));
    mWidgetsManager->addWidgetFactory(new BarFactory("bar", WidgetType::BAR));

    mWidgetSignalParser = new WidgetSignalParser({"BUTTON", "SLIDER", "TEXT_FIELD", "TEXT_INPUT", "BAR", "TABLE", "SLOT"});
    app->signalParserManager()->addAndStoreSignalParser(mWidgetSignalParser);

}



GuiSystem::~GuiSystem()
{

    deleteWidgets();

    guiButtonExclusiveGroups.deleteContent();
    guiKeyboardAndJoystickInput._clearHighlightedWidget();
    guiKeyboardAndJoystickInput.widgets().clear();
    GuiWidget::_SetInteractedWidget(nullptr);

}



void GuiSystem::deleteWidgets()
{

    //---- delete widgets
    for(GuiWidget * w : mWidgets){
        if(w==GuiWidget::GetInteracted()){
            GuiWidget::_SetInteractedWidget(nullptr);
        }
        delete w;
    }
    mWidgets.clear();

}


bool GuiSystem::initCfg(PlayedScene *_scene, const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading gui system ...");


    mParentPlayerScene = _scene;

    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){

        std::string nodeName = std::string(n.name());

        if(nodeName=="widgets"){

            for(pugi::xml_node nChild = n.first_child(); nChild; nChild = nChild.next_sibling()){
                std::string childNodeName = std::string(nChild.name());

                std::string name = nChild.attribute("name").as_string("");

                GuiWidgetCfg *cfg = mWidgetsManager->addWidgetCfg(childNodeName, name);
                if(cfg==nullptr){
                    return false;
                }
                if(cfg->initCfg(nChild)==false){
                    return false;
                }
            }
        }

    }


    dbgSystem.removeLastMessage();

    return true;

}


bool GuiSystem::buildObjects(PlayedScene *_scene)
{


    dbgSystem.addMessage("Building gui system ...");


    //--- create widgets
    for(GuiWidgetCfg* wCfg : mWidgetsManager->widgetCfgs()){

        std::vector<Sprite*>sprites_WidgetNew;
        std::vector<Sprite*>sprites;
        for(SceneMap *sm : _scene->sceneMaps()){
            CollectSpritesWithParameter(sm->map(), sprites_WidgetNew, "widgetNew", wCfg->name);
            CollectSpritesWithParameter(sm->map(), sprites, "widget", wCfg->name);
        }

        if(wCfg->name=="panelButton"){
            DummyFunction();
        }

        for(Sprite* s : sprites){
            GuiWidget * w = wCfg->factory->createWidget(wCfg, s);

            if(w==nullptr){
                return false;
            }

            mWidgets.push_back(w);
        }

    }

    for(GuiWidget* w : mWidgets){

        if(w->initConnections(this)==false){
            return false;
        }

    }


    setWidgetsToInitialState();

    dbgSystem.removeLastMessage();

    return true;

}


bool GuiSystem::initConnections(PlayedScene *_scene)
{

    if(mInitialized) return true;


    dbgSystem.addMessage("Initializing gui system ... ");


    /*
    //--- create widgets
    for(GuiWidgetCfg* wCfg : mWidgetsManager->widgetCfgs()){

        std::vector<Sprite*>sprites;
        for(SceneMap *sm : _scene->sceneMaps()){
            CollectSpritesWithParameter(sm->map(), sprites, "widgetNew", wCfg->name);
        }

        for(Sprite* s : sprites){
            GuiWidget * w = wCfg->factory->createWidget(wCfg, s);

            if(w==nullptr){
                return false;
            }

            mWidgets.push_back(w);
        }

    }
    */


    /*
    for(GuiWidget* w : mWidgets){

        if(w->initConnections(this)==false){
            return false;
        }

    }

    */
    //---
    mInitialized = true;


    //---
    dbgSystem.removeLastMessage();
    return true;
}


bool GuiSystem::initConnections_setParameters(const std::string &parameters, PlayedScene *_scene)
{

    dbgSystem.addMessage("Initializing gui system - setting parameters ... ");


    for(GuiWidget* w : mUsedWidgets){
        w->cleanupFlags();
    }


    if(parameters.empty()){

        for(GuiWidget* w : mWidgets){
            mUsedWidgets.push_back(w);
        }


    }else{

        std::vector<std::string>parts = StdString::splitString(parameters, "|");
        for(const std::string &s : parts){
            GuiWidget *w = findWidget(s, WidgetType::NOT_DEFINED, true);
            if(w==nullptr){
                return false;
            }
            mUsedWidgets.push_back(w);
        }
    }


    //---
    dbgSystem.removeLastMessage();
    return true;

}


void GuiSystem::start()
{

}




void GuiSystem::preUpdate(UpdateMode &_updateMode)
{


    guiKeyboardAndJoystickInput.update();

    guiCursorDeviceInput._setCursorScreenPosition(Vec2f(mouse.posX(), mouse.posY()));
    guiCursorDeviceInput._setCursorPressedStarted(mouse.isButtonPressed(MouseButton::LEFT));
    guiCursorDeviceInput._setPressed(mouse.isButtonDown(MouseButton::LEFT));


    for(SceneMap *sm : mParentPlayerScene->sceneMaps()){

        if(sm->isVisible()==false) continue;

        jugimap::Map *map = sm->map();

        if(map->isHidden()) continue;



        Vec2f cursorInMapPosition = map->camera()->MapPointFromScreenPoint(guiCursorDeviceInput.getCursorScreenPosition());
        guiCursorDeviceInput._setCursorInMapPosition(cursorInMapPosition);

        //if(mWidgetManager){
        //    mWidgetManager->updateWidgets();
        //}

        updateWidgets(map);
    }

}



void GuiSystem::update(UpdateMode &_updateMode)
{

    if(_updateMode.modalOverlay){
        return;
    }

    // nothing to do here as widgets get updated in 'preUpdate'



}



void GuiSystem::postUpdate(UpdateMode &_updateMode)
{

    //for(GuiWidget * w : mUsedWidgets){
    //    w->postUpdateSignals();
    //}

}



void GuiSystem::onStateEnded()
{

}



void GuiSystem::setWidgetsToInitialState()
{

    for(GuiWidget * w : mWidgets){
        w->setToInitialState();
    }
}


void GuiSystem::updateWidgets(Map *_map)
{

    if(mEnabled==false){
        return;
    }


    for(GuiWidget * w : mUsedWidgets){
        if(w->rootMap()!=_map){
            continue;
        }

        if(w->modalBlockLevel()>0){
            continue;
        }

        w->update();
    }
}


GuiWidget* GuiSystem::findWidget(const std::string &_name, WidgetType guiWidgetKind, bool _setErrorMessage)
{

    for(GuiWidget * w : mWidgets){
        if(w->name()==_name){

            if(w->type()==guiWidgetKind || guiWidgetKind==WidgetType::NOT_DEFINED){
                return w;

            }else{
                if(_setErrorMessage){
                    dbgSystem.addMessage("Widget with name '" + _name + " found but is not of type '" + GetGuiWidgetKindString(guiWidgetKind) + "' !" );
                }
                return nullptr;
            }
        }
    }


    if(_setErrorMessage){
        dbgSystem.addMessage("Widget with name '" + _name + " not found!");
    }

    return nullptr;
}


GuiWidget* GuiSystem::findWidget(int _tag, WidgetType guiWidgetKind, bool _setErrorMessage)
{

    for(GuiWidget * w : mWidgets){
        if(w->tag()==_tag){

            if(w->type() != guiWidgetKind){
                if(_setErrorMessage){
                    dbgSystem.addMessage("Widget with tag '" + std::to_string(_tag) + " found but is not of type '" + GetGuiWidgetKindString(guiWidgetKind) + "' !" );
                }
                return nullptr;
            }

            return w;
        }
    }

    if(_setErrorMessage){
        dbgSystem.addMessage("Widget with tag '" + std::to_string(_tag) + " not found!");
    }

    return nullptr;

}


void GuiSystem::appendToUsedWidgets(const std::vector<GuiWidget *> &_widgets)
{

    for(GuiWidget* w : _widgets){
        if(StdVector::contains(mUsedWidgets, w)==false){
            mUsedWidgets.push_back(w);
        }
        w->cleanupFlags();
    }

}


void GuiSystem::removeFromUsedWidgets(const std::vector<GuiWidget *> &_widgets)
{

    for(GuiWidget* w : _widgets){
        w->cleanupFlags();
        StdVector::removeElement(mUsedWidgets, w);
    }

    for(GuiWidget* w : mUsedWidgets){
        w->cleanupFlags();
    }

    //---
    resetInput();    // in case some widget is actiated not intentionaly

}


void GuiSystem::resetInput()
{

    mouse.reset();
    keyboard.reset();
    touch.reset();
    for(Joystick &gc: joysticks) gc.reset();
    GuiWidget::ResetInteractedPerUpdate();
}


/*
void WidgetManager::appendToUsedWidgets_ModalOverlay(const std::vector<GuiWidget *> &_widgets)
{

    for(GuiWidget* w : _widgets){
        if(StdVector::contains(mUsedWidgets_ModalOverlay, w)==false){
            mUsedWidgets_ModalOverlay.push_back(w);
        }
    }


}



void WidgetManager::removeFromUsedWidgets_ModalOverlay(const std::vector<GuiWidget *> &_widgets)
{

    for(GuiWidget* w : _widgets){
        w->CleanupCursorStates();
        StdVector::removeElement(mUsedWidgets_ModalOverlay, w);
    }

}
*/


void GuiSystem::addModalBlockLevelToUsedWidgets()
{

    for(GuiWidget * w : mUsedWidgets){
        w->cleanupFlags();
        w->addModalBlockLevel();
    }

    resetInput();
}


void GuiSystem::removeModalBlockLevelToUsedWidgets()
{
    for(GuiWidget * w : mUsedWidgets){
        w->cleanupFlags();
        w->removeModalBlockLevel();
    }

    resetInput();
}


//=========================================================================================



WidgetsManager::~WidgetsManager()
{

    for(GuiWidgetCfg * cfg : mWidgetCfgs){
        delete cfg;
    }

    for(WidgetFactory * f : mWidgetFactories){
        delete f;
    }

}


void WidgetsManager::addWidgetFactory(WidgetFactory *_engineCreator)
{
    mWidgetFactories.push_back(_engineCreator);
}


GuiWidgetCfg* WidgetsManager::addWidgetCfg(const std::string &_typeSignature, const std::string &_name, bool _setErrorMessage)
{

    for(GuiWidgetCfg * cfg : mWidgetCfgs){
        if(cfg->name == _name){
            if(_setErrorMessage){
                dbgSystem.addMessage("The name '" + _name + "' of a widget configuration is already used! Every configuration must have unique name!");
            }
            return nullptr;
        }
    }


    //---
    GuiWidgetCfg *cfg = nullptr;

    for(WidgetFactory* mec : mWidgetFactories){
        if(mec->typeSignature() == _typeSignature){
            cfg = mec->createWidgetCfg(_name);
            break;
        }
    }

    if(cfg){
        mWidgetCfgs.push_back(cfg);
        return cfg;
    }


    if(_setErrorMessage){
        dbgSystem.addMessage("Type signature '" + _typeSignature + "' of a widget configuration not recognized!");
    }
    return nullptr;

}


GuiWidgetCfg* WidgetsManager::getWidgetCfg(const std::string &_name, bool _setErrorMessage)
{

    for(GuiWidgetCfg * cfg : mWidgetCfgs){
        if(cfg->name == _name){
            return cfg;
        }
    }


    if(_setErrorMessage){
        dbgSystem.addMessage("A widget configuration with name '" + _name + "' not found!");
    }
    return nullptr;

}

/*
GuiWidget* WidgetsManager::createWidget(const std::string &_typeSignature, bool _setErrorMessage)
{

    for(WidgetFactory* mec : mWidgetFactories){
        if(mec->typeSignature() == _typeSignature){
            return mec->createWidget();
        }
    }

    if(_setErrorMessage){
        dbgSystem.addMessage("Type signature '" + _typeSignature + "' of a widget not recognized!");
    }
    return nullptr;

}
*/



}
