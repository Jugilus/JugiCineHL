#include <sstream>
#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmSourceSprite.h"
#include "jmVectorShape.h"
#include "jmTextSprite.h"
#include "jmCompositeSprite.h"
#include "jmStandardSprite.h"
#include "jmInput.h"
#include "jmMap.h"
#include "jmSpriteLayer.h"
#include "jmVectorLayer.h"
#include "jmUtilities.h"

#include "jpGuiCommon.h"
#include "jpGuiBar.h"




namespace jugimap {




bool BarCfg::initCfg(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading widget of type '" + factory->typeSignature() + "' and name '" + name + "'...");


    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());

        if(nodeName=="parameters"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="min"){
                    min = a.as_float(min);

                }else if(attributeName=="max"){
                    max = a.as_float(max);

                }else if(attributeName=="default"){
                    def = a.as_float(def);

                }else{

                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                    return false;
                }
            }

        }else if(nodeName=="style"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="sprite"){
                    sprite = a.as_string();


                }else if(attributeName=="property"){
                    std::string sPropertyController = a.as_string();

                    if(sPropertyController=="X_SCALE"){
                        propertyKind = PropertyKind::X_SCALE;

                    }else if(sPropertyController=="X_SCALE"){
                        propertyKind = PropertyKind::Y_SCALE;

                    }else if(sPropertyController=="FRAME"){
                        propertyKind = PropertyKind::FRAME;

                    }else{
                        dbgSystem.addMessage("Wrong property '" + sPropertyController + "' !");
                        return false;
                    }


                }else if(attributeName=="positionColors"){
                    std::string sColors = a.as_string();
                    std::vector<std::string>parts = StdString::splitString(sColors, ",");

                    for(int i=0; i<int(parts.size())-1; i+=2){
                        if(i+1 >= int(parts.size())){
                            break;
                        }
                        std::string sHexColor  =   parts[i];
                        StdString::removeStringWhiteSpacesOnStartAndEnd(sHexColor);
                        std::string sColorPos = parts[i+1];

                        PositionColor pc;
                        pc.color = ColorRGBA::ParseFromHexString(sHexColor);
                        pc.pos = StdString::stringToFloat(sColorPos, 0.0f);

                        positionColors.push_back(pc);
                    }

                }else{
                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                    return false;
                }
            }


        }else if(nodeName=="label"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="textSprite"){
                    labelSprite = a.as_string();

                }else if(attributeName=="displayedPrecision"){
                    labelPrecision = a.as_int(labelPrecision);

                }else if(attributeName=="suffix"){
                    labelSuffix = a.as_string();

                }else{
                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                    return false;
                }
            }


        }else{
            dbgSystem.addMessage("Unknown node '" + nodeName + "' !");
            return false;
        }
    }


    dbgSystem.removeLastMessage();
    return true;
}


// --------------------------------------------------------------------------------------------


GuiBarSpriteScaleController::GuiBarSpriteScaleController(Sprite *_sprite, BarCfg* _cfg) : GuiBarSpriteController(_sprite)
{

    if(_cfg->propertyKind==PropertyKind::X_SCALE){
        xScale = true;

    }else if(_cfg->propertyKind==PropertyKind::Y_SCALE){
        yScale = true;

    }

    colors = _cfg->positionColors;

}



void GuiBarSpriteScaleController::ParseFromString(const std::string &_text)
{

    std::vector<std::string>values = StdString::splitString(_text, ",");

    if(values.size()>=1){

        std::string sValue = values[0];
        StdString::removeStringWhiteSpacesOnStartAndEnd(sValue);

        if(sValue=="x"){
            xScale = true;

        }else if(sValue=="y"){
            yScale = true;

        }else if(sValue=="xy"){
            xScale = true;
            yScale = true;
        }
    }

    for(int i=1; i<int(values.size())-1; i+=2){

        std::string sHexColor  =   values[i];
        StdString::removeStringWhiteSpacesOnStartAndEnd(sHexColor);
        std::string sColorPos = values[i+1];

        PositionColor pc;
        pc.color = ColorRGBA::ParseFromHexString(sHexColor);
        pc.pos = std::stof(sColorPos);

        colors.push_back(pc);
    }

    if(colors.empty()==false){
        sprite->setColorOverlayBlend(ColorOverlayBlend::SIMPLE_MULTIPLY);
        sprite->setColorOverlayActive(true);
    }

}


