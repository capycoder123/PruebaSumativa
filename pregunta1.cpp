#include <iostream>
using namespace std;

struct Pieza {
    int fila;
    int columna;
    int valor;
    Pieza* next;
};

class slidingPuzzle {
public:
    int filas;
    int columnas;
    Pieza** cabecerasFilas;

    slidingPuzzle(int f, int c) : filas(f), columnas(c) {
        cabecerasFilas = new Pieza*[filas];
        for (int r = 0; r < filas; r++) cabecerasFilas[r] = nullptr;
    }

    ~slidingPuzzle() {
        for (int r = 0; r < filas; r++) {
            Pieza* cur = cabecerasFilas[r];
            while (cur) {
                Pieza* nxt = cur->next;
                delete cur;
                cur = nxt;
            }
            cabecerasFilas[r] = nullptr;
        }
        delete[] cabecerasFilas;
    }

    bool cargarFila(int r, int valores[]) {
        if (r < 0 || r >= filas) return false;

        Pieza* cur = cabecerasFilas[r];
        while (cur) {
            Pieza* nxt = cur->next;
            delete cur;
            cur = nxt;
        }
        cabecerasFilas[r] = nullptr;

        Pieza* head = nullptr;
        Pieza* tail = nullptr;

        for (int c = 0; c < columnas; c++) {
            Pieza* n = new Pieza();
            n->fila = r;
            n->columna = c;
            n->valor = valores[c];
            n->next = nullptr;

            if (!head) head = tail = n;
            else { tail->next = n; tail = n; }
        }

        cabecerasFilas[r] = head;
        return true;
    }

    bool moverPieza(int pieza) {
        if (pieza <= 0) return false;
        if (pieza > filas * columnas - 1) return false;

        Pieza* nodoPieza = nullptr;
        Pieza* nodoVacio = nullptr;

        for (int r = 0; r < filas; ++r) {
            for (Pieza* cur = cabecerasFilas[r]; cur != nullptr; cur = cur->next) {
                if (cur->valor == pieza) nodoPieza = cur;
                else if (cur->valor == 0) nodoVacio = cur;
                if (nodoPieza && nodoVacio) break;
            }
            if (nodoPieza && nodoVacio) break;
        }

        if (!nodoPieza || !nodoVacio) return false;

        int df = nodoPieza->fila - nodoVacio->fila;
        if (df < 0) df = -df;
        int dc = nodoPieza->columna - nodoVacio->columna;
        if (dc < 0) dc = -dc;

        if (df + dc != 1) return false;

        int tmp = nodoPieza->valor;
        nodoPieza->valor = 0;
        nodoVacio->valor = tmp;

        return true;
    }

    void imprimir() const {
        for (int r = 0; r < filas; r++) {
            for (Pieza* cur = cabecerasFilas[r]; cur != nullptr; cur = cur->next) {
                cout << cur->valor << " ";
            }
            cout << "\n";
        }
    }
};