#include "Utility.h"

FILE *open_file(const char *file_name, const char *mode) {
	FILE *f = fopen(file_name, mode);
	if(f == NULL) {
		printf("Can not open file: %s\n", file_name);
		exit(1);
	}

	return f;
}
