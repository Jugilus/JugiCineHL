#ifndef JUGIMAP_GUI_COMMON_H
#define JUGIMAP_GUI_COMMON_H

#include "jmGlobal.h"
#include "jmInput.h"
#include "jmTimer.h"
#include "jmSignal.h"
#include "jmCommonFunctions.h"

#include "jpGlobal.h"


namespace pugi {
class xml_node;
}

namespace jugimap {


class Sprite;
class Map;
class VectorShape;

class GuiWidget;
class WidgetFactory;

class PlayedScene;
class GuiSystem;

class WidgetFactory;

struct SignalQuery;
struct SignalSetter;
class ParsedSignalPath;

/// \addtogroup Gui
/// @{




struct GuiWidgetCfg
{

    GuiWidgetCfg(WidgetFactory *_factory, const std::string &_name) : factory(_factory), name(_name){}
    virtual ~GuiWidgetCfg(){}

    virtual bool initCfg(const pugi::xml_node &_node) = 0;

    WidgetFactory * factory = nullptr;     // LINK
    std::string name;


};


///\brief The GuiWidgetCallback class is the base callback class for widgets.
struct GuiWidgetCallback
{

    virtual ~GuiWidgetCallback(){}


    ///\brief A function call when the cursor is pressed.
    /// ///
    ///  This callback is available for GuiButton.
    virtual void OnPressedStarted(GuiWidget*){}


    ///\brief A function call when the cursor is over the widget.
    /// ///
    ///  This callback is available for GuiButton.
    virtual void OnHovered(GuiWidget*){}


    ///\brief A function call when the cursor is hold down.
    ///
    ///  This callback is available for GuiButton.
    virtual void OnCursorDown(GuiWidget*){}


    ///\brief A function call when the value of a widget is changed.
    ///
    /// This callback is available for GuiSlider and GuiTextInput.
    virtual void OnValueChanged(GuiWidget*){}


    ///\brief A function call when a slider's value is changed via increment or decrement button.
    ///
    /// This callback is available for GuiSlider.
    virtual void OnValueChangedViaButton(GuiWidget*){}

};







///\brief The GuiWidget class is the base widget class.
class GuiWidget : public SignalCallback
{

public:

    GuiWidget(){}

    GuiWidget(WidgetFactory *_factory){ mFactory = _factory; }

    virtual ~GuiWidget();
    GuiWidget(const GuiWidget &src) = delete;
    GuiWidget(const GuiWidget &&src) = delete;
    GuiWidget& operator=(const GuiWidget &src) = delete;
    GuiWidget& operator=(const GuiWidget &&src) = delete;


    virtual bool initCfg(const pugi::xml_node &_node){return true;}

    virtual bool initConnections(GuiSystem *_guiSystem) { return true; }

    //virtual bool init(){ return true; }

    //-----------------------------------------------------------------

    ///\brief Returns true if the cursor is hold down over this widget; otherwise returns false.
    ///
    /// This function is applicable to GuiButton widgets.
    bool isPressed(){ return mPressed; }


    ///\brief Returns true if the cursor is over this widget; otherwise returns false.
    ///
    /// This function is applicable to GuiButton widgets.
    bool isHovered(){ return mHovered; }


    ///\brief Returns true if the cursor is pressed while over this widget; otherwise returns false.
    ///
    /// This function is applicable to GuiButton widgets.
    bool isPressedStarted(){ return mPressedStarted; }


    ///\brief Returns true if this widget's value is changed; otherwise returns false.
    ///
    /// This function is applicable to GuiSlider and GuiTextInput widgets.
    bool isValueChanged(){ return mValueChanged; }


    ///\brief Set the disabled state of this widget.
    ///
    /// \see IsDisabled
    virtual void setDisabled(bool _disabled);


    ///\brief Returns true if this widget is disabled; otherwise returns false.
    ///
    ///\see SetDisabled
    bool isDisabled(){ return mDisabled; }


    /// \brief Set the visibility of this widget.
    ///
    /// This function is not available for GuiTextField.
    /// \see IsVisible
    virtual void setVisible(bool _visible);


    /// \brief Returns true if this widget is visible; otherwise returns false.
    ///
    /// \see SetVisible
    bool isVisible(){ return mVisible; }


