#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include<bits/stdc++.h>

const int ORDER = 20;

struct Nodo{
    size_t nChildren, nKeys;
    Nodo* children[ORDER];
    int keys[ORDER];
    Nodo* father;
    Nodo* next;
    bool isLeaf;

    Nodo();
    bool insertarKey(int, Nodo* = nullptr);
    Nodo* buscar(int);
    ~Nodo();
};

class BplusTree
{
    private:
        Nodo* root;

    public:
        BplusTree();
        void insertar(int);
        void borrar(int);

        ~BplusTree();
};

#endif