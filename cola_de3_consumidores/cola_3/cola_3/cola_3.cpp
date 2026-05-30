#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono> 

struct Semaforo {
    int contador;
    std::mutex mtx;
    std::condition_variable cv;
};

std::mutex mtx_buffer;
std::mutex mtx_cout; 

const int tam = 100000; 
const int limit = 5;    
std::queue<int> buffer;  

Semaforo hay_espacio;
Semaforo hay_dato;

void init(Semaforo& s, int valor) {
    s.contador = valor;
}
void wait(Semaforo& s) {
    std::unique_lock<std::mutex> lock(s.mtx);
    while (s.contador == 0) {
        s.cv.wait(lock);
    }
    s.contador--;
}

void signal(Semaforo& s) {
    std::unique_lock<std::mutex> lock(s.mtx);
    s.contador++;
    s.cv.notify_one();
}

void productor() {
    int producidos = 0;
    for (int i = 0; i < tam; i++) {
        wait(hay_espacio);

        mtx_buffer.lock();
        buffer.push(i);
        producidos++;
        mtx_buffer.unlock();

        signal(hay_dato);
    }


}
void consumidor(int id, int elementos_a_procesar) {
    for (int i = 0; i < elementos_a_procesar; i++) {
        wait(hay_dato);

        mtx_buffer.lock();
        int val = buffer.front();
        buffer.pop();
        mtx_buffer.unlock();

        signal(hay_espacio);
    }

    
    mtx_cout.lock();
    std::cout << "CONSUMIDOR NRO: " << id << " finalizo procesando " << elementos_a_procesar << " elementos." << std::endl;
    mtx_cout.unlock();
}

int main() {
    init(hay_espacio, limit);
    init(hay_dato, 0);

    auto inicio = std::chrono::high_resolution_clock::now();

    
    std::thread t1(productor);

  //DIVIDIR ENTRE 100000 
    std::thread t2(consumidor, 1, 33333);
    std::thread t3(consumidor, 2, 33333);
    std::thread t4(consumidor, 3, 33334);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    
    auto fin = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> tiempo_total = fin - inicio;

    
    std::cout << "Tiempo Maximo: " << tiempo_total.count() << " ms" << std::endl;

    if (buffer.empty()) {
        std::cout << "Buffer vacio correctamente." << std::endl;
    }
    else {
        std::cout << "ERROR: Quedaron " << buffer.size() << " elementos en el buffer." << std::endl;
    }
    

    return 0;
}