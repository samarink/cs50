from cs50 import get_string


def main():
    text = get_string('Text: ')
    index = round((calc_index(text)))

    if index < 1:
        print('Before Grade 1')
    elif index >= 16:
        print('Grade 16+')
    else:
        print(f'Grade {index}')


def calc_index(text):
    letcount = 0
    sentcount = 0
    wordcount = 1

    for char in text:
        if char.isalpha():
            letcount += 1
        elif char.isspace():
            wordcount += 1
        elif char == '.' or char == '!' or char == '?':
            sentcount += 1

    l = letcount / wordcount * 100
    s = sentcount / wordcount * 100
    return 0.0588 * l - 0.296 * s - 15.8


if __name__ == "__main__":
    main()
