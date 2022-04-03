#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include<bits/stdc++.h>

const int ORDER = 20;

struct Nodo{
    size_t nChildren, nKeys;
    Nodo* father;
    Nodo* next;
    bool isLeaf;
    //se consideran estos valores para manejar el caso del overflow
    Nodo* children[ORDER+1];
    int keys[ORDER];

    Nodo();
    bool insertarKey(int , Nodo* = nullptr);
    Nodo* buscar(int);
    Nodo* buscarHoja(int);
    void reordenar();
    Nodo* partir();
    void print();
    void kill();
    ~Nodo();
};

class BplusTree
{
    private:
        Nodo* root;
        void dividir(Nodo *);

    public:
        BplusTree();
        void insertar(int);
        void borrar(int);
        void bfsPrint();
        void print();
        ~BplusTree();
};

#endif