#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include "big.h"
#define ASCII_SIZE 256


char_queue * char_queue_create(void) {
	char_queue *tmp;
	tmp = (char_queue *) malloc(sizeof(char_queue));
	if(tmp) {
		tmp->len = 0;
	}
	return tmp;
}

void char_queue_insert(char_queue *cq, unsigned char uc) {
	cq->arr[cq->len++] = uc;
}

long long int char_queue_isempty(char_queue *cq) {
	return cq->len == 0;
}

long long int char_queue_isfull(char_queue *cq) {
	return cq->len == QUEUE_SIZE;
}

char * char_queue_flush(char_queue *cq) {
	int i;
	char *str;
	str = (char *) malloc((sizeof(char) * cq->len) + 1);
	for(i = 0; i < cq->len; i++) {
		str[i] = cq->arr[i];
	}
	str[i] = '\0';
	cq->len = 0;
	return str;
}

void char_queue_print(char_queue *cq) {
	int i;
	for(i = 0; i < cq->len; i++) {
		printf("%c ", cq->arr[i]); 
	}
	printf("\n");
}


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
int mod(int x) {
	return (x < 0) ? -x : x;
}
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
	unsigned char ch;
	while(read(fd, &ch, 1)) {
		//printf("character read is : %c, <%d>\n", ch, ch);
		
		//printf("storing at index %lld\n", (long long int)(ch));

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
		//printf("printing the heap after deleting\n");
		huff_minheap_print(hp);
		if(h1) {
			if(h1->flag == 1) {
				//printf("h1->character = '%c', <%d> ", h1->character, h1->character);
			}
			//printf("h1->freq = %lld\n", h1->freq);
		}
		huffman_node *h2 = huff_minheap_delete(hp);
		//printf("printing the heap after deleting\n");
		huff_minheap_print(hp);
		if(h2) {
			if(h2->flag == 1) {
				//printf("h2->character = '%c', <%d> ", h2->character, h2->character);
			}
			//printf("h2->freq = %lld\n", h2->freq);
		}
		ht = join_trees(h1, h2);
		//printf("inorder of the tree inside join\n");
		huffman_tree_inorder(ht);
		if(hp->len == 0) {
			break;
		}

		huff_minheap_insert(hp, ht);
		//printf("printing the heap after inserting the joined trees\n");
		huff_minheap_print(hp);
		//printf("\niteration over, heap length = %lld\n\n\n", hp->len);
	}
	return ht;
}

void huffman_tree_inorder(huffman_tree ht) {
	if(ht == NULL) {
		return;
	}
	huffman_tree_inorder(ht->left);
	if(ht->flag != 0) {
		//printf("'%c', <%d>", ht->character, ht->character);
	}
	//printf("%lld\n", ht->freq);
	huffman_tree_inorder(ht->right);
}

