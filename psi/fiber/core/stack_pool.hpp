#pragma once

#include <psi/context/stack.hpp>

namespace psi::fiber
{
    context::stack acquire_stack();
    void release_stack(context::stack stack);

}
