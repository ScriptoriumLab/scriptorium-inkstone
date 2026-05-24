#include "gtest/gtest.h"

#include "modian/core/tokenizer/tokenizer.h"

TEST(tokenizer_test, should_successfully_get_all_possible_spelling_when_input_is_a_raw_string) {
    modian::inkstone::core::tokenizer tokenizer{};
    tokenizer.add_syllabary("fan");
    tokenizer.add_syllabary("gan");
    tokenizer.add_syllabary("fang");
    tokenizer.add_syllabary("an");

    auto tokens = tokenizer.split("fangan");

    ASSERT_EQ(tokens.size(), 2);
    ASSERT_EQ(tokens[0][0], "fan");
    ASSERT_EQ(tokens[0][1], "gan");

    ASSERT_EQ(tokens[1][0], "fang");
    ASSERT_EQ(tokens[1][1], "an");
}

TEST(tokenizer_test, should_successfully_get_only_one_possible_spelling_when_input_is_a_raw_string) {
    modian::inkstone::core::tokenizer tokenizer{};
    tokenizer.add_syllabary("ni");
    tokenizer.add_syllabary("hao");
    tokenizer.add_syllabary("shi");
    tokenizer.add_syllabary("jie");

    auto tokens = tokenizer.split("nihaoshijie");

    ASSERT_EQ(tokens.size(), 1);
    ASSERT_EQ(tokens[0][0], "ni");
    ASSERT_EQ(tokens[0][1], "hao");
    ASSERT_EQ(tokens[0][2], "shi");
    ASSERT_EQ(tokens[0][3], "jie");
}

TEST(tokenizer_test, should_successfully_get_candidates_when_the_last_character_input_is_a_substring_of_syllabary) {
    modian::inkstone::core::tokenizer tokenizer{};
    tokenizer.add_syllabary("ni");
    tokenizer.add_syllabary("hao");
    tokenizer.add_syllabary("shi");
    tokenizer.add_syllabary("jie");

    auto tokens = tokenizer.split("nihaoshij");

    ASSERT_EQ(tokens.size(), 1);
    ASSERT_EQ(tokens[0][0], "ni");
    ASSERT_EQ(tokens[0][1], "hao");
    ASSERT_EQ(tokens[0][2], "shi");
    ASSERT_EQ(tokens[0][3], "j");
}
