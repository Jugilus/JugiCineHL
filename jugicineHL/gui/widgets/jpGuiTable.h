#ifndef PLAYER__WIDGETS_ITEMS_TABLE_H
#define PLAYER__WIDGETS_ITEMS_TABLE_H

#include <assert.h>
#include <algorithm>
#include <cmath>
#include <vector>
#include <map>
#include <array>
#include <string>


#include "jpGuiCommon.h"



namespace jugimap{
	

class VariableManager;
class TriggerVariable;
class GuiButton;
class GuiSlider;
class CompositeSprite;
class Sprite;
class TextSprite;
class GuiTextField;
class GameItem;
class ItemsGroup;
class GuiMultiSlot;
class GuiSlot;




enum class TableLayout
{
    VERTICAL,
    HORIZONTAL
};


enum class TableSlotsKind
{
    FIXED_SLOTS,
    SCROLLING_SLOTS

};


struct TableSlotComponentCfg
{
    //SlotComponentKind kind = SlotComponentKind::SPRITE;
    std::string sprite;
    std::string textSprite;

    std::string textField;
    std::string textFieldSprite;
};


struct TableCfg : public GuiWidgetCfg
{

    TableCfg(WidgetFactory *_factory, const std::string &_name) : GuiWidgetCfg(_factory,_name){}

    bool initCfg(const pugi::xml_node &_node) override;


    TableLayout layout = TableLayout::VERTICAL;
    int numRows = 1;
    int numColumns = 1;
    int spacingColumns = 0;
    int spacingRows = 0;

    //---
    std::string slotButtonSprite;
    std::string slotButton;

    std::string tableSlider;
    std::string tableSliderSprite;


    //enum class SlotComponentKind
    //{
    //    SPRITE,
    //    TEXT_SPRITE,
    //    TEXT_FIELD
    //};




    std::vector<TableSlotComponentCfg>slotComponents;

};




class GuiTable : public GuiWidget
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


    GuiTable(TableCfg *_tableCfg, CompositeSprite * _compositeSprite);
    //GuiTable(CompositeSprite * _compositeSprite);
    ~GuiTable();

    bool initConnections(GuiSystem *_guiSystem) override;
   // bool init() override;



    void setToInitialState() override;
    void update() override;

    void updateSlots();
    void updateSlotsItems_FixedSlots();
    void updateSlotsItems_ScrollingSlots();
    void updateSlotsItems_NoSlider();

    void updateSlotContentForItem(GameItem* _item);

    void setItemsGroup(ItemsGroup * _itemsGroup);
    ItemsGroup* itemsGroup(){ return mItemsGroup; }

    void removeItem(GameItem *_item);
    void removeAllItems();

    GameItem* selectedItem(){ return mSelectedItem; }
    void setSelectedItem(GameItem *_item);

    void ScrollToTheStartOfItems();
    void ScrollToTheEndOfItems();

    TableCfg* cfg(){ return mCfg; }
    //VariableManager *variables(){ return mVariables; }
    //TriggerVariableManager *triggers(){ return mTriggers; }

private:

    TableCfg* mCfg = nullptr;                           //LINK
    CompositeSprite *mRootSprite = nullptr;           // LINK
    ItemsGroup* mItemsGroup = nullptr;              // LINK
    GameItem* mSelectedItem = nullptr;

    TableLayout mLayout = TableLayout::HORIZONTAL;
    //ItemInsertDirection mMainInsertDirection = ItemInsertDirection::NOT_DEFINED;            // inserting multislots into table
    //ItemInsertDirection mSecondaryInsertDirection = ItemInsertDirection::NOT_DEFINED;       // inserting slots inside multislot

    TableSlotsKind mKind = TableSlotsKind::FIXED_SLOTS;

    std::vector<GuiMultiSlot*>mMultiSlots;                      // OWNED
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


    void updateSliderValueStep();
    void clearSelectedItem();

};






//-------------------------------------------------------------------------------------


class GuiMultiSlot
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
    GuiMultiSlot(GuiTable *_parentTable, int _numSlots);
    ~GuiMultiSlot();

    GuiWidget *parentWidget(){ return mParentWidget; }
    std::vector<GuiSlot*>& slots(){ return mSlots; }


private:
    GuiWidget *mParentWidget = nullptr;
    std::vector<GuiSlot*>mSlots;

    Vec2f position;

    //GuiTable::Layout mSlotsLayout = GuiTable::Layout::HORIZONTAL;
    //GuiTable::ItemInsertDirection mItemInsertDirection = GuiTable::ItemInsertDirection::LEFT_TO_RIGHT;

};




class GuiSlot
{
public:


    GuiSlot(GuiMultiSlot* _parentMultislot, CompositeSprite* _rootSprite, GuiButton *_selectingButton);
    //bool init();
    bool initConnections(GuiSystem * _guiSystem,std::vector<TableSlotComponentCfg> &_slotComponents);

    ~GuiSlot();


    GuiMultiSlot* parentMultislot(){ return mParentMultislot; }
    CompositeSprite *rootSprite(){ return mRootSprite;}
    GuiButton* selectingButton(){ return mSelectingButton; }


    GameItem* item(){ return mGameItem; }
    void removeItem();
    void addItem(GameItem* _item);
    void updateComponents();

    void _setMarkedForRemoveItem(bool _marked){ mMarkedForRemoveItem = _marked; }
    bool _markedForRemoveItem(){ return mMarkedForRemoveItem; }

private:
    GuiMultiSlot *mParentMultislot = nullptr;                                 // LINK
    CompositeSprite * mRootSprite = nullptr;                            // LINK
    GuiButton* mSelectingButton = nullptr;                              // OWNED

    //---
    std::vector<std::pair<std::string, Sprite*>>mIcSprites;             // LINKS (cache for quicker access)
    std::vector<std::pair<std::string, TextSprite*>>mIcTextSprites;       // LINKS (cache for quicker access)
    std::vector<std::pair<std::string, GuiTextField*>>mIcTextFields;     // OWNED

    //---
    GameItem* mGameItem = nullptr;                                       // LINK to currently stored item or nulltpr if none

    bool mMarkedForRemoveItem = false;

};



struct SelectingSlotCallback : public GuiWidgetCallback
{

    SelectingSlotCallback(GuiSlot* _slot) : mSlot(_slot) {}


    void OnPressedStarted(jugimap::GuiWidget *_widget) override;


    GuiSlot *mSlot = nullptr;

};



//------------------------------------------------------------------------------------------


class TableFactory : public WidgetFactory
{
public:

    TableFactory(const std::string &_typeSignature, WidgetType _type) : WidgetFactory(_typeSignature, _type){}

    GuiWidgetCfg* createWidgetCfg(const std::string &_name) override;
    GuiWidget* createWidget(GuiWidgetCfg* _cfg, Sprite *_rootSprite) override;

};


}




#endif
