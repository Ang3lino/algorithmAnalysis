#include "BinarySearchTree.hpp"

//private:
    template <typename T>
    BinaryNode<T> * BinarySearchTree<T>::add (BinaryNode<T> *root, T data) {
        if (root == nullptr) { // no se ha insertado nada previamente
            root = new BinaryNode<T>(data);
        } else {
            BinaryNode<T> *parent, *current = root; // empezamos estando en la raiz del arbol
            BinaryNode<T> *toInsert = new BinaryNode<T>(data); 
            bool lastLeft = false; // nos indicara si somos izq. o der. de nuestro padre
            while (current != nullptr) {
                parent = current;
                lastLeft = data < current->data;
                if (data < current->data) current = current->left;
                else if (data > current->data) current = current->right;
                else return root; // no permitimos duplicados
            }

            // empezamos a conectar el nuevo nodo
            toInsert->parent = parent;
            if (lastLeft) parent->left = toInsert; 
            else parent->right = toInsert;
        }
        return root;
    }

    template <typename T>
    void BinarySearchTree<T>::inorder (BinaryNode<T> *self) {
        if (self == nullptr) return;
        inorder(self->left);
        std::cout << self->data << ", ";
        inorder(self->right);
    }

    template <typename T>
    BinaryNode<T> * BinarySearchTree<T>::min (BinaryNode<T> *root) {
        if (root != nullptr)
            while (root->left != nullptr)
                root = root->left;
        return root;
    }

    template <typename T>
    inline bool BinarySearchTree<T>::leaf (BinaryNode<T> *node) {
        return node->right == nullptr && node->left == nullptr;
    }

    template <typename T>
    int BinarySearchTree<T>::numberOfNodes (BinaryNode<T> *root) {
        if (root == nullptr) return 0;
        return numberOfNodes(root->left) + numberOfNodes(root->right) + 1;
    }

    template <typename T>
    void BinarySearchTree<T>::remove (BinaryNode<T> *&self, BinaryNode<T> *parent, const T &x) {
        if (self == nullptr) return; // no encontrado
        if (x < self->data) remove(self->left, self, x);
        else if (self->data < x) remove(self->right, self, x);
        // encontrado!
        else if (self->left != nullptr && self->right != nullptr) { // dos hijos
            self->data = min(self->right)->data;
            remove(self->right, self, self->data); // eliminamos el sucesor duplicado
        } else { // el nodo tiene a lo mas 1 hijo
            BinaryNode<T> *old = self; // guardamos la direccion del nodo a ser borrado
            self = (self->left != nullptr) ? self->left: self->right;
            if (self != nullptr) self->parent = parent;
            delete old;
        }
    }

    template <typename T>
    void BinarySearchTree<T>::destroy (BinaryNode<T> *&root) {
        if (root != nullptr) {
            destroy(root->left);
            destroy(root->right);
            delete root;
        }
        root = nullptr;
    }

    template <typename T>
    bool BinarySearchTree<T>::contains (const BinaryNode<T> *self, const T x) {
        if (self == nullptr) return false;
        if (self->data == x) return true;
        if (x < self->data) return contains(self->left, x);
        else return contains(self->right, x);
    }

//public:

    template <typename T>
    bool BinarySearchTree<T>::contains (const T toFind) {
        return contains(root, toFind);
    }

    template <typename T>
    inline void BinarySearchTree<T>::remove (T x) {
        remove(root, nullptr, x);
    }

    template <typename T>
    inline BinarySearchTree<T>::~BinarySearchTree () { 
        destroy(root);
    }

    template <typename T>
    inline BinarySearchTree<T>::BinarySearchTree () { 
        root = nullptr;
    } 

    template <typename T>
    BinarySearchTree<T>::BinarySearchTree (std::vector<T> v) {
        root = nullptr;
        for (T x: v) 
            root = add(root, x);
    }

    template <typename T>
    int BinarySearchTree<T>::numberOfNodes() {
        return numberOfNodes(root);
    }

    template <typename T>
    inline void BinarySearchTree<T>::add (T data) { 
        root = add(root, data); // actualizamos la nueva raiz 
    }

    template <typename T>
    inline void BinarySearchTree<T>::inorder () { 
        inorder(root);
    }

    template <typename T>
    void BinarySearchTree<T>::sort (std::vector<T> &v) {
        for (T x: v) 
            add(x);

        // recorrido en inorden
        std::stack<BinaryNode<T> *> s;
        BinaryNode<T> *curr = root;
        T data;
        int i = 0;
        while (true) {
            if (curr != nullptr) {
                s.push(curr);
                curr = curr->left;
            } else {
                if (s.empty()) 
                    break; // el nodo actual es nulo y la pila esta vacia
                curr = s.top();  
                s.pop();
                data = curr->data;
                v[i++] = data;
                curr = curr->right;
            }
        }
    }