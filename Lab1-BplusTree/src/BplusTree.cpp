#include "BplusTree.h"

Node::Node() :
	nChildren(0), 
	nKeys(0), 
	parent(nullptr), 
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

void Node::shiftKeys(size_t index, size_t distance, bool direction) {
	if(direction == LEFT) {
		FOR(i,index,nKeys) {
			keys[i] = keys[i+distance];
		}
	}
	if(direction == RIGHT) {
		for(size_t i = nKeys-2; i > index - 1; i--) {
			keys[i+distance] = keys[i];
		}
	}
}

void Node::shiftChildren(size_t index, size_t distance, bool direction) {
	if (direction == LEFT) {
		FOR(i,index,nChildren) {
			children[i] = children[i+distance];
		}
	}
	if (direction == RIGHT) {
		for(int i = nChildren-2; i > 0; i--) {
			children[i+distance] = children[i];
		}
	}
}

Node* Node::findLeftSibling() {
	if(!parent)  return nullptr;
    for(size_t i = 0; i < parent->nChildren;i++){
        if(parent->children[i] == this){
            if(i == 0 && parent->parent){
                Node* uncle = parent->findLeftSibling();
                if(uncle){
                    return uncle->children[uncle->nChildren-1];
                }
                return nullptr;
            }
            else if(i != 0) return parent->children[i-1];
            else if(!parent->parent) return nullptr;
        }
    }
    return nullptr;
}

Node* Node::findRightSibling() {
	if(!parent)  return nullptr;
    for(size_t i = 0; i < parent->nChildren;i++){
        if(parent->children[i] == this){
            if(i == 0 && parent->parent){
                Node* uncle = parent->findRightSibling();
                if(uncle){
                    return uncle->children[0];
                }
                return nullptr;
            }
            else if(i != 0) return parent->children[i+1];
            else if(!parent->parent) return nullptr;
        }
    }
    return nullptr;
}

void Node::updateKeys() {
	if (nChildren > 1) {
		FOR(i,0,nKeys) {
			keys[i] = children[i+1]->keys[0];
		}
	}
}

void BplusTree::eraseKeyInner(Node* p) {
	if (p == nullptr) return;
	p->updateKeys();
	if (p == root && root->nChildren == 1) {
		root = root->children[0];
		return;
	}
	while(p->nChildren < ceil(ORDER/2)) {
		Node* s = p->findLeftSibling();
		if(s) {
			if(s->nChildren - 1 > ceil(ORDER/2)) {
				p->shiftChildren(0,1,RIGHT);
				p->children[0] = s->children[s->nChildren-1];
				s->children[s->nChildren-1] = nullptr;
				s->nChildren--;
				p->updateKeys();
				s->updateKeys();
			}
			else {
				FOR(i,0,p->nChildren) {
					s->children[s->nChildren-1+i] = p->children[i];
					p->children[i] = nullptr;
				}
				s->nChildren += p->nChildren;
				Node* temp = p;
				p = s;
				p->updateKeys();
				delete temp;
			}
		}
		else {
			s = p->findRightSibling();
			if(s->nChildren - 1 > ceil(ORDER/2)) {
				p->children[p->nChildren-1] = s->children[0];
				s->shiftChildren(0,1,LEFT);
				s->nChildren--;
				p->updateKeys();
				s->updateKeys();
			}
			else {
				s->shiftChildren(0, p->nChildren, RIGHT);
				FOR(i,0,p->nChildren) {
					s->children[i] = p->children[i];
					p->children[i] = nullptr;
				}
				s->nChildren += p->nChildren;
				Node* temp = p;
				p = s;
				p->updateKeys();
				delete temp;
			}
		}
	}
	p = p->parent;
	eraseKeyInner(p);
/*
  if p is null:
    stop the procedure
  reset p's keys accordingly
  if p is the root and the root has one child:
    update the root as its child
    stop the procedure
  while the minimum children condition is not met by p:
    if the left sibling s exists:
        if it can borrow:
            borrow children from s
            decrease the number of children in s by 1
            update p's and s's keys
        if it can't:
            merge p onto s (by passing over the remaining children in p to the left of the already existing children in s)
            increase the number of children in s by the number of children passed over
            let temp be p
            set p as s
			update p's keys
            delete temp
    else if the right sibling s exists:
        if it can borrow:
            borrow children from s
            shift s's children to the left
            reduce the number of children in s by 1
            reset p's and s's keys accordingly
        if it can't:
            merge p onto s (by shifting s's children to the right and positioning the remaining children in p in the blank spaces)
            increase the number of children in s by the number of children passed over
            let temp be p
            set p as s
			update p's keys
            delete temp
    update p as p's parent
    run internal deletion for p
*/
}

