import sqlite3


def create_table():
    connection = sqlite3.connect('my_database.db')
    cursor = connection.cursor()
    cursor.execute('''
    CREATE TABLE IF NOT EXISTS Formulas (
    id INTEGER PRIMARY KEY,
    latex TEXT,
    mathml_presentation TEXT,
    mathml_content TEXT,
    search_form TEXT,
    book_titles TEXT
    )
    ''')
    connection.commit()
    connection.close()


def get_book_title(title, author):
    return "[" + title + "," + author + "]"


def have_formula(formula):
    connection = sqlite3.connect('my_database.db')
    cursor = connection.cursor()
    cursor.execute('SELECT * FROM Formulas')
    data = cursor.fetchall()
    connection.close()

    for f in data:
        if f[4] == formula:
            return f[0]
    return -1


def add_title(formula_id, title):
    connection = sqlite3.connect('my_database.db')
    cursor = connection.cursor()
    cursor.execute('SELECT * FROM Formulas where id=?', (str(formula_id),))
    data = cursor.fetchall()[0]
    index = data[5].find(title)
    if index < 0:
        new_data = data[5] + title
        new_title = 'UPDATE Formulas SET book_titles=? where id=?'
        cursor.execute(new_title, (new_data, str(formula_id)))
        connection.commit()
    connection.close()


def insert_formula(latex, mathml_presentation, mathml_content, search_form, used):
    connection = sqlite3.connect('my_database.db')
    cursor = connection.cursor()
    cursor.execute('INSERT INTO Formulas (latex, mathml_presentation, mathml_content, search_form, book_titles) '
                   'VALUES (?, ?, ?, ?, ?)', (latex, mathml_presentation, mathml_content, search_form, used))
    connection.commit()
    connection.close()


def print_table():
    connection = sqlite3.connect('my_database.db')
    cursor = connection.cursor()
    cursor.execute('SELECT * FROM Formulas')
    data = cursor.fetchall()
    connection.close()

    for dt in data:
        for t in dt:
            print(t)
        print()
        #print(dt)
#print_table()

#create_table()