#include "Akinator.h"

#define DEF_CMD(cmd, description, number, code) CMD_##cmd = number,

enum Requests {
    #include "commands.h"
};

#undef DEF_CMD

void printOptions();

int main() {

    Tree tree = {};
    AKconstructTree(&tree);

    AKsay("Hello, I am the most intelligent AI in the world.\n");
    AKsay("I can guess anything you can imagine.\n");

    #define DEF_CMD(cmd, description, number, code) \
        if (command == number) {                    \
            code                                    \
        } else

    while(1) {
        printOptions();
        int command = 0;
        int res = scanf("%d", &command);
        if (res == 0) {
            AKsay("Sorry, that is not a command\n");
            continue;
        }
        //printf("%d\n", command);

        #include "commands.h"
        /* else */ {
            AKsay("Sorry, wrong command\n");
        }
    }
    #undef DEF_CMD
}

void printOptions() {
    #define DEF_CMD(cmd, description, number, code) \
        printf("[%2d] - %s\n", number, description);

    #include "commands.h"
    #undef DEF_CMD
}
