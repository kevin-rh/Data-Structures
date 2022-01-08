#ifndef _AVLTREE_HPP
#define _AVLTREE_HPP

#include <cstdlib>
template <class T>
class Node{
public:
    typedef T value_type;
    typedef value_type &reference_type;
    typedef const value_type &const_reference_type;
    typedef value_type *pointer_type;
    typedef const value_type *const_pointer_type;

    value_type val;
    int key;
    Node *left, *right;
    int height;

    Node(): left(NULL), right(NULL), height(1){}
    Node(int key, value_type val): key(key), val(val), left(NULL), right(NULL), height(1){}
	Node<T>& operator= (const Node<T>& node){
        this->val = node.val;
        this->key = node.key;
        this->left = node.left;
        this->right = node.right;
        this->height = node.height;
        return *this;
    }
    ~Node(){}
};


template <typename T>
class AvlTree{
private:
    typedef Node<T> node_type;
    typedef node_type *node_pointer;
public:
    typedef typename node_type::value_type value_type;
    typedef typename node_type::reference_type reference_type;
    typedef typename node_type::const_reference_type const_reference_type;
private:
    node_pointer root;
    value_type deleted;
    bool found;

    int max(int a, int b){
        return (a > b)? a : b;
    }
    node_pointer update_rebalance(node_pointer node);
    node_pointer rightRotate(node_pointer y);
	node_pointer leftRotate(node_pointer x);
    int balanced(node_pointer N) const;
    node_pointer insert(node_pointer node, int key, value_type val);
    node_pointer Delete(node_pointer cur, int key, bool store);
	int height(node_pointer N) const;
    void preOrder(node_pointer root);
    void inOrder(node_pointer root);
    void postOrder(node_pointer root);
public:
    AvlTree();
    void insert (value_type val, int user_id);
    value_type search_delete(int user_id);
    void reset();
    bool is_search_found()const;
    void preOrder();
    void inOrder();
    void postOrder();
};

template <class T>
AvlTree<T>::AvlTree(): root(NULL), found(false){}

template <class T>
int AvlTree<T>::height(node_pointer N)const{
    if (N == NULL)
        return 0;
    return N->height;
}

template <class T>
typename AvlTree<T>::node_pointer AvlTree<T>::rightRotate(node_pointer y){
    node_pointer x = y->left;
    node_pointer tmp = x->right;
    x->right = y;
    y->left = tmp;
    y->height = max(height(y->left), height(y->right))+1;
    x->height = max(height(x->left), height(x->right))+1;
    return x;
}

template <class T>
typename AvlTree<T>::node_pointer AvlTree<T>::leftRotate(node_pointer x){
    node_pointer y = x->right;
    node_pointer tmp = y->left;
    y->left = x;
    x->right = tmp;
    x->height = max(height(x->left), height(x->right))+1;
    y->height = max(height(y->left), height(y->right))+1;
    return y;
}

template <class T>
int AvlTree<T>::balanced(node_pointer N)const{
    if (N == NULL)
        return 0;
    return height(N->right) - height(N->left);
}

template <class T>
void AvlTree<T>::insert (value_type val, int user_id){
    this->root = this->insert(this->root, user_id, val);
}

template <class T>
typename AvlTree<T>::value_type AvlTree<T>::search_delete(int user_id){
    this->root = this->Delete(this->root, user_id, true);
    //std::cout<<user_id<<".\n";
    //std::system("pause");
    typename AvlTree<T>::value_type tmp = NULL;
    if(this->is_search_found()){
        tmp = deleted;
    }else{
        //std::cout<<"NOT FOUND: "<<user_id<<".\n"; // DEBUG!
    }
    this->reset();
    return tmp;
}

template <class T>
void AvlTree<T>::reset(){
    this->found = false;
    this->deleted = NULL;
}

template <class T>
bool AvlTree<T>::is_search_found()const{
    return this->found;
}

