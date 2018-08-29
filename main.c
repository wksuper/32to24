#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

static long FileSize(FILE *fp)
{
	long m, n;

	m = ftell(fp);
	fseek(fp, 0L, SEEK_END);
	n = ftell(fp);
	fseek(fp, m, SEEK_SET);
	return n;
}

int main(int argc, char const *argv[])
{
	FILE *fin, *fout;
	size_t s;
	long fileSize, sampleCount, bufDataSize;
	uint8_t *buf;

	if (argc != 3) {
		printf("Usage: %s <input-file32-name> <output-file24-name>\n"
		       "Note: little-endian only\n", argv[0]);
		return 0;
	}

	fin = fopen(argv[1], "rb");
	if (!fin) {
		printf("error: can't open input file %s\n", argv[1]);
		goto error1;
	}

	fout = fopen(argv[2], "wb");
	if (!fout) {
		printf("error: can't open output file %s\n", argv[2]);
		goto error2;
	}

	fileSize = FileSize(fin);
	if (fileSize < 4) {
		printf("error: too small input file: %ld bytes\n", fileSize);
		goto error3;
	}

	buf = (uint8_t *)malloc(fileSize);
	if (!buf) {
		printf("error: can't malloc %ld bytes buffer\n", fileSize);
		goto error3;
	}

	bufDataSize = fread(buf, 1, fileSize, fin);
	sampleCount = bufDataSize / 4;
	for (s = 0; s < sampleCount; ++s)
		fwrite(buf + s * 4 + 1, 1, 3, fout);
	free(buf);
error3:
	fclose(fout);
error2:
	fclose(fin);
error1:
	return 0;
}

