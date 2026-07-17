#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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
} VisionFrame;

//other functions

int get_menu();

//menu
VisionFrame *loadfile();
void add_frame();
void print_all();
void filter();
void sort_frame();
void search();
void summary();
void check_memory();
void clear_all(VisionFrame **head);
void exit_it(VisionFrame **head);


int main(){
	bool loop = true;	
	VisionFrame *head = NULL;
	while(loop == true){
		int choice = get_menu();
		switch(choice){
		case 1:{head = loadfile();break;}
		case 2:{add_frame();break;}
		case 3:{print_all();break;}
		case 4:{filter();break;}
		case 5:{sort_frame();break;}
		case 6:{search();break;}
		case 7:{summary();break;}
		case 8:{check_memory();break;}
		case 9:{clear_all(&head);break;}
		case 0:{exit_it(&head); loop = false; break;}
		default:{printf("Invalid choice\n");}
		}

	}
	
}

//linklist tools

VisionFrame *create_node(FrameData value){
	VisionFrame *new_node = malloc(sizeof(VisionFrame));

	if (new_node == NULL){
		fprintf(stderr, "error: Memory allocation failed. \n");
		exit(EXIT_FAILURE);
	}

	new_node -> data = value;
	new_node -> next = NULL;

	return new_node;
}

int delete_node(VisionFrame **head, FrameData value){
	if (head == NULL || *head == NULL) {
		return 0;
	}

	VisionFrame *current = *head;
	VisionFrame *previous = NULL;

	while (current != NULL &&
	       current->data.frame_id != value.frame_id) {
		previous = current;
		current = current->next;
	}

	if (current == NULL) {
		return 0;
	}

	if (previous == NULL) {
		*head = current->next;
	}
	else {
		previous->next = current->next;
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

void free_list(VisionFrame **head){
	if (head == NULL){ return; }
	VisionFrame *current = *head;

	while(current != NULL){
		VisionFrame *next_node = current -> next;
		free(current);
		current = next_node;
	}

	*head = NULL;
}

//other function

int get_menu(void){
	int choice;
	printf(
	"========================================\n"
	"    ROBOT VISION LOG SYSTEM\n"
	"========================================\n"
	"1. Load Data from File\n"
	"2. Add Single Frame Manually\n"
	"3. Print All Logs (Screen & File)\n"
	"4. Filter Low Confidence Frames (< 0.5)\n"
	"5. Sort Frames by Confidence (Desc)\n"
	"6. Search by Category Keyword\n"
	"7. Show Statistics\n"
	"8. Check Memory Usage\n"
	"9. Clear All Data\n"
	"0. Exit\n"
	"========================================\n"
	"Enter choice: "	
	);
	scanf("%d", &choice);
	return choice;
}

//menu function
void clear_all(VisionFrame **head){
	free_list(head);
	printf("[+] All data cleared and memory freed.");
}
void exit_it(VisionFrame **head){
	clear_all(head);
	printf("Exiting program. Goodbye!");
}
