#include <psi/fiber/core/api.hpp>

#include <psi/fiber/core/scheduler.hpp>

namespace psi::fiber
{

    void run_scheduler(fiber_routine init)
    {
        scheduler sched;
        sched.run(std::move(init));
    }

    namespace self
    {
        void yield()
        {
            get_current_scheduler()->yield();
        }

        void spawn(fiber_routine init)
        {
            get_current_scheduler()->spawn(std::move(init));
        }

    }
}
