//
// Created by A7243 on 22.05.2023.
//

#ifndef SCA_FILE_H
#define SCA_FILE_H

#include <string>
#include <vector>
#include <map>
#include <stdexcept>

#define EC_INVALIDFILENAME_EMPTY 0x1
#define EC_INVALIDFILENAME_LONG 0x2

#define EC_FILEEXCEPTION_EOF 0x11
#define EC_FILEEXCEPTION_FAIL 0x12
#define EC_FILEEXCEPTION_BAD 0x13
#define EC_FILEEXCEPTION_CLOSED 0x14

#define EC_IOEXCEPTION_EOF 0x21
#define EC_IOEXCEPTION_FAIL 0x22
#define EC_IOEXCEPTION_BAD 0x23

struct ExceptionWithCodeMsg : public std::exception {
private:
    std::map<uint8_t, std::string> msg = {
            {0, "unknown"},

            {EC_INVALIDFILENAME_EMPTY,"filename empty"},
            {EC_INVALIDFILENAME_LONG,"filename too long"},

            {EC_FILEEXCEPTION_EOF, "end of file reached"},
            {EC_FILEEXCEPTION_FAIL, "fail in internal logic"},
            {EC_FILEEXCEPTION_BAD, "fail in io operation on the stream"},
            {EC_FILEEXCEPTION_CLOSED, "file closed"},

            {EC_IOEXCEPTION_EOF, "end of file reached"},
            {EC_IOEXCEPTION_FAIL, "fail in internal logic"},
            {EC_IOEXCEPTION_BAD, "fail in io operation on the stream"}
            };
    uint8_t errorNumber;

public:
    ExceptionWithCodeMsg(const uint8_t errorNumber) : errorNumber(errorNumber) {}

    const char* what() const noexcept override {
        try {
            return this->msg.at(errorNumber).c_str();
        } catch(const std::out_of_range& e) {
            return "unknown";
        }
    }

    uint8_t getErrorCode() const {
        return errorNumber;
    }
};

/// The InvalidFileName exception indicates an invalid file name was passed to a file service operation.
/// The message provides information describing why the filename was invalid.
struct InvalidFileName : ExceptionWithCodeMsg {
public:
    InvalidFileName(const uint8_t errorNumber) : ExceptionWithCodeMsg(errorNumber) {}
};


/// The FileException indicates a file-related error occurred. The message provides information describing the error.
struct FileException : ExceptionWithCodeMsg {
public:
    FileException(const uint8_t errorNumber) : ExceptionWithCodeMsg(errorNumber) {}
};



class File{
public:
    /// The IOException exception indicates an error occurred during a read or write operation to a File. The message is
    /// component-dependent, providing additional information describing the reason for the error.
    struct IOException : public ExceptionWithCodeMsg {
    public:
        IOException(const uint8_t errorNumber) : ExceptionWithCodeMsg(errorNumber) {}
    };

    /// This exception indicates the file pointer is out of range based upon the current file size.
    struct InvalidFilePointer : public std::exception {
        const char * what () const throw () {
            return "File pointer is out of range";
        }
    };

    /// Applications require the read operation in order to retrieve data from remote files.
    /// The read operation shall read less than the number of octets specified in the input length parameter,
    /// when an end of file is encountered.
    /// \param data output bytes
    /// \param length length of bytes to read
    /// raises File::IOException
    virtual void read(std::vector<char>& data, uint64_t length) = 0;

    /// The write operation writes data to the file referenced.
    /// \param data bytes to write
    /// raises File::IOException
    virtual void write(std::vector<char>& data) = 0;

    /// The sizeOf operation returns the current size of the file.
    /// raises FileException
    virtual uint64_t sizeOf() = 0;

    /// The close operation releases any OE file resources associated with the component.
    /// raises FileException
    virtual void close() = 0;

    /// The setFilePointer operation positions the file pointer where next read or write will occur.
    /// raises File::InvalidFilePointer FileException
    virtual void setFilePointer(uint64_t filePointer) = 0;

    /// filePointer returns file position where the next read or write will occur.
    virtual uint64_t filePointer() = 0;

    /// fileName returns the file name given to the FileSystem open/create operation.
    virtual std::string fileName() = 0;
};

#endif //SCA_FILE_H
