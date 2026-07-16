#include <stdio.h>
#include <stdlib.h>

typedef struct Target {
	char category[20];
	double conf;
	int x;
	int y;
	int w;
	int h;
} Target;

typedef struct FrameData{
	int frame_id;
	char time_str[25];
	char cam_id[10];
	int target_count;
	Target targets[10]; 
} FrameData;

typedef struct VisionFrame {
	struct FrameData data;
	struct VisionFrame *next;
	struct VisionFrame *prev;
} VisionFrame;

loadfile();
add_frame();
print_all();
filter();
sort_frame();
search();
summary();
check_memory();
void clear_all();


int main(){
	do{
		int choice = get_menu();
		switch(choice){
		case 1:{loadfile();break;}
		case 2:{add_frame();break;}
		case 3:{print_all();break;}
		case 4:{filter();break;}
		case 5:{sort_frame();break;}
		case 6:{search();break;}
		case 7:{summary();break;}
		case 8:{check_memory();break;}
		case 9:{clear_all();break;}
		}
	}while(choice != 0);

	clear_all();
	exit();
	
}

//linklist tools

VisionFrame *create_node(FrameData value){
	VisionFrame *new_node = malloc(sizeof(VisionFrame));

	if (new_node == NULL){
		fprintf(stderr, "error: Memory allocation failed. \n");
		exit(EXIT_FAILURE);
	}

	new_node -> data = value;
	new_node -> prev = NULL;
	new_node -> next = NULL;

	return new_node;
}

int delete_node(VisionFrame **head, FrameData value){
	if (head == NULL || *head == NULL) {return 0;}
	VisionFrame *current = *head;

	while (current != NULL && current->data.frame_id != value.frame_id) {
		current = current->next;
	}

	if (current == NULL) {return 0;}

	if (current->prev != NULL) {
		current->prev->next = current->next;
	}else{
		*head = current->next;
	}

	if (current->next != NULL) {
		current->next->prev = current->prev;
	}

	free(current);
	return 1;
}

void append_node(VisionFrame **head, FrameData value){
	
	VisionFrame *new_node = create_node(value);

	if (*head == NULL){
		*head = new_node;
		return;
	}
	VisionFrame *last = *head;
	while(last -> next != NULL){
		last = last -> next; 
	}

	last -> next = new_node;
	new_node -> prev = last;
}

//only print frame_id this version	
void print_list(const VisionFrame *head){
	const VisionFrame *current = head;

	while(current != NULL){
		printf("%d", current -> data.frame_id);
		if(current -> next != NULL){printf("->");}
		current = current -> next;
	}
	printf("->NULL\n");
}

void free_list(VisionFrame *head){
	VisionFrame *current = head;

	while(current != NULL){
		VisionFrame *next_node = current -> next;
		free(current);
		current = next_node;
	}
}
