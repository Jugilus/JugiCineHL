#ifndef JUGIMAP_COLOR_H
#define JUGIMAP_COLOR_H

#include <ncine/Color.h>
#include <string>
#include <vector>
#include "jmUsingDeclarations.h"


namespace jugimap {





/// \addtogroup Common
/// @{

/// \brief Color in RGBA color space - integer variant.
struct ColorRGBA
{
    unsigned char r = 255;            ///< Red component of the color in the range 0 - 255.
    unsigned char g = 255;            ///< Green component of the color in the range 0 - 255.
    unsigned char b = 255;            ///< Red component of the color in the range 0 - 255.
    unsigned char a = 255;            ///< Alpha component of the color in the range 0 - 255.
    std::string name = "color";
    int flags = 0;


    /// Constructs a color with r=0, g=0, b=0 and a=0
    ColorRGBA(){}

    /// Constructs a color with the given **_r**, **_g**, **_b** and **_a**.
    ColorRGBA(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a=255) : r(_r),g(_g),b(_b),a(_a){}

    /// Constructs a color from the given **_rgba**.
    ColorRGBA(int _rgba)
    {
        r = _rgba & 0xFF;
        g = (_rgba >> 8) & 0xFF;
        b = (_rgba >> 16) & 0xFF;
        a = (_rgba >> 24) & 0xFF;
    }


    /// Returns true if the given **_colorRGBA** is equal to this color; otherwise return false;
    bool operator ==(ColorRGBA _other)
    {
        return (r==_other.r && g==_other.g && b==_other.b && a==_other.a && name==_other.name)? true : false;
    }

    /// Returns true if the given **_colorRGBA** is not equal to this color; otherwise return false;
    bool operator !=(ColorRGBA _other)
    {
        return (r==_other.r && g==_other.g && b==_other.b && a==_other.a && name==_other.name)? false : true;
    }


     /// Returns this color as an *rgba* integer value.
    int GetRGBA()  const
    {
        return  r | (g << 8) | (b << 16) | (a << 24);
    }

    /// Set the given **_r**, **_g**, **_b** and **_a**.
    void Set(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a)
    {
        r = _r;
        g = _g;
        b = _b;
        a = _a;
    }


    /// Set the color from the given **_rgba**.
    void Set(int _rgba)
    {
        r = _rgba & 0xFF;
        g = (_rgba >> 8) & 0xFF;
        b = (_rgba >> 16) & 0xFF;
        a = (_rgba >> 24) & 0xFF;
    }

    static ColorRGBA ParseFromHexString(std::string _hexColor);

    std::string toHexString(bool includeAlpha=true);

    ncine::Color toNcineColor(){ return ncine::Color(r,g,b,a);}


};


/// \brief Color in RGBA color space - float variant.
struct ColorRGBAf
{
    float r = 1.0;                  ///< The red component of the color in the range 0.0 - 1.0.
    float g = 1.0;                  ///< The green component of the color in the range 0.0 - 1.0.
    float b = 1.0;                  ///< The blue component of the color in the range 0.0 - 1.0.
    float a = 1.0;                  ///< The alpha component of the color in the range 0.0 - 1.0.


    /// Constructs a color with r=0.0, g=0.0, b=0.0 and a=0.0
    ColorRGBAf(){}

    /// Constructs a color with the given **_r**, **_g**, **_b** and **_a**.
    ColorRGBAf(float _r, float _g, float _b, float _a=1.0) : r(_r), g(_g), b(_b), a(_a){}

    /// Constructs a color from the given **_colorRGBA**.
    ColorRGBAf(const ColorRGBA &_colorRGBA): r(_colorRGBA.r/255.0), g(_colorRGBA.g/255.0), b(_colorRGBA.b/255.0), a(_colorRGBA.a/255.0){}


    bool operator ==(ColorRGBAf _colorRGBA);
    bool operator !=(ColorRGBAf _colorRGBA) { return !(operator ==(_colorRGBA)); }

};




class Color
{
public:
    friend class SourceObjectsLoader_xml;
    friend class SystemSourceObjects;



    Color(){}
    Color(const std::string &_name, ColorRGBA _colorRGBA) : mName(_name), mColorRGBA(_colorRGBA){}

    const std::string &name(){ return mName; }
    ColorRGBA colorRGBA(){ return mColorRGBA; }



private:
    std::string mName;
    ColorRGBA mColorRGBA;


#ifdef JUGI_EDITOR
public:
    friend class jugi::gui::TEditColorDialog;
    friend class jugi::gui::ColorsLibraryDialog;

    int flags = 0;

#endif


};



/// \brief The ColorsLibrary defines storage for ColorRGBA objects.
class ColorsLibrary
{
public:
    friend class SourceObjectsLoader_xml;
    friend class SystemSourceObjects;


    ~ColorsLibrary(){ deleteContent(); }

    void deleteContent();
    //JColor* FindColor(const jugimap::ColorRGBA &_color);
    ColorRGBA getColorRGBA(const std::string &_name);
    Color* getColor(const std::string &_name);
    Color* getDefaultColor();
    int indexOfColor(const Color *_color);


    std::vector<Color*>& colors(){ return mColors; }


    /// \brief Add the given color *_colorRGBA* with identification name *_name* to this library. The function returns the index of the added color in the library.
    void Add(Color *_color){ mColors.push_back(_color); }


    /// \brief Clear all colors in this library.
    void Clear(){ mColors.clear(); }


    /// \brief Find and returns a color with the given *_name*. If the color is not found the function returns white color.
    //Color Find(const std::string &_name);





    /// \brief Returns the color at the given index. If the index is out of storage bounds the white color is returned.
    //ColorRGBA At(int _index);


    /// \brief Returns the number of stored colors.
    int GetSize(){ return mColors.size(); }


private:

    std::vector<Color*> mColors;            // OWNED

    Color* mDefaultColor = nullptr;         // OWNEESHIP by SystemSourceObjects

};


/// \brief A global library for text colors.
extern ColorsLibrary textColorsLibrary;







/// @}      //end addtogroup Common


}


#endif
