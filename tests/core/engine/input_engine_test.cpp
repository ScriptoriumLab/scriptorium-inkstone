#include "gtest/gtest.h"

#include <codecvt>

#include "modian/core/engine/pinyin_engine.h"

TEST(pinyin_engine_test, should_successfully_load_dictionary_and_show_candidates_when_input_correct_pinyin) {
	auto engine = modian::inkstone::core::pinyin_engine{};
	// engine.load_dictionary(std::string{PROJECT_SOURCE_DIR}.append("/data/pinyin_dictionary.txt"));

	const auto candidates = engine.convert(L"ni");

	ASSERT_EQ(candidates.size(), 3);

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	ASSERT_EQ(converter.to_bytes(candidates.at(0)), std::string{"你"});
	ASSERT_EQ(converter.to_bytes(candidates.at(1)), std::string{"尼"});
	ASSERT_EQ(converter.to_bytes(candidates.at(2)), std::string{"泥"});
}
