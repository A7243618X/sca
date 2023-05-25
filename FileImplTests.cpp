//
// Created by A7243 on 22.05.2023.
//

#include <gtest/gtest.h>

#include "FileImpl.h"
#define path_test_file "../test_file.bin"
#define test_file_size 7 //at least 5

TEST(File, EmptyPath) {
    EXPECT_THROW({FileImpl("");}, InvalidFileName);
}

TEST(File, TooLongPath) {
    EXPECT_THROW({FileImpl("../non_existent_test.bin");}, FileException);
}

TEST(File, Open) {
    FileImpl(path_test_file);
}

/* The read operation shall read, from the referenced file, the number of octets specified by the
 * input length parameter and advance the value of the filePointer attribute by the number of octets actually read. */
TEST(File, Read) {
    std::vector<char> data;
    auto file = FileImpl(path_test_file);
    file.read(data, 2);
    EXPECT_EQ(file.filePointer(), 2);
    EXPECT_EQ(data.size(), 2);
}

/* The read operation shall read less than the the number of octets specified in the input length parameter,
 * when an end of file is encountered. */
TEST(File, ReadTooMuch) {
    std::vector<char> data;
    auto file = FileImpl(path_test_file);
    file.read(data, 1000);
    EXPECT_EQ(data.size(), test_file_size);
}

/* The read operation shall read, from the referenced file, the number of octets specified by the input length
 * parameter and advance the value of the filePointer attribute by the number of octets actually read. */
TEST(File, ReadPointerMove) {
    std::vector<char> data;
    auto file = FileImpl(path_test_file);
    file.read(data, 3);
    EXPECT_EQ(file.filePointer(), 3);
}

/* If the filePointer attribute value reflects the end of the File,
 * the read operation shall return a zero-length CF OctetSequence. */
TEST(File, ReadAtEnd) {
    std::vector<char> data;
    auto file = FileImpl(path_test_file);
    file.setFilePointer(file.sizeOf());
    file.read(data, 3);
    EXPECT_EQ(data.size(), 0);
}

/* The write operation shall write data to the file referenced. The write operation shall increment
 * the filePointer attribute to reflect the number of octets written, when the operation is successful. */
TEST(File, WritePointerMove) {
    const char* path = "../temp_test_file.bin";
    std::ofstream outfile(path);
    outfile.close();

    std::vector<char> data = {49, 50, 51};
    auto file = FileImpl(path, false);
    file.write(data);
    EXPECT_EQ(file.filePointer(), data.size());
    file.close();

    std::remove(path);
}

/* If the file was opened using the FileSystem::open operation with an input read_Only parameter value of TRUE,
 * writes to the file are considered to be in error. */
TEST(File, WriteToReadOnly) {
    std::vector<char> data = {1, 2, 3};
    auto file = FileImpl(path_test_file);

    EXPECT_THROW({file.write(data);}, File::IOException);
}

/* If the write is unsuccessful, the value of the filePointer attribute shall maintain or be restored to
 * its value prior to the write operation call. */
TEST(File, WriteFailPointerPosition) {
    std::vector<char> data = {1, 2, 3};
    auto file = FileImpl(path_test_file);
    try {
        file.write(data);
    } catch (File::IOException& e) {}
    EXPECT_EQ(file.filePointer(), 0);
}

/* The sizeOf operation shall return the number of octets stored in the file. */
TEST(File, SizeOf) {
    auto file = FileImpl(path_test_file);
    EXPECT_EQ(file.sizeOf(), 7);
}

/* The close operation shall release any OE file resources associated with the component.
 * The close operation shall make the file unavailable to the component. */
TEST(File, Close) {
    std::vector<char> data;
    auto file = FileImpl(path_test_file);
    file.close();
    EXPECT_THROW({file.read(data, 1);}, FileException);
}

/* The close operation shall raise the CF FileException when it cannot successfully close the file. */
TEST(File, CloseFail) {
    auto file = FileImpl(path_test_file);
    file.close();
    EXPECT_THROW({file.close();}, FileException);
}

/* The setFilePointer operation shall set the filePointer attribute value to the input filePointer. */
TEST(File, SetFilePointer) {
    auto file = FileImpl(path_test_file);
    file.setFilePointer(5);
    EXPECT_EQ(file.filePointer(), 5);
}

/* The setFilePointer operation shall raise the InvalidFilePointer exception,
 * when the value of the filePointer parameter exceeds the file size. */
TEST(File, SetFilePointerToFar) {
    auto file = FileImpl(path_test_file);
    EXPECT_THROW({file.setFilePointer(test_file_size+1);}, File::InvalidFilePointer);
}