#include <psi/fiber/core/api.hpp>

#include <psi/fiber/core/scheduler.hpp>

namespace psi::fiber
{

    void run_scheduler(fiber_routine init)
    {
        scheduler sched;
        sched.run(std::move(init));
    }

    join_handle spawn(fiber_routine init)
    {
        auto *fiber = get_current_scheduler()->spawn(std::move(init));
        return join_handle(fiber);
    }

    namespace self
    {
        void yield()
        {
            get_current_scheduler()->yield();
        }

        fiber_id get_id()
        {
            return get_current_fiber()->id();
        }

    }
}
