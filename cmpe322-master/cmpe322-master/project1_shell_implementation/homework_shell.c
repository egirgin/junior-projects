#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

//Write and read ends of pipe
#define READ_END	0
#define WRITE_END	1

//max input size
#define MAX_COUNT 1000

//helper function to compare strings
int compare_strings(char a[], char b[])
{
   int c = 0;

   while (a[c] == b[c]) {
      if (a[c] == '\n' || b[c] == '\0')
         break;
      c++;
   }

   if (a[c] == '\n' && b[c] == '\0')
      return 0;
   else
      return -1;
}


//helper function to keep history updated if its number exceeds 15
void shift_elements(char hist[15][MAX_COUNT]){
  for(int i=0;i<14;i++){
    strcpy(hist[i], hist[i+1]);
  }
}


int main () {

   printf("Welcome!\n");
   
   char str[MAX_COUNT];

   pid_t pid;

   char history[15][MAX_COUNT];
   int command_counter = 0;

   while(1){
     //get env name and input
     printf("%s >>> ", getenv("USER"));
     fgets(str, MAX_COUNT, stdin);

     //count the number of commands read
     command_counter++;

     //if number of commands exceed 15 then shift the command history once
     if(command_counter>15){
        shift_elements(history);
        strcpy(history[14], str);
     }
     else{
       strcpy(history[command_counter-1], str);
     }

     //compare strings to identify which command it is.
     //if there is a match then it return 0, -1 otherwise.
     char ls[] = "listdir";
     char lsa[] = "listdir -a";
     char pwd[] = "currentpath";
     char hist[] = "footprint";
     char closer[] = "exit";

     int ls_result = compare_strings(str, ls);
     int lsa_result = compare_strings(str, lsa);
     int pwd_result = compare_strings(str, pwd);
     int hist_result = compare_strings(str, hist);
     int closer_result = compare_strings(str, closer);

     //Exit the program if "exit" is typed
     if(closer_result == 0){
       exit(0);
       return(0);
     }
     else{ //if command is not exit, then fork
       pid = fork();
     }

     if(pid == 0){ //child process

       if(ls_result == 0){ //listdir
         execlp("/bin/ls","ls",NULL);
       }
       else if(lsa_result == 0){ //listdir -a
         execlp("/bin/ls","ls","-a",NULL);
       }
       else if(pwd_result == 0){ // currentpath
         execlp("/bin/pwd","pwd",NULL);
       }
       else if(hist_result == 0){ // footprint : print history array
         if(command_counter < 15){
           for(int i=0; i<command_counter;i++){
             printf("%d %s", i+1, history[i]);
           }
         }
         else{
           for(int i=0; i<15;i++){
             printf("%d %s", i+1, history[i]);
           }
         }
         exit(0);
       }
       else if(strstr(str, "printfile") != NULL){ // if input contains printfile
         if(strstr(str, ">") != NULL){ //if input both contains printfile and redirect
           char *token = strtok(str, " "); // dismiss "printfile" token
           char *source = strtok(NULL, " "); // get second token as source
           token = strtok(NULL, " "); // dismiss redirect sign
           char *dest = strtok(NULL, "\n"); // get fourth token as destination
           FILE *fp = fopen(dest, "w+"); // open file in write mode, create if it is not exist
           dup2(fileno(fp), 1); // bind stdout to file
           fclose(fp); // close file, but it can write through pipe
           execlp("/bin/cat", "cat", source, NULL);
         }
         else{ // if printfile without redirect
           char *token = strtok(str, " "); // dismiss "printfile" token
           token = strtok(NULL, "\n"); // read input
           execlp("/bin/cat", "cat", token, NULL);
         }
       }
       else if(strstr(str, "grep") != NULL){ // if inpur contains grep

         char *token = strtok(str, " "); // dismiss first token which is listdir

         char arg[] = "-a";
         token = strtok(NULL, " "); // | or -a
         int arg_result = compare_strings(token, arg); //check whether second argument is either "-a" or "|"
         if(arg_result == 0){ //listdir -a
           token = strtok(NULL, " "); // dismiss pipe |
           token = strtok(NULL, "\""); // dismiss grep
           token = strtok(NULL, "\""); // take argument
         }
         else{
           token = strtok(NULL, "\""); // dismiss grep
           token = strtok(NULL, "\""); // take argument
         }

         // create pipe
         int my_pipe[2];
         pipe(my_pipe);

         pid_t ls_id = fork(); // new child process will be listdir and return its output to parent which is grep

         if(ls_id == 0){ //child process listdir
           close(my_pipe[READ_END]);  // close unused read end of pipe for listdir
           dup2(my_pipe[WRITE_END], 1); // bind stdout -> WRITE_END
           if(arg_result == 0){ //listdir -a
             execlp("/bin/ls","ls","-a",NULL);
           }
           else{ // listdir
             execlp("/bin/ls","ls",NULL);
           }

         }
         else{ // parent process grep
           wait(NULL);

           dup2(my_pipe[READ_END], 0); // stdin <- READ_END
           close(my_pipe[WRITE_END]); // close unused write end of pipe for grep

           execlp("/bin/grep", "grep", token, NULL);
         }
       }
       else{ // if command does not match any of the above
         puts("Unidentified Command!");
       }
       exit(0); // exit the child process if it does not run any exec and reachs this point 
     }
     else{
       wait(NULL);
     }

   }

   return(0);
}