template <class T>
typename AvlTree<T>::node_pointer AvlTree<T>::update_rebalance(node_pointer node){
    // Update the heights
    node->height = max(height(node->left), height(node->right))+1;

    // Get the balance factor of this ancestor
    int n = balanced(node);

    // LL Case
    if(n<-1 && balanced(node->left)<=0){
        // std::cout<<"LL\n";
        node = rightRotate(node);
    }
    // RR Case
    if(n>1 && balanced(node->right)>=0){
        // std::cout<<"RR\n";
        node = leftRotate(node);
    }
    // LR Case
    if(n<-1 && balanced(node->left)>0){
        // std::cout<<"LR\n";
        node->left = leftRotate(node->left);
        node = rightRotate(node);
    }
    // RL Case
    if(n>1 && balanced(node->right)<0){
        // std::cout<<"RL\n";
        node->right = rightRotate(node->right);
        node = leftRotate(node);
    }
    return node;
}

template <class T>
typename AvlTree<T>::node_pointer AvlTree<T>::Delete(node_pointer root, int key, bool store){
    if (root == NULL)
        return root;
    if (key < root->key)
        root->left = Delete(root->left, key, store);
    else if(key > root->key)
        root->right = Delete(root->right, key, store);
    else // data found
    {
        // std::cout<<"("<<root->key<<")";
        if(store) {
            deleted = root->val; //copy data if its exist
            found = true;
        }
        if((root->left == NULL) || (root->right == NULL)){
            node_pointer tmp = (root->left)? root->left : root->right;
            if (tmp == NULL){ //dead end
                // if(store) std::cout<< "leaf\n";
                tmp = root;
                root = NULL;
            }else{
                // if(store) std::cout<< "1 child\n";
                *root = *tmp;
            }
            delete tmp;
        }else{
            // std::cout<<"2 child\n";
            node_pointer tmp = root->right;
            while(tmp->left!=NULL) tmp = tmp->left;
            root->val = tmp->val;
            root->key = tmp->key;
            root->right = Delete(root->right, tmp->key, false);
        }
    }
    if (root == NULL)
    return root;
    
    return update_rebalance(root);
}
template <class T>
void AvlTree<T>::preOrder(){
    preOrder(root);
}

template <class T>
void AvlTree<T>::inOrder(){
    inOrder(root);
}

template <class T>
void AvlTree<T>::postOrder(){
    postOrder(root);
}
template <class T>
void AvlTree<T>::preOrder(node_pointer root){
    if(root==NULL) return;
    preOrder(root->left);
    preOrder(root->right);
    std::cout << (root->key) << " ";
}

template <class T>
void AvlTree<T>::inOrder(node_pointer root){
    if(root==NULL) return;
    std::cout<<"(";
    inOrder(root->left);
    std::cout << (root->key) << " ";
    inOrder(root->right);
    std::cout<<")";
}

template <class T>
void AvlTree<T>::postOrder(node_pointer root){
    if(root==NULL) return;
    std::cout << (root->key) << " ";
    postOrder(root->left);
    postOrder(root->right);
}

template <class T>
typename AvlTree<T>::node_pointer AvlTree<T>::insert(node_pointer root, int key, value_type val){
    if(root == NULL){
        root = new node_type(key, val);
        return root;
    }else{ // data found
        if(key > root->key){
            root->right = insert(root->right, key, val);
        }else{
            root->left = insert(root->left, key, val);
        }
    }
    return update_rebalance(root);
}

#endif
/*
// Driver Code
int main()
{
    node_pointer node = NULL;
    string cmd;
    while(cin >> cmd){
        if(cmd == "insert"){
            int x;
            cin >> x;
            root = insert(root, x);
//            preOrder(root), cout<<'\n',inOrder(root);
        }else if(cmd == "preorder"){
            preOrder(root);
        }else if(cmd == "inorder"){
            inOrder(root);
        }else if(cmd == "exit"){
            return 0;
        }
    }


    return 0;
}

// This code is contributed by
// rathbhupendra

*/
