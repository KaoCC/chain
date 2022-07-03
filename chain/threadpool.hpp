#pragma once

#include <condition_variable>
#include <future>
#include <mutex>
#include <queue>
#include <thread>

namespace chain {

class threadpool final {
 public:
  threadpool();

  /// Enqueue a callable into the thread pool
  /// @param[in] callable The callable object to be called.
  /// @param[in] args the arguments of the callable.
  /// @return The future object of the result of the callable.
  template <class Callable, class... Args>
  [[nodiscard]] auto enqueue(Callable &&callable, Args &&...args)
      -> std::future<std::invoke_result_t<Callable, Args...>>;

  ~threadpool();

 private:
  std::vector<std::thread> workers;

  std::queue<std::packaged_task<void()>> tasks;

  mutable std::mutex queue_mutex;
  std::condition_variable condition;

  bool stop{false};
};

threadpool::threadpool() {
  const auto num_threads{std::thread::hardware_concurrency()};

  for (auto i{0UL}; i < num_threads; ++i) {
    workers.emplace_back([this]() {
      while (true) {
        std::packaged_task<void()> task;
        {
          std::unique_lock<std::mutex> lock(queue_mutex);
          condition.wait(lock, [this] { return stop || !tasks.empty(); });

          if (stop && tasks.empty()) {
            return;
          }
          task = std::move(tasks.front());
          tasks.pop();
        }

        std::invoke(task);
      }
    });
  }
}

template <class Callable, class... Args>
auto threadpool::enqueue(Callable &&callable, Args &&...args) -> std::future<std::invoke_result_t<Callable, Args...>> {
  using return_type = std::invoke_result_t<Callable, Args...>;

  std::packaged_task<return_type()> task{
      [&]() { return std::invoke(std::forward<Callable>(callable), std::forward<Args>(args)...); }};

  auto task_future{task.get_future()};
  {
    std::unique_lock<std::mutex> lock(queue_mutex);
    tasks.emplace(std::move(task));
  }

  condition.notify_one();
  return task_future;
}

threadpool::~threadpool() {
  {
    std::unique_lock<std::mutex> lock(queue_mutex);
    stop = true;
  }

  condition.notify_all();
  for (auto &worker : workers) {
    worker.join();
  }
}

}  // namespace chain
