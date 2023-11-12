#ifndef JUGIMAP_GUI_BUTTON_H
#define JUGIMAP_GUI_BUTTON_H


#include "jmColor.h"
#include "jmParameter.h"

#include "jpQueries.h"
#include "jpGuiCommon.h"


namespace jugimap {


/// \addtogroup Gui
/// @{


class TextSprite;



enum class ButtonState : int
{
    UNKNOWN = -1,
    NORMAL = 0,             // keep these numbers (values from 0 - 8 are used in the vector of properties)
    NORMAL_HOVERED = 1,
    NORMAL_PRESSED = 2,
    CHECKED = 3,
    CHECKED_HOVERED = 4,
    CHECKED_PRESSED = 5,
    NORMAL_HIGHLIGHTED = 6,
    CHECKED_HIGHLIGHTED = 7,
    DISABLED = 8

};


ButtonState GetButtonStateFromString(const std::string &state);

extern std::vector<NamedValue>gButtonStateNamedValues;


enum class GuiButtonSpritePropertyKind
{

    NOT_DEFINED,
    SCALE,
    ROTATION,
    ALPHA,
    LABEL_COLOR,
    COLOR_OVERLAY,
    VISIBILITY,
    FRAME

};



struct ButtonCfg : public GuiWidgetCfg
{

    ButtonCfg(WidgetFactory *_factory, const std::string &_name) : GuiWidgetCfg(_factory,_name){}

    bool initCfg(const pugi::xml_node &_node) override;


    struct AniLoopCfg
    {
        enum class LoopMode
        {
            REVERSE,
            RESTART
        };


        std::string parNameSuffix;

        std::string def;
        std::string min;
        std::string max;
        std::string duration;
        LoopMode loopMode = LoopMode::REVERSE;

        std::string parValue;
    };


    struct SpritePropertyCfg
    {

        bool initCfg(const pugi::xml_node &_node);


        std::string sprite;
        int spriteID = 0;
        GuiButtonSpritePropertyKind property;
        std::string parNamePreffix;
        std::string statesTransitionDelta;

        std::vector<AniLoopCfg> loopCfgs;

    };


    bool checkable = false;
    ButtonState initialState = ButtonState::NORMAL;
    std::string exclusiveGroup;
    std::string statesTransitionDuration;
    //std::string highlightSprite;
    std::vector<SpritePropertyCfg> spritePropertyCfgs;


    bool setSpriteStyleParameters(Sprite *_rootSprite);


};


class GuiButtonSpritePropertyController
{
public:

    GuiButtonSpritePropertyController(Sprite *_sprite, GuiButtonSpritePropertyKind _controlledProperty) : sprite(_sprite), controlledProperty(_controlledProperty){}
    virtual ~GuiButtonSpritePropertyController(){}
    virtual void SetStartingState(ButtonState _buttonVisualState) = 0;
    virtual void Update(ButtonState _buttonVisualState) = 0;


protected:
    Sprite *sprite = nullptr;
    GuiButtonSpritePropertyKind controlledProperty = GuiButtonSpritePropertyKind::NOT_DEFINED;
    ButtonState oldButtonVisualState = ButtonState::NORMAL;

};


struct GuiButtonSpriteFloatPropertyData
{

    float value = 0.0;

    // values if looping is used
    float valueMin = 0.0;
    float valueMax = 0.0;
    int loopDurationMS = 0;

    enum class LoopType
    {
        REVERT_DIRECTION,
        RESTART
    };
    LoopType loopType = LoopType::REVERT_DIRECTION;


    //int reverseSign = 1;

    static GuiButtonSpriteFloatPropertyData ParseFromString(const std::string &text);

};


class GuiButtonSpriteFloatPropertyController : public GuiButtonSpritePropertyController
{
public:


    GuiButtonSpriteFloatPropertyController(Sprite *_sprite, GuiButtonSpritePropertyKind _controlledProperty) : GuiButtonSpritePropertyController(_sprite, _controlledProperty){}
    void ParseMouseOverTransitionData(const std::string &text);

    void SetStartingState(ButtonState _buttonVisualState) override;
    void Update(ButtonState _buttonVisualState) override;

    float GetSpritePropertyValue();
    void ApplyPropertyValueToSprite(float _value);


    std::vector<GuiButtonSpriteFloatPropertyData>propertyDataForStates = std::vector<GuiButtonSpriteFloatPropertyData>(9);      // for every button visual state
    //std::vector<std::shared_ptr<GuiButtonSpriteFloatPropertyData>>propertyDataForStates = std::vector<std::shared_ptr<GuiButtonSpriteFloatPropertyData>>(7);      // for every button visual state


private:
    float currentValue = 0.0;
    float newValue = 0.0;
    //float oldValue = 0.0;
    float targetValue = 0.0;

    int reverseSign = 1;

    bool mouseOverTransition = false;
    float mouseOverTransitionSpeed = 0.0;

    int timeCurrentMS = 0;
    int timeStartMS = 0;
    int timeEndMS = 0;

};


class GuiButtonSpriteBoolPropertyController : public GuiButtonSpritePropertyController
{
public:

