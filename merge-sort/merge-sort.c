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

#include <string.h>
#include <jansson.h>
#include <utility.h>

#define testfile "input.file"

int* array_merge_sort(int *unsort_arr, int srt1_start, int srt2_start, int srt2_end)
{
	int *sorted_arr = malloc((srt2_end - srt1_start) * sizeof(int));
	int index1 = srt1_start, index2 = srt2_start, sorted_index = 0;

	for (; index1 < srt2_start && index2 <= srt2_end; sorted_index++) {
		if (unsort_arr[index1] < unsort_arr[index2]) {
			sorted_arr[sorted_index] = unsort_arr[index1];
			index1++;
		} else {
			sorted_arr[sorted_index] = unsort_arr[index2];
			index2++;
		}
	}
	if (index1 < srt2_start) {
		memcpy(sorted_arr + sorted_index, unsort_arr + index1, (srt2_start - index1) * sizeof(int));
	} else if(index2 < (srt2_end + 1)) {
		memcpy(sorted_arr + sorted_index, unsort_arr + index2, (srt2_end - index2 + 1) * sizeof(int));
	}
	array_print(sorted_arr, srt2_end - srt1_start + 1);
	free(sorted_arr);
}

int main()
{
	json_t *json_subarr;
	int sub_arr_number = 0;
	int *sub_arr_lengths;
	size_t index;
	json_t *value;
	json_t *obj = json_load_file("description.file", 0, 0);
	int json_unpack_res = 0;
	int whole_size = 0;
	FILE *input_f;

	if (!obj) {
		fprintf(stderr, "%s failed\n", __func__);
		return EXIT_FAILURE;
	}

	json_unpack_res = json_unpack(obj,"{s:i}",// "type", &data_type,
			"numb_of_elements", &sub_arr_number);

	if (json_unpack_res == -1) return EXIT_FAILURE;

	sub_arr_lengths = malloc(sub_arr_number * sizeof(int));

	json_subarr = json_object_get(obj, "sub_arr_length");

	if (json_unpack_res == -1) return EXIT_FAILURE;

	json_array_foreach(json_subarr, index, value) {
		sub_arr_lengths[index] = json_integer_value(value);
		printf("index %d len  %d \n", (int) index, sub_arr_lengths[index]);
	}

	for (int i = 0; i < sub_arr_number; i++) {
		whole_size += sub_arr_lengths[i];
	}

	int *unsorted_arr = malloc(whole_size * sizeof(int));

	input_f = fopen(testfile, "r");

	if (!input_f) {
		perror("error opening file");
		return(EXIT_FAILURE);
	}

	for (int i = 0; i < whole_size; i++) {
		fscanf(input_f, "%d", &unsorted_arr[i]);
	}

	int srt2_start = 0, srt2_end = sub_arr_lengths[0];
	for (int i = 0; i < sub_arr_number - 1; i++) {
		srt2_start += sub_arr_lengths[i];
		srt2_end += sub_arr_lengths[i + 1] - 1;
		printf("srt2 start %d srt2end %d\n", srt2_start, srt2_end);
		array_merge_sort(unsorted_arr, 0, srt2_start, srt2_end);
	}
	free(unsorted_arr);
	return EXIT_SUCCESS;
}
