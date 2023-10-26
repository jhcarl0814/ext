#ifndef EXT_INFRASTRUCTURE__EXT_THREAD_H
#define EXT_INFRASTRUCTURE__EXT_THREAD_H

#include <variant>
#include <optional>
#include <exception>
#include <functional>
#include <future>
#include <mutex>
#include <condition_variable>
#include <coroutine>
#include <cassert>
#include <ext_infrastructure/ext_debug.hpp>
#include <ext_infrastructure/overloaded.hpp>
#include <ext_infrastructure/ext_utility.hpp>

template<typename T>
struct shared_state_t
{
    using value_type = std::conditional_t<std::is_same_v<T, void>, std::monostate, T>;

    //return type of `get`
    using reference_type = typename std::conditional_t<std::is_same_v<T, void>, std::type_identity<void>, std::add_lvalue_reference<T>>::type;

    //this type belongs to object type category, so can be stored in return type of other `get` functions
    using reference_wrapper_type = std::reference_wrapper<value_type>;

    using continuation_type = std::variant<
        std::function<void(std::variant<reference_wrapper_type, std::exception_ptr>)>,
        std::function<void(std::optional<reference_wrapper_type>)>,
        std::function<void(std::optional<std::exception_ptr>)>>;
    mutable std::variant<value_type, std::exception_ptr, std::vector<continuation_type>> state{std::in_place_index<2>};

    mutable std::mutex m;
    mutable std::condition_variable cv;
    mutable std::size_t promise_count = 0;
    mutable std::size_t future_count = 0;

    // fulfill
    void notify_values(std::vector<continuation_type> continuations) const
    {
        std::vector<std::exception_ptr> current_exceptions;
        for(continuation_type &continuation : continuations)
        {
            try
            {
                std::visit(overloaded{
                               [this](std::function<void(std::variant<reference_wrapper_type, std::exception_ptr>)> &continuation)
                               {
                                   std::move(continuation)(std::variant<reference_wrapper_type, std::exception_ptr>(std::in_place_index<0>, std::get<0>(state)));
                               },
                               [this](std::function<void(std::optional<reference_wrapper_type>)> &continuation)
                               {
                                   std::move(continuation)(std::optional<reference_wrapper_type>(std::in_place, std::get<0>(state)));
                               },
                               [this](std::function<void(std::optional<std::exception_ptr>)> &continuation)
                               {
                                   std::move(continuation)(std::nullopt);
                               },
                           },
                    continuation);
            }
            catch(...)
            {
                current_exceptions.push_back(std::current_exception());
            }
        }
        assert(current_exceptions.empty());
    }
    void notify_exceptions(std::vector<continuation_type> continuations) const
    {
        std::vector<std::exception_ptr> current_exceptions;
        for(continuation_type &continuation : continuations)
        {
            try
            {
                std::visit(overloaded{
                               [this](std::function<void(std::variant<reference_wrapper_type, std::exception_ptr>)> &continuation)
                               {
                                   std::move(continuation)(std::variant<reference_wrapper_type, std::exception_ptr>(std::in_place_index<1>, std::get<1>(state)));
                               },
                               [this](std::function<void(std::optional<reference_wrapper_type>)> &continuation)
                               {
                                   std::move(continuation)(std::nullopt);
                               },
                               [this](std::function<void(std::optional<std::exception_ptr>)> &continuation)
                               {
                                   std::move(continuation)(std::optional<std::exception_ptr>(std::in_place, std::get<1>(state)));
                               },
                           },
                    continuation);
            }
            catch(...)
            {
                current_exceptions.push_back(std::current_exception());
            }
        }
        assert(current_exceptions.empty());
    }
    template<typename U, typename... Args>
    void fulfill_value(std::initializer_list<U> lst, Args &&...args) const
    {
        std::unique_lock lock(m);
        assert(state.index() == 2);
        std::vector<continuation_type> continuations = std::move(std::get<2>(state));
        state.template emplace<0>(lst, std::forward<Args>(args)...);
        lock.unlock();
        cv.notify_all();
        notify_values(std::move(continuations));
    }
    template<typename... Args>
    void fulfill_value(Args &&...args) const
    {
        std::unique_lock lock(m);
        assert(state.index() == 2);
        std::vector<continuation_type> continuations = std::move(std::get<2>(state));
        state.template emplace<0>(std::forward<Args>(args)...);
        lock.unlock();
        cv.notify_all();
        notify_values(std::move(continuations));
    }
    void fulfill_exception(std::exception_ptr e) const
    {
        std::unique_lock lock(m);
        assert(state.index() == 2);
        std::vector<continuation_type> continuations = std::move(std::get<2>(state));
        state.template emplace<1>(e);
        lock.unlock();
        cv.notify_all();
        notify_exceptions(std::move(continuations));
    }
    void break_() const
    {
        std::unique_lock lock(m);
        assert(state.index() == 2);
        std::vector<continuation_type> continuations = std::move(std::get<2>(state));
        state.template emplace<1>(std::make_exception_ptr(std::future_error(std::future_errc::broken_promise)));
        lock.unlock();
        cv.notify_all();
        notify_exceptions(std::move(continuations));
    }

