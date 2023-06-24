#include <asio.hpp>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <ranges>
#include <vector>

asio::awaitable<void> async_sleep(auto s) {
    asio::steady_timer timer(co_await asio::this_coro::executor, s);
    co_await timer.async_wait(asio::deferred);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr,
                "Please specify the number of tasks\n"
                "Example: %s 10000\n",
                argv[0]);
        return EXIT_FAILURE;
    }

    asio::io_context ctx{static_cast<int>(std::thread::hardware_concurrency())};
    
    std::vector<asio::awaitable<void>> tasks;

    for (auto i : std::views::iota(0, std::stoi(argv[1]))) {
        tasks.push_back(async_sleep(std::chrono::seconds(10)));
    }

    for (auto &&task : tasks) {
        asio::co_spawn(ctx, std::move(task), asio::detached);
    }
    ctx.run();
}