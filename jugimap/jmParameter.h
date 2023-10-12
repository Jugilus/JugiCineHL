#ifndef JM_PARAMETER_H
#define JM_PARAMETER_H

#include <string>
#include <vector>

#include "jmCommon.h"


namespace jugimap{



//--- PARAMETER VALUE
struct CParameter
{
    static const int SAVE_PARAMETERS_VALUE_ONLY = 1;
    bool active = true;

    std::string name;
    std::string value;

    int type = typeINTEGER;
    static const int typeINTEGER = 0;
    static const int typeFLOAT = 1;
    static const int typeBOOLEAN = 2;
    static const int typeSTRING = 3;
    static const int typeVALUES_SET = 4;

    int flags = 0;
    //int static const NOT_EDITABLE = 1;
    //int static const NOT_DELETABLE = 2;

    std::string valueMin;
    std::string valueMax;
    std::vector<std::string> valuesSet;
    //QString defaultValue;

    //---
    //int nameSaveID = -1;

    //----
    Vec2i nameTextSize;
    Vec2i valueTextSize;


    CParameter(){}
    CParameter (const std::string &_name, const std::string &_value) : name(_name),value(_value){}
    CParameter (const std::string &_name, const std::string &_value, int _type) : name(_name),value(_value),type(_type){}
    CParameter (const std::string &_name, const std::string &_value, int _type, int _flags) : name(_name),value(_value),type(_type),flags(_flags){}
    
    //static CParameter Parse(const std::string &s);
    
    CParameter(const std::string& text);
    //bool empty(){ return name.empty(); }  const;

    //void SaveViaQString(TDataStream& Stream, int _flags=0);
    //void Save(TDataStream& Stream, int _flags=0);

    //void SaveOptimized(TDataStream& Stream, int _flags=0);
     //void ExportSave(TDataStream& Stream, int _flags=0);

    //bool LoadViaQStrings(TDataStream& Stream, int _flags=0);
    //bool Load(TDataStream& Stream, int _flags=0);

    std::string typeString();
    static int typeFromString(const std::string &_type);

    void resetTextSize() { nameTextSize.set(0.0f, 0.0f); valueTextSize.set(0.0f, 0.0f);}

    //bool SaveToXML(QXmlStreamWriter &Writer, const QString &_elementName);
    //bool SaveXml(pugi::xml_node &parentNode);
    //bool ReadFromXML(QXmlStreamReader &Reader);
    //bool ReadFromXML_pugi(pugi::xml_node &node);

};


class CParameters
{
public:

    CParameters(){}

    CParameter parameter(const std::string& _name);

    const std::string & value(const std::string& _name, const std::string& _default=std::string());
    int intValue(const std::string& _name, int _default = 0);
    float floatValue(const std::string& _name, float _default = 0.0);
    bool boolValue(const std::string& _name, bool _default = false);

    //std::string Value(const std::string& _name, bool &parameterFound) const;
    //std::string ValueAndActive(const std::string& _name, const std::string& _default=std::string()) const;
    //int IntValue(const std::string& _name, int _default=0) const;
    //int ClampedIntValue(const std::string& _name, int _min, int _max, int _default=0) const;
    bool setValue(const std::string& _name, const std::string& _value);
    bool exists(const std::string &_name);
    bool exists(const std::string &_name, const std::string &_value);

    //bool ActiveParameterExists(const std::string &_name);

    //bool SetActive(const std::string& _name, bool _active);

    //void AssignMissingVariablesToLoadedValues(TQParametersValues &SrcParameters);
    bool remove(const std::string& _name);
    void syncParameters(CParameters &SrcParameters);

    void resetTextSize();




    std::vector<CParameter>& list(){ return mList; }

    int size() const { return mList.size(); }
    bool empty() const { return mList.empty(); }
    void push_back(const CParameter &pv){ mList.push_back(pv); }
    void pop_back(){ mList.pop_back(); }
    CParameter & back(){ return mList.back(); }
    void clear(){ mList.clear();}
    CParameter & operator[](int index){ return mList[index]; }
    CParameter & at(int index) { return mList.at(index);}


    std::vector<CParameter>::iterator begin()    { return mList.begin(); }
    std::vector<CParameter>::iterator end()              { return mList.end(); }
    std::vector<CParameter>::const_iterator cbegin() const     { return mList.cbegin(); }
    std::vector<CParameter>::const_iterator cend() const       { return mList.cend(); }
    std::vector<CParameter>::const_iterator begin() const      { return mList.begin(); }
    std::vector<CParameter>::const_iterator end() const        { return mList.end(); }

private:
    std::vector<CParameter>mList;

    bool hasSameStructure(const CParameters &_ParametersValues);
};



}



#endif // JMPARAMETER_H