    virtual void cleanupFlags();


    void setChildWidget(bool _childWidget){ mChildWidget = _childWidget; }

    bool isChildWidget(){ return mChildWidget; }


    ///\brief Set the name of this widget.
    ///
    /// \see GetName
    void setName(const std::string &_name){ mName = _name;}


    ///\brief Returns the name of this widget.
    ///
    /// \see SetName
    const std::string &name(){ return mName; }


    ///\brief Set the tag of this widget.
    ///
    /// \see GetTag
    void setTag(int _tag);


    ///\brief Returns the tag of this widget.
    ///
    /// \see SetTag
    int tag(){ return mTag; }


    ///\brief Returns the kind of this widget.
    WidgetType type(){ return mType; }


    ///\brief Returns the design kind of this widget.
    std::string designKind(){ return mDesignKind; }


    ///\brief Assign the given *_customObject* to this widget.
    ///
    /// **Important!** The widget take ownership of the custom object!
    /// \see GetCustomObject
    void assignCustomObject(CustomObject* _customObject);


    /// \brief Returns the custom object of this widget or nullptr if none.
    ///
    /// \see AssignCustomObject
    CustomObject* customObject(){ return mObj; }


    ///\brief Assign the given *_callback* to this widget.
    ///
    /// **Important!** The widget take ownership of the callback!
    /// \see GetCallback
    void assignCallback(GuiWidgetCallback *_callback);


    /// \brief Returns the callback of this widget or nullptr if none.
    ///
    /// \see AssignCallback
    GuiWidgetCallback * callback(){ return mCallbackObj; }


    virtual void _setHighlighted(bool _highlighted);


    virtual void update() = 0;

    virtual void setToInitialState(){}


    bool isInitialized(){ return mInitialized; }

    void addModalBlockLevel(){ mModalBlockLevel ++;}

    void removeModalBlockLevel()
    {
        mModalBlockLevel --;
        if(mModalBlockLevel<0){
            mModalBlockLevel=0;
        }
    }

    int modalBlockLevel(){ return mModalBlockLevel; }


    //void preUpdateSignals(){ Signal::preUpdateSignals(mSignals); }
    //void postUpdateSignals(){ Signal::postUpdateSignals(mSignals); }
    //void resetSignals(){ Signal::resetSignals(mSignals); }

    //BoolSignal &visibleSignal(){ return mSigVisible; }
    //BoolSignal &disableSignal(){ return mSigDisabled; }


    bool onSignalSet(Signal *_signal) override;


    virtual void obtainSignal_signalQuery(SignalQuery &_signalQuery, ParsedSignalPath &_psp, bool _setErrorMessage=true);
    virtual void obtainSignal_signalSetter(SignalSetter &_signalSetter, ParsedSignalPath &_psp, bool _setErrorMessage=true);




    // QUERY INTERACTED WIDGET
    //-----------------------------------------------------------------


    /// \brief Returns the interacted widget.
    ///
    /// If there is no widget interacting the function returns nullptr.
    static GuiWidget* GetInteracted(){ return interactedWidget; }


    /// \brief Returns the interacted widget with the given *_name*.
    ///
    /// If there is no such widget interacting the function returns nullptr.
    static GuiWidget* GetInteracted(const std::string &_name){ return (interactedWidget && interactedWidget->name()==_name)? interactedWidget : nullptr;}


    /// \brief Returns the interacted widget with the given *_tag*.
    ///
    /// If there is no such widget interacting the function returns nullptr.
    static GuiWidget* GetInteracted(int _tag){ return (interactedWidget && interactedWidget->tag()==_tag)? interactedWidget : nullptr;}


    /// \brief Returns the interacted widget which has assigned the given *_customObject*.
    ///
    /// If there is no such widget interacting the function returns nullptr.
    static GuiWidget* GetInteracted(CustomObject* _customObject){ return (interactedWidget && interactedWidget->customObject()==_customObject)? interactedWidget : nullptr;}


    //-----------------------------------------------------------------


    /// \brief Returns the interacted widget which is pressed.
    ///
    /// If there is no such widget interacting the function returns nullptr.
    static GuiWidget* GetInteracted_Pressed(){ return (interactedWidget && interactedWidget->isPressedStarted())? interactedWidget : nullptr; }