void BplusTree::eraseKeyLeaf(int k, Node* n) {
	bool kFound = false;
	// locate the key itself
	size_t i;
	for(i= 0; i < n->nKeys; i++) {
		if(n->keys[i] == k) {
			kFound = true;
			break;
		}
	}
	// if not found, abort the procedure
	if (!kFound) return;
	// if found, erase the key
	n->keys[i] = INT_MAX;
	// shift the remaining keys to the left
	if (i < n->nKeys-1) n->shiftKeys(i, 1, LEFT);
	n->nKeys--;
	//if the number of keys condition is not met
	if (n->nKeys < ceil(ORDER/2)-1) {
		Node* s = nullptr;
		Node* p = n->parent;
		// check if left sibling exists
		s = n->findLeftSibling();
		size_t toBorrow = ceil(ORDER/2)-n->nKeys;
		if (s) {
			// if it does, check if it can borrow its sibling's maximum values
			if(s->nKeys - toBorrow > ceil(ORDER/2)) {
				n->shiftKeys(0, ceil(ORDER/2)-1-n->nKeys, RIGHT);
				FOR(y,0,ceil(ORDER/2)-1-n->nKeys) {
					n->keys[y] = s->keys[s->nKeys-1-y];
					s->keys[s->nKeys-1-y] = INT_MAX;
				}
				n->nKeys = ceil(ORDER/2)-1;
				s->nKeys -= toBorrow;
			}
			// if it can't borrow, merge
			else {
				// do the merging part
				size_t x = s->nKeys-1;
				FOR(y,0,ceil(ORDER/2)-1) {
					s->keys[x] = n->keys[y];
					x++;
					s->nKeys++;
				}
				// shift the rest of p's children to the left by 1 if needed
				if (!(n == p->children[p->nChildren-1])) {
					FOR(y,0,p->nChildren) {
						if(p->children[y] == n) {
							p->shiftChildren(y, 1, LEFT);
							break;
						}
					}
				}
				p->nChildren--;
				s->next = n->next;
				delete n;
			}
		}
		else {
			s = n->findRightSibling();
			if (s) {
				if(s->nKeys - toBorrow > ceil(ORDER/2)) {
					// have n borrow the minimum value keys it needs from sibling
					// update n's and s's number of keys
					FOR(y,0,ceil(ORDER/2)-1) {
						n->keys[n->nKeys+y] = s->keys[y];
						s->keys[y] = INT_MAX;
					}
					n->nKeys = ceil(ORDER/2)-1;
					s->nKeys -= toBorrow;
				}
				else {
					// merge s and n by moving the remaining keys in n to s
					s->shiftKeys(0, n->nKeys, RIGHT);
					FOR(y,0,n->nKeys) {
						s->keys[y] = s->keys[y];
						s->nKeys++;
					}
          			// shift the rest of p's children to the left by 1
					FOR(y,0,p->nChildren) {
						if(p->children[y] == n) {
							p->shiftChildren(y, 1, LEFT);
							break;
						}
					}
					p->nChildren--;
					delete n;
				}
			}
		}
		eraseKeyInner(p);
	}
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
        children[i-1]->parent = this;
    }
    // adjust the right-most child node
    children[i-1] = children[i];
    children[i] = nullptr;
    keys[nKeys-1] = INT_MAX;
    children[i-1]->parent = this;
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
	if(!n->parent) {
		Node* p = new Node();
		p->isLeaf = false;
		root = p;
		h->parent = n->parent = p;
		p->insertKey(h->keys[0]);
		p->children[0] = n;
		p->children[1] = h;
		if (n->isLeaf) n->next = h;
	}
	else {
		h->parent = n->parent;
		(n->parent)->insertKey(h->keys[0],h);
	}
	if(!h->isLeaf) h->deleteDuplicate();
	if(n->isLeaf) n->next = h;
	if((n->parent)->nKeys == ORDER) splitNode(n->parent);
}

void BplusTree::insert(int k) {
	if(!root) root = new Node();
	Node* n = root->findNode(k);
	if(!n->insertKey(k)) splitNode(n);
}

void BplusTree::erase(int k) {
	Node* n = root->findNode(k);
	if (!n) return;
	eraseKeyLeaf(k, n);
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