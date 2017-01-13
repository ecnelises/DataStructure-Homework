#ifndef SORT_H
#define SORT_H

#include <algorithm>
#include <iterator>
#include <type_traits>

template<typename InputIt, typename Predicate>
void BubbleSort(InputIt start, InputIt end, Predicate less)
{
    for (auto i = start; i != end; ++i) {
        for (auto j = start; j != end; ++j) {
            if (less(*i, *j)) {
                auto tmp = *i;
                *i = *j;
                *j = tmp;
            }
        }
    }
}

template<typename InputIt>
void BubbleSort(InputIt start, InputIt end)
{
    BubbleSort(start, end, [](const auto& left, const auto& right){
        return left < right;
    });
}

template<typename InputIt, typename Predicate>
void SelectionSort(InputIt start, InputIt end, Predicate less)
{
    auto unsorted = start;
    while (unsorted != end) {
        auto smallest = unsorted;
        for (auto i = unsorted; i != end; ++i) {
            if (less(*i, *smallest)) {
                smallest = i;
            }
        }
        auto tmp = *smallest;
        *smallest = *unsorted;
        *unsorted = tmp;
        ++unsorted;
    }
}

template<typename InputIt>
void SelectionSort(InputIt start, InputIt end)
{
    SelectionSort(start, end, [](const auto& left, const auto& right){
        return left < right;
    });
}

template<typename InputIt, typename Predicate>
void QuickSort(InputIt start, InputIt end, Predicate less)
{
    auto len = end - start;
    if (len <= 1) {
        return;
    } else if (len == 2) {
        if (less(*(start + 1), *start)) {
            auto tmp = *(start + 1);
            *(start + 1) = *start;
            *start = tmp;
        }
        return;
    }
    // Naive QuickSort, simply choose first as pivot
    auto leftmost_less = end;
    auto pivot = *start;
    for (auto i = start + 1; i != end; ++i) {
        if (less(pivot, *i) && leftmost_less == end) {
            leftmost_less = i;
        } else if (less(*i, pivot) && leftmost_less != end) {
            auto tmp = *i;
            *i = *leftmost_less;
            *leftmost_less = tmp;
            if (less(pivot, *(leftmost_less + 1))) {
                ++leftmost_less;
            } else {
                leftmost_less = i;
            }
        }
    }
    if (leftmost_less != end) {
        auto tmp = *start;
        *start = *(leftmost_less - 1);
        *(leftmost_less - 1) = tmp;
        QuickSort(start, leftmost_less - 1, less);
        QuickSort(leftmost_less, end, less);
    } else {
        auto tmp = *start;
        *start = *(end - 1);
        *(end - 1) = tmp;
        QuickSort(start, end - 1, less);
    }
}

template<typename InputIt>
void QuickSort(InputIt start, InputIt end)
{
    QuickSort(start, end, [](const auto& left, const auto& right){
        return left < right;
    });
}

template<typename InputIt, typename Predicate>
void InsertSort(InputIt start, InputIt end, Predicate less)
{
    auto unsorted = start + 1;
    while (unsorted != end) {
        if (!less(*unsorted, *(unsorted - 1))) {
            ++unsorted;
            continue;
        }
        auto unsorted_val = *unsorted;
        auto i = unsorted - 1;
        for (; i != start; --i) {
            // Found propriate position
            if (less(unsorted_val, *i) && !less(unsorted_val, *(i - 1))) {
                *(i + 1) = *i;
                *i = unsorted_val;
                break;
            }
            *(i + 1) = *i;
        }
        if (i == start) {
            *(start + 1) = *start;
            *start = unsorted_val;
        }
        ++unsorted;
    }
}

template<typename InputIt>
void InsertSort(InputIt start, InputIt end)
{
    InsertSort(start, end, [](const auto& left, const auto& right){
        return left < right;
    });
}

