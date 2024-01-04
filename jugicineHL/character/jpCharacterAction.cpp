#include <limits>
#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmSprite.h"
#include "jmCompositeSprite.h"
#include "jmStreams.h"

#include "logic/jpLogicState.h"
#include "jpPlayedScene.h"
#include "jpPlayedApp.h"
#include "jpLogicAction.h"
#include "jpLogicStateCfg.h"

#include "data/jpItem.h"
#include "jpCharacterAction.h"





namespace jugimap{


/*


bool SingleItemDataCollector::initCfg(const pugi::xml_node &_node)
{


    for(pugi::xml_attribute a = _node.first_attribute(); a; a = a.next_attribute()){
        std::string attributeName =std::string(a.name());


        if(attributeName=="data"){
            mPath = a.as_string();



        }else if(attributeName=="factor"){
            std::string sValue = a.as_string();

            if(parseFloatData(sValue, mFactor)==false){
                return false;
            }

        }else{

            dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + std::string(_node.name()) +"' !");
            return false;
        }
    }
    return true;
}


bool SingleItemDataCollector::build(SourceItemsStorage *_sourceItemsStorage)
{


    std::vector<std::string> parts = StdString::splitString(mPath, "/");

    if(parts.size()!=2){
        dbgSystem.addMessage("Error parsing '" + mPath + "'!");
        return false;
    }


    const std::string & rootItemParameterName = parts[0];
    const std::string & path = parts[1];

    for(RootItemParameter &rip : mParentCollectorSet->rootItemParameters()){
        if(rootItemParameterName == rip.name()){
            mRootItemParameter = &rip;
            break;
        }
    }

    if(mRootItemParameter==nullptr){
        dbgSystem.addMessage("Root item parameter '" + rootItemParameterName +"' not recognized!");
        return false;
    }


    //----
    parts = StdString::splitString(path, ":");

    if(parts.size()<1){
        dbgSystem.addMessage("Error parsing '" + path + "'!");
        return false;
    }

    mItemDataCollectingHelper.itemDataName = parts.back();
    parts.pop_back();

    for(const std::string &s : parts){
        SourceItem *si = _sourceItemsStorage->sourceItems().getObject(s);
        if(si==nullptr){
            return false;
        }
        mItemDataCollectingHelper.childSourceItems.push_back(si);

    }


    return true;

}



bool SingleItemDataCollector::run(float &collected)
{

    assert(mRootItemParameter->rootItem());

    mItemDataCollectingHelper.childSourceItemsDepthIndex=0;
    mCollected = 0;

    //mRootItemParameter->rootItem()->collectData(mItemDataCollectingHelper, mCollected);


    return true;
}


//======================================================================================



bool ItemDataCollector::initCfg(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading item data collector '" + mName +"' ...");


    for(pugi::xml_attribute a = _node.first_attribute(); a; a = a.next_attribute()){
        std::string attributeName =std::string(a.name());

        if(attributeName=="name"){


        }else if(attributeName=="rootItems"){
            std::string sValue = a.as_string();
            std::vector<std::string>parts = StdString::splitString(sValue, ",");
            for(const std::string & s : parts){
                mRootItemParameters.emplace_back(s);
            }

        }else{

            dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + std::string(_node.name()) +"' !");
            return false;
        }
    }


    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());

        if(nodeName=="add"){

            mSingleCollectors.emplace_back(this);
            SingleItemDataCollector &d = mSingleCollectors.back();

            if(d.initCfg(n)==false){
                return false;
            }

        }else{

            dbgSystem.addMessage("Unknown node '" + nodeName + "' !");
            return false;

        }
    }


    dbgSystem.removeLastMessage();
    return true;
}


bool ItemDataCollector::build(SourceItemsStorage *_sourceItemsStorage)
{

    dbgSystem.addMessage("Building item data collector '" + mName +"' ...");


    for(SingleItemDataCollector &c : mSingleCollectors){
        if(c.build(_sourceItemsStorage)==false){
            return false;
        }
    }

    dbgSystem.removeLastMessage();
    return false;

}


bool ItemDataCollector::run(std::vector<Item *> &_rootItems)
{

    mCollectedValue = 0.0f;

    if(_rootItems.size()!=mRootItemParameters.size()){
        return false;
    }

    for(unsigned int i=0; i<_rootItems.size(); i++){
        mRootItemParameters[i].setRootItem(_rootItems[i]);
    }


    for(SingleItemDataCollector &c : mSingleCollectors){
        float collected = 0.0;
        if(c.run(collected)==false){
            return false;
        }

        mCollectedValue += collected;
    }


    return true;

}


//======================================================================================


ItemDataAction::ItemDataAction(const std::string &_name) : mName(_name)
{
    mBaseType = BaseObjectType::ITEM_DATA_ACTION;
}


bool ItemDataAction::initCfg(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading item action '" + mName +"' ...");


    for(pugi::xml_attribute a = _node.first_attribute(); a; a = a.next_attribute()){
        std::string attributeName =std::string(a.name());


        if(attributeName=="rootItems"){
           std::string sValue = a.as_string();
           //mRootItemParametersNames = StdString::splitString(sValue, ",");

           std::vector<std::string>parts = StdString::splitString(sValue, ",");
           for(const std::string & s : parts){
               //mRootItemParameters.emplace_back(s);
               mRootItemParameters.addObject(RootItemParameter(s));
           }


        }else{

            dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + std::string(_node.name()) +"' !");
            return false;
        }
    }


    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());

        if(nodeName=="itemDataCollector"){

            std::string name = _node.attribute("name").as_string();
            if(mCollectors.getObject(name,false)){
                dbgSystem.addMessage("Item data collector data with name '" + name +"' already exist!");
                return false;

            }

            ItemDataCollector *cd =  mCollectors.addObject(new ItemDataCollector(name));

            if(cd->initCfg( n)==false){
                return false;
            }


        }else if(nodeName=="a"){

            mActionsCfgs.push_back(ActionCfg());
            ActionCfg & actionCfg = mActionsCfgs.back();

            for(pugi::xml_attribute a = n.first_attribute(); a; a=a.next_attribute()){
                std::string aName = std::string(a.name());
                std::string aValue = std::string(a.as_string());

                if(aName=="disabled"){
                    actionCfg.mDisabled = a.as_bool(false);
                    continue;

                }else if(aName=="dbgId"){
                    actionCfg.mDbgId = aValue;
                    continue;
                }

                const std::string prefix = aName.substr(0,2);

                if(prefix=="on"){
                    actionCfg.mConditions.push_back(ActionConditionCfg(aName, aValue));

                }else if(prefix=="do"){
                    actionCfg.mCommands.push_back(ActionCommandCfg(aName, aValue));

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


bool ItemDataAction::build(SourceItemsStorage *_sourceItemsStorage)
{

    dbgSystem.addMessage("Building item action '" + mName +"' ...");

    for(ItemDataCollector* s : mCollectors.objects()){
        if(s->build(_sourceItemsStorage)==false){
            return false;
        }
    }


    //---
    for(ActionCfg &cfg : mActionsCfgs){
        Action *a = new Action(this);
        if(a->build(cfg)==false){
            return false;
        }
        mActions.push_back(a);
    }


    dbgSystem.removeLastMessage();
    return true;
}


bool ItemDataAction::run(std::vector<Item*> &_rootItems)
{


    //----
    UpdateMode updateMode;

    for(Action* ga : mActions){
        if(ga->update(updateMode)==true){
            //if(updateMode.loopAllStateActions==false){
            //    break;
            //}
        }
    }



    return true;

}


*/



}
