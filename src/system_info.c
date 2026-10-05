#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>

#include "../include/system_info.h"

#define HISTORY_FILE "../logs/operations.log"

static void pause_screen(void)
{
    int ch;

    printf("\nPress ENTER to continue...");
    fflush(stdout);

    while ((ch = getchar()) != '\n' && ch != EOF)
    {
    }

    getchar();
}

void display_standard_fds_menu(void)
{
    printf("\n");
    printf("===============================================================\n");
    printf("                  STANDARD FILE DESCRIPTORS\n");
    printf("===============================================================\n");

    printf("\nFD 0 : Standard Input  (stdin)\n");
    printf("FD 1 : Standard Output (stdout)\n");
    printf("FD 2 : Standard Error  (stderr)\n");

    printf("\nThese are the standard file descriptors of a Linux process.\n");

    pause_screen();
}

void display_process_fd_info_menu(void)
{
    char path[PATH_MAX];
    char target[PATH_MAX];
    ssize_t length;
    DIR *dir;
    struct dirent *entry;

    printf("\n");
    printf("===============================================================\n");
    printf("                    PROCESS FD INFORMATION\n");
    printf("===============================================================\n");

    printf("\nCurrent Process ID : %d\n", (int)getpid());

    snprintf(path, sizeof(path), "/proc/%d/fd", (int)getpid());

    dir = opendir(path);

    if (dir == NULL)
    {
        perror("\nopendir() failed");
        pause_screen();
        return;
    }

    printf("\nFD          TARGET\n");
    printf("---------------------------------------------------------------\n");

    while ((entry = readdir(dir)) != NULL)
    {
        if (!isdigit((unsigned char)entry->d_name[0]))
            continue;

        snprintf(path, sizeof(path),
                 "/proc/%d/fd/%s",
                 (int)getpid(),
                 entry->d_name);

        length = readlink(path, target, sizeof(target) - 1);

        if (length == -1)
            continue;

        target[length] = '\0';

        printf("%-10s  %s\n",
               entry->d_name,
               target);
    }

    closedir(dir);

    printf("\nSource : /proc/<PID>/fd\n");

    pause_screen();
}

void display_current_pid_menu(void)
{
    pid_t pid;

    printf("\n");
    printf("===============================================================\n");
    printf("                    CURRENT PROCESS ID\n");
    printf("===============================================================\n");

    pid = getpid();

    printf("\nCurrent Process ID (PID) : %d\n", (int)pid);
    printf("Operation                : getpid()\n");
    printf("Status                   : SUCCESS\n");

    pause_screen();
}

void display_total_open_fds_menu(void)
{
    char path[PATH_MAX];
    DIR *dir;
    struct dirent *entry;
    int count = 0;

    printf("\n");
    printf("===============================================================\n");
    printf("                    TOTAL OPEN FILE DESCRIPTORS\n");
    printf("===============================================================\n");

    snprintf(path, sizeof(path),
             "/proc/%d/fd",
             (int)getpid());

    dir = opendir(path);

    if (dir == NULL)
    {
        perror("\nopendir() failed");
        pause_screen();
        return;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (isdigit((unsigned char)entry->d_name[0]))
            count++;
    }

    closedir(dir);

    printf("\nCurrent Process ID : %d\n", (int)getpid());
    printf("Total Open FDs     : %d\n", count);
    printf("Source             : /proc/self/fd\n");
    printf("Status             : SUCCESS\n");

    pause_screen();
}

void system_view_history_menu(void)
{
    FILE *file;
    char line[512];

    printf("\n");
    printf("===============================================================\n");
    printf("                    OPERATION HISTORY\n");
    printf("===============================================================\n");

    file = fopen(HISTORY_FILE, "r");

    if (file == NULL)
    {
        printf("\nNo operation history is available yet.\n");
        pause_screen();
        return;
    }

    while (fgets(line, sizeof(line), file) != NULL)
        printf("%s", line);

    fclose(file);

    pause_screen();
}

void system_info_menu(void)
{
    int choice;

    while (1)
    {
        printf("\n");
        printf("===============================================================\n");
        printf("                      SYSTEM INFORMATION\n");
        printf("===============================================================\n");

        printf("\n");
        printf("1. Display Standard File Descriptors\n");
        printf("2. Display Process FD Information\n");
        printf("3. Display Current Process ID\n");
        printf("4. Display Total Open FDs\n");
        printf("5. View Operation History\n");
        printf("6. Back\n");

        printf("\n===============================================================\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n')
                ;

            printf("\nInvalid input.");
            pause_screen();
            continue;
        }

        while (getchar() != '\n')
            ;

        switch (choice)
        {
            case 1:
                display_standard_fds_menu();
                break;

            case 2:
                display_process_fd_info_menu();
                break;

            case 3:
                display_current_pid_menu();
                break;

            case 4:
                display_total_open_fds_menu();
                break;

            case 5:
                system_view_history_menu();
                break;

            case 6:
                return;

            default:
                printf("\nInvalid choice! Please select 1 to 6.");
                pause_screen();
                break;
        }
    }
}
