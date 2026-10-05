#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#include "../include/file_info.h"
#include "../include/fd_manager.h"

void display_complete_file_info_menu(void)
{
    int fd;
    struct stat file_stat;

    printf("\n");
    printf("===============================================================\n");
    printf("                 COMPLETE FILE INFORMATION\n");
    printf("===============================================================\n");

    display_fd_table();

    if (get_active_fd() == -1)
    {
        printf("\nNo active file is selected.\n");
        printf("Please open or select a file first.\n");

        printf("\nPress ENTER to continue...");
        getchar();
        return;
    }

    printf("\nActive File Information\n");
    printf("File Descriptor : %d\n", get_active_fd());
    printf("File            : %s\n",
           get_fd_filename(get_active_fd()));

    printf("\nEnter FD: ");

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

        printf("\nPress ENTER to continue...");
        getchar();
        return;
    }

    if (fstat(fd, &file_stat) == -1)
    {
        perror("\nfstat() failed");

        printf("\nPress ENTER to continue...");
        getchar();
        return;
    }

    printf("\n===============================================================\n");
    printf("                 FILE INFORMATION\n");
    printf("===============================================================\n");

    printf("\nFile Descriptor : %d\n", fd);
    printf("File            : %s\n",
           get_fd_filename(fd));

    printf("\nFile Size       : %ld bytes\n",
           (long)file_stat.st_size);

    printf("Inode Number    : %ld\n",
           (long)file_stat.st_ino);

    printf("Device ID       : %ld\n",
           (long)file_stat.st_dev);

    printf("Hard Links      : %ld\n",
           (long)file_stat.st_nlink);

    printf("Owner UID       : %ld\n",
           (long)file_stat.st_uid);

    printf("Group GID       : %ld\n",
           (long)file_stat.st_gid);

    printf("Permissions     : %04o\n",
           file_stat.st_mode & 0777);

    printf("\nFile Type       : ");

    if (S_ISREG(file_stat.st_mode))
        printf("Regular File\n");
    else if (S_ISDIR(file_stat.st_mode))
        printf("Directory\n");
    else if (S_ISLNK(file_stat.st_mode))
        printf("Symbolic Link\n");
    else
        printf("Other\n");

    printf("\nOperation       : fstat()\n");
    printf("Status          : SUCCESS\n");

    printf("\nPress ENTER to continue...");
    getchar();
}
void display_file_permissions_menu(void)
{
    int fd;
    struct stat file_stat;

    printf("\n");
    printf("===============================================================\n");
    printf("                    FILE PERMISSIONS\n");
    printf("===============================================================\n");

    display_fd_table();

    if (get_active_fd() == -1)
    {
        printf("\nNo active file is selected.\n");
        printf("Please open or select a file first.\n");

        printf("\nPress ENTER to continue...");
        getchar();
        return;
    }

    printf("\nEnter FD: ");

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

        printf("\nPress ENTER to continue...");
        getchar();
        return;
    }

    if (fstat(fd, &file_stat) == -1)
    {
        perror("\nfstat() failed");

        printf("\nPress ENTER to continue...");
        getchar();
        return;
    }

    printf("\n===============================================================\n");
    printf("                 FILE PERMISSION DETAILS\n");
    printf("===============================================================\n");

    printf("\nFile Descriptor : %d\n", fd);
    printf("File            : %s\n",
           get_fd_filename(fd));

    printf("\nNumeric Permissions : %04o\n",
           file_stat.st_mode & 0777);

    printf("\nOwner Permissions\n");
    printf("Read    : %s\n",
           (file_stat.st_mode & S_IRUSR) ? "YES" : "NO");
    printf("Write   : %s\n",
           (file_stat.st_mode & S_IWUSR) ? "YES" : "NO");
    printf("Execute : %s\n",
           (file_stat.st_mode & S_IXUSR) ? "YES" : "NO");

    printf("\nGroup Permissions\n");
    printf("Read    : %s\n",
           (file_stat.st_mode & S_IRGRP) ? "YES" : "NO");
    printf("Write   : %s\n",
           (file_stat.st_mode & S_IWGRP) ? "YES" : "NO");
    printf("Execute : %s\n",
           (file_stat.st_mode & S_IXGRP) ? "YES" : "NO");

    printf("\nOthers Permissions\n");
    printf("Read    : %s\n",
           (file_stat.st_mode & S_IROTH) ? "YES" : "NO");
    printf("Write   : %s\n",
           (file_stat.st_mode & S_IWOTH) ? "YES" : "NO");
    printf("Execute : %s\n",
           (file_stat.st_mode & S_IXOTH) ? "YES" : "NO");

    printf("\nOperation : fstat()\n");
    printf("Status    : SUCCESS\n");

    printf("\nPress ENTER to continue...");
    getchar();
}
void display_file_size_menu(void)
{
    int fd;
    struct stat file_stat;

    printf("\n");
    printf("===============================================================\n");
    printf("                       FILE SIZE\n");
    printf("===============================================================\n");

    display_fd_table();

    if (get_active_fd() == -1)
    {
        printf("\nNo active file is selected.\n");
        printf("Please open or select a file first.\n");

        printf("\nPress ENTER to continue...");
        getchar();
        return;
    }

    printf("\nEnter FD: ");

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

        printf("\nPress ENTER to continue...");
        getchar();
        return;
    }

    if (fstat(fd, &file_stat) == -1)
    {
        perror("\nfstat() failed");

        printf("\nPress ENTER to continue...");
        getchar();
        return;
    }

    printf("\n===============================================================\n");
    printf("                    FILE SIZE INFORMATION\n");
    printf("===============================================================\n");

    printf("\nFile Descriptor : %d\n", fd);
    printf("File            : %s\n",
           get_fd_filename(fd));

    printf("File Size       : %ld bytes\n",
           (long)file_stat.st_size);

    printf("\nOperation       : fstat()\n");
    printf("Status          : SUCCESS\n");

    printf("\nPress ENTER to continue...");
    getchar();
}
void display_current_position_menu(void)
{
    int fd;
    off_t position;

    printf("\n");
    printf("===============================================================\n");
    printf("                 CURRENT FILE POSITION\n");
    printf("===============================================================\n");

    display_fd_table();

    if (get_active_fd() == -1)
    {
        printf("\nNo active file is selected.\n");
        printf("Please open or select a file first.\n");

        printf("\nPress ENTER to continue...");
        getchar();
        return;
    }

    printf("\nEnter FD: ");

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

        printf("\nPress ENTER to continue...");
        getchar();
        return;
    }

    position = lseek(fd, 0, SEEK_CUR);

    if (position == (off_t)-1)
    {
        perror("\nlseek() failed");

        printf("\nPress ENTER to continue...");
        getchar();
        return;
    }

    printf("\n===============================================================\n");
    printf("              CURRENT FILE POSITION INFORMATION\n");
    printf("===============================================================\n");

    printf("\nFile Descriptor : %d\n", fd);
    printf("File            : %s\n",
           get_fd_filename(fd));

    printf("Current Position: %ld bytes\n",
           (long)position);

    printf("\nOperation       : lseek()\n");
    printf("Status          : SUCCESS\n");

    printf("\nPress ENTER to continue...");
    getchar();
}
void display_file_type_menu(void)
{
    int fd;
    struct stat file_stat;

    printf("\n");
    printf("===============================================================\n");
    printf("                       FILE TYPE\n");
    printf("===============================================================\n");

    display_fd_table();

    if (get_active_fd() == -1)
    {
        printf("\nNo active file is selected.\n");
        printf("Please open or select a file first.\n");

        printf("\nPress ENTER to continue...");
        getchar();
        return;
    }

    printf("\nEnter FD: ");

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

        printf("\nPress ENTER to continue...");
        getchar();
        return;
    }

    if (fstat(fd, &file_stat) == -1)
    {
        perror("\nfstat() failed");

        printf("\nPress ENTER to continue...");
        getchar();
        return;
    }

    printf("\n===============================================================\n");
    printf("                    FILE TYPE INFORMATION\n");
    printf("===============================================================\n");

    printf("\nFile Descriptor : %d\n", fd);
    printf("File            : %s\n",
           get_fd_filename(fd));

    printf("\nFile Type       : ");

    if (S_ISREG(file_stat.st_mode))
        printf("Regular File\n");
    else if (S_ISDIR(file_stat.st_mode))
        printf("Directory\n");
    else if (S_ISLNK(file_stat.st_mode))
        printf("Symbolic Link\n");
    else if (S_ISCHR(file_stat.st_mode))
        printf("Character Device\n");
    else if (S_ISBLK(file_stat.st_mode))
        printf("Block Device\n");
    else if (S_ISFIFO(file_stat.st_mode))
        printf("FIFO / Named Pipe\n");
    else if (S_ISSOCK(file_stat.st_mode))
        printf("Socket\n");
    else
        printf("Unknown\n");

    printf("\nOperation       : fstat()\n");
    printf("Status          : SUCCESS\n");

    printf("\nPress ENTER to continue...");
    getchar();
}