void GuiBarSpriteScaleController::Set(float value, float valueMin, float valueMax)
{

    value = ClampValue(value, valueMin, valueMax);

    float scale = (value - valueMin) / (valueMax-valueMin);

    if(xScale){
        if(AreEqual(scale, sprite->scale().x)==false){
            sprite->setScale(Vec2f(scale, sprite->scale().y));
        }
    }

    if(yScale){
        if(AreEqual(scale, sprite->scale().y)==false){
            sprite->setScale(Vec2f(sprite->scale().x, scale));
        }
    }

    //----
    if(colors.empty()==false){

        ColorRGBA color;

        for(int i=0; i<int(colors.size()); i++){

            if(i+1 < int(colors.size())){

                if(scale>=colors[i].pos && scale<=colors[i+1].pos){
                    ColorRGBA c1 = colors[i].color;
                    ColorRGBA c2 = colors[i+1].color;
                    float f = (scale-colors[i].pos) / (colors[i+1].pos - colors[i].pos);

                    color.r = c1.r * (1.0f - f) + c2.r * f;
                    color.g = c1.g * (1.0f - f) + c2.g * f;
                    color.b = c1.b * (1.0f - f) + c2.b * f;
                    color.a = c1.a * (1.0f - f) + c2.a * f;

                    break;
                }


            }else{
                color = colors[i].color;

                break;
            }
        }


        if(color != sprite->overlayColorRGBA()){
            sprite->setColorOverlayRGBA(color);
        }
    }

}


//-------------------------------------------------------------------------------------------------------


void GuiBarSpriteFrameController::ParseFromString(const std::string &_text)
{

    std::vector<std::string>values = StdString::splitString(_text, ",");

    if(values.size()>=1){

        std::string sValue = values[0];

    }

}


void GuiBarSpriteFrameController::Set(float value, float valueMin, float valueMax)
{

    value = ClampValue(value, valueMin, valueMax);

    float ratio = (value - valueMin) / (valueMax-valueMin);

    int nFrames = sprite->sourceSprite()->sourceImages().size();
    float frameF = ratio * nFrames;
    int frameIndex = (int)frameF;

    frameIndex = ClampValue(frameIndex, 0, nFrames-1);

    if(frameIndex != static_cast<StandardSprite*>(sprite)->activeImageFrameIndex()){
        static_cast<StandardSprite*>(sprite)->setActiveImageByFrameIndex(frameIndex);
    }

}



//-------------------------------------------------------------------------------------------------------


GuiBar::GuiBar(BarCfg *_cfg, Sprite *_sprite)
{

    mType = WidgetType::BAR;
    mCfg = _cfg;
    mSprite = _sprite;

}


/*
GuiBar::GuiBar(Sprite *_sprite)
{
    mType = WidgetType::BAR;
    mSprite = _sprite;
}
*/

GuiBar::~GuiBar()
{

    if(mBarSpriteController) delete mBarSpriteController;

}


