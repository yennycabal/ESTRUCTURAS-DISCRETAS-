//operaciones de suma,resta,multiplicación y división  y conversión un numero a cualquier base 
//tanto con numeros enteros y decimales 
#include <iostream>
#include <string>
using namespace std;

// mayusculas y minusculas (a=10 ... f=15)
string digitos = "0123456789ABCDEFabcdef";
string simbolos = "0123456789ABCDEF";

const int DECIMALES = 2;              // decimales del resultado en las operaciones
const int DECIMALES_CONVERSION = 6;   // decimales del resultado al convertir

int valorDeSimbolo(string simbolo) {
    int posicion = digitos.find(simbolo);
    if (posicion == -1) {
        return -1;
    }
    if (posicion > 15) {
        posicion = posicion - 6;   
    }
    return posicion;
}

// cambia las comas por puntos
string cambiarComaPorPunto(string texto) {
    string nuevo = "";
    for (int i = 0; i < (int)texto.length(); i++) {
        string simbolo = texto.substr(i, 1);
        if (simbolo == ",") {
            nuevo = nuevo + ".";
        } else {
            nuevo = nuevo + simbolo;
        }
    }
    return nuevo;
}

// revisa que el numero solo tenga digitos validos para la base
bool numeroValido(string texto, int base) {
    int puntos = 0;
    int cantidadDigitos = 0;

    for (int i = 0; i < (int)texto.length(); i++) {
        string simbolo = texto.substr(i, 1);

        if (simbolo == ".") {
            puntos++;
        } else {
            int valor = valorDeSimbolo(simbolo);
            if (valor == -1 || valor >= base) {
                return false;   
            }
            cantidadDigitos++;
        }
    }

    if (puntos > 1 || cantidadDigitos == 0) {
        return false;
    }
    return true;
}

// --- Paso 1: convertir un numero escrito en base "b" a base 10 ---
double convertirADecimal(string numeroTexto, int base) {
    string parteEntera = "";
    string parteDecimal = "";

    // separamos la parte entera y la parte decimal (si tiene punto)
    int posicionPunto = numeroTexto.find(".");
    if (posicionPunto != (int)string::npos) {
        parteEntera = numeroTexto.substr(0, posicionPunto);
        parteDecimal = numeroTexto.substr(posicionPunto + 1);
    } else {
        parteEntera = numeroTexto;
    }

    // convertimos la parte entera
    double valorEntero = 0;
    for (int i = 0; i < (int)parteEntera.length(); i++) {
        int digito = valorDeSimbolo(parteEntera.substr(i, 1));
        valorEntero = valorEntero * base + digito;
    }

    // convertimos la parte decimal
    double valorDecimal = 0;
    double posicion = 1;
    for (int i = 0; i < (int)parteDecimal.length(); i++) {
        int digito = valorDeSimbolo(parteDecimal.substr(i, 1));
        posicion = posicion * base;
        valorDecimal = valorDecimal + digito / posicion;
    }

    return valorEntero + valorDecimal;
}

// --- Paso 2: convertir un numero en base 10 a base "b", redondeando a "decimales" cifras ---
string convertirABase(double numero, int base, int decimales) {
    string signo = "";
    if (numero < 0) {
        signo = "-";
        numero = -numero;
    }

    // factor = base elevada a "decimales" (en base 10 con 2 decimales: 100)
    long long factor = 1;
    for (int i = 0; i < decimales; i++) {
        factor = factor * base;
    }

    // redondeamos: multiplicamos, sumamos 0.5 y cortamos
    long long total = (long long)(numero * factor + 0.5);

    if (total == 0) {
        signo = "";   // evita que salga "-0"
    }

    long long parteEntera = total / factor;
    long long resto = total % factor;

    // convertir la parte entera
    string textoEntero = "";
    if (parteEntera == 0) {
        textoEntero = "0";
    } else {
        while (parteEntera > 0) {
            int digito = parteEntera % base;
            textoEntero = simbolos.substr(digito, 1) + textoEntero;
            parteEntera = parteEntera / base;
        }
    }

    // convertir la parte decimal 
    string textoDecimal = "";
    for (int i = 0; i < decimales; i++) {
        int digito = resto % base;
        textoDecimal = simbolos.substr(digito, 1) + textoDecimal;
        resto = resto / base;
    }

    // quitamos los ceros sobrantes del final (5,50 -> 5,5)
    while (textoDecimal.length() > 0 &&
           textoDecimal.substr(textoDecimal.length() - 1, 1) == "0") {
        textoDecimal = textoDecimal.substr(0, textoDecimal.length() - 1);
    }

    if (textoDecimal != "") {
        return signo + textoEntero + "," + textoDecimal;
    } else {
        return signo + textoEntero;
    }
}

