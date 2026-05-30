#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#define _GLIBCXX_HAS_GTHREADS
void tarea(const string &nombre) {
    for (int i = 1; i <= 5; i++) {
        scout << nombre << " - Iteracion " << i << endl;
        this_thread::sleep_for(chrono::milliseconds(500));
    }
}

int main() {

    using namespace chrono;

    // -----------------------
    // VERSION SECUENCIAL
    // -----------------------
    auto inicio_seq = high_resolution_clock::now();

    tarea("Tarea A");
    tarea("Tarea B");
    tarea("Tarea C");

    auto fin_seq = high_resolution_clock::now();

    auto duracion_seq =
        duration_cast<milliseconds>(fin_seq - inicio_seq);

    cout << "\nTiempo secuencial: "
              << duracion_seq.count()
              << " ms\n\n";

    // -----------------------
    // VERSION CONCURRENTE
    // -----------------------
    auto inicio_conc = high_resolution_clock::now();

    thread t1(tarea, "Tarea A");
    thread t2(tarea, "Tarea B");
    thread t3(tarea, "Tarea C");

    t1.join();
    t2.join();
    t3.join();

    auto fin_conc = high_resolution_clock::now();

    auto duracion_conc =
        duration_cast<milliseconds>(fin_conc - inicio_conc);

    cout << "\nTiempo concurrente: "
              << duracion_conc.count()
              << " ms\n";

    return 0;
}
