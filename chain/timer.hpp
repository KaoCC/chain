
#pragma once

#include <chrono>
#include <unordered_map>

namespace chain {

template <typename Token, typename Rep = double>
class timer final {
 public:
  using duration = std::chrono::duration<Rep>;

  template <typename Callable, typename... Args>
  duration measure(const Token token, Callable&& callable, Args&&... args) noexcept;

  /// Get the measured time record of the speficied token.
  /// @param[in] token The token of the time record.
  /// @return The corresponding time record of the token.
  [[nodiscard]] duration get_measurement(const Token token) const noexcept;

  /// Reset the time record which corresponds to a specific token.
  /// @param[in] token The token of the time record.
  void reset(const Token token) noexcept;

  /// Clear all the time record.
  void clear() noexcept;

 private:
  std::unordered_map<Token, duration> time_record;
};

template <typename Token, typename Rep>
template <typename Callable, typename... Args>
typename timer<Token, Rep>::duration timer<Token, Rep>::measure(const Token token, Callable&& callable,
                                                                Args&&... args) noexcept {
  const auto start{std::chrono::steady_clock::now()};
  std::invoke(std::forward<Callable>(callable), std::forward<Args>(args)...);
  const auto stop{std::chrono::steady_clock::now()};

  time_record[token] += (stop - start);

  return time_record[token];
}

template <typename Token, typename Rep>
typename timer<Token, Rep>::duration timer<Token, Rep>::get_measurement(const Token token) const noexcept {
  return time_record.at(token);
}

template <typename Token, typename Rep>
void timer<Token, Rep>::reset(const Token token) noexcept {
  if (time_record.count(token) > 0UL) {
    time_record[token] = duration{};
  }
}

template <typename Token, typename Rep>
void timer<Token, Rep>::clear() noexcept {
  time_record.clear();
}

}  // namespace chain