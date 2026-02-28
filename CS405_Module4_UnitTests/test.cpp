// Uncomment the next line to use precompiled headers
#include "pch.h"
// uncomment the next line if you do not use precompiled headers
//#include "gtest/gtest.h"

#include <vector>      // needed for std::vector
#include <memory>      // needed for std::unique_ptr
#include <cassert>     // needed for assert
#include <cstdlib>     // needed for rand, srand
#include <ctime>       // needed for time
#include <stdexcept>   // needed for std::out_of_range

//
// the global test environment setup and tear down
// you should not need to change anything here
class Environment : public ::testing::Environment
{
public:
    ~Environment() override {}

    // Override this to define how to set up the environment.
    void SetUp() override
    {
        //  initialize random seed
        srand(time(nullptr));
    }

    // Override this to define how to tear down the environment.
    void TearDown() override {}
};

// create our test class to house shared data between tests
// you should not need to change anything here
class CollectionTest : public ::testing::Test
{
protected:
    // create a smart point to hold our collection
    std::unique_ptr<std::vector<int>> collection;

    void SetUp() override
    { // create a new collection to be used in the test
        collection.reset(new std::vector<int>);
    }

    void TearDown() override
    { //  erase all elements in the collection, if any remain
        collection->clear();
        // free the pointer
        collection.reset(nullptr);
    }

    // helper function to add random values from 0 to 99 count times to the collection
    void add_entries(int count)
    {
        assert(count > 0);
        for (auto i = 0; i < count; ++i)
            collection->push_back(rand() % 100);
    }
};

// When should you use the EXPECT_xxx or ASSERT_xxx macros?
// Use ASSERT when failure should terminate processing, such as the reason for the test case.
// Use EXPECT when failure should notify, but processing should continue

// Test that a collection is empty when created.
// Prior to calling this (and all other TEST_F defined methods),
//  CollectionTest::StartUp is called.
// Following this method (and all other TEST_F defined methods),
//  CollectionTest::TearDown is called
TEST_F(CollectionTest, CollectionSmartPointerIsNotNull)
{
    // is the collection created
    ASSERT_TRUE(collection);

    // if empty, the size must be 0
    ASSERT_NE(collection.get(), nullptr);
}

// Test that a collection is empty when created.
TEST_F(CollectionTest, IsEmptyOnCreate)
{
    // is the collection empty?
    ASSERT_TRUE(collection->empty());

    // if empty, the size must be 0
    ASSERT_EQ(collection->size(), 0);
}

/* Comment this test out to prevent the test from running
 * Uncomment this test to see a failure in the test explorer */
 // TEST_F(CollectionTest, AlwaysFail)
 // {
 //   FAIL();
 // }

 // TODO: Create a test to verify adding a single value to an empty collection
TEST_F(CollectionTest, CanAddToEmptyVector)
{
    // is the collection empty?
    ASSERT_TRUE(collection->empty());
    ASSERT_EQ(collection->size(), 0u);

    add_entries(1);

    // is the collection still empty?
    EXPECT_FALSE(collection->empty());

    // if not empty, what must the size be?
    EXPECT_EQ(collection->size(), 1u);
}

// TODO: Create a test to verify adding five values to collection
TEST_F(CollectionTest, CanAddFiveValuesToVector)
{
    ASSERT_TRUE(collection->empty());
    ASSERT_EQ(collection->size(), 0u);

    add_entries(5);

    EXPECT_FALSE(collection->empty());
    EXPECT_EQ(collection->size(), 5u);
}

// TODO: Create a test to verify that max size is greater than or equal to size for 0, 1, 5, 10 entries
TEST_F(CollectionTest, MaxSizeIsAlwaysGreaterThanOrEqualToSize)
{
    // 0 entries
    ASSERT_EQ(collection->size(), 0u);
    EXPECT_GE(collection->max_size(), collection->size());

    // 1 entry
    collection->clear();
    add_entries(1);
    EXPECT_GE(collection->max_size(), collection->size());

    // 5 entries
    collection->clear();
    add_entries(5);
    EXPECT_GE(collection->max_size(), collection->size());

    // 10 entries
    collection->clear();
    add_entries(10);
    EXPECT_GE(collection->max_size(), collection->size());
}

