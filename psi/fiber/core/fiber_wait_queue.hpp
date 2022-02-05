#pragma once

#include <memory>

namespace psi::fiber
{

    class fiber_wait_queue
    {
    public:
        fiber_wait_queue();
        ~fiber_wait_queue();

        fiber_wait_queue(const fiber_wait_queue &that) = delete;
        fiber_wait_queue &operator=(const fiber_wait_queue &that) = delete;

        void park();

        void awake_one();

        void awake_all();

    private:
        class impl;
        std::unique_ptr<impl> pimpl_;
    };
}
