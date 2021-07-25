#include <random>
#include <string_view>
#include <iomanip>
#include <ranges>

#include "challenge.h"
// イタチアルゴリズム
// ランダムな文字列を少しずつ目標文字列に近づけていくアルゴリズム
class weasel {
    std::string target;
    std::uniform_int_distribution<> chardist;
    std::uniform_real_distribution<> ratedist;
    std::mt19937 mt;
    constexpr inline static std::string_view const allowed_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";

  public:
    weasel() = delete;
    explicit weasel(std::string_view t)
        : target(t)
        , chardist(0, allowed_chars.size()-1)
        , ratedist(0, 100)
    {
        std::random_device rd;
        auto seed_data = std::array<int, std::mt19937::state_size>{};
        std::ranges::generate(seed_data, std::ref(rd));
        std::seed_seq seq(std::ranges::begin(seed_data), std::ranges::end(seed_data));
        mt.seed(seq);
    }

    void run(int const copies)
    {
        auto parent = make_random();
        int step = 0;
        std::cout << std::left << std::setw(5) << std::setfill(' ') << step
            << parent << std::endl;

        do {
            std::vector<std::string> children;
            std::ranges::generate_n(std::back_inserter(children), copies,
                    [parent, this]() { return mutate(parent, 5); });
            parent = *std::ranges::max_element(children,
                    [this](std::string_view c1, std::string_view c2) {
                    return fitness(c1) < fitness(c2);
                    });

            step++;
            std::cout << std::setw(5) << std::setfill(' ') << step
                << parent << std::endl;
        } while (parent != target);
    }


    int fitness(std::string_view candidate) {
        int score = 0;
        for (auto const i : std::views::iota(0u, candidate.size())) {
            if (candidate[i] == target[i])
                score++;
        }
        return score;
    }

    std::string mutate(std::string_view parent, double const ratio) {
        std::string ret(parent.size(), '\0');
        for (auto const i : std::views::iota(0u, parent.size())) {
            ret[i] = ratedist(mt) > ratio ? parent[i] : allowed_chars[chardist(mt)];
        }
        return ret;
    }

    std::string make_random()
    {
        std::string ret(target.size(), '\0');
        std::ranges::generate_n(ret.begin(), target.size(), [this]() {
                return allowed_chars[chardist(mt)]; 
                });
        return ret;
    }
};

void test0() {
    weasel w("METHINKS IT IS LIKE A WEASEL");
    w.run(100);
}

int main(int, char**) {
    test0();

    return 0;
}
