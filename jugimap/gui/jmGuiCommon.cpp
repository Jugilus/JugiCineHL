#include "jmSprite.h"
#include "jmCompositeSprite.h"
#include "jmVectorShape.h"
#include "jmGuiBar.h"
#include "jmGuiTextInput.h"
#include "jmGuiButton.h"
#include "jmGuiSlider.h"
#include "jmGuiTextField.h"
#include "jmUtilities.h"
#include "jmGuiCommon.h"




namespace jugimap {





GuiCursorDeviceInput guiCursorDeviceInput;

//InteractedGUIWidget interactedGuiWidget;


GuiKeyboardAndJoystickInput guiKeyboardAndJoystickInput;

//================================================================================


GuiWidget::~GuiWidget()
{

    if(obj) delete obj;
    if(callbackObj) delete callbackObj;
    highlightMarkerSprite = nullptr;

}


void GuiWidget::SetDisabled(bool _disabled)
{
    disabled = _disabled;
    if(disabled){
        pressed = false;
        cursorOver = false;
        cursorDown = false;
        valueChanged = false;
    }

}


void GuiWidget::SetVisible(bool _visible)
{

    visible = _visible;
    if(visible==false){
        pressed = false;
        cursorOver = false;
        cursorDown = false;
        valueChanged = false;
    }
}


void GuiWidget::CleanupFlags()
{
    pressed = false;
    cursorOver = false;
    cursorDown = false;
    valueChanged = false;
}


void GuiWidget::AssignCustomObject(CustomObject* _customObject)
{
    if(obj && obj!=_customObject){
        delete obj;
    }

    obj = _customObject;
}


void GuiWidget::AssignCallback(GuiWidgetCallback *_callback)
{
    if(callbackObj && callbackObj!=_callback){
        delete callbackObj;
    }

    callbackObj = _callback;
}


void GuiWidget::_SetHighlighted(bool _highlighted)
{

    //highlighted = _highlighted;

    if(highlightMarkerSprite && highlightMarkerSprite->isVisible()!=_highlighted){
        highlightMarkerSprite->setVisible(_highlighted);
    }

}


void GuiWidget::_SetInteractedWidget(GuiWidget *_widget)
{

    interactedWidget = _widget;


    if(guiKeyboardAndJoystickInput.GetHighlightedWidget() && guiKeyboardAndJoystickInput.GetHighlightedWidget()!=interactedWidget){
        guiKeyboardAndJoystickInput._ClearHighlightedWidget();
    }
    if(guiKeyboardAndJoystickInput.GetHighlightedWidget()==nullptr){
        guiKeyboardAndJoystickInput._ProposeWidgetIndex(interactedWidget);
    }

}


//--------------------------------------------------------------------------------

GuiWidget *GuiWidget::interactedWidget = nullptr;



//================================================================================


void GuiKeyboardAndJoystickInput::deleteContent()
{

    _ClearHighlightedWidget();
    widgets.clear();

}


void GuiKeyboardAndJoystickInput::Update()
{


    highlightedPressed = false;
    highlightedDown = false;
    highlightedChangeValue = 0;


    if(disabled==false && widgets.empty()==false){

        for(KeyCode k : pressKeys){
            if(keyboard.isKeyPressed(k)){
                highlightedPressed = true;
            }
            if(keyboard.isKeyDown(k)){
                highlightedDown = true;
            }
        }

        for(int index : joyPressButtons){
            if(joysticks[0].isButtonPressed(index)){
                highlightedPressed = true;
            }
            if(joysticks[0].isButtonDown(index)){
                highlightedDown = true;
            }
        }


        //----
        int navigate = 0 ;

        for(KeyCode k : navigateForwardKeys){
            if(keyboard.isKeyPressed(k)){
                navigate = 1;
            }
        }
        for(KeyCode k : navigateBackwardKeys){
            if(keyboard.isKeyPressed(k)){
                navigate = -1;
            }
        }
        if(joyNavigateForwardPOV_X!=JoystickPOV_X::NONE && joysticks[0].isPOV_XPressed(joyNavigateForwardPOV_X)){
            navigate = 1;
        }else if(joyNavigateForwardPOV_Y!=JoystickPOV_Y::NONE && joysticks[0].isPOV_YPressed(joyNavigateForwardPOV_Y)){
            navigate = 1;
        }
        if(joyNavigateBackwardPOV_X!=JoystickPOV_X::NONE && joysticks[0].isPOV_XPressed(joyNavigateBackwardPOV_X)){
            navigate = -1;
        }else if(joyNavigateBackwardPOV_Y!=JoystickPOV_Y::NONE && joysticks[0].isPOV_YPressed(joyNavigateBackwardPOV_Y)){
            navigate = -1;
        }

        if(navigate!=0){
            _SetHighlightedNavigate(navigate);
        }

        //----
        for(KeyCode k : incrementValueKeys){
            if(keyboard.isKeyPressed(k)){
                highlightedChangeValue = 1;
            }
        }
        for(KeyCode k : decrementValueKeys){
            if(keyboard.isKeyPressed(k)){
                highlightedChangeValue = -1;
            }
        }
        if(joyIncrementValue_POV_X!=JoystickPOV_X::NONE && joysticks[0].isPOV_XPressed(joyIncrementValue_POV_X)){
            highlightedChangeValue = 1;
        }else if(joyIncrementValue_POV_Y!=JoystickPOV_Y::NONE && joysticks[0].isPOV_YPressed(joyIncrementValue_POV_Y)){
            highlightedChangeValue = 1;
        }
        if(joyDecrementValue_POV_X!=JoystickPOV_X::NONE && joysticks[0].isPOV_XPressed(joyDecrementValue_POV_X)){
            highlightedChangeValue = -1;
        }else if(joyDecrementValue_POV_Y!=JoystickPOV_Y::NONE && joysticks[0].isPOV_YPressed(joyDecrementValue_POV_Y)){
            highlightedChangeValue = -1;
        }

        if(highlightedWidget){
            //interactedGuiWidget._SetWidget(highlightedWidget);
            GuiWidget::_SetInteractedWidget(highlightedWidget);
        }
    }

}


void GuiKeyboardAndJoystickInput::SetWidgets(std::vector<GuiWidget*>_widgets, GuiWidget *_highlightedWidget)
{
    widgets = _widgets;
    indexHighlighted = -1;
    highlightedWidget = nullptr;

    if(_highlightedWidget){
        _ProposeWidgetIndex(_highlightedWidget);
    }

}


void GuiKeyboardAndJoystickInput::_ClearHighlightedWidget()
{

    if(highlightedWidget){
        highlightedWidget->_SetHighlighted(false);
        highlightedWidget = nullptr;
        indexHighlighted = -1;
    }
}


void GuiKeyboardAndJoystickInput::_ProposeWidgetIndex(GuiWidget *_widget)
{

    for(int i=0; i<(int)widgets.size(); i++){
        if(widgets[i]==_widget){
            indexHighlighted = i;
        }
    }

}


void GuiKeyboardAndJoystickInput::_SetHighlightedPressed(bool _pressed)
{
    if(highlightedWidget==nullptr) return;

    highlightedPressed = _pressed;
}


void GuiKeyboardAndJoystickInput::_SetHighlightedDown(bool _down)
{
    if(highlightedWidget==nullptr) return;

    highlightedDown = _down;
}


void GuiKeyboardAndJoystickInput::_SetHighlightedChangeValue(int _highlightedChangeValue)
{
    if(highlightedWidget==nullptr) return;

    highlightedChangeValue = _highlightedChangeValue;
}


void GuiKeyboardAndJoystickInput::_SetHighlightedNavigate(int _highlightedNavigate)
{

    if(widgets.empty()) return;

    if(_highlightedNavigate==-1){
        indexHighlighted --;
        if(indexHighlighted<0 || indexHighlighted>= int(widgets.size())){
            indexHighlighted = widgets.size()-1;
        }

    }else if(_highlightedNavigate==1){
        indexHighlighted ++;
        if(indexHighlighted<0 || indexHighlighted>= int(widgets.size())){
            indexHighlighted = 0;
        }
    }

    if(indexHighlighted==-1){
        if(highlightedWidget){
            highlightedWidget->_SetHighlighted(false);
            highlightedWidget = nullptr;
        }
    }else{

        GuiWidget * w = widgets.at(indexHighlighted);
        if(w != highlightedWidget){
            if(highlightedWidget){
                highlightedWidget->_SetHighlighted(false);
            }
            highlightedWidget = w;
            highlightedWidget->_SetHighlighted(true);
        }
        GuiWidget::_SetInteractedWidget(highlightedWidget);
    }

}



//-------------------------------------------------------------------------



WidgetManager::~WidgetManager()
{
    deleteWidgets();
}



void WidgetManager::deleteWidgets()
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



bool WidgetManager::createWidgets(Map *_map)
{

    std::vector<Sprite*>sprites;
    CollectSpritesWithConstParameter(_map, sprites, "widget");

    for(Sprite *s : sprites){

        std::string widgetType = s->sourceSprite()->constantParameters().value("widget");

        GuiWidget * widget = nullptr;

        if(widgetType=="button"){
            widget = new GuiButton(s);

        }else if(widgetType=="slider" && s->type()==SpriteType::COMPOSITE){
            widget = new GuiSlider(static_cast<CompositeSprite*>(s));

        }else if(widgetType=="bar"){
            widget = new GuiBar(s);

        }else if(widgetType=="textInput" && s->type()==SpriteType::COMPOSITE){
            widget = new GuiTextInput(static_cast<CompositeSprite*>(s));

        }

        if(widget){
            if(widget->init()==false){
                delete widget;
                return false;
            }
            widgets().push_back(widget);
        }

    }


    //----
    std::vector<VectorShape*>shapes;
    CollectVectorShapesWithParameter(_map, shapes, "widget");

    for(VectorShape *vs : shapes){

        std::string widgetType = vs->parameters().value("widget");
        GuiWidget * widget = nullptr;

        if(widgetType=="textField"){
            widget = new GuiTextField(vs, _map);
        }

        if(widget){
            if(widget->init()==false){
                delete widget;
                return false;
            }
            widgets().push_back(widget);
        }
    }


    return true;
}



void WidgetManager::setWidgetsToInitialState()
{

    for(GuiWidget * w : mWidgets){
        w->SetToInitialState();
    }
}


void WidgetManager::updateWidgets()
{

    if(mWidgetsActive==false){
        return;
    }

    //if(mUsedWidgets_ModalOverlay.empty()==false){

   ///     for(GuiWidget * w : mUsedWidgets_ModalOverlay){
   //         w->Update();
   //     }

    //}else{
        for(GuiWidget * w : mUsedWidgets){
            if(w->modalBlockLevel()>0) continue;

            w->Update();
        }

   // }


}


GuiWidget* WidgetManager::findWidget(const std::string &_name)
{

    for(GuiWidget * w : mWidgets){
        if(w->GetName()==_name){
            return w;
        }
    }

    return nullptr;
}


GuiWidget* WidgetManager::findWidget(int _tag)
{

    for(GuiWidget * w : mWidgets){
        if(w->GetTag()==_tag){
            return w;
        }
    }

    return nullptr;

}


void WidgetManager::appendToUsedWidgets(const std::vector<GuiWidget *> &_widgets)
{

    for(GuiWidget* w : _widgets){
        if(StdVector::contains(mUsedWidgets, w)==false){
            mUsedWidgets.push_back(w);
        }
        w->CleanupFlags();
    }

}


void WidgetManager::removeFromUsedWidgets(const std::vector<GuiWidget *> &_widgets)
{

    for(GuiWidget* w : _widgets){
        w->CleanupFlags();
        StdVector::removeElement(mUsedWidgets, w);
    }

    for(GuiWidget* w : mUsedWidgets){
        w->CleanupFlags();
    }

    //---
    resetInput();    // in case some widget is actiated not intentionaly

}


void WidgetManager::resetInput()
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


void WidgetManager::addModalBlockLevelToUsedWidgets()
{

    for(GuiWidget * w : mUsedWidgets){
        w->CleanupFlags();
        w->addModalBlockLevel();
    }

    resetInput();
}


void WidgetManager::removeModalBlockLevelToUsedWidgets()
{
    for(GuiWidget * w : mUsedWidgets){
        w->CleanupFlags();
        w->removeModalBlockLevel();
    }

    resetInput();
}


void DeleteGuiSupportingObjects()
{

    guiButtonExclusiveGroups.deleteContent();
    guiKeyboardAndJoystickInput._ClearHighlightedWidget();
    guiKeyboardAndJoystickInput.GetWidgets().clear();
    GuiWidget::_SetInteractedWidget(nullptr);

}


}



