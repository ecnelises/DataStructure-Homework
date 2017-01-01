#include "Genealogy.h"
#include <iostream>
#include <string>
#include <cstdlib>

void Genealogy::add(const std::string& name, const std::string& child_name)
{
    auto p = get_person_by_name(ancestor, name);
    if (p == nullptr) {
        return;
    }
    p->children.push_back(new Person(child_name, p));
}

void Genealogy::rename(const std::string& old_name, const std::string& new_name)
{
    auto p = get_person_by_name(ancestor, old_name);
    if (p == nullptr) {
        return;
    }
    p->name = new_name;
}

void Genealogy::dismiss(const std::string& name)
{
    auto p = get_person_by_name(ancestor, name);
    if (p == nullptr) {
        return;
    }
    auto parent = p->parent;
    delete p;
    parent->children.erase(std::find(std::begin(parent->children),
        std::end(parent->children), p));
}

Genealogy::Person* Genealogy::get_person_by_name(
    Genealogy::Person* sub_family, const std::string& name)
{
    if (sub_family == nullptr) {
        return nullptr;
    }
    if (sub_family->name == name) {
        return sub_family;
    }
    Person* tmp = nullptr;
    for (auto i : sub_family->children) {
        tmp = get_person_by_name(i, name);
        if (tmp != nullptr) {
            return tmp;
        }
    }
    return nullptr;
}

void encomplete(Genealogy& g)
{
    std::cout << "请输入建立家庭的人的姓名：";
    std::string name;
    std::cin >> name;
    int children_num = 0;
    std::cout << "请输入儿女个数：";
    std::cin >> children_num;
    std::string tmp;
    for (auto i = 0; i < children_num; ++i) {
        std::cin >> tmp;
        g.add(name, tmp);
    }
    std::cout << name << "的第一代子孙有：";
    g.foreach_chilren(name, [&](const auto& n){
        std::cout << n << ' ';
    });
    std::cout << '\n';
}

void add_member(Genealogy& g)
{
    std::cout << "请输入要添加子女的人的姓名：";
    std::string name;
    std::cin >> name;
    std::cout << "请输入他的子女的姓名：";
    std::string child_name;
    std::cin >> child_name;
    g.add(name, child_name);
    std::cout << name << "的第一代子孙有：";
    g.foreach_chilren(name, [&](const auto& n){
        std::cout << n << ' ';
    });
    std::cout << '\n';
}

void dismiss_some(Genealogy& g)
{
    std::cout << "请输入要解散家庭的人的姓名：";
    std::string name;
    std::cin >> name;
    std::cout << name << "的第一代子孙有：";
    g.foreach_chilren(name, [&](const auto& n){
        std::cout << n << ' ';
    });
    std::cout << '\n';
    g.dismiss(name);
}

void change_name(Genealogy& g)
{
    std::cout << "请输入要改名的人的姓名：";
    std::string name;
    std::cin >> name;
    std::string new_name;
    std::cout << "请输入一个新名字：";
    std::cin >> new_name;
    g.rename(name, new_name);
    std::cout << name << "已改名为" << new_name << '\n';
}

void unknown(void)
{
    std::cout << "未知命令！\n";
}

void list_children(Genealogy& g)
{
    std::cout << "请输入名字：";
    std::string name;
    std::cin >> name;
    std::cout << name << "的第一代子孙有：";
    g.foreach_chilren(name, [&](const auto& n){
        std::cout << n << ' ';
    });
    std::cout << '\n';
}

int main(void)
{
    std::cout << "##\t\t家谱管理系统\t\t##\n"
        << "##\t\t\t\t\t##\n"
        << "##\t\tA -> 完善家谱\t\t##\n"
        << "##\t\tB -> 添加成员\t\t##\n"
        << "##\t\tC -> 解散局部家庭\t##\n"
        << "##\t\tD -> 成员改名\t\t##\n"
        << "##\t\tL -> 列出子女\t\t##\n"
        << "##\t\tE -> 退出\t\t##\n";
    std::cout << "首先建立一个家谱\n请输入祖先的名字：";
    std::string ancestor_name;
    std::cin >> ancestor_name;
    Genealogy main_gene(ancestor_name);
    char command = '\0';
    std::cout << "请输入要进行的操作：";
    while (std::cin >> command) {
        switch (command) {
        case 'A': encomplete(main_gene);    break;
        case 'B': add_member(main_gene);    break;
        case 'C': dismiss_some(main_gene);  break;
        case 'D': change_name(main_gene);   break;
        case 'E': std::exit(0);             break;
        case 'L': list_children(main_gene); break;
        default:  unknown();                break;
        }
        std::cout << "请输入要执行的操作：";
    }
}