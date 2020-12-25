#include <stdlib.h>
#include <cstdio>

int main()
{

	FILE* fp = fopen("fwd", "a+");
	fprintf(fp, "hello,");
	fseek(fp, -1, SEEK_CUR);
	char buf[2] = { 0 };
	int read= fread(buf, 1, 1, fp);
	fprintf(fp, "?");
	fclose(fp);
	return 0;
}
