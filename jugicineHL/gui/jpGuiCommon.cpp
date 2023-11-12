#include "jmSprite.h"
#include "jmCompositeSprite.h"
#include "jmVectorShape.h"
#include "jmUtilities.h"
#include "jmSystem.h"

#include "jpQueries.h"
#include "widgets/jpGuiBar.h"
#include "widgets/jpGuiTextInput.h"
#include "widgets/jpGuiButton.h"
#include "widgets/jpGuiSlider.h"
#include "widgets/jpGuiTextField.h"
#include "widgets/jpGuiTable.h"

#include "jpGuiCommon.h"




namespace jugimap {





GuiCursorDeviceInput guiCursorDeviceInput;

//InteractedGUIWidget interactedGuiWidget;


GuiKeyboardAndJoystickInput guiKeyboardAndJoystickInput;

//================================================================================


GuiWidget::~GuiWidget()
{

    if(mObj) delete mObj;
    if(mCallbackObj) delete mCallbackObj;
    mHighlightMarkerSprite = nullptr;

    //for(SignalCallback* sc : mSignalCallbacks){
    //    delete sc;
    //}

}


void GuiWidget::setDisabled(bool _disabled)
{
    mDisabled = _disabled;
    if(mDisabled){
        mPressedStarted = false;
        mHovered = false;
        mPressed = false;
        mValueChanged = false;
    }

}


void GuiWidget::setVisible(bool _visible)
{

    mVisible = _visible;
    if(mVisible==false){
        mPressedStarted = false;
        mHovered = false;
        mPressed = false;
        mValueChanged = false;
    }
}


void GuiWidget::cleanupFlags()
{
    mPressedStarted = false;
    mHovered = false;
    mPressed = false;
    mValueChanged = false;


}

void GuiWidget::setTag(int _tag){ mTag = _tag;}


void GuiWidget::assignCustomObject(CustomObject* _customObject)
{
    if(mObj && mObj!=_customObject){
        delete mObj;
    }

    mObj = _customObject;
}


void GuiWidget::assignCallback(GuiWidgetCallback *_callback)
{
    if(mCallbackObj && mCallbackObj!=_callback){
        delete mCallbackObj;
    }

    mCallbackObj = _callback;
}


void GuiWidget::_setHighlighted(bool _highlighted)
{

    //highlighted = _highlighted;

    if(mHighlightMarkerSprite && mHighlightMarkerSprite->isVisible()!=_highlighted){
        mHighlightMarkerSprite->setVisible(_highlighted);
    }

}


void GuiWidget::_SetInteractedWidget(GuiWidget *_widget)
{

    interactedWidget = _widget;


    if(guiKeyboardAndJoystickInput.highlightedWidget() && guiKeyboardAndJoystickInput.highlightedWidget()!=interactedWidget){
        guiKeyboardAndJoystickInput._clearHighlightedWidget();
    }
    if(guiKeyboardAndJoystickInput.highlightedWidget()==nullptr){
        guiKeyboardAndJoystickInput._proposeWidgetIndex(interactedWidget);
    }

}

bool GuiWidget::onSignalSet(Signal *_signal)
{

    if(_signal==&mSigDisabled){
        bool value = mSigDisabled.value();
        setDisabled(value);
        return true;

    }if(_signal==&mSigVisible){
        bool value = mSigVisible.value();
        setVisible(value);
        return true;
    }

    return false;

}


void GuiWidget::obtainSignal_signalQuery(SignalQuery &_signalQuery, ParsedSignalPath &_psp, bool _setErrorMessage)
{

    if(_psp.signalFullName()=="VISIBLE"){
        _psp.obtainValue(_signalQuery, &mSigVisible);

    }else if(_psp.signalFullName()=="DISABLED"){
        _psp.obtainValue(_signalQuery, &mSigDisabled);

    }

}


void GuiWidget::obtainSignal_signalSetter(SignalSetter &_signalSetter, ParsedSignalPath &_psp, bool _setErrorMessage)
{

    if(_psp.signalFullName()=="VISIBLE"){
        _psp.obtainValue(_signalSetter, &mSigVisible);

    }else if(_psp.signalFullName()=="DISABLED"){
        _psp.obtainValue(_signalSetter, &mSigDisabled);

    }

}




//--------------------------------------------------------------------------------

GuiWidget *GuiWidget::interactedWidget = nullptr;



//================================================================================


void GuiKeyboardAndJoystickInput::deleteContent()
{

    _clearHighlightedWidget();
    mWidgets.clear();

}


void GuiKeyboardAndJoystickInput::update()
{


    mHighlightedPressedStarted = false;
    mHighlightedPressed = false;
    mHighlightedChangedValue = 0;


    if(mDisabled==false && mWidgets.empty()==false){

        for(KeyCode k : mPressKeys){
            if(keyboard.isKeyPressed(k)){
                mHighlightedPressedStarted = true;
            }
            if(keyboard.isKeyDown(k)){
                mHighlightedPressed = true;
            }
        }

        for(int index : mJoyPressButtons){
            if(joysticks[0].isButtonPressed(index)){
                mHighlightedPressedStarted = true;
            }
            if(joysticks[0].isButtonDown(index)){
                mHighlightedPressed = true;
            }
        }


        //----
        int navigate = 0 ;

        for(KeyCode k : mNavigateForwardKeys){
            if(keyboard.isKeyPressed(k)){
                navigate = 1;
            }
        }
        for(KeyCode k : mNavigateBackwardKeys){
            if(keyboard.isKeyPressed(k)){
                navigate = -1;
            }
        }
        if(mJoyNavigateForwardPOV_X!=JoystickPOV_X::NONE && joysticks[0].isPOV_XPressed(mJoyNavigateForwardPOV_X)){
            navigate = 1;
        }else if(mJoyNavigateForwardPOV_Y!=JoystickPOV_Y::NONE && joysticks[0].isPOV_YPressed(mJoyNavigateForwardPOV_Y)){
            navigate = 1;
        }
        if(mJoyNavigateBackwardPOV_X!=JoystickPOV_X::NONE && joysticks[0].isPOV_XPressed(mJoyNavigateBackwardPOV_X)){
            navigate = -1;
        }else if(mJoyNavigateBackwardPOV_Y!=JoystickPOV_Y::NONE && joysticks[0].isPOV_YPressed(mJoyNavigateBackwardPOV_Y)){
            navigate = -1;
        }

        if(navigate!=0){
            _SetHighlightedNavigate(navigate);
        }

        //----
        for(KeyCode k : mIncrementValueKeys){
            if(keyboard.isKeyPressed(k)){
                mHighlightedChangedValue = 1;
            }
        }
        for(KeyCode k : mDecrementValueKeys){
            if(keyboard.isKeyPressed(k)){
                mHighlightedChangedValue = -1;
            }
        }
        if(mJoyIncrementValue_POV_X!=JoystickPOV_X::NONE && joysticks[0].isPOV_XPressed(mJoyIncrementValue_POV_X)){
            mHighlightedChangedValue = 1;
        }else if(mJoyIncrementValue_POV_Y!=JoystickPOV_Y::NONE && joysticks[0].isPOV_YPressed(mJoyIncrementValue_POV_Y)){
            mHighlightedChangedValue = 1;
        }
        if(mJoyDecrementValue_POV_X!=JoystickPOV_X::NONE && joysticks[0].isPOV_XPressed(mJoyDecrementValue_POV_X)){
            mHighlightedChangedValue = -1;
        }else if(mJoyDecrementValue_POV_Y!=JoystickPOV_Y::NONE && joysticks[0].isPOV_YPressed(mJoyDecrementValue_POV_Y)){
            mHighlightedChangedValue = -1;
        }

        if(mHighlightedWidget){
            //interactedGuiWidget._SetWidget(highlightedWidget);
            GuiWidget::_SetInteractedWidget(mHighlightedWidget);
        }
    }

}


void GuiKeyboardAndJoystickInput::setWidgets(std::vector<GuiWidget*>_widgets, GuiWidget *_highlightedWidget)
{
    mWidgets = _widgets;
    mIindexHighlighted = -1;
    mHighlightedWidget = nullptr;

    if(_highlightedWidget){
        _proposeWidgetIndex(_highlightedWidget);
    }

}


void GuiKeyboardAndJoystickInput::_clearHighlightedWidget()
{

    if(mHighlightedWidget){
        mHighlightedWidget->_setHighlighted(false);
        mHighlightedWidget = nullptr;
        mIindexHighlighted = -1;
    }
}


void GuiKeyboardAndJoystickInput::_proposeWidgetIndex(GuiWidget *_widget)
{

    for(int i=0; i<(int)mWidgets.size(); i++){
        if(mWidgets[i]==_widget){
            mIindexHighlighted = i;
        }
    }

}


/*

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

*/

void GuiKeyboardAndJoystickInput::_SetHighlightedNavigate(int _highlightedNavigate)
{

    if(mWidgets.empty()) return;

    if(_highlightedNavigate==-1){
        mIindexHighlighted --;
        if(mIindexHighlighted<0 || mIindexHighlighted>= int(mWidgets.size())){
            mIindexHighlighted = mWidgets.size()-1;
        }

    }else if(_highlightedNavigate==1){
        mIindexHighlighted ++;
        if(mIindexHighlighted<0 || mIindexHighlighted>= int(mWidgets.size())){
            mIindexHighlighted = 0;
        }
    }

    if(mIindexHighlighted==-1){
        if(mHighlightedWidget){
            mHighlightedWidget->_setHighlighted(false);
            mHighlightedWidget = nullptr;
        }
    }else{

        GuiWidget * w = mWidgets.at(mIindexHighlighted);
        if(w != mHighlightedWidget){
            if(mHighlightedWidget){
                mHighlightedWidget->_setHighlighted(false);
            }
            mHighlightedWidget = w;
            mHighlightedWidget->_setHighlighted(true);
        }
        GuiWidget::_SetInteractedWidget(mHighlightedWidget);
    }

}



//-------------------------------------------------------------------------

/*

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

        }else if(widgetType=="table" && s->type()==SpriteType::COMPOSITE){
            widget = new GuiTable(static_cast<CompositeSprite*>(s));
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

    if(mEnabled==false){
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

*/


void DeleteGuiSupportingObjects()
{

    return;
    guiButtonExclusiveGroups.deleteContent();
    guiKeyboardAndJoystickInput._clearHighlightedWidget();
    guiKeyboardAndJoystickInput.widgets().clear();
    GuiWidget::_SetInteractedWidget(nullptr);

}

void GuiCursorDeviceInput::_setCursorPressedStarted(bool _pressedStarted){ mPressedStarted = _pressedStarted; }


}



