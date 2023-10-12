#ifndef JUGIMAP_GUI_BUTTON_H
#define JUGIMAP_GUI_BUTTON_H


#include "jmColor.h"
#include "jmParameter.h"
#include "jmGuiCommon.h"


namespace jugimap {


/// \addtogroup Gui
/// @{


class TextSprite;


enum class GuiButtonVisualState
{
    NORMAL = 0,
    NORMAL_CURSOR_OVER = 1,
    NORMAL_CURSOR_DOWN = 2,
    CHECKED = 3,
    CHECKED_CURSOR_OVER = 4,
    CHECKED_CURSOR_DOWN = 5,
    NORMAL_HIGHLIGHTED = 6,
    CHECKED_HIGHLIGHTED = 7,
    DISABLED = 8,
};


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


class GuiButtonSpritePropertyController
{
public:

    GuiButtonSpritePropertyController(Sprite *_sprite, GuiButtonSpritePropertyKind _controlledProperty) : sprite(_sprite), controlledProperty(_controlledProperty){}
    virtual ~GuiButtonSpritePropertyController(){}
    virtual void SetStartingState(GuiButtonVisualState _buttonVisualState) = 0;
    virtual void Update(GuiButtonVisualState _buttonVisualState) = 0;


protected:
    Sprite *sprite = nullptr;
    GuiButtonSpritePropertyKind controlledProperty = GuiButtonSpritePropertyKind::NOT_DEFINED;
    GuiButtonVisualState oldButtonVisualState = GuiButtonVisualState::NORMAL;

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

    void SetStartingState(GuiButtonVisualState _buttonVisualState) override;
    void Update(GuiButtonVisualState _buttonVisualState) override;

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

    void SetStartingState(GuiButtonVisualState _buttonVisualState) override;
    void Update(GuiButtonVisualState _buttonVisualState) override;

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

    void SetStartingState(GuiButtonVisualState _buttonVisualState) override;
    void Update(GuiButtonVisualState _buttonVisualState) override;

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

    void SetStartingState(GuiButtonVisualState _buttonVisualState) override;
    void Update(GuiButtonVisualState _buttonVisualState) override;

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
    GuiButton(Sprite *_sprite);
    ~GuiButton() override;

    bool init() override;

    void SetToInitialState() override;
    void Update() override;
    void SetDisabled(bool _disabled) override;
    void SetVisible(bool _visible) override;

    void CleanupFlags() override;



    ///\brief Returns the sprite of this button.
    Sprite* GetSprite(){ return rootSprite; }


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
    bool IsCheckable(){ return checkable; }


    ///\brief Set checkable status of this button.
    ///
    /// A checkable button provides toggle behavior.
    /// \see IsCheckable, IsChecked, SetChecked
    void SetCheckable(bool _checkable){ checkable = _checkable; }


    ///\brief Returns true if this button is checked; otherwise returns false.
    ///
    /// The checked state is applicable if the button is checkable.
    /// \see SetChecked, SetCheckable, IsCheckable
    bool IsChecked(){ return checked;}


    ///\brief Set the checked state of this button.
    ///
    /// The checked state is applicable if the button is checkable.
    /// \see IsChecked, SetCheckable, IsCheckable
    void SetChecked(bool _checked);


    GuiButtonExclusiveGroup* GetExclusiveGroup(){ return exclusiveGroup; }


    void setInitialState(GuiButtonVisualState _initialState);

    void _setForceChecked(bool _forceChecked){ mForceChecked = _forceChecked; }



private:
    Sprite *rootSprite = nullptr;                                       // LINK
    TextSprite *labelSprite = nullptr;                                  // LINK
    GuiButtonVisualState state = GuiButtonVisualState::NORMAL;
    GuiButtonVisualState initialState = GuiButtonVisualState::NORMAL;
    bool checkable = false;
    bool checked = false;

    std::vector<GuiButtonSpritePropertyController*>spriteControllers;     // OWNED

    GuiButtonExclusiveGroup* exclusiveGroup = nullptr;                       // LINK

    bool mForceChecked = false;


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



/// @}

}



#endif
