#include <iostream>
#include <fstream> // Biblioteca obligatoria para archivos

int main() {
    // 1. Abrimos el archivo en modo "Append" (agregar al final)
    std::ofstream archivo("prueba.log", std::ios::app);

    // 2. Verificamos que el archivo se haya abierto correctamente
    if (archivo.is_open()) {
        // Escribimos usando << exactamente igual que con std::cout
        archivo << "Hola Mundo desde C++" << std::endl;

        // 3. Cerramos el archivo para asegurar que los datos se guarden en el disco
        archivo.close();
        std::cout << "Log guardado con exito.\n";
    }
    else {
        std::cout << "Error al abrir el archivo.\n";
    }

    return 0;
}