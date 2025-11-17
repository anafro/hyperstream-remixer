#pragma once
#include <iostream>
#include <chrono>


namespace HyperstreamRemixer::Performance {
    static std::string current_stopwatch_label;
    static std::chrono::steady_clock::time_point stopwatch_started_at;
    static std::chrono::steady_clock::time_point stopwatch_stopped_at;

    inline void stopwatch_start(const std::string& stopwatch_label) {
        current_stopwatch_label = stopwatch_label;
        stopwatch_started_at = std::chrono::steady_clock::now();
    }

    inline void stopwatch_stop_and_report() {
        stopwatch_stopped_at = std::chrono::steady_clock::now();
        std::clog << current_stopwatch_label << ": " << std::chrono::duration_cast<std::chrono::milliseconds>(stopwatch_stopped_at - stopwatch_started_at).count() << " ms" << std::endl;
    }
}

