#ifndef PLAYER__LOGIC__COMMON_H
#define PLAYER__LOGIC__COMMON_H

#include <memory>
#include <string>
#include <vector>

#include "jmSignal.h"

#include "jpGlobal.h"



namespace pugi{
class xml_node;
}

namespace jugimap{


class PlayedScene;
class Action;
class LogicState;
class LogicFunction;



class FunctionArgument
{
public:

    FunctionArgument(const std::string &_name, BaseObjectType _type) : mName(_name), mType(_type){}

    const std::string & name() const { return mName; }
    BaseObjectType type(){ return mType; }

    const std::string & objectName() const { return mObjectName; }
    void setObjectName(const std::string & _objectName){ mObjectName = _objectName; }

    BaseObject * object(){ return mObject; }
    void setObject(BaseObject *_object){ mObject = _object; }


private:
    std::string mName;
    BaseObjectType mType = BaseObjectType::NOT_DEFINED;
    std::string mObjectName;
    BaseObject * mObject = nullptr;     // LINK

};




}


#endif
