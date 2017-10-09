// Shell starter file
// You may make any changes to any part of this file.

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>

#define COMMAND_LENGTH 1024
#define NUM_TOKENS (COMMAND_LENGTH / 2 + 1)
#define HISTORY_DEPTH 10
#define BUFFER_SIZE 50
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

// history function
void add_history(const char *buff);
void retrive_history(int cmdnum, _Bool in_background);
void cmdexecint(int cmdnum, _Bool in_background);
void print_history();

int binarySearch(int a[], int n, int key);

//tokenization and processing
int tokenize_command(char *buff, char *tokens[]);
void read_command(char *buff, char *tokens[], _Bool *in_background);
void processcmd(char *buff, char *tokens[], _Bool *in_background);
void cmdhandler(char* tokens[], _Bool in_background);
//signal handler
void handle_SIGINT();

static char buffer[BUFFER_SIZE];
char history[HISTORY_DEPTH][COMMAND_LENGTH];
int count = 0;
int index[HISTORY_DEPTH] = {0};
/**
 * Command Input and Processing
 */

/*
 * Tokenize the string in 'buff' into 'tokens'.
 * buff: Character array containing string to tokenize.
 *       Will be modified: all whitespace replaced with '\0'
 * tokens: array of pointers of size at least COMMAND_LENGTH/2 + 1.
 *       Will be modified so tokens[i] points to the i'th token
 *       in the string buff. All returned tokens will be non-empty.
 *       NOTE: pointers in tokens[] will all point into buff!
 *       Ends with a null pointer.
 * returns: number of tokens.
 */

/* HISTORY FUNCTION */
// void add_history(const char *buff)
// {
//
//   if ( count < HISTORY_DEPTH )
//   {
//     strcpy(history[count], buff);
//
//     // if (in_background)
//     // {
//     //   history[count][i+1] = "&";
//     // }
//     index[count] = count;
//     count++;
//
//   }
//
//   else
//   {
//     int i = 0;
//     for (i = 0; i < HISTORY_DEPTH -1; i++) {
//       strcpy(history[i], history[i+1]);
//       index[i] = index[i+1];
//     }
//
//     strcpy(history[HISTORY_DEPTH-1], buff);
//     index[HISTORY_DEPTH-1] = count;
//
//     count++;
//   }
// }
//
// void print_history()
// {
// 	char indext[10];
//   int j = 0;
// 	int y = MIN(count, HISTORY_DEPTH);
// 	for (j = 0; j < y; j++) {
// 		sprintf(indext, "%d", index[j]);
// 		write(STDOUT_FILENO, indext, strlen(indext));
// 		write(STDOUT_FILENO, "\t", strlen("\t"));
// 		write(STDOUT_FILENO, history[j], strlen(history[j]));
// 		write(STDOUT_FILENO, "\n", strlen("\n"));
// 	}
// }




/**
 * Main and Execute Commands
 */
