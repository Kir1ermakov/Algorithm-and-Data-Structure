#include <gtest/gtest.h>
#include "ascii85.h"
#include <vector>
#include <string>
#include <sstream>

class Ascii85Test : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(Ascii85Test, EncodeBlock_Empty) {
    std::vector<uint8_t> data;
    EXPECT_THROW(ascii85::encode_block(data, 0), std::invalid_argument);
}

TEST_F(Ascii85Test, EncodeBlock_SingleByte) {
    std::vector<uint8_t> data = {0x41}; 
    std::string result = ascii85::encode_block(data, 1);
    EXPECT_EQ(result, "5l");
}

TEST_F(Ascii85Test, EncodeBlock_TwoBytes) {
    std::vector<uint8_t> data = {0x41, 0x42}; 
    std::string result = ascii85::encode_block(data, 2);
    EXPECT_EQ(result, "5sb");
}

TEST_F(Ascii85Test, EncodeBlock_ThreeBytes) {
    std::vector<uint8_t> data = {0x41, 0x42, 0x43}; 
    std::string result = ascii85::encode_block(data, 3);
    EXPECT_EQ(result, "5sdp");
}

TEST_F(Ascii85Test, EncodeBlock_FourBytes) {
    std::vector<uint8_t> data = {0x41, 0x42, 0x43, 0x44}; 
    std::string result = ascii85::encode_block(data, 4);
    EXPECT_EQ(result, "5sdq,");
}

TEST_F(Ascii85Test, EncodeBlock_AllZeros) {
    std::vector<uint8_t> data = {0x00, 0x00, 0x00, 0x00};
    std::string result = ascii85::encode_block(data, 4);
    EXPECT_EQ(result, "z");
}
    
TEST_F(Ascii85Test, DecodeBlock_SingleChar) {
    std::string block = "5l";
    std::vector<uint8_t> result = ascii85::decode_block(block);
    EXPECT_EQ(result, std::vector<uint8_t>({0x41}));
}

TEST_F(Ascii85Test, DecodeBlock_TwoChars) {
    std::string block = "5sb";
    std::vector<uint8_t> result = ascii85::decode_block(block);
    EXPECT_EQ(result, std::vector<uint8_t>({0x41, 0x42}));
}

TEST_F(Ascii85Test, DecodeBlock_ThreeChars) {
    std::string block = "5sdp";
    std::vector<uint8_t> result = ascii85::decode_block(block);
    EXPECT_EQ(result, std::vector<uint8_t>({0x41, 0x42, 0x43}));
}

TEST_F(Ascii85Test, DecodeBlock_FourChars) {
    std::string block = "5sdq,";
    std::vector<uint8_t> result = ascii85::decode_block(block);
    EXPECT_EQ(result, std::vector<uint8_t>({0x41, 0x42, 0x43, 0x44}));
}

TEST_F(Ascii85Test, DecodeBlock_AllZeros) {
    std::string block = "z";
    std::vector<uint8_t> result = ascii85::decode_block(block);
    EXPECT_EQ(result, std::vector<uint8_t>({0x00, 0x00, 0x00, 0x00}));
}

TEST_F(Ascii85Test, DecodeBlock_InvalidChar) {
    std::string block = "5$v"; 
    EXPECT_THROW(ascii85::decode_block(block), std::invalid_argument);
}

TEST_F(Ascii85Test, DecodeBlock_Empty) {
    std::string block = "";
    EXPECT_THROW(ascii85::decode_block(block), std::invalid_argument);
}

TEST_F(Ascii85Test, DecodeBlock_TooLong) {
    std::string block = "5$7<D9"; 
    EXPECT_THROW(ascii85::decode_block(block), std::invalid_argument);
}

TEST_F(Ascii85Test, Encode_Empty) {
    std::vector<uint8_t> data;
    std::string result = ascii85::encode(data);
    EXPECT_EQ(result, "");
}

TEST_F(Ascii85Test, Encode_SingleByte) {
    std::vector<uint8_t> data = {0x41};
    std::string result = ascii85::encode(data);
    EXPECT_EQ(result, "5l");
}

TEST_F(Ascii85Test, Encode_MultipleBytes) {
    std::vector<uint8_t> data = {0x41, 0x42, 0x43, 0x44, 0x45};
    std::string result = ascii85::encode(data);
    EXPECT_EQ(result, "5sdq,70");
}

TEST_F(Ascii85Test, Encode_WithZeros) {
    std::vector<uint8_t> data = {0x00, 0x00, 0x00, 0x00, 0x41};
    std::string result = ascii85::encode(data);
    EXPECT_EQ(result, "z5l");
}

TEST_F(Ascii85Test, Decode_Empty) {
    std::string encoded = "";
    std::vector<uint8_t> result = ascii85::decode(encoded);
    EXPECT_EQ(result, std::vector<uint8_t>());
}

TEST_F(Ascii85Test, Decode_SingleBlock) {
    std::string encoded = "5sdq,";
    std::vector<uint8_t> result = ascii85::decode(encoded);
    EXPECT_EQ(result, std::vector<uint8_t>({0x41, 0x42, 0x43, 0x44}));
}

TEST_F(Ascii85Test, Decode_MultipleBlocks) {
    std::string encoded = "5sdq,70";
    std::vector<uint8_t> result = ascii85::decode(encoded);
    EXPECT_EQ(result, std::vector<uint8_t>({0x41, 0x42, 0x43, 0x44, 0x45}));
}

TEST_F(Ascii85Test, Decode_WithZeros) {
    std::string encoded = "z5l";
    std::vector<uint8_t> result = ascii85::decode(encoded);
    EXPECT_EQ(result, std::vector<uint8_t>({0x00, 0x00, 0x00, 0x00, 0x41}));
}

TEST_F(Ascii85Test, Decode_WithWhitespace) {
    std::string encoded = "5s dp\n"; 
    std::vector<uint8_t> result = ascii85::decode(encoded);
    EXPECT_EQ(result, std::vector<uint8_t>({0x41, 0x42, 0x43}));
}

TEST_F(Ascii85Test, Decode_InvalidData) {
    std::string encoded = "5~"; 
    EXPECT_THROW(ascii85::decode(encoded), std::invalid_argument);
}

TEST_F(Ascii85Test, EncodeDecodeRoundTrip) {
    std::vector<uint8_t> original = {0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64};
    std::string encoded = ascii85::encode(original);
    std::vector<uint8_t> decoded = ascii85::decode(encoded);
    EXPECT_EQ(decoded, original);
}

TEST_F(Ascii85Test, EncodeDecodeRoundTrip_WithZeros) {
    std::vector<uint8_t> original = {0x00, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x03};
    std::string encoded = ascii85::encode(original);
    std::vector<uint8_t> decoded = ascii85::decode(encoded);
    EXPECT_EQ(decoded, original);
}
