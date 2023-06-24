# cpp_tasks_bench

If you've come across the ["How Much Memory Do You Need to Run 1 Million Concurrent Tasks?"](https://pkolaczk.github.io/memory-consumption-of-async/) article, you might've noticed that it lacked any benchmarks against C++ libraries.
As such, I've decided to try pitting several C++ async/await libraries against the current incumbent which is tokio (Rust).

Regardless of the methodology which was criticized in the aforementioned article, I still found it interesting and tried to replicate the same benchmark in C++.

The repo uses 3 C++ libraries:
- [Asio](https://github.com/chriskohlhoff/asio)
- [Concurrencpp](https://github.com/David-Haim/concurrencpp)
- [Libunifex](https://github.com/facebookexperimental/libunifex)
  
And tries to replicate the same program. A vector of awaitable tasks, which are then awaited in a for range loop. Or with unifex, using a sender/receiver algorithm (when_all_range).

## Requirements
- A C++ 20 compiler with coroutine support.
- CMake > 3.20
- A Rust toolchain (Optional)

## Dependencies
- Asio
- Concurrencpp
- Libunifex
  
The CMake script will fetch them using FetchContent.

## Building
```bash
cmake -Bbin
cmake --build bin --parallel
```
The build script will also default to building in Release mode. It will also attempt to build the tokio example if it find `cargo`.

## Running the benchmarks
```bash
/usr/bin/time -v ./bin/<exe name> <num_of_task>
# or
/usr/bin/time -f "Memory: %M kb" ./bin/<exe name> <num of tasks>
#example
/usr/bin/time -v ./bin/asioexe 1000
/usr/bin/time -v ./bin/concurrencppexe 1000
/usr/bin/time -v ./bin/unifexexe 1000
/usr/bin/time -v ./bin/tokio_bench 1000
```

## Results on my system:

### System info
- (WSL) Ubuntu 20.04.6 LTS on Windows 10 x86_64
- 8gb ram
- Intel i7-8550U (8) @ 1.991GHz

### 1 task
- Asio: 1668 kb
- Unifex: 1844 kb
- Concurrencpp: 1928 kb
- Tokio: 3092 kb

### 10k tasks
- Tokio: 6908 kb
- Concurrencpp: 7840 kb
- Asio: 10244 kb
- Unifex: 10328 kb

### 100k tasks
- Tokio: 23824 kb
- Concurrencpp: 61460 kb
- Asio: 86652 kb
- Unifex (hangs)

### 1 million tasks
- Tokio: 214136 kb
- Concurrencpp: 605972 kb
- Asio: 853144 kb
- Unifex (hangs)
