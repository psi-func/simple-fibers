#include <psi/fiber/core/fiber.hpp>

#include <psi/fiber/core/scheduler.hpp>

#include <iostream>

namespace psi::fiber
{
    fiber::fiber(fiber_routine routine, context::stack &&stack, fiber_id id) : id_(id),
                                                                               routine_(routine),
                                                                               call_stack_(std::move(stack)),
                                                                               state_(fiber_state::STARTING)
    {
        execution_context_.setup(call_stack_.view(), trampoline);
    }

    fiber::~fiber()
    {
    }

    void fiber::trampoline()
    {
        auto *fiber = get_current_fiber();
        fiber->set_state(fiber_state::RUNNING);
        try
        {
            fiber->run_routine();
        }
        catch (...)
        {
            std::cerr << "exception on fiber" << std::endl;
        }

        get_current_scheduler()->terminate();
    }
}