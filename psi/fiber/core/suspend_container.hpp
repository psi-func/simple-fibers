#pragma once

namespace psi::fiber
{
    class fiber;

    struct suspend_container
    {
        void suspend();
        void awake();

    private:
        fiber *waitee_;
    };
}