#include <cs50.h>
#include <math.h>
#include <stdio.h>

int main(void)
{
  long card_number;
  do {
    card_number = get_long("Number: ");
  } while (card_number < 0);

  int length = floor(log10(card_number)) + 1;

  long temp = card_number;
  int checksum = 0;
  for (int i = 0; i < length; i++) {
    int digit = temp % 10;
    temp /= 10;

    // every other number starting from second to last
    if (i % 2 == 1) {
      digit *= 2;
      checksum += digit >= 10 ? (digit % 10) + 1 : digit;
    } else {
      // all other numbers
      checksum += digit;
    }
  }

  string type = "";
  // checksum should end with 0
  if (checksum % 10 > 0) {
    type = "INVALID";
  } else {
    // determine type based on first two numbers
    int prefix = card_number / pow(10, length - 2);

    if (length == 15 && (prefix == 34 || prefix == 37)) {
      type = "AMEX";
    } else if (length == 16 && (prefix > 50 && prefix < 56)) {
      type = "MASTERCARD";
    } else {
      prefix /= 10;
      if ((length == 13 || length == 16) && prefix == 4) {
        type = "VISA";
      } else {
        type = "INVALID";
      }
    }
  }

  printf("%s\n", type);

  return 0;
}