    // sync blocking
    void wait() const
    {
        std::unique_lock lock(m);
        cv.wait(lock, [this]()
            { return state.index() != 2; });
    }
    reference_type get() const
    {
        std::unique_lock lock(m);
        cv.wait(lock, [this]()
            { return state.index() != 2; });
        if(state.index() == 1)
            std::rethrow_exception(std::get<1>(state));
        else
            return static_cast<reference_type>(std::get<0>(state));
    }
    std::optional<reference_wrapper_type> get_value() const
    {
        std::unique_lock lock(m);
        cv.wait(lock, [this]()
            { return state.index() != 2; });
        if(state.index() == 1)
            return std::nullopt;
        else
            return std::optional<reference_wrapper_type>(std::in_place, std::get<0>(state));
    }
    std::optional<std::exception_ptr> get_exception() const
    {
        std::unique_lock lock(m);
        cv.wait(lock, [this]()
            { return state.index() != 2; });
        if(state.index() == 1)
            return std::optional<std::exception_ptr>(std::in_place, std::get<1>(state));
        else
            return std::nullopt;
    }
    // sync non-blocking
    std::size_t state_index() const
    {
        std::unique_lock lock(m);
        return state.index();
    }
    bool is_ready() const
    {
        std::unique_lock lock(m);
        return state.index() != 2;
    }
    bool is_value() const
    {
        std::unique_lock lock(m);
        return state.index() == 0;
    }
    bool is_exception() const
    {
        std::unique_lock lock(m);
        return state.index() == 1;
    }
    std::optional<reference_wrapper_type> get_if() const
    {
        std::unique_lock lock(m);
        if(state.index() == 2)
            return std::nullopt;
        else if(state.index() == 1)
            std::rethrow_exception(std::get<1>(state));
        else
            return std::optional<reference_wrapper_type>(std::in_place, std::get<0>(state));
    }
    std::optional<std::optional<reference_wrapper_type>> get_value_if() const
    {
        std::unique_lock lock(m);
        if(state.index() == 2)
            return std::nullopt;
        else if(state.index() == 1)
            return std::optional<std::optional<reference_wrapper_type>>(std::in_place, std::nullopt);
        else
            return std::optional<std::optional<reference_wrapper_type>>(std::in_place, std::in_place, std::get<0>(state));
    }
    std::optional<std::optional<std::exception_ptr>> get_exception_if() const
    {
        std::unique_lock lock(m);
        if(state.index() == 2)
            return std::nullopt;
        else if(state.index() == 1)
            return std::optional<std::optional<std::exception_ptr>>(std::in_place, std::in_place, std::get<1>(state));
        else
            return std::optional<std::optional<std::exception_ptr>>(std::in_place, std::nullopt);
    }

