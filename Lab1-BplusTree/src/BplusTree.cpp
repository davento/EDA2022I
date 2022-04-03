#include "BplusTree.h"
#define FOR(i,a,b) for(size_t i=(a);i<(b);i++)

Nodo::Nodo() :
    nChildren(0),
    nKeys(0),
    father(nullptr),
    next(nullptr),
    isLeaf(true) {
    FOR(i,0,ORDER+1) children[i] = nullptr;
    FOR(i,0,ORDER) keys[i] = INT_MAX;
}

Nodo::~Nodo(){}

// 
bool Nodo::insertarKey(int k, Nodo* n) {
    // añadir una key más al nodo porque se le va a insertar un nuevo valor
    nKeys++;
    // declarar variables que se utilizarán como "iteradores"
    int itK = k;
    Nodo* itN = n;
    // añadir el key en sí depende de si es hoja o no, así que lo determinamos
    bool hijoHoja = (children[0]) ? children[0]->isLeaf : false;

    FOR(i,0,nKeys){
        // si es que va en una posición menor al otro nodo
        if(itK < keys[i]) {
            // hacer un swap de las posiciones de las keys
            // para poder insertar la nueva key
            int t = keys[i];
            keys[i] = itK;
            itK = t;

            // si es una hoja nueva, se va a
            // añadir la conexión su padre
            // (nótese que si esto se inicializa como nullptr
            // no se va a hacer ningún intercambio)
            Nodo* temp = children[i+1];
            children[i+1] = itN;
            itN = temp;

            // y también conectar con la hoja que sigue
            if(hijoHoja) children[i]->next = children[i+1];
        }
    }
    // añadir hijos insertados
    if (nKeys == 1) nChildren += 2;
    else nChildren++;

    // si el nodo está lleno, retorna falso
    // pues no se ha podido insertar en el nodo
    if (nKeys == ORDER) return false;
    // en caso sí se haya insertado, es verdadero
    return true;
}

// ubicar un nodo que contiene a k
Nodo* Nodo::buscar(int k) {
    // si es una hoja, no hay más por explorar
    if (isLeaf) return this;
    // explorar todos las keys en el nodo
    // y sus hijos
    FOR(i,0,nKeys) {
        if (k < keys[i]) {
            if (children[i]) return (children[i])->buscar(k);
        }
    }
    return children[nKeys]->buscar(k);
}

// ubicar la hoja en la que se ubica un key
Nodo* Nodo::buscarHoja(int k) {
    // si está vacío, retornar el mismo nodo
    if (nKeys == 0) return this;
    // sino, seguir verificando
    FOR(i,0,nKeys) {
        // si el key es mayor e insertarlo no produce overflow
        if (k > keys[i] && i == nKeys - 1) {
            // está ocupado por un hijo a la derecha, explorarlo
            if(children[i+1]) return (children[i+1]->buscarHoja(k));
            // si no, esta es la hoja
            return this;
        }
        // si corresponde en la izquierda,
        else if (k < keys[i]) {
            // explorar los hijos a la izquierda, si es que existen
            if(children[i]) return (children[i]->buscarHoja(k));
            // si no, esta es la hoja
            return this;
        }
    }
    // si no se encontró una posición,
    return nullptr;
}

// ajustar las keys e hijos de los nodos particionados
void Nodo::reordenar() {
    // el ajuste mueve uno a la izquierda
    size_t i;
    for (i = 1; i < nKeys; i++) {
        keys[i-1] = keys[i];
        children[i-1] = children[i];
        // asegurar que se ha establecido al padre correctamente
        children[i-1]->father = this;
    }
    // ajustar el valor de más a la derecha
    children[i-1] = children[i];
    children[i] = nullptr;
    keys[nKeys-1] = INT_MAX;
    children[i-1]->father = this;
    // se reduce una llave y un hijo producto del ajuste
    nKeys--;
    nChildren--;
}

// realizar la partición interna del nodo
Nodo* Nodo::partir() {
    Nodo* h = new Nodo();
    
    // ajustar los elementos del hermano
    FOR(i,(ORDER)/2,ORDER) {
        // llenar al nuevo hermano con las keys
        // de la segunda mitad del nodo original
        h->insertarKey(keys[i]);
        // vaciar los elementos de la segunda
        // mitad del nodo original
        keys[i] = INT_MAX;
        // restar la cantidad de keys
        nKeys--;
    }
    // ajustar los hijos del hermano
    FOR(i,(ORDER)/2 +1, ORDER+1) {
        h->children[i - (ORDER)/2] = children[i];
        children[i] = nullptr;
        nChildren--;
    }
    return h;
}

// iterar por las keys en el nodo e imprimirlos
void Nodo::print() {
    std::cout<<"[";
    FOR(i,0,nKeys-1) std::cout<<keys[i]<<" ";
    std::cout<<keys[nKeys-1]<<"]";
}

void Nodo::kill() {
    FOR(i,0,ORDER) {
        if(children[i]) children[i]->kill();
    }
    delete this;
}

// ============================================

BplusTree::BplusTree() : root(new Nodo){}

// dividir el nodo y establecer sus dependencias en el árbol
void BplusTree::dividir(Nodo* n) {
    // se divide el nodo en dos
    Nodo* h = n->partir();
    // si el nodo es una hoja, el nuevo también
    h->isLeaf = n->isLeaf;
    // si no existe un padre, se crea uno (un nivel arriba)
    if(!n->father) {
        // como se está haciendo un nivel superior
        Nodo* p = new Nodo();
        p->isLeaf = false;
        // pasa a ser la raíz
        root = p;
        // se actualiza como el padre de ambos nodos partidos
        h->father = n->father = p;
        // se añade la referencia al key que "sube"
        p->insertarKey(h->keys[0]);
        // se agregan los nodos partidos como sus hijos
        p->children[0] = n;
        p->children[1] = h;
        // si es hoja, se crea la conexión con el nuevo nodo en el linked list
        if (n->isLeaf) n->next = h;
    }
    // caso contrario, se actualiza el nexo con el nodo nuevo
    else {
        h->father = n->father;
        (n->father)->insertarKey(h->keys[0], h);
    }
    // reordenar las keys si es que la referencia se movió con un nodo interno
    if(!h->isLeaf) h->reordenar();
    // si es hoja, se crea la conexión con el nuevo nodo en el linked list (por si existe un padre)
    if(n->isLeaf) n->next = h;
    // verificar si hay overflow en el padre. Si lo hay, dividirlo recursivamente hasta que no haya más
    if((n->father)->nKeys == ORDER) dividir(n->father);
}

void BplusTree::insertar(int k) {
    if(!root) root = new Nodo();
    Nodo* n = root->buscar(k);
    if(!n->insertarKey(k)) dividir(n);
}

// imprimir en BFS
void BplusTree::bfsPrint() {
    // inicializar variables para BFS
    std::list<Nodo*> v;
    v.emplace_back(root);
    // lógica de BFS
    while(!v.empty()) {
        Nodo* toPrint = v.front();
        v.pop_front();
        FOR(i,0,toPrint->nChildren) {
            if(toPrint->children[i]) v.emplace_back(toPrint->children[i]);
        }
        toPrint->print();
    }
    std::cout<<std::endl;
}

void BplusTree::print() {
    // ir hasta el primer nodo
    Nodo* f = root->buscarHoja(-1);
    // avanzar secuencialmente e ir imprimiendo
    while(f->next) {
        f->print();
        std::cout<<" -> ";
        f = f->next;
    }
    f->print();
    std::cout<<std::endl;
}

void BplusTree::borrar(int k) {
    // to do
}

BplusTree::~BplusTree() {root->kill();}