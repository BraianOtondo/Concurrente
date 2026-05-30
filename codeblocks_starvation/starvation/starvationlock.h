#ifndef STARVATIONLOCK_H_INCLUDED
#define STARVATIONLOCK_H_INCLUDED
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <map>

struct StarvationTask {
    std::thread::id hilo;
    int prioridad;
    bool encolado = false;
};

bool compararStarvationTask(const StarvationTask& a, const StarvationTask& b);

struct StarvationLock {
    std::mutex mtx;
    std::condition_variable cv;

    std::priority_queue<StarvationTask, std::vector<StarvationTask>, bool (*)(const StarvationTask&, const StarvationTask&)> colaHilos;

    std::map<std::thread::id, StarvationTask> estadoHilos;

    bool lockTomado = false;

    StarvationLock();
    void lock(int prioridad);
    void unlock();
};



#endif // STARVATIONLOCK_H_INCLUDED
