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
        [[nodiscard]] Guard(Semaphore &semaphore) : semaphore(semaphore) {}

        ~Guard() {
            this->semaphore.release();
        }

    protected:
        Semaphore &semaphore;
    };


    /// Construct a semaphore with a given number of initial tokens that can be handed out.
    /// @param n number of initial tokens
    explicit Semaphore(int n) : n(n) {}

    /// @brief Wait until a token is acquired.
    ///
    [[nodiscard]] Awaitable<> untilAcquired() {
        // check if tokens are available
        if (this->n > 0) {
            --this->n;
            return {};
        }

        // wait until token is available
        return {this->taskList};
    }

    /// @brief Release a token and resume the next coroutine waiting for a token.
    ///
    void release() {
        this->n += 1 - int(this->taskList.doFirst());
    }

protected:
    // number of tokens
    int n;

    // list of waiting coroutines
    TaskList<CoroutineTask> taskList;
};

} // namespace coco
