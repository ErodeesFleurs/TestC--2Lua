#pragma once

#include <future>
#include <thread>
#include <chrono>

#include "sol/sol.hpp"

namespace Promise {
    template <typename ResultType>
    class Promise {
    public:
        Promise() : m_promise(std::make_shared<std::future<ResultType>>()) {}

        Promise(std::future<ResultType>&& promise) : m_promise(std::make_shared<std::future<ResultType>>(std::move(promise))) {}

        bool finished() const;

        bool succeeded() const;

        sol::object result(sol::this_state L) const;
        
        std::string error();

    private:
        std::shared_ptr<std::future<ResultType>> m_promise;
    };
}
