#include <stdlib.h>
#include <cstdio>

int main()
{

	FILE* fp = fopen("fwd", "a");
	fprintf(fp, "hello,");
	fseek(fp, -1, SEEK_CUR);
	fprintf(fp, "?");
	fclose(fp);
	return 0;
}
