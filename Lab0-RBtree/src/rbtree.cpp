#include <algorithm> 
#include "rbtree.h"

Nodo::Nodo(int dato){
    this->dato = dato;
    color = RED;
    
    left   = nullptr;
    right  = nullptr;
    father = nullptr;
}




int RBtree::getColor(Nodo* &puntero){
    if (puntero == nullptr) return BLACK;
    return puntero->color;
}

void RBtree::setColor(Nodo* &puntero, int color){
    if (puntero == nullptr) return;
    puntero->color = color;
}

/*          
 *        X                            Y
 *       / \     Rotar izquierda      / \
 *      A   Y    --------------->    X   C
 *         / \                      / \
 *        B   C                    A   B
 * 
 * La función se aplica a X
 */
void RBtree::rotarIzquierda(Nodo* &puntero){
    // Nodo Y
    Nodo *hijoDerecho = puntero->right;
    
    
    puntero->right = hijoDerecho->left;         // Asignar como hijo derecho de X a B
    if (puntero->right != nullptr)
        puntero->right->father = puntero;       // Asignar como nuevo padre de B a X


    hijoDerecho->father = puntero->father;      // Asignar que el nuevo padre de Y sea el padre de X
                                                // Esto es para conectar con el resto del arbol
    // Asignar la Y como hijo derecho o izquierdo 
    // En el paso anterior contectamos a Y con el resto del arbol, ahora Y será asignado como hijo del nodo superior
    if (puntero->father == nullptr)
        root = hijoDerecho;                     // Si no hay padre, significa que llegamos a la raiz
    else if (puntero == puntero->father->left)  
        puntero->father->left = hijoDerecho;    // Antes de la rotacion, X era el hijo izquiero
    else
        puntero->father->right = hijoDerecho;   // Antes de la rotacion, X era el hijo derecho

    hijoDerecho->left = puntero;                // Asignar a X como hijo izquierdo de Y
    puntero->father = hijoDerecho;              // Asignar a Y como padre de X
}

/*         
 *        Y                        X
 *       / \     Rotar derecha    / \
 *      X   C    ------------->  A   Y
 *     / \                          / \
 *    A   B                        B   C
 * 
 * La función se aplica a Y
 */
void RBtree::rotarDerecha(Nodo* &puntero){
    // Nodo X
    Nodo *hijoIzquierdo = puntero->left;        


    puntero->left = hijoIzquierdo->right;       // Asignar como hijo izquierdo de Y a B
    if (puntero->left != nullptr)
        puntero->left->father = puntero;        // Asignar como nuevo padre de B a Y


    hijoIzquierdo->father = puntero->father;    // Asignar que el nuevo padre de X sea el padre Y
                                                // Esto es para conectar con el resto del arbol
    // Asignar la X como hijo derecho o izquierdo 
    // En el paso anterior contectamos a X con el resto del arbol, ahora X será asignado como hijo del nodo superior
    if (puntero->father == nullptr)
        root = hijoIzquierdo;                   // Si no hay padre, significa que llegamos a la raiz
    else if (puntero == puntero->father->left)
        puntero->father->left = hijoIzquierdo;  // Antes de la rotacion, Y era el hijo izquiero
    else
        puntero->father->right = hijoIzquierdo;  // Antes de la rotacion, Y era el hijo derecho


    hijoIzquierdo->right = puntero;              // Asignar a Y como hijo derecho de X
    puntero->father = hijoIzquierdo;             // Asignar a X como padre de Y
}



Nodo* RBtree::insertarNodo(Nodo* &padre, Nodo* &puntero){
    // Arbol vacio
    if (padre == nullptr) return puntero;
    
    // Si el nuevo numero es menor al padre
    if(puntero->dato < padre->dato){
        padre->left = insertarNodo(padre->left,puntero);
        padre->left->father = padre;
    }
    // Si el nuevo numero es mayo al padre
    else if (puntero->dato > padre->dato){
        padre->right = insertarNodo(padre->right,puntero);
        padre->right->father = padre;
    }
    return padre;
}


void RBtree::corregirArbol(Nodo* &puntero){
    Nodo* padre  = nullptr;
    Nodo* abuelo = nullptr;
    while ( puntero != root &&  getColor(puntero)==RED && getColor(puntero->father)==RED ){
        padre  = puntero->father;
        abuelo = padre  ->father;

        // El padre esta a la izquierda
        if (padre == abuelo->left){
            Nodo* tio = abuelo->right;

            // CASO I: padre y tio son rojos
            if(  getColor(tio) ==RED ){
                setColor(padre ,BLACK);
                setColor(tio   ,BLACK);
                setColor(abuelo,RED  );
                puntero = abuelo;
            }
            else{
                // CASO II: padre y el hijo tienen distintas direcciones
                if(puntero == padre->right){
                    rotarIzquierda(padre);
                    puntero = padre;
                    padre = puntero->father;
                }

                // CASO III: padre y el hijo tienen la misma dirección
                rotarDerecha(abuelo);
                std::swap(padre->color,abuelo->color);
                puntero = padre;
            }
        }

        // El padre esta a la derecha
        else{
            Nodo* tio = abuelo->left;

            // CASO I: padre y tio son rojos
            if(  getColor(tio) ==RED ){
                setColor(padre ,BLACK);
                setColor(tio   ,BLACK);
                setColor(abuelo,RED  );
                puntero = abuelo;
            }
            else{
                // CASO II: padre y el hijo tienen distintas direcciones
                if(puntero == padre->left){
                    rotarDerecha(padre);
                    puntero = padre;
                    padre = puntero->father;
                }

                // CASO III: padre y el hijo tienen la misma dirección
                rotarIzquierda(abuelo);
                std::swap(padre->color,abuelo->color);
                puntero = padre;
            }
        }
    }

    setColor(root,BLACK);
    
}

