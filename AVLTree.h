//
// Created by omerg on 05/05/2021.
//

#ifndef WET1_AVLTREE_H
#define WET1_AVLTREE_H
#ifndef HW1Wet_AVLTREE_H
#define HW1Wet_AVLTREE_H

#include <iostream>
#include <ostream>
#include <algorithm>

using std::max;
using std::endl;
using std::cout;


/*------------------------------------------------------------------------------
------------------------------------AVLNode-------------------------------------
------------------------------------------------------------------------------*/

template <class Key, class Data>
class AVLNode
{
public:
    Key key;
    Data data;
    int height; //height of the tree that this node is the root
    int bf; //balance factor

    AVLNode *parent;
    AVLNode *left;
    AVLNode *right;


    AVLNode(Key k, Data d); //constructor
    ~AVLNode(); //destructor

    void fillTree(int* num){
        if(this== nullptr|| (*num)<0)return ;
        this->right->fillTree(num);
        (*num)-=1;
        this->key=(*num);
        this->data=(*num);
        this->left->fillTree(num);
    }

};


/*------------------------------------------------------------------------------
------------------------------------AVLTree-------------------------------------
------------------------------------------------------------------------------*/

template <class Key, class Data>
class AVLTree
{
public:
    AVLNode<Key,Data> *root = nullptr;
    AVLNode<Key,Data> *min = nullptr;

    AVLTree(); //constructor
    ~AVLTree(); //destructor
    AVLTree(AVLNode<Key,Data>* root):root(root){
        auto node=root;
        while (node =! nullptr) node=node->left;
        min(node);
    }

    //insert Node into the right place in the tree while maintain the balance
    void insertNode(Key k, Data d);
    //remove Node from the tree while maintain the balance
    void removeNode(Key k);
    //find Node from the tree by key
    AVLNode<Key,Data> *findNode(Key k);
    //updates root tree
    void updateRoot(AVLNode<Key,Data> *n);
    void updateMin(AVLNode<Key,Data> *n);
    //finde next node in order
    AVLNode<Key,Data> *nextNodeInOrder(AVLNode<Key,Data> *current);

    //rotations
    void LLrotate(AVLNode<Key,Data> *n);
    void LRrotate(AVLNode<Key,Data> *n);
    void RRrotate(AVLNode<Key,Data> *n);
    void RLrotate(AVLNode<Key,Data> *n);

    //delete all nodes of the tree
    void clearTree();
    //print all nodes of the tree in ascending order
    void printTree();


    void inOrder(AVLNode<Key,Data>* node,void (*func)(AVLNode<Key,Data>* node,int* arg1,int* arg2),
            int* arg1,int* arg2,int *cycles){
        if(node== nullptr || *cycles == 0)return;
        inOrder(node->left,func, arg1,arg2,cycles);
        *cycles-=1;
        func(node,arg1,arg2);
        inOrder(node->right,func, arg1,arg2,cycles);
    }

    int pow2(int n) {
        int res=1;
        while (n>0){
            res*=2;
            n--;
        }
        return res;
    }



    AVLNode<Key,Data>* completeTree(int nodes_num){
        if(nodes_num==0)return nullptr;
        int log2n=0,tmp=nodes_num;
        while (tmp!=0){
            tmp=tmp/2;
            log2n++;
        }
        int hight= log2n-1;
        if(hight<0)return nullptr;
        AVLNode<Key,Data>* avLnode= new AVLNode<int,int>(-1,-1);

        AVLNode<Key,Data>* tmp_right= completeTree(hight-1);
        avLnode->right=tmp_right;
        if(tmp_right!= nullptr)tmp_right->parent=avLnode;

        AVLNode<Key,Data>* tmp_left= completeTree(hight-1) ;
        avLnode->left=tmp_left;
        if(tmp_left!= nullptr)tmp_left->parent=avLnode;

        avLnode->height=hight;
        return avLnode;
    }

