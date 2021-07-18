#include "json.hpp"
#include "gtest/gtest.h"



class JsonParserTest : public ::testing::Test{
protected:
	wjson::Json json1;
	void SetUp() override{
		std::string case1 = "{\"true\":true,   \"false\":false,   \"null\":null , \"arr\":[false,true,null],\"str\":\"string\"}";
		json1 = wjson::Parse(case1);

	}
};


TEST_F(JsonParserTest, BoolType){
	EXPECT_EQ(json1["true"].Get<wjson::Bool>(),true);
	EXPECT_EQ(json1["false"].Get<wjson::Bool>(),false);
	EXPECT_NE(json1["false"].Get<wjson::Bool>(),true);
	EXPECT_EQ(json1["false"].IsNull(),false);
	EXPECT_EQ(json1["false"].IsBool(),true);
}
