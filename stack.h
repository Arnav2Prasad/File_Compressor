
typedef struct stack{
	struct stack_node* head;
	struct stack_node* tail;
	int len;
}stack;
	

typedef struct stack_node{
	char character;
	struct stack_node* next;
	struct stack_node* prev;
}stack_node;


stack* stack_init(void);


void stack_push(stack* st,char ch);

			
char* extract_string(stack* st);

















