#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>
#include <fcntl.h> // for open and close
#include <unistd.h> 

#define BUFFER_SIZE 1024
void printMenu(void);

int main(){
  int exit = 0;
  int file = open("/dev/simple_character_device", O_RDWR);
  printf("\n===Welcome to Nick's simple characeter driver test file!===\n");
  while(!exit){
      char res;
      printMenu();
      scanf(" %c", &res);
      getc(stdin);

      if (res == 'e'){
        // exit
        printf("Goodbye...\n");
        exit = 1;
      }
      else if (res == 'r'){
        // code for read test
        int n, i;
        char *user_buff;
        printf("Enter the number of bytes you want to read: ");
        scanf(" %d", &n);
        //read(file pointer, userspace buffer pointer, length of bytes to read, current file offset pointer)
        //allocate space for userspace buffer using malloc
        user_buff = malloc(n*sizeof(char));
        read(file, user_buff, n);
        //print out what was read based on length
        for (i = 0; i < n; i++){
          printf("%c", user_buff[i]);
        }
        free(user_buff);
      }
      else if (res == 'w') {
        // Code for write test
        char user_buff[BUFFER_SIZE];
        int i;
        printf("Enter data to write to device: ");
        fgets(user_buff, BUFFER_SIZE, stdin);
        //write(file pointer, userspace buffer pointer, length of bytes to read, current file offset pointer)
        write(file, user_buff, strlen(user_buff));
        //print out what you wrote based one length
        for (i = 0; i < strlen(user_buff); i++){
          printf("%c", user_buff[i]);
        }
        printf("\n");
      }
      else if (res == 's'){
        //code for seek
        int whence, offset;
        printf("Enter whence and offset: ");
        scanf(" %d %d", &whence, &offset);
        printf("whence: %d || offset: %d\n", whence, offset);
        //llseek(file pointer, offset, whence)
        lseek(file, offset, whence);
      }
      else {
        printf("Invalid Input.\n");
      }
  }
  close(file);
  return 0;
}

void printMenu(){
  printf("\n===========MENU===========\n");
  printf("Enter 'r' to read from the device.\n");
  printf("Enter 'w' to write to the device.\n");
  printf("Enter 's' to seek through the device.\n");
  printf("Enter 'e' to exit.\n");
  printf("Enter a Command (r, w, s, e): ");
  return;
}
