#include <iostream>
#include<thread>
#include<queue>
#include <mutex>
#include<condition_variable>
#include <chrono>
#include <string>
//int contador = 0;
struct Semaforo {
	int contador = 0;
	std::mutex mtx;
	std::condition_variable cv;
};
std::mutex mtx_array;
std::mutex mtx_buffer;
const int tam= 100000;
const int limit = 5;
std::queue<int> buffer; //recurso compartido
 Semaforo hay_espacio;
 Semaforo hay_dato;
 int elementos_restantes = tam;
 std::mutex mtx_cout;
 int consumidos_por_hilo[3] = { 0, 0, 0 };

void init(Semaforo& s, int valor) {
	s.contador = valor;	

}
void wait(Semaforo& s) {
	std::unique_lock<std::mutex> lock(s.mtx);
	while(s.contador==0) {
		s.cv.wait(lock);
	}
	s.contador--;
}
void signal(Semaforo& s) {
	std::unique_lock<std::mutex> lock(s.mtx);

	s.contador++;
	s.cv.notify_one();
	
}
void productor(){
	int producidos=0;
	
	for(int i=0;i<tam;i++){
		wait(hay_espacio);

			mtx_buffer.lock();
			buffer.push(i);

			producidos++;
			mtx_buffer.unlock();	
			signal(hay_dato);
		
	}
	std::string mensaje = "Producidos: " + std::to_string(producidos) + "\n";

	mtx_cout.lock();
	std::cout << mensaje; // Se envía todo en un solo bloque atómico
	mtx_cout.unlock();

	signal(hay_dato);
	signal(hay_dato);
	signal(hay_dato);
}
void consumidor(int id) {
	int indice = id - 1;

	while (true) {
		mtx_buffer.lock();
		if (elementos_restantes <= 0) {
			mtx_buffer.unlock();
			break;
		}
		mtx_buffer.unlock();

		wait(hay_dato);

		mtx_buffer.lock();
		if (elementos_restantes > 0 && !buffer.empty()) {
			int val = buffer.front();
			buffer.pop();

			elementos_restantes--;
			consumidos_por_hilo[indice]++;

			mtx_buffer.unlock();
			signal(hay_espacio);
		}
		else {
			mtx_buffer.unlock();
			signal(hay_dato); // Propagación de señal para evitar que el siguiente quede atrapado
			break;
		}
	}

	// BLINDAJE DE PANTALLA: Construimos el mensaje individual en memoria local
	std::string mensaje = "[Consumidor " + std::to_string(id) + "] Termino su ejecucion. Consumio: " + std::to_string(consumidos_por_hilo[indice]) + "\n";

	mtx_cout.lock();
	std::cout << mensaje; // Imprime limpiamente sin fragmentarse
	mtx_cout.unlock();
}
int main(){
	init(hay_espacio, limit);
	init(hay_dato, 0);

	auto inicio = std::chrono::high_resolution_clock::now();

	std::thread t1(productor);
	std::thread t2(consumidor, 1);
	std::thread t3(consumidor, 2);
	std::thread t4(consumidor, 3);

	t1.join();
	t2.join();
	t3.join();
	t4.join();

	auto fin = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> tiempo_total = fin - inicio;

	// El hilo principal puede imprimir libremente porque ya se hizo join de todos los hilos
	std::cout << "\n=========================================" << std::endl;
	std::cout << "Tiempo total de ejecucion: " << tiempo_total.count() << " ms" << std::endl;
	std::cout << "=========================================" << std::endl;

	std::cout << "Resumen del arreglo global:" << std::endl;
	std::cout << "-> Hilo 1 consumio: " << consumidos_por_hilo[0] << std::endl;
	std::cout << "-> Hilo 2 consumio: " << consumidos_por_hilo[1] << std::endl;
	std::cout << "-> Hilo 3 consumio: " << consumidos_por_hilo[2] << std::endl;

	int suma_total = consumidos_por_hilo[0] + consumidos_por_hilo[1] + consumidos_por_hilo[2];
	std::cout << "Suma total de tareas en el arreglo: " << suma_total << std::endl;
	std::cout << "=========================================" << std::endl;

	std::cout << "Estado final de buffer: ";
	if (buffer.empty()) {
		std::cout << "Buffer vacio" << std::endl;
	}
	else {
		std::cout << "Quedaron elementos. Tamano: " << buffer.size() << std::endl;
	}

	return 0;
}