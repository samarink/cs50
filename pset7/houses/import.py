from sys import argv
from cs50 import SQL
from csv import DictReader


def main():
    if (len(argv) != 2):
        print('Usage: python3 import.py database.csv')

    sqlize(argv[1])


def sqlize(file):
    db = SQL('sqlite:///students.db')

    with open(file, 'r') as students_file:
        students = DictReader(students_file)

        for student in students:
            fullname = student['name'].split(' ')
            house = student['house']
            birth = int(student['birth'])

            fname = fullname[0]
            if (len(fullname) == 2):
                mname = None
                lname = fullname[1]
            else:
                mname = fullname[1]
                lname = fullname[2]

            db.execute(' INSERT INTO students(first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)',
                       fname, mname, lname, house, birth)


if __name__ == '__main__':
    main()