int main(int argc, char* argv[])
{
	char input_buffer[COMMAND_LENGTH];
	char *tokens[NUM_TOKENS];

	//set up SIGINT
	struct sigaction handler;
	handler.sa_handler = handle_SIGINT;
	handler.sa_flags = 0;
	sigemptyset(&handler.sa_mask);
	sigaction(SIGINT, &handler, NULL);
	// strcpy(buffer,"Ctrl-C!\n");
	// printf("Program now waiting for Ctrl-C.\n");

	while (true) {

		// Get command
		// Use write because we need to use read() to work with
		// signals, and read() is incompatible with printf().
		char tmp[COMMAND_LENGTH];
		if (getcwd(tmp, sizeof(tmp)) != NULL)
		{
			write(STDOUT_FILENO, tmp, strlen(tmp));
		}
		else
		{
			write(STDOUT_FILENO, "cwd failed\n", strlen("cwd failed\n"));
		}

		write(STDOUT_FILENO, "> ", strlen("> "));
		_Bool in_background = false;

		read_command(input_buffer, tokens, &in_background);

		if(strlen(input_buffer) == 0)
		{
			continue;
		}

		for (int i = 0; tokens[i] != NULL; i++) {
			write(STDOUT_FILENO, "   Token: ", strlen("   Token: "));
			write(STDOUT_FILENO, tokens[i], strlen(tokens[i]));
			write(STDOUT_FILENO, "\n", strlen("\n"));
		}
		cmdhandler(tokens, in_background);

		// DEBUG: Dump out arguments:


		//history function debug
		// write(STDOUT_FILENO, "   input_buffer: ", strlen("   input_buffer: "));
		// write(STDOUT_FILENO, input_buffer, strlen(input_buffer));
		// write(STDOUT_FILENO, "\n", strlen("\n"));
    // add_history(input_buffer);
		// print_history();
		//build in command exit
		// assert(!in_background);
		// if (strcmp(tokens[0], "exit") == 0) {
		// 	return 0;
		// }
		//
		// //built-in command pwd
		// if (strcmp(tokens[0], "pwd") == 0){
		// 	char cwd[COMMAND_LENGTH];
		// 	getcwd(cwd, sizeof(cwd));
		// 	write(STDOUT_FILENO, cwd, strlen(cwd));
		// }
		//
		// //built-in command cd
		// if (strcmp(tokens[0], "cd") == 0){
		// 	if (chdir(tokens[1]) != 0)
		// 	{
		// 		write(STDOUT_FILENO, "Path change failed\n", strlen("Path change failed\n"));
		// 	}
		// 	else
		// 	{
		// 		write(STDOUT_FILENO, "Path changed\n", strlen("Path changed\n"));
		// 	}
		// }

		// if (in_background) {
		// 	write(STDOUT_FILENO, "Run in background.\n", strlen("Run in background.\n"));
		// }
		//
		// int pid, exitstatus;
		// pid = fork();
		// switch (pid) {
		// 	case -1:
		// 		perror("fork failed");
		// 		exit(1);
		// 	case 0:
		// 		// printf("CHILD: current pid= %d, parent pid = %d, fpid= %d\n",getpid(), getppid(), pid );//debug purpose
		// 		write(STDOUT_FILENO, "child begin\n", strlen("child begin\n"));
		// 		execvp(tokens[0], tokens);
		// 		perror("execvp failed");
		// 		exit(1);
		// 	default:
		// 		// printf("PARENT: current pid= %d, parent pid = %d, fpid= %d\n",getpid(), getppid(), pid );//debug purpose
		// 		if (!in_background) {
		// 			while(waitpid(-1, &exitstatus, 0) != pid)
		// 				;
		// 			// write(STDOUT_FILENO, "child exited\n", strlen("child exited\n"));
		// 		}
		// 	}

		/**
		 * Steps For Basic Shell:
		 * 1. Fork a child process
		 * 2. Child process invokes execvp() using results in token array.
		 * 3. If in_background is false, parent waits for
		 *    child to finish. Otherwise, parent loops back to
		 *    read_command() again immediately.
		 */
	}
	return 0;
}
int tokenize_command(char *buff, char *tokens[])
{
	int token_count = 0;
	_Bool in_token = false;
	int num_chars = strnlen(buff, COMMAND_LENGTH);
	for (int i = 0; i < num_chars; i++) {
		switch (buff[i]) {
		// Handle token delimiters (ends):
		case ' ':
		case '\t':
		case '\n':
			buff[i] = '\0';
			in_token = false;
			break;

		// Handle other characters (may be start)
		default:
			if (!in_token) {
				tokens[token_count] = &buff[i];
				token_count++;
				in_token = true;
			}
		}
	}
	tokens[token_count] = NULL;
	return token_count;
}

