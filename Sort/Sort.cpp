#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include "Sort.h"

template<typename TimeT = std::chrono::milliseconds>
struct MeasureTime
{
    template<typename F>
    static typename TimeT::rep execution(F func)
    {
        auto start = std::chrono::system_clock::now();
        func();
        auto duration = std::chrono::duration_cast< TimeT>
            (std::chrono::system_clock::now() - start);
        return duration.count();
    }
};

int main(void)
{
    std::cout << "##\t\t排序算法比较\t\t##\n##\t\t\t\t\t##\n";
    unsigned n_numbers = 0;
    std::cout << "请输入要产生的随机数的个数：";
    std::cin >> n_numbers;
    std::vector<int> vec(n_numbers);
    std::random_device rd;
    std::uniform_int_distribution<int> uid;
    auto dur = MeasureTime<>::execution([&](){
        for (auto i = 0U; i < n_numbers; ++i) {
            vec.at(i) = uid(rd);
        }
    });
    std::cout << "经过" << dur <<"ms，" << n_numbers << "个随机数生成完毕\n";

    auto pvec = vec;
    dur = MeasureTime<>::execution([&](){
        InsertSort(std::begin(pvec), std::end(pvec));
    });
    std::cout << "插入排序耗费了" << dur <<"ms\n";

    pvec = vec;
    dur = MeasureTime<>::execution([&](){
        BubbleSort(std::begin(pvec), std::end(pvec));
    });
    std::cout << "冒泡排序耗费了" << dur <<"ms\n";

    pvec = vec;
    dur = MeasureTime<>::execution([&](){
        SelectionSort(std::begin(pvec), std::end(pvec));
    });
    std::cout << "选择排序耗费了" << dur <<"ms\n";

    pvec = vec;
    dur = MeasureTime<>::execution([&](){
        QuickSort(std::begin(pvec), std::end(pvec));
    });
    std::cout << "快速排序耗费了" << dur <<"ms\n";

    pvec = vec;
    dur = MeasureTime<>::execution([&](){
        HeapSort(std::begin(pvec), std::end(pvec));
    });
    std::cout << "堆排序耗费了" << dur <<"ms\n";

    pvec = vec;
    dur = MeasureTime<>::execution([&](){
        ShellSort(std::begin(pvec), std::end(pvec));
    });
    std::cout << "希尔排序耗费了" << dur <<"ms\n";

    pvec = vec;
    dur = MeasureTime<>::execution([&](){
        HalfInsertSort(std::begin(pvec), std::end(pvec));
    });
    std::cout << "折半插入排序耗费了" << dur <<"ms\n";

    pvec = vec;
    dur = MeasureTime<>::execution([&](){
        RadixSort(std::begin(pvec), std::end(pvec));
    });
    std::cout << "基数排序耗费了" << dur <<"ms\n";

    pvec = vec;
    dur = MeasureTime<>::execution([&](){
        MergeSort(std::begin(pvec), std::end(pvec));
    });
    std::cout << "归并排序耗费了" << dur <<"ms\n";

    pvec = vec;
    dur = MeasureTime<>::execution([&](){
        std::sort(std::begin(pvec), std::end(pvec));
    });
    std::cout << "标准库sort耗费了" << dur <<"ms\n";

    pvec = vec;
    dur = MeasureTime<>::execution([&](){
        std::stable_sort(std::begin(pvec), std::end(pvec));
    });
    std::cout << "标准库stable_sort耗费了" << dur <<"ms\n";
}