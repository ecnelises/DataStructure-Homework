#include "StudentManagement.h"
#include <sstream>
#include <limits>
#include <fstream>

std::ostream& operator << (std::ostream& o, const Student& stu)
{
    o << stu.id << '\t' << stu.name << '\t'
        << (stu.gender == Student::MALE ? "男\t" : "女\t")
        << stu.age << '\t' << stu.type;
    return o;
}

// 还没有错误处理
std::istream& operator >> (std::istream& i, Student& stu)
{
    std::string line;
    std::getline(i, line);
    std::istringstream iss(line);
    std::string gender_p;
    iss >> stu.id;
    iss >> stu.name;
    iss >> gender_p;
    if (gender_p == "女") {
        stu.gender = Student::FEMALE;
    } else {
        stu.gender = Student::MALE;
    }
    iss >> stu.age;
    iss >> stu.type;
    return i;
}

void StudentManager::welcome(void)
{
    auto init_students = 0;
    Student tmp;
    std::cout << "首先请建立考生信息系统\n请输入考生人数：";
    std::cin >> init_students;
    std::cin.ignore(std::numeric_limits<int>::max(), '\n');
    std::cout << "请依次输入考生的考号、姓名、性别、年龄及报考类别：\n";
    for (auto i = 0; i < init_students; ++i) {
        std::cin >> tmp;
        students.insert(tmp.id, tmp);
    }
    std::cout << "\n考号\t姓名\t性别\t年龄\t报考类别\n";
    students.foreach([&](const auto& s){
        std::cout << s << '\n';
    });
}

int StudentManager::run(void)
{
    this->welcome();
    auto current_operation = -1;
    auto return_status = 1;
    for (;;) {
        std::cout << "+-------+\n|1为插入|\n|2为删除|\n|3为查找|\n|4为修改|\n|5为统计|\n|6为列表|\n|7读文件|\n|8写文件|\n|0以退出|\n+-------+\n";
        std::cout << "请选择你要进行的操作：\n";
        std::cin >> current_operation;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (std::cin.eof()) {
            break;
        } else if (std::cin.bad()) {
            std::cin.clear();
            continue;
        }
        if (current_operation == 0) {
            return_status = 0;
            break;
        } else if (current_operation > 0) {
            this->dispatch(current_operation);
        } else {
            continue;
        }
    }
    return return_status;
}

void StudentManager::dispatch(int op_code)
{
    switch (op_code) {
    case 1: this->insert();     break;
    case 2: this->destroy();    break;
    case 3: this->search();     break;
    case 4: this->change();     break;
    case 5: this->statistics(); break;
    case 6: this->list_all();   break;
    case 7: this->deserialize();break;
    case 8: this->serialize();  break;
    default: this->unknown();   break;
    }
}

void StudentManager::deserialize(void)
{
    std::cout << "请输入你想要读取的文件名：\n";
    std::string file;
    std::cin >> file;
    std::ifstream infile(file);
    Student tmp;
    while (infile.good()) {
        infile >> tmp;
        students.insert(tmp.id, tmp);
    }
}

void StudentManager::serialize(void)
{
    std::cout << "请输入你想要输出到的文件名：\n";
    std::string file;
    std::cin >> file;
    std::ofstream outfile(file);
    students.foreach([&](const auto& s){
        outfile << s << '\n';
    });
}

void StudentManager::insert(void)
{
    Student tmp;
    std::cout << "请选择你需要插入的考生考号、姓名、性别、年龄和报考类别：\n";
    std::cin >> tmp;
    students.insert(tmp.id, tmp);
    students.foreach([&](const auto& s){
        std::cout << s << '\n';
    });
}

void StudentManager::unknown(void)
{
    std::cout << "未知命令！\n";
}

void StudentManager::destroy(void)
{
    std::cout << "请输入你要删除的考生的考号：";
    unsigned dest_id = 0;
    std::cin >> dest_id;
    students.remove(dest_id);
    students.foreach([&](const auto& s){
        std::cout << s << '\n';
    });
}

void StudentManager::list_all(void)
{
    std::cout << "考号\t姓名\t性别\t年龄\t报考类别\n";
    students.foreach([&](const auto& s){
        std::cout << s << '\n';
    });
}

void StudentManager::search(void)
{
    std::cout << "请输入你要查找的考生的考号：";
    unsigned dest_id = 0;
    std::cin >> dest_id;
    students.apply(dest_id, [&](const auto& s){
        std::cout << s << '\n';
    }, [&](){
        std::cout << "查无此人！\n";
    });
}

void StudentManager::change(void)
{
    std::cout << "请输入你要修改的考生的考号：";
    unsigned dest_id = 0;
    std::cin >> dest_id;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    students.apply(dest_id, [&](auto& s){
        Student tmp;
        std::cin >> tmp;
        if (tmp.id == dest_id) { // 不改键
            s = tmp;
        } else { // 键被修改
            students.remove(dest_id);
            students.insert(tmp.id, tmp);
            std::cout << tmp << '\n';
        }
    }, [&](){
        std::cout << "查无此人！\n";
    });
}

void StudentManager::statistics(void)
{
    unsigned st_res = 0;
    unsigned male = 0;
    students.foreach([&](const auto& s){
        st_res += 1;
        if (s.gender == Student::MALE) {
            male += 1;
        }
    });
    std::cout << "共有" << st_res << "名学生；"
            << male << "名男生，" << st_res - male  << "名女生。\n";
}

int main(void)
{
    StudentManager m;
    return m.run();
}