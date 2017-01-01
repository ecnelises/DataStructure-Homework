#ifndef GENEALOGY_H
#define GENEALOGY_H

#include <string>
#include <vector>
#include <algorithm>

class Genealogy {
public:
    Genealogy(const std::string& ancestor_name) : ancestor(new Person(ancestor_name, nullptr)) {}
    ~Genealogy()
    {
        delete ancestor;
    }
    void add(const std::string& name, const std::string& child_name);
    void rename(const std::string& old_name, const std::string& new_name);
    void dismiss(const std::string& name);
    template<typename F>
    void foreach_chilren(const std::string& name, F fn)
    {
        auto p = get_person_by_name(ancestor, name);
        if (p == nullptr) {
            return;
        }
        for (auto i : p->children) {
            fn(i->name);
        }
    }
private:
    struct Person {
        Person(const std::string& n, Person* p) : name(n), parent(p) {}
        ~Person()
        {
            for (auto i : children) {
                delete i;
            }
        }
        std::string name;
        Person* parent;
        std::vector<Person*> children;
    };
    Person* get_person_by_name(Person* sub_family, const std::string& name);
    Person* ancestor;
};

#endif // GENEALOGY_H