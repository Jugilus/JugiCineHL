#ifndef PLAYER__GUI_BAR_H
#define PLAYER__GUI_BAR_H


#include "jmColor.h"
#include "jpGuiCommon.h"


namespace jugimap {


/// \addtogroup Gui
/// @{


class TextSprite;


struct PositionColor
{
    float pos = 0.0;        // relative position in bar 0.0 - 1.0
    ColorRGBA color;
};


enum class PropertyKind
{
    FRAME,
    X_SCALE,
    Y_SCALE

};


struct BarCfg : public GuiWidgetCfg
{

    BarCfg(WidgetFactory *_factory, const std::string &_name) : GuiWidgetCfg(_factory,_name){}

    bool initCfg(const pugi::xml_node &_node) override;


    float def = 0.0;
    float min = 0.0f;
    float max = 1.0f;

    std::string sprite;
    PropertyKind propertyKind = PropertyKind::X_SCALE;
    std::vector<PositionColor>positionColors;

    std::string labelSprite;
    int labelPrecision = 0;
    std::string labelSuffix;

};



class GuiBarSpriteController
{
public:

    GuiBarSpriteController(Sprite *_sprite) : sprite(_sprite){}
    virtual ~GuiBarSpriteController(){}


    virtual void Set(float value, float valueMin, float valueMax) = 0;
    //virtual float GetValue(float valueMin, float valueMax) = 0;

protected:
    Sprite *sprite = nullptr;       // LINK

};


class GuiBarSpriteScaleController : public GuiBarSpriteController
{
public:

    GuiBarSpriteScaleController(Sprite * _sprite) : GuiBarSpriteController(_sprite){}
    GuiBarSpriteScaleController(Sprite *_sprite, BarCfg* _cfg);
    void ParseFromString(const std::string &_text);

    void Set(float value, float valueMin, float valueMax) override;
    //float GetValue(float valueMin, float valueMax) override;


private:
    bool xScale = false;
    bool yScale = false;
    std::vector<PositionColor>colors;
};


class GuiBarSpriteFrameController : public GuiBarSpriteController
{
public:

    GuiBarSpriteFrameController(Sprite * _sprite) : GuiBarSpriteController(_sprite){}
    void ParseFromString(const std::string &_text);

    void Set(float value, float valueMin, float valueMax) override;


private:
    int frameIndexMin = 0;
    int frameIndexMax = 0;
};



///\brief The GuiBar class provides a widget for bars.
///
/// A bar is a visual representation of values in range.
class GuiBar : public GuiWidget
{

public:

    ///\brief Construct a new gui bar from the given *_sprite*.
    ///
    /// The bar properties are obtained from the sprite custom parameters.
    GuiBar(BarCfg *_cfg, Sprite* _sprite);

    //GuiBar(Sprite* _sprite);

    ~GuiBar();

    bool initConnections(GuiSystem *_guiSystem) override;

    //bool init() override;



    void setToInitialState() override;
    void update() override;


    BarCfg* cfg(){ return mCfg; }


    ///\brief Set the value of this bar.
    void SetValue(float _value);


    ///\brief Returns the value of this bar.
    float GetValue(){ return mValue; }


    ///\brief Returns the value of this bar rounded to integer.
    int GetIntValue(){ return (int) std::roundf(mValue); }


    ///\brief Set the minimum value of this bar.
    void SetValueMin(float _valueMin);


    ///\brief Returns the minimum value of this bar.
    float GetValueMin(){ return mValueMin; }


    ///\brief Set the maximum value of this bar.
    void SetValueMax(float _valueMax);


    ///\brief Returns the maximum value of this bar.
    float GetValueMax(){ return mValueMax; }


private:
    BarCfg *mCfg = nullptr;                     //LINK
    Sprite *mSprite = nullptr;                   // LINK
    TextSprite *mValueLabel = nullptr;                           // LINK

    float mValue = 5.0;
    float mValueMin = 0.0;
    float mValueMax = 10.0;

    GuiBarSpriteController *mBarSpriteController = nullptr;      //OWNED

    int mValueLabelPrecision = 0;
    std::string mValueLabelSuffix;


    void BuildControllersForSprite(Sprite * _sprite);
    void _AddControllersForSprite(Sprite * _sprite);

    void SetValueLabelText();

};




//------------------------------------------------------------------------------------------


class BarFactory : public WidgetFactory
{
public:

    BarFactory(const std::string &_typeSignature, WidgetType _type) : WidgetFactory(_typeSignature, _type){}

    GuiWidgetCfg* createWidgetCfg(const std::string &_name) override;
    GuiWidget* createWidget(GuiWidgetCfg* _cfg, Sprite *_rootSprite) override;

};



/// @}

}



#endif
