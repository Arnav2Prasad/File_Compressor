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

huffman_node* join_trees(huffman_node* h1 , huffman_node* h2){
	huffman_node* nn = (huffman_node *)malloc(sizeof(huffman_node));
	if (nn){
		nn->flag=0;
		nn->character ='\0';
		nn->freq = h1->freq + h2->freq;
		nn->left = h1;
		nn ->right = h2;
		nn->parent=NULL;
		h1->parent = nn;
		h2->parent=nn;
	}
	return nn;
}

