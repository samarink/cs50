#include <cs50.h>
#include <stdio.h>

int main(void)
{

  int height;
  do {
    height = get_int("n: ");
  } while (height < 1 || height > 8);

  for (int i = 1; i <= height; i++) {

    // spaces
    for (int j = 1; j <= height - i; j++) {
      printf(" ");
    }

    // left ladder
    for (int k = 1; k <= i; k++) {
      printf("#");
    }

    // ladder delimeter
    printf("  ");

    // right ladder
    for (int k = 1; k <= i; k++) {
      printf("#");
    }

    // level delimeter
    printf("\n");
  }

  return 0;
}
