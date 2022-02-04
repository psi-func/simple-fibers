#pragma once

#include <functional>

#include <psi/utility/id.hpp>

#include <chrono>

namespace psi::fiber
{
    using fiber_routine = std::function<void()>;

    using fiber_id = psi::utility::id;

    void run_scheduler(fiber_routine init);

    namespace self
    {
        void yield();

        void spawn(fiber_routine routine);

    }

}