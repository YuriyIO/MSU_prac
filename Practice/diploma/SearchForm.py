from ProcessFormula import get_formula_M1
from classes import is_id



def get_search_form(formula):
    f = get_formula_M1(formula)

    res = ""
    for i in range(0, len(f)):
        if f[i].isdigit():
            res += "num"
        elif f[i].isalpha() or is_id(f[i]):
            res += "id"
        else:
            res += f[i]
    return res
