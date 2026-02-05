#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "modian/infra/utils/utils.h"

#include <filesystem>

TEST(utils_test, should_successfully_get_dictionary_path_when_call_get_dict_path) {
	const auto expected_path = std::filesystem::path("Modian") / "Local" / "pinyin_dictionary.txt";
	ASSERT_THAT(modian::inkstone::infra::utils::get_dict_path(), testing::EndsWith(expected_path.string()));
}

TEST(utils_test, should_convert_utf8_to_wstring_when_use_utils) {
	const auto expected_string = L"NI";
	ASSERT_EQ(modian::inkstone::infra::utils::utf8_to_wstring("NI"), expected_string);
}