/**
 * Read a command from the keyboard into the buffer 'buff' and tokenize it
 * such that 'tokens[i]' points into 'buff' to the i'th token in the command.
 * buff: Buffer allocated by the calling code. Must be at least
 *       COMMAND_LENGTH bytes long.
 * tokens[]: Array of character pointers which point into 'buff'. Must be at
 *       least NUM_TOKENS long. Will strip out up to one final '&' token.
 *       tokens will be NULL terminated (a NULL pointer indicates end of tokens).
 * in_background: pointer to a boolean variable. Set to true if user entered
 *       an & as their last token; otherwise set to false.
 */
void read_command(char *buff, char *tokens[], _Bool *in_background)
{
	*in_background = false;

	// Read input
	int length = read(STDIN_FILENO, buff, COMMAND_LENGTH-1);

	if (length < 0 && (errno != EINTR)) {
		perror("Unable to read command from keyboard. Terminating.\n");
		exit(-1);
	}

	if (length < 0 && (errno == EINTR)) {
		perror("Read failed. Terminating.\n");
		exit(-1);
	}


	// Null terminate and strip \n.
	buff[length] = '\0';
	if (buff[strlen(buff) - 1] == '\n') {
		buff[strlen(buff) - 1] = '\0';
	}

	if (buff[0] != '!' && buff[0] != '\0'){
		add_history(buff);
	}

	// Tokenize (saving original command string)
	int token_count = tokenize_command(buff, tokens);
	if (token_count == 0) {
		return;
	}

	// Extract if running in background:
	if (token_count > 0 && strcmp(tokens[token_count - 1], "&") == 0) {
		*in_background = true;
		tokens[token_count - 1] = 0;
	}
}

void processcmd(char *buff, char *tokens[], _Bool *in_background) {
	write(STDOUT_FILENO, "start processing\n", strlen("start processing\n"));
	*in_background = false;
	int length = strnlen(buff, COMMAND_LENGTH);

	if ( length < 0)
	{
		perror("Unable to read command\n");
		exit(-1);
	}

	buff[length] = '\0';
	if (buff[strlen(buff) - 1] == '\n') {
		buff[strlen(buff) - 1] = '\0';
	}

	if (buff[0] != '!' && buff[0] != '\0'){
		add_history(buff);
	}
	int token_count = tokenize_command(buff, tokens);
	if (token_count == 0) {
		return;
	}

	// Extract if running in background:
	if (token_count > 0 && strcmp(tokens[token_count - 1], "&") == 0) {
		*in_background = true;
		tokens[token_count - 1] = 0;
	}
}

void add_history(const char *buff)
{

  if ( count < HISTORY_DEPTH )
  {
    strcpy(history[count], buff);

    // if (in_background)
    // {
    //   history[count][i+1] = "&";
    // }

    index[count] = count+1;
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

    index[HISTORY_DEPTH-1] = count+1;
		// printf("index end is %d\n", index[HISTORY_DEPTH-1] );
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
	printf("print_history count is:%d\n", count );
}

void retrive_history(int cmdnum, _Bool in_background)
{
  if ( cmdnum < index[0] || cmdnum > count)
  {
    write(STDOUT_FILENO, "Invalid command number\n", strlen("Invalid command number\n"));
    return;
  }

  else
  {
    cmdexecint(cmdnum, in_background);
  }

}

void cmdexecint(int cmdnum, _Bool in_background)
{
	// write(STDOUT_FILENO, "start cmdexecint\n", strlen("start cmdexecint\n"));
  char cmdbuff[COMMAND_LENGTH];
  char *tokens[NUM_TOKENS];

	int pos;
	if (count < HISTORY_DEPTH)
	{
		pos = cmdnum - 1;
	}
	else
	{
		pos = binarySearch(index, HISTORY_DEPTH, cmdnum);
	}

	if ( pos == -1)
	{
		write(STDOUT_FILENO, "not valie pos\n", strlen("not valie pos\n"));
	}

	// printf("pos is %d\n", pos );

  strcpy(cmdbuff, history[pos]);

	processcmd(cmdbuff, tokens, &in_background);

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
				// printf("mid val is %d\n", midVal );
        if(midVal<key)
            low = mid + 1;
        else if(midVal>key)
            high = mid - 1;
        else
            return mid;
    }
    return -1;
}

