#ifndef WORD_SEARCH_H
#define WORD_SEARCH_H

#include <string>

size_t naive_search(const std::string& text, const std::string& pattern);
size_t rabin_karp_search(const std::string& text, const std::string& pattern);

#endif // WORD_SEARCH_H