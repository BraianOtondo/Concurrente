#include "fairlock.h"
#include <iostream>
#include <thread>

void FairLock::lock() {
    std::unique_lock<std::mutex> l(mtx);
    unsigned long mi_turno = proximo_turno++;
    std::cout << "Turno: " << mi_turno << " - Thread entrante: " << std::this_thread::get_id() << std::endl;

    // El hilo se bloquea hasta que sea su turno exacto
    while (mi_turno != turno_actual) {
        cv.wait(l);
    }
}

void FairLock::unlock() {
    std::lock_guard<std::mutex> l(mtx);
    std::cout << "Turno: " << turno_actual << " - Thread saliente: " << std::this_thread::get_id() << std::endl;
    turno_actual++;
    // Despertamos a todos para que revisen si es su turno
    cv.notify_all();
}
