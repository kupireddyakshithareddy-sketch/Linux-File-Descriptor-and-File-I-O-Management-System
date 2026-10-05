#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "../include/fd_manager.h"

typedef struct
{
    int fd;
    char filename[256];
    int active;
} FileDescriptor;

static FileDescriptor fd_table[MAX_OPEN_FILES];
static int active_fd = -1;

void register_file_descriptor(int fd, const char *filename)
{
    int i;

    for (i = 0; i < MAX_OPEN_FILES; i++)
    {
        if (!fd_table[i].active)
        {
            fd_table[i].fd = fd;

            strncpy(fd_table[i].filename,
                    filename,
                    sizeof(fd_table[i].filename) - 1);

            fd_table[i].filename[
                sizeof(fd_table[i].filename) - 1
            ] = '\0';

            fd_table[i].active = 1;

            return;
        }
    }

    printf("\nERROR: FD table is full.\n");
}

void display_fd_table(void)
{
    int i;
    int found = 0;

    printf("\n");
    printf("===============================================================\n");
    printf("                    OPEN FILE DESCRIPTORS\n");
    printf("===============================================================\n");

    printf("%-10s %-35s %-10s\n", "FD", "FILE", "ACTIVE");
    printf("---------------------------------------------------------------\n");

    for (i = 0; i < MAX_OPEN_FILES; i++)
    {
        if (fd_table[i].active)
        {
            printf("%-10d %-35s %-10s\n",
                   fd_table[i].fd,
                   fd_table[i].filename,
                   (fd_table[i].fd == active_fd) ? "YES" : "NO");

            found = 1;
        }
    }

    if (!found)
    {
        printf("No application files are currently open.\n");
    }

    printf("===============================================================\n");
}

int is_fd_registered(int fd)
{
    int i;

    for (i = 0; i < MAX_OPEN_FILES; i++)
    {
        if (fd_table[i].active && fd_table[i].fd == fd)
        {
            return 1;
        }
    }

    return 0;
}

void set_active_fd(int fd)
{
    if (is_fd_registered(fd))
    {
        active_fd = fd;
    }
}

int get_active_fd(void)
{
    return active_fd;
}

const char *get_fd_filename(int fd)
{
    int i;

    for (i = 0; i < MAX_OPEN_FILES; i++)
    {
        if (fd_table[i].active && fd_table[i].fd == fd)
        {
            return fd_table[i].filename;
        }
    }

    return NULL;
}

void duplicate_fd_menu(void)
{
    int source_fd;
    int new_fd;
    const char *filename;

    printf("\n===============================================================\n");
    printf("                 DUPLICATE FILE DESCRIPTOR\n");
    printf("===============================================================\n");

    display_fd_table();

    printf("\nEnter source FD: ");
    scanf("%d", &source_fd);

    if (!is_fd_registered(source_fd))
    {
        printf("\nERROR: FD %d is not registered.\n", source_fd);
        printf("\nPress ENTER to continue...");
        getchar();
        getchar();
        return;
    }

    new_fd = dup(source_fd);

    if (new_fd == -1)
    {
        perror("\ndup() failed");
        printf("\nPress ENTER to continue...");
        getchar();
        getchar();
        return;
    }

    filename = get_fd_filename(source_fd);

    register_file_descriptor(new_fd, filename);

    printf("\nSUCCESS!\n");
    printf("Original FD : %d\n", source_fd);
    printf("New FD      : %d\n", new_fd);
    printf("File        : %s\n", filename);

    printf("\nBoth descriptors refer to the same open file.\n");

    display_fd_table();

    printf("\nPress ENTER to continue...");
    getchar();
    getchar();
}

