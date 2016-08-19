//
// Created by Kosho Owa on 2016/08/19.
//

#ifndef ESPERF_STATS_H
#define ESPERF_STATS_H

#include "Options.h"
#include <iostream>

using namespace std;

class Stats {
    Options *options;

    chrono::steady_clock::time_point clock_start = chrono::steady_clock::now();
    chrono::steady_clock::time_point clock_end;

    int iProgressWidth = 15;
    int iResultWidth = 15;
    bool finished = false;

    // counters from 0 sec
    atomic_ulong ulSuccess;
    atomic_ulong ulErrorCurl;
    atomic_ulong ulErrorHttp;
    atomic_ulong ulSizeUpload;
    atomic_ulong ulSizeDownload;
    atomic<double> dTimeTransfer {0.0};

    void add_to_atomic_double(atomic<double> *var, double val) {
        auto current = var->load();
        while (!var->compare_exchange_weak(current, current + val));
    }

    // counters after wrap up
    atomic_ulong ulAwSuccess;
    atomic_ulong ulAwErrorCurl;
    atomic_ulong ulAwErrorHttp;
    atomic_ulong ulAwSizeUpload;
    atomic_ulong ulAwSizeDownload;
    atomic<double> dAwTimeTransfer;

    unsigned long prevSuccess = 0;
    unsigned long prevErrorCurl = 0;
    unsigned long prevErrorHttp = 0;
    unsigned long prevSizeUpload = 0;
    unsigned long prevSizeDownload = 0;
    double prevTimeTransfer = 0;

public:
    Stats(Options *options);
    bool isFinished() const;
    void count(int success, int errorCurl, int errorHttp, unsigned long sizeUpload, unsigned long sizeDownload, double timeTransfer);
    void showProgressHeader();
    void showProgress();
    void showResult();
    void printLine(string strOtion, unsigned int value);
    void printLine(string strOtion, double value);
    void printLine(string strOtion, string value);
    void printLine(string strOtion, bool value);
};


#endif //ESPERF_STATS_H
