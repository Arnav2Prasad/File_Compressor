
#include "huffman_tree.h"
typedef struct huff_minheap{
	huffman_node** arr;
	int size;
	int len;
}huff_minheap;


huff_minheap* huff_minheap_init(int size);

void huff_minheap_swap(huffman_node** a,huffman_node** b);

void huff_minheap_insert(huff_minheap* hp,huffman_node* ele);

void huff_minheap_print(huff_minheap* hp);

huffman_node* huff_minheap_delete(huff_minheap* hp);
