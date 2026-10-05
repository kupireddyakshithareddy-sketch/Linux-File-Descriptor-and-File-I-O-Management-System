#ifndef FD_MANAGER_H
#define FD_MANAGER_H

#define MAX_OPEN_FILES 100

void register_file_descriptor(int fd, const char *filename);
void display_fd_table(void);
void duplicate_fd_menu(void);
void duplicate_fd2_menu(void);
void close_fd_menu(void);
void close_all_fds(void);

int is_fd_registered(int fd);

void set_active_fd(int fd);
int get_active_fd(void);

const char *get_fd_filename(int fd);
void set_active_fd_menu(void);
#endif