    void deleteForComplete(int* num_toDel) {
        if(this == nullptr ||  (*num_toDel)<=0)return;
        this->right->deleteForComplete(num_toDel);
        if(this->isLeaf()) {
            (*num_toDel) -= 1;
            if (this->parent->right != nullptr) {
                this->parent->right=nullptr;
                delete this;
            } else {
                this->parent->left=nullptr;
                delete this;
            }
            return;
        }
        this->left->deleteForComplete(num_toDel);
    }



};




/*------------------------------------------------------------------------------
-----------------------------AVLNode-Implementation-----------------------------
------------------------------------------------------------------------------*/

template <class Key, class Data>
AVLNode<Key, Data>::AVLNode(Key k, Data d): key(k), data(d), height(0), bf(0),
                                            parent(nullptr), left(nullptr), right(nullptr){}

/*----------------------------------------------------------------------------*/

template <class Key, class Data>
AVLNode<Key, Data>::~AVLNode(){
    parent = nullptr;
}

/*------------------------------------------------------------------------------
-----------------------------AVLTree-Implementation-----------------------------
------------------------------------------------------------------------------*/

template <class Key, class Data>
AVLTree<Key, Data>::AVLTree(): root(nullptr) {}

/*----------------------------------------------------------------------------*/

template<class Key, class Data>
AVLTree<Key, Data>::~AVLTree(){
    this->clearTree();
}

/*----------------------------------------------------------------------------*/

template<class Key, class Data>
void AVLTree<Key, Data>::insertNode(Key k, Data d){
    if (this->root == nullptr){ //tree is empty
        this->root = new AVLNode<Key, Data> (k, d);
        return;
    }

    AVLNode<Key, Data> *sub_root = this->root;
    while (sub_root != nullptr){
        if(k < sub_root->key){
            if (sub_root->left != nullptr)  sub_root = sub_root->left;
            else{
                sub_root->left = new AVLNode<Key, Data> (k, d);
                sub_root->left->parent = sub_root; //update parent of new node
                updateNodeHeight(sub_root->left);
                updateNodeBF(sub_root);
                rebalanceTreeAfterInsert(this, sub_root->left);
                if(!this->min||k < this->min->key) updateMin(sub_root->left);
                return;
            }
        }

        else if(k > sub_root->key){
            if (sub_root->right != nullptr)     sub_root = sub_root->right;
            else{
                sub_root->right = new AVLNode<Key, Data> (k, d);
                sub_root->right->parent = sub_root; //update parent of new node
                updateNodeHeight(sub_root->right);
                updateNodeBF(sub_root);
                rebalanceTreeAfterInsert(this, sub_root->right);
                return;
            }
        }

        else return; //k is already exist in the tree
    }
}

/*----------------------------------------------------------------------------*/

template<class Key, class Data>
void AVLTree<Key, Data>::removeNode(Key k){
    AVLNode<Key, Data> *toRemove = findNode(k);
    if (toRemove == nullptr) return; //k does not exist in the tree
    if (toRemove == this->min) updateMin(this->min->parent);
    if (isLeaf(toRemove)){
        leafRemove(this, toRemove);
        return;
    }
    if (toRemove->left == nullptr && toRemove->right != nullptr){
        onlyRightSonRemove(this, toRemove);
        return;
    }
    if (toRemove->left != nullptr && toRemove->right == nullptr){
        onlyLeftSonRemove(this, toRemove);
        return;
    }
    else{ //toRemove->left != nullptr && toRemove->right != nullptr
        AVLNode<Key, Data> *nextNode = findNextNode(toRemove);
        swapNodes(toRemove, nextNode);
        if(toRemove == this->root){
            nextNode->parent = nullptr;
            this->root = nextNode; //update root
        }
        //now k is a leaf or has only one son
        if (isLeaf(toRemove)){
            leafRemove(this, toRemove);
            return;
        }
        if (toRemove->left == nullptr && toRemove->right != nullptr){
            onlyRightSonRemove(this, toRemove);
            return;
        }
        if (toRemove->left != nullptr && toRemove->right == nullptr){
            onlyLeftSonRemove(this, toRemove);
            return;
        }return;
    }
}

