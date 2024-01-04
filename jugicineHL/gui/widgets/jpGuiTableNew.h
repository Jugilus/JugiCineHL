#ifndef PLAYER__WIDGETS_ITEMS_TABLE_NEW_H
#define PLAYER__WIDGETS_ITEMS_TABLE_NEW_H

#include <assert.h>
#include <algorithm>
#include <cmath>
#include <vector>
#include <map>
#include <array>
#include <string>


#include "jpQueries.h"
#include "jpGuiTable.h"
#include "jpGuiCommon.h"



namespace jugimap{
	

//class VariableManager;
//class TriggerVariable;
class GuiButton;
class GuiSlider;
class CompositeSprite;
class Sprite;
class TextSprite;
class GuiTextField;
//class GameItem;
//class ItemsGroup;
class Item;
class GuiMultiSlotNew;
class GuiSlotNew;



struct TableSlotNewComponentCfg
{
    std::string sprite;
    std::string textSprite;

    std::string textField;
    std::string textFieldSprite;
};


enum class TableDirectNavigationStyle
{
    LOOP,
    VIA_DIRECTION_COMMANDS
};


struct TableNewCfg : public GuiWidgetCfg
{

    TableNewCfg(WidgetFactory *_factory, const std::string &_name) : GuiWidgetCfg(_factory,_name){}

    bool initCfg(const pugi::xml_node &_node) override;


    TableLayout layout = TableLayout::VERTICAL;
    int numRows = 1;
    int numColumns = 1;
    int spacingColumns = 0;
    int spacingRows = 0;

    //---
    std::string slotSprite;
    std::string tableSlot;

    std::string tableSlider;
    std::string tableSliderSprite;

    std::string directSlotNavigationSignal;
    TableDirectNavigationStyle directSelectSlotNavigatorStyle;


    //enum class SlotComponentKind
    //{
    //    SPRITE,
    //    TEXT_SPRITE,
    //    TEXT_FIELD
    //};




    std::vector<TableSlotComponentCfg>slotComponents;

};





class GuiTableNew : public GuiWidget
{
public:


    /*
    enum class ItemInsertDirection
    {
        NOT_DEFINED,
        LEFT_TO_RIGHT,
        RIGHT_TO_LEFT,
        TOP_TO_BOTTOM,
        BOTTOM_TO_TOP
    };
    */


    /*
    enum class Flags
    {
        NONE,
        SELECT_FIRST_ITEM

    };
    */


    GuiTableNew(TableNewCfg *_tableCfg, CompositeSprite * _compositeSprite);
    //GuiTable(CompositeSprite * _compositeSprite);
    ~GuiTableNew();

    bool initConnections(GuiSystem *_guiSystem) override;
   // bool init() override;

    bool onChildItemChanged(Item *_parentItem, Item *addedChildItem, Item *removedChildItem, Item *newSelectedItem);

    void setToInitialState() override;
    void update() override;

    void SetSelectedViaDirectNavigation(TableDirectNavigationStyle _style);
    void updateSlots();
    void updateSlotsItems_FixedSlots();
    void _updateSlotsItems_FixedSlots(int itemIndexOfFirstItemInTable, int numItemsNeto);
    void updateSlotsItems_ScrollingSlots();
    void updateSlotsItems_NoSlider();

    void updateSlotContentForItem(Item* _item);

    void setItem(Item * _groupItem) override;
    Item* item(){ return mGroupItem; }

    void removeItem(Item *_item);
    void removeAllItems();

    Item* selectedItem(){ return mSelectedItem; }
    void setSelectedItem(Item *_item);

    void ScrollToTheStartOfItems();
    void ScrollToTheEndOfItems();

    TableNewCfg* cfg(){ return mCfg; }
    //VariableManager *variables(){ return mVariables; }
    //TriggerVariableManager *triggers(){ return mTriggers; }

private:

    TableNewCfg* mCfg = nullptr;                           //LINK
    CompositeSprite *mRootSprite = nullptr;             // LINK
    Item* mGroupItem = nullptr;                              // LINK
    Item* mSelectedItem = nullptr;

    TableLayout mLayout = TableLayout::HORIZONTAL;
    //ItemInsertDirection mMainInsertDirection = ItemInsertDirection::NOT_DEFINED;            // inserting multislots into table
    //ItemInsertDirection mSecondaryInsertDirection = ItemInsertDirection::NOT_DEFINED;       // inserting slots inside multislot

    TableSlotsKind mKind = TableSlotsKind::FIXED_SLOTS;

    std::vector<GuiMultiSlotNew*>mMultiSlots;                      // OWNED
    GuiSlider *mSlider = nullptr;                     // LINK

    int mNumSlotsInMultiSlot = 0;

    int mNumSlotsHorizontal = 1;
    int mNumSlotsVertical = 1;
    int mSlotsSpacingHorizontal = 0;                       // spacing between multislots
    int mSlotsSpacingVertical = 0;                         // spacing between slots in multislots

    //---
    bool mAutoSelectSingleSlotItem = true;                 // if the table has one slot only, autoselect visible item
    //VariableManager *mVariables;
    //TriggerVariableManager *mTriggers;                  // OWNED
    //TriggerVariable *mTrigger_siDeletableChanged = nullptr;       // LINK for quick access

    SignalQuery mQDirectSlotNavigation;

    void updateSliderValueStep();
    void clearSelectedItem();

    int getNumItemsNeto();

};






//-------------------------------------------------------------------------------------


class GuiMultiSlotNew
{
public:

friend class GuiTable;
    /*
    enum class  SlotsLayout
    {
        HORIZONTAL,
        VERTICAL
    };

    enum class ItemInsertDirection
    {
        LEFT_TO_RIGHT,
        RIGHT_TO_LEFT,
        TOP_TO_BOTTOM,
        BOTTOM_TO_TOP
    };
    */


    //GuiMultiSlot(GuiTable *_parentTable, int _numSlots, SlotsLayout _slotsLayout, ItemInsertDirection _itemInsertDirection);
    GuiMultiSlotNew(GuiTableNew *_parentTable, int _numSlots);
    ~GuiMultiSlotNew();

    GuiWidget *parentWidget(){ return mParentWidget; }
    std::vector<GuiSlotNew*>& slots(){ return mSlots; }
    GuiSlotNew* slotAt(int _index){ return mSlots[_index]; }


private:
    GuiWidget *mParentWidget = nullptr;
    std::vector<GuiSlotNew*>mSlots;

    Vec2f position;

    //GuiTable::Layout mSlotsLayout = GuiTable::Layout::HORIZONTAL;
    //GuiTable::ItemInsertDirection mItemInsertDirection = GuiTable::ItemInsertDirection::LEFT_TO_RIGHT;

};







//------------------------------------------------------------------------------------------


class TableNewFactory : public WidgetFactory
{
public:

    TableNewFactory(const std::string &_typeSignature, WidgetType _type) : WidgetFactory(_typeSignature, _type){}

    GuiWidgetCfg* createWidgetCfg(const std::string &_name) override;
    GuiWidget* createWidget(GuiWidgetCfg* _cfg, Sprite *_rootSprite) override;

};


}




#endif