    /// \brief Returns the interacted widget with the given *_name* which is pressed.
    ///
    /// If there is no such widget interacting the function returns nullptr.
    static GuiWidget* GetInteracted_Pressed(const std::string &_name){ return (interactedWidget && interactedWidget->isPressedStarted() && interactedWidget->name()==_name)? interactedWidget : nullptr; }


    /// \brief Returns the interacted widget with the given *_tag* which is pressed.
    ///
    /// If there is no such widget interacting the function returns nullptr.
    static GuiWidget* GetInteracted_Pressed(int _tag){ return (interactedWidget && interactedWidget->isPressedStarted() && interactedWidget->tag()==_tag)? interactedWidget : nullptr; }


    /// \brief Returns the interacted widget with assigned *_customObject* which is pressed.
    ///
    /// If there is no such widget interacting the function returns nullptr.
    static GuiWidget* GetInteracted_Pressed(CustomObject* _customObject){ return (interactedWidget && interactedWidget->isPressedStarted() && interactedWidget->customObject()==_customObject)? interactedWidget : nullptr; }


    //-----------------------------------------------------------------


    /// \brief Returns the interacted widget which has changed value.
    ///
    /// If there is no such widget interacting the function returns nullptr.
    static GuiWidget* GetInteracted_ValueChanged(){ return (interactedWidget && interactedWidget->isValueChanged())? interactedWidget : nullptr; }


    /// \brief Returns the interacted widget with the given *_name* which has changed value.
    ///
    /// If there is no such widget interacting the function returns nullptr.
    static GuiWidget* GetInteracted_ValueChanged(const std::string &_name){ return (interactedWidget && interactedWidget->isValueChanged() && interactedWidget->name()==_name)? interactedWidget : nullptr; }


    /// \brief Returns the interacted widget with the given *_tag* which has changed value.
    ///
    /// If there is no such widget interacting the function returns nullptr.
    static GuiWidget* GetInteracted_ValueChanged(int _tag){ return (interactedWidget && interactedWidget->isValueChanged() && interactedWidget->tag()==_tag)? interactedWidget : nullptr; }


    /// \brief Returns the interacted widget with assigned *_customObject* which has changed value.
    ///
    /// If there is no such widget interacting the function returns nullptr.
    static GuiWidget* GetInteracted_ValueChanged(CustomObject* _customObject){ return (interactedWidget && interactedWidget->isValueChanged() && interactedWidget->customObject()==_customObject)? interactedWidget : nullptr; }


    //-----------------------------------------------------------------


    /// \brief Returns the interacted widget which has the cursor over.
    ///
    /// If there is no such widget interacting the function returns nullptr.
    static GuiWidget* GetInteracted_CursorOver(){ return (interactedWidget && interactedWidget->isHovered())? interactedWidget : nullptr; }


    /// \brief Returns the interacted widget with the given *_name* which has the cursor over.
    ///
    /// If there is no such widget interacting the function returns nullptr.
    static GuiWidget* GetInteracted_CursorOver(const std::string &_name){ return (interactedWidget && interactedWidget->isHovered() && interactedWidget->name()==_name)? interactedWidget : nullptr; }


    /// \brief Returns the interacted widget with the given *_tag* which has the cursor over.
    ///
    /// If there is no such widget interacting the function returns nullptr.
    static GuiWidget* GetInteracted_CursorOver(int _tag){ return (interactedWidget && interactedWidget->isHovered() && interactedWidget->tag()==_tag)? interactedWidget : nullptr; }


    /// \brief Returns the interacted widget with assigned *_customObject* which has the cursor over.
    ///
    /// If there is no such widget interacting the function returns nullptr.
    static GuiWidget* GetInteracted_CursorOver(CustomObject* _customObject){ return (interactedWidget && interactedWidget->isHovered() && interactedWidget->customObject()==_customObject)? interactedWidget : nullptr; }


    //-----------------------------------------------------------------


    /// \brief Returns the interacted widget which has the cursor hold down.
    ///
    /// If there is no such widget interacting the function returns nullptr.
    static GuiWidget* GetInteracted_cursorDown(){ return (interactedWidget && interactedWidget->isPressed())? interactedWidget : nullptr; }


