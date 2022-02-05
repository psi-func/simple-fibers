#pragma once

#include <psi/fiber/core/fiber_wait_queue.hpp>
#include <psi/fiber/sync/mutex.hpp>

namespace psi::fiber
{
    class condition_variable
    {
    public:
        template <typename Unlockable>
        void wait(Unlockable &sync)
        {
            sync.unlock();
            queue_.park();
            sync.lock();
        }

        template <typename Unlockable, typename Pred>
        void wait(Unlockable &sync, Pred predicate)
        {
            while (!predicate())
            {
                wait(sync);
            }
        }

                void notify_one()
        {
            queue_.awake_one();
        }

        void notify_all()
        {
            queue_.awake_all();
        }

    private:
        fiber_wait_queue queue_;
    };
}