//
// Created by Kosho Owa on 2016/08/19.
// Timer thread to monitor statistics of Worker threads
//

#include "Timer.h"

void Timer::Start() {
    stats_->ShowProgressHeader();
    while (true){
        this_thread::sleep_for(chrono::seconds(options_->interval_sec_));
        stats_->ShowProgress();
        if (stats_->IsFinished()) break;
    }
}

Timer::Timer(Stats *stats, Options *options) : stats_(stats), options_(options) {}