void get_encodings(huffman_tree ht, encoding_table *et) {
	long long int i;
	for(i = 0; i < ASCII_SIZE; i++) {
		if(et->enc_arr[i].frequency != 0) {
			stack *st = stack_init();
			//printf("getting encodings for %lld\n", i);
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
			//printf("encoding for %c, <%lld> is %s\n", (char)i, i, et->enc_arr[i].enc_key);
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
			//printf("%c %lld \n",hp->arr[i]->character, hp->arr[i]->freq);
		}
		else {
			//printf("tree encountered in heap\n");
			huffman_tree_inorder(hp->arr[i]);
			//printf("\n");
		}
	}
	//printf("\n");
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
	//printf("entered stack push\n");
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

void print_encoding_table_encoding_not_done(encoding_table *et) {
	long long int i;
	for(i = 0; i < et->len; i++) {
		if(et->enc_arr[i].frequency != 0) {
			printf("%c, <%lld>, freq = %lld\n", (char) i, i, et->enc_arr[i].frequency);
		}
	}
	return;
}


void print_encoding_table_encoding(encoding_table *et) {
	long long int i;
	for(i = 0; i < et->len; i++) {
		if(et->enc_arr[i].frequency != 0) {
			printf("%c, <%lld>, freq = %lld", (char) i, i, et->enc_arr[i].frequency);
			printf(", encoding = %s\n", et->enc_arr[i].enc_key);
		}
	}
	return;
}

/* writes the encoding table into the file fd
 * binary codes are written for characters with frequency more than zero
 */
void put_encoding_table(int fd, encoding_table *et) {
	int i;
	// char tmp[1024];
	unsigned char ch;
	int j;
	unsigned char ascii;
	unsigned char tmp;
	for(i = 0; i < et->len; i++) {
		if(et->enc_arr[i].frequency != 0) {
			ascii = i;
			if(ascii == '0') {
				tmp = '!';
				write(fd, &tmp, 1);
				write(fd, &tmp, 1);
			}
			else if(ascii == '1') {
				tmp = '#';
				write(fd, &tmp, 1);
				write(fd, &tmp, 1);
			}
			else {
				write(fd, &ascii, 1);
			}
			j = 0;

			while(et->enc_arr[i].enc_key[j] != '\0') {
				write(fd, &(et->enc_arr[i].enc_key[j]), 1);
				j++;
			}
		}
	}
	ch = '$';
	write(fd, &ch, 1);
	write(fd, &ch, 1);
	//printf("ended\n");
	return;
}

void encode_file_eff(char *file, encoding_table *et) {
	int from = open_file_reading(file);
	if(from == -1) {
		printf("%s : open failed\n", file);
		return;
	}
	
	char tmp[1024];
	strcpy(tmp, file);
	strcat(tmp, "_compressed");

	int final = open_file_writing(tmp);
	
	if(final == -1) {
		printf("%s : open failed\n", tmp);
		return;
	}

	put_encoding_table(final, et);
	write_encoding_into_file(from, final, et);

	/*
	while((result = read(to, &buffer[0], 8))) {
		buffer[8] = '\0';
		//printf("buffer = %s\n", buffer);
		//printf("result = %d\n", result);
		ascii = get_ascii_from_bits(buffer, result);
		//printf("ascii = %d\n", ascii);
		//printf("ascii in character format = %c\n", ascii);
		write(final, &ascii, 1);
	}
	*/
	close(from);
	close(final);

	return;
}

void write_encoding_into_file(int from, int final, encoding_table * et) {
	if(from == -1 || final == -1) {
		printf("files fd invalid\n");
		return;
	}
	unsigned char ch_from;
	char encoding[1024];
	char * flush_result;
	char_queue *cq;
	cq = char_queue_create();
	int i;
	unsigned char ascii;
	while(read(from, &ch_from, 1)) {
		strcpy(encoding, et->enc_arr[(int)ch_from].enc_key);
		i = 0;
		while(encoding[i] != '\0') {
			if(char_queue_isfull(cq)) {
				flush_result = char_queue_flush(cq);
				ascii = get_ascii_from_bits(flush_result, strlen(flush_result));
			
				printf("ascii character int value = %d\n", ascii); 
				write(final, &ascii, 1);
			}
			char_queue_insert(cq, encoding[i]);
			i++;
		}
	}
	if(!char_queue_isempty(cq)) {
		flush_result = char_queue_flush(cq);
		ascii = get_ascii_from_bits(flush_result, strlen(flush_result));
		printf("final ascii character int value = %d\n", ascii); 
		write(final, &ascii, 1);
	}
}

int get_ascii_from_bits(char str[], int start) {
	int ascii = 0;
	int i = start - 1;
	int coeff = 1;
	while(i >= 0) {
		ascii += (str[i] - '0') * coeff;
		coeff = coeff * 2;
		i--;
	}
	return ascii;
}

int open_file_reading(char str[]) {
	int fd = open(str, O_RDONLY);
	if(fd == -1) {
		return -1;
	}
	return fd;
}

int open_file_writing(char str[]) {
	int fd = open(str, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if(fd == -1) {
		return -1;
	}
	return fd;
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
	close(fd);
	print_encoding_table_encoding_not_done(et);

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


	encode_file_eff(argv[1], et);
	return 0;
}

#include <limits.h>

//decompreess

unsigned int binary_to_decimal(char str[], int start) {
	int ascii = 0;
	int i = start - 1;
	int coeff = 1;
	while(i >= 0) {
		ascii += (str[i] - '0') * coeff;
		coeff = coeff * 2;
		i--;
	}
	return ascii;
}

unsigned int* read_encoding_table(int from){
	unsigned char ch_from;
	unsigned char ch_next;
	unsigned int *ch_arr = (unsigned char *)malloc(sizeof(unsigned char) * 2048);

	int ch_pos;
	unsigned char * tmp= (unsigned char *)malloc(sizeof(unsigned char) * 2048);

	int prev_pos = 0;

	for (int i = 0; i < 2048; i++)
	{
		ch_arr[i] = INT_MAX;
	}

	int counter=0;
	int i = 0;

	while (read(from, &ch_from, 1))
	{
		if (ch_from == '$'){
			read(from, &ch_next, 1);
			if (ch_next == '$'){
				return ch_arr;
			}
			lseek(from, -1, SEEK_CUR);
			// arr[xxxxxx] = ch_from;
			i = 0;
			while (read(from, &ch_next, 1))
			{
				if (ch_next == '0' || ch_next == '1'){
					tmp[i] = ch_next;
					i++;
				}
			}
			tmp[i] = '\0';
			lseek(from, -1, SEEK_CUR);

			ch_arr[binary_to_decimal(tmp,i-1)] = ch_from;
		}

		else  if (ch_from == '!'){
			read(from, &ch_next, 1);
			if (ch_next == '!'){
				ch_from = '0';
			}
			lseek(from, -1, SEEK_CUR);
			// arr[xxxxxx] = ch_from;
			i = 0;
			while (read(from, &ch_next, 1))
			{
				if (ch_next == '0' || ch_next == '1'){
					tmp[i] = ch_next;
					i++;
				}
			}
			tmp[i] = '\0';
			lseek(from, -1, SEEK_CUR);

			ch_arr[binary_to_decimal(tmp,i-1)] = ch_from;
		}
		else if (ch_from == '#'){
			read(from, &ch_next, 1);
			if (ch_next == '#'){
				ch_from = '1';
			}
			lseek(from, -1, SEEK_CUR);
			// arr[xxxxxx] = ch_from;
			i = 0;
			while (read(from, &ch_next, 1))
			{
				if (ch_next == '0' || ch_next == '1'){
					tmp[i] = ch_next;
					i++;
				}
			}
			tmp[i] = '\0';
			lseek(from, -1, SEEK_CUR);

			ch_arr[binary_to_decimal(tmp,i-1)] = ch_from;
		}
		else{
			i = 0;
			while (read(from, &ch_next, 1))
			{
				if (ch_next == '0' || ch_next == '1'){
					tmp[i] = ch_next;
					i++;
				}
			}
			tmp[i] = '\0';
			lseek(from, -1, SEEK_CUR);

			ch_arr[binary_to_decimal(tmp,i-1)] = ch_from;
		}
	}
}






