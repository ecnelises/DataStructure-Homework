#include "WordSearch.h"
#include <fstream>
#include <iostream>
#include <string>
#include <limits>

size_t naive_match(const std::string& text, const std::string& pattern)
{
    if (text.size() < pattern.size()) {
        return 0UL;
    }
    auto count = 0UL;
    for (auto i = 0UL; i <= text.size() - pattern.size(); ++i) {
        ++count;
        for (auto j = 0UL; j < pattern.size(); ++j) {
            if (text[i + j] != pattern[j]) {
                --count;
                break;
            }
        }
    }
    return count;
}

size_t rabin_karp_search(const std::string& text, const std::string& pattern)
{
    if (text.size() < pattern.size()) {
        return 0UL;
    }
    auto sum = 0UL;
    auto b_sum = 0UL;
    auto count = 1UL;
    constexpr auto hashkey = 131UL;
    auto hashtop = 1UL;
    for (auto i = 0UL; i < pattern.size(); ++i) {
        if (pattern[i] != text[i]) {
            count = 0UL;
        }
        sum = sum * hashkey + pattern[i];
        b_sum = b_sum * hashkey + text[i];
        hashtop *= hashkey;
    }
    hashtop /= hashkey;
    for (auto i = 1UL; i <= text.size() - pattern.size(); ++i) {
        b_sum = (b_sum - text[i - 1] * hashtop) * hashkey + text[i + pattern.size() - 1];
        if (sum == b_sum) {
            ++count;
            for (auto j = 0; j < pattern.size(); ++j) {
                if (pattern[j] != text[i + j]) {
                    --count;
                    break;
                }
            }
        }
    }
    return count;
}

int main(void)
{
    std::string filename;
    std::cout << "请先输入文件名：";
    std::cin >> filename;
    std::ofstream output(filename);
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string content;
    std::string tmp;
    std::cout << "请输入一段英文，以空行结尾：\n";
    do {
        content += tmp;
        std::getline(std::cin, tmp);
    } while (tmp != "");
    output << content;
    std::cout << "本段文本已保存在文本文件'" << filename << "'中。\n";
    std::cout << "请输入要检索的关键字：";
    std::cin >> tmp;
    std::cout << "关键字'" << tmp << "'共出现" << rabin_karp_search(content, tmp) << "次\n";
}