    /// \brief Returns the interacted widget with the given *_name* which has the cursor hold down.
    ///
    /// If there is no such widget interacting the function returns nullptr.
    static GuiWidget* GetInteracted_CursorDown(const std::string &_name){ return (interactedWidget && interactedWidget->isPressed() && interactedWidget->name()==_name)? interactedWidget : nullptr; }


    /// \brief Returns the interacted widget with the given *_tag* which has the cursor hold down.
    ///
    /// If there is no such widget interacting the function returns nullptr.
    static GuiWidget* GetInteracted_CursorDown(int _tag){ return (interactedWidget && interactedWidget->isPressed() && interactedWidget->tag()==_tag)? interactedWidget : nullptr; }


    /// \brief Returns the interacted widget with assigned *_customObject* which has the cursor hold down.
    ///
    /// If there is no such widget interacting the function returns nullptr.
    static GuiWidget* GetInteracted_CursorDown(CustomObject* _customObject){ return (interactedWidget && interactedWidget->isPressed() && interactedWidget->customObject()==_customObject)? interactedWidget : nullptr; }




    //-----------------------------------------------------------------



    static void _SetInteractedWidget(GuiWidget *_widget);
    static void ResetInteractedPerUpdate(){ interactedWidget = nullptr; }


protected:
    WidgetFactory *mFactory = nullptr;

    WidgetType mType = WidgetType::NOT_DEFINED;
    std::string mName;
    int mTag = 0;
    std::string mDesignKind;
    CustomObject* mObj = nullptr;                            // owned
    GuiWidgetCallback *mCallbackObj = nullptr;               // owned

    // state flags
    bool mDisabled = false;
    bool mVisible = true;

    BoolSignal mSigDisabled;
    BoolSignal mSigVisible;


    // interaction flags
    bool mHovered = false;
    bool mPressedStarted = false;
    bool mPressed = false;
    bool mValueChanged = false;


    Sprite *mHighlightMarkerSprite = nullptr;                // LINK

    bool mInitialized = false;

    bool mChildWidget = false;
    int mModalBlockLevel = 0;

    //---
    //std::vector<SignalCallback*>mSignalCallbacks;       // owned

    //std::vector<Signal*>mSignals;                           // LINKS



private:
    static GuiWidget *interactedWidget;                     // LINK

};



//================================================================================================



///\brief The GuiCursorDeviceInput class provides the mouse and touch input information for widget interactions.
///
/// This object is used internally by widgets.
class GuiCursorDeviceInput
{

public:

    Vec2f getCursorScreenPosition(){ return mCursorScreenPosition; }
    Vec2f getCursorInMapPosition(){ return mCursorPosition; }
    bool isPressedStarted(){ return mPressedStarted; }
    bool isPressed(){ return mPressed; }


    void _setCursorScreenPosition(Vec2f _pos){ mCursorScreenPosition = _pos; }
    void _setCursorInMapPosition(Vec2f _pos){ mCursorPosition = _pos; }
    void _setCursorPressedStarted(bool _pressedStarted);
    void _setPressed(bool _pressed){ mPressed = _pressed; }


private:
    Vec2f mCursorScreenPosition;
    Vec2f mCursorPosition;
    bool mPressedStarted = false;
    bool mPressed = false;

};


///\brief Global GuiCursorDeviceInput object used by widgets.
extern GuiCursorDeviceInput guiCursorDeviceInput;



///\brief The GuiKeyboardAndJoystickInput class provides the keyboard and joystick input information for widgets.
///
/// This object is used internally by widgets.
class GuiKeyboardAndJoystickInput
{

public:


    void deleteContent();

    void update();

    GuiWidget* highlightedWidget(){ return mHighlightedWidget; }
    bool isHighlighedPressedStarted(){ return mHighlightedPressedStarted; }
    bool isHighlighedPressed(){ return mHighlightedPressed; }
    int isHighlightedChangedValue(){ return mHighlightedChangedValue; }


    ///\brief Set the disabled state.
    ///
    /// \see IsDisabled
    void setDisabled(bool _disabled){ mDisabled = _disabled; }


    ///\brief Returns true if keyboard and joystick input is disabled; otherwise return false.
    ///
    /// \see SetDisabled
    bool isDisabled(){ return mDisabled; }


