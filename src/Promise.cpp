#include "Promise.hpp"

template <typename ResultType>
bool Promise::Promise<ResultType>::finished() const {
    return m_promise->wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}

template <typename ResultType>
bool Promise::Promise<ResultType>::succeeded() const {
    if (finished()) {
        try {
            m_promise->get();
            return true;
        } catch (...) {
            return false;
        }
        return false;
    }
}

template <typename ResultType>
sol::object Promise::Promise<ResultType>::result(sol::this_state L) const {
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
std::string Promise::Promise<ResultType>::error() {
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
