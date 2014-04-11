#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <sys/types.h>
#include <dirent.h>

#include "utils.h"

inline bool string_isnumber(const char *str)
{
	bool ret = true;
	const char *p = str;

	for (; *p != '\0'; p++) {
		if (!isdigit(*p)) {
			ret = false;
			break;
		}
	}

	return ret;
}

pid_t get_pid_by_name(const char *proc_name)
{	
	DIR *dp;
	struct dirent *dirp;
	pid_t target = 0;

	if ((dp = opendir("/proc")) == NULL)
		return -1;

	while ((dirp = readdir(dp)) != NULL) {
		if (strncmp(dirp->d_name, ".", 1) == 0
		    || strncmp(dirp->d_name, "..", 2) == 0)
			continue;

		if (string_isnumber(dirp->d_name) 
		    && dirp->d_type == DT_DIR) {
			char path[32] = {0}, buf[64] = {0};
			FILE *fin = NULL;
			pid_t pid = atoi(dirp->d_name);

			snprintf(path, sizeof(path), 
			         "/proc/%s/cmdline", dirp->d_name);
			if ((fin = fopen(path, "r")) == NULL)
				continue;
			if ((fgets(buf, sizeof(buf), fin)) == NULL) {
				fclose(fin);
				continue;
			}

			if (strncmp(proc_name, buf, strlen(buf)) == 0
			    && strlen(proc_name) == strlen(buf))
				return pid;
		}
	}

	return -2;
}

pid_t get_ppid(pid_t pid)
{
	FILE *fin = NULL;
	char proc_path[256] = {0};
	char tmp[64] = {0};
	int ppid = -1;

	snprintf(proc_path, sizeof(proc_path), 
	         "/proc/%d/stat", pid);

	if ((fin = fopen(proc_path, "r")) == NULL)
		return -1;
	
	fscanf(fin, "%s %s %s %d", tmp, tmp, tmp, &ppid);

	fclose(fin);

	return ppid;
}

uid_t get_uid_by_pid(pid_t pid)
{
	FILE *fin = NULL;
	char proc_path[256] = {0}, line[256] = {0};
	uid_t uid = -1;

	snprintf(proc_path, sizeof(proc_path),
	         "/proc/%d/status", pid);
	if ((fin = fopen(proc_path, "r")) == NULL)
		return -1;
	
	while ((fgets(line, sizeof(line), fin)) != NULL) {
		if (strncmp(line, "Uid:", 4) == 0) {
			sscanf(line, "Uid:\t%d", &uid);
			break;
		}
		memset(line, 0, sizeof(line));
	}

	fclose(fin);

	return uid;
}

int get_name_by_pid(pid_t pid, char *procname, int len)
{
	FILE *fin = NULL;
	char path[256] = {0}, line[256] = {0};
	int ret;

	memset(procname, 0, len);
	snprintf(path, sizeof(path), "/proc/%d/cmdline", pid);

	if ((fin = fopen(path, "r")) == NULL)
		return -1;

	if (fgets(line, sizeof(line), fin)) {
		strncpy(procname, line, len>strlen(line)?strlen(line):len);
		ret = 0;
	} else {
		ret = 1;
	}

	return ret;
}
