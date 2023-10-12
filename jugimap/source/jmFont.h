#ifndef JUGIMAP_FONT_H
#define JUGIMAP_FONT_H

#include "ncine/Texture.h"
#include "ncine/Font.h"
#include "ncine/TextNode.h"
#include "jmLayer.h"

#include "jmUsingDeclarations.h"

namespace jugimap {





///\ingroup MapElements
///\brief The Font class defines fonts used in TextSprite objects.
class Font
{
public:
    friend class SourceObjectsLoader_xml;
    friend class SystemSourceObjects;

    static std::string textForHeight;


    Font(){}
    Font(const Font &f) = delete;
    Font& operator=(const Font &f) = delete;

    ///\brief Destructor.
    virtual ~Font();


    bool loadExternalData();


    const std::string & name(){ return mName;}

    /// Returns the file path of this font.
    const std::string & relativeFilePath(){ return mRelativeFilePath; }


    int GetCommonPixelHeight(){ return mPixelHeightCommon; }


    virtual int GetPixelHeight(const std::string &s);
    virtual int GetPixelWidth(const std::string &s);
    virtual Vec2f GetPixelSize(const std::string &s);

    ncine::Font* GetNcFont(){ return mNcineFont; }


private:
    std::string mName;
    std::string mRelativeFilePath;
    int mPixelHeightCommon = 0;

    //--------
    ncine::Font* mNcineFont = nullptr;             // OWNED
    ncine::TextNode* mNcineTextNode = nullptr;      // OWNED
    std::string mText;


#ifdef JUGI_EDITOR

    friend class jugi::gui::TEditFontDialog;
    friend class jugi::gui::TFontsLibraryDialog;

    int flags = 0;

#endif

};



///\ingroup MapElements
/// \brief The FontsLibrary class defines storage for Font objects.
class FontsLibrary
{
public:
    friend class SourceObjectsLoader_xml;
    friend class SystemSourceObjects;


    ~FontsLibrary();
    void deleteContent();

    bool loadExternalData();



    std::vector<Font*>& fonts(){ return mFonts; }


    /// \brief Add the given font *_font* with identification name *_name* to this library. The function returns the index of the added font in the library.
    //void add(Font* _font){ mFonts.push_back(_font); };


    /// \brief Find and returns a font with the given *_name*.
    ///
    ///  If the font is not found the function returns the first font in the library, or nullptr if library is empty.
    Font* getFont(const std::string &_name);


    /// \brief Returns a font at the given *_index*.
    ///
    ///   If the index is out of storage bounds the function returns the first font in the library, or nullptr if library is empty.
    //Font* getFont(int _index);


    /// \brief Returns the number of stored fonts.
    //int GetSize(){ return mFonts.size(); }


    //Font* FindStyle(const std::string &_nameID);
    Font* GetSystemFont() { return systemFontStyle; }
    int indexOfStyle(const std::string &name);


private:
    std::vector<Font*> mFonts;                          // OWNED

    Font* systemFontStyle = nullptr;                    // OWNERSHIP by SystemSourceObjects
    ncine::Texture* mSystemFontTexture = nullptr;       // OWNEESHIP by SystemSourceObjects

};


/// \brief A global library for fonts.
extern FontsLibrary fontsLibrary;


}


#endif
