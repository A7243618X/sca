//
// Created by A7243 on 22.05.2023.
//

#ifndef SCA_FILEIMPL_H
#define SCA_FILEIMPL_H

#include "File.h"

#include <iostream>
#include <fstream>

class FileImpl : public File {
public:
    FileImpl(const std::string& fileName, bool readOnly = true);
    ~FileImpl();

    void read(std::vector<char>& data, uint64_t length) override;
    void write(std::vector<char>& data) override;
    uint64_t sizeOf() override;
    void close() override;
    void setFilePointer(uint64_t filePointer) override;
    uint64_t filePointer() override;
    std::string fileName() override;;

protected:
    std::fstream fileStream;
    std::string pFileName;

    void checkStreamError();
    void checkFileError();
};

#endif //SCA_FILEIMPL_H
