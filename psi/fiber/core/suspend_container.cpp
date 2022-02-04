#include <psi/fiber/core/suspend_container.hpp>

#include <psi/fiber/core/scheduler.hpp>

namespace psi::fiber
{
    void suspend_container::suspend()
    {
        waitee_ = get_current_fiber();
        get_current_scheduler()->suspend();
    }

    void suspend_container::awake()
    {
        if (waitee_ != nullptr)
        {
            get_current_scheduler()->resume(waitee_);
        }
    }

}