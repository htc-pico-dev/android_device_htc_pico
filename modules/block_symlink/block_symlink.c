/*
 * Copyright (C) 2018 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MTD_PROC_FILENAME "/proc/mtd"
#define MTD_SYMLINK_DIR "/dev/block/mtd/by-name"

#define S_IRUGO (S_IRUSR | S_IRGRP | S_IROTH)
#define S_IXUGO (S_IXUSR | S_IXGRP | S_IXOTH)

int main() {
	char buf[2048];
	const char *bufp;
	int fd;
	ssize_t nbytes;

	char mtd_src_path[PATH_MAX];
	char mtd_symlink_path[PATH_MAX];

	struct stat stat_buf;

	if (stat(MTD_SYMLINK_DIR, &stat_buf) != 0) {
		mkdir("/dev/block/mtd", S_IRUGO | S_IWUSR | S_IXUGO);
		mkdir("/dev/block/mtd/by-name", S_IRUGO | S_IWUSR | S_IXUGO);
	}

	fd = open(MTD_PROC_FILENAME, O_RDONLY);
	if (fd < 0) {
		goto bail;
	}

	nbytes = TEMP_FAILURE_RETRY(read(fd, buf, sizeof(buf) - 1));
	close(fd);
	if (nbytes < 0) {
		goto bail;
	}
	buf[nbytes] = '\0';

	bufp = buf;
	while (nbytes > 0) {
		int mtdnum, mtdsize, mtderasesize;
		int matches;
		char mtdname[64];
		mtdname[0] = '\0';
		mtdnum = -1;

		matches = sscanf(bufp, "mtd%d: %x %x \"%63[^\"]",
			&mtdnum, &mtdsize, &mtderasesize, mtdname);
		/* This will fail on the first line, which just contains
		* column headers.
		*/
		if (matches == 4) {
			sprintf(mtd_src_path, "/dev/block/mtdblock%d", mtdnum);
			sprintf(mtd_symlink_path, MTD_SYMLINK_DIR "/%s", mtdname);

			symlink(mtd_src_path, mtd_symlink_path);

			fprintf(stdout, "%s -> %s\n", mtd_symlink_path, mtd_src_path);
		}

		/* Eat the line.
		*/
		while (nbytes > 0 && *bufp != '\n') {
			bufp++;
			nbytes--;
		}
		if (nbytes > 0) {
			bufp++;
			nbytes--;
		}
	}

bail:
	if (stat(MTD_SYMLINK_DIR "/system", &stat_buf) != 0) {
		fprintf(stderr, "making default system device symlink\n");
		symlink("/dev/block/mtdblock3", MTD_SYMLINK_DIR "/system");
	}

	if (stat(MTD_SYMLINK_DIR "/userdata", &stat_buf) != 0) {
		fprintf(stderr, "making default userdata device symlink\n");
		symlink("/dev/block/mtdblock5", MTD_SYMLINK_DIR "/userdata");
	}

	if (stat(MTD_SYMLINK_DIR "/cache", &stat_buf) != 0) {
		fprintf(stderr, "making default cache device symlink\n");
		symlink("/dev/block/mtdblock4", MTD_SYMLINK_DIR "/cache");
	}

	return 0;
}