    // async
    void then(std::variant_alternative_t<0, continuation_type> fn) const
    {
        std::unique_lock lock(m);
        if(state.index() != 2)
        {
            if(state.index() == 1)
            {
                std::variant<reference_wrapper_type, std::exception_ptr> result{std::in_place_index<1>, std::get<1>(state)};
                lock.unlock();
                fn(std::move(result));
            }
            else
            {
                std::variant<reference_wrapper_type, std::exception_ptr> result{std::in_place_index<0>, std::get<0>(state)};
                lock.unlock();
                fn(std::move(result));
            }
        }
        else
            std::get<2>(state).emplace_back(std::in_place_index<0>, std::move(fn));
    }
    void then(std::variant_alternative_t<1, continuation_type> fn) const
    {
        std::unique_lock lock(m);
        if(state.index() != 2)
        {
            if(state.index() == 1)
            {
                lock.unlock();
                fn(std::nullopt);
            }
            else
            {
                std::optional<reference_wrapper_type> result{std::in_place, std::get<0>(state)};
                lock.unlock();
                fn(std::move(result));
            }
        }
        else
            std::get<2>(state).emplace_back(std::in_place_index<1>, std::move(fn));
    }
    void then(std::variant_alternative_t<2, continuation_type> fn) const
    {
        std::unique_lock lock(m);
        if(state.index() != 2)
        {
            if(state.index() == 1)
            {
                std::optional<std::exception_ptr> result{std::in_place, std::get<1>(state)};
                lock.unlock();
                fn(std::move(result));
            }
            else
            {
                lock.unlock();
                fn(std::nullopt);
            }
        }
        else
            std::get<2>(state).emplace_back(std::in_place_index<2>, std::move(fn));
    }
};

template<typename T>
struct promise_t;

namespace detail
{
    constexpr bool log_ctors_and_dtor = false;
}

template<typename T>
struct future_t
{
    mutable shared_state_t<T> *p_shared_state;
    promise_t<T> get_promise() const { return promise_t(p_shared_state); }

    future_t(std::nullptr_t)
        : p_shared_state(nullptr)
    {
        if constexpr(detail::log_ctors_and_dtor) ext_debug_log((this), qDebug_compact());
    }
    void reference_count_increment() const
    {
        if(p_shared_state != nullptr)
        {
            std::unique_lock lock(p_shared_state->m);
            ++p_shared_state->future_count;
        }
    }
    void reference_count_decrement() const
    {
        if(p_shared_state != nullptr)
        {
            //            std::unique_lock lock(p_shared_state->m);
            try
            {
                p_shared_state->m.lock();
            }
            catch(...)
            {
                std::unreachable();
            }
            std::unique_lock lock(p_shared_state->m, std::adopt_lock);
            --p_shared_state->future_count;
            if(p_shared_state->promise_count == 0 && p_shared_state->future_count == 0)
            {
                assert(p_shared_state->state.index() != 2);
                lock.unlock();
                delete std::exchange(p_shared_state, nullptr);
            }
        }
    }
    future_t()
        : p_shared_state(new shared_state_t<T>)
    {
        if constexpr(detail::log_ctors_and_dtor) ext_debug_log((this), qDebug_compact());
        reference_count_increment();
    }
    future_t(shared_state_t<T> *p_shared_state)
        : p_shared_state(p_shared_state)
    {
        if constexpr(detail::log_ctors_and_dtor) ext_debug_log((this), qDebug_compact());
        reference_count_increment();
    }
    future_t(future_t const &other)
        : p_shared_state(other.p_shared_state)
    {
        reference_count_increment();
    }
    future_t &operator=(future_t const &other)
    {
        if(this != &other)
        {
            reference_count_decrement();
            p_shared_state = other.p_shared_state;
            reference_count_increment();
        }
        return *this;
    }
    future_t(future_t &&other)
        : p_shared_state(std::exchange(other.p_shared_state, nullptr))
    {}
    future_t &operator=(future_t &&other)
    {
        if(this != &other)
        {
            reference_count_decrement();
            p_shared_state = std::exchange(other.p_shared_state, nullptr);
        }
        return *this;
    }
    ~future_t()
    {
        if constexpr(detail::log_ctors_and_dtor) ext_debug_log((this), qDebug_compact());
        reference_count_decrement();
    }

