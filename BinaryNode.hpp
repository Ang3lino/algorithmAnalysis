#ifndef BINARY_NODE_H
#define BINARY_NODE_H

template <typename T>
struct BinaryNode {

    T data;
    BinaryNode *left;
    BinaryNode *right;
    BinaryNode *parent;

    BinaryNode (T data, BinaryNode *left, BinaryNode *right, BinaryNode *parent);
	BinaryNode (T data);
	BinaryNode ();
};

#endif