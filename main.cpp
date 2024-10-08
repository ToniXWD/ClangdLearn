#include "fmt/core.h" // 引入fmt库
#include <algorithm>
#include <coroutine>
#include <iostream>
#include <optional>
#include <ranges>
#include <vector>


// 使用C++20的range库
void print_even_numbers(const std::vector<int> &numbers) {
  auto even_numbers =
      numbers | std::views::filter([](int n) { return n % 2 == 0; });
  for (const auto &num : even_numbers) {
    fmt::print("{} ", num);
  }
  fmt::print("\n");
}

// 使用C++20的协程
struct Generator {
  struct promise_type {
    int current_value;
    auto yield_value(int value) -> std::suspend_always {
      current_value = value;
      return {};
    }
    auto initial_suspend() -> std::suspend_always { return {}; }
    auto final_suspend() noexcept -> std::suspend_always { return {}; }
    auto get_return_object() -> Generator { return Generator{this}; }
    void return_void() {}
    void unhandled_exception() { std::terminate(); }
  };

  using handle_type = std::coroutine_handle<promise_type>;
  handle_type coro;

  Generator(promise_type *p) : coro(handle_type::from_promise(*p)) {}
  ~Generator() {
    if (coro)
      coro.destroy();
  }

  auto move_next() -> bool {
    coro.resume();
    return !coro.done();
  }
  [[nodiscard]] auto current_value() const -> int {
    return coro.promise().current_value;
  }
};

auto generate_numbers(int max) -> Generator {
  for (int i = 1; i <= max; ++i) {
    co_yield i;
  }
}

auto main() -> int {
  std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  fmt::print("Even numbers: ");
  print_even_numbers(numbers);

  fmt::print("Generated numbers: ");
  auto gen = generate_numbers(10);
  while (gen.move_next()) {
    fmt::print("{} ", gen.current_value());
  }
  fmt::print("\n");

  return 0;
}
