#ifndef PLAYER__SYSTEM__VARIOUS_H
#define PLAYER__SYSTEM__VARIOUS_H


#include <string>
#include <vector>

#include "jmGuiCommon.h"
#include "jpVariables.h"
#include "jpGlobal.h"



namespace jugimap{


class PlayedApp;





struct DelayedTask
{

    enum class Task
    {
        NONE,
        SAVE_SETTINGS_INI_FILE,
        SAVE_ACTIVE_USER
    };


    DelayedTask(Task _task, int _timeStepS) : mTask(_task), mTimeStepS(_timeStepS){}

    Task mTask = Task::NONE;
    int mTimeStepS = 4;

};



class DelayedTaskManager
{
public:

    DelayedTaskManager();
    ~DelayedTaskManager();
    void update();

    void addDelayedTask(DelayedTask _delayedTask);
    bool containsDelayedTask(DelayedTask::Task _task);

    std::vector<DelayedTask> & tasks(){ return mDelayedTasks; }

private:
    std::vector<DelayedTask>mDelayedTasks;

};




//--------------------------------------------------------------



class ExtWidgetManager : public WidgetManager
{

    bool createWidgets(Map *_map) override;


};










}

#endif // PLAYER_BASE_OBJECTS_H
