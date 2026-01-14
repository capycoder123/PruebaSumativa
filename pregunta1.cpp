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

    bool moverPieza(int pieza) {
        if (pieza <= 0) return false;

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
};
