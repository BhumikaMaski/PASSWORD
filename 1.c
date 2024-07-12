// Write A C Program To Truncate The Files In The Directory 
// Created After A Certain Date To Half Its Original Size. 
// Inputs To The Program : Directory And Date as run time arguments 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
int isFile(const char *path);
void truncateFiles(const char *dirPath, time_t cutoffDate);

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <directory> <date>\n", argv[0]);
        return 1;
    }
    const char *directory = argv[1];
    const char *dateStr = argv[2];
    struct tm tm;
    if (strptime(dateStr, "%Y-%m-%d", &tm) == NULL)
    {
        printf("Invalid date format. Use YYYY-MM-DD format.\n");
        return 1;
    }
    tm.tm_hour = 0;
    tm.tm_min = 0;
    tm.tm_sec = 0;
    time_t cutoffDate = mktime(&tm);
    truncateFiles(directory, cutoffDate);
    printf("Files truncated successfully.\n");
    return 0;
}
int isFile(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}
void truncateFiles(const char *dirPath, time_t cutoffDate)
{
    DIR *dir;
    struct dirent *ent;
    dir = opendir(dirPath);
    if (dir != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            char filePath[512];
            snprintf(filePath, sizeof(filePath), "%s/%s", dirPath, ent->d_name);
            if (isFile(filePath))
            {
                struct stat fileInfo;
                if (stat(filePath, &fileInfo) != 0)
                {
                    perror("Error getting file information");
                    continue;
                }
                if (difftime(fileInfo.st_mtime, cutoffDate) > 0)
                {
                    off_t originalSize = fileInfo.st_size;
                    off_t newSize = originalSize / 2;
                    truncate(filePath, newSize);
                    printf("Truncated file: %s\n", filePath);
                }
            }
        }
        closedir(dir);
    }
    else
    {
        perror("Error opening directory");
    }
}
