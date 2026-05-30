#include <iostream>
#include<thread>
#include<queue>
#include <mutex>
#include<condition_variable>
struct Semaforo {
	int contador;
	std::mutex mtx;
	std::condition_variable cv;
};
std::mutex mtx_buffer;
const int tam= 10;
const int limit = 5;
std::queue<int> buffer; //recurso compartido
 Semaforo hay_espacio;
 Semaforo hay_dato;

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
	std::cout << "Producidos: " << producidos <<std::endl;
}

void consumidor(){
	for(int i=0;i<tam;i++){
		wait(hay_dato);
		mtx_buffer.lock();
			int val = buffer.front();
			buffer.pop();
		mtx_buffer.unlock();
		signal(hay_espacio);
	}
}

int main(){
	init(hay_espacio, limit);
	init(hay_dato, 0);	
	std::thread t1(productor);
	std::thread t2(consumidor);
	//std::thread t3(consumidor);
	//std::thread t4(consumidor);

	t1.join();
	t2.join();
	//t3.join();	
	//t4.join();
	std::cout<<"Estado final de buffer:"<<std::endl;

	if (buffer.empty()) {
		std::cout << "Buffer vacio" << std::endl;
	}
	else {
		while (!buffer.empty()) {
			std::cout << "Quedaron" << buffer.size() << " elementos en el buffer" << std::endl;
			std::cout << "Elemento: " << buffer.front() << std::endl;
			buffer.pop();
		}
		std::cout << std::endl;
	}
	return 0;
}