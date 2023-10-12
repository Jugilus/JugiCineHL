#include <assert.h>
#include <utility>
#include <memory>
#include "jmCommonFunctions.h"
#include "jmSourceSprite.h"
#include "jmMap.h"
#include "jmLayer.h"
#include "jmSprite.h"
#include "jmVectorShape.h"
#include "jmVectorShapeUtilities.h"
#include "jmFrameAnimation.h"
#include "jmStreams.h"


using namespace std;


namespace jugimap{




StdBinaryFileStreamReader::StdBinaryFileStreamReader(const std::string &fileName)
{
    fs.open(fileName, ios::binary);

    if(fs.is_open()){
        fs.seekg(0, ios::end);
        size = fs.tellg();
        fs.seekg(0, ios::beg);
    }
}



std::string StdBinaryFileStreamReader::ReadString()
{
    int length = ReadInt();
    char *buf = new char[length];
    fs.read(buf, length);
    std::string value(buf, length);
    delete[] buf;
    return value;
}


//-----------------------------------------------------------------------------------------------


StdTextFileStreamReader::StdTextFileStreamReader(const std::string &fileName)
{

    fs.open(fileName);

    if(fs.is_open()){
        fs.seekg(0, ios::end);
        size = fs.tellg();
        fs.seekg(0, ios::beg);
    }

}



//==================================================================================================


TextStreamWriter::TextStreamWriter(const std::string &fileName)
{

    ncIFile = ncine::IFile::createFileHandle(fileName.c_str());
    //ncIFile->setExitOnFailToOpen(false);
    ncIFile->open(ncine::IFile::OpenMode::WRITE);
}


void TextStreamWriter::WriteLine(const string &value)
{

    int length = value.size();
    char *buf = new char[length+1];
    value.copy(buf, length);
    buf[length] = '\n';
    ncIFile->write(buf, length+1);
    delete[] buf;

}




//--------------------------------------------------------------------------------------------------


BinaryStreamWriter::BinaryStreamWriter(const std::string &fileName)
{
    ncIFile = ncine::IFile::createFileHandle(fileName.c_str());
    //ncIFile->setExitOnFailToOpen(false);
    ncIFile->open(ncine::IFile::OpenMode::WRITE | ncine::IFile::OpenMode::BINARY);
}


void BinaryStreamWriter::writeString(const std::string &value)
{
    int length = value.size();
    writeInt(length);

    char *buf = new char[length];
    value.copy(buf, length);
    ncIFile->write(buf, length);
    delete[] buf;

}


//==================================================================================================


BinaryStreamReader* NewBinaryStreamReader(const string &fileName)
{

#if defined(__ANDROID__)
    return new AndroidBinaryFileStreamReaderNC(fileName);
#else
    return new StdBinaryFileStreamReader(fileName);
#endif
}



TextStreamReader* NewTextStreamReader(const std::string &fileName)
{
#if defined(__ANDROID__)
    return new StdTextFileStreamReader(fileName);
#else
    return new StdTextFileStreamReader(fileName);
#endif

}



}