    // sync blocking
    decltype(auto) wait() const
    {
        assert(p_shared_state != nullptr);
        return p_shared_state->wait();
    }
    decltype(auto) get() const
    {
        assert(p_shared_state != nullptr);
        return p_shared_state->get();
    }
    decltype(auto) get_value() const
    {
        assert(p_shared_state != nullptr);
        return p_shared_state->get_value();
    }
    decltype(auto) get_exception() const
    {
        assert(p_shared_state != nullptr);
        return p_shared_state->get_exception();
    }
    // sync non-blocking
    decltype(auto) state_index() const
    {
        assert(p_shared_state != nullptr);
        return p_shared_state->state_index();
    }
    decltype(auto) is_ready() const
    {
        assert(p_shared_state != nullptr);
        return p_shared_state->is_ready();
    }
    decltype(auto) is_value() const
    {
        assert(p_shared_state != nullptr);
        return p_shared_state->is_value();
    }
    decltype(auto) is_exception() const
    {
        assert(p_shared_state != nullptr);
        return p_shared_state->is_exception();
    }
    decltype(auto) get_if() const
    {
        assert(p_shared_state != nullptr);
        return p_shared_state->get_if();
    }
    decltype(auto) get_value_if() const
    {
        assert(p_shared_state != nullptr);
        return p_shared_state->get_value_if();
    }
    decltype(auto) get_exception_if() const
    {
        assert(p_shared_state != nullptr);
        return p_shared_state->get_exception_if();
    }
    // async
    decltype(auto) then(std::variant_alternative_t<0, typename shared_state_t<T>::continuation_type> fn) const
    {
        assert(p_shared_state != nullptr);
        return p_shared_state->then(std::move(fn));
    }
    decltype(auto) then(std::variant_alternative_t<1, typename shared_state_t<T>::continuation_type> fn) const
    {
        assert(p_shared_state != nullptr);
        return p_shared_state->then(std::move(fn));
    }
    decltype(auto) then(std::variant_alternative_t<2, typename shared_state_t<T>::continuation_type> fn) const
    {
        assert(p_shared_state != nullptr);
        return p_shared_state->then(std::move(fn));
    }
    using promise_type = promise_t<T>;
    bool await_ready() const { return is_ready(); }
    void await_suspend(std::coroutine_handle<> fn) const
    {
        then(std::variant_alternative_t<0, typename shared_state_t<T>::continuation_type>([fn](auto &&...) -> decltype(auto)
            { return fn(); }));
    }
    typename shared_state_t<T>::reference_type await_resume() const { return get(); }
};

template<typename T>
struct promise_base_t
{
    mutable shared_state_t<T> *p_shared_state;
    future_t<T> get_future() const { return future_t(p_shared_state); }

    promise_base_t(std::nullptr_t)
        : p_shared_state(nullptr)
    {
        if constexpr(detail::log_ctors_and_dtor) ext_debug_log((this), qDebug_compact());
    }
    void reference_count_increment() const
    {
        if(p_shared_state != nullptr)
        {
            std::unique_lock lock(p_shared_state->m);
            ++p_shared_state->promise_count;
        }
    }
    void reference_count_decrement() const
    {
        if(p_shared_state != nullptr)
        {
            //            std::unique_lock lock(p_shared_state->m);
            try
            {
                p_shared_state->m.lock();
            }
            catch(...)
            {
                std::unreachable();
            }
            std::unique_lock lock(p_shared_state->m, std::adopt_lock);
            --p_shared_state->promise_count;
            if(p_shared_state->promise_count == 0)
            {
                if(p_shared_state->state.index() == 2)
                {
                    p_shared_state->break_();
                }
                if(p_shared_state->future_count == 0)
                {
                    lock.unlock();
                    delete std::exchange(p_shared_state, nullptr);
                }
            }
        }
    }
    promise_base_t()
        : p_shared_state(new shared_state_t<T>)
    {
        if constexpr(detail::log_ctors_and_dtor) ext_debug_log((this), qDebug_compact());
        reference_count_increment();
    }
    promise_base_t(shared_state_t<T> *p_shared_state)
        : p_shared_state(p_shared_state)
    {
        if constexpr(detail::log_ctors_and_dtor) ext_debug_log((this), qDebug_compact());
        reference_count_increment();
    }
    promise_base_t(promise_base_t const &other)
        : p_shared_state(other.p_shared_state)
    {
        reference_count_increment();
    }
    promise_base_t &operator=(promise_base_t const &other)
    {
        if(this != &other)
        {
            reference_count_decrement();
            p_shared_state = other.p_shared_state;
            reference_count_increment();
        }
        return *this;
    }
    promise_base_t(promise_base_t &&other)
        : p_shared_state(std::exchange(other.p_shared_state, nullptr))
    {}
    promise_base_t &operator=(promise_base_t &&other)
    {
        if(this != &other)
        {
            reference_count_decrement();
            p_shared_state = std::exchange(other.p_shared_state, nullptr);
        }
        return *this;
    }
    ~promise_base_t()
    {
        if constexpr(detail::log_ctors_and_dtor) ext_debug_log((this), qDebug_compact());
        reference_count_decrement();
    }

