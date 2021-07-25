#include <iostream>
#include <optional>
#include <concepts>

#include "challenge.h"

template <auto limit>
    requires std::is_invocable_r_v<double, decltype(limit)>
struct role {
    constexpr bool can_approve(double amount) {
        constexpr double lim_amt = std::invoke(limit);
        return amount <= lim_amt;
    }
};

struct expense {
    double const amount;
    std::string const description;
};

template <class Role>
constexpr std::function<bool(double)> make_approvable_predicate() {
    return [](double amount) noexcept {
        return Role{}.can_approve(amount);
    };
}

struct employee {
    template <class Role>
    employee(std::string_view name, Role, employee const* direct_manager = nullptr)
        : name{name}, can_approve{make_approvable_predicate<Role>()}, direct_manager{direct_manager}
    {}

    template <class Role>
    void set_role()
    { can_approve = make_approvable_predicate<Role>(); }

    void set_direct_manager(employee const* manager)
    { direct_manager = manager; }

    bool approve(expense const& exp) const {
        if (can_approve(exp.amount)) {
            std::cout << name << " approved expense '" << exp.description
                << "', cost=" << exp.amount << std::endl;
            return true;
        } else if (direct_manager != nullptr) {
            return direct_manager->approve(exp);
        }
        return true;
    }

  private:
    std::string const name;
    std::function<bool(double)> can_approve;
    employee const* direct_manager;
};

using employee_role = role<[]() noexcept { return 1000.0; }>;
using team_manager_role = role<[]() noexcept { return 10000.0; }>;
using department_manager_role = role<[]() noexcept { return 100000.0; }>;
using president_role = role<[]() noexcept { return std::numeric_limits<double>::max(); }>;

void test0() {
    auto cecil = employee("cecil williamson", president_role{});
    auto david = employee("david jones", department_manager_role{}, &cecil);
    auto robert = employee("robert booth", team_manager_role{}, &david);
    auto john = employee("john smith", employee_role{}, &robert);

    john.approve({ 500, "manazines"});
    john.approve({ 5000, "hotel accomodation"});
    john.approve({ 50000, "conference costs"});
    john.approve({ 500000, "new lorry"});
}

int main(int, char**) {
    test0();

    return 0;
}
