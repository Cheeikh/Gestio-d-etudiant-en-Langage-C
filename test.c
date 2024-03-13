#include <stdio.h>
#include <unistd.h>

int main(void)
{
    const int trigger = 500; // ms
    const int numDots = 4;
    const char prompt[] = "Loading";

    while (1) {
        // Return and clear with spaces, then return and print prompt.
        printf("\r%*s\r%s", sizeof(prompt) - 1 + numDots, "", prompt);
        fflush(stdout);

        // Print numDots number of dots, one every trigger milliseconds.
        for (int i = 0; i < numDots; i++) {
            usleep(trigger * 1000);
            fputc('.', stdout);
            fflush(stdout);
        }
    }
}