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
#include "data/jpData.h"
#include "data/jpItem.h"

#include "jpObjectParser.h"

#include "jpGuiBar.h"
#include "jpGuiButton.h"
#include "jpGuiSlider.h"
#include "jpGuiTextField.h"
#include "jpGuiTextInput.h"
#include "jpGuiSlot.h"
#include "jpGuiSystem.h"
#include "jpVariables.h"
#include "jpLogicAction.h"
#include "jpItemsCommon.h"
#include "jpItemSlotData.h"
#include "jpPlayedScene.h"
#include "jpPlayedApp.h"
#include "jpInput.h"
#include "jpGuiTableNew.h"


namespace jugimap{




bool TableNewCfg::initCfg(const pugi::xml_node &_node)
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
                    slotSprite = a.as_string();

                }else if(attributeName=="slot"){
                    tableSlot = a.as_string();

                }else{
                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                    return false;
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

        }else if(nodeName=="directSlotNavigation"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="signal"){
                    directSlotNavigationSignal = a.as_string();

                }else if(attributeName=="style"){
                    std::string sValue = a.as_string();

                    if(sValue=="LOOP"){
                        directSelectSlotNavigatorStyle = TableDirectNavigationStyle::LOOP;
                    }else if(sValue=="VIA_DIRECTION_COMMANDS"){
                        directSelectSlotNavigatorStyle = TableDirectNavigationStyle::VIA_DIRECTION_COMMANDS;
                    }

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



GuiTableNew::GuiTableNew(TableNewCfg *_tableCfg, CompositeSprite * _compositeSprite)
{
    mType = WidgetType::TABLE;
    mCfg = _tableCfg;
    mRootSprite = _compositeSprite;

    mAttributeFlags |= Attributes::ITEM_ASSIGNABLE;
}



GuiTableNew::~GuiTableNew()
{

    for(GuiMultiSlotNew *ms : mMultiSlots){
        delete ms;
    }

}



bool GuiTableNew::initConnections(GuiSystem * _guiSystem)
{

    dbgSystem.addMessage("Init connections for the widget '" + mCfg->name + "' ...");


    mRootMap = mRootSprite->parentLayer()->rootMap();
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

    CollectSpritesWithName(mRootSprite->map(), sprites, mCfg->slotSprite);
    if(sprites.empty()){
        dbgSystem.addMessage("Slot button sprite '" + mCfg->slotSprite + "' not found!");
        return false;
    }
    if(sprites.front()->type()!=SpriteType::COMPOSITE){
        dbgSystem.addMessage("Slot button sprite must be a composite sprite!");
        return false;
    }

    GuiSlotNewCfg * slotCfg = dynamic_cast<GuiSlotNewCfg *>(_guiSystem->widgetsManager()->getWidgetCfg(mCfg->tableSlot));
    if(slotCfg==nullptr){
        dbgSystem.addMessage("Slot '" + mCfg->tableSlot +"' not found!");
        return false;
    }


    //--- firstSlotRootSprite also serves as root sprite of the selecting button.
    // Make sure that there is at least one child sprite which provides collision and visual button states.
    // Button checked state is used for marking slots as selected so it should not be ommited!

    CompositeSprite * firstSlotRootSprite = static_cast<CompositeSprite*>(sprites.front());

    GuiSlotNew *firstSlot = new GuiSlotNew(slotCfg, firstSlotRootSprite, this);
    firstSlot->setChildWidget(true);
    if(firstSlot->initConnections(_guiSystem)==false){
        return false;
    }
    firstSlot->setName("slotButton");


    Vec2f slotSize = firstSlotRootSprite->boundingBox().GetSize();
    Vec2f posStart = firstSlotRootSprite->position();
    Vec2f pos = posStart;



    //--- firstSlotRootSprite must be positioned in the top-left; the other slots will be positioned according to that!

    if(mLayout==TableLayout::VERTICAL){

        mMultiSlots.resize(mNumSlotsVertical);

        for(int iy = 0; iy<mNumSlotsVertical; iy++){

            pos.y = posStart.y - iy*(slotSize.y + mSlotsSpacingVertical);

            mMultiSlots[iy] = new GuiMultiSlotNew(this, mNumSlotsHorizontal);

            for(int ix = 0; ix<mNumSlotsHorizontal; ix++){

                pos.x = posStart.x + ix*(slotSize.x + mSlotsSpacingHorizontal);


                if(ix==0 && iy==0){
                    mMultiSlots[iy]->slots()[ix] = firstSlot;

                }else{

                    CompositeSprite * slotRootSprite = static_cast<CompositeSprite*>(firstSlotRootSprite->makeActiveCopy());
                    slotRootSprite->setPosition(pos);

                    GuiSlotNew *slot = new GuiSlotNew(slotCfg, slotRootSprite, this);
                    slot->setChildWidget(true);
                    if(slot->initConnections(_guiSystem)==false){
                        return false;
                    }
                    mMultiSlots[iy]->slots()[ix] = slot;
                }
            }
        }

    }else{

        mMultiSlots.resize(mNumSlotsHorizontal);

        for(int ix = 0; ix<mNumSlotsHorizontal; ix++){

            pos.x = posStart.x + ix*(slotSize.x + mSlotsSpacingHorizontal);
            mMultiSlots[ix] = new GuiMultiSlotNew(this, mNumSlotsVertical);

            for(int iy = 0; iy<mNumSlotsVertical; iy++){

                pos.y = posStart.y - iy*(slotSize.y + mSlotsSpacingVertical);

                if(ix==0 && iy==0){
                    mMultiSlots[ix]->slots()[iy] = firstSlot;

                }else{

                    CompositeSprite * slotRootSprite = static_cast<CompositeSprite*>(firstSlotRootSprite->makeActiveCopy());
                    slotRootSprite->setPosition(pos);

                    GuiSlotNew *slot = new GuiSlotNew(slotCfg, slotRootSprite, this);
                    slot->setChildWidget(true);
                    if(slot->initConnections(_guiSystem)==false){
                        return false;
                    }

                    mMultiSlots[ix]->slots()[iy] = slot;
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

    //----
    if(mCfg->directSlotNavigationSignal.empty()==false){
        if(mQDirectSlotNavigation.parseBoolSignal(_guiSystem->parentPlayerScene(), mCfg->directSlotNavigationSignal, true, nullptr)==false){
            return false;
        }
    }


    mInitialized = true;

    dbgSystem.removeLastMessage();
    return true;
}


bool GuiTableNew::onChildItemChanged(Item *_parentItem, Item *addedChildItem, Item *removedChildItem, Item *newSelectedItem)
{

    if(removedChildItem){
        if(removedChildItem == mSelectedItem){
            clearSelectedItem();
        }
    }
    if(newSelectedItem){
        setSelectedItem(newSelectedItem);
    }

    updateSlots();
    return true;
}


void GuiTableNew::setItem(Item *_groupItem)
{

    if(mGroupItem == _groupItem) return;


    if(mSelectedItem){
        clearSelectedItem();
    }


    if(mGroupItem){
        removeAllItems();
        mGroupItem->setCallbackOnChangedChildreen(nullptr);
    }

    mGroupItem = _groupItem;



    if(mGroupItem){
        //mGroupItem->_setTable(this);
        updateSlots();
        mGroupItem->setCallbackOnChangedChildreen(this);

        if(mGroupItem->sourceItem()->type()==ItemType::SELECTABLE_GROUP){
            if(mGroupItem->childItems().empty()==false){
                setSelectedItem(mGroupItem->childItems().back());
            }
        }
    }


}


void GuiTableNew::removeItem(Item *_item)
{

    if(mSelectedItem==_item){
        clearSelectedItem();
    }

     for(int i=0; i<mMultiSlots.size(); i++){
         for(GuiSlotNew* s : mMultiSlots[i]->slots()){
             if(s->item()==_item){
                 s->removeItem();
                 return;
             }
        }
    }
}


void GuiTableNew::removeAllItems()
{

    clearSelectedItem();

     for(GuiMultiSlotNew *ms : mMultiSlots){
         for(GuiSlotNew* s : ms->slots()){
            s->removeItem();
        }
    }

}


void GuiTableNew::setSelectedItem(Item *_item)
{

    if(mSelectedItem==_item) return;

    //if(_item && _item->isSelectable()==false){
    if(_item && _item->isSelectable()==false){
        return;
    }


    mValueChanged = false;

    //--- clear current item if any
    clearSelectedItem();


    mSelectedItem = _item;

    if(mSelectedItem){
        for(GuiMultiSlotNew *ms : mMultiSlots){
            for(GuiSlotNew *s : ms->slots()){
                if(s->item() == mSelectedItem){
                    s->selectingButton()->SetChecked(true);
                }
            }
        }

        //if(mSelectedItem->doOnSelected()){
        //    UpdateMode updateMode;
        //    mSelectedItem->doOnSelected()->update(updateMode);
        //}
    }
    mSelectedItem->setSelected(true);

    //---
    mValueChanged = true;
    GuiWidget::_SetInteractedWidget(this);

    if(mSelectedItem){
        print("Table '" + mName + "' selected item '" +mSelectedItem->name() +"'.");
    }else{
        print("Table '" + mName + "' no selected item.");
    }


}


void GuiTableNew::ScrollToTheStartOfItems()
{

    if(mSlider){
        mSlider->SetValue(0.0);
    }
    updateSlots();

}


void GuiTableNew::ScrollToTheEndOfItems()
{

    if(mSlider){
        mSlider->SetValue(1.0);
    }
    updateSlots();
}


void GuiTableNew::setToInitialState()
{

    for(GuiMultiSlotNew *ms : mMultiSlots){
        for(GuiSlotNew *s : ms->slots()){
            s->selectingButton()->setInitialState(ButtonState::DISABLED);
            s->selectingButton()->setToInitialState();
        }
    }

    if(mSlider){
        mSlider->setToInitialState();
    }

}


void GuiTableNew::update()
{

    mValueChanged = false;

        //--- manage disabled state
    if(mGroupItem==nullptr || mGroupItem->childItems().empty()){

        if(isDisabled()==false){
            for(GuiMultiSlotNew *ms : mMultiSlots){
                for(GuiSlotNew *s : ms->slots()){
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
            for(GuiMultiSlotNew *ms : mMultiSlots){
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
    for(GuiMultiSlotNew *ms : mMultiSlots){
        for(GuiSlotNew *s : ms->slots()){
            if(s->item()){
                s->selectingButton()->update();
            }
        }
    }

    //---
    if(mQDirectSlotNavigation.isValid()){
        if(mQDirectSlotNavigation.active()){
            SetSelectedViaDirectNavigation(mCfg->directSelectSlotNavigatorStyle);
        }
    }

}


void GuiTableNew::SetSelectedViaDirectNavigation(TableDirectNavigationStyle _style)
{

    if(mGroupItem==nullptr) return;
    if(mGroupItem->childItems().empty()) return;

    std::vector<Item*> &items = mGroupItem->childItems();
    int numItems = items.size();

    int numMultislots = mMultiSlots.size();

    // number of items which is effective on scrolling
    int numItemsNeto = numItems / mNumSlotsInMultiSlot;
    if(numItems % mNumSlotsInMultiSlot > 0){
        numItemsNeto ++;
    }

    //----
    int currentMainIndex = 0;
    int currentSecondaryIndex = 0;

    for(Item *i : items){
        if(currentSecondaryIndex >= mNumSlotsInMultiSlot){
            currentSecondaryIndex = 0;
            currentMainIndex++;
        }
        if(i==mSelectedItem){
            break;
        }
        currentSecondaryIndex++;
    }


    /*
    for(unsigned int iMS=0; iMS<mMultiSlots.size(); iMS++){
        GuiMultiSlotNew *ms = mMultiSlots[iMS];
        bool found = false;
        for(unsigned int iS=0; iS<ms->slots().size(); iS++){
            GuiSlotNew *s = ms->slots()[iS];
            if(s->item()==mSelectedItem){
                currentMultiSlotIndex = iMS;
                currentSlotIndex = iS;
                found = true;
                break;
            }
        }
        if(found){
            break;
        }
    }
    */


    int nextMainIndex = currentMainIndex;
    int nextSecondaryIndex = currentSecondaryIndex;

    if(mLayout==TableLayout::VERTICAL){


    }else if(mLayout==TableLayout::HORIZONTAL){

        if(_style==TableDirectNavigationStyle::LOOP){

            if(nextSecondaryIndex+1 < mNumSlotsInMultiSlot){
                nextSecondaryIndex++;

            }else{
                nextSecondaryIndex = 0;
                nextMainIndex++;
                if(nextMainIndex==numItemsNeto){
                    nextMainIndex = 0;
                }
            }

        }else if(_style==TableDirectNavigationStyle::VIA_DIRECTION_COMMANDS){

            if(app->inputSystem()->rightDirection().activeStarted(true)){
                nextMainIndex++;

            }else if(app->inputSystem()->leftDirection().activeStarted(true)){
                nextMainIndex--;

            }else if(app->inputSystem()->upDirection().activeStarted(true)){
                nextSecondaryIndex--;

            }else if(app->inputSystem()->downDirection().activeStarted(true)){
                nextSecondaryIndex++;
            }

            Clamp(nextMainIndex, 0,numItemsNeto-1);
            Clamp(nextSecondaryIndex, 0,mNumSlotsInMultiSlot-1);
        }
    }

    if(nextMainIndex==currentMainIndex && nextSecondaryIndex==currentSecondaryIndex){
        return;
    }


    //----
    int mainIndexOfFirstItemInTable = 0;

    //if(nextMainIndex >= numMultislots){
    //    itemIndexOfFirstItemInTable = nextMainIndex - numMultislots + 1;
    //}

    if(nextMainIndex <= numMultislots/2){
        mainIndexOfFirstItemInTable = 0;

    }else if(nextMainIndex >= numItemsNeto - numMultislots/2){
        mainIndexOfFirstItemInTable = numItemsNeto - numMultislots;

    }else{
        mainIndexOfFirstItemInTable = nextMainIndex - (numMultislots/2);
    }


    _updateSlotsItems_FixedSlots(mainIndexOfFirstItemInTable, numItemsNeto);


    //GuiMultiSlotNew *ms = mMultiSlots[nextMainIndex];
    //GuiSlotNew *s = ms->slots()[nextSecondaryIndex];

    int nextIndex = nextMainIndex*mNumSlotsInMultiSlot + nextSecondaryIndex;
    Clamp(nextIndex, 0, numItems-1);
    Item *newSelectedItem = items.at(nextIndex);
    setSelectedItem(newSelectedItem);

    //if(s->item()){
    //    setSelectedItem(s->item());
    //}

}



void GuiTableNew::updateSlots()
{

    if(mSlider){
        if(mKind==TableSlotsKind::FIXED_SLOTS){
            updateSlotsItems_FixedSlots();

        }else if (mKind==TableSlotsKind::SCROLLING_SLOTS){


        }

    }else{
        //updateSlotsItems_NoSlider();
        if(mGroupItem==nullptr) return;
        //if(mGroupItem->childItems().empty()) return;

        std::vector<Item*> &items = mGroupItem->childItems();
        int numItems = items.size();

        // number of items which is effective on scrolling
        int numItemsNeto = numItems / mNumSlotsInMultiSlot;
        if(numItems % mNumSlotsInMultiSlot > 0){
            numItemsNeto ++;
        }

        _updateSlotsItems_FixedSlots(0, numItemsNeto);

    }


}


void GuiTableNew::updateSlotsItems_FixedSlots()
{

    if(mGroupItem==nullptr) return;
    if(mSlider==nullptr) return;


    std::vector<Item*> &items = mGroupItem->childItems();
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


    _updateSlotsItems_FixedSlots(itemIndexOfFirstItemInTable, numItemsNeto);

    /*
   for(int i=0; i<mMultiSlots.size(); i++){
       for(GuiSlotNew* s : mMultiSlots[i]->slots()){
           //s->removeItem();
           s->_setMarkedForRemoveItem(true);
       }
   }

   int multislotIndex = 0;

   for(int i=0; i<numItemsNeto; i++){

       if(i>=itemIndexOfFirstItemInTable && multislotIndex<mMultiSlots.size()){

           GuiMultiSlotNew *ms = mMultiSlots[multislotIndex];

           for(int j = 0; j<mNumSlotsInMultiSlot; j++ ){
               int iItem = i*mNumSlotsInMultiSlot + j;

               Item* item = items[iItem];
               //GuiSlotData *itemSlotData = item->slotData();
               //assert(itemSlotData);

               ms->slots()[j]->setItem(item);
               ms->slots()[j]->_setMarkedForRemoveItem(false);

           }
           multislotIndex++;
       }
   }

   //---
    for(int i=0; i<mMultiSlots.size(); i++){
        for(GuiSlotNew* s : mMultiSlots[i]->slots()){
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
    */


}



void GuiTableNew::_updateSlotsItems_FixedSlots(int itemIndexOfFirstItemInTable, int numItemsNeto)
{


    std::vector<Item*> &items = mGroupItem->childItems();
    int numItems = items.size();
    int numMultislots = mMultiSlots.size();



   for(unsigned int i=0; i<mMultiSlots.size(); i++){
       for(GuiSlotNew* s : mMultiSlots[i]->slots()){
           //s->removeItem();
           s->_setMarkedForRemoveItem(true);
       }
   }

   int multislotIndex = 0;

   for(int i=0; i<numItemsNeto; i++){

       if(i>=itemIndexOfFirstItemInTable && multislotIndex<mMultiSlots.size()){

           GuiMultiSlotNew *ms = mMultiSlots[multislotIndex];

           for(int j = 0; j<mNumSlotsInMultiSlot; j++ ){
               int iItem = i*mNumSlotsInMultiSlot + j;

               Item* item = items[iItem];
               //GuiSlotData *itemSlotData = item->slotData();
               //assert(itemSlotData);

               ms->slots()[j]->setItem(item);
               ms->slots()[j]->_setMarkedForRemoveItem(false);

           }
           multislotIndex++;
       }
   }

   //---
    for(int i=0; i<mMultiSlots.size(); i++){
        for(GuiSlotNew* s : mMultiSlots[i]->slots()){
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



void GuiTableNew::updateSlotsItems_ScrollingSlots()
{

}


void GuiTableNew::updateSlotsItems_NoSlider()
{

    if(mGroupItem==nullptr) return;

    int iItem = 0;

    for(GuiMultiSlotNew *ms : mMultiSlots){
        for(GuiSlotNew *s : ms->slots()){

            if(iItem < mGroupItem->childItems().size()){

                Item* item = mGroupItem->childItems().at(iItem);

                //GuiSlotData *itemSlotData = item->slotData();
                //assert(itemSlotData);

                s->setItem(item);

            }else{
                s->removeItem();

            }
        }
    }

}


void GuiTableNew::updateSlotContentForItem(Item *_item)
{

    for(GuiMultiSlotNew *ms : mMultiSlots){
        for(GuiSlotNew *s : ms->slots()){
            if(s->item()==_item){
                s->updateComponents();
            }
        }
    }

}


void GuiTableNew::updateSliderValueStep()
{

    if(mGroupItem==nullptr) return;
    if(mSlider==nullptr) return;


    int numItems =  mGroupItem->childItems().size();
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


void GuiTableNew::clearSelectedItem()
{

    if(mSelectedItem){

        // reset 'checked' button of the slot with the currently selected item.
        for(GuiMultiSlotNew *ms : mMultiSlots){
            for(GuiSlotNew *s : ms->slots()){
                if(s->item() == mSelectedItem){
                    s->selectingButton()->SetChecked(false);
                }
            }
        }
        mSelectedItem->setSelected(false);
        mSelectedItem = nullptr;
    }

}


int GuiTableNew::getNumItemsNeto()
{

    if(mGroupItem==nullptr) return 0;

    std::vector<Item*> &items = mGroupItem->childItems();
    int numItems = items.size();

    int numItemsNeto = numItems / mNumSlotsInMultiSlot;
    if(numItems % mNumSlotsInMultiSlot > 0){
        numItemsNeto ++;
    }

    return numItemsNeto;

}


//---------------------------------------------------------------------------------------------




GuiMultiSlotNew::GuiMultiSlotNew(GuiTableNew *_parentTable, int _numSlots)
{

    mParentWidget = _parentTable;
    mSlots.resize(_numSlots);

}


GuiMultiSlotNew::~GuiMultiSlotNew()
{
    for(GuiSlotNew *s : mSlots){
        delete  s;
    }

}


//========================================================================================



GuiWidgetCfg* TableNewFactory::createWidgetCfg(const std::string &_name)
{
    return new TableNewCfg(this, _name);
}


GuiWidget* TableNewFactory::createWidget(GuiWidgetCfg *_cfg, Sprite *_rootSprite)
{
    assert(_cfg->factory==this);

    if(_rootSprite->type()!=SpriteType::COMPOSITE){
        dbgSystem.addMessage("Creating widget '" + _cfg->name + " of type '" + _cfg->factory->typeSignature() +"' from sprite '"
                             + _rootSprite->sourceSprite()->name() +"' error !");
        dbgSystem.addMessage("The sprite must be a composite sprite!'");
        return nullptr;
    }

    return new GuiTableNew(static_cast<TableNewCfg*>(_cfg), static_cast<CompositeSprite*>(_rootSprite));
}




}




