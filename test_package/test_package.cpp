
#include <iostream>

#include "chain/threadpool.hpp"
#include "chain/timer.hpp"

int main() {
  chain::timer<int, double> t;
  chain::threadpool p;

  std::cout << "Pass ! " << std::endl;

  return 0;
}