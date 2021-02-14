#include <iostream>
#include <random>
#include <algorithm>
#include <array>
#include <thread>
#include <chrono>
#include <ranges>

class universe {
    universe() = delete;
    size_t rows_;
    size_t cols_;

    std::vector<unsigned char> grid;
    static inline constexpr const unsigned char ALIVE = 1;
    static inline constexpr const unsigned char DEAD = 0;

    std::uniform_int_distribution<> dist;
    std::mt19937 mt;

    public:
    enum class seed {
        random,
        ten_cell_row,
        small_explorer,
        explorer,
    };

    universe(size_t const width, size_t const height)
        : rows_{height}, cols_{width}, grid(width * height), dist(0, 4)
    {
        std::random_device rd;
        auto seed_data = std::array<int, std::mt19937::state_size>{};
        std::ranges::generate(seed_data, std::ref(rd));
        std::seed_seq seq(std::ranges::begin(seed_data), std::ranges::end(seed_data));
        mt.seed(seq);
    }

    void run(
            seed const s,
            int const generations,
            std::chrono::milliseconds const ms = std::chrono::milliseconds(100)
            ) {
        reset();
        initialize(s);
        display();

        for (auto i = 0; i < generations || generations == 0; ++i) {
            next_generation();
            display();
            std::this_thread::sleep_for(ms);
        }
    }

    void next_generation()
    {
        std::vector<unsigned char> newgrid(grid.size());

        for (auto r : std::views::iota(0u, rows_)) {
            for (auto c : std::views::iota(0u, cols_)) {
                auto count = count_neighbors(r, c);
                if (cell(c, r) == ALIVE)
                    newgrid[r * cols_ + c] = (count == 2 || count == 3) ? ALIVE : DEAD;
                else
                    newgrid[r * cols_ + c] = (count == 3) ? ALIVE : DEAD;
            }
        }
        grid.swap(newgrid);
    }

    void reset_display() {
#ifdef _WIN32
        system("cls");
#endif
#ifdef __linux__
        system("clear");
#endif
    }
    
    void display() {
        reset_display();

        for (size_t r = 0; r < rows_; ++r) {
            for (size_t c = 0; c < cols_; ++c) {
                std::cout << (cell(c, r) ? '*' : ' ');
            }
            std::cout << "\n";
        }
    }

    void initialize(seed const s) {
        if (s == seed::small_explorer) {
            auto mr = rows_ / 2;
            auto mc = cols_ / 2;

            cell(mc, mr) = ALIVE;
            cell(mc - 1, mr + 1) = ALIVE;
            cell(mc, mr + 1) = ALIVE;
            cell(mc + 1, mr + 1) = ALIVE;
            cell(mc - 1, mr + 2) = ALIVE;
            cell(mc + 1, mr + 2) = ALIVE;
            cell(mc, mr + 3) = ALIVE;
        } else if (s == seed::explorer) {
            auto mr = rows_ / 2;
            auto mc = cols_ / 2;

            cell(mc - 2, mr - 2) = ALIVE;
            cell(mc, mr - 2) = ALIVE;
            cell(mc + 2, mr - 2) = ALIVE;
            cell(mc - 2, mr - 1) = ALIVE;
            cell(mc + 2, mr - 1) = ALIVE;
            cell(mc - 2, mr) = ALIVE;
            cell(mc + 2, mr) = ALIVE;
            cell(mc - 2, mr + 1) = ALIVE;
            cell(mc + 2, mr + 1) = ALIVE;
            cell(mc - 2, mr + 2) = ALIVE;
            cell(mc, mr - 2) = ALIVE;
            cell(mc + 2, mr + 2) = ALIVE;
        } else if (s == seed::ten_cell_row) {
            for (size_t c{cols_/2-5}, end{cols_/2+5}; c < end; ++c) {
                cell(c, rows_ / 2) = ALIVE;
            }
        } else {
            for (size_t r = 0; r < rows_; ++r) {
                for (size_t c = 0; c < cols_; ++c) {
                    cell(c, r) = dist(mt) == 0 ? ALIVE : DEAD;
                }
            }
        }
    }

    void reset() {
        for (size_t r = 0; r < rows_; ++r) {
            for (size_t c = 0; c < cols_; ++c) {
                cell(c, r) = DEAD;
            }
        }
    }

#if 0
    int count_alive() { return 0; }
    template <class T0, class... Args>
        int count_alive(T0 s, Args... arg) {
            return s + count_alive(args...);
        }
#endif
    template <class... T>
    int count_alive(T... v) {
        return (v + ...);
    }

    int count_neighbors(size_t const row, size_t col) {
        if (row == 0 && col == 0)
            return count_alive(cell(1, 0), cell(1, 1), cell(0, 1));
        if (row == 0 && col == cols_ - 1)
            return count_alive(cell(cols_-1, 0), cell(col-1, 1), cell(col, 1));
        if (row == rows_ - 1 && col == 0)
            return count_alive(cell(0, row -1), cell(1, row-1), cell(1, row));
        if (row == rows_ - 1 && col == cols_ - 1)
            return count_alive(cell(col, row-1), cell(col-1, row-1), cell(col-1, row));
        if (row == 0 && col > 0 && col < cols_ - 1)
            return count_alive(cell(col-1, 0), cell(col-1, 1), cell(col, 1), cell(col+1, 1), cell(col+1, 0));
        if (row == rows_ - 1 && col > 0 && col < cols_ - 1)
            return count_alive(cell(col-1, row), cell(col-1, row-1), cell(col, row-1),
                   cell(col+1, row-1), cell(col+1, row));
        if (col == 0 && row > 0 && row < rows_ - 1)
            return count_alive(cell(0, row-1), cell(1, row-1), cell(1, row), cell(1, row+1), cell(0, row+1));
        if (col == cols_ - 1 && row > 0 && row < rows_ - 1)
            return count_alive(cell(col, row-1), cell(col-1, row-1), cell(col-1, row),
                    cell(col-1, row+1), cell(col, row+1));



        return count_alive(
                cell(col-1, row-1), cell(col,row-1), cell(col+1, row-1),
                cell(col-1, row), cell(col+1, row),
                cell(col-1, row+1), cell(col,row+1), cell(col+1, row+1));
    }

    unsigned char& cell(size_t const col, size_t const row) {
        return grid[row * cols_ + col];
    }
};

void test0() {
    using namespace std::chrono_literals;

    universe life_game(50, 20);
    life_game.run(universe::seed::random, 100, 100ms);
}

int main(int, char**) {
    test0();

    return 0;
}
