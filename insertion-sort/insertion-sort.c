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

#include <jansson.h>
#include <utility.h>

#define testfile "input.file"

void array_insertion_sort(int *array, int size)
{
	int count;
	for (int i = 1; i < size; i++) {
		int key = array[i];
		int j = i - 1;
		while (j >= 0 && array[j] > key) {
			array[j + 1] = array[j];
			j--;
			count++;
		}
		array[j + 1] = key;
	}
	printf("count %d\n", count);
}

int main()
{
	int *unsorted_arr = NULL;
	int elements;
	int json_unpack_res;
	char *data_type;
	FILE *input_f;

	json_t *obj = json_load_file("description.file", 0, 0);

	if (!obj) {
		fprintf(stderr, "%s failed\n", __func__);
		return EXIT_FAILURE;
	}
	json_unpack_res = json_unpack(obj,"{s:i}",// "type", &data_type,
			"length", &elements);

	if (json_unpack_res != -1) {
		printf("%d elements\n", elements);
	}

	unsorted_arr = malloc(elements * sizeof(int));
	input_f = fopen(testfile, "r");

	if (!input_f) {
		perror("error opening file");
		return(EXIT_FAILURE);
	}

	for (int i = 0; i < elements; i++) {
		fscanf(input_f, "%d", &unsorted_arr[i]);
	}

	printf("unsorted array\n");
	array_print(unsorted_arr, elements);
	array_insertion_sort(unsorted_arr, elements);
	printf("sorted array\n");
	array_print(unsorted_arr, elements);

	return EXIT_SUCCESS;
}
