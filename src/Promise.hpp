#pragma once

#include <future>
#include <thread>
#include <chrono>
#include <string>

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
        
        std::string error() const;

    private:
        std::shared_ptr<std::future<ResultType>> m_promise;
    };

    template <typename ResultType>
    bool Promise<ResultType>::finished() const {
        return m_promise->wait_for(std::chrono::seconds(0)) == std::future_status::ready;
    }

    template <typename ResultType>
    bool Promise<ResultType>::succeeded() const {
        if (finished()) {
            try {
                m_promise->get();
                return true;
            } catch (...) {
                return false;
            }
            return false;
        }
        return false;
    }

    template <typename ResultType>
    sol::object Promise<ResultType>::result(sol::this_state L) const {
        if (finished()) {
            try {
                return sol::make_object(L, m_promise->get());
            } catch (...) {
                return sol::make_object(L, sol::nil);
            }
        }
        return sol::make_object(L, sol::nil);
    }

    template <typename ResultType>
    std::string Promise<ResultType>::error() const {
        if (finished()) {
            try {
                m_promise->get();
                return "";
            } catch (std::exception& e) {
                return e.what();
            }
        }
        return "";
    }
}
