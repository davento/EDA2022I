#include "BplusTree.h"

Node::Node() :
	nChildren(0), 
	nKeys(0), 
	father(nullptr), 
	next(nullptr),
	isLeaf(true) {
	FOR(i,0,ORDER+1) children[i] = nullptr;
	FOR(i,0,ORDER) keys[i] = INT_MAX;
}

bool Node::insertKey(int k, Node* n) {
	nKeys++;
	int itK = k;
	Node* itN = n;
	bool childIsLeaf = (children[0]) ? children[0]->isLeaf : false;
	FOR(i,0,nKeys) {
		if(itK < keys[i]) {
			int temp = keys[i];
			keys[i] = itK;
			itK = temp;

			Node* t = children[i+1];
			children[i+1] = itN;
			itN = t;

			if(childIsLeaf) children[i]->next = children[i+1];
		}
	}
	nChildren += (nKeys == 1) ? 2 : 1;
	if (nKeys==ORDER) return false;
	return true;
}

void Node::shiftKeys(size_t i) {
	for(i=0; i < nChildren; i++) {
		//shift
	}
}

void Node::shiftChildren() {

}

int Node::nextAvailableKey() {
	FOR(i,0,nKeys-1) {
		if(keys[i] == INT_MAX) return i;
	}
	return {};
}

Node* Node::findNode(int k, int& j) {
	if(isLeaf) {
		//locate what child it is and assign that to j
		return this;
	}
	FOR(i,0,nKeys) {
		if(k < keys[i] && children[i]) return children[i]->findNode(k);
	}
	return children[nKeys]->findNode(k);
}

bool Node::eraseKey(int k, int j) {
	bool kFound = false;
	// locate the key itself
	size_t i;
	for(i= 0; i < nKeys; i++) {
		if(keys[i] == k) {
			kFound = true;
			break;
		}
	}
	if (!kFound) return false;
	keys[i] = INT_MAX;
	if (i < nKeys-1) shiftKeys(i);
	nKeys--;

	if (nKeys < ceil(ORDER/2)-1) {
		Node* s = nullptr;
		//check if left sibling exists
		if(j - 1 >= 1) s = father->children[j-1];
		else {
			//check if left uncle has a kid and pick that one as s
		}
		// if it does, check if it can borrow
		if(s->nKeys > ceil(ORDER/2)) {
			
		}
		// if it can't borrow, merge
		else {
			int x = s->nextAvailableKey();
			FOR(y,0,ceil(ORDER/2)-1) {
				s->keys[x] = keys[y];
				keys[y] = INT_MAX;
				x++; nKeys--; s->nKeys++;
			}
		}
	}
	return true;
}

Node* Node::findNode(int k) {
	if(isLeaf) return this;
	FOR(i,0,nKeys) {
		if(k < keys[i] && children[i]) return children[i]->findNode(k);
	}
	return children[nKeys]->findNode(k);
}

void Node::deleteDuplicate() {
    size_t i;
    for (i = 1; i < nKeys; i++) {
        keys[i-1] = keys[i];
        children[i-1] = children[i];
        children[i-1]->father = this;
    }
    // adjust the right-most child node
    children[i-1] = children[i];
    children[i] = nullptr;
    keys[nKeys-1] = INT_MAX;
    children[i-1]->father = this;
    // reduce key and child values due to duplicate erradication
    nKeys--;
    nChildren--;
}

Node* Node::splitNode() {
	Node* h = new Node();
	// transfer keys to the new node
	// and wipe the unused keys on the old node
	FOR(i,ORDER/2,ORDER) {
		h->insertKey(keys[i]);
		keys[i] = INT_MAX;
	}
	nKeys -= ORDER - ORDER/2;

	// transfer children to the new node
	// and wipe the unused children spaces in the old node
	FOR(i,(ORDER/2)+1,ORDER+1) {
		h->children[i - ORDER/2] = children[i];
		children[i] = nullptr;
	}
	nChildren -= (ORDER+1)-((ORDER/2)+1);

	// return the new node
	return h;
}

void Node::printNode() {
    std::cout<<"[";
    FOR(i,0,nKeys-1) std::cout<<keys[i]<<" ";
    std::cout<<keys[nKeys-1]<<"]";
}

void Node::killNode() {
    FOR(i,0,ORDER) {
        if(children[i]) children[i]->killNode();
    }
    delete this;
}

Node::~Node() {}

// ========================================


BplusTree::BplusTree(){
	root = nullptr;
}

void BplusTree::splitNode(Node* n) {
	Node* h = n->splitNode();
	h->isLeaf = n->isLeaf;
	if(!n->father) {
		Node* p = new Node();
		p->isLeaf = false;
		root = p;
		h->father = n->father = p;
		p->insertKey(h->keys[0]);
		p->children[0] = n;
		p->children[1] = h;
		if (n->isLeaf) n->next = h;
	}
	else {
		h->father = n->father;
		(n->father)->insertKey(h->keys[0],h);
	}
	if(!h->isLeaf) h->deleteDuplicate();
	if(n->isLeaf) n->next = h;
	if((n->father)->nKeys == ORDER) splitNode(n->father);
}

void BplusTree::insert(int k) {
	if(!root) root = new Node();
	Node* n = root->findNode(k);
	if(!n->insertKey(k)) splitNode(n);
}

void BplusTree::erase(int k) {
	// locate the node that has the key
	int j = 0;
	Node* n = root->findNode(k, j);
	if (!n) return;
	n->eraseKey(k, j);
}

void BplusTree::bfsPrint() {
    std::list<Node*> v;
    v.emplace_back(root);
    while(!v.empty()) {
        Node* toPrint = v.front();
        v.pop_front();
        FOR(i,0,toPrint->nChildren) {
            if(toPrint->children[i]) v.emplace_back(toPrint->children[i]);
        }
        toPrint->printNode();
    }
    std::cout<<std::endl;
}

void BplusTree::linealPrint() {
    // go till the first node
    Node* f = root->findNode(-1);
    // print sequentially
    while(f->next) {
        f->printNode();
        std::cout<<" -> ";
        f = f->next;
    }
    f->printNode();
    std::cout<<std::endl;
}

BplusTree::~BplusTree() {root->killNode();}