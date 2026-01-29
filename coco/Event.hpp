#pragma once

#include "Coroutine.hpp"


namespace coco {

/// @brief Manual reset event inspired by Win32 event object (https://learn.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-createeventa)
///
class Event {
public:
    /// @brief Construct an event in nonsignaled state
    ///
    Event() = default;

    /// @brief Set the event to signaled state
    ///
    void set() {
        this->state = true;
        this->taskList.doAll();
    }

    /// @brief Reseset the event to nonsignaled state
    ///
    void reset() {
        this->state = false;
    }

    /// @brief Returns true if the event is in signaled state
    ///
    bool signaled() {
        return this->state;
    }

    /// @brief Returns true if the event is in nonsignaled state
    ///
    bool nonsignaled() {
        return !this->state;
    }

    /// @brief Wait until the event is in signaled state
    ///
    [[nodiscard]] Awaitable<> untilSignaled() {
        if (this->state) {
            return {};
        }
        return {this->taskList};
    }

protected:
    // list of waiting coroutines
    TaskList<CoroutineTask> taskList;
    bool state = false;
};

} // namespace coco
