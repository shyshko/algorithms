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
//#include <utility.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>

#define DEFAULT_LENGTH 20
#define DESCR_FILE "description.file"
#define INPUT_FILE "input.file"

struct unsorted_array {
	int length;
	int max_value;
	int min_value;
};

struct sorted_array {
	int nums;
	int max_value;
	int min_value;
	int sub_arrays_length[];
};

static int is_random;

static void usage()
{
	fprintf(stderr,
"Usage: generator OPTIONS  | help\n"
"-t[type] of test data sorted unsorted\n"
"-l[length] length\n"
"-M[max_value]\n"
"-m[min_value]\n"
"-n[num_of_arrs]\n"
"random | fixed way to generate length\n");

	exit(1);
}

static int cmpfunc (const void * a, const void * b)
{
	return ( *(int*)a - *(int*)b );
}

static void gen_unsorted_array(struct unsorted_array descr)
{
	FILE *f = fopen(INPUT_FILE, "w");

	if (!f) {
		perror("fopen");
		return;
	}

	for (int i = 0; i < descr.length; i++) {
		int value = rand() % (descr.max_value + 1 + abs(descr.min_value))
			+ descr.min_value;
		fprintf(f, "%d ", value);
	}
	fclose(f);
}

static void gen_sorted_array(struct sorted_array *descr)
{
	FILE *f = fopen(INPUT_FILE, "w");

	if (!f) {
		perror("fopen");
		return;
	}

	for (int i = 0; i < descr->nums; i++) {
		int *unsorted_arr = malloc(descr->sub_arrays_length[i] * sizeof(int));
		for (int j = 0; j < descr->sub_arrays_length[i]; j++) {
			unsorted_arr[j] = rand() % (descr->max_value + 1 +
					abs(descr->min_value)) + descr->min_value;
		}
		qsort(unsorted_arr, descr->sub_arrays_length[i], sizeof(int), cmpfunc);
		int *sorted_arr = unsorted_arr;
		for (int k = 0; k < descr->sub_arrays_length[i]; k++) {
			fprintf(f, "%d ", sorted_arr[k]);
		}
		free(sorted_arr);
	}
	fclose(f);
}

static void gen_description_file_unsorted(struct unsorted_array descr)
{
	json_t *obj;

	obj = json_pack("{s:s,s:i,s:i,s:i}",
			"type", "unsorted",
			"length", descr.length,
			"max_value", descr.max_value,
			"min_value", descr.min_value);

	if (!obj) {
		printf("obj is NULL\n");
		return;
	}

	json_dump_file(obj, DESCR_FILE, JSON_INDENT(4));
	json_decref(obj);
}

static void gen_description_file_sorted(struct sorted_array *descr)
{
	json_t *obj;
	json_t *json_arr = json_array();
	obj = json_pack("{s:s, s:i,s:i,s:i}",
			"type", "sorted",
			"numb_of_elements", descr->nums,
			"max_value", descr->max_value,
			"min_value", descr->min_value);

	if (!obj) {
		printf("obj is NULL\n");
		return;
	}

	json_object_set_new(obj, "sub_arr_length", json_arr);
	for (int i = 0; i < descr->nums; i++) {
		json_array_append(json_arr, json_integer(descr->sub_arrays_length[i]));
	}
	json_dump_file(obj, DESCR_FILE, JSON_INDENT(4));
	json_decref(obj);
}

int main(int argc, char *argv[])
{
	int length = DEFAULT_LENGTH;
	char *data_type = NULL;
	int max_value = 0X7FFFFFFF;
	int min_value = 0XF0000000;
	int number_of_arrays = 2;

	int c;

	while (1) {
		static struct option long_options[] = {
			{"type",    required_argument, 0,    't'},
			{"length",  optional_argument, 0,    'l'},
			{"max_val", optional_argument, 0,    'M'},
			{"min_val", optional_argument, 0,    'm'},
			{"num_of_arrs", optional_argument, 0,'n'},
			{"random",  no_argument, &is_random,  1},
			{"fixed",   no_argument, &is_random,  0},
			{0, 0, 0, 0}
		};
		/* getopt_long stores the option index here. */
		int option_index = 0;

		c = getopt_long (argc, argv, "t:l:m:M:n:",
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
			case 't':
				data_type = optarg;
				break;
			case 'l':
				length = atoi(optarg);
				break;
			case 'm':
				min_value = atoi(optarg);
				break;
			case 'M':
				max_value = atoi(optarg);
				break;
			case 'n':
				number_of_arrays = atoi(optarg);
				break;
			default:
				usage();
				abort ();
		}
	}

	if (!data_type) {
		usage();
		abort();
	}

	if (strcmp(data_type, "unsorted") == 0) {
		struct unsorted_array ua;
		ua.length = length;
		ua.max_value = max_value;
		ua.min_value = min_value;
		gen_description_file_unsorted(ua);
		gen_unsorted_array(ua);
	} else if (strcmp(data_type, "sorted") == 0) {
		struct sorted_array *sa;
		sa = malloc(sizeof(struct sorted_array) +
				number_of_arrays * sizeof(int));
		sa->max_value = max_value;
		sa->min_value = min_value;
		sa->nums = number_of_arrays;
		for (int i = 0; i < number_of_arrays; i++) {
			sa->sub_arrays_length[i] = is_random ? rand() % length : length;
		}
		gen_description_file_sorted(sa);
		gen_sorted_array(sa);
		free(sa);
	}
	return 1;
}
