#include <stdio.h>
#include <stdlib.h>
#include "min_heap.h"


huff_minheap* huff_minheap_init(int size){
	huff_minheap* hp = (huff_minheap *)malloc(sizeof(huff_minheap));
	if (hp){
		hp->len=0;
		hp->size=size;
		hp->arr = (huffman_node **)malloc(sizeof(huffman_node *)*size);

		for (int i=0;i<size;i++){
			hp->arr[i] = NULL;
		}
	}
	
	return hp;
}

void huff_minheap_swap(huffman_node** a,huffman_node** b){
	huffman_node* temp=*a;
	*a=*b;
	*b=temp;
	return;
}

void huff_minheap_insert(huff_minheap* hp,huffman_node* ele){
	if (hp->size == hp->len){
		return;
	}
	int i = hp->len;
	hp->arr[i] = ele;

	while (i>0 && hp->arr[i]->freq < hp->arr[(i-1)/2]->freq){
		huff_minheap_swap(&hp->arr[i],&hp->arr[(i-1)/2]);
		i = (i-1)/2;
	
	}
	hp->len++;
	return;
}

void huff_minheap_print(huff_minheap* hp){
	if (hp->len==0){
		return;
	}
	for (int i=0;i<hp->size;i++){
		if (hp->arr[i]->flag != 0){
			printf("%c %d ",hp->arr[i]->character, hp->arr[i]->freq);
		}
	}
	printf("\n");
	return;
}

huffman_node* huff_minheap_delete(huff_minheap* hp){
	if (hp->len==0){
		return NULL;
	}
	int i = hp->len-1;
	huff_minheap_swap(&hp->arr[0],&hp->arr[i]);

	int k=0;
	while ( (2*k) + 1 < i){
		int large = (2*k)+1;

		if ( (2*k) + 2 < i && hp->arr[large]->freq > hp->arr[(2*k)+2]->freq){
			large = (2*k)+2;
		}

		if (hp->arr[k]->freq > hp->arr[large]->freq){
			huff_minheap_swap(&hp->arr[k],&hp->arr[large]);
		}
		k = large;
	}
	hp->len--;
	return hp->arr[hp->len+1];
}

/*
int main(){
	huff_minheap* hp = init(10);

	insert(hp,3);
	insert(hp,1);
	insert(hp,4);
	insert(hp,9);
	insert(hp,11);
	insert(hp,-1);

	print(hp);

	for (int i=0;i<7;i++){
		delete_min(hp);
		print(hp);
	}

	return 0;
}
*/
