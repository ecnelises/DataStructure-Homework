#ifndef STUDENT_MANAGEMENT_H
#define STUDENT_MANAGEMENT_H

#include <string>
#include <iostream>

template<typename Key, typename Value>
class SkipList {
public:
    SkipList() = default;
    using size_t = unsigned long;
    static const size_t MaxLayer = 8;
    void remove(const Key& val)
    {
        auto layer_in = current_layer;
        if (head[0].first == nullptr) {
            return;
        }
        // 删除整个链表第一个的话还要考虑一件事情，就是我们需要保证每一层都有第一个元素，所以单独处理
        if (head[0].first->key == val) {
            // 如果删掉了惟一的一个
            if (head[0].first->next == nullptr) {
                for (auto i = layer_in; i > 0; --i) {
                    delete head[i - 1].first;
                    head[i - 1].first = nullptr;
                }
                current_layer = 0;
            } else {
                auto tmp = head[0].first->next;
                head[0].first->next = nullptr;
                delete head[0].first;
                head[0].first = tmp;
                ListNode* bottom = head[0].first;
                for (auto i = 2; i <= layer_in; ++i) {
                    auto tmp = head[i - 1].first->next;
                    head[i - 1].first->next = nullptr;
                    delete head[i - 1].first;
                    head[i - 1].first = tmp;
                    // 如果这一层删除以后，这层的第一个不是现在的链表第一个了
                    if (tmp == nullptr || tmp->key > head[0].first->key) {
                        auto tmp = new ListNode {bottom};
                        tmp->next = head[i - 1].first;
                        head[i - 1].first = tmp;
                        bottom = tmp;
                    }
                }
            }
            return;
        }
        // 逐层删除
        for (auto i = layer_in; i > 0; --i) {
            auto cr = head[i - 1].first;
            // 如果是这行第一个
            if (cr->key == val) {
                head[i - 1].first = cr->next;
                cr->next = nullptr;
                delete cr;
                // 这行已经空了
                if (head[i - 1].first == nullptr) {
                    --current_layer;
                }
                // 补齐第一个
            } else {
                while (cr->next != nullptr) {
                    // 只删一个
                    if (cr->next->key == val) {
                        auto tmp = cr->next;
                        cr->next = tmp->next;
                        tmp->next = nullptr;
                        delete tmp;
                        break;
                    }
                    cr = cr->next;
                }
            }
        }
    }
    void insert(const Key& k, const Value& val)
    {
        // 整个链表为空
        if (head[0].first == nullptr) {
            head[0].first = new ListNode {k, val};
            current_layer = 1;
            return;
        }
        ListNode* to_ins = nullptr;
        auto cr = head[0].first;
        for (; cr->next != nullptr; cr = cr->next) {
            // 在最底层插入完成
            if (cr->next->key == k) {
                return; // 这个键已经有了
            } else if (cr->next->key > k) {
                to_ins = new ListNode {k, val};
                to_ins->next = cr->next;
                cr->next = to_ins;
                break;
            }
        }
        // 要插入的那个值比列表里的都大
        if (cr->key < k) {
            to_ins = new ListNode {k, val};
            cr->next = to_ins;
        }
        // 随机向上扩展
        auto layer_in = 2;
        for (auto rp = random(); rp; rp = random()) {
            if (layer_in > MaxLayer) {
                break;
            }
            // 如果这层是空的，先得补上第一个
            if (layer_in > current_layer) {
                current_layer = layer_in;
                head[layer_in - 1].first = new ListNode {head[layer_in - 2].first};
                // 如果下一行插入的不是第一个，那就把这个数放到第一个后面去
                if (to_ins == head[layer_in - 2].first) {
                    to_ins = head[layer_in - 1].first->next = new ListNode {to_ins};
                }
            } else {
                for (auto i = head[layer_in - 1].first; i->next != nullptr; i = i->next) {
                    if (i->next->key > k) {
                        auto tmp = new ListNode {to_ins};
                        to_ins = tmp;
                        tmp->next = i->next->next;
                        i->next = tmp;
                    }
                }
            }
            ++layer_in;
        }
    }
    bool search(const Key& val)
    {
        return find_by_key(val) != nullptr;
    }
    template<typename F>
    void foreach(F fn)
    {
        for (auto cr = head[0].first; cr != nullptr; cr = cr->next) {
            fn(*(cr->value));
        }
    }
    template<typename F>
    void apply(const Key& k, F fn)
    {
        auto des = find_by_key(k);
        if (des == nullptr) {
            return;
        }
        fn(*(des->value));
    }
    template<typename F1, typename F2>
    void apply(const Key& k, F1 onsuccess, F2 onerror)
    {
        auto des = find_by_key(k);
        if (des == nullptr) {
            onerror();
        } else {
            onsuccess(*(des->value));
        }
    }
private:
    struct ListNode {
        Key key;
        Value* value;
        ListNode* next;
        ListNode* down;
        ListNode(ListNode* bottom) : key(bottom->key), down(bottom), value(bottom->value), next(nullptr) {}
        ListNode(const Key& k, const Value& val) : key(k), value(new Value {val}), next(nullptr), down(nullptr) {}
        ~ListNode()
        {
            // 真正的变量由底层所有
            if (down == nullptr) {
                delete value;
            }
        }
    };
    struct ListHead {
        ListNode* first;
        ListHead() : first(nullptr) {}
        ~ListHead()
        {
            for (auto i = first; i != nullptr; i = i->next) {
                delete i;
            }
        }
    };
    bool random()
    {
        static unsigned rk = 0;
        return (++rk) % 4 == 0;
    }
    ListNode* find_by_key(const Key& val)
    {
        auto layer_in = current_layer;
        // 在最上面的层寻找最接近但小于它的值
        auto cr = head[layer_in - 1].first;
        // 所以我们应该保证至少跳表的第一个数在每个层都存在
        if (cr != nullptr && cr->key == val) {
            return cr;
        }
        // 从上到下寻找
        while (layer_in > 0) {
            // 遍历当前这一层
            while (cr != nullptr && cr->next != nullptr) {
                // 在当前这一层就找到了
                if (cr->next->key == val) {
                    return cr->next;
                } else if (cr->next->key > val) {
                    break;
                }
                cr = cr->next;
            }
            // 现在cr是当前层最大的比val小的节点，进入下一层
            cr = cr->down;
            --layer_in;
        }
        // 在所有层都没有找到
        return nullptr;
    }
    size_t current_layer; // 从1开始
    ListHead head[MaxLayer];
};

struct Student {
    unsigned id;
    std::string name;
    enum Gender {
        MALE, FEMALE
    } gender;
    unsigned age;
    std::string type;
};

std::ostream& operator << (std::ostream& o, const Student& stu);
std::istream& operator >> (std::istream& i, Student& stu);

class StudentManager {
public:
    int run(void);
private:
    void welcome(void);
    void dispatch(int op_code);
    void insert(void);
    void destroy(void);
    void search(void);
    void change(void);
    void statistics(void);
    void list_all(void);
    void unknown(void);
    void deserialize(void);
    void serialize(void);
private:
    SkipList<unsigned, Student> students;
};

#endif // STUDENT_MANAGEMENT_H