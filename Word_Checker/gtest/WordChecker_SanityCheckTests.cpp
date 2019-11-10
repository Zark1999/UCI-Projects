// WordChecker_SanityCheckTests.cpp
//
// ICS 46 Winter 2019
// Project #3: Set the Controls for the Heart of the Sun
//
// This is a set of "sanity checking" unit tests for your WordChecker
// implementation, provided primarily as a mechanism for ensuring that
// your implementation is compatible with the unit tests we'll be using
// to test it, and to give you a quick example of how each of the member
// functions works.  These tests don't verify all of the functionality
// you're implementing.
//
// As with the sanity-checking unit tests you saw in other projects, you
// should not modify these in any way -- don't add #include directives,
// and don't change the code within the tests.  These should all compile
// and pass before you submit your WordChecker implementation.

#include <string>
#include <vector>
#include <gtest/gtest.h>
#include "ListSet.hpp"
#include "WordChecker.hpp"


TEST(WordChecker_SanityCheckTests, canCreateWithSetOfStrings)
{
    WordChecker w{ListSet<std::string>{}};
}


TEST(WordChecker_SanityCheckTests, wordsExistWhenInSet)
{
    ListSet<std::string> set;
    set.add("HELLO");
    set.add("THERE");
    set.add("BOO");

    WordChecker checker{set};

    EXPECT_TRUE(checker.wordExists("HELLO"));
    EXPECT_TRUE(checker.wordExists("THERE"));
    EXPECT_TRUE(checker.wordExists("BOO"));
}


TEST(WordChecker_SanityCheckTests, wordsDoNotExistWhenNotInSet)
{
    ListSet<std::string> set;
    set.add("HELLO");
    set.add("THERE");
    set.add("BOO");

    WordChecker checker{set};

    EXPECT_FALSE(checker.wordExists("NOTBOO"));
    EXPECT_FALSE(checker.wordExists("HELLOTHERE"));
}


TEST(WordChecker_SanityCheckTests, canGenerateSuggestions)
{
    ListSet<std::string> set;
    set.add("ABDC");
    set.add("ZZZZZ");

    WordChecker checker{set};

    std::vector<std::string> suggestions = checker.findSuggestions("ABCD");

    ASSERT_EQ(1, suggestions.size());
    ASSERT_EQ("ABDC", suggestions[0]);
}

