#include <psi/fiber/api.hpp>

#include <iostream>
#include <cassert>

using namespace psi::fiber;

// some bad and slow code
size_t fibbo(int n)
{
    if (n == 0 || n == 1)
    {
        return 1;
    }
    return fibbo(n - 1) + fibbo(n - 2);
}

int main()
{
    run_scheduler([]()
                  {
                      auto handler = spawn([]()
                                           {
                                               std::cout << "Hello from fiber " << self::get_id() << std::endl
                                                         << std::flush;
                                               self::yield();
                                               auto f = fibbo(40);
                                               assert(f == 165580141);
                                               std::cout << f << std::endl
                                                         << std::flush;
                                           });

                      std::cout << "Hello from fiber " << self::get_id() << std::endl;

                      self::yield();

                      std::cout << "after yield fiber "  << self::get_id() << std::endl;
                      handler.join(); }

    );

    return 0;
}