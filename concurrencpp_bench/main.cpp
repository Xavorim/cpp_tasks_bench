#include <concurrencpp/concurrencpp.h>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <ranges>
#include <vector>

concurrencpp::result<void> async_sleep(auto s, std::shared_ptr<concurrencpp::worker_thread_executor> wte,
                 std::shared_ptr<concurrencpp::timer_queue> tq) {
    co_await tq->make_delay_object(s, wte);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr,
                "Please specify the number of tasks\n"
                "Example: %s 10000\n",
                argv[0]);
        return EXIT_FAILURE;
    }
    
    concurrencpp::runtime runtime;
    const auto thread_worker = runtime.make_worker_thread_executor();
    const auto timer_queue = runtime.timer_queue();

    std::vector<concurrencpp::result<void>> tasks;

    for (auto i : std::views::iota(0, std::stoi(argv[1]))) {
        tasks.push_back(async_sleep(std::chrono::seconds(10), thread_worker, timer_queue));
    }

    for (auto &task : tasks) {
        task.wait();
    }
}