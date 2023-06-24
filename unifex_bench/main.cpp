#include <unifex/sync_wait.hpp>
#include <unifex/task.hpp>
#include <unifex/timed_single_thread_context.hpp>
#include <unifex/when_all_range.hpp>

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <ranges>
#include <vector>

unifex::timed_single_thread_context TIMER;

auto async_sleep(auto s) { return unifex::schedule_after(TIMER.get_scheduler(), s); }

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr,
                      "Please specify the number of tasks\n"
                      "example: %s 10000\n",
                      argv[0]);
        return EXIT_FAILURE;
    }

    std::vector<unifex::task<void>> tasks;

    for (auto i : std::views::iota(0, std::stoi(argv[1]))) {
        tasks.push_back(
            []() -> unifex::task<void> { co_await async_sleep(std::chrono::seconds(10)); }());
    }
     
    unifex::sync_wait(unifex::when_all_range(std::move(tasks)));
}