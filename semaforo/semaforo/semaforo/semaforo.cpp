#include<iostream>
#include<thread>
#include<chrono>
#include<mutex>
#include<string>
std::mutex mtx_semaforo;
void Cruze(std::string carro, std::string zona_origen) {
	
	
	mtx_semaforo.lock();
	std::cout << "El carro " << carro << " que viene del " << zona_origen << " esta esperando para cruzar\n" << std::endl;
	std::cout << "El carro " << carro << " que viene del" << zona_origen << " esta cruzando\n" << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	mtx_semaforo.unlock();
	std::cout << "Cruzo el carro " << carro<<"\n" << std::endl;

}

int main() {
	std::thread t1(Cruze, "1", "Norte");
	std::thread t2(Cruze, "2", "Sur");
	std::thread t3(Cruze, "3", "Este");
	std::thread t4(Cruze, "4", "Oeste");
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	return 0;
}