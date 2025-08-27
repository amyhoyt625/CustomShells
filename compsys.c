
//LOOP example

print prompt: write(1, ….);
read command: read(0, ….); scanf();
int childpid = fork();

if(childpid == 0) { // Child process
  execvp(...cmd…);
} 
else { // Parent process
  waitpid(childpid, …, …);
}

//PIPE example

int fd[2];
pipe(fd);
if (fork() == 0) {
  dup2(fd[1], 1);
  close(fd[0]);
  close(fd[1]);
  execvp("ls", args);
} 
else {
  dup2(fd[0], 0);
  close(fd[0]);
  close(fd[1]);
  execvp("wc", args);
}


#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
  int fd[2]; // File descriptors
  char msg[] = "Hello from parent!";
  char buffer[100];
  
  
  if (pipe(fd) == -1) { // Create the pipe
    perror("pipe failed");
    return 1;
  }

  pid_t pid = fork(); // Create child process
  
  if (pid == 0) { // Child process
    close(fd[1]); // Close unused write end
    read(fd[0], buffer, sizeof(buffer)); // Read from pipe
    printf("Child received: %s\n", buffer);
    close(fd[0]); // Close read end
  } 
  else { // Parent process
    close(fd[0]); // Close unused read end
    write(fd[1], msg, strlen(msg) + 1); // Write to pipe
    close(fd[1]); // Close write end
  }

return 0;


//XV6 example

struct file {
  enum { FD_NONE, FD_PIPE, FD_INODE } type;
  int ref; // Reference count
  struct pipe *pipe;
  struct inode *ip;
  int off;
  short readable;
  short writable;
}


//ASSEMBLY SYNTAX example

.data
myvar: .word 42, 37
.text
global _start

_start:
mov eax, 1
mov ebx, 0
int 0x80
