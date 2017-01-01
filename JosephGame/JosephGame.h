#ifndef JOSEPHGAME_CIRCLE_H
#define JOSEPHGAME_CIRCLE_H

#include <vector>

class JosephCircle {
public:
    JosephCircle(unsigned n_people, unsigned len_section, unsigned n_alive, unsigned start = 1)
        : livings(living_), died(died_)
    {
        std::vector<bool> status_table(n_people, true);
        unsigned looper;
        if (start + len_section >= n_people) {
            looper = (start + len_section) % n_people;
        } else {
            looper = start + len_section - 1;
        }
        status_table.at(looper) = false;
        auto alive = n_people - 1;
        if (alive > n_alive) {
            died_.push_back(looper == 0 ? n_people: looper);
        }
        unsigned counter;
        while (alive > n_alive) {
            counter = 0;
            for (auto i = looper + 1; counter < len_section ; ++i) {
                if (i >= n_people) {
                    i = 0;
                }
                if (status_table.at(i)) {
                    ++counter;
                }
                if (counter == len_section) {
                    status_table.at(i) = false;
                    died_.push_back(i == 0 ? n_people : i);
                    --alive;
                    looper = i;
                    break;
                }
            }
        }
        for (auto i = 0; i < status_table.size(); ++i) {
            if (status_table.at(i)) {
                living_.push_back(i == 0 ? n_people : i);
            }
        }
    }
    const std::vector<unsigned>& livings;
    const std::vector<unsigned>& died;
private:
    std::vector<unsigned> died_;
    std::vector<unsigned> living_;
};

#endif //JOSEPHGAME_CIRCLE_H
