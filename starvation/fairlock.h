#ifndef FAIRLOCK_H
#define FAIRLOCK_H

#include <mutex>
#include <condition_variable>

struct FairLock {
    std::mutex mtx;
    std::condition_variable cv;
    unsigned long proximo_turno = 0;
    unsigned long turno_actual = 0;

    void lock();
    void unlock();
};

#endif
