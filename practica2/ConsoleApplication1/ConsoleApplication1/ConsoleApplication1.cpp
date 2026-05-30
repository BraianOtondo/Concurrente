
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
using namespace std;

void tarea(std::string nombreTarea, int ms) {
    for (int i = 1;i <= 5;i++) {
        cout << nombreTarea << " - Iteracion " << i << endl;
        this_thread::sleep_for(std::chrono::milliseconds(ms));

    }

}

int main(){
    
	//VERSION SECUENCIAL
    auto inicio_seq = std::chrono::high_resolution_clock::now();



    tarea("Generar Reporte", 500);
    tarea("Procesar Pedidos", 650);
    tarea("Enviar notificaciones", 800);

    auto fin_seq = std::chrono::high_resolution_clock::now();

    auto duracion_seq = std::chrono::duration_cast<std::chrono::milliseconds>(fin_seq - inicio_seq);

    std::cout << "\nTiempo secuencial: "
        << duracion_seq.count()
        << " ms\n\n";

	std::cout << "--------------------------" << std::endl;
	// VERSION CON HILOS
    auto inicio_conc = std::chrono::high_resolution_clock::now();


    std::thread t1(tarea,"Generar Reporte", 500);
    std::thread t2(tarea,"Procesar Pedidos", 650);
    std::thread t3(tarea,"Enviar notificaciones", 800);


    t1.join();
	t2.join();  
	t3.join();
    auto fin_conc = std::chrono::high_resolution_clock::now();

    auto duracion_conc =
        duration_cast<std::chrono::milliseconds>(fin_conc - inicio_conc);

    std::cout << "\nTiempo concurrente: "
        << duracion_conc.count()
        << " ms\n";
	return 0;
}

