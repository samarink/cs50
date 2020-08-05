// https://cs50.harvard.edu/x/2020/psets/2/substitution/
#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

bool validate();

int main(int argc, string argv[]) {
  if (argc != 2) {
    printf("Usage ./substitution key\n");
    return 1;
  }

  string key = argv[1];
  if (!validate(key)) {
    printf("Key must contain 26 non-repeating characters.\n");
    return 1;
  }

  string plaintext = get_string("plaintext: ");
  int len = strlen(plaintext);
  char ciphertext[len];

  for (int i = 0; i < len; i++) {
    int charcode = plaintext[i];

    if (charcode >= 65 && charcode <= 90) {
      // handle upcase
      ciphertext[i] = toupper(key[charcode - 65]);
    } else if (charcode >= 97 && charcode <= 122) {
      // handle lowcase
      ciphertext[i] = tolower(key[charcode - 97]);
    } else {
      // handle non alpha
      ciphertext[i] = plaintext[i];
    }
  }

  // terminate the string correctly
  ciphertext[len] = '\0';

  printf("ciphertext: %s\n", ciphertext);
  return 0;
}

bool validate(string key) {
  if (strlen(key) != 26)
    return false;

  int matches = 0;
  for (char c = 'a'; c <= 'z'; c++) {
    for (int i = 0; i < 26; i++) {
      if (tolower(key[i]) == c) {
        matches++;
        break;
      }
    }
  }

  return matches == 26;
}
