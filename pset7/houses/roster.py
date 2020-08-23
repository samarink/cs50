from sys import argv
from cs50 import SQL


def main():
    if len(argv) != 2:
        print('Usage: python3 roster.py housename')

    print_students(argv[1])


def print_students(house):
    db = SQL('sqlite:///students.db')
    students = db.execute(
        'SELECT * FROM students WHERE house = ? ORDER BY last', house)

    for student in students:
        fname = student['first']
        lname = student['last']
        birth = student['birth']

        if student['middle'] != None:
            mname = ' ' + student['middle'] + ' '
        else:
            mname = ' '

        print(f'{fname}{mname}{lname}, born {birth}')


if __name__ == '__main__':
    main()
