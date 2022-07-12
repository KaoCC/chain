

#include <catch2/catch_test_macros.hpp>

#include "threadpool.hpp"
#include "timer.hpp"

TEST_CASE("Timer Operations", "[timer]") {
  enum class test_token : int { test_0, test_1, test_2 };

  chain::timer<test_token, double> timer;

  // empty
  const auto test_callable = []() { std::this_thread::sleep_for(std::chrono::duration<double>(0.1)); };

  timer.measure(test_token::test_0, test_callable);

  const auto duration = timer.get_measurement(test_token::test_0);

  CHECK(std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() >= 0UL);
}