    // fulfill
    template<typename U, typename... Args>
    decltype(auto) fulfill_value(std::initializer_list<U> lst, Args &&...args) const
    {
        assert(p_shared_state != nullptr);
        return p_shared_state->fulfill_value(lst, std::forward<Args>(args)...);
    }
    template<typename... Args>
    decltype(auto) fulfill_value(Args &&...args) const
    {
        assert(p_shared_state != nullptr);
        return p_shared_state->fulfill_value(std::forward<Args>(args)...);
    }
    decltype(auto) fulfill_exception(std::exception_ptr e) const
    {
        assert(p_shared_state != nullptr);
        return p_shared_state->fulfill_exception(e);
    }
    // async
    future_t<T> get_return_object() const noexcept { return get_future(); }
    std::suspend_never initial_suspend() const noexcept { return {}; }
    void unhandled_exception() const noexcept { fulfill_exception(std::current_exception()); }
    std::suspend_never final_suspend() const noexcept { return {}; }
};

template<typename T>
struct promise_t: public promise_base_t<T>
{
    using promise_base_t<T>::promise_base_t;
    using promise_base_t<T>::operator=;
    template<typename U, typename... Args>
    void return_value(std::initializer_list<U> lst, Args &&...args) const { this->fulfill_value(lst, std::forward<Args>(args)...); }
    template<typename... Args>
    void return_value(Args &&...args) const { this->fulfill_value(std::forward<Args>(args)...); }
};
template<>
struct promise_t<void>: public promise_base_t<void>
{
    using promise_base_t<void>::promise_base_t;
    using promise_base_t<void>::operator=;
    void return_void() const { this->fulfill_value(); }
};

//template struct future_t<void>;
//template struct future_t<int>;
//template struct future_t<std::unique_ptr<int>>;
//template struct future_t<std::reference_wrapper<int>>;

//template struct promise_t<void>;
//template struct promise_t<int>;
//template struct promise_t<std::unique_ptr<int>>;
//template struct promise_t<std::reference_wrapper<int>>;

template<typename Promise = void>
struct coroutine_get_promise_t
{
    mutable std::coroutine_handle<Promise> ch;
    bool await_ready() const { return false; }
    bool await_suspend(std::coroutine_handle<Promise> ch) const
    {
        this->ch = ch;
        return false;
    }
    Promise &await_resume() const { return ch.promise(); }
};
template<typename Promise = void>
struct coroutine_get_coroutine_handle_t
{
    mutable std::coroutine_handle<Promise> ch;
    bool await_ready() const { return false; }
    bool await_suspend(std::coroutine_handle<Promise> ch) const
    {
        this->ch = ch;
        return false;
    }
    std::coroutine_handle<Promise> await_resume() const { return ch; }
};

#endif // EXT_INFRASTRUCTURE__EXT_THREAD_H
