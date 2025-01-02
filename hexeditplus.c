#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "hexeditplus.h"

#define MAX_FILE_NAME_SIZE 100
#define MAX_FILE_SIZE 10000

static char* hex_formats[] = {"%#hhx", "%#hx", "No such unit", "%#x"};
static char* dec_formats[] = {"%#hhd", "%#hd", "No such unit", "%#d"};

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
        buffer[strlen(buffer)-1] = '\0';
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
            perror("Error! invalid input\n\n");
    }
}

void load_into_memory(state* s){
    char buffer[51];
    int offset, length;
    if(s->file_name == NULL || *s->file_name=='\0'){
        perror("Error! no file name. can't load! \\n");
        return;
    }
    FILE* file = fopen(s->file_name,"r");
    if(file == NULL){
        printf("Error! couldn't load file named \"%s\". can't load! \\n",s->file_name);
        return;
    }

    printf("Please enter <location> <length>\n");
    if (fgets(buffer, 50, stdin) != NULL)
    {
        sscanf(buffer,"%x %d", &offset,&length);
    }
    else{
        perror("Error! couldn't get an input");
        goto end;
    }

    // if(length == -1 || offset == -1){
    //     perror("Error! error parsing numbers");
    //     goto end;
    // }
    
    if (fseek(file, offset, SEEK_SET) != 0) {
        perror("Error seeking to offset");
        goto end;
    }

    if(s->debug_mode)
        fprintf(stderr, "Debug: reading from file \"%s\", starting from location 0x%x (%d), for %d unites (unit = %d bytes)\n", 
            s->file_name, offset,offset, length*s->unit_size, s->unit_size);
    
    if (fread(s->mem_buf,1,length * s->unit_size,file) <= 0) {
        perror("Error reading from file");
        goto end;
    }
    end:
        fclose(file);

}
void toggle_display_mode(state* s){
    s->display_mode = 1 - s->display_mode;
    if(s->display_mode)
        printf("Display flag now on, hexadecimal representation\n");
    else
        printf("Display flag now off, decimal representation\n");
}

void file_display(state* s){
    char buffer[51];
    int offset, length;
    int u = s->unit_size;
    char* toPrint;

    printf("Please enter file offset and length as: <address> <length>\n");
    if (fgets(buffer, 50, stdin) != NULL)
    {
        sscanf(buffer,"%x %d", &offset,&length);
    }
    else{
        perror("Error! couldn't get an input");
        return;
    }

    FILE* file = fopen(s->file_name,"r");
    if(file == NULL){
        printf("Error! couldn't load file named \"%s\". can't load! \\n",s->file_name);
        return;
    }

    if (fseek(file, offset, SEEK_SET) != 0) {
        perror("Error seeking to offset");
        goto end;
    }

    if(s->debug_mode)
        fprintf(stderr, "Debug: displpaying from file \"%s\", starting from location 0x%x (%d), for %d unites (unit = %d bytes)\n", 
            s->file_name, offset,offset, length*u, u);
    
    toPrint = (char*)malloc(sizeof(char) * length * u);
    if (fread(toPrint,1,length * s->unit_size,file) <= 0) {
        perror("Error reading from file");
        goto end;
    }
    printf("%s\n========\n",s->display_mode?"Hexadecimal":"Decimal");
    for(int i = 0; i < length * u && toPrint[i] != '\0'; i+= u){
        unsigned int numToPrint = 0;
        for(int j = 0; j < u; j++){
            numToPrint<<=8;
            numToPrint += toPrint[i+j];
        }
        if(s->display_mode){//hexa print
            printf(hex_formats[u-1], numToPrint);
        }
        else{//decimal print
            printf(dec_formats[u-1], numToPrint);
        }
        printf("\n");
    }

    free(toPrint);
    end:
        fclose(file);
}

