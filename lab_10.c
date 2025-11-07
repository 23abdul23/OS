#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> //toupper()
#include <sys/ipc.h> //IPC key constants
#include <sys/shm.h> //shmget(), shmat(), shmdt()
#include <unistd.h> 
#include <string.h> //strcpy()


#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h" // Assuming cJSON.h is in your include path

#define SHM_SIZE 1024

int main() {
    int shmid = shmget(IPC_PRIVATE, SHM_SIZE, 0666);
    char *shm;
    FILE *fp;
    long lSize;
    char *buffer;


    if (fork() == 0) { 
        shm = shmat(shmid, NULL, 0);
        //busy wait
        while (shm[0] == '\0'); // wait until parent writes

        print(shm)


        shmdt(shm);
        exit(0);
    } 
    
    else { // Parent: write & print
        shm = shmat(shmid, NULL, 0);


        fp = fopen("data.json", "rb"); // Open the file in binary read mode
        if (!fp) {
            perror("Error opening file");
            return 1;
        }

        fseek(fp, 0L, SEEK_END); // Go to end of file
        lSize = ftell(fp);       // Get file size
        rewind(fp);              // Go back to start of file

        // Allocate memory for the entire file content
        buffer = (char*) malloc(lSize + 1);
        if (!buffer) {
            fclose(fp);
            perror("Memory allocation failed");
            return 1;
        }

        // Read the file into the buffer
        if (fread(buffer, lSize, 1, fp) != 1) {
            fclose(fp);
            free(buffer);
            perror("Error reading file");
            return 1;
        }
        buffer[lSize] = '\0'; // Null-terminate the buffer

        fclose(fp); // Close the file

        shm.strcpy(buffer)

        free(buffer)
        return 0;
        

        shmdt(shm);
    }

    return 0;
}
