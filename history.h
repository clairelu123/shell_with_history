#ifndef HISTORY_H
#define  HISTORY_H

#define HISTORY_DEPTH 10

void add_history(const char *buff);
void retrive_history(int cmdnum);
void cmdexecint(int cmdnum);
void print_history();

int binarySearch(int a[], int n, int key);


#endif
