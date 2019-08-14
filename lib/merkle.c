
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "crypto.h"
#include "merkle.h"
#include "stack.h"

/* dumbly casts to an int, so merkle trees must be an exponent of 2 */
int log_2(int x) { return (int)log(x) / log(2); }

// size is how many hashes
char *get_root(char **hashes, int size)
{
	double x = log(8);
	char **h = hashes;

	int tree_height = log_2(size) + 1;
	char **temp_leaves = malloc(1024);
	char *child_leaf_1;
	char *child_leaf_2;
	char *temp_parent_leaf = malloc(65);
	memset(temp_parent_leaf, '\0', 65);
	char *hashed_parent_leaf;
	int index = 0;
	int parent_index = 0;

	//printf("merkle start\n");
	for (int i = tree_height; i >= 1; i--) {
		//printf("merkle first for\n");
		index = 0;
		parent_index = 0;

		for (int y = 0; y < pow(2, i - 1); y++) {
			//printf("merkle second for\n");
			child_leaf_1 = h[index++];
			child_leaf_2 = h[index++];
			//printf("%s\n",child_leaf_1);
			//printf("%s\n",child_leaf_2);

			strncpy(temp_parent_leaf, child_leaf_1, 32);

			temp_parent_leaf[32] = '\0';
			strcat(temp_parent_leaf, child_leaf_2);
			temp_parent_leaf[64] = '\0';
			hashed_parent_leaf = hash(temp_parent_leaf);
			temp_leaves[parent_index] = malloc(33);
			strncpy(temp_leaves[parent_index], hashed_parent_leaf, 32);
			temp_leaves[parent_index++][32] = '\0';
			// free(child_leaf_1);
			// free(child_leaf_2);
			// free(hashed_parent_leaf);
			// temp_leaves[index] = NULL;
			memset(temp_parent_leaf, '\0', 65);
		}
		h = temp_leaves;
		// printf("\n");
	}
	return h[0];
}


char *get_siblings(char **target_hash_list, char **hashes)
{
	char **t = target_hash_list;
	int c_1 = 0, c_2 = 0;

	int alignment = 0; // 0=left  1=right

	while (*t) {
		t++;
		c_1++;
	}
	c_1 = c_1 - 1;
	int o_c_1 = c_1;

	t = hashes;
	while (*t) {
		t++;
		c_2++;
	}
	c_2 = c_2 - 1;
	int o_c_2 = c_2 + 1;

	/* get the alignment
	 */
	if ((strcmp(target_hash_list[0], hashes[0])) == 0)
		alignment = 0;
	else if ((strcmp(target_hash_list[c_1], hashes[c_2])) == 0)
		alignment = 1;
	else
		printf("Error\n");

	stack _stack = create_stack();

	/* calc the next level for all of hashes
	 */
	char *parent = malloc(65); // twice the hash
	char *p_hash;

	// printf("c_2: %d\n",c_2);

	// now all next layer hashes are calculated

	// get the even number of target_hashes
	if (((c_1) % 2) == 1)
		c_1 = c_1 + 1;
	printf("new c_1 ( # target_hashes ): %d\n", c_1);

	/* left aligned
	 * Format: [#]target_hash,target_hash,sib_hash,sib_hash,sib_hash,sib_hash
	 * right aligned
	 * Format: [-#]sib_hash,sib_hash,sib_hash,sib_hash,target_hash,target_hash
	 */

	// process based of alignment
	// alignment: 0=left 1=right
	//
	char *front = malloc(100);
	char *converted_stack;
	char *rest;
	char *parent_string;
	stack _target_stack = create_stack();
	char *to_return = malloc(4096);

	if (alignment == 0) {
		// set the front
		sprintf(front, "[%d]", c_1);

		// get the targets from target_hash_list
		for (int i = c_1 - 1; i >= 0; i--)
			push(_target_stack, target_hash_list[i]);

		// get the parents of the next level = siblings
		while (c_2 > (c_2 + 1) / 2) {
			sprintf(parent, "%s%s", hashes[c_2--], hashes[c_2--]);
			// p_hash = hash(parent);
			// printf("%s\n",p_hash);
			push(_stack, parent);
		}
		parent_string = convert_to_string(_stack, 0);
		converted_stack = convert_to_string(_target_stack, 0);
		sprintf(to_return, "%s%s%s", front, converted_stack, parent_string);
	}
	else if (alignment == 1) {

		sprintf(front, "[-%d]", c_1);

		printf("[debug] o_c_1: %d\n", o_c_1);
		for (int i = o_c_1; i >= !(o_c_1 % 2); i--) {
			// printf("tt: %s\n",target_hash_list[i]);
			push(_target_stack, target_hash_list[i]);
		}

		int x = c_2 - c_1;
		while (x > 0) {
			sprintf(parent, "%s%s", hashes[x--], hashes[x--]);
			push(_stack, parent);
			// printf("parent = %s\n",parent);
		}
		parent_string = convert_to_string(_stack, 0);
		converted_stack = convert_to_string(_target_stack, 0);
		sprintf(to_return, "%s%s%s", front, parent_string, converted_stack);
	}

	return to_return;
}

/* TODO
 */
char *get_root_from_siblings(char **target_hash_list, char **siblings)
{
	// get the next level of hashes from the target_hash_list
	// then calculate as normal using the siblings
}
