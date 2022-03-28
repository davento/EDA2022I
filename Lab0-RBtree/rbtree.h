#ifndef RBTREE_H
#define RBTREE_H

#include <bits/stdc++.h>

// Lista de colores del arbol
enum Color {RED,BLACK, DOUBLEBLACK};
enum Lado {IZQUIERDA, DERECHA};

struct Nodo{
    // Atributos
    int dato;
    int color;

    // Relacion con otros Nodos
    Nodo *left;
    Nodo *right;
    Nodo *father;

    // Constructor
    Nodo(int);
};

class RBtree
{
private:
    Nodo *root;
    Nodo *insertarNodo(Nodo* &, Nodo* &);
    void corregirArbol(Nodo* &);

    int getColor(Nodo* &);
    void setColor(Nodo* &, int );
    void pasarNegro(Nodo* &n);

    void rotarIzquierda(Nodo* &);
    void rotarDerecha  (Nodo* &);

    void preorder(std::vector<Nodo*> &, Nodo*);
    void inorder(std::vector<Nodo*> &, Nodo*);
    void postorder(std::vector<Nodo*> &, Nodo*);

    Nodo* eliminarBST(int);
    void identificarCaso(Nodo* &);
    void liberarNodo(Nodo* &);
    Nodo* buscarNodo(Nodo*&, int);
    Nodo* ubicarSucesor(Nodo* &);
    void corregirEliminar(Nodo* &);

public:
    RBtree();
    void insertar(int);
    void eliminar(int);
    std::vector<Nodo*> preorder();
    std::vector<Nodo*> inorder();
    std::vector<Nodo*> postorder();
    ~RBtree();
};






#endif