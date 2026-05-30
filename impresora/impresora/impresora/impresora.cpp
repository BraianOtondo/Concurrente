#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
std::mutex impresora;
std::mutex scanner;

void procesoA() {
	impresora.lock();
	std::cout << "Proceso A Obtuvo impresora\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	std::cout << "A: intentando obtener scanner...\n";
	scanner.lock();
	std::cout << "A: obtuvo scanner\n";
	// trabajo
	scanner.unlock();
	impresora.unlock();
}
void procesoB() {
	impresora.lock();
	std::cout << "B: obtuvo scanner\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	std::cout << "B: intentando obtener impresora...\n";
	
	scanner.lock();
	std::cout << "B: obtuvo impresora\n";
	// trabajo
	scanner.unlock();
	impresora.unlock();
	
}
int main() {

	std::thread t1(procesoA);
	std::thread t2(procesoB);
	t1.join();
	t2.join();
	std::cout << "Finaliza? SI\n";

}