void duplicate_fd2_menu(void)
{
    int source_fd;
    int target_fd;
    int new_fd;
    const char *filename;

    printf("\n===============================================================\n");
    printf("              DUPLICATE TO SPECIFIC FD (dup2)\n");
    printf("===============================================================\n");

    display_fd_table();

    printf("\nEnter source FD: ");
    scanf("%d", &source_fd);

    if (!is_fd_registered(source_fd))
    {
        printf("\nERROR: Source FD %d is not registered.\n",
               source_fd);

        printf("\nPress ENTER to continue...");
        getchar();
        getchar();
        return;
    }

    printf("Enter target FD: ");
    scanf("%d", &target_fd);

    if (source_fd == target_fd)
    {
        printf("\nSource and target FD are the same.\n");
        printf("dup2() does not create another descriptor in this case.\n");

        printf("\nPress ENTER to continue...");
        getchar();
        getchar();
        return;
    }

    new_fd = dup2(source_fd, target_fd);

    if (new_fd == -1)
    {
        perror("\ndup2() failed");
        printf("\nPress ENTER to continue...");
        getchar();
        getchar();
        return;
    }

    filename = get_fd_filename(source_fd);

    register_file_descriptor(new_fd, filename);

    printf("\nSUCCESS!\n");
    printf("Source FD : %d\n", source_fd);
    printf("Target FD : %d\n", new_fd);
    printf("File      : %s\n", filename);

    display_fd_table();

    printf("\nPress ENTER to continue...");
    getchar();
    getchar();
}

void close_fd_menu(void)
{
    int fd;

    printf("\n===============================================================\n");
    printf("                    CLOSE FILE DESCRIPTOR\n");
    printf("===============================================================\n");

    display_fd_table();

    printf("\nEnter FD to close: ");
    scanf("%d", &fd);

    if (!is_fd_registered(fd))
    {
        printf("\nERROR: FD %d is not registered.\n", fd);

        printf("\nPress ENTER to continue...");
        getchar();
        getchar();
        return;
    }

    if (close(fd) == -1)
    {
        perror("\nclose() failed");

        printf("\nPress ENTER to continue...");
        getchar();
        getchar();
        return;
    }

    printf("\nFD %d closed successfully.\n", fd);

    if (active_fd == fd)
    {
        active_fd = -1;
    }

    for (int i = 0; i < MAX_OPEN_FILES; i++)
    {
        if (fd_table[i].active && fd_table[i].fd == fd)
        {
            fd_table[i].active = 0;
            break;
        }
    }

    display_fd_table();

    printf("\nPress ENTER to continue...");
    getchar();
    getchar();
}

void close_all_fds(void)
{
    int i;
    int count = 0;

    printf("\n===============================================================\n");
    printf("                  CLOSE ALL FILE DESCRIPTORS\n");
    printf("===============================================================\n");

    for (i = 0; i < MAX_OPEN_FILES; i++)
    {
        if (fd_table[i].active)
        {
            if (close(fd_table[i].fd) == 0)
            {
                count++;
            }

            fd_table[i].active = 0;
        }
    }

    active_fd = -1;

    printf("\n%d file descriptor(s) closed successfully.\n", count);

    display_fd_table();

    printf("\nPress ENTER to continue...");
    getchar();
}
void set_active_fd_menu(void)
{
    int fd;

    printf("\n");
    printf("===============================================================\n");
    printf("                       SELECT ACTIVE FD\n");
    printf("===============================================================\n");

    display_fd_table();

    printf("\nEnter FD to make active: ");

    if (scanf("%d", &fd) != 1)
    {
        while (getchar() != '\n')
            ;

        printf("\nInvalid FD input.");
        printf("\nPress ENTER to continue...");
        getchar();
        return;
    }

    while (getchar() != '\n')
        ;

    if (!is_fd_registered(fd))
    {
        printf("\nERROR: FD %d is not registered.\n", fd);
        printf("Please select an open FD.\n");

        printf("\nPress ENTER to continue...");
        getchar();
        return;
    }

    set_active_fd(fd);

    printf("\n===============================================================\n");
    printf("                 ACTIVE FD UPDATED SUCCESSFULLY\n");
    printf("===============================================================\n");

    printf("\nActive FD : %d\n", fd);
    printf("File      : %s\n", get_fd_filename(fd));
    printf("Status    : ACTIVE\n");

    printf("\nUpdated FD Table:\n");
    display_fd_table();

    printf("\nPress ENTER to continue...");
    getchar();
}