void RBtree::insertar(int dato){
    Nodo *puntero = new Nodo(dato);
    root = insertarNodo(root,puntero);
    corregirArbol(puntero);
}

RBtree::RBtree(){
    root = nullptr;
}

RBtree::~RBtree(){
}

// Funciones privadas para recorridos.
// Funcionan de manera recursiva.

// preorder: padre, izquierda, derecha
void RBtree::preorder(std::vector<Nodo*> &pre, Nodo* n) {
    if (n != nullptr) {
        pre.push_back(n);
        preorder(pre, n->left);
        preorder(pre, n->right);
    }
}

// inorder: izquierda, padre, derecha
void RBtree::inorder(std::vector<Nodo*> &in, Nodo* n) {
    if (n != nullptr) {
        inorder(in, n->left);
        in.push_back(n);
        inorder(in, n->right);
    }
}

// postorder: izquierda, derecha, padre
void RBtree::postorder(std::vector<Nodo*> &pos, Nodo* n) {
    if (n != nullptr) {
        postorder(pos, n->left);
        postorder(pos, n->right);
        pos.push_back(n);
    }
}

// Funciones públicas para recorridos
// Retornan un vector de nodos con los nodos ordenados en el orden recorrido

// llamar a la función de preorder desde la raíz
std::vector<Nodo*> RBtree::preorder() {
    std::vector<Nodo*> pre;
    preorder(pre, root);
    return pre;
}

// llamar a la función de inorder desde la raíz
std::vector<Nodo*> RBtree::inorder() {
    std::vector<Nodo*> in;
    inorder(in, root);
    return in;
}

// llamar a la función de postorder desde la raíz
std::vector<Nodo*> RBtree::postorder() {
    std::vector<Nodo*> pos;
    postorder(pos, root);
    return pos;
}

// Búsqueda binaria para encontrar un nodo
Nodo* RBtree::buscarNodo(Nodo* &n, int d) {
    // si no se encuentra (no existe), devuelve un puntero nulo
    if (n == nullptr || d == n->dato) return n;
    // si el dato es menor, va por la izquierda
    if (d < n->dato) buscarNodo(n->left, d);
    // si el dato es mayor, va por la derecha
    else if (n->dato < d) buscarNodo(n->right, d);   
    // retorna el nodo si lo encuentra
}

// Función para ubicar al sucesor inorder de un nodo
Nodo* RBtree::ubicarSucesor(Nodo* & n) {
    // ir a la derecha
    Nodo* sucesor = n->right;
    // y navegar hasta el nodo más a la izquierda
    while (sucesor->left) {
        sucesor = sucesor->left;
    }
    // para elegirlo como sucesor
    return sucesor;
}

// Modificación del BST delete para RBTs
// La idea es retornar el nodo que se va a eliminar
Nodo* RBtree::eliminarBST(int d) {
    
    // Primero ubicar el nodo
    Nodo* v = buscarNodo(root, d);

    // si el nodo no existe, no hay nada más que hacer
    if (v == nullptr) return v;
    // si el nodo es una hoja, retornar el nodo
    if (v->left == nullptr && v->right == nullptr) return v;
    // si tiene que ser reemplazado, ubicar su sucesor (u)
    Nodo* u = ubicarSucesor(v);
    std::cout<<"u ="<<u->dato<<", v ="<<v->dato<<std::endl;
    // reemplazar los datos del sucesor con los del nodo
    v->dato = u->dato;
    // si hay más desbalances por arreglar antes de eliminar,
    // seguir explorando la derecha del sucesor
    while(u->right) {
        Nodo* temp = ubicarSucesor(u);
        u->dato = temp->dato;
        u = temp;
    }
    // retornar el nodo del sucesor, cual es el que se va a eliminar
    return u;
}

void RBtree::pasarNegro(Nodo* &n) {
    // Si el padre es rojo, se vuelve negro
    if ((n->father)->color == RED) (n->father)->color = BLACK;
    // Si el padre es negro, se vuelve doubleblack
    else (n->father)->color = DOUBLEBLACK;
}

