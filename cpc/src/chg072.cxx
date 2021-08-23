#include <vector>
#include <string>
#include "challenge.h"


struct discount_type {
    virtual double discount_percent(double const price, double const quantity) const noexcept = 0;
    virtual ~discount_type() = default;
};

struct fixed_discount final : public discount_type {
    explicit fixed_discount(double const discount) noexcept 
    : discount{discount} {
    }

    virtual double discount_percent(double const, double const) const noexcept override {
        return discount;
    }

private:
    double const discount;
};

struct volume_discount final : public discount_type {
    volume_discount(double const discount, double const min_quantity)
    : discount{discount}, min_quantity{min_quantity} {
    }
    virtual double discount_percent(double const, double const quantity) const noexcept override {
        return quantity >= min_quantity ? discount : 0.0;
    }

private:
    double const discount;
    double const min_quantity;
};

struct price_discount : public discount_type {
    price_discount(double const price, double const discount)
    : discount{discount}, min_total_price{price} {
    }

    virtual double discount_percent(double const price, double const quantity) const noexcept override {
        return price * quantity >= min_total_price ? discount : 0.0;
    }
private:
    double const discount;
    double const min_total_price;
};

struct amount_discount : public discount_type {
    amount_discount(double const price, double const discount)
    : discount{discount}, min_total_price{price} {
    }

    virtual double discount_percent(double const price, double const) const noexcept override {
        return price >= min_total_price ? discount : 0.0;
    }
private:
    double const discount;
    double const min_total_price;
};

struct customer {
    std::string name;
    discount_type* discount;
};

enum class article_unit {
    piece, kg, meter, sqmeter, cmeter, liter,
};

struct article {
    int id;
    std::string name;
    double price;
    article_unit unit;
    discount_type* discount;
};

struct order_line {
    article product;
    int quantity;
    discount_type* discount;
};

struct order {
    int id;
    customer* buyer;
    std::vector<order_line> lines;
    discount_type* discount;
};

// price calculator
struct price_calculator {
    virtual double calculate_price(order const& o) = 0;
    virtual ~price_calculator() = default;
};

struct cumulative_price_calculator : public price_calculator {
    virtual double calculate_price(order const& o) override {
        double price = 0.0;

        for (auto ol : o.lines) {
            double line_price = ol.product.price * ol.quantity;

            if (ol.product.discount != nullptr)
                line_price *= (1.0 - ol.product.discount->discount_percent(ol.product.price, ol.quantity));

            if (ol.discount != nullptr)
                line_price *= (1.0 - ol.discount->discount_percent(ol.product.price, ol.quantity));

            if (o.buyer != nullptr && o.buyer->discount != nullptr)
                line_price *= (1.0 - o.buyer->discount->discount_percent(ol.product.price, ol.quantity));

            price += line_price;
        }

        if (o.discount != nullptr)
            price *= (1.0 - o.discount->discount_percent(price, 0.0));

        return price;
    }
};

void test0() {
    fixed_discount d1{ 0.1 };
    volume_discount d2{ 10, 0.15 };
    price_discount d3{ 100, 0.05 };
    amount_discount d4{ 100, 0.05 };

    customer c1{ "default", nullptr };
    customer c2{ "john", &d1 };
    customer c3{ "joane", &d3 };

    article a1{ 1, "pen", 5, article_unit::piece, nullptr };
    article a2{ 2, "expensive pen", 15, article_unit::piece, &d1 };
    article a3{ 3, "scissors", 10, article_unit::piece, &d2 };

    cumulative_price_calculator calculator;

    order o1{ 101, &c1, {{a1, 1, nullptr}}, nullptr};
    std::cout << "o1: " << calculator.calculate_price(o1) << std::endl;

    order o2{ 103, &c1, {{a2, 1, nullptr}}, nullptr };
    std::cout << "o2: " << calculator.calculate_price(o2) << std::endl;
}

int main(int, char**) {
    test0();
    return 0;
}