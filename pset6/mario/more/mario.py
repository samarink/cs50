from cs50 import get_int


def main():
    while True:
        height = get_int('Height: ')
        if height > 0 and height < 9:
            break
        else:
            print('Height must be positive')

    ladder(height)


def ladder(rows):
    for row in range(1, rows + 1):
        # spaces
        print(' ' * (rows - row), end='')

        # left ladder
        print('#' * row, end='')

        # delim
        print('  ', end='')

        # right ladder
        print('#' * row)


if __name__ == "__main__":
    main()
