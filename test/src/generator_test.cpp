#include "json.hpp"
#include "gtest/gtest.h"

#include <cstdint>
#include <iostream>
#include <sstream>
using namespace std;

class JsonGeneratorTest : public ::testing::Test{
protected:
	wjson::Json json1;
	void SetUp() override{
		//bool
		json1.To<wjson::Object>();
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
		json1["array"].To<wjson::Array>().Resize(3);
		json1["array"][2] = "idx2";
		json1["array"][0] = 1245364;

	}
};

TEST_F(JsonGeneratorTest, BoolType){
	EXPECT_EQ(json1["happy"].Get<wjson::Bool>(), true);
	EXPECT_EQ(json1["sad"].Get<wjson::Bool>(), false);
	EXPECT_EQ(json1["drunk"].Get<wjson::Bool>(), false);
	EXPECT_EQ(json1["happy"].IsBool(), true);
	EXPECT_EQ(json1["sad"].IsBool(), true);
	EXPECT_EQ(json1["drunk"].IsBool(), true);
	EXPECT_EQ(json1["drunk"].IsNull(), false);
}

TEST_F(JsonGeneratorTest, Object){
	EXPECT_EQ(json1["country"].IsObject(), true);
	EXPECT_EQ(json1["country"].IsNull(), false);
	EXPECT_EQ(json1["country"]["China"].Get<wjson::Bool>(), true);
	EXPECT_EQ(json1["country"]["USA"].Get<wjson::Bool>(), false);
	EXPECT_EQ(json1["country"].Size(), 2);
}

TEST_F(JsonGeneratorTest, String){
	EXPECT_EQ(json1["str_c"].Get<wjson::String>(), "string_c");
	EXPECT_EQ(json1["str"].Get<wjson::String>(), "string");
}

TEST_F(JsonGeneratorTest, Number){
	EXPECT_EQ(json1["int"].Get<wjson::Number>(), 500);
	EXPECT_EQ(json1["double"].Get<wjson::Number>(), 5.03);
	EXPECT_EQ(json1["long long"].Get<wjson::Number>(), (long long)(0x80000000));
}

TEST_F(JsonGeneratorTest, Array){
	EXPECT_EQ(json1["array"].Size(), 3);
	EXPECT_EQ(json1["array"][1].IsNull(), true);
	EXPECT_EQ(json1["array"][0].IsNull(), false);
	EXPECT_EQ(json1["array"][0].Get<wjson::Number>(), 1245364);
	EXPECT_EQ(json1["array"][0].Get<wjson::Number>(), 1245364);
}

TEST_F(JsonGeneratorTest, Null){
	EXPECT_EQ(json1["null"].IsNull(), true);
}
int main(int argc,char* argv[]){

	testing::InitGoogleTest(&argc,argv);
	return RUN_ALL_TESTS();


}