void cmdhandler(char* tokens[], _Bool in_background)
{
  //built-in command exit
  if (strcmp(tokens[0], "exit") == 0) {
    exit(0);
  }

  //built-in command pwd
  else if (strcmp(tokens[0], "pwd") == 0){
    char cwd[COMMAND_LENGTH];
    getcwd(cwd, sizeof(cwd));
    write(STDOUT_FILENO, cwd, strlen(cwd));
		write(STDOUT_FILENO, "\n", strlen("\n"));
  }

  //built-in command cd
  else if (strcmp(tokens[0], "cd") == 0){
    if (chdir(tokens[1]) != 0)
    {
      write(STDOUT_FILENO, "Path change failed\n", strlen("Path change failed\n"));
    }
    else
    {
      write(STDOUT_FILENO, "Path changed\n", strlen("Path changed\n"));
    }
  }

  else if (strcmp(tokens[0], "history") == 0)
  {
    print_history();
  }

  // history internal command !! and !n
  else if (tokens[0][0] == '!')
  {
    if (count == 0)
    {
      write(STDOUT_FILENO, "NO HISTORY COMMAND\n", strlen("NO HISTORY COMMAND\n"));
      return;
    }
    else
    {
      if (strcmp(tokens[0], "!!") == 0)
      {
				write(STDOUT_FILENO, "!! received\n", strlen("!! received\n"));
				// printf("count is %d\n", count );
        retrive_history(count, in_background);
      }

      else
      {
				write(STDOUT_FILENO, "!n checking\n", strlen("!n checking\n"));
        char tmp[COMMAND_LENGTH];
        for (int i = 0; i < strlen(tokens[0]) - 1; i++) {
          tmp[i] = tokens[0][i+1];
					// write(STDOUT_FILENO, "in loop\n", strlen("in loop\n"));
					// printf("%c\n",tmp[i] );
        }

        tmp[strlen(tokens[0]) - 1] = '\0';
				write(STDOUT_FILENO, tmp, strlen(tmp));
				write(STDOUT_FILENO, "\n", strlen("\n"));

        int cpos = atoi(tmp);

				printf("cpos is %d\n",cpos );

        retrive_history(cpos, in_background);

      }
    }
  }

	else
	{
		if (in_background) {
			write(STDOUT_FILENO, "Run in background.\n", strlen("Run in background.\n"));
		}

		int pid, exitstatus;
		pid = fork();
		switch (pid) {
			case -1:
				perror("fork failed");
				exit(1);
			case 0:
				// printf("CHILD: current pid= %d, parent pid = %d, fpid= %d\n",getpid(), getppid(), pid );//debug purpose
				write(STDOUT_FILENO, "child begin\n", strlen("child begin\n"));
				execvp(tokens[0], tokens);
				perror("execvp failed");
				exit(1);
			default:
				// printf("PARENT: current pid= %d, parent pid = %d, fpid= %d\n",getpid(), getppid(), pid );//debug purpose
				if (!in_background) {
					while(waitpid(-1, &exitstatus, 0) != pid)
						;
					// write(STDOUT_FILENO, "child exited\n", strlen("child exited\n"));
				}
				while( waitpid(-1, NULL, WNOHANG) > 0 )
					;
			}
			// while( waitpid(-1, NULL, WNOHANG) > 0 )
			// 	;
	}
}

void handle_SIGINT(/* arguments */) {
  write(STDOUT_FILENO, buffer, strlen(buffer));
  print_history();
  write(STDOUT_FILENO, "signal testing\n", strlen("signal testing\n"));
  exit(0);
}
