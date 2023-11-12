#include <chrono>
#include <assert.h>
#include <sstream>
#include "pugixml/pugixml.hpp"
#include "jmCommonFunctions.h"
#include "jmSystem.h"

#include "jpSourceEntity.h"
#include "jpEntity.h"
#include "jpEntityUtilities.h"
#include "jpTaskBase.h"


namespace jugimap{




TaskType TaskEngine::type() const
{
    return mFactory->type();

}


void TaskEngine::obtainSignal_signalQuery(SignalQuery &_signalQuery, ParsedSignalPath &_psp, bool _setErrorMessage)
{

    if(_setErrorMessage){
        dbgSystem.addMessage("There is no signal with name '" + _psp.signalFullName() + "' inside the task engine '" + mFactory->typeSignature()+"' !");
    }

}

void TaskEngine::obtainSignal_signalSetter(SignalSetter &_signalSetter, ParsedSignalPath &_psp, bool _setErrorMessage)
{
    if(_setErrorMessage){
        dbgSystem.addMessage("There is no signal with name '" + _psp.signalFullName() + "' inside the task engine '" + mFactory->typeSignature()+"' !");
    }
}






//=========================================================================================


TaskEngineManager::TaskEngineManager()
{

    mDummyTaskEngineFactory = new DummyTaskEngineFactory("dummyDecider", TaskType::NO_DECIDER_SET);
    mDummyTaskEngine = new DummyTaskEngine(mDummyTaskEngineFactory);
}



TaskEngineManager::~TaskEngineManager()
{

    for(TaskEngineCfg * cfg : mTaskEngineCfgs){
        delete cfg;
    }

    for(TaskEngineFactory * f : mFactories){
        delete f;
    }

    delete mDummyTaskEngine;
    delete mDummyTaskEngineFactory;
}


void TaskEngineManager::addTaskEngineFactory(TaskEngineFactory *_taskEngineFactory)
{
    mFactories.push_back(_taskEngineFactory);
}


TaskEngineCfg* TaskEngineManager::addTaskEngineCfg(const std::string &_typeSignature, const std::string &_name, bool _setErrorMessage)
{

    for(TaskEngineCfg * cfg : mTaskEngineCfgs){
        if(cfg->name == _name){
            if(_setErrorMessage){
                dbgSystem.addMessage("The name '" + _name + "' of a Decider configuration is already used! Every configuration must have unique name!");
            }
            return nullptr;
        }
    }


    //---
    TaskEngineCfg *cfg = nullptr;

    for(TaskEngineFactory* f : mFactories){
        if(f->typeSignature() == _typeSignature){
            cfg = f->createTaskEngineCfg(_name);
            break;
        }
    }

    if(cfg){
        mTaskEngineCfgs.push_back(cfg);
        return cfg;
    }


    if(_setErrorMessage){
        dbgSystem.addMessage("Type signature '" + _typeSignature + "' for a Decider configuration not recognized!");
    }
    return nullptr;

}


TaskEngineCfg* TaskEngineManager::getTaskEngineCfg(const std::string &_name, bool _setErrorMessage)
{

    for(TaskEngineCfg * cfg : mTaskEngineCfgs){
        if(cfg->name == _name){
            return cfg;
        }
    }


    if(_setErrorMessage){
        dbgSystem.addMessage("A Decider configuration with name '" + _name + "' not found!");
    }
    return nullptr;

}


TaskEngine* TaskEngineManager::createTaskEngine(const std::string &_typeSignature, bool _setErrorMessage)
{

    for(TaskEngineFactory* f : mFactories){
        if(f->typeSignature() == _typeSignature){
            return  f->createTaskEngine();
        }
    }

    if(_setErrorMessage){
        dbgSystem.addMessage("Type signature '" + _typeSignature + "' for a Decider not recognized!");
    }
    return nullptr;

}






}
