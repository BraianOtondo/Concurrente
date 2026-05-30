#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <string>
#include <vector>

// 1. ESTRUCTURA DEL JOB
struct Job {
	int id;
	int prioridad; // 1 = Premium, 0 = Free
};

// 2. ESTRUCTURA DEL SEMÁFORO (Hecho a mano, visto en clase)
struct Semaforo {
	int contador;
	std::mutex mtx;
	std::condition_variable cv;
};

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



// 8. CONFIGURACIÓN DEL MAIN
int main() {


	return 0;
}