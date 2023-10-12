#ifndef PLAYER__VARIABLES_H
#define PLAYER__VARIABLES_H


#include <string>
#include <vector>


namespace jugimap{



class TextStreamWriter;

enum class VariableType
{
    INTEGER,
    FLOAT,
    BOOL,
    STRING,
    INTEGER_VECTOR,
    FLOAT_VECTOR,
    BOOL_VECTOR,
    STRING_VECTOR

};


VariableType GetVariableTypeFromString(const std::string &_type);
std::string GetVariableTypeString(VariableType _type);



class Variable
{
public:
    static Variable* copy(Variable *_src);
    static void setValueFromString(Variable* _var, const std::string &_value);
    static bool haveSameValues(Variable *_var1, Variable *_var2);

    virtual ~Variable(){}

    Variable(const std::string &_name) : mName(_name){}

    VariableType type(){ return mType; }
    const std::string &name(){ return mName;}

    virtual void writeAsIniEntry(TextStreamWriter &_writer) {}


protected:
    VariableType mType = VariableType::STRING;
    std::string mName;

};



class IntVar : public Variable
{
public:

    IntVar(const std::string &_name, int _value) : Variable(_name), mValue(_value){ mType = VariableType::INTEGER; }

    int value(){ return mValue; }
    void setValue(int _value){ mValue = _value; }
    void writeAsIniEntry(TextStreamWriter &_writer) override;

private:
    int mValue;
};



class FloatVar : public Variable
{
public:

    FloatVar(const std::string &_name, float _value) : Variable(_name), mValue(_value){ mType = VariableType::FLOAT; }

    float value(){ return mValue; }
    void setValue(float _value){ mValue = _value; }
    void writeAsIniEntry(TextStreamWriter &_writer) override;

private:
    float mValue;
};



class BoolVar : public Variable
{
public:

    BoolVar(const std::string &_name, bool _value) : Variable(_name), mValue(_value){ mType = VariableType::BOOL; }

    bool value(){ return mValue; }
    void setValue(bool _value){ mValue = _value; }
    void writeAsIniEntry(TextStreamWriter &_writer) override;

private:
    bool mValue;
};



class StringVar : public Variable
{
public:

    StringVar(const std::string &_name, const std::string &_value) : Variable(_name), mValue(_value){ mType = VariableType::STRING; }

    const std::string & value(){ return mValue; }
    void setValue(const std::string & _value){ mValue = _value; }
    void writeAsIniEntry(TextStreamWriter &_writer) override;

private:
    std::string mValue;

};



class IntVarVector : public Variable
{
public:

    IntVarVector(const std::string &_name, const std::vector<int>&_value) : Variable(_name), mValue(_value){ mType = VariableType::INTEGER_VECTOR; }

    std::vector<int> & value(){ return mValue; }
    void setValue(const std::vector<int> & _value){ mValue = _value; }
    void writeAsIniEntry(TextStreamWriter &_writer) override;

private:
    std::vector<int>mValue;

};



class FloatVarVector : public Variable
{
public:

    FloatVarVector(const std::string &_name, const std::vector<float>&_value) : Variable(_name), mValue(_value){ mType = VariableType::FLOAT_VECTOR; }

    std::vector<float> & value(){ return mValue; }
    void setValue(const std::vector<float> & _value){ mValue = _value; }
    void writeAsIniEntry(TextStreamWriter &_writer) override;

private:
    std::vector<float>mValue;

};



class BoolVarVector : public Variable
{
public:

    BoolVarVector(const std::string &_name, const std::vector<bool>&_value) : Variable(_name), mValue(_value){ mType = VariableType::BOOL_VECTOR; }

    std::vector<bool> & value(){ return mValue; }
    void setValue(const std::vector<bool> & _value){ mValue = _value; }
    void writeAsIniEntry(TextStreamWriter &_writer) override;

private:
    std::vector<bool>mValue;

};



class StringVarVector : public Variable
{
public:

    StringVarVector(const std::string &_name, const std::vector<std::string>&_value) : Variable(_name), mValue(_value){ mType = VariableType::STRING_VECTOR; }

    std::vector<std::string> & value(){ return mValue; }
    void setValue(const std::vector<std::string> & _value){ mValue = _value; }
    void writeAsIniEntry(TextStreamWriter &_writer) override;

private:
    std::vector<std::string>mValue;

};


//---------------------------------------------------------------------------


class VariableManager
{
public:

    VariableManager();
    ~VariableManager();
    void deleteContent();

    Variable* addVariable(const std::string &_name, const std::string &_value, VariableType _type);
    Variable* addVariable(Variable* _var);
    Variable* addAnonymousVariable(const std::string &_name, const std::string &_value, VariableType _type);
    Variable* addAnonymousVariable(Variable* _var);
    Variable *getVariable(const std::string &_name, bool _setErrorMessage = true);

    std::vector<Variable*>& variables(){ return mVariables;}

private:
    std::vector<Variable*>mVariables;
    Variable* _addVariable(const std::string &_name, const std::string &_value, VariableType _type);

};


//--------------------------------------------------------------------------------------------------------------


class TriggerVariable
{
public:
    friend class AppConfigurationLoader_xml;

    TriggerVariable(){}
    TriggerVariable(const std::string &_name) : mName(_name){}
    TriggerVariable(const std::string &_name, Variable *_variable, bool _ownVariable);

    ~TriggerVariable();

    //---
    void doSetTriggered(){ mSetTriggered = true; }

    void update();
    bool resetPerUpdateFlags();

    //---
    bool isTriggered(){ return mTriggered; }

    const std::string &name(){ return mName; }

    void setVariable(Variable *_variable, bool _ownVariable){ mVariable = _variable; mVariableOwned = _ownVariable; }
    Variable *variable(){ return mVariable; }

protected:
    std::string mName;
    bool mSetTriggered = false;
    bool mTriggered = false;
    //---
    Variable *mVariable = nullptr;      // LINK
    bool mVariableOwned= false;

};


//-----------------------------------------------------------------------------



class TriggerVariableManager
{
public:
    TriggerVariableManager();
    ~TriggerVariableManager();
    void deleteContent();

    TriggerVariable * addTrigger(const std::string &_name);
    TriggerVariable *getTrigger(const std::string &_name, bool _setErrorMessage = true);

private:
    std::vector<TriggerVariable*>mTriggerVariables;         // OWNED

};


//-----------------------------------------------------------------------------


class ActivatedTriggersUpdater
{
public:

    void update();
    void resetPerUpdateFlags();

    void addTrigger(TriggerVariable* _trigger);


private:
    std::vector<TriggerVariable*>mTriggerVariables;         // LINKS
};




}


#endif // PLAYER__VARIABLES_H
