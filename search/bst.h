#ifndef bst_h
#define bst_h

#include <iostream>
#include <vector>
#include <stack>

using namespace std;

/** Arbol binario de busqueda generico */
template < typename T >
class bst {

private:
    /** Nodo binario interno de la clase */
    struct BinaryNode {
        T data;
        BinaryNode *left, *right, *parent;

        // constructores con iniciadores miembro
        BinaryNode(T mdata, BinaryNode *mleft, BinaryNode *mright, BinaryNode *mparent) 
            : data{ mdata }, left{ mleft }, right{ mright }, parent { mparent } { } 

        BinaryNode(T mdata) 
            : data{ mdata }, left{ nullptr }, right{ nullptr }, parent { nullptr } { } 

        BinaryNode() 
            : left{ nullptr }, right{ nullptr }, parent { nullptr } { } 
    };

    BinaryNode *root;

    /** Agrega un elemento al arbol de forma iterativa, no se inserta el valor en caso de 
     * estar repetido. */
    BinaryNode *add(BinaryNode *root, T data) {
        if (root == nullptr) { // no se ha insertado nada previamente
            root = new BinaryNode(data);
        } else {
            BinaryNode *parent, *current = root; // empezamos estando en la raiz del arbol
            BinaryNode *toInsert = new BinaryNode(data); 
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

    /** Imprimimos los valores en inorden */
    void inorder(BinaryNode *self) {
        if (self == nullptr) return;
        inorder(self->left);
        cout << self->data << "\n";
        inorder(self->right);
    }

    /** Obtenemos el valor minimo del arbol, el cual se encuentra en la izquierda del nodo
     * un paso antes de encontrar un valor nulo. */
    BinaryNode *min(BinaryNode *root) {
        if (root != nullptr)
            while (root->left != nullptr)
                root = root->left;
        return root;
    }


    inline bool leaf(BinaryNode *node) {
        return node->right == nullptr && node->left == nullptr;
    }

    int numberOfNodes(BinaryNode *root) {
        if (root == nullptr) return 0;
        return numberOfNodes(root->left) + numberOfNodes(root->right) + 1;
    }

    void remove(BinaryNode *&self, BinaryNode *parent, const T &x) {
        if (self == nullptr) return; // no encontrado
        if (x < self->data) remove(self->left, self, x);
        else if (self->data < x) remove(self->right, self, x);
        // encontrado!
        else if (self->left != nullptr && self->right != nullptr) { // dos hijos
            self->data = min(self->right)->data;
            remove(self->right, self, self->data); // eliminamos el sucesor duplicado
        } else { // el nodo tiene a lo mas 1 hijo
            BinaryNode *old = self; // guardamos la direccion del nodo a ser borrado
            self = (self->left != nullptr) ? self->left: self->right;
            if (self != nullptr) self->parent = parent;
            delete old;
        }
    }

    /** Destruimos el arbol recorriendolo en postorden, es llamada por el destructor */
    void destroy(BinaryNode *&root) {
        if (root != nullptr) {
            destroy(root->left);
            destroy(root->right);
            delete root;
        }
        root = nullptr;
    }

    /** Funcion de busqueda la cual determina en O (n log n) si un valor se encuentra */
    bool contains(const BinaryNode *self, const T x) {
        if (self == nullptr) return false; // hemos llegado al fondo y nunca nos lo encontramos
        if (self->data == x) return true; // encontrado !
        if (x < self->data) return contains(self->left, x);
        return contains(self->right, x); // un valor x > self->data 
    }

    /** 
     * Construimos un arbol dado un vector ordenado de manera eficiente.
     */
    BinaryNode *sorted_to_bst(vector<T> &sorted, const int left, const int right) {
        if (left > right) return nullptr;
        int mid = left + (right - left) / 2; 
        BinaryNode *node = new BinaryNode(sorted[mid]);
        node->left = sorted_to_bst(sorted, left, mid - 1);
        node->right = sorted_to_bst(sorted, mid + 1, right);
        return node;
    }

public:

    inline void sorted_to_bst(vector<T> &sorted) {
        root = sorted_to_bst(sorted, 0, sorted.size() - 1);
    }

    inline bool contains(const T toFind) {
        return contains(root, toFind);
    }

    inline void remove(T x) {
        remove(root, nullptr, x);
    }

    inline ~bst() { 
        destroy(root);
    }

    inline bst() { root = nullptr; } 

    bst(vector<T> v) {
        root = nullptr;
        for (T x: v) 
            root = add(root, x);
    }

    int numberOfNodes() {
        return numberOfNodes(root);
    }

    inline void add(T data) { 
        root = add(root, data); // actualizamos la nueva raiz 
    }

    inline void inorder() { inorder(root); }

    void sort(vector<T> &v) {
        for (T x: v) add(x);

        // recorrido en inorden
        stack<BinaryNode *> s;
        BinaryNode *curr = root;
        T data;
        int i = 0;
        while (true) {
            if (curr != nullptr) {
                s.push(curr);
                curr = curr->left;
            } else {
                if (s.empty()) break; // el nodo actual es nulo y la pila esta vacia
                curr = s.top();  
                s.pop();
                data = curr->data;
                v[i++] = data;
                curr = curr->right;
            }
        }
    }

};

#endif 