#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include "big.h"
#define ASCII_SIZE 256

huffman_node* join_trees(huffman_node* h1 , huffman_node* h2){
	huffman_node* nn = (huffman_node *)malloc(sizeof(huffman_node));
	if (nn){
		nn->flag = 0;
		nn->freq = 0;
		nn->character ='\0';
		if(h1) {
			nn->freq += h1->freq;
			h1->parent = nn;
			nn->left = h1;
		}
		if(h2) {
			nn->freq += h2->freq;
			h2->parent=nn;
			nn ->right = h2;
		}
		nn->parent=NULL;
	}
	return nn;
}

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
	long long int i;
	for(i = 0; i < ASCII_SIZE; i++) {
		et->enc_arr[i].frequency = 0;
		et->enc_arr[i].huff_ptr = NULL; 
	}
	char ch;
	while(read(fd, &ch, 1)) {
		et->enc_arr[(long long int)(ch - '\0')].frequency += 1;
	}
	return et;
}

huff_minheap * get_huff_minheap(encoding_table * et) {
	long long int i;

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
	// long long int i;
	huffman_tree ht;
	while(1) {
		huffman_node *h1 = huff_minheap_delete(hp);
		printf("printing the heap after deleting\n");
		huff_minheap_print(hp);
		if(h1) {
			if(h1->flag == 1) {
				printf("h1->character = '%c' ", h1->character);
			}
			printf("h1->freq = %lld\n", h1->freq);
		}
		huffman_node *h2 = huff_minheap_delete(hp);
		printf("printing the heap after deleting\n");
		huff_minheap_print(hp);
		if(h2) {
			if(h2->flag == 1) {
				printf("h2->character = '%c' ", h2->character);
			}
			printf("h2->freq = %lld\n", h2->freq);
		}
		ht = join_trees(h1, h2);
		printf("inorder of the tree inside join\n");
		huffman_tree_inorder(ht);
		if(hp->len == 0) {
			break;
		}

		huff_minheap_insert(hp, ht);
		printf("printing the heap after inserting the joined trees\n");
		huff_minheap_print(hp);
		printf("\niteration over, heap length = %lld\n\n\n", hp->len);
	}
	return ht;
}

void huffman_tree_inorder(huffman_tree ht) {
	if(ht == NULL) {
		return;
	}
	huffman_tree_inorder(ht->left);
	if(ht->flag != 0) {
		printf("'%c', <%d>", ht->character, ht->character);
	}
	printf("%lld\n", ht->freq);
	huffman_tree_inorder(ht->right);
}

void get_encodings(huffman_tree ht, encoding_table *et) {
	long long int i;
	for(i = 0; i < ASCII_SIZE; i++) {
		if(et->enc_arr[i].frequency != 0) {
			stack *st = stack_init();
			printf("getting encodings for %c\n", (int)i - '\0');
			huffman_node *h_node = et->enc_arr[i].huff_ptr;
			while(h_node->parent) {
				if(h_node->parent->right == h_node) {
					stack_push(st, '1');
				}
				else {
					stack_push(st, '0');
				}
				h_node = h_node->parent;
			}
			et->enc_arr[i].enc_key = extract_string(st);
			printf("encoding for %c is %s\n", (int)i - '\0', et->enc_arr[i].enc_key);
		}
	}
	return;
}
huff_minheap* huff_minheap_init(long long int size){
	huff_minheap* hp = (huff_minheap *)malloc(sizeof(huff_minheap));
	if (hp){
		hp->len=0;
		hp->size=size;
		hp->arr = (huffman_node **)malloc(sizeof(huffman_node *)*size);

		for (long long int i=0;i<size;i++){
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
	long long int i = hp->len;
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
	for (long long int i=0;i<hp->len;i++){
		if (hp->arr[i]->flag != 0){
			printf("%c %lld \n",hp->arr[i]->character, hp->arr[i]->freq);
		}
		else {
			printf("tree encountered in heap\n");
			huffman_tree_inorder(hp->arr[i]);
			printf("\n");
		}
	}
	printf("\n");
	return;
}

huffman_node* huff_minheap_delete(huff_minheap* hp){
	if (hp->len==0){
		return NULL;
	}
	long long int i = hp->len-1;
	huff_minheap_swap(&hp->arr[0],&hp->arr[i]);

	long long int k=0;
	while ( (2*k) + 1 < i){
		long long int large = (2*k)+1;

		if ( (2*k) + 2 < i && hp->arr[large]->freq > hp->arr[(2*k)+2]->freq){
			large = (2*k)+2;
		}

		if (hp->arr[k]->freq > hp->arr[large]->freq){
			huff_minheap_swap(&hp->arr[k],&hp->arr[large]);
		}
		k = large;
	}
	hp->len--;
	return hp->arr[hp->len];
}
stack* stack_init(void){
	struct stack* st = (struct stack*)malloc(sizeof(struct stack));
	if (st){
		st->head=NULL;
		st->tail=NULL;
		st->len=0;
	}
	return st;
}


void stack_push(stack* st,char ch){
	printf("entered stack push\n");
	//if (st==NULL)
	stack_node* st_node = (stack_node*)malloc(sizeof(stack_node));
	if (st_node){
		st_node -> character = ch;
		if (st->head==NULL && st->tail==NULL){
			st->head = st_node;
			st->tail=st_node;
			st_node->next=NULL;
			st_node->prev = NULL;
			st->len++;
			return;
		}
		st->tail->next = st_node;
		st_node ->prev= st->tail;
		st->tail = st_node;
		st_node ->next = NULL;
		st->len++;
		return;
	}
	return;
}
			
char* extract_string(stack* st){
	char* arr = (char*)malloc(sizeof(char)*st->len + 1);
	long long int counter =0;
	if (arr){
		stack_node* temp = st->tail;
		while (temp != NULL){
			arr[counter++] = temp->character;
			temp = temp->prev;
			if (temp){
				free(temp->next);
			}
		}
		free(st->head);
		free(st);
		arr[counter]='\0';
	}
	return arr;
}

void print_encoding_table_freq(encoding_table *et) {
	int i;
	for(i = 0; i < et->len; i++) {
		if(et->enc_arr[i].frequency != 0) {
			printf("%c %lld\n", i - '\0', et->enc_arr[i].frequency);
		}
	}
	return;
}


void print_encoding_table_encoding(encoding_table *et) {
	int i;
	for(i = 0; i < et->len; i++) {
		if(et->enc_arr[i].frequency != 0) {
			printf("%c %lld ", i - '\0', et->enc_arr[i].frequency);
			printf("%s\n", et->enc_arr[i].enc_key);
		}
	}
	return;
}

int main(int argc, char *argv[]) {
	if(argc != 2) {
		return 1;
	}
	int fd = open(argv[1], O_RDONLY);
	if(fd == -1) {
		printf("open failed\n");
		return 1;
	}
	encoding_table * et;
	et = get_character_freq_fromfile(fd);
	print_encoding_table_freq(et);

	huff_minheap *hp;
	hp = get_huff_minheap(et);
	printf("printing minheap\n");
	huff_minheap_print(hp);

	huffman_tree ht = NULL;
	ht = construct_huffman_tree(hp);
	printf("printing inorder traversal of huffman tree\n");
	huffman_tree_inorder(ht);
	get_encodings(ht, et);
	print_encoding_table_encoding(et);
	return 0;
}


