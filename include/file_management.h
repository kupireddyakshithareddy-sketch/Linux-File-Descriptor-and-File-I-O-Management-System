#ifndef FILE_MANAGEMENT_H
#define FILE_MANAGEMENT_H

void file_management_menu(void);
void list_saved_files_management_menu(void);
void rename_file_menu(void);
void delete_file_menu(void);
void create_directory_menu(void);
void change_file_permissions_menu(void);

void log_operation(const char *operation, const char *details);
void view_operation_history_menu(void);
#endif
