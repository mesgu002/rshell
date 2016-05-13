  #include <sys/types.h>
  #include <sys/wait.h>
  #include <unistd.h>
  #include <time.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <vector>
  #include <iostream>
  
  using namespace std;

  int main(void)
  {
     int i, status;
     
    vector<char *> commandVector;
    commandVector.push_back("echo");
    commandVector.push_back("testing");
    commandVector.push_back("1");
    commandVector.push_back("2");
    commandVector.push_back("3");
    commandVector.push_back(NULL);
    char **command = &commandVector[0];
     
     
     pid_t childID, endID;
     time_t when;

     if ((childID = fork()) == -1) {     /* Start a child process.      */
        perror("fork error");
        exit(EXIT_FAILURE);
     }
     else if (childID == 0) {            /* This is the child.          */
        time(&when);
        printf("Child process started at %s", ctime(&when));
        execvp(command[0], command);
        exit(EXIT_SUCCESS);
     }
     else {                              /* This is the parent.         */
        time(&when);
        printf("Parent process started at %s", ctime(&when));

          /* Wait 15 seconds for child process to terminate.           */
        for(i = 0; i < 15; i++) {
          endID = waitpid(childID, &status, WNOHANG|WUNTRACED);
          if (endID == -1) {            /* error calling waitpid       */
              perror("waitpid error");
              exit(EXIT_FAILURE);
          }
          else if (endID == 0) {        /* child still running         */
              time(&when);
              printf("Parent waiting for child at %s", ctime(&when));
              sleep(1);
          }
          else if (endID == childID) {  /* child ended                 */
              if (WIFEXITED(status)) {
                  execvp(command[0], command);
                  printf("Child ended normally.n");
              }
              else if (WIFSIGNALED(status))
                 printf("Child ended because of an uncaught signal.n");
              else if (WIFSTOPPED(status))
                 printf("Child process has stopped.n");
              exit(EXIT_SUCCESS);
          }
        }
     }
  }

// #include <vector>
// #include <iostream>
// #include <unistd.h>

// using namespace std;

// int main(void) {
//   vector<char *> commandVector;

//   // do a push_back for the command, then each of the arguments
//   commandVector.push_back("echo");
//   commandVector.push_back("testing");
//   commandVector.push_back("1");
//   commandVector.push_back("2");
//   commandVector.push_back("3");  

//   // push NULL to the end of the vector (execvp expects NULL as last element)
//   commandVector.push_back(NULL);

//   // pass the vector's internal array to execvp
//   char **command = &commandVector[0];

//   int status = execvp(command[0], command);
//   cout << status;
//   return 0;
// }