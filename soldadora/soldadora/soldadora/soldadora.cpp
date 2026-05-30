#include<chrono>
#include<iostream>
#include<thread>
#include<mutex>
std::mutex mtx_sol;
std::mutex mtx_ator;
void tareaA() {
	std::cout << "A:Prepara soldadora\n" << std::endl;
	mtx_sol.lock();
	mtx_ator.lock();
	std::cout << "A:Bloquea atornillador\n"<<std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	
	mtx_sol.unlock();
	mtx_ator.unlock();
	std::cout << "A:Pieza soldada\n" << std::endl;
}
void tareaB() {
	std::cout << "B:Prepara atornilladora\n" << std::endl;
	mtx_ator.lock();
	mtx_sol.lock();
	std::cout << "B:Bloquea soldadora\n"<<std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	
	
	mtx_sol.unlock();
	mtx_ator.unlock();
	std::cout << "B:Pieza atornillada\n" << std::endl;
}
//using namespace std;
int main() {
	std::thread t1(tareaA);
	std::thread t2(tareaB);
	t1.join();
	t2.join();

	std::cout << "FINALIZA" << std::endl;

}