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
//#include "jmGuiText.h"

#include "jpGuiBar.h"
#include "jpGuiButton.h"
#include "jpGuiSlider.h"
#include "jpGuiTextField.h"
#include "jpGuiTextInput.h"
#include "jpGuiSystem.h"
#include "jpVariables.h"
#include "jpLogicState.h"
#include "jpItemsCommon.h"
#include "jpItemSlotData.h"
#include "jpPlayedScene.h"
#include "jpPlayedApp.h"
#include "jpGuiTable.h"


namespace jugimap{




bool TableCfg::initCfg(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading widget of type '" + factory->typeSignature() + "' and name '" + name + "'...");


    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());

        if(nodeName=="parameters"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="scrollingLayout"){
                    std::string sScrollingLayout = a.as_string();
                    if(sScrollingLayout=="HORIZONTAL"){
                        layout = TableLayout::HORIZONTAL;

                    }else if(sScrollingLayout=="VERTICAL"){
                        layout = TableLayout::VERTICAL;

                    }else{
                        dbgSystem.addMessage("Wrong scrolling layout '" + sScrollingLayout + "'!");
                        return false;
                    }

                }else if(attributeName=="numRows"){
                    numRows = a.as_int(numRows);

                }else if(attributeName=="numColumns"){
                    numColumns = a.as_int(numColumns);

                }else if(attributeName=="spacingColumns"){
                    spacingColumns = a.as_int(spacingColumns);

                }else if(attributeName=="spacingRows"){
                    spacingRows = a.as_int(spacingRows);

                }else{

                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                    return false;
                }
            }

        }else if(nodeName=="slot"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="sprite"){
                    slotButtonSprite = a.as_string();

                }else if(attributeName=="button"){
                    slotButton = a.as_string();

                }else{
                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                    return false;
                }
            }

            for(pugi::xml_node nChildNode = n.first_child(); nChildNode; nChildNode = nChildNode.next_sibling()){
                std::string childNodeName = std::string(nChildNode.name());

                slotComponents.push_back(TableSlotComponentCfg());
                TableSlotComponentCfg &sc = slotComponents.back();

                if(childNodeName=="spriteComponent"){

                    for(pugi::xml_attribute a = nChildNode.first_attribute(); a; a = a.next_attribute()){
                        std::string attributeName =std::string(a.name());

                        if(attributeName=="sprite"){
                            sc.sprite = a.as_string();

                        }else{
                            dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + childNodeName +"' !");
                            return false;
                        }
                    }

                }else if(childNodeName=="textSpriteComponent"){

                    for(pugi::xml_attribute a = nChildNode.first_attribute(); a; a = a.next_attribute()){
                        std::string attributeName =std::string(a.name());

                        if(attributeName=="textSprite"){
                            sc.textSprite = a.as_string();

                        }else{
                            dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + childNodeName +"' !");
                            return false;
                        }
                    }

                }else if(childNodeName=="textFieldComponent"){

                    for(pugi::xml_attribute a = nChildNode.first_attribute(); a; a = a.next_attribute()){
                        std::string attributeName =std::string(a.name());

                        if(attributeName=="textField"){
                            sc.textField = a.as_string();

                        }else if(attributeName=="textFieldSprite"){
                            sc.textFieldSprite = a.as_string();

                        }else{
                            dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + childNodeName +"' !");
                            return false;
                        }
                    }

                }
            }


        }else if(nodeName=="tableSlider"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="sprite"){
                    tableSliderSprite = a.as_string();

                }else if(attributeName=="slider"){
                    tableSlider = a.as_string();

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



GuiTable::GuiTable(TableCfg *_tableCfg, CompositeSprite * _compositeSprite)
{

    mType = WidgetType::TABLE;
    mCfg = _tableCfg;
    mRootSprite = _compositeSprite;

    //mVariables = new VariableManager();

    //mTriggers = new TriggerVariableManager();
    //mTriggers->addTrigger("siDeletableChanged");

   // mTrigger_siDeletableChanged = mTriggers->getTrigger("siDeletableChanged");
   // mTrigger_siDeletableChanged->setVariable(new BoolVar("siDeletable", true), true);

}


/*
GuiTable::GuiTable(CompositeSprite * _compositeSprite)
{

    mType = WidgetType::TABLE;
    mRootSprite = _compositeSprite;

    //mVariables = new VariableManager();

    //mTriggers = new TriggerVariableManager();
    //mTriggers->addTrigger("siDeletableChanged");

   // mTrigger_siDeletableChanged = mTriggers->getTrigger("siDeletableChanged");
   // mTrigger_siDeletableChanged->setVariable(new BoolVar("siDeletable", true), true);

}
*/


GuiTable::~GuiTable()
{

    for(GuiMultiSlot *ms : mMultiSlots){
        delete ms;
    }
   //delete mTriggers;

}



bool GuiTable::initConnections(GuiSystem * _guiSystem)
{

    dbgSystem.addMessage("Init connections for the widget '" + mCfg->name + "' ...");

    mName = mRootSprite->parameters().value("wName");
    if(mName.empty()){
        mName = mRootSprite->name();
    }

    if(mName=="" && mChildWidget==false){
        dbgSystem.addMessage("Missing parameter 'wName' (identification name of the widget) !");
    }

    mLayout = mCfg->layout;
    mNumSlotsHorizontal = mCfg->numColumns;
    mNumSlotsVertical = mCfg->numRows;

    if(mLayout==TableLayout::VERTICAL){
        mNumSlotsInMultiSlot = mNumSlotsHorizontal;
    }else if( mLayout==TableLayout::HORIZONTAL){
        mNumSlotsInMultiSlot = mNumSlotsVertical;
    }

    mSlotsSpacingHorizontal = mCfg->spacingColumns;
    mSlotsSpacingVertical = mCfg->spacingRows;


    //--- obtain first slot root sprite

    std::vector<Sprite*>sprites;

    CollectSpritesWithName(mRootSprite->map(), sprites, mCfg->slotButtonSprite);
    if(sprites.empty()){
        dbgSystem.addMessage("Slot button sprite '" + mCfg->slotButtonSprite + "' not found!");
        return false;
    }
    if(sprites.front()->type()!=SpriteType::COMPOSITE){
        dbgSystem.addMessage("Slot button sprite must be a composite sprite!");
        return false;
    }

    ButtonCfg * slotButtonCfg = dynamic_cast<ButtonCfg *>(_guiSystem->widgetsManager()->getWidgetCfg(mCfg->slotButton));
    if(slotButtonCfg==nullptr){
        dbgSystem.addMessage("Slot button '" + mCfg->slotButton +"' not found!");
        return false;
    }
    slotButtonCfg->checkable = true;        //just in case


    //--- firstSlotRootSprite also serves as root sprite of the selecting button.
    // Make sure that there is at least one child sprite which provides collision and visual button states.
    // Button checked state is used for marking slots as selected so it should not be ommited!

    CompositeSprite * firstSlotRootSprite = static_cast<CompositeSprite*>(sprites.front());

    GuiButton *firstSlotSelectingButton = new GuiButton(slotButtonCfg, firstSlotRootSprite);
    firstSlotSelectingButton->setChildWidget(true);
    if(firstSlotSelectingButton->initConnections(_guiSystem)==false){
        return false;
    }
    firstSlotSelectingButton->setName("slotButton");


    Vec2f slotSize = firstSlotRootSprite->boundingBox().GetSize();
    Vec2f posStart = firstSlotRootSprite->position();
    Vec2f pos = posStart;



    //--- firstSlotRootSprite must be positioned in the top-left; the other slots will be positioned according to that!

    if(mLayout==TableLayout::VERTICAL){

        mMultiSlots.resize(mNumSlotsVertical);

        for(int iy = 0; iy<mNumSlotsVertical; iy++){

            pos.y = posStart.y - iy*(slotSize.y + mSlotsSpacingVertical);

            mMultiSlots[iy] = new GuiMultiSlot(this, mNumSlotsHorizontal);

            for(int ix = 0; ix<mNumSlotsHorizontal; ix++){

                pos.x = posStart.x + ix*(slotSize.x + mSlotsSpacingHorizontal);


                if(ix==0 && iy==0){
                    mMultiSlots[iy]->mSlots[ix] = new GuiSlot(mMultiSlots[iy], firstSlotRootSprite, firstSlotSelectingButton);

                }else{

                    CompositeSprite * slotRootSprite = static_cast<CompositeSprite*>(firstSlotRootSprite->makeActiveCopy());
                    slotRootSprite->setPosition(pos);

                    GuiButton *slotSelectingButton = new GuiButton(slotButtonCfg, slotRootSprite);
                    slotSelectingButton->setChildWidget(true);
                    slotSelectingButton->initConnections(_guiSystem);

                    mMultiSlots[iy]->mSlots[ix] = new GuiSlot(mMultiSlots[iy], slotRootSprite, slotSelectingButton);
                }

               if(mMultiSlots[iy]->mSlots[ix]->initConnections(_guiSystem, mCfg->slotComponents) == false){
                   return false;
               }
            }
        }

    }else{

        mMultiSlots.resize(mNumSlotsHorizontal);

        for(int ix = 0; ix<mNumSlotsHorizontal; ix++){

            pos.x = posStart.x + ix*(slotSize.x + mSlotsSpacingHorizontal);
            mMultiSlots[ix] = new GuiMultiSlot(this, mNumSlotsVertical);

            for(int iy = 0; iy<mNumSlotsVertical; iy++){

                pos.y = posStart.y - iy*(slotSize.y + mSlotsSpacingVertical);

                if(ix==0 && iy==0){
                    mMultiSlots[ix]->mSlots[iy] = new GuiSlot(mMultiSlots[ix], firstSlotRootSprite, firstSlotSelectingButton);

                }else{

                    CompositeSprite * slotRootSprite = static_cast<CompositeSprite*>(firstSlotRootSprite->makeActiveCopy());
                    slotRootSprite->setPosition(pos);

                    //GuiButton *slotSelectingButton = new GuiButton(slotRootSprite);
                    //slotSelectingButton->init();
                    GuiButton *slotSelectingButton = new GuiButton(slotButtonCfg, slotRootSprite);
                    slotSelectingButton->setChildWidget(true);
                    slotSelectingButton->initConnections(_guiSystem);

                    mMultiSlots[ix]->mSlots[iy] = new GuiSlot(mMultiSlots[ix], slotRootSprite, slotSelectingButton);
                }

               if( mMultiSlots[ix]->mSlots[iy]->initConnections(_guiSystem, mCfg->slotComponents) == false){
                   return false;
               }
            }
        }
    }


    //--- slider
    if(mCfg->tableSliderSprite.empty()==false && mCfg->tableSlider.empty()==false){

        sprites.clear();
        CollectSpritesWithName(mRootSprite->map(), sprites, mCfg->tableSliderSprite);

        if(sprites.empty()){
            dbgSystem.addMessage("Slider sprite '" + mCfg->tableSliderSprite + "' not found!");
            return false;
        }
        if(sprites.front()->type()!=SpriteType::COMPOSITE){
            dbgSystem.addMessage("Slider sprite must be a composite sprite!");
            return false;
        }


        SliderCfg * sliderCfg = dynamic_cast<SliderCfg *>(_guiSystem->widgetsManager()->getWidgetCfg(mCfg->tableSlider));
        if(sliderCfg==nullptr){
            dbgSystem.addMessage("Table slider '" + mCfg->tableSlider +"' not found!");
            return false;
        }

        //if(sliderCfg->slidingButton.empty()==false && sliderCfg->slidingButtonSprite.empty()==false){
            sliderCfg->min = 0.0f;
            sliderCfg->max = 1.0f;
            sliderCfg->def = 0.0f;
            sliderCfg->deltaValue = 0.01f;
            sliderCfg->deltaInPercents = false;
        //}else{

        //}

        mSlider = new GuiSlider(sliderCfg, static_cast<CompositeSprite*>(sprites.front()));
        mSlider->setChildWidget(true);
        if(mSlider->initConnections(_guiSystem)==false){
            return false;
        }
    }


    mInitialized = true;

    dbgSystem.removeLastMessage();
    return true;
}


/*
bool GuiTable::init()
{

    dbgSystem.addMessage("init GuiTable");

    //name = mRootSprite->parameters().value("wName", "");
    mName = mRootSprite->name();
    if(mName.empty()){
        mName = mRootSprite->parameters().value("wName");
    }

    //tag = mRootSprite->parameters().intValue("wTag");
    //designKind = mRootSprite->sourceSprite()->constantParameters().value("wDesign");
    mTag = mRootSprite->id();

    std::string layout = mRootSprite->sourceSprite()->constantParameters().value("wLayout");
    if(layout=="horizontal"){
        mLayout = TableLayout::HORIZONTAL;
    }else{
        mLayout = TableLayout::VERTICAL;
    }

    mNumSlotsHorizontal = mRootSprite->sourceSprite()->constantParameters().intValue("wNumSlotsHorizontal", 1);
    mNumSlotsVertical = mRootSprite->sourceSprite()->constantParameters().intValue("wNumSlotsVertical" ,1);

    if(mLayout==TableLayout::VERTICAL){
        mNumSlotsInMultiSlot = mNumSlotsHorizontal;
    }else if( mLayout==TableLayout::HORIZONTAL){
        mNumSlotsInMultiSlot = mNumSlotsVertical;
    }


    mSlotsSpacingHorizontal = mRootSprite->sourceSprite()->constantParameters().intValue("wSlotsSpacingHorizontal", 0);
    mSlotsSpacingVertical = mRootSprite->sourceSprite()->constantParameters().intValue("wSlotsSpacingVertical", 0);


    //--- obtain first slot root sprite

    std::vector<Sprite*>sprites;
    CollectSpritesWithConstParameter(mRootSprite->map(), sprites, "wFirstSlot");
    if(sprites.empty()){
        dbgSystem.addMessage("Missing sprite with 'wFirstSlot' parameter in table '" + mName + "'!");
        return false;
    }
    if(sprites.front()->type()!=SpriteType::COMPOSITE){
        dbgSystem.addMessage("Slot sprite must be a composite sprite!");
        return false;
    }


    CompositeSprite * firstSlotRootSprite = static_cast<CompositeSprite*>(sprites.front());


    //--- firstSlotRootSprite also serves as root sprite of the selecting button.
    // Make sure that there is at least one child sprite which provides collision and visual button states.
    // Button checked state is used for marking slots as selected so it should not be ommited!

    //

    //--- Add some required parameters for the button creation if they are not already defined.
    if(firstSlotRootSprite->sourceSprite()->constantParameters().exists("wCheckable")==false){
        firstSlotRootSprite->sourceSprite()->constantParameters().push_back(CParameter("wCheckable", "true", CParameter::typeBOOLEAN));
    }
    //if(firstSlotRootSprite->parameters().exists("wExclusiveGroup")==false){
    //    firstSlotRootSprite->parameters().push_back(CParameter("wExclusiveGroup", "slotsOf"+name, CParameter::typeBOOLEAN));
    //}

    //---
    GuiButton *firstSlotSelectingButton = new GuiButton(firstSlotRootSprite);
    if(firstSlotSelectingButton->init()==false){
        return false;
    }

    Vec2f slotSize = firstSlotRootSprite->boundingBox().GetSize();
    Vec2f posStart = firstSlotRootSprite->position();
    Vec2f pos = posStart;




    //--- firstSlotRootSprite must be positioned in the top-left; the other slots will be positioned according to that!

    if(mLayout==TableLayout::VERTICAL){

        mMultiSlots.resize(mNumSlotsVertical);

        for(int iy = 0; iy<mNumSlotsVertical; iy++){

            pos.y = posStart.y - iy*(slotSize.y + mSlotsSpacingVertical);

            mMultiSlots[iy] = new GuiMultiSlot(this, mNumSlotsHorizontal);

            for(int ix = 0; ix<mNumSlotsHorizontal; ix++){

                pos.x = posStart.x + ix*(slotSize.x + mSlotsSpacingHorizontal);


                if(ix==0 && iy==0){
                    mMultiSlots[iy]->mSlots[ix] = new GuiSlot(mMultiSlots[iy], firstSlotRootSprite, firstSlotSelectingButton);

                }else{

                    CompositeSprite * slotRootSprite = static_cast<CompositeSprite*>(firstSlotRootSprite->makeActiveCopy());
                    slotRootSprite->setPosition(pos);

                    GuiButton *slotSelectingButton = new GuiButton(slotRootSprite);
                    slotSelectingButton->init();

                    mMultiSlots[iy]->mSlots[ix] = new GuiSlot(mMultiSlots[iy], slotRootSprite, slotSelectingButton);
                }

               if(mMultiSlots[iy]->mSlots[ix]->init() == false){
                   return false;
               }
            }
        }

    }else{

        mMultiSlots.resize(mNumSlotsHorizontal);

        for(int ix = 0; ix<mNumSlotsHorizontal; ix++){

            pos.x = posStart.x + ix*(slotSize.x + mSlotsSpacingHorizontal);
            mMultiSlots[ix] = new GuiMultiSlot(this, mNumSlotsVertical);

            for(int iy = 0; iy<mNumSlotsVertical; iy++){

                pos.y = posStart.y - iy*(slotSize.y + mSlotsSpacingVertical);

                if(ix==0 && iy==0){
                    mMultiSlots[ix]->mSlots[iy] = new GuiSlot(mMultiSlots[ix], firstSlotRootSprite, firstSlotSelectingButton);

                }else{

                    CompositeSprite * slotRootSprite = static_cast<CompositeSprite*>(firstSlotRootSprite->makeActiveCopy());
                    slotRootSprite->setPosition(pos);

                    GuiButton *slotSelectingButton = new GuiButton(slotRootSprite);
                    slotSelectingButton->init();

                    mMultiSlots[ix]->mSlots[iy] = new GuiSlot(mMultiSlots[ix], slotRootSprite, slotSelectingButton);
                }

               if( mMultiSlots[ix]->mSlots[iy]->init() == false){
                   return false;
               }
            }
        }
    }



    GuiSlider* slider = nullptr;

    sprites.clear();
    CollectSpritesWithConstParameter(mRootSprite->map(), sprites, "wSlider");

    //if(mRootSprite->sourceSprite()->constantParameters().exists("wSlider")){
    //    std::string sliderName = mRootSprite->sourceSprite()->constantParameters().value("wSlider");

    //    sprites.clear();
    //    CollectSpritesWithParameter(mRootSprite->map(), sprites, "wName", sliderName);

    //    if(sprites.empty()){
    //        dbgSystem.addMessage("Missing sprite with 'wName' parameter of value '" + sliderName +"' in the table '" + name + "'!");
    //        return false;
    //    }

    //    if(sprites.front()->type()!=SpriteType::COMPOSITE){
    //        dbgSystem.addMessage("Sprite for slider '" + sliderName + "' of the table '" + name + "' must be composite sprite!");
    //        return false;
    //    }

    if(sprites.empty()==false){

        if(sprites.front()->type()!=SpriteType::COMPOSITE){
            dbgSystem.addMessage("Sprite for slider of the table'" + mName + "' must be a composite sprite!");
            return false;
        }

        mSlider = new GuiSlider(static_cast<CompositeSprite*>(sprites.front()));
        if(mSlider->init()==false){
            return false;
        }

        if(mSlider->slidingButton()){
            mSlider->SetValueMin(0.0);
            mSlider->SetValueMax(1.0);
            mSlider->SetValue(0.0);
            //mSlider->SetDeltaValue(0.25, false);
            mSlider->SetDeltaValue(0.01, false);

        }else{
            mSlider->SetValueMin(0);
            mSlider->SetValueMax(1);
            mSlider->SetValue(0);
            //mSlider->SetDeltaValue(1, false);
            mSlider->SetDeltaValue(0.01, false);

        }
    }

    dbgSystem.removeLastMessage();
    return true;
}
*/

void GuiTable::setItemsGroup(ItemsGroup *_itemsGroup)
{

    if(mItemsGroup == _itemsGroup) return;

    clearSelectedItem();

    if(mItemsGroup){
        removeAllItems();
    }

    mItemsGroup = _itemsGroup;

    //if(mItemsGroup->items().empty()==false){
    //    setSelectedItem(mItemsGroup->items().front());
    //}


    if(mItemsGroup){
        mItemsGroup->_setTable(this);
        updateSlots();
    }


}


void GuiTable::removeItem(GameItem *_item)
{

     for(int i=0; i<mMultiSlots.size(); i++){
         for(GuiSlot* s : mMultiSlots[i]->slots()){
             if(s->item()==_item){
                 s->removeItem();
                 return;
             }
        }
    }
}


void GuiTable::removeAllItems()
{

     for(GuiMultiSlot *ms : mMultiSlots){
         for(GuiSlot* s : ms->slots()){
            s->removeItem();
        }
    }

}


void GuiTable::setSelectedItem(GameItem *_item)
{

    if(mSelectedItem==_item) return;

    if(_item && _item->isSelectable()==false){
        return;
    }


    mValueChanged = false;

    //--- clear current item if any
    clearSelectedItem();
    /*
    if(mSelectedItem){
        // reset 'checked' button of the slot with the currently selected item.
        for(GuiMultiSlot *ms : mMultiSlots){
            for(GuiSlot *s : ms->mSlots){
                if(s->item() == mSelectedItem){
                    s->selectingButton()->SetChecked(false);
                }
            }
        }
    }
    */

    mSelectedItem = _item;

    if(mSelectedItem){
        for(GuiMultiSlot *ms : mMultiSlots){
            for(GuiSlot *s : ms->mSlots){
                if(s->item() == mSelectedItem){
                    s->selectingButton()->SetChecked(true);
                }
            }
        }

        if(mSelectedItem->doOnSelected()){
            UpdateMode updateMode;
            mSelectedItem->doOnSelected()->update(updateMode);
        }
    }


    //---
    mValueChanged = true;
    GuiWidget::_SetInteractedWidget(this);

    if(mSelectedItem){
        print("Table '" + mName + "' selected item '" +mSelectedItem->name() +"'.");
    }else{
        print("Table '" + mName + "' no selected item.");
    }


}


void GuiTable::ScrollToTheStartOfItems()
{

    if(mSlider){
        mSlider->SetValue(0.0);
    }
    updateSlots();

}


void GuiTable::ScrollToTheEndOfItems()
{

    if(mSlider){
        mSlider->SetValue(1.0);
    }
    updateSlots();
}


void GuiTable::setToInitialState()
{

    for(GuiMultiSlot *ms : mMultiSlots){
        for(GuiSlot *s : ms->mSlots){
            s->selectingButton()->setInitialState(ButtonState::DISABLED);
            s->selectingButton()->setToInitialState();
        }
    }

    if(mSlider){
        mSlider->setToInitialState();
    }

}


void GuiTable::update()
{

    mValueChanged = false;

        //--- manage disabled state
    if(mItemsGroup==nullptr || mItemsGroup->items().empty()){

        if(isDisabled()==false){
            for(GuiMultiSlot *ms : mMultiSlots){
                for(GuiSlot *s : ms->mSlots){
                    s->selectingButton()->setDisabled(true);
                }
            }
            if(mSlider){
                mSlider->setDisabled(true);
            }

            setDisabled(true);
        }

         return;

    }else{

        if(isDisabled()){
            for(GuiMultiSlot *ms : mMultiSlots){
                //for(GuiSlot *s : ms->mSlots){
                //    s->selectingButton()->SetDisabled(false);
                //}
            }
            if(mSlider){
                mSlider->setDisabled(false);
            }

            setDisabled(false);
        }
    }


    //---
    if(mSlider){
        mSlider->update();
        if(GuiWidget::GetInteracted_ValueChanged()==mSlider){
            updateSlots();
        }
    }

    //---
    for(GuiMultiSlot *ms : mMultiSlots){
        for(GuiSlot *s : ms->mSlots){
            if(s->item()){
                s->selectingButton()->update();
            }
        }
    }

}


/*
void GuiTable::updateSlotsItems()
{

    if(mItemsGroup==nullptr) return;



    std::vector<GameItem*> &items = mItemsGroup->items();

    // number of items which is effective on scrolling
    int nItemsNeto = items.size() / mNumSlotsInMultiSlot;
    if(items.size() % mNumSlotsInMultiSlot > 0){
        nItemsNeto ++;
    }


    int itemIndexOfFirstItemInTable = 0;

    if(nItemsNeto <= mMultiSlots.size()){
        mSlider->SetDeltaValue(1.0, false);       // full range

    }else{

        // number of slots in sliding range
        int slidingRangeInMultiSlots = nItemsNeto;


        itemIndexOfFirstItemInTable = mSlider->GetValue() * slidingRangeInMultiSlots;


        float sliderStep = 100 * (mMultiSlots.size()-2) / (float)slidingRangeInMultiSlots;
        mSlider->SetDeltaValue(sliderStep, false);

        print("sliderValue:" + std::to_string(mSlider->GetValue()) + " indexStart:" + std::to_string(itemIndexOfFirstItemInTable));
    }



   for(int i=0; i<mMultiSlots.size(); i++){
       for(GuiSlot* s : mMultiSlots[i]->slots()){
           s->removeItem();
       }
   }



   int multislotIndex = 0;

   for(int i=0; i<nItemsNeto; i++){

       if(i>=itemIndexOfFirstItemInTable && multislotIndex<mMultiSlots.size()){

           GuiMultiSlot *ms = mMultiSlots[multislotIndex];

           for(int j = 0; j<mNumSlotsInMultiSlot; j++ ){
               int iItem = i*mNumSlotsInMultiSlot + j;

               GameItem* item = items[iItem];
               GuiSlotData *itemSlotData = item->slotData();

               assert(itemSlotData);
               assert(itemSlotData->slot()==nullptr);

               ms->mSlots[j]->addItem(item);
           }
           multislotIndex++;
       }
   }



}
*/



void GuiTable::updateSlots()
{

    if(mSlider){

        //updateSliderValueStep();

        if(mKind==TableSlotsKind::FIXED_SLOTS){
            updateSlotsItems_FixedSlots();

        }else if (mKind==TableSlotsKind::SCROLLING_SLOTS){


        }

    }else{
        updateSlotsItems_NoSlider();

    }

}


void GuiTable::updateSlotsItems_FixedSlots()
{

    if(mItemsGroup==nullptr) return;
    if(mSlider==nullptr) return;


    std::vector<GameItem*> &items = mItemsGroup->items();
    int numItems = items.size();
    int numMultislots = mMultiSlots.size();


    // number of items which is effective on scrolling
    int numItemsNeto = numItems / mNumSlotsInMultiSlot;
    if(numItems % mNumSlotsInMultiSlot > 0){
        numItemsNeto ++;
    }



    int itemIndexOfFirstItemInTable = 0;

    if(numItemsNeto <= numMultislots){
        mSlider->SetDeltaValue(1.0, false);       // full range
        mSlider->setDisabled(true);

    }else{

        // number of slots in sliding range
        //int slidingRangeInMultiSlots = numItemsNeto;


        //itemIndexOfFirstItemInTable = int(std::round(mSlider->GetValue() * slidingRangeInMultiSlots));
        itemIndexOfFirstItemInTable = int(std::round(mSlider->GetValue() * numItemsNeto));


        //float sliderStep = 100 * ((int(mMultiSlots.size())-2) / (float)slidingRangeInMultiSlots);
        //float sliderStep = 100 * (numMultislots/(float)slidingRangeInMultiSlots);
        //float sliderStep = 100 * (numMultislots/(float)numItemsNeto);
        float sliderStep = numMultislots/(float)numItemsNeto;

        mSlider->setDisabled(false);
        mSlider->SetDeltaValue(sliderStep, false);

        //print("sliderValue:" + std::to_string(mSlider->GetValue()) + " indexStart:" + std::to_string(itemIndexOfFirstItemInTable));

        if(itemIndexOfFirstItemInTable>=numItemsNeto){      // situation at slider value 1.0
            itemIndexOfFirstItemInTable --;
        }
    }


   for(int i=0; i<mMultiSlots.size(); i++){
       for(GuiSlot* s : mMultiSlots[i]->slots()){
           //s->removeItem();
           s->_setMarkedForRemoveItem(true);
       }
   }

   int multislotIndex = 0;

   for(int i=0; i<numItemsNeto; i++){

       if(i>=itemIndexOfFirstItemInTable && multislotIndex<mMultiSlots.size()){

           GuiMultiSlot *ms = mMultiSlots[multislotIndex];

           for(int j = 0; j<mNumSlotsInMultiSlot; j++ ){
               int iItem = i*mNumSlotsInMultiSlot + j;

               GameItem* item = items[iItem];
               GuiSlotData *itemSlotData = item->slotData();

               assert(itemSlotData);
               //assert(itemSlotData->slot()==nullptr);

               ms->mSlots[j]->addItem(item);
               ms->mSlots[j]->_setMarkedForRemoveItem(false);

           }
           multislotIndex++;
       }
   }

   //---
    for(int i=0; i<mMultiSlots.size(); i++){
        for(GuiSlot* s : mMultiSlots[i]->slots()){
            if(s->_markedForRemoveItem()){
                s->removeItem();
                s->_setMarkedForRemoveItem(false);
            }
       }
   }

    //---
    if(numMultislots * mNumSlotsInMultiSlot == 1  && mAutoSelectSingleSlotItem){
        setSelectedItem(mMultiSlots[0]->slots().at(0)->item());
    }


}


void GuiTable::updateSlotsItems_ScrollingSlots()
{


}


void GuiTable::updateSlotsItems_NoSlider()
{

    if(mItemsGroup==nullptr) return;

    int iItem = 0;

    for(GuiMultiSlot *ms : mMultiSlots){
        for(GuiSlot *s : ms->slots()){

            if(iItem < mItemsGroup->items().size()){

                GameItem* item = mItemsGroup->items().at(iItem);
                GuiSlotData *itemSlotData = item->slotData();

                assert(itemSlotData);
                //assert(itemSlotData->slot()==nullptr);

                s->addItem(item);

            }else{
                s->removeItem();

            }
        }
    }

}




void GuiTable::updateSlotContentForItem(GameItem* _item)
{

    for(GuiMultiSlot *ms : mMultiSlots){
        for(GuiSlot *s : ms->slots()){
            if(s->item()==_item){
                s->updateComponents();
            }
        }
    }

}




void GuiTable::updateSliderValueStep()
{

    if(mItemsGroup==nullptr) return;
    if(mSlider==nullptr) return;


    int numItems =  mItemsGroup->items().size();
    int numMultislots = mMultiSlots.size();

    // number of items which is effective on scrolling
    int numItemsNeto = numItems / mNumSlotsInMultiSlot;
    if(numItems % mNumSlotsInMultiSlot > 0){
        numItemsNeto ++;
    }

    if(numItemsNeto <= numMultislots){
        mSlider->SetDeltaValue(1.0, false);       // full range

    }else{

        float sliderStep = numMultislots/(float)numItemsNeto;
        mSlider->SetDeltaValue(sliderStep, false);
    }

}


void GuiTable::clearSelectedItem()
{

    if(mSelectedItem){

        // reset 'checked' button of the slot with the currently selected item.
        for(GuiMultiSlot *ms : mMultiSlots){
            for(GuiSlot *s : ms->mSlots){
                if(s->item() == mSelectedItem){
                    s->selectingButton()->SetChecked(false);
                }
            }
        }

        mSelectedItem = nullptr;
    }

}


//---------------------------------------------------------------------------------------------



/*
GuiMultiSlot::GuiMultiSlot(GuiTable *_parentTable, int _numSlots, SlotsLayout _slotsLayout, ItemInsertDirection _itemInsertDirection)
{

    mParentTable = _parentTable;
    mSlotsLayout = _slotsLayout;
    mItemInsertDirection = _itemInsertDirection;
    mSlots.resize(_numSlots);
    //for(int i = 0; i<mSlots.size(); i++){
    //    mSlots[i] = new GuiSlot();
    //}

}
*/


GuiMultiSlot::GuiMultiSlot(GuiTable *_parentTable, int _numSlots)
{

    mParentWidget = _parentTable;
    mSlots.resize(_numSlots);

}


GuiMultiSlot::~GuiMultiSlot()
{
    for(GuiSlot *s : mSlots){
        delete  s;
    }

}


//---------------------------------------------------------------------------------------------



GuiSlot::GuiSlot(GuiMultiSlot *_parentMultislot, CompositeSprite *_rootSprite, GuiButton *_selectingButton)
{

    mParentMultislot = _parentMultislot;
    mRootSprite = _rootSprite;
    mSelectingButton = _selectingButton;
    mSelectingButton->assignCallback(new SelectingSlotCallback(this));
    mSelectingButton->_setForceChecked(true);
    //

}


bool GuiSlot::initConnections(GuiSystem *_guiSystem, std::vector<TableSlotComponentCfg> &_slotComponents)
{

    dbgSystem.addMessage("Init slot connections...");


    std::vector<Sprite*>sprites;

    for(TableSlotComponentCfg &cfg : _slotComponents){

        if(cfg.sprite.empty()==false){
            sprites.clear();

            CollectSpritesWithName(mRootSprite->map(), sprites, cfg.sprite);
            if(sprites.empty()){
                dbgSystem.addMessage("No sprites with name '" + cfg.sprite +"' found within the parent slot sprite!");
            }

            for(Sprite *s : sprites){

                if(s->type()==SpriteType::TEXT){
                    dbgSystem.addMessage("Sprite '" + cfg.sprite +"' is a text sprite! Use 'textSpriteComponent' node for adding text components to the slot!");
                    return false;
                }

                s->setVisible(false);   // this sprite serves as a positioning marker for sprites in items and must be hidden

                mIcSprites.emplace_back(std::pair<std::string, Sprite*>(cfg.sprite, s));
            }



        }else if(cfg.textSprite.empty()==false){

            sprites.clear();
            CollectSpritesWithName(mRootSprite->map(), sprites, cfg.textSprite);
            if(sprites.empty()){
                dbgSystem.addMessage("No sprites with name '" + cfg.textSprite +"' found within the parent slot sprite!");
            }

            for(Sprite *s : sprites){

                if(s->type()!=SpriteType::TEXT){
                    dbgSystem.addMessage("Sprite '" + cfg.textSprite +"' is not a text sprite!");
                    return false;
                }

                TextSprite* ts = static_cast<TextSprite*>(s);
                ts->setText("");    // no text at start

                mIcTextSprites.emplace_back(std::pair<std::string, TextSprite*>(cfg.textSprite, ts));
            }



        }else if(cfg.textField.empty()==false && cfg.textFieldSprite.empty()==false){

            sprites.clear();
            CollectSpritesWithName(mRootSprite->map(), sprites, cfg.textFieldSprite);

            if(sprites.empty() && cfg.textFieldSprite==mRootSprite->name()){
                sprites.push_back(mRootSprite);         // also possible to avoid creating special composite sprite for a text field (obviously this is possible only for one text field)
            }

            if(sprites.empty()){
                dbgSystem.addMessage("Text field sprite '" + cfg.textFieldSprite + "' not found!");
                return false;
            }
            if(sprites.front()->type()!=SpriteType::COMPOSITE){
                dbgSystem.addMessage("Text field sprite must be a composite sprite!");
                return false;
            }


            TextFieldCfg * textFieldCfg = dynamic_cast<TextFieldCfg *>(_guiSystem->widgetsManager()->getWidgetCfg(cfg.textField));
            if(textFieldCfg==nullptr){
                dbgSystem.addMessage("Text field '" + cfg.textField +"' not found!");
                return false;
            }

            CompositeSprite * textFieldSprite = static_cast<CompositeSprite*>(sprites.front());

            GuiTextField *textField = new GuiTextField(textFieldCfg, textFieldSprite);
            textField->setChildWidget(true);
            if(textField->initConnections(_guiSystem)==false){
                return false;
            }
            mIcTextFields.emplace_back(std::pair<std::string, GuiTextField*>(cfg.textField, textField));

        }
    }



    dbgSystem.removeLastMessage();
    return true;

}


/*
bool GuiSlot::init()
{

    dbgSystem.addMessage("init GuiSlot");


    //---- sprites ( text sprites NOT included)

    std::vector<Sprite*>sprites;
    //CollectSpritesWithParameter(mRootSprite->map(), sprites, "wSlotSprite");
    CollectSpritesWithConstParameter(mRootSprite->map(), sprites, "wSlotSprite");

    for(Sprite *s : sprites){

        //const std::string & spriteName = s->name();
        const std::string & componentName = s->sourceSprite()->constantParameters().value("wSlotSprite");
        if(componentName.empty()){
            dbgSystem.addMessage("Sprite with source sprite '" + s->sourceSprite()->name() +"' has defined parameter 'wSlotSprite' but is without name! The name is required for identification of slot objects!");
            return false;
        }

        if(s->type()==SpriteType::TEXT){
            dbgSystem.addMessage("Sprite with source sprite '" + s->sourceSprite()->name() +"' is a text sprite and should not have defined 'wSlotSprite' parameter! Use 'wSlotTextSprite' instead!");
            return false;
        }

        s->setVisible(false);   // this sprite serves as a positioning marker for sprites in items and must be hidden

        mIcSprites.emplace_back(std::pair<std::string, Sprite*>(componentName, s));
    }


    //---- text sprites

    sprites.clear();
    //CollectSpritesWithParameter(mRootSprite->map(), sprites, "wSlotTextSprite", "true");
    CollectSpritesWithConstParameter(mRootSprite->map(), sprites, "wSlotTextSprite");

    for(Sprite *s : sprites){


        //std::string spriteName = s->name();
        const std::string & componentName = s->sourceSprite()->constantParameters().value("wSlotTextSprite");
        if(componentName.empty()){
            dbgSystem.addMessage("Sprite has defined parameter 'wSlotTextSprite' but is without name! The name is required for identification of slot objects!");
            return false;
        }
        if(s->type()!=SpriteType::TEXT){
            dbgSystem.addMessage("Sprite with source sprite '" + s->sourceSprite()->name() +"' has defined 'wSlotTextSprite' parameter but is not a text sprite!");
            return false;
        }

        TextSprite* ts = static_cast<TextSprite*>(s);
        ts->setText("");    // no text at start

        mIcTextSprites.emplace_back(std::pair<std::string, TextSprite*>(componentName, ts));
    }


    //---- create text fields
    //VectorShape* shape = FindVectorShapeWithParameter(mRootSprite->map(), "icTextField" , "",  ShapeKind::RECTANGLE);
    std::vector<VectorShape*>shapes;
    //CollectVectorShapesWithParameter(mRootSprite->map(), shapes, "wSlotTextField", "true", ShapeKind::RECTANGLE);
    CollectVectorShapesWithParameter(mRootSprite->map(), shapes, "wSlotTextField");

    for(VectorShape* vs : shapes){

        if(vs->kind()!=ShapeKind::RECTANGLE){
            dbgSystem.addMessage("VectorShape has defined parameter 'wSlotTextField' but is not rectangle!");
            return false;
        }

        const std::string & componentName = vs->parameters().value("wSlotTextField");
        if(componentName.empty()){
            dbgSystem.addMessage("VectorShape has defined parameter 'wSlotTextField' but is without name! The name is required for identification of slot objects!");
            return false;
        }

        GuiTextField * textField = new GuiTextField(vs, mRootSprite->map());
        if(textField->init()==false){
            return false;
        }
        textField->SetLocalLinesSpacingFactor(-0.3);
        mIcTextFields.emplace_back(std::pair<std::string, GuiTextField*>(componentName, textField));
    }

    dbgSystem.removeLastMessage();
    return true;
}
*/


GuiSlot::~GuiSlot()
{

    for(std::pair<std::string, GuiTextField*>& p : mIcTextFields){
        GuiTextField * tf = p.second;
        delete tf;
    }

    delete mSelectingButton;
}


void GuiSlot::removeItem()
{

    if(mGameItem==nullptr) return;

    GuiSlotData *slotData = mGameItem->slotData();

    for(GuiSlotDataComponent *c : slotData->components()){


        if(c->mType==GuiSlotDataComponent::Type::SPRITE){
            GuiSlotDataSprite *ic = static_cast<GuiSlotDataSprite*>(c);

            Sprite *slotSprite = FindCachedObject<Sprite>(mIcSprites, ic->mSlotSpriteName);

            if(ic->mMode==GuiSlotDataSprite::Mode::REPLACE_SLOT_SPRITE){

                ic->mReplacementSprite->parentLayer()->takeSprite(ic->mReplacementSprite);                    // remove sprite from layer
                ic->mOwned = true;

            }else if(ic->mMode==GuiSlotDataSprite::Mode::MODIFY_SLOT_SPRITE){

                if(slotSprite->isVisible()){
                    slotSprite->setVisible(false);
                }
            }


        }else if(c->mType==GuiSlotDataComponent::Type::TEXT_SPRITE){
            GuiSlotDataTextSprite *ic = static_cast<GuiSlotDataTextSprite*>(c);

            //ic->mSprite->parentLayer()->takeSprite(ic->mSprite);
            //ic->mOwned = true;

            TextSprite *textSprite = FindCachedObject<TextSprite>(mIcTextSprites, ic->mSlotTextSpriteName);
            assert(textSprite);
            textSprite->setText("");


        }else if(c->mType==GuiSlotDataComponent::Type::TEXT_SEGMENT){
            GuiSlotDataTextField * ic = static_cast<GuiSlotDataTextField*>(c);

            GuiTextField *textField = FindCachedObject<GuiTextField>(mIcTextFields, ic->mSlotTextFieldName);
            assert(textField);
            assert(textField->GetTextSegment()==ic->mTextSegment);
            textField->Reset();
        }
    }

    slotData->setSlot(nullptr);
    mGameItem = nullptr;


    //---
    if(mSelectingButton->IsChecked()){
        mSelectingButton->SetChecked(false);
    }
    mSelectingButton->setDisabled(true);


}


void GuiSlot::addItem(GameItem *_item)
{

    if(mGameItem == _item) return;

    mGameItem = _item;


    updateComponents();


    //---
    if(mGameItem->isSelectable()){

        mSelectingButton->setDisabled(false);
        if(mParentMultislot->parentWidget()->type() == WidgetType::TABLE){
            GuiTable *table = static_cast<GuiTable*>(mParentMultislot->parentWidget());
            //if(mGameItem == table->selectedItem()){
                mSelectingButton->SetChecked(mGameItem == table->selectedItem());
            //}
        }

    }else{
        mSelectingButton->setDisabled(true);
    }
}



void GuiSlot::updateComponents()
{

    GuiSlotData *slotData = mGameItem->slotData();
    slotData->setSlot(this);


    for(GuiSlotDataComponent *c : slotData->components()){

        if(c->mType==GuiSlotDataComponent::Type::SPRITE){
            GuiSlotDataSprite *ic = static_cast<GuiSlotDataSprite*>(c);

            Sprite *slotSprite = FindCachedObject<Sprite>(mIcSprites, ic->mSlotSpriteName);

            if(slotSprite==nullptr){
                assert(slotSprite);
                continue;
            }


            if(ic->mMode==GuiSlotDataSprite::Mode::REPLACE_SLOT_SPRITE){
                if(slotSprite->isVisible()==false){
                    slotSprite->setVisible(false);
                }

                //--- add item componenet sprite to the layer of the marker sprite and set it to its position
                slotSprite->parentLayer()->addSprite(ic->mReplacementSprite);
                ic->mOwned = false;
                ic->mReplacementSprite->setPosition(slotSprite->position(Sprite::IncludedPositions::BASE));

            }else if(ic->mMode==GuiSlotDataSprite::Mode::MODIFY_SLOT_SPRITE){

                if(slotSprite->isVisible()!= ic->mVisible){
                    slotSprite->setVisible(ic->mVisible);
                }
                if(slotSprite->type()==SpriteType::STANDARD){
                    StandardSprite *s = static_cast<StandardSprite*>(slotSprite);
                    if(s->activeImageFrameIndex() != ic->mFrame){
                        s->setActiveImageByFrameIndex(ic->mFrame);
                    }
                }
            }

        }else if(c->mType==GuiSlotDataComponent::Type::TEXT_SPRITE){
            GuiSlotDataTextSprite *ic = static_cast<GuiSlotDataTextSprite*>(c);

            TextSprite *textSprite = FindCachedObject<TextSprite>(mIcTextSprites, ic->mSlotTextSpriteName);

            if(textSprite==nullptr){
                assert(textSprite);
                continue;
            }

            textSprite->setText(ic->mText);


        }else if(c->mType==GuiSlotDataComponent::Type::TEXT_SEGMENT){

            GuiSlotDataTextField *ic = static_cast<GuiSlotDataTextField*>(c);

            GuiTextField *textField = FindCachedObject<GuiTextField>(mIcTextFields, ic->mSlotTextFieldName);

            if(textField==nullptr){
                assert(textField);
                continue;
            }

            textField->Start(ic->mTextSegment);

        }
    }

}


//---------------------------------------------------------------------------------------------



void SelectingSlotCallback::OnPressedStarted(jugimap::GuiWidget *_widget)
{

    GuiButton *b = static_cast<GuiButton*>(_widget);

    if(mSlot->parentMultislot()->parentWidget()->type()==WidgetType::TABLE){
        GuiTable* table = static_cast<GuiTable*>(mSlot->parentMultislot()->parentWidget());

        if(b->IsChecked()){
            table->setSelectedItem(mSlot->item());
        }

    }

}




//========================================================================================



GuiWidgetCfg* TableFactory::createWidgetCfg(const std::string &_name)
{
    return new TableCfg(this, _name);
}


GuiWidget* TableFactory::createWidget(GuiWidgetCfg *_cfg, Sprite *_rootSprite)
{
    assert(_cfg->factory==this);

    if(_rootSprite->type()!=SpriteType::COMPOSITE){
        dbgSystem.addMessage("Creating widget '" + _cfg->name + " of type '" + _cfg->factory->typeSignature() +"' from sprite '"
                             + _rootSprite->sourceSprite()->name() +"' error !");
        dbgSystem.addMessage("The sprite must be a composite sprite!'");
        return nullptr;
    }

    return new GuiTable(static_cast<TableCfg*>(_cfg), static_cast<CompositeSprite*>(_rootSprite));
}




}




