/*
 *
 * Copyright 2016 volodymyr shyshko (shyshko.vladimir@gmail.com)
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
*/

#include "utility.h"

char* read_file (const char* filename, size_t* length)
{
	int fd;
	struct stat file_info;
	char* buffer;
	
	/* Open the file.  */
	fd = open (filename, O_RDONLY);
	
	/* Get information about the file.  */
	fstat (fd, &file_info);
	*length = file_info.st_size;
	/* Make sure the file is an ordinary file.  */
	if (!S_ISREG (file_info.st_mode)) {
		/* It's not, so give up.  */
		close (fd);
		return NULL;
	}
	
	/* Allocate a buffer large enough to hold the file's contents.  */
	buffer = (char*) malloc (*length);
	/* Read the file into the buffer.  */
	read (fd, buffer, *length);
	
	/* Finish up.  */
	close (fd);
	return buffer;
}

void array_print(int *array, int size)
{
	for (int i = 0; i < size; i++) {
		printf("%d ", array[i]);
	}
}
