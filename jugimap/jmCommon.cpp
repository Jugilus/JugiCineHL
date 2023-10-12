#include <string>
#include <sstream>
#include "jmCommonFunctions.h"
#include "jmCommon.h"
#include "jmGlobal.h"



namespace jugimap {


/*

bool Parameter::Exists(const std::vector<Parameter> &parameters, const std::string &name, const std::string &value)
{
    for(const Parameter &P : parameters){
        if(P.name == name){
            if(value=="" || P.value==value){
                return true;
            }
        }
    }
    return false;
}


std::string Parameter::GetValue(const std::vector<Parameter> &parameters, const std::string &name, const std::string &defaultValue)
{

    for(const Parameter &P : parameters){
        if(P.name == name){
            return P.value;
        }
    }
    return defaultValue;

}


int Parameter::GetIntValue(const std::vector<Parameter> &parameters, const std::string &name, int defaultValue)
{
    for(const Parameter &P : parameters){
        if(P.name == name){
            return std::stoi(P.value);
        }
    }
    return defaultValue;
}


float Parameter::GetFloatValue(const std::vector<Parameter> &parameters, const std::string &name, float defaultValue)
{
    for(const Parameter &P : parameters){
        if(P.name == name){
            return std::stof(P.value);
        }
    }
    return defaultValue;

}


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



//=============================================================================================




}
