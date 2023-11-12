#include <chrono>
#include <assert.h>
#include <sstream>
#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmMap.h"
#include "jmCamera.h"
#include "jmSceneLayout.h"
#include "jmCommonFunctions.h"
//#include "jmText.h"
#include "jmSpriteLayer.h"
#include "jmStandardSprite.h"
#include "jmCompositeSprite.h"
#include "jmVectorLayer.h"
#include "jmVectorShape.h"
#include "jmSprite.h"
#include "jmSourceSprite.h"
#include "jmUtilities.h"

#include "items/jpItemsCommon.h"
#include "jpPlayedApp.h"
#include "jpPlayedScene.h"
//#include "jpItemsTable.h"
#include "jpUtilities.h"
#include "jpLogicState.h"
#include "jpGlobal.h"
#include "jpQueries.h"
#include "gfxObjects/jpAnimationObjects.h"

#include "jpGuiCommon.h"
#include "widgets/jpGuiButton.h"
#include "widgets/jpGuiSlider.h"
#include "widgets/jpGuiTable.h"
#include "widgets/jpGuiTextField.h"
#include "widgets/jpGuiTextInput.h"
#include "widgets/jpGuiBar.h"

#include "jpGuiSystem.h"
#include "jpGuiUtilities.h"



namespace jugimap{





void WidgetSignalParser::parseSignalAccessor(PlayedScene *scene, const std::string &_path, SignalAccessor &_signalAccessor, BaseObject *obj1, BaseObject *obj2)
{


    GuiSystem *guiSystem = dynamic_cast<GuiSystem*>(scene->componentsGroup()->getComponent("guiSystem"));
    assert(guiSystem);

    std::vector<std::string>pathParts = StdString::splitString(_path, "/");

    std::string signalOrigin;
    std::string signalPath;
    std::string signalStyle;


    if(pathParts.size()>0){
        signalOrigin = pathParts[0];
    }
    if(pathParts.size()>1){
        signalPath = pathParts[1];
    }
    if(pathParts.size()>2){
        signalStyle = pathParts[2];
    }


    //-----

    GuiWidget *widget = nullptr;
    std::string widgetName;

    if(signalOrigin.substr(0,7)=="BUTTON:"){

        widgetName = signalOrigin.substr(7);
        widget = guiSystem->findWidget(widgetName, WidgetType::BUTTON);


    }else if(signalOrigin.substr(0,7)=="SLIDER:"){

        widgetName = signalOrigin.substr(7);
        widget = guiSystem->findWidget(widgetName, WidgetType::SLIDER);


    }else if(signalOrigin.substr(0,11)=="TEXT_FIELD:"){

        widgetName = signalOrigin.substr(11);
        widget = guiSystem->findWidget(widgetName, WidgetType::TEXT_FIELD);


    }else if(signalOrigin.substr(0,11)=="TEXT_INPUT:"){

        widgetName = signalOrigin.substr(11);
        widget = guiSystem->findWidget(widgetName, WidgetType::TEXT_INPUT);


    }else if(signalOrigin.substr(0,4)=="BAR:"){

        widgetName = signalOrigin.substr(4);
        widget = guiSystem->findWidget(widgetName, WidgetType::BAR);


    }else if(signalOrigin.substr(0,6)=="TABLE:"){

        widgetName = signalOrigin.substr(6);
        widget = guiSystem->findWidget(widgetName, WidgetType::TABLE);

    }else{

        dbgSystem.addMessage("Wrong signal origin '" + signalOrigin +"' !");
        return;
    }


    if(widget==nullptr){
        return;
    }


    ParsedSignalPath psp(signalPath);
    //psp.parsePath_name_value(signalPath);

    /*
    std::string signalName;
    std::string signalValue;

    std::vector<std::string> signalParts = StdString::splitString(signalPath, ":");

    if(signalParts.size()>0){
        signalName = signalParts[0];
    }
    if(signalParts.size()>1){
        signalValue = signalParts[1];
    }
    */

    //---
    if(_signalAccessor.mType==SignalAccessor::Type::QUERY){
        widget->obtainSignal_signalQuery(static_cast<SignalQuery&>(_signalAccessor), psp);

    }else if(_signalAccessor.mType==SignalAccessor::Type::SETTER){
        widget->obtainSignal_signalSetter(static_cast<SignalSetter&>(_signalAccessor), psp);

    }

    if(_signalAccessor.mSignal){
        return;
    }


    dbgSystem.addMessage("Error parsing '" + _path +"' !");
    return;


}



}
