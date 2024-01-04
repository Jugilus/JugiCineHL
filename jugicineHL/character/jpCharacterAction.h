#ifndef PLAYER__CHARACTER_ACTION_H
#define PLAYER__CHARACTER_ACTION_H

#include <assert.h>
#include <string>
#include <vector>

#include "jmSignal.h"
#include "jmStorage.h"

#include "jpGlobal.h"
#include "data/jpDataUtilities.h""
#include "data/jpData.h"
#include "data/jpItem.h"


namespace pugi {

class xml_node;

}



namespace jugimap{

/*

//class Character;
class Item;
class ItemDataAction;
//struct ItemSourceData;
struct ItemDataCollector;





//------------------------------------------------------------------------





class SingleItemDataCollector
{
public:
    SingleItemDataCollector(ItemDataCollector* _parent) : mParentCollectorSet(_parent){}


    bool initCfg(const pugi::xml_node &_node);
    bool build(SourceItemsStorage *_sourceItemsStorage);

    bool run(float & collected);


private:
    ItemDataCollector *mParentCollectorSet = nullptr;        // LINK

    ItemDataCollectingHelper mItemDataCollectingHelper;
    RootItemParameter *mRootItemParameter = nullptr;        // LINK
    float mFactor = 1.0f;
    float mCollected = 0.0f;

    //--- cfg
    std::string mPath;

};



class ItemDataCollector
{
public:

    ItemDataCollector(const std::string &_name) : mName(_name){}

    bool initCfg(const pugi::xml_node &_node);
    bool build(SourceItemsStorage *_sourceItemsStorage);


    bool run(std::vector<Item*> &_rootItems);

    float collectedValue(){ return mCollectedValue;}

    const std::string & name(){ return mName;}
    std::vector<SingleItemDataCollector> & singleCollectors(){ return mSingleCollectors; }
    std::vector<RootItemParameter> &rootItemParameters(){ return mRootItemParameters; }

private:
    std::string mName;
    std::vector<SingleItemDataCollector>mSingleCollectors;
    std::vector<RootItemParameter>mRootItemParameters;

    float mCollectedValue = 0;

};



//======================================================================



class ItemDataAction : public BaseObject
{
public:

    ItemDataAction(const std::string &_name);


    bool initCfg(const pugi::xml_node &_node);
    bool build(SourceItemsStorage *_sourceItemsStorage);

    bool run(std::vector<Item*> &_rootItems);

    const std::string & name(){ return mName;}

    SimpleNoNameStorage<ItemDataCollector*> & collectors(){ return mCollectors; }
    SimpleNoNameStorage<RootItemParameter> & rootItemParameters(){ return mRootItemParameters; }
    //std::vector<std::string> & rootItemParametersNames(){ return mRootItemParametersNames; }



private:
    std::string mName;
    //std::vector<Item*>mRootItems;                                     // LINKS
    SimpleNoNameStorage<ItemDataCollector*> mCollectors;                // OWNED


    std::vector<Action*>mActions;                                       // OWNED

    //std::vector<std::string>mRootItemParametersNames;
    SimpleNoNameStorage<RootItemParameter>mRootItemParameters;

    //cfg data
    std::vector<ActionCfg>mActionsCfgs;


};

*/





}


#endif