/*----------------------------------------------------------------------------*/

template<class Key, class Data>
AVLNode<Key, Data> *AVLTree<Key, Data>::findNode(Key k)
{
    AVLNode<Key, Data> *candidate = this->root;
    while (candidate != nullptr){
        if (k < candidate->key)     candidate = candidate->left;
        else if (k > candidate->key)     candidate = candidate->right;
        else    return candidate; // k == candidate->key
    }return nullptr; //k is not found in the tree
}
/*----------------------------------------------------------------------------*/

template<class Key, class Data>
void AVLTree<Key, Data>::updateRoot(AVLNode<Key, Data> *n) {
    this->root = n;
    n->parent = nullptr;
}

/*----------------------------------------------------------------------------*/

template<class Key, class Data>
void AVLTree<Key, Data>::updateMin(AVLNode<Key, Data> *n) {
    this->min = n;
}

/*----------------------------------------------------------------------------*/

template<class Key, class Data>
void AVLTree<Key, Data>::LLrotate(AVLNode<Key, Data> *n){
    AVLNode<Key,Data> *temp=n->left;
    if(isRoot(n))   updateRoot(temp);
    else {
        temp->parent = n->parent;
        if (isLeftSon(n)) n->parent->left = temp;
        else n->parent->right = temp;
    }

    n->left = temp->right;
    if (temp->right != nullptr) temp->right->parent = n;

    temp->right = n;
    n->parent = temp;
    updateNodeHeight(n); //update changed heights
    updateNodeBF(n); //update relevant BF's
    updateNodeBF(temp);
    return;
}

/*----------------------------------------------------------------------------*/

template<class Key, class Data>
void AVLTree<Key, Data>::LRrotate(AVLNode<Key, Data> *n){
    AVLNode<Key,Data> *tempA = n->left;
    AVLNode<Key,Data> *tempB = tempA->right;
    if(isRoot(n))   updateRoot(tempB);
    else{
        tempB->parent = n->parent;
        if (isLeftSon(n)) n->parent->left = tempB;
        else n->parent->right = tempB;
    }
    if(!isLeaf(tempB)){
        n->left = tempB->right;
        if (tempB->right != nullptr) tempB->right->parent = n;
        tempA->right = tempB->left;
        if (tempB->left != nullptr) tempB->left->parent = tempA;
    }else{
        n->left = nullptr;
        tempA->right = nullptr;
    }
    tempB->right = n;
    n->parent = tempB;
    tempB->left = tempA;
    tempA->parent = tempB;
    n->height = n->height-2; //update changed heights
    tempA->height = tempA->height-1;
    tempB->height = tempB->height+1;
    updateNodeBF(n); //update relevant BF's
    updateNodeBF(tempA);
    updateNodeBF(tempB);
    return;
}

/*----------------------------------------------------------------------------*/

template<class Key, class Data>
void AVLTree<Key, Data>::RRrotate(AVLNode<Key, Data> *n){
    AVLNode<Key,Data> *temp=n->right;
    if(isRoot(n))   updateRoot(temp);
    else{
        temp->parent = n->parent;
        if(isRightSon(n))    n->parent->right = temp;
        else                n->parent->left = temp;
    }

    n->right = temp->left;
    if (temp->left != nullptr) temp->left->parent = n;

    temp->left = n;
    n->parent = temp;
    updateNodeHeight(n); //update changed heights
    updateNodeBF(n); //update relevant BF's
    updateNodeBF(temp);
    return;
}

/*----------------------------------------------------------------------------*/

