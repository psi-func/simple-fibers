#include <psi/fiber/api.hpp>

#include <psi/fiber/sync/mutex.hpp>

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

static int counter = 0;
psi::fiber::mutex mutex;

int main()
{
    run_scheduler([]()
                  {
                      

                      auto handler1 = spawn([]()
                                           {
                                               ::mutex.lock();
                                               std::cout << "Hello from fiber " << self::get_id() << std::endl
                                                         << std::flush;
                                                self::yield();
                                               auto f = fibbo(40);
                                               assert(f == 165580141);
                                               std::cout << f << std::endl
                                                         << std::flush;
                                                ::mutex.unlock();
                                           });

                        auto handler2 = spawn([]() {
                            ::mutex.lock();
                            std::cout << "from fiber " << self::get_id() << std::endl;
                            ::mutex.unlock();

                        });

                        self::yield();

                        std::cout << "Hello from main fiber " << self::get_id() << std::endl;

                        handler1.join();
                        handler2.join();

                      std::cout << "after join fiber "  << self::get_id() << std::endl; }

    );

    return 0;
}