/**
 * @file    test_moving_around_containers.cpp
 * @brief   Benchmarks how much time does moving around various containers.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include <array>
#include <memory>
#include <vector>

#include "benchmark_timer.hpp"
#include "serial_logger.hpp"

#include "batch_of_samples.hpp"

#include "jungles_os_helpers/freertos/thread.hpp"

static __attribute__((optimize("O0"))) void run_benchmark();

extern "C" void app_main()
{
    jungles::freertos::thread thread{"benchmarks", 2560, 24};
    thread.start(run_benchmark);
    thread.join();
}

static void run_benchmark()
{
    constexpr unsigned number_of_tests{1024};
    constexpr unsigned container_size{1024};

    SerialLogger logger;
    BenchmarkTimer timer;

    logger.log(LogLevel::info) << "Performing construction and then move-construction benchmark, for "
                               << number_of_tests << " and container size: " << container_size;

    {
        auto begin_time_us{timer.time_microseconds()};
        for (unsigned i{0}; i < number_of_tests; ++i)
        {
            auto p_arr{std::make_unique<std::array<unsigned, container_size>>()};
            [[maybe_unused]] auto new_p{std::move(p_arr)};
        }
        auto end_time_us{timer.time_microseconds()};
        unsigned duration_us{end_time_us - begin_time_us};
        logger.log(LogLevel::info) << "std::unique_ptr<std::array<unsigned, " << container_size
                                   << ">> duration: " << duration_us;
    }

    {
        auto begin_time_us{timer.time_microseconds()};
        for (unsigned i{0}; i < number_of_tests; ++i)
        {
            std::array<unsigned, container_size> arr{};
            [[maybe_unused]] auto new_arr{std::move(arr)};
        }
        auto end_time_us{timer.time_microseconds()};
        unsigned duration_us{end_time_us - begin_time_us};
        logger.log(LogLevel::info) << "std::array<unsigned, " << container_size << "> duration: " << duration_us;
    }

    {
        auto begin_time_us{timer.time_microseconds()};
        for (unsigned i{0}; i < number_of_tests; ++i)
        {
            std::vector<unsigned> vec(container_size);
            [[maybe_unused]] auto new_vec{std::move(vec)};
        }
        auto end_time_us{timer.time_microseconds()};
        unsigned duration_us{end_time_us - begin_time_us};
        logger.log(LogLevel::info) << "std::vector<unsigned> with size: " << container_size
                                   << ", duration: " << duration_us;
    }

    {
        using BatchOfSamples = BatchOfSamplesTemplate<container_size>;

        auto begin_time_us{timer.time_microseconds()};
        for (unsigned i{0}; i < number_of_tests; ++i)
        {
            BatchOfSamples b;
            [[maybe_unused]] auto new_b{std::move(b)};
        }
        auto end_time_us{timer.time_microseconds()};
        unsigned duration_us{end_time_us - begin_time_us};
        logger.log(LogLevel::info) << "BatchOfSamplesTemplate<" << container_size << "> duration: " << duration_us;
    }
}