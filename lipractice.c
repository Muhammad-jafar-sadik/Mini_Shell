#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
#include<fcntl.h>

int main()
{
    pid_t pid;
    char command[100];

    while(1)
    {
        printf("my_shell$ "); // typing command 
        
        fgets(command, sizeof(command), stdin);//reading input through fgets so every char of command including space being read
        
        command[strcspn(command, "\n")] = '\0';// strcspn is used to remove newline char which comes while using fgets()
        //so strcspn finds the char position with that we will replace '\0' char in place of the '\n' so we dont need newline char

        char *argv[10];// array of 10 char pointers
        char *token;//pointer to string/sequence of characters  
        int i = 0;

        token = strtok(command, " ");

        while(token != NULL)
        {
        argv[i] = token;
        i++;
        token = strtok(NULL, " ");
        }

        argv[i] = NULL;

        if(argv[0] == NULL)
        {
         continue;
        }
        
        if(strcmp(argv[0], "cd") == 0)// we are comapring first command "cd" in zeroth index and then
        //as  we need to implement change working directory so it is cd /tmp
        {
          if(argv[1] != NULL)
          {
            char *path = argv[1];
            if(chdir(path) < 0)
            {
              perror("chdir");
            }            
          }
        /* we created a pointer *path which points to command present in argv[1]th place 
        as it is /tmp becuase space between cd /tmp has been cut through strtok() as it has now treated as token 
        it cuts space as delimeter and separates two strings so argv[0]=cd, argv[1] = /tmp
         here chdir() will execute /tmp as we stored it in path(pointer)*/
        }

        else
        {
          
          if(argv[1] != NULL && strcmp(argv[1], ">") == 0)
          {
           char *filename = argv[2];
           argv[1] = NULL;
          
           pid = fork();
           if(pid < 0)
            {
             perror("fork"); // fork error handling
            } 

           else if(pid == 0)
            {
              int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);

              if(fd < 0)
              {
                perror("open"); // file error handling
                return 1;
              }

              dup2(fd, STDOUT_FILENO);
              close(fd);
              execvp(argv[0], argv);
              perror("execvp");
            }
            else
            {
             wait(NULL);
            }
         }
      
        
         else if(argv[1] != NULL && strcmp(argv[1], "<") == 0)
         {
           char *filename = argv[2];
           argv[1] = NULL;

           pid = fork(); // child creation
           if(pid < 0)
           {
            perror("fork"); // fork error handling
           }

           else if(pid == 0)
           {
            int fd = open(filename, O_RDONLY);

            if(fd < 0)
            {
              perror("open"); // file error handling
              return 1;
            }

              dup2(fd, STDIN_FILENO);
              close(fd);
              execvp(argv[0], argv);
              perror("execvp");
            }
            else
            {
             wait(NULL);
            }
         }
         
         // pipe implemenation 

        else if(argv[1] != NULL && strcmp(argv[1], "|") == 0)
         {
          char *right[10]; //creating new arguement array for running execvp() for 2nd command "wc -l"

          right[0] = argv[2]; // storing  rest of commands/chars living "ls" in new arguement for implemenation of 
          right[1] = argv[3]; // command "wc -l"
          right[2] = NULL;

          argv[1] = NULL; // by this the '|' (pipe) will be detected , beacuse at place of '|' we are storing NULL
          
          //for ls left side command implementation
           
          int fd[2]; // created 2 new file descriptors
         
          if(pipe(fd) < 0) // pipe calling 
           {
            perror("pipe"); //pipe error handling
            continue;
           }

           pid = fork(); // creating child process
           if(pid < 0)
           {
            perror("fork"); // fork error handling
           }
           else if(pid == 0)
           {
            dup2(fd[1], STDOUT_FILENO); //dup2() for pointing stdout

            close(fd[0]);//closeing files
            close(fd[1]);

            execvp(argv[0], argv); // runs the arguement gives "ls"
            perror("execvp");
           }
         
           //for wc -l right side command implementation
           else
           {
            pid = fork(); // 2nd child creation
            if(pid < 0)
            {
              perror("fork"); // fork error handling
            }
            else if(pid == 0)
            {
              dup2(fd[0], STDIN_FILENO); // dup2() pointing stdin 

              close(fd[0]); // closing files 
              close(fd[1]);

              execvp(right[0], right); // runs the 2nd arguement gives "wc -l"
              perror("execvp");
            }
            else
            {
              close(fd[0]); //closing files
              close(fd[1]);

              wait(NULL); // parent waiting till child returns
              wait(NULL);// 2nd parent waiting till child returns
            }

           }
         }
         
        
        else 
        {
         pid = fork();
           if(pid < 0)
           {
            perror("fork"); // fork error handling
           }
          else if(pid == 0)
           {
            execvp(argv[0], argv);
            perror("execvp"); //execvp error handling
           }
          else
           {
             wait(NULL);
           }
        }
     
   }
  }

    return 0;
}