    GuiButtonSpriteBoolPropertyController(Sprite *_sprite, GuiButtonSpritePropertyKind _controlledProperty) : GuiButtonSpritePropertyController(_sprite, _controlledProperty){}

    void SetStartingState(ButtonState _buttonVisualState) override;
    void Update(ButtonState _buttonVisualState) override;

    bool GetSpritePropertyValue();
    void ApplyPropertyValueToSprite(bool _value);


    std::vector<bool>propertyDataForStates = std::vector<bool>(9);      // for every button visual state

private:
    bool currentValue = false;
    bool newValue = false;
    bool oldValue =false;

};


class GuiButtonSpriteIntPropertyController : public GuiButtonSpritePropertyController
{
public:

    GuiButtonSpriteIntPropertyController(Sprite *_sprite, GuiButtonSpritePropertyKind _controlledProperty) : GuiButtonSpritePropertyController(_sprite, _controlledProperty){}

    void SetStartingState(ButtonState _buttonVisualState) override;
    void Update(ButtonState _buttonVisualState) override;

    int GetSpritePropertyValue();
    void ApplyPropertyValueToSprite(int _value);


    std::vector<int>propertyDataForStates = std::vector<int>(9);      // for every button visual state

private:
    int currentValue = 0;
    int newValue = 0;
    int oldValue = 0;

};


struct GuiButtonSpriteColorPropertyData
{

    ColorRGBAf value;

    // values if looping is used
    ColorRGBAf valueMin;
    ColorRGBAf valueMax;
    int loopDurationMS = 0;

    enum class LoopType
    {
        REVERT_DIRECTION,
        RESTART
    };
    LoopType loopType = LoopType::REVERT_DIRECTION;


    //int reverseSign = 1;

    static GuiButtonSpriteColorPropertyData ParseFromString(const std::string &text);

};


class GuiButtonSpriteColorPropertyController : public GuiButtonSpritePropertyController
{
public:


    GuiButtonSpriteColorPropertyController(Sprite *_sprite, GuiButtonSpritePropertyKind _controlledProperty) : GuiButtonSpritePropertyController(_sprite, _controlledProperty){}
    void ParseMouseOverTransitionTime(const std::string &text);
    void ParseColorOverlayBlend(std::string text);

    void SetStartingState(ButtonState _buttonVisualState) override;
    void Update(ButtonState _buttonVisualState) override;

    ColorRGBAf GetSpriteOverlayColor();
    void ApplyOverlayColorToSprite(ColorRGBAf _value);


    std::vector<GuiButtonSpriteColorPropertyData>propertyDataForStates = std::vector<GuiButtonSpriteColorPropertyData>(9);      // for every button visual state


private:
    ColorRGBAf currentValue;
    ColorRGBAf newValue;
    ColorRGBAf targetValue;

    int reverseSign = 1;

    bool mouseOverTransition = false;
    float mouseOverTransitionSpeed = 0.0;

    ColorOverlayBlend blend;


    bool reachedR = false;
    bool reachedG = false;
    bool reachedB = false;
    bool reachedA = false;


    void CheckColorReach(float &color, float targetColor, int sign, bool &reached);
    void CheckColorReach(float &color, float targetColor1, float targetColor2, int sign, bool &reached);
};


class GuiButtonExclusiveGroup;




///\brief The GuiButton class provides a widget for buttons.
class GuiButton : public GuiWidget
{
public:

    ///\brief Construct a new gui button from the given *_sprite*.
    ///
    /// The button properties are obtained from the sprite custom parameters.
    //GuiButton(WidgetFactory *_widgteFactory);
    GuiButton(ButtonCfg* _cfg, Sprite *_rootSprite);
    //GuiButton(Sprite *_sprite);
    ~GuiButton() override;

    bool initConnections(GuiSystem * _guiSystem) override;
    //bool init() override;


    void setToInitialState() override;
    void update() override;
    void setDisabled(bool _disabled) override;
    void setVisible(bool _visible) override;

    void cleanupFlags() override;


    //BoolSignal & hoveredSignal(){ return mSigHovered; }
    //BoolSignal & pressedSignal(){ return mSigPressed; }
    //BoolSignal & checkedSignal(){ return mSigChecked; }
    //BoolSignal & checkedSignal_setter(){ return mSigChecked_setter; }         // a separate signal is needed
    //IntSignal & stateSignal(){ return  mSigState; }

    //void onSetBySignalSetter(Signal *_signal) override;
    bool onSignalSet(Signal *_signal) override;

    void obtainSignal_signalQuery(SignalQuery &_signalQuery, ParsedSignalPath &_psp, bool _setErrorMessage=true) override;
    void obtainSignal_signalSetter(SignalSetter &_signalSetter, ParsedSignalPath &_psp, bool _setErrorMessage=true) override;



    ButtonCfg* cfg(){ return mCfg; }

    ///\brief Returns the sprite of this button.
    Sprite* GetSprite(){ return mRootSprite; }