// --- Paso 3: programa principal ---
int main() {
    int opcion, base;
    string numero1Texto, numero2Texto;

    // 1) primero el menu
    cout << "Que quieres hacer?" << endl;
    cout << "1) Sumar" << endl;
    cout << "2) Restar" << endl;
    cout << "3) Multiplicar" << endl;
    cout << "4) Dividir" << endl;
    cout << "5) Convertir un numero a otra base" << endl;
    cout << "Elige una opcion: ";
    cin >> opcion;

    if (opcion < 1 || opcion > 5) {
        cout << "Opcion invalida" << endl;
        return 0;
    }

    // ---------- OPCION 5: CONVERTIR A OTRA BASE ----------
    if (opcion == 5) {
        cout << "\nNumero que quieres convertir: ";
        cin >> numero1Texto;
        cout << "A que base lo quieres convertir? (2-16): ";
        cin >> base;

        if (base < 2 || base > 16) {
            cout << "Base invalida, debe estar entre 2 y 16" << endl;
            return 0;
        }

        numero1Texto = cambiarComaPorPunto(numero1Texto);

        // aceptamos numeros negativos
        string signo = "";
        if (numero1Texto.substr(0, 1) == "-") {
            signo = "-";
            numero1Texto = numero1Texto.substr(1);
        }

        if (!numeroValido(numero1Texto, 10)) {
            cout << "El numero no es valido" << endl;
            return 0;
        }

        double numero = convertirADecimal(numero1Texto, 10);
        if (signo == "-") {
            numero = -numero;
        }

        cout << "\nResultado en base " << base << ": "
             << convertirABase(numero, base, DECIMALES_CONVERSION) << endl;
        return 0;
    }

    // ---------- OPCIONES 1 A 4: OPERACIONES ----------
    cout << "\nIngresa el primer numero: ";
    cin >> numero1Texto;
    cout << "Ingresa el segundo numero: ";
    cin >> numero2Texto;
    cout << "En que base estan los numeros? (2-16): ";
    cin >> base;

    if (base < 2 || base > 16) {
        cout << "Base invalida, debe estar entre 2 y 16" << endl;
        return 0;
    }

    // aceptamos coma o punto como separador decimal
    numero1Texto = cambiarComaPorPunto(numero1Texto);
    numero2Texto = cambiarComaPorPunto(numero2Texto);

    // revisamos que los digitos existan en esa base
    if (!numeroValido(numero1Texto, base)) {
        cout << "El primer numero no es valido en base " << base << endl;
        return 0;
    }
    if (!numeroValido(numero2Texto, base)) {
        cout << "El segundo numero no es valido en base " << base << endl;
        return 0;
    }

    // pasamos ambos numeros a base 10 para poder operar
    double numero1 = convertirADecimal(numero1Texto, base);
    double numero2 = convertirADecimal(numero2Texto, base);

    double resultado = 0;

    if (opcion == 1) {
        resultado = numero1 + numero2;
    } else if (opcion == 2) {
        resultado = numero1 - numero2;
    } else if (opcion == 3) {
        resultado = numero1 * numero2;
    } else if (opcion == 4) {
        if (numero2 == 0) {
            cout << "No se puede dividir entre 0" << endl;
            return 0;
        }
        resultado = (long long)(numero1 / numero2);  
    }

    cout << "\nResultado en base " << base << ": "
         << convertirABase(resultado, base, DECIMALES) << endl;

    return 0;
}