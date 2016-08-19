//
// Created by Kosho Owa on 2016/08/19.
//

#include "Esperf.h"

void Esperf::run()
{
    Stats stats(options);

    // Workers
    thread *thWorker;
    thWorker = new thread[options->number_of_threads];
    for (int i = 0; i < options->number_of_threads; i++) {
        thWorker[i] = thread(&Worker::run, Worker(&stats, options));
    }

    // create threads
    thread th_timer(&Timer::start, Timer(&stats, options));

    // run threads
    for (int i = 0; i < options->number_of_threads; i++) {
        thWorker[i].join();
    }
    th_timer.join();
    options->print();
    stats.showResult();
}

Esperf::Esperf(Options *options) : options(options) {}