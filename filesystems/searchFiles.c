#include <dirent.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void searchFiles(const char *directory, const char *suffix, const char* prevDir)
{
    DIR *dir = opendir(directory);
    if (dir == NULL)
    {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_DIR)
        {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
            { // Skip "." and ".."
                char path[PATH_MAX];
                snprintf(path, PATH_MAX, "%s/%s", directory, entry->d_name);
                searchFiles(path, suffix, directory);
            }
        }
        else {
            if (entry->d_type == DT_LNK)
            {
                // if it is a symbolic link, we need to check if it is a directory
                char path[PATH_MAX];
                snprintf(path, PATH_MAX, "%s/%s", directory, entry->d_name);
                DIR *linkDir = opendir(path);
                if (linkDir != NULL)
                {
                    closedir(linkDir);
                    
                    if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
                    {
                        // if the link is a directory, we need to check if it is the same directory
                        // to avoid infinite loops
                        char realPath[PATH_MAX];
                        realpath(path, realPath);

                        char prevRealPath[PATH_MAX];
                        realpath(prevDir, prevRealPath);
                        if (prevDir != NULL && strcmp(realPath, prevRealPath) == 0)
                        {
                            continue;
                        }

                        searchFiles(path, suffix, directory);
                    }
                }

                continue;
            }
            else if (entry->d_type == DT_REG)
            {
                // check if the file has the desired suffix
                char *dot = strrchr(entry->d_name, '.');
                if (dot != NULL && strcmp(dot, suffix) == 0)
                {
                    printf("%s/%s\n", directory, entry->d_name);
                }
            }
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <directory> <suffix>\n", argv[0]);
        return 1;
    }

    searchFiles(argv[1], argv[2], NULL);

    return 0;
}