    ///\brief Set widgets which can be interacted with keyboard or joystick.
    ///
    /// The order of widgets is also the order of navigaton between them.
    void setWidgets(std::vector<GuiWidget*>_widgets, GuiWidget* _highlightedWidget=nullptr);


    ///\brief Returns a reference to the vector of widgets.
    std::vector<GuiWidget*>& widgets(){ return mWidgets; }


    ///\brief Set the keyborad keys used for forward navigation between widgets.
    void setNavigateForwardKeys(const std::vector<KeyCode> &_keys){ mNavigateForwardKeys = _keys; }


    ///\brief Set the keyborad keys used for backward navigation between widgets.
    void setNavigateBackwardKeys(const std::vector<KeyCode> &_keys){ mNavigateBackwardKeys = _keys; }


    ///\brief Set the keyborad keys used for pressing the highlighted widget.
    void setPressKeys(const std::vector<KeyCode> &_keys){ mPressKeys = _keys; }


    ///\brief Set the keyborad keys used for incrementing the value of the highlighted widget.
    ///
    /// This function is applicable to GuiSlider widgets.
    void setIncrementValueKeys(const std::vector<KeyCode> &_keys){ mIncrementValueKeys = _keys; }


    ///\brief Set the keyborad keys used for incrementing the value of the highlighted widget.
    ///
    /// This function is applicable to GuiSlider widgets.
    void setDecrementValueKeys(const std::vector<KeyCode> &_keys){ mDecrementValueKeys = _keys; }


    ///\brief Set the joystick POV X button used for forward navigation between widgets.
    void setNavigateForwardJoystickPOV_X(JoystickPOV_X _pov){ mJoyNavigateForwardPOV_X = _pov; }


    ///\brief Set the joystick POV Y button used for forward navigation between widgets.
    void setNavigateForwardJoystickPOV_Y(JoystickPOV_Y _pov){ mJoyNavigateForwardPOV_Y = _pov; }


    ///\brief Set the joystick POV X button used for backward navigation between widgets.
    void setNavigateBackwardJoystickPOV_X(JoystickPOV_X _pov){ mJoyNavigateBackwardPOV_X = _pov; }


    ///\brief Set the joystick POV Y button used for backward navigation between widgets.
    void setNavigateBackwardJoystickPOV_Y(JoystickPOV_Y _pov){ mJoyNavigateBackwardPOV_Y = _pov; }


    ///\brief Set the joystick buttons used for pressing the highlighted widget.
     void setJoystickPressButtons(const std::vector<int> &_joyPressButtons){ mJoyPressButtons = _joyPressButtons; }


    ///\brief Set the joystick POV X button for incrementing the value of the highlighted widget.
    ///
    /// This function is applicable to GuiSlider widgets.
    void setIncrementValueJoystickPOV_X(JoystickPOV_X _pov){ mJoyIncrementValue_POV_X = _pov; }


    ///\brief Set the joystick POV Y button for incrementing the value of the highlighted widget.
    ///
    /// This function is applicable to GuiSlider widgets.
    void setIncrementValueJoystickPOV_Y(JoystickPOV_Y _pov){ mJoyIncrementValue_POV_Y = _pov; }


    ///\brief Set the joystick POV X button for decrementing the value of the highlighted widget.
    ///
    /// This function is applicable to GuiSlider widgets.
    void setDecrementValueJoystickPOV_X(JoystickPOV_X _pov){ mJoyDecrementValue_POV_X = _pov; }


    ///\brief Set the joystick POV Y button for decrementing the value of the highlighted widget.
    ///
    /// This function is applicable to GuiSlider widgets.
    void setDecrementValueJoystickPOV_Y(JoystickPOV_Y _pov){ mJoyDecrementValue_POV_Y = _pov; }



    void _clearHighlightedWidget();
    void _proposeWidgetIndex(GuiWidget * _widget);


private:
    bool mDisabled = false;
    GuiWidget *mHighlightedWidget = nullptr;             // LINK
    bool mHighlightedPressedStarted = false;
    bool mHighlightedPressed = false;
    int mHighlightedChangedValue = 0;

    std::vector<GuiWidget*>mWidgets;                     // LINKs
    int mIindexHighlighted = -1;

