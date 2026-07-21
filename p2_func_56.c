//function 5: sort the list by average confidence
void sort_frame(){
    if(head == NULL){
        printf("The list is empty. \n");
        return;
    }
    FrameData temp;
    VisionFrame *current = head;
    int SwapTime=1;
    while(SwapTime != 0){
        current = head;
        SwapTime=0;
        while(current->next != NULL){
            if(average_confidence(current->data) < average_confidence(current->next->data)){
                temp = current->data;
                current->data = current->next->data;
                current->next->data = temp;
                SwapTime++;
            }
            current = current->next;
        }
    }
    printf("[+] The list sorted by the average confidence. \n");
}

//a function for calculating the average confidence in a frame, used in function 5. 
double average_confidence(FrameData frame){
    if(frame.target_count == 0)    {
        return 0;
    }
    double sum = 0;
    for(int i = 0; i < frame.target_count; i++){
        sum += frame.targets[i].conf;
    }
    return sum / frame.target_count;
}

//function 6: search for frames
void search(){
    char keyword[20];
    printf("Enter keyword to search: ");
    scanf("%s", keyword);
    printf("\n--- Search Results for '%s' ---\n", keyword);
    VisionFrame *current = head;
    int result = 0;
    while(current != NULL){
        int found = 0;
        for(int i = 0; i < current->data.target_count; i++){
            if(strstr(current->data.targets[i].category, keyword) != NULL){
                found = 1;
                result = 1;
                break;
            }
        }
        if(found){
            printf("Frame ID: %d | Time: %s | Cam: %s\n",
                   current->data.frame_id,
                   current->data.time_str,
                   current->data.cam_id);
        }
        current = current->next;
    }
    if(result==0){
        printf("No matching frames found.\n");
    }
}