#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    char *hello = "Hello, world!\n";
    write(1, hello, strlen(hello));

    return 0;
}
