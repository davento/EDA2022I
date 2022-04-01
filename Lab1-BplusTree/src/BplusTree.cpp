#include "BplusTree.h"
#define FOR(i,a,b) for(size_t i=(a);i<(b);i++)
#define INT_MAX std::numeric_limits<int>::max();

Nodo::Nodo() :
nChildren(0),
nKeys(0),
father(nullptr),
next(nullptr),
isLeaf(true)
{FOR(i,0,ORDER+1) children[i] = nullptr;
{FOR(i,0,ORDER) keys[i] = INT_MAX;}}

Nodo::~Nodo(){}

// k y el nodo que sigue
bool Nodo::insertarKey(int k, Nodo* n = nullptr) {
    // añadir una llave más al nodo porque se le va a insertar un nuevo valor
    nKeys++;
    // declarar variables que se utilizarán como "iteradores"
    int itK = k;
    Nodo* itN = n;
    
}

// ubicar un nodo que contiene a k
Nodo* Nodo::buscar(int k) {
    // si el nodo está vacío, retornar nada
    if (nKeys == 0) return nullptr;
    // caso contrario, explorar todos las keys en el nodo
    FOR(i,0,nKeys) {
        // bias a la derecha
        if (k >= keys[i]) {
            // si es que k es mayor que el elemento, explorar sus hijos
            if (children[i+1]) return (children[i+1])->buscar(k);
            // si contiene a k, retornar el mismo nodo
            else if (k == keys[i]) return this;
            else return nullptr;
        }
        // si es menor, explorar sus hijos
        else if (k < keys[i]) {
            if (children[i]) return (children[i])->buscar(k);
            else return nullptr;
        }
    }
    // simplemente no está en este nodo
    return nullptr;
}

BplusTree::BplusTree() {

}

void BplusTree::insertar(int k) {
    Nodo* n = root->buscar(k);
}