#ifndef PLAYER__ENTITY_SYSTEM__TASK__TASK_BASE__H
#define PLAYER__ENTITY_SYSTEM__TASK__TASK_BASE__H

#include <memory>
#include <string>
#include <vector>
#include "jpEntityGlobal.h"
#include "jpQueries.h"



namespace pugi{
class xml_node;
}

namespace jugimap{

class PlayedScene;
class Entity;
class TaskEngineFactory;





struct TaskEngineCfg
{

    TaskEngineCfg(TaskEngineFactory *_factory, const std::string &_name) : factory(_factory), name(_name){}
    virtual ~TaskEngineCfg(){}

    virtual bool initCfg(const pugi::xml_node &_node) = 0;

    TaskEngineFactory * factory = nullptr;     // LINK
    std::string name;

};


struct TaskEngineData
{

    virtual ~TaskEngineData(){}


    TaskEngineFactory * factory = nullptr;     // LINK

};



class TaskEngine
{
public:

    TaskEngine(TaskEngineFactory * _deciderFactory) : mFactory(_deciderFactory){}

    virtual ~TaskEngine(){}

    TaskType type() const;
    TaskEngineFactory * factory(){ return mFactory; }

    virtual void init(Entity* _entity) { mParentEntity =_entity; }

    virtual bool start(TaskEngineData *_data) = 0;
    virtual void createDataObjects(std::vector<TaskEngineCfg*> &_cfgs) = 0;
    virtual bool initDataObjectsConnections(PlayedScene *_scene, Entity *_actor) = 0;
    virtual TaskEngineData* getData(const std::string &_name, bool _setErrorMessage) = 0;
    virtual TaskEngineData* currentData() = 0;

    virtual void update(TaskUpdateParameters _tup) = 0;

    virtual void obtainSignal_signalQuery(SignalQuery &_signalQuery, const std::string &_data, const std::string &_signalName, const std::string &_signalValue, bool _setErrorMessage=true);
    virtual void obtainSignal_signalSetter(SignalSetter &_signalSetter, const std::string &_data, const std::string &_signalName, const std::string &_signalValue, bool _setErrorMessage=true);


    void preUpdateSignals(){ Signal::preUpdateSignals(mSignals); }
    void postUpdateSignals(){ Signal::postUpdateSignals(mSignals); }
    std::vector<Signal*> &signals(){ return mSignals; }



protected:
    TaskEngineFactory *mFactory = nullptr;         // LINK
    float mStateEllapsedTimeS = 0.0f;

    Entity *mParentEntity = nullptr;

    std::vector<Signal*>mSignals;                           // LINKS
};


//------------------------------------------------------------------------------------


class DummyTaskEngine : public TaskEngine
{
public:

    DummyTaskEngine(TaskEngineFactory * _factory) : TaskEngine(_factory){}
    virtual ~DummyTaskEngine(){}

    void init(Entity*) override {}
    bool start(TaskEngineData *) override { return true; }
    void createDataObjects(std::vector<TaskEngineCfg*> &) override {}
    bool initDataObjectsConnections(PlayedScene *, Entity *) override { return true; }
    void update(TaskUpdateParameters ) override {}
    TaskEngineData* getData(const std::string &, bool ) override { return nullptr; }
    TaskEngineData* currentData() override { return nullptr; }


};



//------------------------------------------------------------------------------------

class TaskEngineFactory
{
public:

    virtual ~TaskEngineFactory(){}


    TaskEngineFactory(const std::string &_typeSignature, TaskType _type) : mTypeSignature(_typeSignature), mType(_type){}

    const std::string & typeSignature(){ return mTypeSignature;}
    TaskType type(){ return mType; }


    virtual TaskEngineCfg* createTaskEngineCfg(const std::string &_name) = 0;
    virtual TaskEngine* createTaskEngine() = 0;

    virtual void obtainCustomSignal_signalQuery(SignalQuery &_signalQuery, const std::string &_signalName, const std::string &_signalValue){}
    virtual void obtainCustomSignal_signalSetter(SignalSetter &_signalSetter, const std::string &_signalName, const std::string &_signalValue){}


protected:
    std::string mTypeSignature;
    TaskType mType = TaskType::UNKNOWN;

};



class DummyTaskEngineFactory : public TaskEngineFactory
{
public:

    DummyTaskEngineFactory(const std::string &_typeSignature, TaskType _type) : TaskEngineFactory(_typeSignature, _type){}

    TaskEngineCfg* createTaskEngineCfg(const std::string &) override { return nullptr; }
    TaskEngine* createTaskEngine() override { return nullptr;}
};


//------------------------------------------------------------------------------------


class TaskEngineManager
{
public:

    TaskEngineManager();
    ~TaskEngineManager();


    void addTaskEngineFactory(TaskEngineFactory *_taskEngineFactory);

    TaskEngineCfg* addTaskEngineCfg(const std::string &_typeSignature, const std::string &_name, bool _setErrorMessage=true);
    TaskEngine* createTaskEngine(const std::string &_typeSignature, bool _setErrorMessage=true);
    TaskEngineCfg* getTaskEngineCfg(const std::string &_name, bool _setErrorMessage=true);
    DummyTaskEngine* getDummyTaskEngine(){ return mDummyTaskEngine; }

private:
    std::vector<TaskEngineFactory*>mFactories;
    std::vector<TaskEngineCfg*>mTaskEngineCfgs;
    DummyTaskEngineFactory * mDummyTaskEngineFactory = nullptr;
    DummyTaskEngine *mDummyTaskEngine = nullptr;
};








}






#endif
