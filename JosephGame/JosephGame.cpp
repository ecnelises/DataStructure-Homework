#include "JosephGame.h"
#include <iostream>

int main()
{
    int people, interval, alive;
    std::cout << "请输入船上的总人数：";
    std::cin >> people;
    std::cout << "请输入游戏的间隔人数：";
    std::cin >> interval;
    std::cout << "请输入要活下来的人数：";
    std::cin >> alive;
    JosephCircle circle(people, interval, alive);
    for (auto i = 0; i < circle.died.size(); ++i) {
        std::cout << "第" << i + 1 << "个死者的位置是" << circle.died.at(i) << '\n';
    }
    std::cout << "最后剩下" << circle.livings.size() << "人";
    return 0;
}