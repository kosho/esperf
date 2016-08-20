//
// Created by Kosho Owa on 2016/08/19.
//

#include "Esperf.h"

void Esperf::Run()
{
    Stats stats(options_);

    // Workers
    thread *thWorker;
    thWorker = new thread[options_->num_threads_];
    for (int i = 0; i < options_->num_threads_; i++) {
        thWorker[i] = thread(&Worker::Run, Worker(&stats, options_));
    }

    // create threads
    thread th_timer(&Timer::Start, Timer(&stats, options_));

    // run threads
    for (int i = 0; i < options_->num_threads_; i++) {
        thWorker[i].join();
    }
    th_timer.join();
    options_->Print();
    stats.ShowResult();
}

Esperf::Esperf(Options *options) : options_(options) {}