template<class Key, class Data>
void AVLTree<Key, Data>::RLrotate(AVLNode<Key, Data> *n) {
    AVLNode<Key, Data> *tempA = n->right;
    AVLNode<Key, Data> *tempB = tempA->left;
    if (isRoot(n)) updateRoot(tempB);
    else {
        tempB->parent = n->parent;
        if (isRightSon(n)) n->parent->right = tempB;
        else n->parent->left = tempB;
    }
    if (!isLeaf(tempB)){
        n->right = tempB->left;
        if (tempB->left != nullptr) tempB->left->parent = n;
        tempA->left = tempB->right;
        if (tempA->left != nullptr) tempB->right->parent = tempA;
    }
    else{
        n->right = nullptr;
        tempA->left = nullptr;
    }

    tempB->left = n;
    n->parent = tempB;
    tempB->right = tempA;
    tempA->parent = tempB;
    n->height = n->height-2;        //update changed heights
    tempA->height = tempA->height-1;
    tempB->height = tempB->height+1;
    updateNodeBF(n);                //update relevant BF's
    updateNodeBF(tempA);
    updateNodeBF(tempB);
    return;
}

/*----------------------------------------------------------------------------*/

template<class Key, class Data>
void AVLTree<Key, Data>::clearTree(){
    if (this->root == nullptr) return;

    if (this->root->left != nullptr){
        clearSubTree(this->root->left);
        this->root->left = nullptr;
    }

    if (this->root->right != nullptr){
        clearSubTree(this->root->right);
        this->root->right = nullptr;
    }

    delete this->root;
    this->root = nullptr;
}

/*----------------------------------------------------------------------------*/

template<class Key, class Data>
void AVLTree<Key, Data>::printTree()
{
    if (this->root == nullptr) return;

    if (this->root->left != nullptr){
        printSubTree(this->root->left);
    }

    cout<<this->root->key<<" BF: "<<this->root->bf<<" Height: "<<this->root->height<<endl;

    if (this->root->right != nullptr){
        printSubTree(this->root->right);
    }
}

/*----------------------------------------------------------------------------*/

/*
template<class Key, class Data>
AVLNode<Key, Data> *
AVLTree<Key, Data>::nextNodeInOrder(AVLNode<Key, Data> *current) {
    if (!this->lastInOrder){
        if (!current->parent){
            this->lastInOrder = current;
            return current->parent;
        }
        return nullptr;
    }
    else if (!current->right){
        if (current->right->key != this->lastInOrder->key){
            this->lastInOrder = current;
            if (!current->left){
                current->right;
            }
        }
        else{
            if(current->parent != nullptr){
                if (current->parent->key != this->lastInOrder->key){
                    this->lastInOrder = current;
                    return current->parent;
                }
            }
            this->lastInOrder = nullptr;
            return nullptr;
        }
    }
    if (current->left != nullptr){
        if (current->left->key == this->lastInOrder->key){
            this->lastInOrder = current;
            if (!this->lastInOrder->right){
                return this->lastInOrder->right;
            }
            return this->lastInOrder->parent;
        }
    }
    return nullptr;
}
 */

/*------------------------------------------------------------------------------
--------------------------------static functions--------------------------------
------------------------------------------------------------------------------*/

template<class Key, class Data>
void rebalanceTreeAfterInsert(AVLTree<Key,Data> *tree, AVLNode<Key,Data> *n)
{
    while (!isRoot(n)){
        if (n->parent->height>=n->height+1) return;

        updateNodeHeight(n->parent);
        updateNodeBF(n->parent);

        if (n->parent->bf == 2 && n->parent->left != nullptr
            && n->parent->left->bf >= 0){
            tree->LLrotate(n->parent);
            return;
        }
        if (n->parent->bf == 2 && n->parent->left != nullptr
            && n->parent->left->bf == -1){
            tree->LRrotate(n->parent);
            return;
        }
        if (n->parent->bf == -2 && n->parent->right != nullptr
            && n->parent->right->bf == 1){
            tree->RLrotate(n->parent);
            return;
        }
        if (n->parent->bf == -2 && n->parent->right != nullptr
            && n->parent->right->bf <= 0){
            tree->RRrotate(n->parent);
            return;
        }
        n = n->parent;
    }
}

