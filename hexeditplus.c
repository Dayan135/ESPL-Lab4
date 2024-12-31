#include <stdio.h>
#include<stdlib.h>
#include "hexeditplus.h"


char debug_mode = 0;

void toggle_debug_mode(){
    debug_mode = 1 - debug_mode;
    if(debug_mode)
        printf("Debug flag now on\n");
    else
        printf("Debug flag now off!\n");
}

Command cmnds[] = {{"0-Toggle Debug Mode",toggle_debug_mode},{"1-Set File Name",NULL},{"2-Set Unit Size",NULL},{"3-Load Into Memory",NULL},{"4-Toggle Display Mode",NULL},
        {"5-File Display",NULL},{"6-Memory Display",NULL},{"7-Save Into File",NULL},{"8-Memory Modify",NULL},{"9-Quit",NULL},{NULL,NULL}};


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
            if(debug_mode)
                fprintf(stderr, "Got input: %s",buffer);
            if(buffer[1] != '\n'){
                printf("Wrong Input!!! try again: ");
                continue;
            }
            c = buffer[0];
            if (c<'0' || c>'9'){
                printf("Wrong Input!!! try again: ");
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

    c[1] = '\0';
    while(1){
        print_menu();
        c[0] = get_input();

        if(c[0] == '9') break;

        chosenCmd = cmnds[atoi(c)];
        if((*chosenCmd.fun) == NULL){
            printf("Not yet implemented!\n");
            continue;
        }
        (*chosenCmd.fun)();
    }

    return 0;
}