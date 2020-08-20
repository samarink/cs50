from sys import argv
from csv import reader, DictReader


def main():
    if len(argv) != 3:
        print('Usage: python3 dna.py database.csv sequence.txt')

    db = argv[1]
    seq = argv[2]
    result = find_match(db, seq)
    print(result)


def find_match(db, seq):
    with open(db, newline='') as csvfile:
        rdr = reader(csvfile)
        all_seqs = next(rdr)[1:]

        with open(seq) as seqfile:
            s = seqfile.read()
            matches = [max_repeat(s, sq) for sq in all_seqs]

        for line in rdr:
            name = line[0]
            vals = [int(val) for val in line[1:]]
            if vals == matches:
                return name
        return 'No match'


def max_repeat(seq, STR):
    # stores max num of repeats given substr[i:]
    table = [0] * len(seq)

    for i in range(len(seq) - len(STR), -1, -1):
        if seq[i: i + len(STR)] == STR:
            if i + len(STR) > len(seq) - 1:
                table[i] = 1
            else:
                table[i] = 1 + table[i + len(STR)]

    return max(table)


if __name__ == '__main__':
    main()
