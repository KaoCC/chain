
#pragma once

#include <chrono>
#include <unordered_map>

namespace chain {

template <typename Token, typename Rep = double>
class timer final {
 public:
  using duration = std::chrono::duration<Rep>;

  template <typename Callable, typename... Args>
  duration measure(const Token token, Callable&& callable, Args&&... args) noexcept {
    const auto start{std::chrono::steady_clock::now()};
    std::invoke(std::forward<Callable>(callable), std::forward<Args>(args)...);
    const auto stop{std::chrono::steady_clock::now()};

    time_record[token] += (stop - start);

    return time_record[token];
  }

  [[nodiscard]] duration get_measurement(const Token token) const noexcept { return time_record.at(token); }

  void reset(const Token token) noexcept {
    if (time_record.count(token) > 0UL) {
      time_record[token] = duration{};
    }
  }

  void clear() noexcept { time_record.clear(); }

 private:
  std::unordered_map<Token, duration> time_record;
};

}  // namespace chain