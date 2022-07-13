#include <gtest/gtest.h>

#include "TestAutomatic.hpp"

class AutograderTests : public ::testing::Test {
protected:
	MyDB::TestAutomatic theTests;

	AutograderTests() : theTests(std::cout) { }
};

TEST_F(AutograderTests, CompileTest) {
	ASSERT_TRUE(theTests.doCompileTest());
}

TEST_F(AutograderTests, AppTest) {
	ASSERT_TRUE(theTests.doAppTest());
}

TEST_F(AutograderTests, ReaderTest) {
	ASSERT_TRUE(theTests.doReaderTest());
}

TEST_F(AutograderTests, DBCommandsTest) {
	ASSERT_TRUE(theTests.doDBCommandsTest());
}

TEST_F(AutograderTests, TablesTest) {
	ASSERT_TRUE(theTests.doTablesTest());
}

TEST_F(AutograderTests, InsertTest) {
	ASSERT_TRUE(theTests.doInsertTest());
}

TEST_F(AutograderTests, SelectTest) {
	ASSERT_TRUE(theTests.doSelectTest());
}

TEST_F(AutograderTests, UpdateTest) {
	ASSERT_TRUE(theTests.doUpdateTest());
}

TEST_F(AutograderTests, DeleteTest) {
	ASSERT_TRUE(theTests.doDeleteTest());
}

TEST_F(AutograderTests, DropTableTest) {
	ASSERT_TRUE(theTests.doDropTest());
}

TEST_F(AutograderTests, JoinTest) {
	ASSERT_TRUE(theTests.doJoinTest());
}

TEST_F(AutograderTests, IndexTest) {
	ASSERT_TRUE(theTests.doIndexTest());
}

TEST_F(AutograderTests, BlockCacheTest)
{
	ASSERT_TRUE(theTests.doBlockCacheTest());
}

TEST_F(AutograderTests, RowCacheTest)
{
	//ASSERT_TRUE(theTests.doRowCacheTest());
}

TEST_F(AutograderTests, ViewCacheTest)
{
	//ASSERT_TRUE(theTests.doViewCacheTest());
}
