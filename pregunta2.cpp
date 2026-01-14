#include <iostream>
#include <string>
#include <cctype>

using namespace std;

struct ResultadoEval {
    bool ok;
    long long valor;
    string error;
};

struct NodoChar {
    char dato;
    NodoChar* next;
    NodoChar(char d, NodoChar* n = nullptr) : dato(d), next(n) {}
};

class PilaChar {
public:
    PilaChar() : tope(nullptr) {}
    ~PilaChar() { while (!vacia()) pop(); }

    bool vacia() const { return tope == nullptr; }

    void push(char x) { tope = new NodoChar(x, tope); }

    bool pop() {
        if (vacia()) return false;
        NodoChar* t = tope;
        tope = tope->next;
        delete t;
        return true;
    }

    char top() const { return tope->dato; }

private:
    NodoChar* tope;
};

struct NodoLL {
    long long dato;
    NodoLL* next;
    NodoLL(long long d, NodoLL* n = nullptr) : dato(d), next(n) {}
};

class PilaLL {
public:
    PilaLL() : tope(nullptr) {}
    ~PilaLL() { while (!vacia()) pop(); }

    bool vacia() const { return tope == nullptr; }

    void push(long long x) { tope = new NodoLL(x, tope); }

    bool pop() {
        if (vacia()) return false;
        NodoLL* t = tope;
        tope = tope->next;
        delete t;
        return true;
    }

    long long top() const { return tope->dato; }

private:
    NodoLL* tope;
};

static bool esOperador(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

static int precedencia(char op) {
    if (op == '*' || op == '/') return 2;
    if (op == '+' || op == '-') return 1;
    return 0;
}

static bool aplicarOp(long long a, long long b, char op, long long& out, string& error) {
    if (op == '+') { out = a + b; return true; }
    if (op == '-') { out = a - b; return true; }
    if (op == '*') { out = a * b; return true; }
    if (op == '/') {
        if (b == 0) { error = "División por cero."; return false; }
        out = a / b;
        return true;
    }
    error = "Operador inválido.";
    return false;
}

static bool aplicarTope(PilaLL& valores, PilaChar& ops, string& error) {
    if (ops.vacia()) { error = "Faltan operadores."; return false; }

    char op = ops.top();
    ops.pop();

    if (valores.vacia()) { error = "Faltan operandos para un operador."; return false; }
    long long b = valores.top();
    valores.pop();

    if (valores.vacia()) { error = "Faltan operandos para un operador."; return false; }
    long long a = valores.top();
    valores.pop();

    long long res = 0;
    if (!aplicarOp(a, b, op, res, error)) return false;

    valores.push(res);
    return true;
}

ResultadoEval resolverEcuacionEnterosPositivos(const string& ecuacion) {
    PilaChar ops;
    PilaLL valores;

    bool esperaOperando = true;

    for (size_t i = 0; i < ecuacion.size(); ) {
        char c = ecuacion[i];

        if (isspace(static_cast<unsigned char>(c))) { ++i; continue; }

        if (isdigit(static_cast<unsigned char>(c))) {
            if (!esperaOperando) return {false, 0, "Falta un operador entre números."};

            long long num = 0;
            while (i < ecuacion.size() && isdigit(static_cast<unsigned char>(ecuacion[i]))) {
                int d = ecuacion[i] - '0';
                num = num * 10 + d;
                ++i;
            }

            if (num <= 0) return {false, 0, "Solo se permiten enteros positivos (> 0)."};

            valores.push(num);
            esperaOperando = false;
            continue;
        }

        if (c == '(') {
            if (!esperaOperando) return {false, 0, "Falta un operador antes de '('."};
            ops.push('(');
            ++i;
            continue;
        }

        if (c == ')') {
            if (esperaOperando) return {false, 0, "Paréntesis de cierre sin operando antes."};

            bool encontroApertura = false;
            while (!ops.vacia()) {
                char top = ops.top();
                if (top == '(') {
                    ops.pop();
                    encontroApertura = true;
                    break;
                }
                string err;
                if (!aplicarTope(valores, ops, err)) return {false, 0, err};
            }
            if (!encontroApertura) return {false, 0, "Paréntesis no balanceados: sobra ')'."};

            ++i;
            esperaOperando = false;
            continue;
        }

        if (esOperador(c)) {
            if (esperaOperando) return {false, 0, "Operador en posición inválida (no se permiten signos unarios)."};

            while (!ops.vacia() && ops.top() != '(' && precedencia(ops.top()) >= precedencia(c)) {
                string err;
                if (!aplicarTope(valores, ops, err)) return {false, 0, err};
            }

            ops.push(c);
            ++i;
            esperaOperando = true;
            continue;
        }

        return {false, 0, string("Carácter inválido: '") + c + "'."};
    }

    if (esperaOperando) return {false, 0, "Expresión incompleta: termina en operador o '('."};

    while (!ops.vacia()) {
        if (ops.top() == '(') return {false, 0, "Paréntesis no balanceados: falta ')'."};
        string err;
        if (!aplicarTope(valores, ops, err)) return {false, 0, err};
    }

    if (valores.vacia()) return {false, 0, "No se pudo evaluar la expresión."};

    long long resultadoFinal = valores.top();
    valores.pop();

    if (!valores.vacia()) return {false, 0, "Expresión mal descrita: sobran operandos."};

    return {true, resultadoFinal, ""};
}