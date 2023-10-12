#ifndef JUGIMAP_GUI_TEXT_H
#define JUGIMAP_GUI_TEXT_H

#include <assert.h>
#include <algorithm>
#include <cmath>
#include <vector>
#include <map>
#include <array>
#include <string>


#include "jmGuiCommon.h"
#include "jmFont.h"
#include "jmParameter.h"
#include "jmCommon.h"


namespace jugimap{
	


class TextLibrary;
class TextBook;
class TextSequence;
class TextSprite;
class CompositeSprite;
class StandardSprite;
class ColorsLibrary;
class FontsLibrary;



class UnicodeString
{
public:

    //std::string s;

    UnicodeString(){}
    UnicodeString(const std::string &_s);

    void Set(const std::string &_s);
    void Append(const std::string &_s);
    void Append(const UnicodeString &_us);
    void Append(int _code);
    void Clear();
    bool IsEmpty(){ return codes.empty(); }
    int GetLength(){ return codes.size(); }

    std::string ToStdString();
    std::string ToStdString(int _startIndex, int _endIndex=-1);


    std::vector<int> & GetCodes(){ return codes; }

private:
    std::vector<int>codes;

};


void VerifyAndFixUtf8string(std::string& str);


std::string GetSubstring(const std::string &text, int indexStart, int indexEnd, bool &endReached);


void GetStringUnicodes(const std::string &text, std::vector<int> &unicodes);

int GetCharacterUnicode(const std::string &character);

std::string GetUnicodeCharacter(int unicode);


int GetStringNumberOfCharacters(const std::string &text);



class TextSegmentWord
{
public:
    std::string word;
    int nCharacters = 0;

    float width = -1;                       // width in pixels
    float height = -1;

    //std::string replacementKeyword;
    std::string originalWord;
    ColorRGBA color{255,255,255};
    Font * font = nullptr;                  // LINK
    float fFontHeight = 1.0;

    std::string button;
    //ColorRGBA buttonColor{255,255,255};
    ColorRGBA buttonColor_CO{200,0,0};
    //int offset = 0;


    //int horSpacing = 0;
    //int verSpacing = 0;




    enum Content{
        TEXT,
        //HOR_OFFSET,
        OFFSET,
        VER_SPACING,
        INDENT,
        TAB_SPACING
    };

    Content content = TEXT;


    void UpdateSize(bool _forceUpdate = false);

};



///\ingroup Gui
///\brief The TextSegment class defines a segment of text which can be accessed and displayed separatelly from other texts.
///
/// Text segments are usually created by parsing texts from text files via special markup codes.
/// In addition to text data, a text segment also stores styling information and custom parameters.
/// Text segments are displayed on screen with GuiTextField widgets.
class TextSegment
{
public:
    friend class TextLibrary;
    friend class TextBook;
    friend class TextSequence;


    TextSegment(int _id) : mId(_id)
    {
        currentColor = defaultColor;                    // index 0
        currentFont = defaultFont;                          // index 0
        currentIndent = defaultIndent;
        currentTabSpacing = defaultTabSpacing;

    }

    ~TextSegment();


    /// \brief Returns the identification number of this text segment.
    int GetId(){ return mId; }


    ///\brief Returns a reference to the vector of stored custom parameter.
    CParameters& parameters() { return mParameters; }


    ///\brief Replace the given word *_searchedWord* with the *newWord*.
    ///
    /// **Important!** The *_searchedWord* is allways the original word stored in the text segment at creation.
    /// When you call replace more times for the same original word, use the original word as *_searchedWord* every time!
    ///
    /// If the searched word was found and replaced the function returns true, otherwise it returns false.
    bool Replace(const std::string &_searchedWord, const std::string &newWord);


    ///\brief Returns true if this text segment contain the given *_word*; otherwise it returns false.
    ///
    /// The flag *compareWithOriginalWord* decide if comparing is done with original words or current words. A current word can be
    /// different then original if TextSegment::Replace has been used.
    bool Contains(const std::string &_word, bool compareWithOriginalWord = false);


    void AddParagraph(const std::string &_text, ColorsLibrary &_colors, FontsLibrary &_fonts);


    std::vector<TextSegmentWord*> &GetWords(){ return mWords; }
    void SetWordSizesIfNeeded();

    std::string plainText();


private:
    int mId = 0;
    std::vector<TextSegmentWord*> mWords;
    CParameters mParameters;



static ColorRGBA defaultColor;
static Font *defaultFont;
static ColorRGBA currentColor;
static Font *currentFont;
static std::string currentButton;
static ColorRGBA currentButtonColor;
static ColorRGBA currentButtonColor_CO;
//static int currentOffset;
static int defaultIndent;
static int defaultTabSpacing;
static int currentIndent;
static int currentTabSpacing;


    void ProcessAddingWord(TextSegmentWord *word, ColorsLibrary &colors, FontsLibrary &fonts);

};


///\ingroup Gui
///\brief The TextSequence class defines a storage for text segments.
///
/// Text sequences are used for grouping related text segments. Text sequence are usually defined in text files
/// and created together with text segments. A text sequence can also have stored custom parameters.
///
/// A TextSequence object takes ownership of TextSequence objects added to it.
class TextSequence
{
public:

