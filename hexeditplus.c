#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hexeditplus.h"

#define MAX_FILE_NAME_SIZE 100

static char* hex_formats[] = {"%#hhx\n", "%#hx\n", "No such unit", "%#x\n"};
static char* dec_formats[] = {"%#hhd\n", "%#hd\n", "No such unit", "%#d\n"};
state* state0;

void toggle_debug_mode(state* s){
    s->debug_mode = 1 - s->debug_mode;
    if(s->debug_mode)
        printf("Debug flag now on\n");
    else
        printf("Debug flag now off\n");
}

void set_file_name(state* s){
    char buffer[MAX_FILE_NAME_SIZE];
    printf("Insert file name: ");
    if (fgets(buffer, MAX_FILE_NAME_SIZE, stdin)) {
        if(s->debug_mode)
            fprintf(stderr, "Debug: file name set to %s\n", buffer);
        strcpy(s->file_name, buffer);
    }
}
void set_unit_size(state* s){
    char buffer[20];
    printf("Insert new unit size (1,2 or 4): ");
    if (fgets(buffer, MAX_FILE_NAME_SIZE, stdin)) {
        if(strcmp(buffer, "1\n") == 0 || strcmp(buffer, "2\n") == 0 || strcmp(buffer, "4\n") == 0){
            s->unit_size = atoi(buffer);
            if(s->debug_mode)
                fprintf(stderr, "Debug: set size to %d\n", s->unit_size);
        }
        else
            printf("Error! invalid input\n\n");
    }
}

void load_into_memory(state* s){
    printf("Not Yet Implemented");
}
void toggle_display_mode(state* s){
    printf("Not Yet Implemented");
}
void file_display(state* s){
    printf("Not Yet Implemented");
}
void memory_display(state* s){
    printf("Not Yet Implemented");
}
void save_into_file(state* s){
    printf("Not Yet Implemented");
}
void memory_modify(state* s){
    printf("Not Yet Implemented");
}
void quit(state* s){
    if(s->debug_mode)
        printf("quiting\n");
    exit(0);
}

Command cmnds[] = {{"0-Toggle Debug Mode",toggle_debug_mode},{"1-Set File Name",set_file_name},{"2-Set Unit Size",set_unit_size},{"3-Load Into Memory",load_into_memory},
        {"4-Toggle Display Mode",toggle_display_mode},{"5-File Display",file_display},{"6-Memory Display",memory_display},
        {"7-Save Into File",save_into_file},{"8-Memory Modify",memory_modify},{"9-Quit",quit},{NULL,NULL}};


void print_menu(){
    Command curr = cmnds[0];
    int i = 0;
    while(curr.name != NULL){
        printf("%s\n", curr.name);
        curr = cmnds[++i];
    }
}

char get_input(){
    char c;
    char buffer[20];
    while(1){
        if (fgets(buffer, 20, stdin)) {
            if(buffer[1] != '\n'){
                printf("Wrong Input!!! try again!\n");
                continue;
            }
            c = buffer[0];
            if (c<'0' || c>'9'){
                printf("Wrong Input!!! try again\n");
                continue;
            }
            return c;
        }
    }
}

int main(){
    char c[2];
    char buffer[20];
    Command chosenCmd;

    state0 = (state*)malloc(sizeof(state));

    c[1] = '\0';
    while(1){
        print_menu();
        c[0] = get_input();

        chosenCmd = cmnds[atoi(c)];
        (*chosenCmd.fun)(state0);
    }

    free(state0);

    return 0;
}