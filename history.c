// #include "history.h"
// #include <stdio.h>
// #include <stdbool.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>

void add_history(const char *buff)
{

  if ( count < HISTORY_DEPTH )
  {
    strcpy(history[count], buff);

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
      strcpy(history[i], history[i+1]);
      index[i] = index[i+1];
    }

    strcpy(history[HISTORY_DEPTH-1], buff);
    index[HISTORY_DEPTH-1] = count;

    count++;
  }
}

void print_history()
{
	char indext[10];
  int j = 0;
	int y = MIN(count, HISTORY_DEPTH);
	for (j = 0; j < y; j++) {
		sprintf(indext, "%d", index[j]);
		write(STDOUT_FILENO, indext, strlen(indext));
		write(STDOUT_FILENO, "\t", strlen("\t"));
		write(STDOUT_FILENO, history[j], strlen(history[j]));
		write(STDOUT_FILENO, "\n", strlen("\n"));
	}
}





void retrive_history(int cmdnum)
{
  if ( cmdnum < index[0] || cmdnum > count)
  {
    write(STDOUT_FILENO, "Invalid command number\n", strlen("Invalid command number\n"));
    return;
  }

  else
  {
    cmdexecint(cmdnum);
  }

}

void cmdexecint(int cmdnum)
{
  char cmdbuff[COMMAND_LENGTH];
  char *tokens[NUM_TOKENS];

  int pos = binarySearch(index[HISTORY_DEPTH], HISTORY_DEPTH, cmdnum);

  strcpy(cmdbuff, history[pos]);

  read_command(cmdbuff, tokens, &in_background);

  // DEBUG: Dump out arguments:
  for (int i = 0; tokens[i] != NULL; i++) {
    write(STDOUT_FILENO, "   Token: ", strlen("   Token: "));
    write(STDOUT_FILENO, tokens[i], strlen(tokens[i]));
    write(STDOUT_FILENO, "\n", strlen("\n"));
  }

  cmdhandler(tokens, in_background);

}


int binarySearch(int a[], int n, int key)
{
    int low = 0;
    int high = n - 1;
    while(low<= high){
        int mid = (low + high)/2;
        int midVal = a[mid];
        if(midVal<key)
            low = mid + 1;
        else if(midVal>key)
            high = mid - 1;
        else
            return mid;
    }
    return -1;
}

// void cmdhandler(char* tokens[], _Bool in_background)
// {
//   //built-in command exit
//   if (strcmp(tokens[0], "exit") == 0) {
//     return 0;
//   }
//
//   //built-in command pwd
//   if (strcmp(tokens[0], "pwd") == 0){
//     char cwd[COMMAND_LENGTH];
//     getcwd(cwd, sizeof(cwd));
//     write(STDOUT_FILENO, cwd, strlen(cwd));
//   }
//
//   //built-in command cd
//   if (strcmp(tokens[0], "cd") == 0){
//     if (chdir(tokens[1]) != 0)
//     {
//       write(STDOUT_FILENO, "Path change failed\n", strlen("Path change failed\n"));
//     }
//     else
//     {
//       write(STDOUT_FILENO, "Path changed\n", strlen("Path changed\n"));
//     }
//   }
//
//   if (strcmp(tokens[0], "history") == 0)
//   {
//     print_history();
//   }
//
//   // history internal command !! and !n
//   if (tokens[0][0] == '!')
//   {
//     if (count == 0)
//     {
//       write(STDOUT_FILENO, "NO HISTORY COMMAND\n", strlen("NO HISTORY COMMAND\n"));
//       return;
//     }
//     else
//     {
//       if (strcmp(tokens[0], "!!") == 0)
//       {
//         retrive_history(count);
//       }
//
//       else
//       {
//         char tmp[COMMAND_LENGTH];
//         for (int i = 0; i < strlen(tokens[0] - 1); i++) {
//           tmp[i] = tokens[0][i+1];
//         }
//
//         tmp[strlen(tokens[0]) - 1] = '\0';
//
//         int cpos = atoi(tmp);
//
//         retrive_history(cpos);
//
//       }
//     }
//   }
//
//
// }


void handle_SIGINT(/* arguments */) {
  write(STDOUT_FILENO, buffer, strlen(buffer));
  print_history();
  write(STDOUT_FILENO, "signal testing\n", strlen("signal testing\n"));
  exit(0);
}
