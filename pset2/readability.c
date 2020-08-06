// https://cs50.harvard.edu/x/2020/psets/2/redability/
#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int main(void) {

  string text = get_string("Text: ");
  int len = strlen(text);
  if (!len)
    return 1;

  int letcount = 0;
  int sentcount = 0;
  int wordcount = 1;

  for (int i = 0; i < len; i++) {
    char currchar = text[i];

    if (isalpha(currchar)) {
      letcount += 1;
    } else if (isspace(currchar)) {
      wordcount += 1;
    } else if (currchar == '!' || currchar == '.' || currchar == '?') {
      sentcount += 1;
    }
  }

  // https://en.wikipedia.org/wiki/Coleman%E2%80%93Liau_index
  float l = (float) letcount / wordcount * 100;
  float s = (float) sentcount / wordcount * 100;
  int index = round(0.0588 * l - 0.296 * s - 15.8);

  if (index < 1) {
    printf("Before Grade 1\n");
  } else if (index >= 16) {
    printf("Grade 16+\n");
  } else {
    printf("Grade %i\n", index);
  }

  return 0;
}