template<typename InputIt, typename Predicate>
void MergeSort(InputIt start, InputIt end, Predicate less)
{
    auto len = end - start;
    if (len <= 1) {
        return;
    } else if (len == 2) {
        if (less(*(start + 1), *start)) {
            auto tmp = *(start + 1);
            *(start + 1) = *start;
            *start = tmp;
        }
        return;
    }
    auto tmp_start = new typename std::iterator_traits<InputIt>::value_type[len];
    auto tmp_end = tmp_start + len;
    std::copy(start, end, tmp_start);
    auto tmp_middle = tmp_start + len / 2;
    MergeSort(tmp_start, tmp_middle, less);
    MergeSort(tmp_middle, tmp_end, less);
    auto ires = start;
    auto imerge1 = tmp_start;
    auto imerge2 = tmp_middle;
    while (ires != end && imerge1 != tmp_middle && imerge2 != tmp_end) {
        if (less(*imerge1, *imerge2)) {
            *(ires++) = *(imerge1++);
        } else {
            *(ires++) = *(imerge2++);
        }
    }
    while (imerge1 != tmp_middle) {
        *(ires++) = *(imerge1++);
    }
    while (imerge2 != tmp_end) {
        *(ires++) = *(imerge2++);
    }
    delete[] tmp_start;
}

template<typename InputIt>
void MergeSort(InputIt start, InputIt end)
{
    MergeSort(start, end, [](const auto& left, const auto& right){
        return left < right;
    });
}

template<typename InputIt>
void RadixSort(InputIt start, InputIt end)
{
    static_assert(std::is_integral<typename std::iterator_traits<InputIt>::value_type>::value,
        "Value type of iterator used in radix sort must be integral!");
    auto len = end - start;
    using value_type = typename std::iterator_traits<InputIt>::value_type;
    auto bucket0 = new value_type[len];
    auto bucket1 = new value_type[len];
    auto b0ptr = bucket0;
    auto b1ptr = bucket1;
    auto rsptr = start;
    constexpr auto value_signed = std::is_signed<value_type>::value;
    constexpr auto n_bits = sizeof(value_type) * 8;
    constexpr auto loop_times = value_signed ? n_bits - 1 : n_bits;
    value_type bit_comp = 1;
    for (auto current_bit = 0; current_bit < loop_times; ++current_bit) {
        for (auto i = start; i != end; ++i) {
            if (bit_comp & (*i)) {
                *(b1ptr++) = *i;
            } else {
                *(b0ptr++) = *i;
            }
        }
        for (auto i = bucket0; i != b0ptr; ++i) {
            *(rsptr++) = *i;
        }
        for (auto i = bucket1; i != b1ptr; ++i) {
            *(rsptr++) = *i;
        }
        b0ptr = bucket0;
        b1ptr = bucket1;
        rsptr = start;
        bit_comp <<= 1;
    }
    if (value_signed) {
        for (auto i = start; i != end; ++i) {
            if (bit_comp & (*i)) {
                *(b1ptr++) = *i;
            } else {
                *(b0ptr++) = *i;
            }
        }
        for (auto i = bucket1; i != b1ptr; ++i) {
            *(rsptr++) = *i;
        }
        for (auto i = bucket0; i != b0ptr; ++i) {
            *(rsptr++) = *i;
        }
    }
    delete[] bucket0;
    delete[] bucket1;
}

template<typename InputIt, typename Predicate>
void HeapSort(InputIt start, InputIt end, Predicate less)
{
    auto sorted = end;
    while (sorted != start + 1) {
        auto dis = sorted - 1 - start;
        for (auto i = sorted - 1; i != start; --i, --dis) {
            if (less(*(start + dis / 2), *i)) {
                auto tmp = *i;
                *i = *(start + dis / 2);
                *(start + dis / 2) = tmp;
            }
        }
        --sorted;
        auto tmp = *start;
        *start = *sorted;
        *sorted = tmp;
    }
}

template<typename InputIt>
void HeapSort(InputIt start, InputIt end)
{
    HeapSort(start, end, [](const auto& left, const auto& right){
        return left < right;
    });
}