/*----------------------------------------------------------------------------*/

template<class Key, class Data>
void rebalanceTreeAfterRemove(AVLTree<Key,Data> *tree, AVLNode<Key,Data> *n)
{
    while (n != nullptr){
        int old_n_height = n->height;

        if (n->bf == 2 && n->left != nullptr
            && n->left->bf >= 0){
            tree->LLrotate(n);
            if (old_n_height == n->height)  return;
        }
        if (n->bf == 2 && n->left != nullptr
            && n->left->bf == -1){
            tree->LRrotate(n);
            if (old_n_height == n->height)  return;
        }
        if (n->bf == -2 && n->right != nullptr
            && n->right->bf == 1){
            tree->RLrotate(n);
            if (old_n_height == n->height)  return;
        }
        if (n->bf == -2 && n->right != nullptr
            && n->right->bf <= 0){
            tree->RRrotate(n);
            if (old_n_height == n->height)  return;
        }
        n = n->parent;
        if (n != nullptr){
            updateNodeHeight(n);
            updateNodeBF(n);
        }
    }
}

/*----------------------------------------------------------------------------*/

//check if n is a root
template<class Key, class Data>
static bool isRoot (AVLNode<Key, Data> *n)
{
    return (n->parent == nullptr);
}

/*----------------------------------------------------------------------------*/

//check if n is a leaf
template<class Key, class Data>
static bool isLeaf (AVLNode<Key, Data> *n)
{
    return (n->left == nullptr && n->right == nullptr);
}

/*----------------------------------------------------------------------------*/

//check if node is the left son of his parent
template<class Key, class Data>
static bool isLeftSon (AVLNode<Key, Data> *n)
{
    return (!isRoot(n) && n->parent->left == n);
}

/*----------------------------------------------------------------------------*/

//check if node is the right son of his parent
template<class Key, class Data>
static bool isRightSon (AVLNode<Key, Data> *n)
{
    return (!isRoot(n) && n->parent->right == n);
}

/*----------------------------------------------------------------------------*/

//remove a leaf node
template<class Key, class Data>
static void leafRemove(AVLTree<Key, Data> *tree, AVLNode<Key, Data> *toRemove)
{
    if (isRoot(toRemove)){
        tree->root = nullptr;
        delete(toRemove);
        return;
    }

    else{
        if (isLeftSon(toRemove))    toRemove->parent->left = nullptr;

        if (isRightSon(toRemove))   toRemove->parent->right = nullptr;

        AVLNode<Key, Data> *to_rebalance = toRemove->parent;
        delete(toRemove);
        while (to_rebalance) {
            updateNodeHeight(to_rebalance);
            updateNodeBF(to_rebalance);
            rebalanceTreeAfterRemove(tree, to_rebalance);
            to_rebalance = to_rebalance->parent;
        }
    }
}

/*----------------------------------------------------------------------------*/

//remove node that has only right son
template<class Key, class Data>
static void onlyRightSonRemove(AVLTree<Key, Data> *tree,
                               AVLNode<Key,Data> *toRemove){
    if (isRoot(toRemove)){
        tree->updateRoot(toRemove->right);
        delete(toRemove);
        return;
    }

    else{
        if (isLeftSon(toRemove)){
            toRemove->parent->left = toRemove->right;
            toRemove->right->parent = toRemove->parent;
        }
        if (isRightSon(toRemove)){
            toRemove->parent->right = toRemove->right;
            toRemove->right->parent = toRemove->parent;
        }
        AVLNode<Key, Data> *to_rebalance = toRemove->right;
        delete(toRemove);
        updateNodeHeight(to_rebalance);
        updateNodeBF(to_rebalance);
        rebalanceTreeAfterRemove(tree, to_rebalance);
    }
}

