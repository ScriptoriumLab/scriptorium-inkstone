#include "gtest/gtest.h"

#include "modian/core/tokenizer/tokenizer.h"

TEST(tokenizer_test, should_successfully_get_all_possible_spelling_when_input_is_a_raw_string) {
    modian::inkstone::core::tokenizer tokenizer{};
    auto tokens = tokenizer.split("fangan");

    ASSERT_EQ(tokens.size(), 2);
    ASSERT_EQ(tokens[0][0], "fan");
    ASSERT_EQ(tokens[0][1], "gan");

    ASSERT_EQ(tokens[1][0], "fang");
    ASSERT_EQ(tokens[1][1], "an");
}

TEST(tokenizer_test, should_successfully_get_only_one_possible_spelling_when_input_is_a_raw_string) {
    modian::inkstone::core::tokenizer tokenizer{};
    auto tokens = tokenizer.split("nihaoshijie");

    ASSERT_EQ(tokens.size(), 1);
    ASSERT_EQ(tokens[0][0], "ni");
    ASSERT_EQ(tokens[0][1], "hao");
    ASSERT_EQ(tokens[0][2], "shi");
    ASSERT_EQ(tokens[0][3], "jie");
}
