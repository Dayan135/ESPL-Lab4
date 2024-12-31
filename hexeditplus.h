#ifndef HEXEDITPLUS_H
#define HEXEDITPLUS_H

typedef struct command{
    char* name;
    void (*fun)();
} Command;

typedef struct {
  char debug_mode;
  char file_name[128];
  int unit_size;
  unsigned char mem_buf[10000];
  size_t mem_count;
  /*
   .
   .
   Any additional fields you deem necessary
  */
} state;

void toggle_debug_mode();
void print_menu();
char get_input();


#endif
