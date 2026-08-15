#include "gtest/gtest.h"

#include <codecvt>

#include "scriptorium/core/engine/pinyin_engine.h"

TEST(pinyin_engine_test, should_successfully_load_dictionary_and_show_candidates_when_input_correct_pinyin) {
	auto engine = scriptorium::inkstone::core::pinyin_engine{std::string{PROJECT_SOURCE_DIR}.append("/data/pinyin_dictionary.txt")};

	engine.update_input_state('n');
	engine.update_input_state('i');
	const auto candidates = engine.get_candidates();

	ASSERT_EQ(candidates.size(), 3);
	ASSERT_EQ(candidates.at(0).word, std::string{"你"});
	ASSERT_EQ(candidates.at(1).word, std::string{"尼"});
	ASSERT_EQ(candidates.at(2).word, std::string{"泥"});
}
