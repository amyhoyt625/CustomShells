#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// You must fill in wherever it says 'HOMEWORK'.

void help();
int read_command(char *program);

// In C, a string is of type 'char []' or equivalently, 'char *'
int main(int argc, char *argv[]) {
  help();

  while (1) { // This is a "read-exec-print" loop.
    printf("%% "); // print prompt
    fflush(stdout); // Don't wait for 'newline'.  Flush stdout to screen now.

    int cmd;
    while (1) {
      cmd = read_command(argv[0]);
      if (cmd == '\n') { continue; } // Ignore newlines
      if (cmd == '#') {
        printf("HOMEWORK: This is a comment char.  Read through newline.\n");
        skipline();
      }
      break;
    }

    // The syntax "1" is an int and "'1'" is a char.
    switch (cmd) {
      case 'h':
        help();
        break; // Don't forget 'break'.  Otherwise control passes to next case.
      case 'x':
      case 'q':
        printf("Exiting\n");
        exit(0); // The argument 0 of exit says return code is 0:  success.

      case '1':
        printf("HOMEWORK:  Execute 'ls' to list files.\n");
        fork_a_command("ls");
        break;
      case '2':
        printf("HOMEWORK:  Execute 'ls -l' to list files.\n");
        fork_a_command("ls -l");
        break;
      case '3': 
        char dir[100]; // Buffer to store the directory name
        printf("HOMEWORK:  See 'man 2 chdir'; Implement 'cd' to change directory.\n");
        //by see man 2 chdir do you mean fork the command or just for us to look at it? 
        //fork_a_command("man 2 chdir");
        printf("Enter directory to change to: ");
        scanf("%s", dir); // Read the directory name input from keyboard
        //???how to properly implement cd, should I make a fork cd command?
        if (chdir(dir) == 0) {
            printf("Successfully changed directory to: %s\n", dir);
        } else {
            perror("chdir failed"); }
        break;
      case '4':
        // You'll need to continue to read 'env var' and stop at newline.
        printf("HOMEWORK:  See 'man 3 getenv'; print env var (e.g., PWD).\n");
        fork_a_command("man 3 getenv");
        char env_var[100];
        printf("Enter environment variable name: ");
        scanf("%s", env_var); 
        char *env_value = getenv(env_var);  // Fetch the value using getenv
        if (env_value != NULL) {
            // If the environment variable exists, print its value
            printf("The value of %s is: %s\n", env_var, env_value);
        } else {
            // If the environment variable doesn't exist, inform the user
            printf("Environment variable %s not found.\n", env_var);
        }
        break;
      case '5':
        // You'll need to to read 'env var' and string; stop at newline.
        printf("HOMEWORK:  See 'man 3 setenv'.\n");
        fork_a_command("man 3 setenv");

        break;
      case '6':
        // Continue to read 'src' and 'dest' files and stop at newline for each.
        printf("HOMEWORK:  Execute 'cp src dest'.\n");
        //????
        fork_a_command("cp src dest");

        break;
      case '7':
        // You'll need to continue to read 'dir' and stop at newline.
        printf("HOMEWORK:  Execute 'mkdir dir'.\n");
        char dir[100];
        printf("Enter the dir: ");
        scanf("%[^\n]", dir);
        fork_2_commands("mkdir", dir);
        skipline(); 
        
        break;
      case '8':
        // You'll need to continue to read 'file' and stop at newline.
        printf("HOMEWORK:  Execute 'rm file'.\n");
        char file[100];
        printf("Enter the file name to remove: ");
        scanf("%[^\n]", file);
        fork_2_commands("rm", file);
        break;
      case '9':
        // You'll need to continue to read 'dir' and stop at newline.
        printf("HOMEWORK:  Execute 'rmdir dir'.\n");
        char dir[100];
        printf("Enter the dir: ");
        scanf("%[^\n]", dir);
        fork_2_commands("rmdir", dir);
        break;

      default:
        printf("Unrecognized command: %c\n", (char)cmd);
        printf("HOMEWORK: Read through newline.\n");
        // Skip the rest of the line 
        char buf;
        // Keep reading until newline, 0 means read from keyboard, &buf where char is stored
        // 1 is to read 1 byte/char, == 1 checks if read 1 byte and that it is not \n
        while (read(0, &buf, 1) == 1 && buf != '\n'); 
        break;
    }
  }
 return 0;  // When the process exits, this is its return code. 0 is success.
}

