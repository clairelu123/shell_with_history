#include "history.h"

void add_history(const char *buff, const _Bool in_background)
{

  if ( count < HISTORY_DEPTH )
  {
    strncpy(history[count], buff);

    // if (in_background)
    // {
    //   history[count][i+1] = "&";
    // }
    index[count] = count;
    count++;

  }

  else
  {
    int i = 0;
    for (i = 0; i < HISTORY_DEPTH -1; i++) {
      strncpy(history[i], history[i+1]);
      index[i] = index[i+1];
    }

    strncpy(history[HISTORY_DEPTH-1], buff);
    index[HISTORY_DEPTH-1] = count;

    count++;
  }

}




void retrive_history(/* arguments */)
{

}

void print_history()
{
  int j = 0
  for (j = 0; j < HISTORY_DEPTH; j++) {
    write(STDOUT_FILENO, index[i], sizeof(int));
    write(STDOUT_FILENO, "\t", strlen("\t"));
    write(STDOUT_FILENO, history[j], strlen(history[j]));
    write(STDOUT_FILENO, "\n", strlen("\n"));
  }
}
