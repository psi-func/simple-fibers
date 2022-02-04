#pragma once

namespace psi::fiber
{
    struct observed_source
    {
        virtual ~observed_source() = default;
        virtual void on_complete() = 0;
    };
}
