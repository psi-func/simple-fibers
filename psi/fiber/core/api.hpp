#pragma once

#include <functional>
#include <chrono>

#include <psi/utility/id.hpp>
#include <psi/fiber/core/join_handle.hpp>

namespace psi::fiber
{
    using fiber_routine = std::function<void()>;

    using fiber_id = psi::utility::id;

    void run_scheduler(fiber_routine init);

    join_handle spawn(fiber_routine routine);

    namespace self
    {
        void yield();

        fiber_id get_id();

    }

}