#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include "../include/advanced_io.h"
#include "../include/fd_manager.h"

static void pause_screen(void)
{
    int ch;

    printf("\nPress ENTER to continue...");
    fflush(stdout);

    while ((ch = getchar()) != '\n' && ch != EOF)
    {
        /* Clear input */
    }

    getchar();
}

void display_fd_flags_menu(void)
{
    int fd;
    int flags;

    printf("\n");
    printf("===============================================================\n");
    printf("                       FD FLAGS INFORMATION\n");
    printf("===============================================================\n");

    display_fd_table();

    if (get_active_fd() == -1)
    {
        printf("\nNo active file descriptor is selected.\n");
        printf("Please open a file first.\n");

        pause_screen();
        return;
    }

    printf("\nActive File Information\n");
    printf("File Descriptor : %d\n", get_active_fd());
    printf("File            : %s\n",
           get_fd_filename(get_active_fd()));

    printf("\nEnter FD to inspect: ");

    if (scanf("%d", &fd) != 1)
    {
        while (getchar() != '\n')
            ;

        printf("\nInvalid FD input.\n");
        pause_screen();
        return;
    }

    while (getchar() != '\n')
        ;

    if (!is_fd_registered(fd))
    {
        printf("\nFD %d is not registered.\n", fd);
        pause_screen();
        return;
    }

    flags = fcntl(fd, F_GETFL);

    if (flags == -1)
    {
        perror("\nfcntl(F_GETFL) failed");
        pause_screen();
        return;
    }

    printf("\n===============================================================\n");
    printf("                    FD FLAGS DETAILS\n");
    printf("===============================================================\n");

    printf("\nFile Descriptor : %d\n", fd);
    printf("File            : %s\n", get_fd_filename(fd));

    printf("\nAccess Mode:\n");

    if ((flags & O_RDWR) == O_RDWR)
    {
        printf("Read/Write      : YES\n");
    }
    else if (flags & O_WRONLY)
    {
        printf("Write Only      : YES\n");
    }
    else
    {
        printf("Read Only       : YES\n");
    }

    printf("\nFile Status Flags:\n");
    printf("O_APPEND        : %s\n",
           (flags & O_APPEND) ? "ON" : "OFF");

    printf("O_NONBLOCK      : %s\n",
           (flags & O_NONBLOCK) ? "ON" : "OFF");

    printf("O_SYNC          : %s\n",
           (flags & O_SYNC) ? "ON" : "OFF");

    printf("O_DSYNC         : %s\n",
           (flags & O_DSYNC) ? "ON" : "OFF");

    printf("\nRaw Flags Value : %d\n", flags);
    printf("Operation       : fcntl(F_GETFL)\n");
    printf("Status          : SUCCESS\n");

    pause_screen();
}
void file_locking_menu(void)
{
    int fd;
    int choice;
    struct flock lock;

    printf("\n");
    printf("===============================================================\n");
    printf("                         FILE LOCKING\n");
    printf("===============================================================\n");

    display_fd_table();

    if (get_active_fd() == -1)
    {
        printf("\nNo active file descriptor is selected.\n");
        printf("Please open a file first.\n");

        pause_screen();
        return;
    }

    printf("\nActive File Information\n");
    printf("File Descriptor : %d\n", get_active_fd());
    printf("File            : %s\n",
           get_fd_filename(get_active_fd()));

    printf("\nEnter FD to lock: ");

    if (scanf("%d", &fd) != 1)
    {
        while (getchar() != '\n')
            ;

        printf("\nInvalid FD input.\n");
        pause_screen();
        return;
    }

    while (getchar() != '\n')
        ;

    if (!is_fd_registered(fd))
    {
        printf("\nFD %d is not registered.\n", fd);
        pause_screen();
        return;
    }

    printf("\n");
    printf("1. Acquire Read Lock\n");
    printf("2. Acquire Write Lock\n");
    printf("3. Unlock File\n");

    printf("\nEnter locking choice: ");

    if (scanf("%d", &choice) != 1)
    {
        while (getchar() != '\n')
            ;

        printf("\nInvalid choice.\n");
        pause_screen();
        return;
    }

    while (getchar() != '\n')
        ;

    lock.l_type = F_UNLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();

    if (choice == 1)
    {
        lock.l_type = F_RDLCK;
    }
    else if (choice == 2)
    {
        lock.l_type = F_WRLCK;
    }
    else if (choice == 3)
    {
        lock.l_type = F_UNLCK;
    }
    else
    {
        printf("\nInvalid locking choice.\n");
        pause_screen();
        return;
    }

    if (fcntl(fd, F_SETLK, &lock) == -1)
    {
        perror("\nfcntl(F_SETLK) failed");

        printf("\nLocking Operation : FAILED\n");

        pause_screen();
        return;
    }

    printf("\n");
    printf("===============================================================\n");
    printf("                    FILE LOCK OPERATION\n");
    printf("===============================================================\n");

    printf("\nFile Descriptor : %d\n", fd);
    printf("File            : %s\n",
           get_fd_filename(fd));

    if (choice == 1)
    {
        printf("Lock Type       : READ LOCK\n");
        printf("Lock Status     : ACQUIRED\n");
    }
    else if (choice == 2)
    {
        printf("Lock Type       : WRITE LOCK\n");
        printf("Lock Status     : ACQUIRED\n");
    }
    else
    {
        printf("Lock Type       : UNLOCK\n");
        printf("Lock Status     : RELEASED\n");
    }

    printf("Operation       : fcntl(F_SETLK)\n");
    printf("Status          : SUCCESS\n");

    pause_screen();
}
void synchronize_file_menu(void)
{
    int fd;

    printf("\n");
    printf("===============================================================\n");
    printf("                       SYNCHRONIZE FILE\n");
    printf("===============================================================\n");

    display_fd_table();

    if (get_active_fd() == -1)
    {
        printf("\nNo active file descriptor is selected.\n");
        printf("Please open a file first.\n");
        pause_screen();
        return;
    }

    printf("\nActive File Information\n");
    printf("File Descriptor : %d\n", get_active_fd());
    printf("File            : %s\n", get_fd_filename(get_active_fd()));

    printf("\nEnter FD to synchronize: ");

    if (scanf("%d", &fd) != 1)
    {
        while (getchar() != '\n')
            ;

        printf("\nInvalid FD input.\n");
        pause_screen();
        return;
    }

    while (getchar() != '\n')
        ;

    if (!is_fd_registered(fd))
    {
        printf("\nFD %d is not registered.\n", fd);
        pause_screen();
        return;
    }

    printf("\nSynchronizing file...\n");

    if (fsync(fd) == -1)
    {
        perror("\nfsync() failed");

        printf("\n===============================================================\n");
        printf("                    SYNCHRONIZATION RESULT\n");
        printf("===============================================================\n");

        printf("\nFile Descriptor : %d\n", fd);
        printf("File            : %s\n", get_fd_filename(fd));
        printf("Operation       : fsync()\n");
        printf("Status          : FAILED\n");

        pause_screen();
        return;
    }

    printf("\n");
    printf("===============================================================\n");
    printf("                    SYNCHRONIZATION RESULT\n");
    printf("===============================================================\n");

    printf("\nFile Descriptor : %d\n", fd);
    printf("File            : %s\n", get_fd_filename(fd));
    printf("Operation       : fsync()\n");
    printf("Status          : SUCCESS\n");
    printf("Synchronization : File data flushed successfully\n");

    pause_screen();
}
void truncate_file_menu(void)
{
    long long new_size;
    off_t current_size;
    off_t old_size;
    int active_fd;

    printf("\n");
    printf("===============================================================\n");
    printf("                         TRUNCATE FILE\n");
    printf("===============================================================\n");

    display_fd_table();

    active_fd = get_active_fd();

    if (active_fd == -1)
    {
        printf("\nNo active file descriptor is selected.\n");
        printf("Please open a file first.\n");
        pause_screen();
        return;
    }

    printf("\nActive File Information\n");
    printf("File Descriptor : %d\n", active_fd);
    printf("File            : %s\n", get_fd_filename(active_fd));

    current_size = lseek(active_fd, 0, SEEK_END);

    if (current_size == (off_t)-1)
    {
        perror("\nlseek() failed");
        pause_screen();
        return;
    }

    if (lseek(active_fd, 0, SEEK_SET) == (off_t)-1)
    {
        perror("\nlseek() failed");
        pause_screen();
        return;
    }

    printf("Current File Size: %lld bytes\n",
           (long long)current_size);

    printf("\nEnter new file size in bytes: ");

    if (scanf("%lld", &new_size) != 1)
    {
        while (getchar() != '\n')
            ;

        printf("\nInvalid file size.\n");
        pause_screen();
        return;
    }

    while (getchar() != '\n')
        ;

    if (new_size < 0)
    {
        printf("\nFile size cannot be negative.\n");
        pause_screen();
        return;
    }

    old_size = current_size;

    printf("\nTruncating file...\n");

    if (ftruncate(active_fd, (off_t)new_size) == -1)
    {
        perror("\nftruncate() failed");

        printf("\n");
        printf("===============================================================\n");
        printf("                     TRUNCATION RESULT\n");
        printf("===============================================================\n");

        printf("\nFile Descriptor : %d\n", active_fd);
        printf("File            : %s\n",
               get_fd_filename(active_fd));
        printf("Old File Size   : %lld bytes\n",
               (long long)old_size);
        printf("Requested Size  : %lld bytes\n",
               new_size);
        printf("Operation       : ftruncate()\n");
        printf("Status          : FAILED\n");

        pause_screen();
        return;
    }

    printf("\n");
    printf("===============================================================\n");
    printf("                     TRUNCATION RESULT\n");
    printf("===============================================================\n");

    printf("\nFile Descriptor : %d\n", active_fd);
    printf("File            : %s\n",
           get_fd_filename(active_fd));
    printf("Old File Size   : %lld bytes\n",
           (long long)old_size);
    printf("New File Size   : %lld bytes\n",
           new_size);
    printf("Operation       : ftruncate()\n");
    printf("Status          : SUCCESS\n");

    pause_screen();
}
