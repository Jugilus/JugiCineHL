#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmGuiCommon.h"
#include "jmGuiBar.h"
#include "jmAnimationCommon.h"
#include "map/jmSpriteLayer.h"
#include "jmSystem.h"
#include "jmGlobal.h"
#include "jpUtilities.h"
#include "jpTriggers_Input.h"


namespace jugimap{





std::string EkeyPressed::type = "onKeyPressed";

EkeyPressed::EkeyPressed(Action *_parentGameAction, const pugi::xml_node &_node)
{
    mParentGameAction = _parentGameAction;
    mKeyName = _node.attribute("key").as_string("");
}


bool EkeyPressed::initConnections(PlayedScene *scene)
{
    dbgSystem.addMessage("init '" + type + "'");

    mKeyCode = keyboard.keyCodeForKeyName(mKeyName);

    if(mKeyCode==KeyCode::UNKNOWN){
        dbgSystem.addMessage("Wrong key '" + mKeyName + "'!");
        return false;
    }

    dbgSystem.removeLastMessage();
    return true;
}


bool EkeyPressed::isTriggered()
{

    bool result = keyboard.isKeyPressed(mKeyCode);

    return result;
}







}
