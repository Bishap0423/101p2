#include <string.h>

// ============ 辅助函数：解析一行文本 ============
FrameData parse_line(char *line) {
    FrameData frame;
    frame.target_count = 0;
    
    // 去除末尾的换行符
    size_t len = strlen(line);
    if (len > 0 && line[len-1] == '\n') {
        line[len-1] = '\0';
    }
    if (len > 0 && line[len-1] == '\r') {
        line[len-1] = '\0';
    }
    
    // 第1步：解析固定部分：帧号 日期 时间 摄像头ID
    char date[20], time[20];
    int n = 0;
    int result = sscanf(line, "%d %s %s %s %n", 
                        &frame.frame_id, date, time, frame.cam_id, &n);
    
    if (result != 4) {
        printf("Parse error: Expected 'FrameID Date Time CamID'\n");
        frame.target_count = 0;
        return frame;
    }
    
    // 合并日期和时间为 time_str
    sprintf(frame.time_str, "%s %s", date, time);
    
    // 第2步：指针移到第一个目标的位置
    char *ptr = line + n;
    
    // 第3步：循环解析目标（最多10个）
    while (frame.target_count < 10) {
        int m = 0;
        Target *t = &frame.targets[frame.target_count];
        
        int fields_read = sscanf(ptr, "%s %lf %d %d %d %d %n",
                                  t->category, &t->conf, 
                                  &t->x, &t->y, &t->w, &t->h, &m);
        
        if (fields_read == 6) {
            // 验证置信度
            if (t->conf < 0.0 || t->conf > 1.0) {
                printf("Invalid confidence: %.2f, skipping entire frame\n", t->conf);
                frame.target_count = 0;
                return frame;
            }
            frame.target_count++;
            ptr += m;
        } else {
            break;
        }
    }
    
    return frame;
}

// 功能1：从文件加载数据
VisionFrame* load_from_file(void) {
    VisionFrame *head = NULL;  // 局部头指针
    char filename[100];
    
    printf("Enter filename to load (e.g., logs.txt): ");
    scanf("%s", filename);
    
    // 打开文件
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Cannot open file '%s'\n", filename);
        return NULL;
    }
    
    char line[500];
    int success_count = 0;
    int fail_count = 0;
    int line_number = 0;
    
    // 逐行读取
    while (fgets(line, sizeof(line), file)) {
        line_number++;
        
        // 跳过空行
        if (strlen(line) <= 1) {
            continue;
        }
        
        // 解析这一行
        FrameData frame = parse_line(line);
        
        if (frame.target_count > 0) {
            append_node(&head, frame);
            printf("Frame %d added successfully.\n", frame.frame_id);
            success_count++;
        } else {
            printf("Line %d skipped (invalid format): %s", line_number, line);
            fail_count++;
        }
    }
    
    fclose(file);
    printf("\n========================================\n");
    printf("Finished loading.\n");
    printf("Total lines processed: %d\n", success_count + fail_count);
    printf("Successfully added: %d frames\n", success_count);
    printf("Skipped (invalid): %d lines\n", fail_count);
    printf("========================================\n");
    
    return head;
}

// 功能2：手动输入一帧
void add_frame_manual(void) {
    char line[500];
    
    printf("\n");
    printf("Enter frame data in the following format:\n");
    printf("FrameID Date Time CamID Category Conf X Y W H [Category Conf X Y W H ...]\n");
    printf("\n");
    printf("Example:\n");
    printf("105 2026-07-04 15:22 CAM01 person 0.92 120 45 30 60 car 0.41 88 90 22 28\n");
    printf("\n");
    printf("Your input: ");
    
    // 清空输入缓冲区
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
    
    // 从键盘读取一行
    if (fgets(line, sizeof(line), stdin) == NULL) {
        printf("Input failed.\n");
        return;
    }
    
    // 解析输入
    FrameData frame = parse_line(line);
    
    if (frame.target_count > 0) {
        // 解析成功，加入链表
        printf("\n[+] Frame %d parsed successfully.\n", frame.frame_id);
        printf("    Targets: %d objects\n", frame.target_count);
        for (int i = 0; i < frame.target_count; i++) {
            printf("    - %s (conf: %.2f)\n", 
                   frame.targets[i].category, frame.targets[i].conf);
        }
        printf("\n[!] WARNING: This frame was NOT added to the list.\n");
        printf("    You need a global 'head' variable or pass 'head' as parameter.\n");
    } else {
        printf("\n[!] Invalid input. Frame not added.\n");
    }