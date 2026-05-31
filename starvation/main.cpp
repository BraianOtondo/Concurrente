#include "pcv4.h"
#include "semaforo.h"
#include <iostream>
#include <thread>
#include <queue>
#include <vector>

std::queue<int> buffer; // recurso compartido
Semaforo hay_espacio;
Semaforo hay_datos;

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
