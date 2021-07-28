#include "json.h"
#include "gtest/gtest.h"
#include "value/number.h"


#include <cstdint>
#include <iostream>
#include <sstream>
using namespace std;

class JsonGeneratorTest : public ::testing::Test{
protected:
	wjson::Json json1;
	void SetUp() override{
		//bool
		json1.to<wjson::Object>();
		json1["happy"] = true;
		json1["sad"] = false;
		json1["drunk"] = false;
		//object object
		json1["country"]["China"] = true;
		json1["country"]["USA"] = false;

		//defautl null
		json1["null"];

		//str
		json1["str_c"] = "string_c";
		json1["str"] = std::string("string");

		//number
		json1["int"] = 500;
		json1["double"] = 5.03;
		json1["long long"] = (long long)1 + INT32_MAX;
		json1["long long max"] = (long long)INT64_MAX;

		//array
		json1["array"].to<wjson::Array>().resize(3);
		json1["array"][2] = "idx2";
		json1["array"][0] = 1245364;

	}
};

TEST_F(JsonGeneratorTest, BoolType){
	EXPECT_EQ(json1["happy"].get<wjson::Bool>(), true);
	EXPECT_EQ(json1["sad"].get<wjson::Bool>(), false);
	EXPECT_EQ(json1["drunk"].get<wjson::Bool>(), false);
	EXPECT_EQ(json1["happy"].is<wjson::Bool>(), true);
	EXPECT_EQ(json1["sad"].is<wjson::Bool>(), true);
	EXPECT_EQ(json1["drunk"].is<wjson::Bool>(), true);
	EXPECT_EQ(json1["drunk"].is<wjson::Null>(), false);
}

TEST_F(JsonGeneratorTest, Object){
	EXPECT_EQ(json1["country"].is<wjson::Object>(), true);
	EXPECT_EQ(json1["country"].is<wjson::Null>(), false);
	EXPECT_EQ(json1["country"]["China"].get<wjson::Bool>(), true);
	EXPECT_EQ(json1["country"]["USA"].get<wjson::Bool>(), false);
	EXPECT_EQ(json1["country"].size(), 2);
}

TEST_F(JsonGeneratorTest, String){
	EXPECT_EQ(json1["str_c"].get<wjson::String>(), "string_c");
	EXPECT_EQ(json1["str"].get<wjson::String>(), "string");
}

TEST_F(JsonGeneratorTest, Number){
	EXPECT_EQ((double)json1["int"].get<wjson::Number>(), 500);
	EXPECT_EQ((double)json1["double"].get<wjson::Number>(), 5.03);
	EXPECT_EQ((double)json1["long long"].get<wjson::Number>(), (long long)(0x80000000));
}

TEST_F(JsonGeneratorTest, Array){
	EXPECT_EQ(json1["array"].size(), 3);
	EXPECT_EQ(json1["array"][1].is<wjson::Null>(), true);
	EXPECT_EQ(json1["array"][0].is<wjson::Null>(), false);
	EXPECT_EQ(json1["array"][0].get<wjson::Number>(), 1245364);
	EXPECT_EQ(json1["array"][0].get<wjson::Number>(), 1245364);
}

TEST_F(JsonGeneratorTest, Null){
	EXPECT_EQ(json1["null"].is<wjson::Null>(), true);
}
int main(int argc,char* argv[]){

	testing::InitGoogleTest(&argc,argv);
	return RUN_ALL_TESTS();


}
