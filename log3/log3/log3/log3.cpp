#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>   // Biblioteca para proteger el recurso compartido
#include <string>

// Estructura idéntica a la que te pide el TP
struct Job {
    int id;
    std::string prioridad; // "Premium" o "Free"
};

// MUTEX GLOBAL EXCLUSIVO PARA EL LOG
// Esta es la "llave" que se van a tener que prestar los hilos para escribir en el archivo
std::mutex mtx_log;

void registrar_log_concurrente(Job mi_job, std::string evento) {

    // --- ZONA DE EXCLUSIÓN MUTUA ---
    // El hilo que llegue acá cierra la puerta. Si viene otro hilo, se queda esperando afuera.
    mtx_log.lock();

    std::ofstream archivo("actividad.log", std::ios::app);
    if (archivo.is_open()) {

        // Formato limpio y atómico pedido por el enunciado de tu TP
        archivo << "Job ID: " << mi_job.id
            << " - Prioridad: " << mi_job.prioridad
            << " - Evento: " << evento << std::endl;

        archivo.close();
    }

    // El hilo termina de escribir y abre la puerta para el siguiente
    mtx_log.unlock();
    // --------------------------------
}

// Esta es la función que ejecutarán los hilos en paralelo
void trabajar(int id_hilo, Job tarea) {
    registrar_log_concurrente(tarea, "CREADO");
    registrar_log_concurrente(tarea, "EN_COLA");
}

int main() {
    // Creamos dos tareas que se van a procesar en paralelo
    Job job_A = { 500, "Premium" };
    Job job_B = { 501, "Free" };

    std::cout << "Lanzando hilos concurrentes para escribir en el log..." << std::endl;

    // Lanzamos dos hilos que van a competir ferozmente por usar el archivo
    std::thread t1(trabajar, 1, job_A);
    std::thread t2(trabajar, 2, job_B);

    t1.join();
    t2.join();

    std::cout << "Proceso terminado. Si miras el archivo 'actividad.log', las lineas estan perfectas y no se pisaron." << std::endl;
    return 0;
}