    TextSequence(){}
    TextSequence(int _id) : id(_id){}
    ~TextSequence();


    /// \brief Returns the identification number of this text segment.
    int GetId(){ return id; }


    ///\brief Returns a reference to the vector of stored text segments.
    std::vector<TextSegment*>& GetTextSegments(){ return TextSegments; }


    ///\brief Returns a reference to the vector of stored custom parameters.
    CParameters& GetParameters() { return parameters; }


private:
    int id = 0;
    std::vector<TextSegment*>TextSegments;          // OWNED pointers
    CParameters parameters;

};


//=======================================================================================


///\ingroup Gui
///\brief The TextBook class defines a storage for text segments and text sequences.
///
/// A TextBook object takes ownership of TextSegment and TextSequence objects added to it.
class TextBook
{
public:
    friend class SourceObjectsLoader_xml;


    /// \brief Constructor.
    ///
    /// Create a text book with the given *_name*.
    TextBook(const std::string &_name) : mName(_name){}

    TextBook(const TextBook &_src) = delete;
    TextBook(const TextBook &&_src) = delete;
    TextBook& operator=(const TextBook &_src) = delete;


    /// \brief Destructor.
    ~TextBook();


    /// \brief Returns the name of this text book.
    std::string name(){ return mName; }


    std::string relativeFilePath(){ return mRelativeFilePath; }


    bool loadExternalData();

    void deleteExternalData();


    bool isExternalDataLoaded(){ return mExternalDataLoaded; }


    /// \brief Load text segments and text sequences from a text file.
    ///
    /// The text file must be written using *jugimap text markup codes* which allow parsing of the text.
    /// \param _filename The file path of the text file.
    /// \param _colors A reference to a *colors library* which is used during text parsing for obtaining colors.
    /// \param _fonts A reference to a *fonts library* which is used during text parsing for obtaining fonts.
    bool loadContent(ColorsLibrary &_colors, FontsLibrary &_fonts);


    /// \brief Add the given *_textSegment* to this text book.
    ///
    /// This function is called during the loading and should usually not be called manually.
    bool addTextSegment(TextSegment *_textSegment);


    /// \brief Add the given *_textSequence* to this text book.
    ///
    /// This function is called during the loading and should usually not be called manually.
    bool addTextSequence(TextSequence *_textSequence);


    ///\brief Returns a reference to the vector of stored text segments.
    std::vector<TextSegment*>& textSegments(){ return mTextSegments; }


    ///\brief Returns a reference to the vector of stored text sequences.
    std::vector<TextSequence*>& textSequences(){ return mTextSequences; }





private:
    std::string mRelativeFilePath;
    std::string mName;
    std::vector<TextSegment*>mTextSegments;              // OWNED pointers
    std::vector<TextSequence*>mTextSequences;            // OWNED pointers
    bool mExternalDataLoaded = false;


    void _parseParametersIfDefined(std::string &text, CParameters&_parameters);

    //---
    TextBook(){}

#ifdef JUGI_EDITOR
    friend class jugi::gui::TTextsLibraryDialog;

public:
    bool mReloaded = false;

#endif

};



//========================================================================================================================


///\ingroup Gui
///\brief The TextLibrary class defines a storage for text books.
///
/// A TextLibrary object takes ownership of TextBook objects added to it.
class TextLibrary
{
public:
    //static std::string path;
    //static std::string pathPrefix;



    ~TextLibrary();


    /// \brief Create a new text book with the given *_bookName* and returns a pointer link.
    ///
    /// If a text book with the given name allready exists in this library no book will be created
    /// and the function will return a pointer to the existing text book.
    TextBook* CreateNewBook(const std::string  &_bookName);


    /// \brief Returns a pointer link to the text book with the given *_bookName*.
    ///
    /// If the book is not found the function returns nullptr.
    TextBook *FindBook(const std::string &_bookName);


    /// \brief Returns a pointer link to the text segment within a book.
    ///
    /// \param _bookName The name of the text book.
    /// \param _segmentID The id number of the text segment.
    ///
    /// If the text book is not found or the text segment id is outside the storage vector bounds, the function returns nullptr.
    TextSegment *GetTextSegmentFromBook(const std::string &_bookName, int _segmentID);

    std::vector<TextBook*>& books(){ return mBooks; }


    bool loadExternalData();


    //---
    const std::string & relativeTextsPath(){ return mRelativeTextsPath; }
    void setRelativeTextsPath(const std::string &_path){ mRelativeTextsPath = _path; }

    const std::string & activeLanguage(){ return mActiveLanguage; }
    void setActiveLanguage(const std::string &_language){ mActiveLanguage = _language; }

    std::vector<std::string> &languages(){ return mLanguages; }


private:

    std::vector<TextBook*>mBooks;

    std::string mRelativeTextsPath;
    std::vector<std::string> mLanguages;
    std::string mActiveLanguage;
};


/// \brief The global text library for storing text books.
extern TextLibrary textLibrary;



	
}




#endif
