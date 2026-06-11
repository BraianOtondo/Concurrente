#include <iostream>
#include <thread>
#include <queue>
#include <vector>
#include <condition_variable>
#include <chrono>
#include <mutex>
#include <vector>
#include <map>
struct Semaforo {
    int contador;
    std::mutex mtx;
    std::condition_variable cv;
};

std::mutex mtx_buffer;
std::queue<int> buffer; // recurso compartido
Semaforo hay_espacio;
Semaforo hay_datos;
//-------
struct StarvationTask {
    std::thread::id hilo;
    int prioridad;
    bool encolado = false;
};

bool compararStarvationTask(const StarvationTask& a, const StarvationTask& b) {
    return a.prioridad < b.prioridad;
}

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


//---------

struct FairLock {
    std::mutex mtx;
    std::condition_variable cv;
    unsigned long proximo_turno = 0;
    unsigned long turno_actual = 0;

    void lock();
    void unlock();
};

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



void init(Semaforo& s, int n) {
    s.contador = n;
}
void wait(Semaforo& s) {
    std::unique_lock<std::mutex> lock(s.mtx);

    while (s.contador == 0) {
        s.cv.wait(lock);  // bloquea el hilo
    }

    s.contador--;  // consume un permiso
}
void signal(Semaforo& s) {
    std::unique_lock<std::mutex> lock(s.mtx);

    s.contador++;        // libera un permiso
    s.cv.notify_one();   // despierta UN hilo en espera
}

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
        std::cout << "Produciendo\n";
        buffer.push(val++);
        mtx_buffer.unlock();

        // 3. Avisa que hay un nuevo dato disponible
        signal(hay_datos);
    }
}

void consumidor() {
    for (int i = 0; i < tam * 10; i++) {
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



int main() {
    const int NUM_PRODUCTORES = 10;
    init(hay_espacio, 50);
    init(hay_datos, 0);

    std::vector<std::thread> productores;

    for (int i = 0; i < NUM_PRODUCTORES; ++i) {
        productores.emplace_back(productor);
    }

    // Un solo consumidor
    std::thread c(consumidor);

    for (auto& t : productores) t.join();
    c.join();
}