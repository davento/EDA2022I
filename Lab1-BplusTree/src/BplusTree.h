#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include<bits/stdc++.h>

const int ORDER = 20;

struct Nodo{
    size_t nChildren, nKeys;
    Nodo* children[ORDER+1];
    int keys[ORDER];
    Nodo* father;
    Nodo* next;
    bool isLeaf;

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
        Nodo* dividir(Nodo *);

    public:
        BplusTree();
        void insertar(int);
        void borrar(int);
        void bfsPrint();
        void print();
        ~BplusTree();
};

#endif