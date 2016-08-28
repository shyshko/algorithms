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
#include <getopt.h>

#define testfile "input.file"


static int icrsng_sort;

static void usage()
{
	fprintf(stderr,
"Usage: insertion_sort [OPTIONS]  | help\n"
"--incr_sort [--i] | --decr_sort [--d] incrising or decrising sort\n");

	exit(1);
}

static void fnd_place(int *sorted_arr, int key_pos)
{
	int key = sorted_arr[key_pos];
	int j = key_pos - 1;
	while (j >= 0 && (icrsng_sort && sorted_arr[j] > key) ||
			(!icrsng_sort && sorted_arr[j] < key)) { /* Find the place for current element */
		sorted_arr[j + 1] = sorted_arr[j];
		j--;
	}
	sorted_arr[j + 1] = key;
}

static void insrtn_sort(int *array, int size)
{
	for (int i = 1; i < size; i++) { /* Loop through unsorted elements */
		fnd_place(array, i);
	}
}

int main(int argc, char *argv[])
{
	int *unsorted_arr = NULL;
	int elements;
	int json_unpack_res;
	char *data_type;
	FILE *input_f;

	int c;

	while (1) {
		static struct option long_options[] = {
			{"incr_sort",   no_argument, &icrsng_sort, 1},
			{"decr_sort",   no_argument, &icrsng_sort, 0},
			{0, 0, 0, 0}
		};
		/* getopt_long stores the option index here. */
		int option_index = 0;

		c = getopt_long (argc, argv, "i:d:",
		                 long_options, &option_index);

		/* Detect the end of the options. */
		if (c == -1)
		  break;

		switch (c) {

        case 0:
			/* If this option set a flag, do nothing else now. */
			if (long_options[option_index].flag != 0)
				break;
			printf ("option %s", long_options[option_index].name);
			if (optarg)
				printf (" with arg %s", optarg);
			printf ("\n");
			break;
			default:
				usage();
				abort();
		}
	}
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

	printf("sort type %d\n", icrsng_sort);

	printf("unsorted array\n");
	array_print(unsorted_arr, elements);
	insrtn_sort(unsorted_arr, elements);
	printf("sorted array\n");
	array_print(unsorted_arr, elements);

	return EXIT_SUCCESS;
}
