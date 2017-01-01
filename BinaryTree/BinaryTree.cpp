#include "BinaryTree.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <limits>

void make_tree(BinaryTree<int>& tree)
{
    if (!tree.empty()) {
        std::cout << "树已建立！\n";
        return;
    }
    std::cout << "请输入初始元素：\n";
    std::string line;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, line);
    std::istringstream iss(line);
    int tmp = 0;
    while (iss >> tmp) {
        if (tree.search(tmp)) {
            std::cout << "键<" << tmp << ">已在树中！\n";
        } else {
            tree.insert(tmp);
        }
    }
}

void get_insert(BinaryTree<int>& tree)
{
    std::cout << "请输入要插入的键：";
    int target = 0;
    std::cin >> target;
    if (tree.search(target)) {
        std::cout << "键<" << target << ">已在树中！\n";
    } else {
        tree.insert(target);
    }
}

void get_search(BinaryTree<int>& tree)
{
    std::cout << "请输入要查找的键：";
    int target = 0;
    std::cin >> target;
    if (tree.search(target)) {
        std::cout << "查找成功！\n";
    } else {
        std::cout << "并不存在！\n";
    }
}

void get_remove(BinaryTree<int>& tree)
{
    std::cout << "请输入要删除的键：";
    int target = 0;
    std::cin >> target;
    if (tree.search(target)) {
        tree.remove(target);
        std::cout << "删除成功！\n";
    } else {
        std::cout << "并不存在！\n";
    }
}

void unknown()
{
    std::cout << "未知命令，请重新输入。\n";
}

int main(void)
{
    std::cout << "##\t\t二叉排序树\t\t##\n"
        << "##\t\t\t\t\t##\n"
        << "##\t\t1 -> 建立树\t\t##\n"
        << "##\t\t2 -> 插入元素\t\t##\n"
        << "##\t\t3 -> 查找元素\t\t##\n"
        << "##\t\t4 -> 删除元素\t\t##\n"
        << "##\t\t0 -> 退出\t\t##\n";
    BinaryTree<int> the_tree;
    int command = 0;
    std::string input_prompt = "请选择命令：";
    std::cout << input_prompt;
    while (std::cin >> command) {
        switch (command) {
        case 1: make_tree(the_tree);    break;
        case 2: get_insert(the_tree);   break;
        case 3: get_search(the_tree);   break;
        case 4: get_remove(the_tree);   break;
        case 0: std::exit(0);           break;
        default: unknown();             break;
        }
        std::cout << input_prompt;
    }
}