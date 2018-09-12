#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include <stack>
#include <vector>
#include <iostream>
#include "BinaryNode.hpp"

template <typename T>
class BinarySearchTree {

    private:

        //**************************
        //********ATRIBUTOS
        //**************************

        BinaryNode<T> *root;

        //**************************
        //*********MÉTODOS
        //**************************

        /**/
        BinaryNode<T> * add (BinaryNode<T> *root, T data);
        /**/
        void inorder (BinaryNode<T> *self);
        /**/
        BinaryNode<T> * min (BinaryNode<T> *root);
        /**/
        inline bool leaf (BinaryNode<T> *node);
        /**/
        int numberOfNodes (BinaryNode<T> *root);
        /**/
        void remove (BinaryNode<T> *&self, BinaryNode<T> *parent, const T &x);
        /**/
        void destroy (BinaryNode<T> *&root);
        /**/
        bool contains (const BinaryNode<T> *self, const T x);

    public:

        /**/
        bool contains (const T toFind);
        /**/
        inline void remove (T x);
        /**/
        inline ~BinarySearchTree ();
        /**/
        inline BinarySearchTree ();
        /**/
        BinarySearchTree (std::vector<T> v);
        /**/
        int numberOfNodes ();
        /**/
        inline void add (T data);
        /**/
        inline void inorder ();
        /**/
        void sort (std::vector<T> &v);
};

#endif