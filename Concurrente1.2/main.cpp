#include <iostream>

using namespace std;
char validarChar(){
char op;
bool valido = false;
while (!valido) {
cout << "Ingrese el signo (+, -, *, /): ";
cin >> op;
if (op == '+' || op == '-' || op == '*' || op == '/') {
valido = true; // Rompe el bucle
} else {
cout << "Error: '" << op << "' no es un operador valido. " << endl;
cin.ignore(1000, '\n');
}
}
return op;
}
int suma(int n1,int n2){
return n1+n2;
}
int resta (int n1,int n2){
return n1-n2;
}
int multiplicacion(int n1, int n2){
int resultado = 0;
for (int i = 0; i < n2; i++) {
resultado += n1;
}
return resultado;
}
void divisionConRestas(int dividendo, int divisor, int *p_resultado, int *p_resto) {
int contador = 0;
int auxiliar = dividendo;
while (auxiliar >= divisor) {
auxiliar = auxiliar - divisor;
contador++;
}
*p_resultado = contador;
*p_resto = auxiliar;
}
int validarDenominador(){
        int n2;
        do{
         cout << "Ingrese un denominador distinto que 0: ";
            std::cin >> n2;
        }while(n2==0);



return n2;
}
void menu(char op,int n1,int n2){

int resultado,resto;
switch (op) {
case '+':
cout << "Resultado suma: " << suma(n1,n2)<< endl;
break;
case '-':
cout << "Resultado resta: " << resta(n1,n2)<< endl;
break;
case '*':
cout << "Resultado multiplicacion: " << multiplicacion(n1,n2)<< endl;
break;
case '/':
if (n2 == 0) {
cout << "Error: No se puede dividir por cero." << endl;

n2=validarDenominador();

}

divisionConRestas(n1, n2, &resultado, &resto);
cout << "Cociente: " << resultado << endl;
cout << "Resto: " << resto << endl;

break;
default:
cout << "Operador no reconocido." << endl;
break;
}

}
int n1(){
int n1;
cout << "Ingrese el primer numero entero: ";
while (!(cin >> n1)) {
cout << "Error. Ingrese un entero: ";
cin.clear();
cin.ignore(1000, '\n');
}

return n1;
}
int n2(){
int n2;
cout << "Ingrese el segundo numero entero: ";
while (!(cin >> n2)) {
cout << "Error. Ingrese un entero: ";
cin.clear();
cin.ignore(1000, '\n');
}
return n2;
}


int main(){

int numero1, numero2;
char operacion;

numero1=n1();
numero2=n2();
operacion= validarChar();
menu(operacion,numero1,numero2);
return 0;
}
