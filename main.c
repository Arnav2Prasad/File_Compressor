#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "stack.h"
#include "min_heap.h"
#include "huffman_tree.h"

#define ASCII_SIZE 256

typedef struct encoding_table_node{
	//char character;
	long long int frequency;
	char * enc_key;
	huffman_node* huff_ptr;
}encoding_table_node;

typedef struct encoding_table{
	struct encoding_table_node* enc_arr;
	long long int len;
}encoding_table;

encoding_table * get_character_freq_fromfile(int fd) {
	encoding_table *et;
	et = (encoding_table * ) malloc(sizeof(encoding_table));
	et->len = ASCII_SIZE;
	et->enc_arr = (encoding_table_node *) malloc(sizeof(encoding_table_node) * et->len);
	int i;
	for(i = 0; i < ASCII_SIZE; i++) {
		et->enc_arr[i].frequency = 0;
		et->enc_arr[i].huff_ptr = NULL; 
	}
	char ch;
	while(read(fd, &ch, 1)) {
		et->enc_arr[ch - '\0'] += 1;
	}
	return et;
}
/*

typedef struct huffman_node{
	char character;
	int freq;
	int flag;
	struct huffman_node* left;
	struct huffman_node* right;
	struct huffman_node* parent;
}huffman_node;


*/
huff_minheap * get_huff_minheap(encoding_table * et) {
	int i;

	huff_minheap * hp = huff_minheap_init(ASCII_SIZE);

	for(i = 0; i < ASCII_SIZE; i++) {
		if(et->enc_arr[i].frequency != 0) {
			huffman_node * hp_node = (huffman_node *) malloc(sizeof(huffman_node));
			if(hp_node) {
				hp_node->freq = et->enc_arr[i].frequency;
				hp_node->character = (char) i;
				hp_node->flag = 1;
				hp_node->left = hp_node->right = hp_node->parent = NULL;
				et->enc_arr[i].huff_ptr = hp_node;
				huff_minheap_insert(hp, hp_node);
			}
		}
	}
	return hp;
}

huffman_tree construct_huffman_tree(huff_minheap * hp) {
	int i;
	huffman_tree ht;
	for(i = 0; i < hp->len; i++) {
		ht = join_trees(huff_minheap_delete(hp), huff_minheap_delete(hp));
		if(hp->len == 0) {
			break;
		}
		huff_minheap_insert(hp, ht);
	}
	return ht;
}

void get_encodings(huffman_tree ht, encoding_table *et) {
	int i;
	stack *st = stack_init();
	for(i = 0; i < ASCII_SIZE; i++) {
		if(et->enc_arr[i].frequency != 0) {
			huffman_node *h_node = et->enc_arr[i].huff_ptr;
			while(h_node->parent) {
				if(h_node->parent->right == h_node) {
					stack_push(st, '1');
				}
				else {
					stack_push(st, '0');
				}
			}
			et->enc_arr[i].enc_key = extract_string(st);
		}
	}
	return;
}


int main() {
	return 0;
}


