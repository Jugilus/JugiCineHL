#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmSprite.h"
#include "jmCompositeSprite.h"
#include "jmStreams.h"
#include "jpItemsTable.h"
#include "jmUtilities.h"
#include "jpVariables.h"


namespace jugimap{




VariableType GetVariableTypeFromString(const std::string &_type)
{

    if(_type=="integer"){
        return VariableType::INTEGER;

    }else if(_type=="float"){
        return VariableType::FLOAT;

    }else if(_type=="bool"){
        return VariableType::BOOL;

    }else if(_type=="string"){
        return VariableType::STRING;

    }else if(_type=="integerVector"){
        return VariableType::INTEGER_VECTOR;

    }else if(_type=="floatVector"){
        return VariableType::FLOAT_VECTOR;

    }else if(_type=="boolVector"){
        return VariableType::BOOL_VECTOR;

    }else if(_type=="stringVector"){
        return VariableType::STRING_VECTOR;

    }

    return VariableType::STRING;

}


std::string GetVariableTypeString(VariableType _type)
{

    if(_type==VariableType::INTEGER){
        return "integer";

    }else if(_type==VariableType::FLOAT){
        return "float";

    }else if(_type==VariableType::BOOL){
        return "bool";

    }else if(_type==VariableType::STRING){
        return "string";

    }else if(_type==VariableType::INTEGER_VECTOR){
        return "integerVector";

    }else if(_type==VariableType::FLOAT_VECTOR){
        return "floatVector";

    }else if(_type==VariableType::BOOL_VECTOR){
        return "boolVector";

    }else if(_type==VariableType::STRING_VECTOR){
        return "stringVector";

    }

    return "string";

}


//----------------------------------------------------------------------------------------


Variable* Variable::copy(Variable* _src)
{


    if(_src->type()==VariableType::INTEGER){
        return new IntVar(*(static_cast<IntVar*>(_src)));

    }else if(_src->type()==VariableType::FLOAT){
        return new FloatVar(*(static_cast<FloatVar*>(_src)));

    }else if(_src->type()==VariableType::BOOL){
        return new BoolVar(*(static_cast<BoolVar*>(_src)));

    }else if(_src->type()==VariableType::STRING){
        return new StringVar(*(static_cast<StringVar*>(_src)));

    }else if(_src->type()==VariableType::INTEGER_VECTOR){
        return new IntVarVector(*(static_cast<IntVarVector*>(_src)));

    }else if(_src->type()==VariableType::FLOAT_VECTOR){
        return new FloatVarVector(*(static_cast<FloatVarVector*>(_src)));

    }else if(_src->type()==VariableType::BOOL_VECTOR){
        return new BoolVarVector(*(static_cast<BoolVarVector*>(_src)));

    }else if(_src->type()==VariableType::STRING_VECTOR){
        return new StringVarVector(*(static_cast<StringVarVector*>(_src)));

    }

    assert(false);
    return nullptr;
}


void Variable::setValueFromString(Variable *_var, const std::string &_value)
{
    if(_var->type()==VariableType::INTEGER){
        IntVar *v = static_cast<IntVar*>(_var);
        v->setValue(StdString::stringToInt(_value, 0));

    }else if(_var->type()==VariableType::FLOAT){
        FloatVar *v = static_cast<FloatVar*>(_var);
        v->setValue(StdString::stringToFloat(_value, 0.0f));

    }else if(_var->type()==VariableType::BOOL){
        BoolVar *v = static_cast<BoolVar*>(_var);
        v->setValue(StdString::stringToBool(_value));

    }else if(_var->type()==VariableType::STRING){
        StringVar *v = static_cast<StringVar*>(_var);
        v->setValue(_value);

    }else if(_var->type()==VariableType::INTEGER_VECTOR){
        assert(false);

    }else if(_var->type()==VariableType::FLOAT_VECTOR){
        assert(false);

    }else if(_var->type()==VariableType::BOOL_VECTOR){
        assert(false);

    }else if(_var->type()==VariableType::STRING_VECTOR){
        assert(false);

    }

}




bool Variable::haveSameValues(Variable *_var1, Variable *_var2)
{

    assert(_var1->type() == _var2->type());

    if(_var1->type()==VariableType::INTEGER){
        return (static_cast<IntVar*>(_var1)->value() == static_cast<IntVar*>(_var2)->value());

    }else if(_var1->type()==VariableType::FLOAT){
        return AreEqual(static_cast<FloatVar*>(_var1)->value(), static_cast<FloatVar*>(_var2)->value());

    }else if(_var1->type()==VariableType::BOOL){
        return (static_cast<BoolVar*>(_var1)->value() == static_cast<BoolVar*>(_var2)->value());

    }else if(_var1->type()==VariableType::STRING){
        return (static_cast<StringVar*>(_var1)->value() == static_cast<StringVar*>(_var2)->value());

    }else{

        assert(false);
    }

    return false;
}



//-----------------------------------------------------------------------------------------


void IntVar::writeAsIniEntry(TextStreamWriter &_writer)
{
    _writer.WriteLine(mName + ": " + std::to_string(mValue));
}


void FloatVar::writeAsIniEntry(TextStreamWriter &_writer)
{
    _writer.WriteLine(mName + ": " + std::to_string(mValue));
}



void BoolVar::writeAsIniEntry(TextStreamWriter &_writer)
{

    _writer.WriteLine(mName + ": " +  std::to_string(int(mValue)));
}


void StringVar::writeAsIniEntry(TextStreamWriter &_writer)
{
    _writer.WriteLine(mName + ": " + mValue);
}


void IntVarVector::writeAsIniEntry(TextStreamWriter &_writer)
{

    std::string text;
    for(int i : mValue){
         StdString::appendNonEmptyString(text, std::to_string(i), ",");
    }

    _writer.WriteLine(mName + ": " + text);
}


void FloatVarVector::writeAsIniEntry(TextStreamWriter &_writer)
{
    std::string text;
    for(float f : mValue){
         StdString::appendNonEmptyString(text, std::to_string(f), ",");
    }

    _writer.WriteLine(mName + ": " + text);
}



void BoolVarVector::writeAsIniEntry(TextStreamWriter &_writer)
{
    std::string text;
    for(bool b : mValue){
         StdString::appendNonEmptyString(text, std::to_string(int(b)), ",");
    }

    _writer.WriteLine(mName + ": " +  text);
}


void StringVarVector::writeAsIniEntry(TextStreamWriter &_writer)
{
    std::string text;
    for(const std::string s : mValue){
         StdString::appendNonEmptyString(text, s, ",");
    }

    _writer.WriteLine(mName + ": " + text);
}


//----------------------------------------------------------------------------------------


VariableManager::VariableManager()
{

}


VariableManager::~VariableManager()
{
    deleteContent();
}


void VariableManager::deleteContent()
{

    for(Variable *v : mVariables){

        if(v->type()==VariableType::INTEGER){
            delete static_cast<IntVar*>(v);

        }else if(v->type()==VariableType::FLOAT){
            delete static_cast<FloatVar*>(v);

        }else if(v->type()==VariableType::BOOL){
            delete static_cast<BoolVar*>(v);

        }else if(v->type()==VariableType::STRING){
            delete static_cast<StringVar*>(v);

        }
    }
}


Variable *VariableManager::addVariable(const std::string &_name, const std::string &_value, VariableType _type)
{

    if(getVariable(_name, false)){
        dbgSystem.addMessage("An variable with name '" + _name +"' already exists!");
        return nullptr;
    }

    Variable *var = _addVariable(_name, _value, _type);

    /*
    Variable *var = nullptr;

    if(_type==VariableType::INTEGER){

        int value = StdString::stringToInt(_value,0);
        var = new IntVar(_name, value);


    }else if(_type==VariableType::FLOAT){

        float value = StdString::stringToFloat(_value,0.0f);
        var = new FloatVar(_name, value);


    }else if(_type==VariableType::BOOL){

        bool value = StdString::stringToBool(_value);
        var = new BoolVar(_name, value);


    }else if(_type==VariableType::STRING){

        var = new StringVar(_name, _value);


    }else if(_type==VariableType::INTEGER_VECTOR){

        assert(false);      // need to do

    }else if(_type==VariableType::FLOAT_VECTOR){

        assert(false);      // need to do


    }else if(_type==VariableType::BOOL_VECTOR){

        assert(false);      // need to do

    }else if(_type==VariableType::STRING_VECTOR){

        assert(false);      // need to do

    }

    mVariables.push_back(var);
    */

    return var;
}


Variable *VariableManager::_addVariable(const std::string &_name, const std::string &_value, VariableType _type)
{

    Variable *var = nullptr;

    if(_type==VariableType::INTEGER){

        int value = StdString::stringToInt(_value,0);
        var = new IntVar(_name, value);


    }else if(_type==VariableType::FLOAT){

        float value = StdString::stringToFloat(_value,0.0f);
        var = new FloatVar(_name, value);


    }else if(_type==VariableType::BOOL){

        bool value = StdString::stringToBool(_value);
        var = new BoolVar(_name, value);


    }else if(_type==VariableType::STRING){

        var = new StringVar(_name, _value);


    }else if(_type==VariableType::INTEGER_VECTOR){

        assert(false);      // need to do

    }else if(_type==VariableType::FLOAT_VECTOR){

        assert(false);      // need to do


    }else if(_type==VariableType::BOOL_VECTOR){

        assert(false);      // need to do

    }else if(_type==VariableType::STRING_VECTOR){

        assert(false);      // need to do

    }

    mVariables.push_back(var);

    return var;
}

Variable * VariableManager::addVariable(Variable *_var)
{

    if(getVariable(_var->name(), false)){
        dbgSystem.addMessage("An variable with name '" + _var->name() +"' already exists!");
        return nullptr;
    }

    mVariables.push_back(_var);

    return _var;
}


Variable * VariableManager::addAnonymousVariable(const std::string &_name, const std::string &_value, VariableType _type)
{
    return _addVariable(_name, _value, _type);
}



Variable * VariableManager::addAnonymousVariable(Variable *_var)
{

    mVariables.push_back(_var);
    return _var;
}


Variable* VariableManager::getVariable(const std::string &_name, bool _setErrorMessage)
{

    for(Variable* var : mVariables){
        if(var->name()==_name){
            return var;
        }
    }

    if(_setErrorMessage){
        dbgSystem.addMessage("A variable with name '" + _name + "' not found!");
    }

    return nullptr;
}


//----------------------------------------------------------------------------------


TriggerVariable::TriggerVariable(const std::string &_name, Variable *_variable, bool _ownVariable)
{

    mName = _name;
    mVariable = _variable;
    mVariableOwned = _ownVariable;
}



TriggerVariable::~TriggerVariable()
{
    if(mVariableOwned){
        delete mVariable;
    }

}



void TriggerVariable::update()
{
    mTriggered = false;

    if(mSetTriggered){
        mTriggered = true;
        mSetTriggered = false;      // ! to separate these triggers from new added in the updated frame

        print("Trigger '" + mName +"' triggered!");
    }
}


bool TriggerVariable::resetPerUpdateFlags()
{

    if(mSetTriggered==false){   //
        mTriggered = false;
        return true;
    }

    // trigger has been added during the current frame - it will be processed next frame
    return false;

}


//----------------------------------------------------------------------------------


TriggerVariableManager::TriggerVariableManager()
{

}



TriggerVariableManager::~TriggerVariableManager()
{
    deleteContent();
}


void TriggerVariableManager::deleteContent()
{
    for(TriggerVariable* t : mTriggerVariables){
        delete t;
    }
    mTriggerVariables.clear();
}


TriggerVariable * TriggerVariableManager::addTrigger(const std::string &_name)
{

    if(getTrigger(_name, false)){
        dbgSystem.addMessage("A trigger with name '" + _name +"' already exists!");
        return nullptr;
    }

    TriggerVariable *t = new TriggerVariable(_name);
    mTriggerVariables.push_back(t);

    return t;
}



TriggerVariable* TriggerVariableManager::getTrigger(const std::string &_name, bool _setErrorMessage)
{

    for(TriggerVariable* var : mTriggerVariables){
        if(var->name()==_name){
            return var;
        }
    }

    if(_setErrorMessage){
        dbgSystem.addMessage("A trigger with name '" + _name + "' not found!");
    }

    return nullptr;
}


//----------------------------------------------------------------------------------


void ActivatedTriggersUpdater::update()
{
    for(TriggerVariable* t : mTriggerVariables){
        t->update();
    }
}


void ActivatedTriggersUpdater::resetPerUpdateFlags()
{

    for(int i = int(mTriggerVariables.size())-1; i>=0; i-- ){
        TriggerVariable *t = mTriggerVariables[i];
        if(t->resetPerUpdateFlags()){
            mTriggerVariables.erase(mTriggerVariables.begin()+i);
        }
    }
}


void ActivatedTriggersUpdater::addTrigger(TriggerVariable* _trigger)
{

    _trigger->doSetTriggered();
    if(StdVector::contains(mTriggerVariables, _trigger)==false){
        mTriggerVariables.push_back(_trigger);
    }

}


}
