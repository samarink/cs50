from cs50 import get_int
import math


def main():
    while True:
        card_number = get_int('Card number: ')
        if card_number > 0:
            break

    print(card_type(card_number))


def card_type(number):
    temp = number
    checksum = 0
    length = int(math.log10(number)) + 1

    for i in range(length):
        digit = temp % 10
        temp //= 10

        if i % 2 == 1:
            digit *= 2
            checksum += digit if digit < 10 else ((digit % 10) + 1)
        else:
            checksum += digit

    if (checksum % 10) > 0:
        type = 'INVALID'
    else:
        pref = number // 10 ** (length - 2)

        if length == 15 and (pref == 34 or pref == 37):
            type = 'AMEX'
        elif length == 16 and (pref > 50 and pref < 56):
            type = 'MASTERCARD'
        else:
            pref //= 10
            if (length == 13 or length == 16) and pref == 4:
                type = 'VISA'
            else:
                type = 'INVALID'

        return type


if __name__ == "__main__":
    main()