bool GuiBar::initConnections(GuiSystem *_guiSystem)
{

    dbgSystem.addMessage("Init connections for the widget '" + mCfg->name + "' ...");


    mName = mSprite->parameters().value("wName");
    if(mName=="" && mChildWidget==false){
        dbgSystem.addMessage("Missing parameter 'wName' (identification name of the widget) !");
    }


    //--- values
    mValueMin = mCfg->min;
    mValueMax = mCfg->max;
    if(mValueMin>mValueMax){
        std::swap(mValueMin, mValueMax);
    }

    mValue = mCfg->def;
    if(mSprite->parameters().exists("wValue")){
        mValue = mSprite->parameters().floatValue("wValue");
    }

    mValue = ClampValue(mValue, mValueMin, mValueMax);


    //--- label
    if(mCfg->labelSprite.empty()==false){

        if(mSprite->type()!=SpriteType::COMPOSITE){
            dbgSystem.addMessage("Label text sprite is specifed but the bar sprite is not a composite sprite!");
        }

        std::vector<Sprite*>sprites;
        CollectSpritesWithName(static_cast<CompositeSprite*>(mSprite)->map(), sprites, mCfg->labelSprite);

        if(sprites.empty()){
            dbgSystem.addMessage("Label text sprite '" + mCfg->labelSprite +"' not found!");
            return false;
        }
        if(sprites.front()->type() != SpriteType::TEXT){
            dbgSystem.addMessage("Label text sprite '" + mCfg->labelSprite +"' is not a text sprite!");
            return false;
        }

        mValueLabel = static_cast<TextSprite*>(sprites.front());
        mValueLabelPrecision = mCfg->labelPrecision;
        mValueLabelSuffix = mCfg->labelSuffix;
        StdString::removeStringWhiteSpacesOnStartAndEnd(mValueLabelSuffix);
    }


    //--- style property controller
    BuildControllersForSprite(mSprite);


    mInitialized = true;

    //---
    dbgSystem.removeLastMessage();

    return true;

}


/*
bool GuiBar::init()
{

    mName = mSprite->parameters().value("wName");

    dbgSystem.addMessage("init bar '" + mName + "'");

    mTag = mSprite->parameters().intValue("wTag");
    mDesignKind = mSprite->sourceSprite()->constantParameters().value("wDesign");


    //--- value range
    std::string valueRange = mSprite->sourceSprite()->constantParameters().value("wValueRange");
    std::vector<std::string>values = StdString::splitString(valueRange, ",");

    if(values.size()>=2){
        mValueMin = std::stof(values[0]);
        mValueMax = std::stof(values[1]);
        if(mValueMin>mValueMax){
            std::swap(mValueMin, mValueMax);
        }
        mValue = (mValueMin+mValueMax)/2.0;
    }

    //--- value
    if(mSprite->parameters().exists("wValue")){
        mValue = mSprite->parameters().floatValue("wValue");
    }
    mValue = ClampValue(mValue, mValueMin, mValueMax);


    //--- value label
    if(mSprite->type()==SpriteType::COMPOSITE){

        std::vector<Sprite*>sprites;
        //CollectSpritesWithOrigConstParameter(static_cast<CompositeSprite*>(sprite), sprites, "wLabel");
        CollectSpritesWithConstParameter(static_cast<CompositeSprite*>(mSprite), sprites, "wLabel");
        if(sprites.empty()==false && sprites.front()->type()==SpriteType::TEXT){

            mValueLabel = static_cast<TextSprite*>(sprites.front());
            mValueLabelPrecision = 0;
            mValueLabelSuffix = "";

            std::string sliderValueLabel = mValueLabel->sourceSprite()->constantParameters().value("wLabel");
            if(sliderValueLabel!=""){
                values = StdString::splitString(sliderValueLabel, ",");

                if(values.size()==1){
                    mValueLabelPrecision = (int)std::stof(values[0]);

                }else if(values.size()==2){
                    mValueLabelPrecision = (int)std::stof(values[0]);
                    mValueLabelSuffix = values[1];
                    StdString::removeStringWhiteSpacesOnStartAndEnd(mValueLabelSuffix);
                }
            }
        }
    }


    BuildControllersForSprite(mSprite);


    mInitialized = true;

    //---
    dbgSystem.removeLastMessage();

    return true;
}
*/


void GuiBar::BuildControllersForSprite(Sprite * _sprite)
{

    if(mBarSpriteController) return;

    //if(_sprite->GetKind()==SpriteKind::STANDARD){

        _AddControllersForSprite(_sprite);


    //}else
    if(_sprite->type()==SpriteType::COMPOSITE){

        CompositeSprite *cs = static_cast<CompositeSprite*>(_sprite);

        for(SpriteLayer *sl : cs->map()->spriteLayers()){
            for(Sprite *s : sl->sprites()){


        //for(ncine::SceneNode *n : cs->node()->children()){
        //    LayerElement *l = static_cast<LayerNode*>(n)->layerElement;

        //    if(l->kind()==LayerKind::SPRITE_LAYER){
        //        SpriteLayer *sl = static_cast<SpriteLayer*>(l);
        //        for(ncine::SceneNode *n : sl->node()->children()){
        //            Sprite *s = castNodeToSprite(n);

                    _AddControllersForSprite(s);

                    if(s->type()==SpriteType::COMPOSITE){
                        BuildControllersForSprite(s);
                    }
                //}
            }
        }
    }
}


