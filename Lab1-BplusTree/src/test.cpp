#include "BplusTree.h"

std::vector<int> v = {1, 4, 7, 10, 17, 21, 31, 25, 19, 20, 28, 42};

int main() {
    auto tree = new BplusTree();
    for(auto &x : v){
        tree->insertar(x);
    }
    std::cout<<"Inorder:\n";
    tree->print();
    std::cout<<"BFS:\n";
    tree->bfsPrint();
    delete tree;   

}