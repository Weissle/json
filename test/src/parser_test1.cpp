#include "json.h"
#include "value/number.h"
#include "gtest/gtest.h"



class JsonParserTest : public ::testing::Test{
protected:
	wjson::Json json1;
	void SetUp() override{
		std::string case1 = "{\"true\":true,   \"false\":false,   \"null\":null , \"arr\":[false,true,null],\"str\":\"string\"}";
		json1.parse(case1);

	}
};


TEST_F(JsonParserTest, BoolType){
	EXPECT_EQ(json1["true"].get<wjson::Bool>(),true);
	EXPECT_EQ(json1["false"].get<wjson::Bool>(),false);
	EXPECT_NE(json1["false"].get<wjson::Bool>(),true);
	EXPECT_EQ(json1["false"].is<wjson::Null>(),false);
	EXPECT_EQ(json1["false"].is<wjson::Bool>(),true);
}