void fork_a_command(char *cmd) {
  printf("*** The child will be created and execute 'ls'\n");
  int childpid = fork(); // create new process in process table
                 // The new child process will look like clone of this process.

  if (childpid == 0) { // if this is a child
    char *args[] = {cmd, NULL};
    execvp(cmd, args);
  } else { // else this is the original parent process that ran 'main'.
    sleep(2);  // wait 2 seconds and then print.
    printf("*** The child should now have executed 'ls'\n");
  }
}

void fork_2_commands(const char *cmd, const char *arg) {
    printf("*** The child will be created and execute");
    int childpid = fork(); // Create a new process

    if (childpid == 0) { // If this is the child process
        char *args[] = {(char *)cmd, (char *)arg, NULL};
        execvp(cmd, args); // Execute the command with the argument
    } else { // Parent process
        sleep(2); // Wait 2 seconds
        printf("*** The child should now have executed");
    }
}

void fork_cd_command(const char *dir) {
    int childpid = fork();
    if (childpid == 0) {
        // In the child process, use execvp to run cd
        char *args[] = {"cd", (char *)dir, NULL}; // Prepare the arguments
        execvp("cd", args);  // Execute the command
        perror("execvp failed");  // If execvp fails, print the error
        exit(1);
    } else {
        wait(NULL);  // Parent waits for the child process to finish
    }
}

void skip_line() {
    // Skip the rest of the line (ex. characters after # until newline)
    char buf;
    // Keep reading until newline, 0 means read from keyboard, &buf where char is stored
    // 1 is to read 1 byte/char, == 1 checks if read 1 byte and that it is not \n
    while (read(0, &buf, 1) == 1 && buf != '\n'); 
}

void help() {
  printf("HOMEWORK:  Print help statement showing ALL cases.\n");
  printf("EXAMPLE:\n 1: ...\n 2: ...\n h: ...\n x: ...\n q: ...\n #: ...\n");
  printf(" 1: Execute 'ls' to list files in the current directory.\n"
        " 2: Execute 'ls -l' to list files with detailed information.\n"
        " 3: Execute 'man 2 chdir' to view the manual for the 'chdir' system call.\n"
        " 4: View the manual for 'getenv' (e.g., PWD environment variable).\n"
        " 5: View the manual for 'setenv' to set environment variables.\n"
        " 6: Execute 'cp src dest' to copy files from src to dest.\n"
        " 7: Execute 'mkdir dir' to create a new directory.\n"
        " 8: Execute 'rm file' to remove a file.\n"
        " 9: Execute 'rmdir dir' to remove an empty directory.\n"
        " h: Show this help message.\n"
        " x or q: Exit the program.\n"
        " #: Comment, ignore the rest of the line after this character.\n");
}

int read_command(char *program) {
  char buf[1];
  while (1) {
    int rc;
    rc = read(0, buf, 1);  // fd is 0 (stdin); read just 1 character into buf.
    if (rc == 1) { // If 1 char was read
      break;
    } else if (rc == -1) { // If error
      if (rc == EAGAIN || rc == EINTR) {
        continue;  // The O/S kernel wasn't ready.  Ask it again.
      } else {
        fprintf(stderr, "%s: Error in reading command\n", program);
        exit(1);  // Error:  Set return code to error number 1
      }
    } else if (rc == 0) {
      printf("%s: End-of-input\n", program);
      exit(0);
    }
  }

  int cmd = buf[0];  // Convert 'char' to an 'int'.
  return cmd;
}