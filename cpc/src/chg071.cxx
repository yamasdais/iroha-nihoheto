#include "challenge.h"
#include "observable_vector.h"

struct observer : public cpc::container_observer {
    virtual void container_changed(cpc::container_change_notifier notifier) override {
        std::cout << "action: " << to_string(notifier.action);
        if (!notifier.indices.empty()) {
            std::cout << ", indices: ";
            for (auto const i : notifier.indices) {
                std::cout << i << ' ';
            }
        }
        std::cout << std::endl;
    }
};

void test0() {
    cpc::observable_vector<int> v;
    auto o = std::make_shared<observer>();

    v.add_observer(o);

    v.push_back(1);
    v.push_back(2);
    v.pop_back();
    v.clear();

    v.remove_observer(o);

    v.push_back(3);
    v.push_back(4);

    v.add_observer(o);

    v = cpc::observable_vector<int> { 7, 8, 9};
}

int main(int, char**) {
    test0();
    return 0;
}