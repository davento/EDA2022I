#include "rbtree.h"
using namespace std;

void printVectorN(vector<Nodo*> v, string nombre) {
    cout<<nombre+": ";
    for(auto &n : v) {
        cout<<n->dato<<'-';
    }
    cout<<"\n";
}

int main() {
    RBtree t;
    int n;
    int data;
    vector<Nodo*> pre;
    vector<Nodo*> in;
    vector<Nodo*> pos;

    cout<<"Creando RBTree...\n";

    cin>>n;
    while(n--) {
        cin>>data;
        t.insertar(data);
    }

    pre = t.preorder();
    in = t.inorder();
    pos = t.postorder();

    printVectorN(pre, "Preorden");
    printVectorN(in, "Inorder");
    printVectorN(pos, "Postorder");

    return 0;
}