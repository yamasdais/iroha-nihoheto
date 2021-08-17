#include <vector>
#include <memory>
#include <ranges>

namespace challenge100 {
    enum class container_action {
        add,
        remove,
        clear,
        assign,
    };

    std::string to_string(container_action act) {
        switch (act)
        {
          case container_action::add:
            return "add";
          case container_action::remove:
            return "remove";
          case container_action::clear:
            return "clear";
          case container_action::assign:
            return "assign";
        }
        return {};
    }

    struct container_change_notifier {
        container_action action;
        std::vector<size_t> indices;
    };

    struct container_observer {
        virtual void container_changed(container_change_notifier notifier) = 0;
        virtual ~container_observer() = default;
    };

    template <class T, class Alloc = std::allocator<T>>
    class observable_vector final {
        using size_type = typename std::vector<T, Alloc>::size_type;
        std::vector<T, Alloc> data;
        std::vector<std::weak_ptr<container_observer>> observers;

        void notify(container_change_notifier notifier) {
            for (auto o : observers) {
                auto p = o.lock();
                if (p)
                    p->container_changed(notifier);
            }

        }

      public:
        observable_vector() noexcept(noexcept(Alloc()))
        : observable_vector(Alloc()) {
        }
        explicit observable_vector(Alloc const& alloc) noexcept
        : data(alloc) {
        }
        observable_vector(size_type count, T const& value, Alloc const& alloc = Alloc())
            noexcept(noexcept(data(count, value, alloc)))
        : data(count, value, alloc) {
        }
        explicit observable_vector(size_type count, Alloc const& alloc = Alloc())
            noexcept(noexcept(data(count, alloc)))
        : data(count, alloc) {
        }
        observable_vector(observable_vector&& other) noexcept
        : data(std::move(other.data)) {
        }
        observable_vector(observable_vector&& other, Alloc const& alloc)
        : data(std::move(other.data), alloc) {
        }
        observable_vector(std::initializer_list<T> init, Alloc const& alloc = Alloc())
        : data(init, alloc) {
        }
        template <class Itr>
        observable_vector(Itr first, Itr last, Alloc const& alloc = Alloc())
        : data(first, last, alloc) {
        }

        observable_vector& operator=(observable_vector const& other) {
            if (this != &other)  {
                data = other.data;
                notify({container_action::assign, std::vector<size_t>{}});
            }
            return *this;
        }
        observable_vector& operator=(observable_vector&& other) {
            if (this != &other)  {
                data = std::move(other.data);
                notify({container_action::assign, std::vector<size_t>{}});
            }
            return *this;
        }
        void push_back(T&& value) {
            data.push_back(std::move(value));
            notify({container_action::add, std::vector<size_t>{data.size()-1}});
        }
        void pop_back() {
            if (data.empty())
                return;

            data.pop_back();
            notify({container_action::remove, std::vector<size_t>{data.size()+1}});
        }
        void clear() {
            if (data.empty())
                return;
            data.clear();
            notify({container_action::clear, std::vector<size_t>{}});
        }
        size_type size() const noexcept {
            return data.size();
        }
        [[nodiscard]] bool empty() const noexcept {
            return data.empty();
        }

        void add_observer(std::weak_ptr<container_observer> o) {
            auto found = std::ranges::find_if(observers, [p=o.lock()](auto const& cur){
                return p == cur;
            }, &decltype(o)::lock);
            if (found == std::ranges::end(observers)) {
                observers.push_back(std::move(o));
            }
        }
        void remove_observer(std::weak_ptr<container_observer> o) {
            auto found = std::ranges::find_if(observers, [p=o.lock()](auto const& cur){
                return p == cur;
            }, &decltype(o)::lock);
            if (found != std::ranges::end(observers)) {
                observers.erase(found);
            }
        }
    };

}