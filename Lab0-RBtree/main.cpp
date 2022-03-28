#include "rbtree.h"
using namespace std;

void printVectorN(vector<Nodo*> v, string nombre) {
    cout<<nombre+": ";
    for(auto &n : v) {
        cout<<n->dato<<'-';
    }
    cout<<"\n";
}

void printTraversals(RBtree t) {

    vector<Nodo*> pre;
    vector<Nodo*> in;
    vector<Nodo*> pos;
    
    pre = t.preorder();
    in = t.inorder();
    pos = t.postorder();

    printVectorN(pre, "Preorden");
    printVectorN(in, "Inorder");
    printVectorN(pos, "Postorder");
}

int main() {
    RBtree t;
    int n;
    int data;
    int eliminar;

    cin>>n;
    cout<<"Creando RBTree...\n";
    while(n--) {
        cin>>data;
        t.insertar(data);
    }

    printTraversals(t);

    cin>>n;
    cout<<"Eliminando valor...\n";
    while(n--) {
        cin>>data;
        t.eliminar(data);
    }
    
    printTraversals(t);

    return 0;
}