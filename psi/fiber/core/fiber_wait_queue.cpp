#include <psi/fiber/core/fiber_wait_queue.hpp>

#include <psi/fiber/core/scheduler.hpp>

namespace psi::fiber
{
    class fiber_wait_queue::impl
    {
    public:
        void park()
        {
            fiber *fiber = get_current_fiber();
            queue_.push_back(fiber);
            get_current_scheduler()->suspend();
        }

        void awake_one()
        {
            if (queue_.empty())
                return;
            fiber *fiber = queue_.pop_front();
            get_current_scheduler()->resume(fiber);
        }

        void awake_all()
        {
            while (!queue_.empty())
            {
                fiber *fiber = queue_.pop_front();
                get_current_scheduler()->resume(fiber);
            }
        }

        ~impl()
        {
            assert(queue_.empty());
        }

    private:
        fiber_queue queue_;
    };

    fiber_wait_queue::fiber_wait_queue() : pimpl_(std::make_unique<impl>())
    {
    }

    fiber_wait_queue::~fiber_wait_queue()
    {
    }

    void fiber_wait_queue::park()
    {
        pimpl_->park();
    }

    void fiber_wait_queue::awake_one()
    {
        pimpl_->awake_one();
    }

    void fiber_wait_queue::awake_all()
    {
        pimpl_->awake_all();
    }
}