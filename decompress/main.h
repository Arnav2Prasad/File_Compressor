typedef struct huffman_node{
	unsigned char character;
	long long int freq;
	long long int flag;
	struct huffman_node* left;
	struct huffman_node* right;
	struct huffman_node* parent;
}huffman_node;

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


typedef huffman_node * huffman_tree;

void huffman_tree_inorder(huffman_tree ht);

huffman_node* join_trees(huffman_node* h1 , huffman_node* h2);

typedef struct huff_minheap{
	huffman_node** arr;
	long long int size;
	long long int len;
}huff_minheap;


huff_minheap* huff_minheap_init(long long int size);

void huff_minheap_swap(huffman_node** a,huffman_node** b);

void huff_minheap_insert(huff_minheap* hp,huffman_node* ele);

void huff_minheap_print(huff_minheap* hp);

huffman_node* huff_minheap_delete(huff_minheap* hp);

typedef struct stack{
	struct stack_node* head;
	struct stack_node* tail;
	long long int len;
}stack;
	

typedef struct stack_node{
	char character;
	struct stack_node* next;
	struct stack_node* prev;
}stack_node;


stack* stack_init(void);


void stack_push(stack* st,char ch);

			
char* extract_string(stack* st);

int open_file_writing(char str[]);

int open_file_reading(char str[]);

int get_ascii_from_bits(char str[], int start);

#define QUEUE_SIZE 1024
typedef struct char_queue {
	unsigned char arr[QUEUE_SIZE];
	int len;
	int front;
} char_queue;

char_queue * char_queue_create(void);

void char_queue_insert(char_queue *cq, unsigned char uc);

long long int char_queue_isempty(char_queue *cq);

char * char_queue_flush(char_queue *cq);

long long int char_queue_isfull(char_queue *cq);

void char_queue_print(char_queue *cq);

void write_encoding_into_file(int from, int final, encoding_table * et);

typedef struct ll_char_queue_node{
	char data;
	struct ll_char_queue_node *next;
} ll_char_queue_node;

typedef struct ll_char_queue
{
	ll_char_queue_node *front;
	ll_char_queue_node *rear;
} ll_char_queue;

ll_char_queue * ll_char_queue_init(void);

void ll_char_queue_insert(ll_char_queue *cq, unsigned char uc);

long long int ll_char_queue_isempty(ll_char_queue *cq);

long long int ll_char_queue_isfull(ll_char_queue *cq);

void ll_char_queue_print(ll_char_queue *cq);

