#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: ./recover image");
    return 1;
  }

  FILE *inptr = fopen(argv[1], "r");
  if (inptr == NULL) {
    fprintf(stderr, "Could not open the file %s\n", argv[1]);
    return 2;
  }

  FILE *outptr;
  unsigned char buffer[512];
  char fname[8];
  int count = 0;
  bool jpg = false;

  while (fread(buffer, sizeof(buffer), 1, inptr)) {
    // check whether jpeg
    if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
        (buffer[3] & 0xf0) == 0xe0) {

      if (jpg) {
        fclose(outptr);
        jpg = false;
      }

      sprintf(fname, "%03i.jpg", count);
      outptr = fopen(fname, "w");
      fwrite(buffer, sizeof(buffer), 1, outptr);

      jpg = true;
      count++;
    } else {
      if (jpg) {
        fwrite(buffer, sizeof(buffer), 1, outptr);
      }
    }
  }

  fclose(outptr);
  fclose(inptr);
  return 0;
}
