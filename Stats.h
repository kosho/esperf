//
// Created by Kosho Owa on 2016/08/19.
//

#ifndef ESPERF_STATS_H
#define ESPERF_STATS_H

#include <iostream>
#include <sstream>

#ifdef __linux__
#include <mutex>
#include <atomic>
#endif

#include "Options.h"

using namespace std;

class Stats {
public:
    Stats(Options *options_, mutex *mtx_for_cout_);

    bool IsFinished() const;

    void Count(const int success, const int error_curl, const int error_http,
               const u_long size_upload, const u_long size_download, const double time_transfer);

    void ShowProgressHeader();

    void ShowProgress();

    void ShowResult();

private:
    Options *options_;
    mutex *mtx_for_cout_;

    // Keep start and stop time
    chrono::steady_clock::time_point clock_start_ = chrono::steady_clock::now();
    chrono::steady_clock::time_point clock_stop_;

    // Finished processing
    bool finished_ = false;

    // Counters from 0 sec
    atomic_ulong success_;
    atomic_ulong error_curl_;
    atomic_ulong error_http_;
    atomic_ulong size_upload_;
    atomic_ulong size_download_;
    atomic<double> time_transfer_{0.0};

    // Counters after warm-up seconds
    atomic_ulong wu_success_;
    atomic_ulong wu_error_curl_;
    atomic_ulong wu_error_http_;
    atomic_ulong wu_size_upload_;
    atomic_ulong wu_size_download_;
    atomic<double> wu_time_transfer_;

    u_long prev_success_ = 0;
    u_long prev_error_curl_ = 0;
    u_long prev_error_http_ = 0;
    u_long prev_size_upload_ = 0;
    u_long prev_size_download_ = 0;
    double prev_time_transfer_ = 0;

    // Function to add value to atomic double
    void add_to_atomic_double(atomic<double> *var, double val) {
        auto current = var->load();
        while (!var->compare_exchange_weak(current, current + val));
    }

    void PrintLine(const string option, const u_int value);

    void PrintLine(const string option, double value);

    void safe_cout(const string msg);

    void safe_cerr(const string msg);

};

#endif //ESPERF_STATS_H
