#ifndef JUGIMAP_GUI_SLIDER_H
#define JUGIMAP_GUI_SLIDER_H


#include "jmGuiCommon.h"


namespace jugimap {


/// \addtogroup Gui
/// @{



class TextSprite;
class CompositeSprite;
class StandardSprite;
class GuiButton;



enum class GuiSliderState
{
    IDLE = 0,
    MOVING = 1,
    //HIGHLIGHTED = 2,
    DISABLED = 3
};


///\brief The GuiSlider class provides a widget for sliders.
class GuiSlider : public GuiWidget
{
public:

    ///\brief Construct a new gui slider from the given *_sprite*.
    ///
    /// The slider properties are obtained from the sprite custom parameters.
    GuiSlider(CompositeSprite * _sprite);
    ~GuiSlider() override;

    bool init() override;

    void SetToInitialState() override;
    void Update() override;
    void SetDisabled(bool _disabled) override;
    void SetVisible(bool _visible) override;


    ///\brief Returns true is the value has been changed with increment or decrement button.
    bool IsValueChangedViaStepButtons(){ return mValueChangedViaStepButtons;}


    ///\brief Set the value of this slider.
    void SetValue(float _value);


    ///\brief Returns the value of this slider.
    float GetValue(){ return mValue; }


    ///\brief Returns the value of this slider rounded to integer.
    int GetIntValue(){ return (int) std::roundf(mValue); }


    ///\brief Set the minimum value of this slider.
    void SetValueMin(float _valueMin);


    ///\brief Returns the minimum value of this slider.
    float GetValueMin(){ return mValueMin; }


    ///\brief Set the maximum value of this slider.
    void SetValueMax(float _valueMax);


    ///\brief Returns the maximum value of this slider.
    float GetValueMax(){ return mValueMax; }


    ///\brief Set the displayed label precision (number of decimal digits) of this slider.
    void SetValueLabelPrecision(int _precision){ mValueLabelPrecision = _precision; }


    ///\brief Set the label suffix of this slider.
    void SetValueLabelSuffix(const std::string &_suffix){ mValueLabelSuffix = _suffix; }


    ///\brief Set the delta value step fot the incrementing and decrementing buttons.
    ///
    /// If the *_inPercents* is set to true, the *_spinStep* must be a percentage number from 0 to 100
    /// in which case the actual step will be obtained from the slider range.
    void SetDeltaValue(float _deltaValue, bool _inPercents);


    void setDeltaValueInPercents(bool _deltaValueInPercents){ mDeltaValueInPercents = _deltaValueInPercents;}


    ///\brief Set the snap division.
    ///
    /// A slider with snap division will be constrained to snapping point along the scale.
    /// The snap division must be larger then 1. To disable snapping set the snap division to 0.
    void SetSnapDivision(int _nSnapDivision);

    //---
    GuiButton* slidingButton(){ return mSlidingButton; }



private:

    // link pointers
    CompositeSprite *mRootSprite = nullptr;                           // LINK
    StandardSprite *mSlidingScaleSprite = nullptr;               // LINK
    TextSprite *mValueLabel = nullptr;                           // LINK

    // owned data
    GuiButton *mSlidingButton = nullptr;                         // OWNED
    GuiButton *mIncrementButton = nullptr;                       // OWNED
    GuiButton *mDecrementButton = nullptr;                       // OWNED


    GuiSliderState mState = GuiSliderState::IDLE;

    // status flags
    bool mValueChangedViaStepButtons = false;

    // data properties
    float mValue = 5.0;
    float mValueMin = 0.0;
    float mValueMax = 1.0;

    float mDeltaValue = 0.05;
    bool mDeltaValueInPercents = true;

    int mNumSnapDivisions = -1;
    std::vector<Vec2f>mSnapPositions;

    Vec2f mSlidingPosMin;
    Vec2f mSlidingPosMax;
    Vec2f mVirtualSlidingPos;

    Vec2f mStartMovingOffset;

    int mValueLabelPrecision = 0;
    std::string mValueLabelSuffix;




    void SetSlidingButtonPositionFromValues();
    void SetValueFromSlidingButtonPosition();
    void SetValueLabelText();
    Vec2f GetSnappedPosition(Vec2f _pos);


};



/// @}

}



#endif
