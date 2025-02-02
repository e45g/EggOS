#include <tty.h>
#include <vga.h>
#include <common.h>

void print_help(void) {
    printf("\n========================== GENERIC ==========================");
    printf("\n= help           : Displays this help message               =");
    printf("\n= fetch          : Displays the logo                        =");
    printf("\n= exit           : Exits the shell                          =");
    printf("\n= get_esp        : Displays the current ESP                 =");
    printf("\n= get_stack      : Displays the current stack               =");
    printf("\n= clear          : Clears the screen                        =");
    printf("\n=============================================================\n");
}


void fetch() {
    terminal_set_color(VGA_COLOR_RED, VGA_COLOR_BLACK);
    printf("               __\n");
    printf("              / _)  ");

    terminal_set_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
    printf("DinoOS\n");
    terminal_set_color(VGA_COLOR_RED, VGA_COLOR_BLACK);

    printf("     _.----._/ /  \n");
    printf("    /         /   \n");
    printf(" __/ (  | (  |    \n");
    printf("/__.-'|_|--|_|    \n");

    terminal_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
}
