#ifndef bst_h
#define bst_h

#include <iostream>
#include <vector>
#include <stack>

using namespace std;

// clase de arbol binario de busqueda generico
template < typename T >
class bst {

private:
    struct BinaryNode { // /estructura interna
        T data;
        BinaryNode *left, *right, *parent; // punteros recursivos

        // constructores con iniciadores miembro
        BinaryNode(T mdata, BinaryNode *mleft, BinaryNode *mright, BinaryNode *mparent) 
            : data{ mdata }, left{ mleft }, right{ mright }, parent { mparent } { } 

        BinaryNode(T mdata) 
            : data{ mdata }, left{ nullptr }, right{ nullptr }, parent { nullptr } { } 

        BinaryNode() 
            : left{ nullptr }, right{ nullptr }, parent { nullptr } { } 
    };

    BinaryNode *root;

    // agrega un elemento al bst
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

    // imprime en inorden
    void inorder(BinaryNode *self) {
        if (self == nullptr) return;
        inorder(self->left);
        cout << self->data << ", ";
        inorder(self->right);
    }


    // retorna el puntero al valor minimo
    BinaryNode *min(BinaryNode *root) {
        if (root != nullptr)
            while (root->left != nullptr)
                root = root->left;
        return root;
    }

    // determina si el nodo es hoja
    inline bool leaf(BinaryNode *node) {
        return node->right == nullptr && node->left == nullptr;
    }

    // retorna el numero de nodos
    int numberOfNodes(BinaryNode *root) {
        if (root == nullptr) return 0;
        return numberOfNodes(root->left) + numberOfNodes(root->right) + 1;
    }

    // removemos un nodo, es una funcion miembro de ayuda
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

    // destruimos el arbol de manera recursiva
    void destroy(BinaryNode *&root) {
        if (root != nullptr) {
            destroy(root->left);
            destroy(root->right);
            delete root;
        }
        root = nullptr;
    }

    // funcion booleana que determina si el elemento se encuentra en el nodo
    bool contains(const BinaryNode *self, const T x) {
        if (self == nullptr) return false;
        if (self->data == x) return true;
        if (x < self->data) return contains(self->left, x);
        else return contains(self->right, x);
    }

public:

    // retorna verdad en caso de el valor este en el nodo
    bool contains(const T toFind) {
        return contains(root, toFind);
    }

    // removemos el valor del nodo
    inline void remove(T x) {
        remove(root, nullptr, x);
    }

    // destructor de la clase que llama a la funcion miembro destroy
    inline ~bst() { 
        destroy(root);
    }

    // constructor, al principio la raiz es nuls
    inline bst() { root = nullptr; } 

    // dado un vector generico rellenaremos el arbol, se asume que de alguna manera cada elemento
    // tiene los operadores de comparacion ya definidos
    bst(vector<T> v) {
        root = nullptr;
        for (T x: v) 
            root = add(root, x);
    }

    // regresa el numero de nodos
    int numberOfNodes() {
        return numberOfNodes(root);
    }

    // agrega un valor al arbol, si es repetido no se agrega
    inline void add(T data) { 
        root = add(root, data); // actualizamos la nueva raiz 
    }

    // imprime el recorrido en inorden del arbol
    inline void inorder() { inorder(root); }

    // dado un vector, este se ordena usando el algoritmo Treesort
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