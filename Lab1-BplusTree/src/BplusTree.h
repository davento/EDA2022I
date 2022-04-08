#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include<bits/stdc++.h>
#define FOR(i,a,b) for(size_t i=(a);i<(b);i++)
#define ORDER 20

enum Side {LEFT, RIGHT};

struct Node{
    size_t nChildren, nKeys;
    Node* parent;
    Node* next;
    bool isLeaf;
    Node* children[ORDER+1];
    int keys[ORDER];

    Node();
    bool insertKey(int, Node* = nullptr);
    void shiftKeys(size_t, size_t = 1, bool = LEFT);
    void shiftChildren(size_t, size_t = 1, bool = LEFT);
    void updateKeys();
    Node* findLeftSibling();
    Node* findRightSibling();
    Node* findNode(int);
    void deleteDuplicate();
    Node* splitNode();
    void printNode();
    void killNode();
    ~Node();
};

class BplusTree
{
    private:
        Node* root;
        void splitNode(Node *);
        void eraseKeyLeaf(int, Node*);
        void eraseKeyInner(Node*);
    public:
        BplusTree();
        void insert(int);
        void erase(int);
        void bfsPrint();
        void linealPrint();
        ~BplusTree();
};

#endif