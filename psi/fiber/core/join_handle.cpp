#include <psi/fiber/core/join_handle.hpp>
#include <psi/fiber/core/scheduler.hpp>

namespace psi::fiber
{

    // implementation observed source interface
    void join_handle::on_complete()
    {
        completed_ = true;
        waitee_.awake();
    }

    void join_handle::join()
    {
        is_attached();

        if (!completed_)
        {
            waitee_.suspend();
        }

        fiber_ = nullptr;
    }

    void join_handle::detach()
    {
        is_attached();
        fiber_->set_watcher(nullptr);
        fiber_ = nullptr;
    }

    void join_handle::is_attached()
    {
        assert(fiber_ != nullptr);
    }

    join_handle::join_handle(fiber *fiber) : fiber_(fiber)
    {
        fiber_->set_watcher(this);
    }

    join_handle::~join_handle()
    {
        assert(fiber_ == nullptr);
    }

    join_handle::join_handle(join_handle &&that)
    {
        fiber_ = std::exchange(that.fiber_, nullptr);
        completed_ = that.completed_;
        if (!completed_)
        {
            fiber_->set_watcher(this);
        }
    }

}