#pragma once

#include "Coroutine.hpp"


namespace coco {

/// @brief Semaphore.
/// See https://en.wikipedia.org/wiki/Semaphore_(programming)
class Semaphore {
public:
    /// @brief Guard that can be used to ensure posting to the semaphore at function exit.
    ///
    /// Coroutine foo() {
    ///   // wait until wie acquired a token from the semaphore
    ///   co_await semaphore.untilAcquired();
    ///
    ///   // we passed the semaphore and acquired a token, now make sure that we release the token again
    ///   Semaphore::Guard guard(semaphore);
    ///
    ///   if (bar) {
    ///     // semaphore.release() gets called here (before exiting the function)
    ///     co_return;
    ///   }
    ///
    ///   // semaphore.release() gets called here (before exiting the function)
    /// }
    class Guard {
    public:
        [[nodiscard]] Guard(Semaphore &semaphore) : semaphore_(semaphore) {}

        ~Guard() {
            semaphore_.release();
        }

    protected:
        Semaphore &semaphore_;
    };


    /// Construct a semaphore with a given number of initial tokens that can be handed out.
    /// @param n number of initial tokens
    explicit Semaphore(int n) : n_(n) {}

    /// @brief Wait until a token is acquired.
    ///
    [[nodiscard]] Awaitable<CoroutineTask<>> untilAcquired() {
        // check if tokens are available
        if (n_ > 0) {
            --n_;
            return {};
        }

        // wait until token is available
        return {taskList_};
    }

    /// @brief Release a token and resume the next coroutine waiting for a token.
    ///
    void release() {
        n_ += 1 - int(taskList_.doFirst());
    }

protected:
    // number of tokens
    int n_;

    // list of waiting coroutines
    CoroutineTaskList<> taskList_;
};

} // namespace coco
