#include <queue>
#include <iostream>
#include <random>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "challenge.h"
#include "logger.h"
#include "random.h"

class ticketing_machine {
    int first_ticket;
    int last_ticket;
public:
    explicit ticketing_machine(int const start)
        : first_ticket{start}
        , last_ticket{start}
    {}

    int next() { return last_ticket++; }
    int last() { return last_ticket - 1; }
    void reset() { last_ticket = first_ticket; }
};

class customer {
    int number;
    friend bool operator<(customer const& l, customer const& r) {
        return l.number > r.number;
    }
  public:
    explicit customer(int const num)
        : number{num}
    {}

    int ticket_number() const noexcept {
        return number;
    }
};

void test0() {
    std::priority_queue<customer> customers;
    bool isOfficeOpened = true;
    std::mutex mt;
    std::condition_variable cv;

    std::vector<std::thread> desks;
    for (int i = 0; i < 3; ++i) {
        desks.emplace_back([&, i] {
            cpc::random_generator<std::uniform_int_distribution<int>> randgen(2000, 3000);
            cpc::logger::instance().log("desk " + std::to_string(i) + " open");

            while (isOfficeOpened || !customers.empty()) {
                std::unique_lock locker(mt);

                cv.wait_for(locker, std::chrono::seconds(1),
                        [&customers] { return !customers.empty(); });

                if (!customers.empty()) {
                    auto const c = customers.top();
                    customers.pop();
                    cpc::logger::instance().log(
                            "[-] desk " + std::to_string(i) + " handling customer "
                            + std::to_string(c.ticket_number()));
                    cpc::logger::instance().log(
                            "[=] queue size: " + std::to_string(customers.size()));

                    locker.unlock();
                    cv.notify_one();

                    std::this_thread::sleep_for(std::chrono::milliseconds(randgen()));

                    cpc::logger::instance().log(
                            "[ ] desk " + std::to_string(i) + " done with customer "
                            + std::to_string(c.ticket_number()));
                }
            }
            cpc::logger::instance().log(
                    "[ ] desk " + std::to_string(i) + " closed");
        });
    }

    std::thread store([&isOfficeOpened, &customers, &cv] {
            ticketing_machine tm(100);
            cpc::random_generator<std::uniform_int_distribution<int>> randgen(200, 500);

            for (int i = 1; i < 25; ++i) {
                customer c(tm.next());
                customers.push(c);

                cpc::logger::instance().log("[+] new customer with ticket "
                        + std::to_string(c.ticket_number()));
                cpc::logger::instance().log(
                        "[=] queue size: " + std::to_string(customers.size()));

                cv.notify_one();
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
            }
            isOfficeOpened = false;
            });

    store.join();

    for (auto& desk : desks)
        desk.join();

}

int main(int, char**) {
    test0();

    return 0;
}
