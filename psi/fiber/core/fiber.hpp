#pragma once

#include <psi/intrusive/intrusive_list.hpp>

#include <psi/fiber/core/api.hpp>

#include <psi/context/context.hpp>
#include <psi/context/stack.hpp>

namespace psi::fiber
{
    enum class fiber_state
    {
        STARTING,
        RUNNABLE,
        RUNNING,
        SUSPENDED,
        TERMINATED,
    };

    class fiber : public intrusive::intrusive_node<fiber>
    {
    public:
        friend class scheduler;

        ~fiber();

        utility::id id() const
        {
            return id_;
        }

        context::exec_context &context()
        {
            return execution_context_;
        }

        context::stack &stack()
        {
            return call_stack_;
        }

        fiber_state state()
        {
            return state_;
        }

        // setter
        void set_state(fiber_state target_state)
        {
            state_ = target_state;
        }

        void run_routine()
        {
            routine_();
        }

    private:
        static void trampoline();

        fiber(fiber_routine routine, context::stack &&stack, fiber_id id);

    private:
        fiber_id id_;
        fiber_state state_;
        fiber_routine routine_;
        context::stack call_stack_;
        context::exec_context execution_context_;
    };
}
