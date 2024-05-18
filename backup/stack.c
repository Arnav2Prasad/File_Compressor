#include <stdio.h>
#include <stdlib.h>

typedef struct stack{
	struct stack_node* head;
	struct stack_node* tail;
	int len;
}stack;
	

typedef struct stack_node{
	char character;
	struct stack_node* next;
	struct srack_node* prev;
}stack_node;


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
		stack_node* temp = st->head;
		while (temp->next != NULL){
			temp = temp->next;
		}
		temp->next = st_node;
		st_node ->prev= temp;
		st->tail = st_node;
		st_node ->next = NULL;
		st->len++;
		return;
	}
	return;
}

int stack_is_empty(stack* st){
	if (st->head == NULL && st->tail==NULL)
			
char* extract_string(stack* st){
	char* arr = (char*)malloc(sizeof(char)*st->len + 1);
	int counter =0;
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


























