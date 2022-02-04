#include <psi/fiber/api.hpp>

#include <iostream>

using namespace psi::fiber;

int main()
{
    run_scheduler([]()
                  {
                      self::spawn([]()
                                        { std::cout << "Hello fron fiber 2" << std::endl; });

                      std::cout << "Hello from fiber 1" << std::endl;

                      self::yield();

                      std::cout << "after yield fiber 1" << std::endl; }

    );

    return 0;
}