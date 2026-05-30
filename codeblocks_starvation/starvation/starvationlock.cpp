#include "starvationlock.h"

bool compararStarvationTask(const StarvationTask& a, const StarvationTask& b) {
    return a.prioridad < b.prioridad;
}

StarvationLock::StarvationLock() : colaHilos(compararStarvationTask) {}

void StarvationLock::lock(int prioridad) {
    std::unique_lock<std::mutex> l(mtx);
    std::thread::id hiloId = std::this_thread::get_id();

    //lo busco para no volver a encolarlo, o lo creo si no estaba encolado
    std::map<std::thread::id, StarvationTask>::iterator it = estadoHilos.find(hiloId);
    if (it == estadoHilos.end()) {
        StarvationTask tarea;
        tarea.hilo = hiloId;
        tarea.prioridad = prioridad;
        tarea.encolado = false;
        estadoHilos[hiloId] = tarea;
        it = estadoHilos.find(hiloId);
    }

    it->second.prioridad = prioridad;

    StarvationTask t;
    t.hilo = hiloId;
    t.prioridad = it->second.prioridad;

    if (!it->second.encolado) {
        colaHilos.push(t);
        it->second.encolado = true;
    }

    while (lockTomado || colaHilos.empty() || colaHilos.top().hilo != t.hilo) {
        cv.wait(l);
    }

    lockTomado = true;
    colaHilos.pop();

    it = estadoHilos.find(hiloId);
    if (it != estadoHilos.end()) {
        it->second.encolado = false;
    }
}

void StarvationLock::unlock() {
    std::lock_guard<std::mutex> l(mtx);
    lockTomado = false;
    cv.notify_all();
}
