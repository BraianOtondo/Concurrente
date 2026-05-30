#include <iostream>
#include <thread>
#include <mutex>

std::mutex mutex_A;
std::mutex mutex_B;

void hilo_uno() {
    bool logrado = false;
    while (!logrado) {
        if (mutex_A.try_lock()) {
            std::cout << ">>> Hilo 1 toma mutex_A <<<\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            if (mutex_B.try_lock()) {
                std::cout << ">>> Hilo 1 obtuvo ambos locks <<<\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(1500));
                logrado = true;
                mutex_B.unlock();
                mutex_A.unlock();
            }
            else {
                std::cout << ">>> Hilo 1 cede mutex A porque B esta tomado<<<\n";
                mutex_A.unlock();
            }
        }
    }
}

void hilo_dos() {
    bool logrado = false;
    while (!logrado) {
        if (mutex_B.try_lock()) {
            std::cout << ">>> Hilo 2 toma mutex_B <<<\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            if (mutex_A.try_lock()) {
                std::cout << ">>> Hilo 2 obtuvo ambos locks <<<\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                logrado = true;
                mutex_A.unlock();
                mutex_B.unlock();
            }
            else {
                std::cout << ">>> Hilo 2 cede mutex B porque A esta tomado<<<\n";
                mutex_B.unlock();
            }
        }
    }
}

int main() {
    std::thread t1(hilo_uno);
    std::thread t2(hilo_dos);

    t1.join();
    t2.join();

    return 0;
}