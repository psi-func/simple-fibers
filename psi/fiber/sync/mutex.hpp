#pragma once

#include <psi/fiber/core/fiber_wait_queue.hpp>

#include <cassert>

namespace psi::fiber
{

    class mutex
    {
    public:
        mutex()
        {
        }

        mutex(const mutex &that) = delete;
        mutex &operator=(const mutex &that) = delete;

        mutex(mutex &&that);
        mutex &operator=(mutex &&that);

        void lock()
        {
            while (locked_)
            {
                queue_.park();
            }

            locked_ = true;
        }

        void unlock()
        {
            assert(locked_ == true);
            locked_ = false;
            queue_.awake_one();
        }

        bool try_lock()
        {
            bool was_locked = locked_;
            locked_ = true;
            return !was_locked;
        }

    private:
        bool locked_ = false;
        fiber_wait_queue queue_;
    };
}
