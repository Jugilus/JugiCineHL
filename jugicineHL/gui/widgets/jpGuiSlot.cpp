#include <assert.h>
#include <algorithm>
#include <cmath>
#include <vector>
#include <array>
#include <fstream>
#include <string>
#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmCommonFunctions.h"
#include "jmInput.h"
#include "jmStreams.h"
#include "jmFont.h"
#include "jmUtilities.h"
#include "jmVectorShape.h"
#include "jmTextSprite.h"
#include "jmCompositeSprite.h"
#include "jmStandardSprite.h"
#include "jmSourceSprite.h"
#include "jmMap.h"
#include "jmSpriteLayer.h"
#include "jmText.h"

#include "gui/jpGuiSystem.h"
#include "gui/widgets/jpGuiBar.h"

#include "jpObjectFactory.h"
#include "jpLogicState.h"
#include "jpUtilities.h"

#include "data/jpData.h"
#include "data/jpItem.h"

#include "jpGuiButton.h"
#include "jpGuiTextField.h"
#include "jpGuiTableNew.h"
#include "jpGuiSlot.h"


namespace jugimap{






bool GuiSlotNewCfg::initCfg(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading widget of type '" + factory->typeSignature() + "' and name '" + name + "'...");


    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());

        if(nodeName=="selectingButton"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="button"){
                    selectingButton = a.as_string();

                }else if(attributeName=="buttonSprite"){
                    selectingButtonSprite = a.as_string();

                }else{

                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                    return false;
                }
            }

        }else if(nodeName=="components"){

            for(pugi::xml_node nChild = n.first_child(); nChild; nChild = nChild.next_sibling()){
                std::string childNodeName = std::string(nChild.name());

                if(childNodeName=="spritePosMarker"){

                    slotSpritePosMarkerCfgs.emplace_back();
                    GuiSlotSpritePosMarkerCfg &cfg = slotSpritePosMarkerCfgs.back();

                    for(pugi::xml_attribute a = nChild.first_attribute(); a; a = a.next_attribute()){
                        std::string attributeName =std::string(a.name());

                        if(attributeName=="pointShape"){
                            cfg.pointShape = a.as_string();

                        }else if(attributeName=="layer"){
                            cfg.layer = a.as_string();

                        }else if(attributeName=="itemSpriteData"){
                            cfg.itemSpriteData = a.as_string();

                        }else{

                            dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                            return false;
                        }
                    }


                }else if(childNodeName=="textSprite"){

                    slotTextSpriteCfgs.emplace_back();
                    GuiSlotTextSpriteCfg &cfg = slotTextSpriteCfgs.back();

                    for(pugi::xml_attribute a = nChild.first_attribute(); a; a = a.next_attribute()){
                        std::string attributeName =std::string(a.name());

                        if(attributeName=="sprite"){
                            cfg.sprite = a.as_string();

                        }else if(attributeName=="itemData"){
                            cfg.itemData = a.as_string();

                        }else if(attributeName=="numDigits"){
                            std::string sValue = a.as_string();
                            int numDigits = 0;
                            if(StdString::integerNumber(sValue, numDigits)==false){
                                return false;
                            }
                            for(int i=0; i<numDigits; i++){
                                cfg.digitsWithZero += "0";
                            }

                        }else{

                            dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                            return false;
                        }
                    }


                }else if(childNodeName=="textField"){

                    slotTextFieldCfgs.emplace_back();
                    GuiSlotTextFieldCfg &cfg = slotTextFieldCfgs.back();

                    for(pugi::xml_attribute a = nChild.first_attribute(); a; a = a.next_attribute()){
                        std::string attributeName =std::string(a.name());

                        if(attributeName=="sprite"){
                            cfg.textField = a.as_string();

                        }else if(attributeName=="itemTextSegmentData"){
                            cfg.itemTextSegmentData = a.as_string();

                        }else{

                            dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                            return false;
                        }
                    }

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


//===================================================================================



GuiSlotNew::GuiSlotNew(GuiSlotNewCfg *_cfg, CompositeSprite *_rootSprite) :
    mCfg(_cfg),
    mRootSprite(_rootSprite)
{

}


GuiSlotNew::GuiSlotNew(GuiSlotNewCfg *_cfg, CompositeSprite *_rootSprite, GuiTableNew *_parentTable) :
    mCfg(_cfg),
    mRootSprite(_rootSprite),
    mParentTable(_parentTable)
{

}


GuiSlotNew::~GuiSlotNew()
{

    //for(std::pair<std::string, GuiTextField*>& p : mIcTextFields){
    //    GuiTextField * tf = p.second;
    //    delete tf;
    //}
    for(GuiSlotComponent *c : mComponents){
        delete c;
    }

    if(mSelectingButton){
        delete mSelectingButton;
    }

}


bool GuiSlotNew::initConnections(GuiSystem *_guiSystem)
{

    dbgSystem.addMessage("Initialize connections for the widget '" + mCfg->name + "' ...");


    mRootMap = mRootSprite->parentLayer()->rootMap();

    mName = mRootSprite->parameters().value("wName");
    if(mName=="" && mChildWidget==false){
        dbgSystem.addMessage("Missing parameter 'wName' (identification name of the widget) !");
    }


    if(mCfg->selectingButton.empty()==false && mCfg->selectingButtonSprite.empty()==false){

        Sprite *buttonSprite = nullptr;

        if(mRootSprite->name() == mCfg->selectingButtonSprite){
            buttonSprite = mRootSprite;

        }else{
            std::vector<Sprite*>sprites;
            CollectSpritesWithName(mRootSprite, sprites, mCfg->selectingButtonSprite);
            if(sprites.empty()){
                dbgSystem.addMessage("Sliding button sprite '" + mCfg->selectingButtonSprite +"' not found!");
                return false;
            }
            buttonSprite = sprites.front();
        }

        ButtonCfg * buttonCfg = dynamic_cast<ButtonCfg *>(_guiSystem->widgetsManager()->getWidgetCfg(mCfg->selectingButton));
        if(buttonCfg==nullptr){
            dbgSystem.addMessage("Sliding button '" + mCfg->selectingButton +"' not found!");
            return false;
        }

        mSelectingButton = new GuiButton(buttonCfg, buttonSprite);
        mSelectingButton->SetCheckable(true);           // just in case it wasn't set in button cfg
        mSelectingButton->setChildWidget(true);
        if(mSelectingButton->initConnections(_guiSystem)==false){
            return false;
        }
        mSelectingButton->setName("selectingButton");

        mSelectingButton->assignCallback(new SelectingSlotNewCallback(this));
        mSelectingButton->_setForceChecked(true);

    }


    for(GuiSlotSpritePosMarkerCfg &cfg : mCfg->slotSpritePosMarkerCfgs){

        GuiSlotSpritePosMarker *c = new GuiSlotSpritePosMarker(&cfg, this);
        if(c->initConnections(_guiSystem->parentPlayerScene())==false){
            return false;
        }
        mComponents.push_back(c);
    }

    for(GuiSlotTextSpriteCfg &cfg : mCfg->slotTextSpriteCfgs){

        GuiSlotTextSprite *c = new GuiSlotTextSprite(&cfg, this);
        if(c->initConnections(_guiSystem->parentPlayerScene())==false){
            return false;
        }
        mComponents.push_back(c);
    }

    for(GuiSlotTextFieldCfg &cfg : mCfg->slotTextFieldCfgs){

        GuiSlotTextField *c = new GuiSlotTextField(&cfg, this);
        if(c->initConnections(_guiSystem->parentPlayerScene())==false){
            return false;
        }
        mComponents.push_back(c);
    }


    dbgSystem.removeLastMessage();
    return true;

}


void GuiSlotNew::update()
{
    // so far nothing to put here...

}


void GuiSlotNew::removeItem()
{

    if(mItem==nullptr) return;

    for(GuiSlotComponent *c : mComponents){
        c->clear();
    }

    //---
    if(mSelectingButton){
        if(mSelectingButton->IsChecked()){
            mSelectingButton->SetChecked(false);
        }
        mSelectingButton->setDisabled(true);
    }

    //mItem->setSelected(false);        // NO !
    mItem = nullptr;

}


void GuiSlotNew::setItem(Item *_item)
{

    if(mItem == _item) return;

    mItem = _item;


    for(GuiSlotComponent *c : mComponents){
        c->updateOnAddedItem(mItem);
    }


    //---
    if(mParentTable && mSelectingButton){

        if(mItem->isSelectable()){

            mSelectingButton->setDisabled(false);
            //mSelectingButton->SetChecked(mItem == mParentTable->selectedItem());
            mSelectingButton->SetChecked(mItem->isSelected());

        }else{
            mSelectingButton->setDisabled(true);
        }

    }

}



void GuiSlotNew::updateComponents()
{

    for(GuiSlotComponent *c : mComponents){
        c->update();
    }

}




//---------------------------------------------------------------------------------------------



void SelectingSlotNewCallback::OnPressedStarted(jugimap::GuiWidget *_widget)
{

    GuiButton *b = static_cast<GuiButton*>(_widget);

    if(mSlot->parentTable()){
        if(b->IsChecked()){
            mSlot->parentTable()->setSelectedItem(mSlot->item());
        }
    }

}



//================================================================================





GuiWidgetCfg* GuiSlotNewFactory::createWidgetCfg(const std::string &_name)
{
    return new GuiSlotNewCfg(this, _name);
}


GuiWidget* GuiSlotNewFactory::createWidget(GuiWidgetCfg *_cfg, Sprite *_rootSprite)
{

    assert(_cfg->factory==this);

    if(_rootSprite->type()!=SpriteType::COMPOSITE){
        dbgSystem.addMessage("Creating widget '" + _cfg->name + " of type '" + _cfg->factory->typeSignature() +"' from sprite '"
                             + _rootSprite->sourceSprite()->name() +"' error !");
        dbgSystem.addMessage("The sprite must be a composite sprite!'");
        return nullptr;
    }

    return new GuiSlotNew(static_cast<GuiSlotNewCfg*>(_cfg), static_cast<CompositeSprite*>(_rootSprite));
}


//=================================================================================


bool GuiSlotComponent::onDataChanged(ItemData *_data)
{

    update();

    return true;
}



//=================================================================================


GuiSlotSpritePosMarker::GuiSlotSpritePosMarker(GuiSlotSpritePosMarkerCfg *_cfg, GuiSlotNew *_parentSlot) :
    GuiSlotComponent(_parentSlot, GuiSlotComponentType::SPRITE_POS_MARKER),
    mCfg(_cfg)
{

}


bool GuiSlotSpritePosMarker::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("Initializing slot component for sprite position marker ... ");


    mPointShape = FindVectorShapeWithName(mParentSlot->rootSprite()->map(), mCfg->pointShape);
    if(mPointShape==nullptr){
        dbgSystem.addMessage("Point shape '" + mCfg->pointShape +"' not found!");
        return false;
    }

    if(mPointShape->kind()!=ShapeKind::SINGLE_POINT){
        dbgSystem.addMessage("Shape '" + mCfg->pointShape +"' is not of point shape kind!");
        return false;
    }

    //----
    mSpriteLayer = dynamic_cast<SpriteLayer*>(FindLayerWithName(mParentSlot->rootSprite()->map(), mCfg->layer));
    if(mSpriteLayer==nullptr){
        dbgSystem.addMessage("Layer '"+ mCfg->layer + "' not found!");
        return false;
    }


    dbgSystem.removeLastMessage();

    return true;
}


void GuiSlotSpritePosMarker::updateOnAddedItem(Item *_item)
{

    if(mItemSpriteData && mItemSpriteData->sprite()->parentLayer()!=mSpriteLayer){
        DummyFunction();
    }

    clear();

    mItemSpriteData = dynamic_cast<SpriteData*>(_item->findData(mCfg->itemSpriteData));
    if(mItemSpriteData==nullptr){
        return;
    }

    if(mItemSpriteData->isSpriteOwned()==false){   // item has been already stored in another slot
        assert(mItemSpriteData->sprite()->parentLayer());
        mItemSpriteData->sprite()->parentLayer()->takeSprite(mItemSpriteData->sprite());
    }


    mItemSpriteData->setCallback(this);

    //---
    Sprite *s = mItemSpriteData->sprite();
    s->_setParentLayer(mSpriteLayer);
    s->setPosition(mPointShape->singlePointF());
    s->initEngineObjects();

    mSpriteLayer->addSprite(s);
    mItemSpriteData->setSpriteOwned(false);

}


void GuiSlotSpritePosMarker::update()
{

}


void GuiSlotSpritePosMarker::clear()
{

    if(mItemSpriteData==nullptr){
        return;
    }
    if(mItemSpriteData->sprite()->parentLayer()!=mSpriteLayer){     // the item of the mItemSpriteData is stored in another slot
        mItemSpriteData = nullptr;
        return;
    }

    if(mSpriteLayer->takeSprite(mItemSpriteData->sprite())){        // remove sprite from layer
        mItemSpriteData->setSpriteOwned(true);                      // the item takes ownership back
    }
    mItemSpriteData = nullptr;
}


//=================================================================================


GuiSlotTextSprite::GuiSlotTextSprite(GuiSlotTextSpriteCfg *_cfg, GuiSlotNew *_parentSlot) :
    GuiSlotComponent(_parentSlot, GuiSlotComponentType::TEXT_SPRITE),
    mCfg(_cfg)
{

}


bool GuiSlotTextSprite::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("Initializing slot component for text sprite ... ");


    std::vector<Sprite*>sprites;
    CollectSpritesWithName(mParentSlot->rootSprite(), sprites, mCfg->sprite);
    if(sprites.empty()){
        dbgSystem.addMessage("Sprite '" + mCfg->sprite +"' not found!");
        return false;
    }

    if(sprites.front()->type() != SpriteType::TEXT){
        dbgSystem.addMessage("Sprite '" + mCfg->sprite +"' is not a text sprite!");
        return false;
    }

    mTextSprite = static_cast<TextSprite*>(sprites.front());
    mTextSprite->setText("");

    dbgSystem.removeLastMessage();

    return true;
}


void GuiSlotTextSprite::updateOnAddedItem(Item *_item)
{

    clear();

    mItemData = _item->findData(mCfg->itemData);

    if(mItemData==nullptr){
        return;
    }


    mItemData->setCallback(this);

    update();

}


void GuiSlotTextSprite::update()
{

    if(mItemData==nullptr){
        return;
    }

    if(mItemData->type()==ItemDataType::INT){
        IntItemData *i = static_cast<IntItemData*>(mItemData);

        std::string textValue = std::to_string(i->value());

        if(mCfg->digitsWithZero.empty()==false){
            if(textValue.size() < mCfg->digitsWithZero.size()){
                int pos = mCfg->digitsWithZero.size()-textValue.size();
                textValue = mCfg->digitsWithZero.substr(0, pos) + textValue;
            }
        }
        mTextSprite->setText(textValue);

    }else if(mItemData->type()==ItemDataType::FLOAT){
        FloatItemData *i = static_cast<FloatItemData*>(mItemData);
        mTextSprite->setText(std::to_string(i->value()));

    }else if(mItemData->type()==ItemDataType::TEXT_STRING){
        TextData *i = static_cast<TextData*>(mItemData);
        mTextSprite->setText(i->text());
    }

}


void GuiSlotTextSprite::clear()
{

    if(mItemData==nullptr){
        return;
    }

    mTextSprite->setText("");

    mItemData = nullptr;
}


//=================================================================================


GuiSlotTextField::GuiSlotTextField(GuiSlotTextFieldCfg *_cfg, GuiSlotNew *_parentSlot) :
    GuiSlotComponent(_parentSlot, GuiSlotComponentType::TEXT_FIELD),
    mCfg(_cfg)
{

}



bool GuiSlotTextField::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("Initializing slot component for text field ... ");


    mTextField = dynamic_cast<GuiTextField*>(ObtainGuiWidget(_scene, mCfg->textField, WidgetType::TEXT_FIELD));
    if(mTextField==nullptr){
        return false;
    }

    dbgSystem.removeLastMessage();

    return true;
}


void GuiSlotTextField::updateOnAddedItem(Item *_item)
{

    clear();

    mItemTextSegmentData = dynamic_cast<TextSegmentData*>(_item->findData(mCfg->itemTextSegmentData));

    if(mItemTextSegmentData==nullptr){
        return;
    }

    mItemTextSegmentData->setCallback(this);

    mTextField->Start(mItemTextSegmentData->textSegment());

}


void GuiSlotTextField::update()
{

}


void GuiSlotTextField::clear()
{

    if(mItemTextSegmentData==nullptr){
        return;
    }

    mTextField->Reset();
    mItemTextSegmentData->setCallback(nullptr);

    mItemTextSegmentData = nullptr;
}





}




