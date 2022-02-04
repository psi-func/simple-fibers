#pragma once

#include <psi/context/context.hpp>

#include <psi/fiber/core/fiber.hpp>
#include <psi/fiber/core/api.hpp>

#include <psi/utility/id.hpp>

// #include <psi/intrusive/intrusive_list.hpp>

namespace psi::fiber
{
    using fiber_queue = intrusive::intrusive_list<fiber>;

    class scheduler
    {
    public:
        void run();

        /**
         * @brief Get the current fiber object from global scope
         *
         * @return fiber* pointer to target fiber
         */
        fiber *get_current_fiber();

        /**
         * @brief create fiber and schedule it
         *
         * @param routine fiber routine
         * @return fiber* pointer to created fiber
         */
        fiber *spawn(fiber_routine routine);

        /**
         * @brief spawns first fiber and starts main loop
         *
         * @param init fiber routine
         */
        void run(fiber_routine init);

        /**
         * @brief switch to scheduler and reschedule bu youself
         *
         */
        void yield();

        /**
         * @brief permanently terminate fiber with deleting
         *
         */
        void terminate();

        /**
         * @brief suspend current fiber
         *
         */
        void suspend();

        /**
         * @brief return fiber back to scheduler
         *
         */
        void resume(fiber *fiber);

    private:
        // control loop
        void run_loop();

        void aside(fiber *fiber);
        void dispatch(fiber *fiber);

        void schedule(fiber *fiber);

        // building
        fiber *create_fiber(fiber_routine routine);
        void destroy(fiber *fiber);

        // low level
        void switch_to_scheduler(fiber *self);

        void switch_to_fiber(fiber *fiber);

    private:
        fiber *cur_running = nullptr;
        fiber_queue run_queue_;
        context::exec_context loop_;
        utility::id_generator id_gen_;
    };

    scheduler *get_current_scheduler();
    fiber *get_current_fiber();
}
