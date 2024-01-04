#include <chrono>
#include <assert.h>
#include <sstream>
#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmSprite.h"
#include "jmSpriteLayer.h"
#include "jmTextSprite.h"
#include "jmCommonFunctions.h"
#include "jmSceneLayout.h"
#include "jmMap.h"
#include "jmUtilities.h"

#include "jpQueries.h"
#include "jpPlayedApp.h"
#include "jpPlayedScene.h"
#include "jpUtilities.h"
#include "jpObjectParser.h"
#include "jpMoving.h"



namespace jugimap{






Moving::Moving()
{

}


Moving::Moving(MovingCfg *_cfg, BaseObject *_parentObject) : mCfg(_cfg), mParentObject(_parentObject)
{

}



Moving::~Moving()
{
    /*
    for(auto &p : mData){
        delete p.second;
    }
    */

}



//===========================================================================================


bool LinearMovingCfg::initCfg(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading mover '" + mName +"' ...");


    //-----
    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());

        /*
        if(nodeName=="int"){

            IntItemData *sid = new IntItemData(n.attribute("name").as_string());
            if(sid->initCfg(n)==false){
                return false;
            }
            if(data.addObject(sid)==nullptr){
                return false;
            }


        }else if(nodeName=="float"){

            FloatItemData *sid = new FloatItemData(n.attribute("name").as_string());
            if(sid->initCfg(n)==false){
                return false;
            }
            if(data.addObject(sid)==nullptr){
                return false;
            }


        }else if(nodeName=="bool"){

            BoolItemData *sid = new BoolItemData(n.attribute("name").as_string());
            if(sid->initCfg(n)==false){
                return false;
            }
            if(data.addObject(sid)==nullptr){
                return false;
            }


        }else
        */
        if(nodeName=="new"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="sourceSprite"){
                    sourceSprite = a.as_string();

                }else if(attributeName=="animation"){
                    animation = a.as_string();

                }else if(attributeName=="layer"){
                    layer = a.as_string();

                }else if(attributeName=="map"){
                    map = a.as_string();

                }else{

                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                    return false;
                }
            }

        /*
        }else if(nodeName=="dataConnections"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="text"){
                    std::string sValue = a.as_string();
                    dataConnections.push_back(DataConnectionCfg(DataConnectionType::TEXT, sValue));

                }else{

                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                    return false;
                }
            }
        */

        }else{

            dbgSystem.addMessage("Unknown node '" + nodeName + "' !");
            return false;

        }
    }


    dbgSystem.removeLastMessage();
    return true;
}







}
