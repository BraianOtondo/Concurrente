#include "pcv4.h"
#include "semaforo.h"
#include <iostream>
#include <queue>
#include <mutex>
#include <thread>
#include <chrono>
//#include "starvationlock.h"
//#include "fairlock.h"

// Recursos compartidos
extern std::queue<int> buffer;
extern Semaforo hay_espacio;
extern Semaforo hay_datos;
std::mutex mtx_buffer;
//StarvationLock mtx_buffer;
//FairLock mtx_buffer;

const int tam = 5;
int val = 0;

void productor() {
    int producidos = 0;
    for (int i = 0; i < tam; i++) {
        // 1. Espera a que haya un hueco libre
        wait(hay_espacio);

        // 2. Mutex de la cola
        //mtx_buffer.lock(100);
        mtx_buffer.lock();
        std::cout<<"Produciendo\n";
        buffer.push(val++);
        mtx_buffer.unlock();

        // 3. Avisa que hay un nuevo dato disponible
        signal(hay_datos);
    }
}

void consumidor() {
    for (int i = 0; i < tam*10; i++) {
        // 1. Espera a que haya al menos un dato
        wait(hay_datos);

        // 2. Mutex de la cola
        //mtx_buffer.lock(0);
        mtx_buffer.lock();
        int val = buffer.front();
        std::cout << ">>> [Consumidor] Procesando elemento: " << val << std::endl;
        buffer.pop();
        mtx_buffer.unlock();

        // 3. Avisa que libera un espacio
        signal(hay_espacio);
    }
}
