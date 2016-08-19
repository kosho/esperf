//
// Created by Kosho Owa on 2016/08/19.
//

#include "Timer.h"

void Timer::start() {
    stats->showProgressHeader();
    while (true){
        this_thread::sleep_for(chrono::seconds(options->interval));
        stats->showProgress();
        if (stats->isFinished()) break;
    }
}

Timer::Timer(Stats *stats, Options *options) : stats(stats), options(options) {}
