#pragma once

#include <atomic>
#include <cstddef>

namespace psi::utility
{
    using id = std::size_t;

    extern const id invalid_id;

    class id_generator
    {
    public:
        id next_id()
        {
            return next_.fetch_add(1) + 1;
        }

        void reset()
        {
            next_ = 0;
        }

    private:
        std::atomic<id> next_{0};
    };

}
