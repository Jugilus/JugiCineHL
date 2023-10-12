#include "ncine/Application.h"
#include "ncine/IGfxDevice.h"
#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmGuiCommon.h"
#include "jmGuiText.h"
#include "jmGuiBar.h"
#include "jmAnimationCommon.h"
#include "jmSpriteLayer.h"
#include "jmCompositeSprite.h"
#include "jmUtilities.h"
#include "jmGlobal.h"
#include "jmStreams.h"
#include "jpItemsTable.h"
#include "jpPlayedApp.h"
#include "jpUser.h"
#include "jpSystemVarious.h"


namespace jugimap{




DelayedTaskManager::DelayedTaskManager()
{
}

DelayedTaskManager::~DelayedTaskManager()
{
}


void DelayedTaskManager::update()
{

    int passedS = time.GetPassedNetTimeMS()/1000;


    for(int i = int(mDelayedTasks.size())-1; i>=0; i-- ){
        DelayedTask &dt = mDelayedTasks[i];

        if( passedS % dt.mTimeStepS == 0 ){

            if(dt.mTask==DelayedTask::Task::SAVE_SETTINGS_INI_FILE){
                app->systemSettings()->saveSettingsIniFile();

            }else if(dt.mTask==DelayedTask::Task::SAVE_ACTIVE_USER){
                assert(app->usersDatabase()->activeUser());
                app->usersDatabase()->saveUser(app->usersDatabase()->activeUser());
            }



            //---
            mDelayedTasks.erase(mDelayedTasks.begin()+i);
        }
    }

}



void DelayedTaskManager::addDelayedTask(DelayedTask _delayedTask)
{

    if(containsDelayedTask(_delayedTask.mTask)==false){
        mDelayedTasks.push_back(_delayedTask);
    }

}


bool DelayedTaskManager::containsDelayedTask(DelayedTask::Task _task)
{

    for(DelayedTask &dt : mDelayedTasks){
        if(dt.mTask==_task){
            return true;
        }
    }

    return false;
}



//-----------------------------------------------------------------------------


bool ExtWidgetManager::createWidgets(Map *_map)
{

    if(WidgetManager::createWidgets(_map)==false){
        return false;
    }


    std::vector<Sprite*>sprites;
    CollectSpritesWithConstParameter(_map, sprites, "widget");

    for(Sprite *s : sprites){

        std::string widgetType = s->sourceSprite()->constantParameters().value("widget");

        GuiWidget *widget = nullptr;

        if(widgetType=="table" && s->type()==SpriteType::COMPOSITE){
            widget = new GuiTable(static_cast<CompositeSprite*>(s));
        }

        if(widget){
            if(widget->init()==false){
                return false;
            }
            widgets().push_back(widget);
        }



    }

    return  true;

}




}
