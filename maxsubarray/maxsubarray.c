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
#include <string.h>
#include <limits.h>

#define testfile "input.file"

struct subarr {
	int max_left;
	int max_right;
	int sum;
};

static struct subarr find_max_crossing_subarr(int *arr, int low, int mid, int high)
{
	int left_sum = INT_MIN;
	int right_sum = INT_MIN;
	int sum = 0;
	int max_left = 0, max_right = 0;
	struct subarr result;

	memset(&result, 0, sizeof result);
	for (int i = mid; i >= low; i--) {
		sum += arr[i];
		if (sum > left_sum) {
			left_sum = sum;
			max_left = i;
		}
	}
	sum = 0;
	for (int j = mid + 1; j <= high; j++) {
		sum += arr[j];
		if (sum > right_sum) {
			right_sum = sum;
			max_right = j;
		}
	}
	result.max_left = max_left;
	result.max_right = max_right;
	result.sum = right_sum + left_sum;
	return result;
}

static struct subarr find_max_subarray(int *input_arr, int low, int high)
{

	if (low == high) {
		struct subarr result;
		result.max_left = low;
		result.max_right = high;
		result.sum = input_arr[low];
		return result;
	} else {
		struct subarr left, cross, right;
		int mid = (low + high) / 2;
		left = find_max_subarray(input_arr, low, mid);
		right = find_max_subarray(input_arr, mid + 1, high);
		cross = find_max_crossing_subarr(input_arr, low, mid, high);
		if (left.sum >= right.sum && left.sum >= cross.sum) {
			return left;
		} else if (right.sum >= left.sum && right.sum >= cross.sum) {
			return right;
		} else {
			return cross;
		}
	}
}

int main()
{
	int *input_arr = NULL;
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

	input_arr = malloc(elements * sizeof(int));
	input_f = fopen(testfile, "r");

	if (!input_f) {
		perror("error opening file");
		return(EXIT_FAILURE);
	}

	for (int i = 0; i < elements; i++) {
		fscanf(input_f, "%d", &input_arr[i]);
	}

	printf("input array\n");
	array_print(input_arr, elements);

	struct subarr result = find_max_subarray(input_arr, 0, elements - 1);

	printf("max_left %d max_right %d summ %d\n", result.max_left, result.max_right, result.sum);

	return EXIT_SUCCESS;
}