/*----------------------------------------------------------------------------*/

//remove node that have only left son
template<class Key, class Data>
static void onlyLeftSonRemove(AVLTree<Key, Data> *tree,
                              AVLNode<Key, Data> *toRemove)
{
    if (isRoot(toRemove)){
        tree->updateRoot(toRemove->left);
        delete(toRemove);
        return;
    }

    else{
        if (isLeftSon(toRemove)){
            toRemove->parent->left = toRemove->left;
            toRemove->left->parent = toRemove->parent;
        }
        if (isRightSon(toRemove)){
            toRemove->parent->right = toRemove->left;
            toRemove->left->parent = toRemove->parent;
        }
        AVLNode<Key, Data> *to_rebalance = toRemove->left;
        delete(toRemove);
        updateNodeHeight(to_rebalance);
        updateNodeBF(to_rebalance);
        rebalanceTreeAfterRemove(tree, to_rebalance);
    }
}

/*----------------------------------------------------------------------------*/

//find the leftmost son of root
template<class Key, class Data>
static AVLNode<Key, Data> * findLeftmostSon(AVLNode<Key, Data> *root)
{
    if (root->left == nullptr) return root;
    return findLeftmostSon(root->left);
}

/*----------------------------------------------------------------------------*/

//n is not the biggest node in tree!
//find the next node in the tree
template<class Key, class Data>
static AVLNode<Key, Data> * findNextNode(AVLNode<Key, Data> *n){
    if (n->right != nullptr)    return findLeftmostSon(n->right);

    else{//n->right == nullptr
        //if (isRoot(n))          return nullptr;
        if (isLeftSon(n))  return n->parent;
        else     return n->parent->parent;//isRightSon(n)
    }
}

/*----------------------------------------------------------------------------*/

//find the next node post order in the tree
template<class Key, class Data>
static AVLNode<Key, Data> * nextNodePostOrder(AVLNode<Key, Data> *n){
    if (n->parent->right != nullptr)    return findLeftmostSon(n->parent->right);
    return n->parent;
}



/*----------------------------------------------------------------------------*/

template<class Key, class Data>
static void swapNodes(AVLNode<Key, Data> *n1, AVLNode<Key, Data> *n2){
    if ((n1->right != nullptr && n1->right->key == n2->key) ||
        (n1->left != nullptr && n1->left->key == n2->key)) //n1 is parent of n2
    {
        swapSonAndParent(n2, n1);
        return;
    }

    else if ((n2->right != nullptr && n2->right->key == n1->key) ||
             (n2->left != nullptr && n2->left->key == n1->key)) //n2 is parent of n1
    {
        swapSonAndParent(n1, n2);
        return;
    }

    swapSeparateNodes(n1, n2);
}

/*----------------------------------------------------------------------------*/

template<class Key, class Data>
static void swapSonAndParent(AVLNode<Key, Data> *son, AVLNode<Key,
        Data> *parent)
{
    int son_is_left_son_of_parent = isLeftSon(son);

    if (son->left != nullptr)   son->left->parent = parent;
    if (son->right != nullptr)  son->right->parent = parent;
    if (parent->parent != nullptr){
        if (isLeftSon(parent))   parent->parent->left = son;
        else    parent->parent->right = son;
    }
    if (isRightSon(son) && parent->left != nullptr) parent->left->parent = son;
    else if (isLeftSon(son)&&parent->right!=nullptr)parent->right->parent = son;

    //backup son pointers
    AVLNode<Key, Data> *son_left = son->left;
    AVLNode<Key, Data> *son_right = son->right;
    int son_height = son->height;
    int son_bf = son->bf;

    //update son pointers
    son->parent = parent->parent;
    if (son_is_left_son_of_parent){
        son->left = parent;
        son->right = parent->right;
    }
    else{ //son_is_right_son_of_parent
        son->left = parent->left;
        son->right = parent;
    }
    son->height = parent->height;
    son->bf = parent->bf;

    //update parent pointers
    parent->parent = son;
    parent->left = son_left;
    parent->right = son_right;
    parent->height = son_height;
    parent->bf = son_bf;
}

