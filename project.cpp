 
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdlib>
 
using namespace std;
 
#define BUFFER_SIZE 256 // Buffer size for handling larger messages
 
// Function to handle parent-child communication
void processCommunication(int pipe1[], int pipe2[]) {
   pid_t pid = fork();
 
   if (pid < 0) { // Fork failed
       cerr << "Fork failed" << endl;
       exit(1);
   }
 
   if (pid == 0) { // Child process
       close(pipe1[1]); // Close write end of first pipe
       close(pipe2[0]); // Close read end of second pipe
       
       while (true) {
           char readBuffer[BUFFER_SIZE] = {0};
           read(pipe1[0], readBuffer, sizeof(readBuffer)); // Read from parent
           
           if (strcmp(readBuffer, "exit") == 0) {
               cout << "[Child] Exiting..." << endl;
               break;
           }
 
           cout << "[Child] Received: " << readBuffer << endl;
           
           string response;
           cout << "[Child] Enter response: ";
           getline(cin, response);
           
           write(pipe2[1], response.c_str(), response.length() + 1);
       }
 
       close(pipe1[0]); // Close read end after reading data
       close(pipe2[1]); // Close write end after sending data
       exit(0);
   } else { // Parent process
       close(pipe1[0]); // Close read end of first pipe
       close(pipe2[1]); // Close write end of second pipe
      while (true) {
           string message;
           cout << "Enter message to child: ";
           getline(cin, message);
           write(pipe1[1], message.c_str(), message.length() + 1);
           
            if (message == "exit") {
              cout << "[Parent] Exiting..." << endl;
               break; }
           char readBuffer[BUFFER_SIZE] = {0};
          read(pipe2[0], readBuffer, sizeof(readBuffer)); // Read response from child
           cout << "[Parent] Received: " << readBuffer << endl;
       }
close(pipe1[1]); // Close write end after sending data
       close(pipe2[0]); // Close read end after reading data
       wait(NULL); // Wait for child process to finish
   }
}
int main() {
   int pipe1[2], pipe2[2]; // File descriptors for two pipes
   if (pipe(pipe1) == -1 || pipe(pipe2) == -1) { // Create two pipes
       cerr << "Pipe creation failed" << endl;
       return 1;
   } cout << "Simulating continuous message exchange using unnamed pipes..." << endl;
   processCommunication(pipe1, pipe2);
   
   return 0;
}