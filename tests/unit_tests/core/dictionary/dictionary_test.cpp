#include "gtest/gtest.h"

#include "scriptorium/core/dictionary/dictionary.h"

TEST(dictionary_test, should_return_top_candidates_by_using_weight_when_lookup_with_exact_spelling) {
    scriptorium::inkstone::core::dictionary_entry entry1{"ni", "你", 100};
    scriptorium::inkstone::core::dictionary_entry entry2{"ni", "泥", 50};
    scriptorium::inkstone::core::dictionary_entry entry3{"ni", "拟", 70};

    scriptorium::inkstone::core::dictionary dict;
    dict.build(entry1);
    dict.build(entry2);
    dict.build(entry3);

    auto candidates = dict.lookup("ni");
    ASSERT_EQ(candidates.size(), 3);
    ASSERT_EQ(candidates[0], "你");
    ASSERT_EQ(candidates[1], "拟");
    ASSERT_EQ(candidates[2], "泥");
}

TEST(dictionary_test, should_return_top_mixed_candidates_with_different_weight_when_lookup_with_fuzzy_spelling) {
    scriptorium::inkstone::core::dictionary_entry entry1{"ni", "你", 100};
    scriptorium::inkstone::core::dictionary_entry entry2{"ni", "泥", 50};
    scriptorium::inkstone::core::dictionary_entry entry3{"nian", "年", 70};

    scriptorium::inkstone::core::dictionary dict;
    dict.build(entry1);
    dict.build(entry2);
    dict.build(entry3);

    auto candidates = dict.lookup("n");
    ASSERT_EQ(candidates.size(), 3);
    ASSERT_EQ(candidates[0], "你");
    ASSERT_EQ(candidates[1], "年");
    ASSERT_EQ(candidates[2], "泥");
}

TEST(dictionary_test, should_truncate_to_max_candidates_and_keep_highest_weights_when_exceeding_capacity) {
    scriptorium::inkstone::core::dictionary dict;

    std::vector<std::string> characters{
        "你", "尼", "泥", "拟", "逆", "倪", "妮", "腻", "匿", "霓",
        "溺", "铌", "昵", "猊", "鲵", "伲", "怩", "坭", "旎", "睨",
        "呢", "妳", "祢", "禰", "儞", "苨", "狔", "婗", "淣", "跜",
        "馜", "隬", "屔", "屰", "秜", "郳", "棿", "貎", "蚭", "埿",
        "掜", "迡", "麑", "臡", "輗", "齯", "抳", "薿", "痆", "嫟",
        "惄", "檷", "腝", "儗", "嶷"
    };

    for (int i = 0; i < characters.size(); ++i) {
        scriptorium::inkstone::core::dictionary_entry entry{ "ni", characters[i], i + 1 };
        dict.build(entry);
    }

    auto candidates = dict.lookup("ni");

    ASSERT_EQ(candidates.size(), 50);
    ASSERT_EQ(candidates.at(0), "嶷");
    ASSERT_EQ(candidates.at(1), "儗");
    ASSERT_EQ(candidates.at(49), "倪");
}