/*----------------------------------------------------------------------------*/

template<class Key, class Data>
static void swapSeparateNodes(AVLNode<Key, Data> *n1, AVLNode<Key,
        Data> *n2)
{
    if (n1->left != nullptr)    n1->left->parent = n2;

    if (n1->right != nullptr)   n1->right->parent = n2;

    if (n1-> parent != nullptr){
        if (isLeftSon(n1))  n1->parent->left = n2;
        else    n1->parent->right = n2;
    }

    if (n2->left != nullptr)    n2->left->parent = n1;

    if (n2->right != nullptr)   n2->right->parent = n1;

    if (n2-> parent != nullptr){
        if (isLeftSon(n2))      n2->parent->left = n1;
        else     n2->parent->right = n1;
    }

    //backup n1 pointers
    AVLNode<Key, Data> *n1_parent = n1->parent;
    AVLNode<Key, Data> *n1_left = n1->left;
    AVLNode<Key, Data> *n1_right = n1->right;
    int n1_height = n1->height;
    int n1_bf = n1->bf;

    //update n1 pointers
    n1->parent = n2->parent;
    n1->left = n2->left;
    n1->right = n2->right;
    n1->height = n2->height;
    n1->bf = n2->bf;

    //update n2 pointers
    n2->parent = n1_parent;
    n2->left = n1_left;
    n2->right = n1_right;
    n2->height = n1_height;
    n2->bf = n1_bf;
}

/*----------------------------------------------------------------------------*/

//update the height of the vortex n
template<class Key, class Data>
static void updateNodeHeight(AVLNode<Key, Data> *n){
    if (n->left == nullptr && n->right == nullptr)  n->height = 0;

    else if (n->left == nullptr && n->right != nullptr)
        n->height = 1+n->right->height;

    else if (n->left != nullptr && n->right == nullptr)
        n->height = 1+n->left->height;

    else    n->height = 1 + max(n->left->height, n->right->height);
}

/*----------------------------------------------------------------------------*/

//update the BF of n and all his ancestors
template<class Key, class Data>
static void updateNodeBF(AVLNode<Key, Data> *n)
{
    int left_son_height;
    int right_son_height;

    if (n->left == nullptr && n->right == nullptr) {
        left_son_height = 0;
        right_son_height = 0;
    } else if (n->left == nullptr && n->right != nullptr) {
        left_son_height = 0;
        right_son_height = 1 + n->right->height;
    } else if (n->right == nullptr && n->left != nullptr) {
        right_son_height = 0;
        left_son_height = 1 + n->left->height;
    } else {
        left_son_height = 1 + n->left->height;
        right_son_height = 1 + n->right->height;
    }
    n->bf = left_son_height - right_son_height;
}

/*----------------------------------------------------------------------------*/

template<class Key, class Data>
static void clearSubTree(AVLNode<Key, Data> *root)
{
    if (root->left != nullptr){
        clearSubTree(root->left);
        root->left = nullptr;
    }

    if (root->right != nullptr){
        clearSubTree(root->right);
        root->right = nullptr;
    }

    delete root;
}

/*----------------------------------------------------------------------------*/

template<class Key, class Data>
static void printSubTree(AVLNode<Key, Data> *root)
{
    if (root->left != nullptr)  printSubTree(root->left);

    cout<<root->key<<" BF: "<<root->bf<<" Height: "<<root->height<<endl;

    if (root->right != nullptr)  printSubTree(root->right);
}

#endif //HW1Wet_AVLTREE_H
#endif //WET1_AVLTREE_H