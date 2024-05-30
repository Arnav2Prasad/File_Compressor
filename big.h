typedef struct huffman_node{
	unsigned char character;
	long long int freq;
	long long int flag;
	struct huffman_node* left;
	struct huffman_node* right;
	struct huffman_node* parent;
}huffman_node;

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