void memory_display(state* s){
    char buffer[51];
    int offset, length;
    int u = s->unit_size;

    printf("Please enter memory offset and length as: <address> <length>\n");
    if (fgets(buffer, 50, stdin) != NULL)
    {
        sscanf(buffer,"%x %d", &offset,&length);
    }
    else{
        perror("Error! couldn't get an input");
        return;
    }

    if(offset >= strlen(s->mem_buf)){
        perror("Error! offset is bigger then memory saved");
        return;
    }

    printf("%s\n========\n",s->display_mode?"Hexadecimal":"Decimal");

    unsigned char* mem2display = s->mem_buf + offset;
    for(int i = 0; i < length * u && mem2display[i] != '\0'; i+= u){
        unsigned int numToPrint = 0;
        for(int j = 0; j < u; j++){
            numToPrint<<=8;
            numToPrint += mem2display[i+j];
        }
        if(s->display_mode){//hexa print
            printf(hex_formats[u-1], numToPrint);
        }
        else{//decimal print
            printf(dec_formats[u-1], numToPrint);
        }
        printf("\n");
    }
}

void save_into_file(state* s){
    char buffer[51];
    int source_addr, target_addr, length;
    int u = s->unit_size;
    int mem_size = strlen(s->mem_buf);
    char* toPrint;

    printf("Please enter <source-address> <target-location> <length>\n");
    if (fgets(buffer, 50, stdin) != NULL)
    {
        sscanf(buffer,"%x %x %d", &source_addr, &target_addr, &length);
    }
    else{
        perror("Error! couldn't get an input");
        return;
    }

    FILE* file = fopen(s->file_name,"r+b");
    if(file == NULL){
        printf("Error! couldn't load file named \"%s\". can't load! \\n",s->file_name);
        return;
    }

    if (fseek(file, target_addr, SEEK_SET) != 0) {
        perror("Error seeking to target address");
        goto end;
    }

    if(mem_size < source_addr){ //if there are less bytes then requested to write
        perror("Error got target address that is over the memory saved");
        goto end;
    }


    if(s->debug_mode)
        fprintf(stderr, "Debug: writing to file \"%s\", to address 0x%x (%d), for %d unites (unit = %d bytes)\n", 
            s->file_name, target_addr, target_addr, length*u, u);
    
    fwrite(s->mem_buf + source_addr, length*u, 1, file);

    end:
        fclose(file);
    //
}
void memory_modify(state* s){
    char buffer[51];
    int location, val;
    int u = s->unit_size;
    int mem_size = strlen(s->mem_buf);
    char bytes[4];

    printf("Please enter <location> <val>\n");
    if (fgets(buffer, 50, stdin) != NULL)
    {
        sscanf(buffer,"%x %x", &location, &val);
    }
    else{
        perror("Error! couldn't get an input");
        return;
    }

    if(location + u >= mem_size){
        perror("Error location (+ unit size) inserted is bigger then mem_size");
        return;
    }
    
    if(val >= 1<<(8*u)){
        perror("Error value inserted is bigger then unit size");
        return;
    }

    for(int i = 0; i < u; i++){
        s->mem_buf[location + i] = val % (1<<8);
        val/= 1<<8;
    }
}
void quit(state* s){
    if(s->debug_mode)
        printf("quiting\n");
    exit(0);
}

Command cmnds[] = {{"0-Toggle Debug Mode",toggle_debug_mode},{"1-Set File Name",set_file_name},{"2-Set Unit Size",set_unit_size},
    {"3-Load Into Memory",load_into_memory}, {"4-Toggle Display Mode",toggle_display_mode},{"5-File Display",file_display},
    {"6-Memory Display",memory_display},{"7-Save Into File",save_into_file},{"8-Memory Modify",memory_modify},{"9-Quit",quit},{NULL,NULL}};


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

    
    state* state0 = (state*)malloc(sizeof(state));
    state0->debug_mode = 0;
    state0->display_mode = 0;
    state0->unit_size = 1;

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