// TODO: Create a test to verify that capacity is greater than or equal to size for 0, 1, 5, 10 entries
TEST_F(CollectionTest, CapacityIsAlwaysGreaterThanOrEqualToSize)
{
    // 0 entries
    ASSERT_EQ(collection->size(), 0u);
    EXPECT_GE(collection->capacity(), collection->size());

    // 1 entry
    collection->clear();
    add_entries(1);
    EXPECT_GE(collection->capacity(), collection->size());

    // 5 entries
    collection->clear();
    add_entries(5);
    EXPECT_GE(collection->capacity(), collection->size());

    // 10 entries
    collection->clear();
    add_entries(10);
    EXPECT_GE(collection->capacity(), collection->size());
}

// TODO: Create a test to verify resizing increases the collection
TEST_F(CollectionTest, ResizeIncreasesCollectionSize)
{
    add_entries(5);
    const auto oldSize = collection->size();

    collection->resize(oldSize + 5);

    ASSERT_EQ(collection->size(), oldSize + 5);
    EXPECT_FALSE(collection->empty());
}

// TODO: Create a test to verify resizing decreases the collection
TEST_F(CollectionTest, ResizeDecreasesCollectionSize)
{
    add_entries(10);
    const auto oldSize = collection->size();
    ASSERT_EQ(oldSize, 10u);

    collection->resize(5);

    ASSERT_EQ(collection->size(), 5u);
    EXPECT_LT(collection->size(), oldSize);
}

// TODO: Create a test to verify resizing decreases the collection to zero
TEST_F(CollectionTest, ResizeDecreasesCollectionToZero)
{
    add_entries(5);
    ASSERT_EQ(collection->size(), 5u);

    collection->resize(0);

    EXPECT_EQ(collection->size(), 0u);
    EXPECT_TRUE(collection->empty());
}

// TODO: Create a test to verify clear erases the collection
TEST_F(CollectionTest, ClearErasesCollection)
{
    add_entries(5);
    ASSERT_FALSE(collection->empty());

    collection->clear();

    EXPECT_TRUE(collection->empty());
    EXPECT_EQ(collection->size(), 0u);
}

// TODO: Create a test to verify erase(begin,end) erases the collection
TEST_F(CollectionTest, EraseBeginEndErasesCollection)
{
    add_entries(10);
    ASSERT_EQ(collection->size(), 10u);

    collection->erase(collection->begin(), collection->end());

    EXPECT_TRUE(collection->empty());
    EXPECT_EQ(collection->size(), 0u);
}

// TODO: Create a test to verify reserve increases the capacity but not the size of the collection
TEST_F(CollectionTest, ReserveIncreasesCapacityNotSize)
{
    // keep size fixed
    add_entries(5);
    const auto oldSize = collection->size();
    const auto oldCap = collection->capacity();

    // reserve beyond current capacity (guaranteed non-decrease of capacity)
    collection->reserve(oldCap + 20);

    EXPECT_EQ(collection->size(), oldSize);                 // reserve must not change size
    EXPECT_GE(collection->capacity(), oldCap + 20);         // capacity should be at least requested
    EXPECT_GE(collection->capacity(), collection->size());  // still must be >= size
}

// TODO: Create a test to verify the std::out_of_range exception is thrown when calling at() with an index out of bounds
// NOTE: This is a negative test
TEST_F(CollectionTest, AtThrowsOutOfRangeWhenIndexIsInvalid)
{
    add_entries(5);
    ASSERT_EQ(collection->size(), 5u);

    EXPECT_THROW(collection->at(999), std::out_of_range);
}

// TODO: Create 2 unit tests of your own to test something on the collection - do 1 positive & 1 negative

// Custom Positive: verify add_entries generates values in the expected range [0, 99]
TEST_F(CollectionTest, AddedEntriesAreWithinExpectedRange)
{
    add_entries(20);
    ASSERT_EQ(collection->size(), 20u);

    for (const auto value : *collection)
    {
        EXPECT_GE(value, 0);
        EXPECT_LE(value, 99);
    }
}

// Custom Negative: calling at(0) on an empty collection should throw std::out_of_range
TEST_F(CollectionTest, AtThrowsOutOfRangeOnEmptyCollection)
{
    ASSERT_TRUE(collection->empty());
    EXPECT_THROW(collection->at(0), std::out_of_range);
}