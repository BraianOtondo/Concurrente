#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
std::mutex R1; // impresora
std::mutex R2; // scanner
std::mutex R3; // disco
void procesoA() {
	R1.lock();
	std::cout << "A: obtuvo R1 IMPRESORA\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	std::cout << "A: intenta R2\n";
	R2.lock();
	std::cout << "A: obtuvo R2\n";
	R2.unlock();
	R1.unlock();
}
void procesoB() {
	R2.lock();
	std::cout << "B: obtuvo R2 SCANNER \n";
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	std::cout << "B: intenta R3\n";
	R3.lock();
	std::cout << "B: obtuvo R3 DISCO\n";
	R3.unlock();
	R2.unlock();
}

// ARREGLADO
void procesoC() {
	R1.lock();
	std::cout << "C: obtuvo R1\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	std::cout << "C: intenta R3\n";
	R3.lock();
	std::cout << "C: obtuvo R3\n";

	R3.unlock();
	R1.unlock();
}

int main() {
	std::thread t1(procesoA);
	std::thread t2(procesoB);
	std::thread t3(procesoC);
	t1.join();
	t2.join();
	t3.join();
	std::cout << "Fin\n";
	return 0;
}