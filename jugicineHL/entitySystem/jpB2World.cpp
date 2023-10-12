#include <chrono>
#include <assert.h>
#include <sstream>
#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmMap.h"
#include "jmCamera.h"
#include "jmSceneLayout.h"
#include "jmCommonFunctions.h"
#include "jmSourceContainers.h"
#include "jmVectorShape.h"
#include "jmTextSprite.h"
#include "jmUtilities.h"
#include "items/jpItemsCommon.h"
#include "jpPlayedApp.h"
#include "jpPlayedScene.h"
#include "jpItemsTable.h"
#include "jpUtilities.h"
#include "jpActionsCommon.h"


#include "scripting/jpBehavior.h"
#include "jpB2World.h"



namespace jugimap{



void WorldInfo::init()
{
    //mB2World.reset(new b2World({0.0f, 0.0f}));
}



WorldInfo gWorldInfo;








}
