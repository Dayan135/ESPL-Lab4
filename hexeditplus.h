#ifndef HEXEDITPLUS_H
#define HEXEDITPLUS_H

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

typedef struct command{
    char* name;
    void (*fun)(state*);
} Command;



void toggle_debug_mode(state*);
void set_file_name(state*);
void set_unit_size(state*);
void load_into_memory(state*);
void toggle_display_mode(state*);
void file_display(state*);
void memory_display(state*);
void save_into_file(state*);
void memory_modify(state*);
void quit(state*);


void print_menu();
char get_input();



#endif
