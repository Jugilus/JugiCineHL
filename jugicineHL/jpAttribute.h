#ifndef PLAYER__ATTRIBUTE_H
#define PLAYER__ATTRIBUTE_H

#include <assert.h>
#include <string>
#include <vector>

#include "jmSignal.h"

#include "jpLogicState.h"
#include "jpLogicStateCfg.h"
#include "jpGlobal.h"


namespace pugi {

class xml_node;

}



namespace jugimap{




struct AttributeSetCfg
{

    AttributeSetCfg(const std::string &_name) : mName(_name){}

    bool initCfg(const pugi::xml_node &_node);

    const std::string & name(){ return mName; }




    struct AttributeCfg
    {
        SignalType type;
        std::string name;
        //---
        std::string minValue;
        std::string maxValue;
        std::string startValue;
    };

    std::string mName;
    std::vector<AttributeCfg>attributeCfgs;

};



class AttributeSet
{
public:

    AttributeSet(AttributeSetCfg *_cfg): mCfg(_cfg){}


    bool build();


    std::vector<Signal*> & attributeSignals(){return mAttributeSignals; }


private:
    AttributeSetCfg *mCfg = nullptr;            // link

    std::vector<Signal*>mAttributeSignals;

};




struct AttributeLogicStateCfg : public LogicStateCfg
{

    AttributeLogicStateCfg(const std::string &_name) : LogicStateCfg(_name){}

    bool initCfg(const pugi::xml_node &_node);

    LogicStateCfg* createLogicStateCfg(const std::string &_name) override;
    CustomLogicState* createCustomState(LogicState *_parentState) override;

};



//---------------------------------------------------------------------------------------


class AttributeCustomState : public CustomLogicState
{
public:


    AttributeCustomState(AttributeLogicStateCfg *_logicStateCfg, LogicState *_parent);

    void setAttributeSignals(std::vector<Signal*> & _attributeSignals, bool _owningAttributeSignals);
    bool initConnections(PlayedScene *_scene) override;

    void start() override;
    void finish() override;


private:
    LogicState *mParentState = nullptr;                      // LINK
    AttributeLogicStateCfg *mLogicStateCfg = nullptr;       // LINK


};



//---------------------------------------------------------------------------------------




class Attribute
{
public:



};









}


#endif
