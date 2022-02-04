#pragma once

#include <psi/fiber/core/observed_source.hpp>
#include <psi/fiber/core/suspend_container.hpp>

namespace psi::fiber
{

    struct join_handle : public observed_source
    {
        friend class fiber;

    public:
        join_handle(fiber *fiber);

        // move
        join_handle(join_handle &&that);

        ~join_handle();

        // non-copyable
        join_handle(const join_handle &that) = delete;
        join_handle &operator=(const join_handle &that) = delete;

        void join();
        void detach();

        void on_complete() override;

    private:
        void is_attached();

    private:
        bool completed_ = false;
        fiber *fiber_;
        suspend_container waitee_;
    };

}