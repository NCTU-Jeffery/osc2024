#ifndef __COMMAND_H
#define __COMMAND_H

#define COMMAND_HELP            "help"
#define COMMAND_HELLO           "hello"
#define COMMAND_INFO            "info"
#define COMMAND_REBOOT          "reboot"
#define COMMAND_LS              "ls"
#define COMMAND_CAT             "cat"
#define COMMAND_MALLOC          "malloc"
#define COMMAND_LOAD_PROGRAM    "lp"
#define COMMAND_ASYNC           "async"
#define COMMAND_DT              "dt"

void help(void);
void hello(void);
void info(void);
void reboot(void);
void ls(void);
void cat(char*);
void malloc(char*);
void load_program(char*);
void dt(void);
void async(void);

void undefined(void);

#endif