bool ambosHijosNegros(Nodo* &n) {
    return 
        // (ambos son nil)
        (n->left == nullptr && n->right == nullptr) ||
        // (la izquierda es nil, la derecha es negra)
        (n->left == nullptr && n->right->color == BLACK) ||
        // (la izquierda es negra, la derecha es nil)
        (n->left->color == BLACK && n->right == nullptr) ||
        // (ambos son negros)
        (n->left->color == BLACK && n->right->color == BLACK);
}

// Arreglar el árbol acorde al caso identificado
void RBtree::identificarCaso(Nodo* & n) {
    
    //==========================
    // CASO 1:
    // si el nodo es rojo, simplemente eliminar
    if (n->color == RED) {
        return;
    }
    //==========================


    // si es negro, establecer como doubleblack
    // (recordar que el nodo a eliminar es una hoja,
    // así que si es negro, sus dos hijos son nil negros)
    n->color = DOUBLEBLACK;


    //==========================
    // CASO 2:
    // si el nodo es la raíz, remover el doubleblack
    if (n == root) {
        n->color = BLACK;
        return;
    }
    //==========================


    // Se identifica la posición en la que está el nodo a eliminar y su hermano
    bool pos;
    Nodo* hermano;
    Nodo* sobrinoLejano;
    // Si el nodo está a la izquierda, el hermano está a la derecha
    if ((n->father)->left == n) {
        pos = IZQUIERDA;
        hermano = (n->father)->right;
    }
    // Si el nodo está a la derecha, el hermano está a la izquierda
    else {
        pos = DERECHA;
        hermano = (n->father)->left;
    }

    // Si el hermano del nodo con DB es negro...
    if (hermano->color == BLACK) {

    //==========================
    // CASO 3:
    // Y tiene dos hijos negros,
        if (ambosHijosNegros(hermano)) {
            // Remover el DB y pasarle un negro al padre
            pasarNegro(n->father);
            // Hacer al hermano rojo
            hermano->color = RED;
            // Si hay más por arreglar, pasar a otros casos
            if ((n->father)->color == DOUBLEBLACK) identificarCaso(n->father);
            return;
        }
    //==========================

    //==========================
    // CASO 5:
    // Y el "sobrino" más lejano del nodo con DB es negro, pero el cercano es rojo

        else if (pos == IZQUIERDA &&
        ((hermano->right)->color == BLACK 
        && (hermano->left)->color == RED)) {
            // Rotar al hermano en sentido contrario al nodo con DB
            std::swap(hermano->color, (hermano->left)->color);
            rotarDerecha(hermano);
            // Pasar al caso 6
        }
        else if (pos == DERECHA &&
        ((hermano->left)->color == BLACK 
        && (hermano->right)->color == RED)) {
            // Rotar al hermano en sentido contrario al nodo con DB
            std::swap(hermano->color, (hermano->right)->color);
            rotarIzquierda(hermano);
            // Pasar al caso 6
        }
    //==========================

    //==========================
    // CASO 6:
    // El "sobrino" lejano es de color rojo
        // En el caso que esté a la izquierda
        else if(pos = IZQUIERDA && (hermano->right)->color == RED) {
            sobrinoLejano = hermano->right;
            std::swap(hermano->color, (n->father)->color);
            rotarIzquierda(n->father);
            n->color = BLACK;
            sobrinoLejano->color = BLACK;
        }
        
        // Lo mismo pero en el caso que esté a la derecha
        else if (pos = DERECHA && (hermano->left)->color == RED) {
            sobrinoLejano = hermano->left;
            // Cambiar el color entre padre y hermano
            std::swap(hermano->color, (n->father)->color);
            // Rotar al padre en dirección del DB
            rotarDerecha(n->father);
            // Volver el DB en negro
            n->color = BLACK;
            // Volver el sobrino lejano en negro
            sobrinoLejano->color = BLACK;
        }
    //==========================
    }

    //==========================
    // CASO 4:
    // Si el hermano del nodo es rojo
        // Cambiar el color del hermano y el padre
        std::swap((n->father)->color, hermano->color);
        // Rotar al padre en dirección del DB
        if(pos == IZQUIERDA) rotarIzquierda(n->father);
        else rotarDerecha(n->father);
        // Si hay más por arreglar, pasar a otros casos
        identificarCaso(n);
    //==========================
}

void RBtree::liberarNodo(Nodo* & n) {
    // Si el nodo es la raíz, marcar la raíz como nula
    if (n == root) root = nullptr;
    // Si no lo es, desvincular al nodo de su padre
    else {
        // si está a la izquierda
        if ((n->father)->left == n) (n->father)->left = nullptr;
        // si está a la derecha
        else if ((n->father)->right == n) (n->father)->right = nullptr;
    }
    // Finalmente, eliminar el nodo
    delete n;
}

// Función pública para eliminar

void RBtree::eliminar(int d) {
    // Ubica el nodo a eliminar (v) empleando el BST delete
    Nodo* v = eliminarBST(d);
    // si el nodo no existe, no hay nada más que hacer
    if (v == nullptr) return;
    // si existe, evaluar cada caso para eliminar este nodo
    identificarCaso(v);
    // Liberar la memoria ocupada por el nodo
    liberarNodo(v);
}