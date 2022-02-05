#include <psi/fiber/api.hpp>

#include <psi/fiber/sync/mutex.hpp>
#include <psi/fiber/sync/condition_variable.hpp>

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
psi::fiber::condition_variable not_empty_;

std::vector<std::size_t> container;

int main()
{
    run_scheduler([]()
                  {
                      

                      auto handler1 = spawn([]()
                                           {
                                               self::yield();
                                               std::cout << "Hello from fiber " << self::get_id() << std::endl
                                                         << std::flush;
                                                ::mutex.lock();
                                               auto f = fibbo(40);
                                               assert(f == 165580141);
                                               container.push_back(f);
                                               ::mutex.unlock();
                                               not_empty_.notify_one();
                                               
                                               std::cout << f << std::endl
                                                         << std::flush;
                                                
                                           });

                        auto handler2 = spawn([]() {
                            ::mutex.lock();
                            
                            while (::container.empty())
                            {
                                std::cout << "waiting for numbers!!" << std::endl << std::flush;
                                ::not_empty_.wait(::mutex);
                            }
                            
                            std::cout << "from fiber " << self::get_id() << " " << ::container.back() << std::endl;
                            ::container.pop_back();
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