#pragma once

#include <mutex>
#include <iostream>

namespace challenge100 {

struct logger {
    static logger& instance() {
        static logger log__;
        return log__;
    }
    logger(logger const&) = delete;
    logger& operator=(logger const&) = delete;
    logger(logger&&) = delete;
    logger& operator=(logger&&) = delete;

    void log(std::string_view const& message) {
        std::lock_guard lock(mt);
        std::cout << "LOG: " << message << std::endl;
    }

  protected:
    logger() = default;

  private:
    std::mutex mt;
};

} // ns:challenge100
