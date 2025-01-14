import os
from PIL import Image
from pix2tex.cli import LatexOCR
from MathML1 import get_MathML_Presentation
from MathML2 import get_MathML_Content
from SearchForm import get_search_form
from table_utils import have_formula, insert_formula,\
    print_table, add_title, get_book_title, create_table


def read_file(path):
    res = 0
    try:
        f = open(path, 'r', encoding='utf-8')
        res = f.read()
        f.close()
    except FileNotFoundError:
            print("file", path, "is not found")
    return res


def get_imgs(path):
    if os.path.isdir(path):
        res = os.listdir(path)
        for name in res:
            if not name.endswith('.jpg'):
                print("wrong format documents in the directory", path)
                return 0
        return res
    print("directory", path, "is not found")
    return 0


def my_test():
    model = LatexOCR()
    path_to_imgs = "example_doc//Formulas"

    imgs = get_imgs(path_to_imgs)
    print(imgs)

    for img_ in imgs:
        img = Image.open(path_to_imgs + "\\" + img_)
        latex = model(img)
        print(latex)




def start():
    #path_to_title = input("input path to title: ")
    #path_to_author = input("input path to author: ")
    #path_to_imgs = input("input path to formulas: ")
    path_to_title = "example_doc//title.txt"
    path_to_author = "example_doc//author.txt"
    path_to_imgs = "example_doc//Formulas"

    title = read_file(path_to_title)
    author = read_file(path_to_author)
    imgs = get_imgs(path_to_imgs)


    if title == 0 or author == 0 or imgs == 0:
        print("wrong input data")
        return
    print("create table")
    create_table()
    print("done")

    print("loading model")
    model = LatexOCR()
    print("finish loading")
    #print_table()
    for img_ in imgs:
        latex = ""

        img = Image.open(path_to_imgs + "\\" + img_)
        try:
            latex = model(img)
        except:
            print("fail to recognize:", img_)

        if latex != "":
            try:
                mathml_presentation = get_MathML_Presentation(latex)
                mathml_content = get_MathML_Content(latex)
                search_form = get_search_form(latex)

                formula_id = have_formula(search_form)
                book_title = get_book_title(title, author)
                if formula_id < 0:
                    insert_formula(latex, mathml_presentation, mathml_content, search_form, book_title)
                else:
                    add_title(formula_id, book_title)
            except:
                print("fail to process:", img_)
    print_table()


start()
