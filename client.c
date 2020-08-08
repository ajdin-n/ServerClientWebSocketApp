    #include <unistd.h>
    #include <stdio.h>
    #include <stdlib.h>

    int main(int argc, char *argv[])
    {
       if(argv[1] == NULL)
       {
           printf("call function with: \n");
           printf("./client\"192.168.1.5:8080\" \n");
           printf("that is, IP_ADDRESS:PORT defined by server\n");
       }

        char* prog[3];
    	prog[0] = "firefox";
    	prog[1] = argv[1];
    	prog[2] = '\0';
    	execvp(prog[0], prog);

        return 0;
    }