template<typename InputIt, typename Predicate>
void ShellSort(InputIt start, InputIt end, Predicate less)
{
    // Recommended steps are 1, 5, 19, 41, 109, 209, 505, 929, 2161, 3905 ...
    // Here we just use n/2 as initial step
    auto len = end - start;
    auto step = len / 2;
    while (step != 0) {
        for (auto i = start; i != start + step; ++i) {
            // Setting the bound
            auto unsorted = i + step;
            auto sub_end = i + (len / step) * step;
            if (i - start < len % step) {
                sub_end += step;
            }
            // Do insert sort for each part
            while (unsorted != sub_end) {
                if (!less(*unsorted, *(unsorted - step))) {
                    unsorted += step;
                    continue;
                }
                auto unsorted_val = *unsorted;
                auto j = unsorted - step;
                for (; j != i; j -= step) {
                    // Found propriate position
                    if (less(unsorted_val, *j) && !less(unsorted_val, *(j - step))) {
                        *(j + step) = *j;
                        *j = unsorted_val;
                        break;
                    }
                    *(j + step) = *j;
                }
                if (j == i) {
                    *(i + step) = *i;
                    *i = unsorted_val;
                }
                unsorted += step;
            }
        }
        step /= 2;
    }
}

template<typename InputIt>
void ShellSort(InputIt start, InputIt end)
{
    ShellSort(start, end, [](const auto& left, const auto& right){
        return left < right;
    });
}

template<typename InputIt, typename Predicate>
void HalfInsertSort(InputIt start, InputIt end, Predicate less)
{
    auto unsorted = start + 1;
    while (unsorted != end) {
        if (!less(*unsorted, *(unsorted - 1))) {
            ++unsorted;
            continue;
        }
        auto unsorted_val = *unsorted;
        auto pstart = start;
        auto i = unsorted - 1;
        auto sub_len = i - pstart;
        while (i != pstart + 1 && i != pstart) {
            sub_len = i - pstart;
            if (less(unsorted_val, *(pstart + sub_len / 2))) {
                i = pstart + sub_len / 2;
            } else {
                pstart += sub_len / 2;
            }
        }
        for (auto j = unsorted - 1; j != pstart; --j) {
            *(j + 1) = *j;
        }
        if (less(unsorted_val, *start) && pstart == start) {
            *(start + 1) = *start;
            *start = unsorted_val;
        } else {
            *i = unsorted_val;
        }
        ++unsorted;
    }
}

template<typename InputIt>
void HalfInsertSort(InputIt start, InputIt end)
{
    HalfInsertSort(start, end, [](const auto& left, const auto& right){
        return left < right;
    });
}

template<typename InputIt, typename Predicate>
void TournamentSort(InputIt start, InputIt end, Predicate less)
{
    struct node {
        node() : isinf(false) {}
        typename std::iterator_traits<InputIt>::value_type val;
        bool isinf;
    };
    if (end - start <= 1) {
        return;
    }
    SelectionSort(start, end, less);
    // auto even = (end - start) % 2 == 0;
    // auto elen = (end - start) / 2 * 2;
    // auto tree = new node[elen];
    // auto tree_tmp = tree + elen;
    // for (auto i = start; i != end; ++i, ++tree_tmp) {
    //     *(tree_tmp - 1) = *i; 
    // }
    // for (auto p = elen - 1; p != 0; --p) {
    //     if (less(tree[2 * p - 1].val, tree[2 * p].val)) {
    //         tree[p - 1].val = tree[2 * p - 1].val;
    //     } else {
    //         tree[p - 1].val = tree[2 * p].val;
    //     }
    // }
    // auto unsorted = start;
    // *(start++) = tree[0].val;
}

template<typename InputIt>
void TournamentSort(InputIt start, InputIt end)
{
    TournamentSort(start, end, [](const auto& left, const auto& right){
        return left < right;
    });
}

#endif // SORT_H