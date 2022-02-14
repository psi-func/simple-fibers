#include <psi/fiber/core/stack_pool.hpp>

#include <vector>

namespace psi::fiber
{
    using context::stack;

    static const std::size_t kDefaultStackSizeInPages = 8;

    class stack_pool
    {
    public:
        stack_pool() = default;
        ~stack_pool() = default;

        context::stack acquire_stack()
        {
            if (!reusables_.empty())
            {
                auto stack = std::move(reusables_.back());
                reusables_.pop_back();
                return stack;
            }
            else
            {
                return context::stack::allocate_pages(kDefaultStackSizeInPages);
            }
        }

        void release_stack(context::stack stack)
        {
            reusables_.push_back(std::move(stack));
        }

    private:
        std::vector<context::stack> reusables_;
    };

    static thread_local stack_pool stack_pool;

    context::stack acquire_stack()
    {
        return stack_pool.acquire_stack();
    }

    void release_stack(context::stack stack)
    {
        stack_pool.release_stack(std::move(stack));
    }

}
