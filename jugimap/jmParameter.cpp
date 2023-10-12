#include <assert.h>
#include "jmCommonFunctions.h"
#include "jmParameter.h"

namespace jugimap{




//--- PARAMETER VALUE
CParameter::CParameter(const std::string &text)
{
    
    int pos = text.find(":");
    if(pos!=std::string::npos){
        name = text.substr(0, pos);
        value = text.substr(pos+1);
    }else{
        name = text;
    }
    
    StdString::removeStringWhiteSpacesOnStartAndEnd(name);
    StdString::removeStringWhiteSpacesOnStartAndEnd(value);
}


/*
Parameter Parameter::Parse(const std::string &s)
{

    Parameter p;

    std::size_t pos = s.find(":");

    if(pos != std::string::npos){

        p.name = s.substr(0, pos);
        p.value = s.substr(pos+1);

    }else{
        p.name = s;
    }

    removeStringWhiteSpacesOnStartAndEnd(p.name);
    removeStringWhiteSpacesOnStartAndEnd(p.value);

    return p;

}

*/


std::string CParameter::typeString()
{

    switch (type) {
    case typeINTEGER:
        return "int";
    case typeFLOAT:
        return "float";
    case typeBOOLEAN:
        return "bool";
    case typeSTRING:
        return "string";
    case typeVALUES_SET:
        return "values set";
    }

    return "?";
}


int CParameter::typeFromString(const std::string &_type)
{
    if(_type=="int"){
        return typeINTEGER;
    }else if(_type=="float"){
        return typeFLOAT;
    }else if(_type=="bool"){
        return typeBOOLEAN;
    }else if(_type=="string"){
        return typeSTRING;
    }else if(_type=="values set"){
        return typeVALUES_SET;
    }

    return typeINTEGER;
}



CParameter CParameters::parameter(const std::string& _name)
{

    for(CParameter& pv : mList){
        if(pv.name==_name){
            return pv;
        }
    }

    return CParameter();
}



const std::string & CParameters::value(const std::string &_name, const std::string &_default)
{
    for(CParameter &p : mList){
        if(p.name==_name && p.active) return p.value;
    }
    return _default;
}


int CParameters::intValue(const std::string &_name, int _default)
{

    for(CParameter &p : mList){
        if(p.name==_name && p.active) return StdString::stringToInt(p.value, _default);
    }
    return _default;
}


float CParameters::floatValue(const std::string &_name, float _default)
{

    for(CParameter &p : mList){
        if(p.name==_name && p.active) return StdString::stringToFloat(p.value, _default);
    }
    return _default;
}


bool CParameters::boolValue(const std::string &_name, bool _default)
{

    for(CParameter &p : mList){
        if(p.name==_name && p.active) return StdString::stringToBool(p.value);
    }
    return _default;
}




/*
std::string TQParametersValues::Value(const std::string &_name, bool &parameterFound) const
{
    for(int i=0; i<mList.size(); i++){
        if(mList[i].name == _name){
            parameterFound = true;
            return mList[i].value;
        }
    }
    parameterFound = false;
    return "";
}
*/

/*
std::string TQParametersValues::ValueAndActive(const std::string &_name, const std::string &_default) const
{
    for(int i=0; i<mList.size(); i++){
        if(mList[i].name == _name && mList[i].active) return mList[i].value;
    }
    return _default;
}
*/

/*
int TQParametersValues::IntValue(const std::string &_name, int _default) const
{
    for(int i=0; i<mList.size(); i++){
        if(mList[i].name == _name) return std::stoi(mList[i].value);
    }
    return _default;
}


int TQParametersValues::ClampedIntValue(const std::string &_name, int _min, int _max, int _default) const
{
    for(int i=0; i<mList.size(); i++){
        if(mList[i].name == _name){
            int v = std::stoi(mList[i].value);
            //Clamp<int>(v, _min, _max);
            if(v<_min)
                v = _min;
            else if(v>_max)
                v = _max;
            return v;
        }
    }
    return _default;
}

*/


bool CParameters::setValue(const std::string &_name, const std::string &_value)
{
    for(int i=0; i<mList.size(); i++){
        if(mList[i].name == _name){
            mList[i].value = _value;
            return true;
        }
    }
    return false;

}


bool CParameters::remove(const std::string &_name)
{
    //int index = -1;
    for(int i=0; i<mList.size(); i++){
        if(mList[i].name == _name){
            //index = i;
            mList.erase(mList.begin()+i);
            return true;
            //break;
        }
    }
    //if(index>=0){
    //    List.erase(List.begin()+index);
    //    return true;
    //}
    return false;

}


bool CParameters::exists(const std::string &_name)
{
    //for(int i=0; i<mList.size(); i++){
    //    if(mList[i].name == _name) return true;
    //}

    for(CParameter &p : mList){
        if(p.name==_name && p.active) return true;
    }
    return false;

}


bool CParameters::exists(const std::string &_name, const std::string &_value)
{

    if(_value==""){
        return exists(_name);
    }

   // for(int i=0; i<mList.size(); i++){
    //    if(mList[i].name == _name && mList[i].value == _value) return true;
    //}

    for(CParameter &p : mList){
        if(p.name==_name && p.value==_value && p.active) return true;
    }

    return false;
}


/*
bool TQParametersValues::ActiveParameterExists(const std::string &_name)
{
    for(int i=0; i<mList.size(); i++){
        if(mList[i].name == _name && mList[i].active) return true;
    }

    return false;
}
*/

/*
bool TQParametersValues::SetActive(const std::string &_name, bool _active)
{
    for(TQParameterValue &pv : mList){
        if(pv.name == _name){
            pv.active = _active;
            return true;
        }
    }
    return false;
}
*/


/*
void TQParametersValues::AssignMissingVariablesToLoadedValues(TQParametersValues &SrcParameters)
{
    assert(mList.size()==SrcParameters.mList.size());

    // type in value sta ze naložena
    for(int i=0; i<mList.size(); i++){
        TQParameterValue &PV = mList[i];
        TQParameterValue &PVsrc = SrcParameters.mList[i];
        PV.name = PVsrc.name;
        PV.flags = PVsrc.flags;
        PV.valueMin = PVsrc.valueMin;
        PV.valueMax = PVsrc.valueMax;
        PV.valuesSet = PVsrc.valuesSet;
    }
}
*/

bool CParameters::hasSameStructure(const CParameters &_ParametersValues)
{
    if(mList.size()!= _ParametersValues.mList.size()) return false;
    for(int i=0; i<mList.size(); i++){
        if(mList[i].name!=_ParametersValues.mList[i].name || mList[i].type!=_ParametersValues.mList[i].type) return false;
        if(mList[i].valuesSet.size()!= _ParametersValues.mList[i].valuesSet.size()) return false;
        for(int j=0; j<mList[i].valuesSet.size(); j++){
            if(mList[i].valuesSet[j] != _ParametersValues.mList[i].valuesSet[j]) return false;
        }

    }
    return true;
}


void CParameters::syncParameters(CParameters &SrcParameters)
{
    if(hasSameStructure(SrcParameters)) return;


    std::vector<CParameter>NewList;

    for(CParameter &pSrc : SrcParameters){
        bool found = false;
        for(CParameter &p : mList){
            if(pSrc.name==p.name && pSrc.type==p.type){

                if(p.type==CParameter::typeVALUES_SET){
                    p.valuesSet = pSrc.valuesSet;
                    //if(PV.valuesSet.empty()==false && PV.valuesSet.contains(PV.value)==false){
                    if(p.valuesSet.empty()==false && GetIndexOfVectorElement(p.valuesSet, p.value)==false){
                        p.value = p.valuesSet.front();
                    }
                }

                NewList.push_back(p);
                found = true;
                break;
            }
        }
        if(found==false){
            NewList.push_back(pSrc);
        }

    }

    mList = NewList;

    resetTextSize();
}



void CParameters::resetTextSize()
{
    for(CParameter &p : mList){
        p.resetTextSize();
    }
}













}
