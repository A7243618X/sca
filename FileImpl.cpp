//
// Created by A7243 on 22.05.2023.
//

#include "FileImpl.h"

FileImpl::FileImpl(const std::string &fileName, bool readOnly) {
    //check path
    if(fileName.empty()) {
        throw InvalidFileName(EC_INVALIDFILENAME_EMPTY);
    } if (fileName.length() > 4096) {
        throw InvalidFileName(EC_INVALIDFILENAME_LONG);
    }

    pFileName = fileName;
    auto openFlags = std::ios::binary | std::ios::in;
    if(!readOnly) {
        openFlags |= std::ios::out;
    }
    fileStream.open(fileName.c_str(), openFlags);
    fileStream.seekg(0);
    checkFileError();
}

FileImpl::~FileImpl() {
    fileStream.close();
}

void FileImpl::read(std::vector<char> &data, uint64_t length) {
    fileStream.clear();

    //limit length to end of file
    const uint64_t size = sizeOf();
    const uint64_t pos = fileStream.tellg();
    length = std::min(length, size - pos);

    data.resize(length);
    fileStream.read(data.data(), length);
    checkStreamError();
}

void FileImpl::write(std::vector<char> &data) {
    fileStream.clear();
    int64_t pos = fileStream.tellg();
    fileStream.write(data.data(), data.size());
    if(!fileStream.good()) {
        fileStream.seekg(pos);
    }
    checkStreamError();
}

uint64_t FileImpl::sizeOf() {
    fileStream.clear();
    std::streampos end, current;
    current = fileStream.tellg();
    fileStream.seekg(0, std::ios::end);
    end = fileStream.tellg();
    fileStream.seekg(current, std::ios::beg);
    checkFileError();
    return end;
}

void FileImpl::close() {
    if (fileStream.is_open()) {
        fileStream.close();
    } else {
        throw FileException(EC_FILEEXCEPTION_CLOSED);
    }
}

void FileImpl::setFilePointer(uint64_t filePointer) {
    fileStream.clear();
    if(filePointer <= sizeOf()) {
        fileStream.seekg(filePointer);
        checkFileError();
    }else{
        throw InvalidFilePointer();
    }
}

uint64_t FileImpl::filePointer() {
    fileStream.clear();
    return fileStream.tellg();
}

std::string FileImpl::fileName() {
    return pFileName;
}

//protected
void FileImpl::checkStreamError() {
    if(!fileStream.good()) {
        if(fileStream.eof()) {
            throw IOException(EC_IOEXCEPTION_EOF);
        }
        if(fileStream.fail()) {
            throw IOException(EC_IOEXCEPTION_FAIL);
        }
        if(fileStream.bad()) {
            throw IOException(EC_IOEXCEPTION_BAD);
        }
    }
}

void FileImpl::checkFileError() {
    if(!fileStream.good()) {
        if(fileStream.eof()) {
            throw FileException(EC_FILEEXCEPTION_EOF);
        }
        if(fileStream.fail()) {
            throw FileException(EC_FILEEXCEPTION_FAIL);
        }
        if(fileStream.bad()) {
            throw FileException(EC_FILEEXCEPTION_BAD);
        }
    }
}
