#include <psi/fiber/core/scheduler.hpp>
#include <psi/fiber/core/stack_pool.hpp>

#include <psi/context/context.hpp>
// #include <psi/context/stack.hpp>

#include <iostream>

namespace psi::fiber
{

    static thread_local scheduler *current_scheduler = nullptr;

    scheduler *get_current_scheduler()
    {
        return current_scheduler;
    }

    fiber *get_current_fiber()
    {
        auto ptr = get_current_scheduler();
        return ptr->get_current_fiber();
    }

    struct scope
    {
        scope(scheduler *scheduler)
        {
            if (current_scheduler != nullptr)
            {
                std::abort();
            }
            current_scheduler = scheduler;
        }

        ~scope()
        {
            current_scheduler = nullptr;
        }
    };

    ///////////////////////////////////

    fiber *scheduler::get_current_fiber()
    {
        return cur_running;
    }

    fiber *scheduler::spawn(fiber_routine routine)
    {
        fiber *new_fiber = create_fiber(routine);
        schedule(new_fiber);
        return new_fiber;
    }

    void scheduler::run(fiber_routine init)
    {
        scope scope(this);
        spawn(std::move(init));
        run_loop();
    }

    void scheduler::yield()
    {
        auto fiber = get_current_fiber();
        fiber->set_state(fiber_state::RUNNABLE);
        switch_to_scheduler(fiber);
    }

    void scheduler::terminate()
    {
        auto fiber = get_current_fiber();
        fiber->set_state(fiber_state::TERMINATED);
        switch_to_scheduler(fiber);
    }

    void scheduler::suspend()
    {
        auto *fiber = get_current_fiber();
        fiber->set_state(fiber_state::SUSPENDED);
        get_current_scheduler()->switch_to_scheduler(fiber);
    }

    void scheduler::resume(fiber *fiber)
    {
        assert(fiber->state() == fiber_state::SUSPENDED);

        fiber->set_state(fiber_state::RUNNABLE);
        schedule(fiber);
    }

    // CONTROL SECTION

    void scheduler::run_loop()
    {
        while (!run_queue_.empty())
        {
            auto *selected_fiber = run_queue_.pop_front();
            aside(selected_fiber);
            dispatch(selected_fiber);
        }
    }

    void scheduler::aside(fiber *fiber)
    {
        cur_running = fiber;
        fiber->set_state(fiber_state::RUNNING);
        switch_to_fiber(fiber);
        cur_running = nullptr;
    }

    void scheduler::dispatch(fiber *fiber)
    {
        switch (fiber->state())
        {
        case fiber_state::RUNNABLE: // from yield
            schedule(fiber);
            break;
        case fiber_state::SUSPENDED:
            // do not reschedule
            break;
        case fiber_state::TERMINATED: // from terminate
            destroy(fiber);
            break;
        default:
            std::cerr << "Uexpected fiber state" << std::endl;
            break;
        }
    }

    void scheduler::schedule(fiber *fiber)
    {
        run_queue_.push_back(fiber);
    }

    fiber *scheduler::create_fiber(fiber_routine routine)
    {
        auto stack = acquire_stack();
        fiber_id id = id_gen_.next_id();
        return new fiber(std::move(routine), std::move(stack), id);
    }

    void scheduler::destroy(fiber *fiber)
    {
        release_stack(std::move(fiber->stack()));
        delete fiber;
    }

    // low level operations
    void scheduler::switch_to_fiber(fiber *fiber)
    {
        loop_.switch_to(fiber->context());
    }

    void scheduler::switch_to_scheduler(fiber *self)
    {
        self->context().switch_to(loop_);
    }

}