    // keyboard navigation keys
    std::vector<KeyCode>mNavigateForwardKeys;
    std::vector<KeyCode>mNavigateBackwardKeys;
    std::vector<KeyCode>mIncrementValueKeys;
    std::vector<KeyCode>mDecrementValueKeys;
    std::vector<KeyCode>mPressKeys;

    // joystick navigation keys
    JoystickPOV_X mJoyNavigateForwardPOV_X = JoystickPOV_X::NONE;
    JoystickPOV_X mJoyNavigateBackwardPOV_X = JoystickPOV_X::NONE;
    JoystickPOV_Y mJoyNavigateForwardPOV_Y = JoystickPOV_Y::NONE;
    JoystickPOV_Y mJoyNavigateBackwardPOV_Y = JoystickPOV_Y::NONE;
    JoystickPOV_X mJoyIncrementValue_POV_X = JoystickPOV_X::NONE;
    JoystickPOV_X mJoyDecrementValue_POV_X = JoystickPOV_X::NONE;
    JoystickPOV_Y mJoyIncrementValue_POV_Y = JoystickPOV_Y::NONE;
    JoystickPOV_Y mJoyDecrementValue_POV_Y = JoystickPOV_Y::NONE;

    std::vector<int>mJoyPressButtons;



    //void _SetHighlightedPressed(bool _pressed);
    //void _SetHighlightedDown(bool _down);
    //void _SetHighlightedChangeValue(int _highlightedChangeValue);
    void _SetHighlightedNavigate(int _highlightedNavigate);

};


///\brief Global GuiKeyboardAndJoystickInput object used by widgets.
extern GuiKeyboardAndJoystickInput guiKeyboardAndJoystickInput;



//================================================================================================


/*

class WidgetManager
{
public:


    virtual ~WidgetManager();

    void deleteWidgets();

    virtual bool createWidgets(Map *_map);

    void setWidgetsToInitialState();

    void updateWidgets();

    void addWidget(GuiWidget* _widget){ mWidgets.push_back(_widget); }

    GuiWidget* findWidget(const std::string &_name);

    GuiWidget* findWidget(int _tag);

    std::vector<GuiWidget*>& widgets(){ return mWidgets; }


    void setEnabled(bool _enabled){ mEnabled = _enabled; }

    bool isEnabled(){ return mEnabled; }


    void appendToUsedWidgets(const std::vector<GuiWidget*> &_widgets);

    void removeFromUsedWidgets(const std::vector<GuiWidget*> &_widgets);

    std::vector<GuiWidget*> &usedWidgets(){ return mUsedWidgets; }

    //---
    //void appendToUsedWidgets_ModalOverlay(const std::vector<GuiWidget*> &_widgets);

    //void removeFromUsedWidgets_ModalOverlay(const std::vector<GuiWidget*> &_widgets);

    //std::vector<GuiWidget*> &usedWidgets_ModalOverlay(){ return mUsedWidgets_ModalOverlay; }

    void addModalBlockLevelToUsedWidgets();
    void removeModalBlockLevelToUsedWidgets();

    void resetInput();


private:

    std::vector<GuiWidget*>mWidgets;                            // OWNED
    bool mEnabled = true;

    std::vector<GuiWidget*>mUsedWidgets;                            // LINK
    //std::vector<GuiWidget*>mUsedWidgets_ModalOverlay;            // LINK

    Timer mDelayTimer;

};

*/

//------------------------------------------------------------------------------------

class WidgetFactory
{
public:

    virtual ~WidgetFactory(){}


    WidgetFactory(const std::string &_typeSignature, WidgetType _type) : mTypeSignature(_typeSignature), mType(_type){}

    const std::string & typeSignature(){ return mTypeSignature;}
    WidgetType type(){ return mType; }


    virtual GuiWidgetCfg* createWidgetCfg(const std::string &_name) = 0;
    virtual GuiWidget* createWidget(GuiWidgetCfg* _cfg, Sprite *_rootSprite) = 0;

protected:
    std::string mTypeSignature;
    WidgetType mType = WidgetType::NOT_DEFINED;

};





void DeleteGuiSupportingObjects();


/// @}      //end addtogroup Gui

}



#endif
