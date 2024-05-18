#include <stdio.h>
#include <stdlib.h>

typedef struct huffman_node{
	char character;
	int freq;
	int flag;
	struct huffman_node* left;
	struct huffman_node* right;
	struct huffman_node* parent;
}huffman_node;

typedef huffman_node * huffman_tree;

huffman_node* join_trees(huffman_node* h1 , huffman_node* h2);