    ///\brief Set the text label of this button if applicable.
    ///
    /// Text label can be applied if the button sprite is a TextSprite or a ComposedSprite which contains
    /// a child TextSprite marked as the label.
    void SetTextLabel(const std::string &_label);


    ///\brief Set the text label of this button if applicable.
    ///
    /// Text label can be applied if the button sprite is a TextSprite or a ComposedSprite which contains
    /// a child TextSprite marked as the label.
    std::string GetTextLabel();



    ///\brief Returns true if this button is checkable; otherwise returns false;
    ///
    /// A checkable button provides toggle behavior.
    /// \see SetCheckable, IsChecked, SetChecked
    bool IsCheckable(){ return mCheckable; }


    ///\brief Set checkable status of this button.
    ///
    /// A checkable button provides toggle behavior.
    /// \see IsCheckable, IsChecked, SetChecked
    void SetCheckable(bool _checkable){ mCheckable = _checkable; }


    ///\brief Returns true if this button is checked; otherwise returns false.
    ///
    /// The checked state is applicable if the button is checkable.
    /// \see SetChecked, SetCheckable, IsCheckable
    bool IsChecked(){ return mChecked;}


    ///\brief Set the checked state of this button.
    ///
    /// The checked state is applicable if the button is checkable.
    /// \see IsChecked, SetCheckable, IsCheckable
    void SetChecked(bool _checked);


    GuiButtonExclusiveGroup* GetExclusiveGroup(){ return mExclusiveGroup; }


    void setInitialState(ButtonState _initialState);

    void _setForceChecked(bool _forceChecked){ mForceChecked = _forceChecked; }



private:
    ButtonCfg* mCfg = nullptr;                                              // LINK
    Sprite *mRootSprite = nullptr;                                          // LINK
    TextSprite *mLabelSprite = nullptr;                                     // LINK

    bool mCheckable = false;
    bool mChecked = false;
    bool mForceChecked = false;
    GuiButtonExclusiveGroup* mExclusiveGroup = nullptr;                     // LINK

    std::vector<GuiButtonSpritePropertyController*>mSpriteControllers;      // OWNED
    ButtonState mState = ButtonState::NORMAL;
    ButtonState mInitialState = ButtonState::NORMAL;

    //---
    BoolSignal mSigHovered;
    BoolSignal mSigPressed;
    BoolSignal mSigChecked;
    BoolSignal mSigChecked_setter;          // a separate signal is needed
    IntSignal mSigState;





    void BuildControllersForSprite(Sprite * _sprite);
    void _AddControllersForSprite(Sprite * _sprite);
    void _AddFloatPropertyControllerIfNeeded(Sprite *_sprite, CParameters& cp, const std::string &propertyName, GuiButtonSpritePropertyKind propertyKind);
    void _AddIntPropertyControllerIfNeeded(Sprite *_sprite, CParameters& cp, const std::string &propertyName, GuiButtonSpritePropertyKind propertyKind);
    void _AddBooleanPropertyControllerIfNeeded(Sprite *_sprite, CParameters& cp, const std::string &propertyName, GuiButtonSpritePropertyKind propertyKind);
    void _AddColorPropertyControllerIfNeeded(Sprite *_sprite, CParameters& cp, const std::string &propertyName, GuiButtonSpritePropertyKind propertyKind);

    int _ParseIntPropertyFromParameter(CParameters& cp, const std::string &parameterName, GuiButtonSpritePropertyKind propertyKind);

};


class GuiButtonExclusiveGroup
{
public:

    GuiButtonExclusiveGroup(const std::string &_name) : name(_name){}

    void AddButton(GuiButton* _button);
    std::string GetName(){ return name; }
    std::vector<GuiButton*>& GetButtons(){ return buttons; }


private:
    std::string name;
    std::vector<GuiButton*> buttons;        // LINKS

};



class GuiButtonExclusiveGroups
{
public:

    ~GuiButtonExclusiveGroups();
    void deleteContent();

    GuiButtonExclusiveGroup* FindGroup(const std::string &_name);
    void AddGroup(GuiButtonExclusiveGroup *_g){ groups.push_back(_g); }


private:
    std::vector<GuiButtonExclusiveGroup*> groups;        // OWNED
};


extern GuiButtonExclusiveGroups guiButtonExclusiveGroups;


//------------------------------------------------------------------------------------------

/*
class SetCheckedButtonSignalCallback : public SignalCallback
{
public:

    SetCheckedButtonSignalCallback(GuiButton* _button) : SignalCallback(SignalCallback::CallFrom::SIGNAL_SETTER_SET), button(_button){}

    void onSetBySignalSetter(BoolSignal *_signal, bool _value) override;

private:
    GuiButton* button = nullptr;        //LINK
};
*/


//------------------------------------------------------------------------------------------


class ButtonFactory : public WidgetFactory
{
public:

    ButtonFactory(const std::string &_typeSignature, WidgetType _type) : WidgetFactory(_typeSignature, _type){}

    GuiWidgetCfg* createWidgetCfg(const std::string &_name) override;
    GuiWidget* createWidget(GuiWidgetCfg* _cfg, Sprite *_rootSprite) override;


};




/// @}

}



#endif
