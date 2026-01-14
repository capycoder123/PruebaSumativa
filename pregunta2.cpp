#include <iostream>
#include <string>
#include <cctype>
#include <stdexcept>

using namespace std;

struct EvalResult {
    bool ok;
    long long value;
    string error;
};

class Parser {
public:
    explicit Parser(const string& s) : str(s), i(0) {}

    EvalResult solve() {
        try {
            skipSpaces();
            if (i >= str.size()) throw runtime_error("Expresión vacía.");
            long long val = parseExpr();
            skipSpaces();
            if (i != str.size()) throw runtime_error(string("Carácter inesperado: '") + str[i] + "'.");
            return {true, val, ""};
        } catch (const exception& e) {
            return {false, 0, e.what()};
        }
    }

private:
    const string& str;
    size_t i;

    void skipSpaces() {
        while (i < str.size() && isspace(static_cast<unsigned char>(str[i]))) i++;
    }

    long long parseExpr() {
        long long left = parseTerm();
        while (true) {
            skipSpaces();
            if (i >= str.size()) break;
            char op = str[i];
            if (op != '+' && op != '-') break;
            ++i;
            long long right = parseTerm();
            if (op == '+') left += right;
            else left -= right;
        }
        return left;
    }

    long long parseTerm() {
        long long left = parseFactor();
        while (true) {
            skipSpaces();
            if (i >= str.size()) break;
            char op = str[i];
            if (op != '*' && op != '/') break;
            ++i;
            long long right = parseFactor();
            if (op == '*') left *= right;
            else {
                if (right == 0) throw runtime_error("División por cero.");
                left /= right;
            }
        }
        return left;
    }

    long long parseFactor() {
        skipSpaces();
        if (i >= str.size()) throw runtime_error("Se esperaba un número o '(' al final.");
        if (str[i] == '(') {
            ++i;
            long long val = parseExpr();
            skipSpaces();
            if (i >= str.size() || str[i] != ')') throw runtime_error("Paréntesis no cerrado: falta ')'.");
            ++i;
            return val;
        }
        if (str[i] == '+' || str[i] == '-') throw runtime_error("Número con signo no permitido (solo números positivos).");
        return parseNumber();
    }

    long long parseNumber() {
        skipSpaces();
        if (i >= str.size() || !isdigit(static_cast<unsigned char>(str[i]))) {
            char c = (i < str.size() ? str[i] : '\0');
            if (c == '\0') throw runtime_error("Se esperaba un número, pero terminó la expresión.");
            throw runtime_error(string("Se esperaba un número, pero se encontró: '") + c + "'.");
        }
        long long num = 0;
        while (i < str.size() && isdigit(static_cast<unsigned char>(str[i]))) {
            int d = str[i] - '0';
            num = num * 10 + d;
            ++i;
        }
        return num;
    }
};

EvalResult resolverEcuacion(const string& ecuacion) {
    Parser p(ecuacion);
    return p.solve();
}

int main() {
    string eq;
    getline(cin, eq);
    EvalResult r = resolverEcuacion(eq);
    if (r.ok) cout << r.value << "\n";
    else cout << "Error: " << r.error << "\n";
    return 0;
}