void GuiBar::_AddControllersForSprite(Sprite * _sprite)
{

    if(mBarSpriteController) return;


    if(mCfg){

        if(mCfg->propertyKind==PropertyKind::X_SCALE || mCfg->propertyKind==PropertyKind::Y_SCALE){
            mBarSpriteController = new GuiBarSpriteScaleController(_sprite, mCfg);

        }else if(mCfg->propertyKind==PropertyKind::FRAME){
            mBarSpriteController = new GuiBarSpriteFrameController(_sprite);

        }


    }else{

        CParameters& cp = _sprite->sourceSprite()->constantParameters();

        if(cp.exists("wScale")){

            mBarSpriteController = new GuiBarSpriteScaleController(_sprite);
            static_cast<GuiBarSpriteScaleController*>(mBarSpriteController)->ParseFromString(cp.value("wScale"));

        }else if(cp.exists("wFrame") && _sprite->type()==SpriteType::STANDARD){

            mBarSpriteController = new GuiBarSpriteFrameController(_sprite);
            //static_cast<GuiBarSpriteScaleController*>(barSpriteController)->ParseFromString(Parameter::Value(cp, "scale"));

        }
    }

}


void GuiBar::setToInitialState()
{
    SetValue(mValue);
}


void GuiBar::SetValue(float _value)
{

    mValue = _value;

    if(mBarSpriteController){
        mBarSpriteController->Set(mValue, mValueMin, mValueMax);
    }
    SetValueLabelText();

}


void GuiBar::SetValueMin(float _valueMin)
{
    mValueMin = _valueMin;
    if(mBarSpriteController){
        mBarSpriteController->Set(mValue, mValueMin, mValueMax);
    }
}


void GuiBar::SetValueMax(float _valueMax)
{
    mValueMax = _valueMax;
    if(mBarSpriteController){
        mBarSpriteController->Set(mValue, mValueMin, mValueMax);
    }
}


void GuiBar::update()
{

    assert(mInitialized);

    if(mBarSpriteController==nullptr) return;

}


/*
void GuiBar::SetValueFromBarSpriteController()
{
    if(barSpriteController){
        value = barSpriteController->GetValue(valueMin, valueMax);
    }
}
*/


void GuiBar::SetValueLabelText()
{

    if(mValueLabel==nullptr) return;

    //std::string valueText = std::to_string(value);

    std::ostringstream out;
    out.precision(mValueLabelPrecision);
    out << std::fixed << mValue;
    std::string valueText = out.str();
    //DbgOutput("SetValueText value: " + std::to_string(value));

    print("SetValueText value: " + std::to_string(GetIntValue()));


    if(mValueLabelSuffix != ""){
        valueText += " " + mValueLabelSuffix;
    }

    if(valueText != mValueLabel->text()){
        mValueLabel->setText(valueText);
    }


}

//========================================================================================


GuiWidgetCfg* BarFactory::createWidgetCfg(const std::string &_name)
{
    return new BarCfg(this, _name);
}


GuiWidget* BarFactory::createWidget(GuiWidgetCfg *_cfg, Sprite *_rootSprite)
{
    assert(_cfg->factory==this);

    //if(_rootSprite->type()!=SpriteType::COMPOSITE){
    //    dbgSystem.addMessage("Creating widget '" + _cfg->name + " of type '" + _cfg->factory->typeSignature() +"' from sprite '"
    //                         + _rootSprite->sourceSprite()->name() +"' error !");
    //    dbgSystem.addMessage("The sprite must be a composite sprite!'");
    //    return nullptr;
    //}

    return new GuiBar(static_cast<BarCfg*>(_cfg), _rootSprite);
}


}
