import re

from classes import is_id, is_num, is_open_bracket, is_close_bracket
#formula = ""

#вход: текст
#выход: набор формул

#вход: формула
#выход: удобное представление


spec_ch = [('\a', '\\a'), ('\b', '\\b') , ('\t', '\\t'),
           ('\v', '\\v'), ('\f', '\\f'), ('\r', '\\r')]


def index_close_bracket(my_list):
    open_count = 0
    i = 0
    while i < len(my_list):
        if is_open_bracket(my_list[i]):
            open_count += 1
        elif is_close_bracket(my_list[i]):
            open_count -= 1
            if open_count == 0:
                return i
        i += 1
    return -1


def replace_spec_ch(formula):
    for i in range(0, len(spec_ch)):
        formula = formula.replace(spec_ch[i][0], spec_ch[i][1])
    return formula


'''def unite_slash(f):
    res = []
    i = 0
    while i < len(f):
        if f[i] == '\\':
            elem = "\\"
            i += 1
            if i < len(f):
                elem += f[i]
                i += 1
            res.append(elem)
        else:
            res.append(f[i])
            i += 1
    return res


def add_bracket(f):
    res = []
    i = 0
    while i < len(f):
        if f[i] in ['_', '^'] and f[i + 1] != '{':
            res.append(f[i])
            i += 1
            res.append('{')
            res.append(f[i])
            res.append('}')
            i += 1
        else:
            res.append(f[i])
            i += 1
    return res


def div_elems(f):
    res = []
    i = 0
    while i < len(f):
        if f[i].isalpha() and f[i][0] == 'd' and len(f[i]) == 2:
            res.append(f[i][0])
            res.append(f[i][1])
            i += 1
        elif f[i].isalnum() and not f[i].isdigit():
            cur_elems = re.split(r'(\d+)', f[i])
            i += 1
            for j in range(0, len(cur_elems)):
                if cur_elems[j] != '' and cur_elems[j] != ' ':
                    if cur_elems[j].isdigit():
                        res.append(cur_elems[j])
                    else:
                        for k in range(0, len(cur_elems[j])):
                            res.append(cur_elems[j][k])
        else:
            res.append(f[i])
            i += 1
    return res


def unite_brackets(f):
     res = []
     i = 0
     while i < len(f):
        if (f[i] == "\\right" or f[i] == "\left") and i < len(f) - 1:
            res.append(f[i]+ f[i+1])
            i += 2
        else:
            res.append(f[i])
            i += 1
     return res'''

def unite_elems(f):
    res = []
    i = 0
    while i < len(f):
        if f[i] == '\\':
            elem = "\\"
            i += 1
            if i < len(f):
                elem += f[i]
                i += 1
            res.append(elem)
        elif (f[i] == "\\right" or f[i] == "\left") and i < len(f) - 1:
            res.append(f[i] + f[i + 1])
            i += 2
        else:
            res.append(f[i])
            i += 1
    return res


def sep_d(f):
    res = []
    i = 0
    while i < len(f):
        if f[i].isalpha() and f[i][0] == 'd' and len(f[i]) == 2:
            res.append(f[i][0])
            res.append(f[i][1])
            i += 1
        else:
            res.append(f[i])
            i += 1
    return res


def add_bracket(f):
    res = []
    i = 0
    while i < len(f):
        if f[i] in ['_', '^'] and f[i + 1] != '{':
            res.append(f[i])
            i += 1
            res.append('{')
            res.append(f[i])
            res.append('}')
            i += 1
        else:
            res.append(f[i])
            i += 1
    return res


def prep_frac(f):
    res = []
    i = 0
    while i < len(f):
        if f[i] == "\\frac" and f[i + 1] != '{':
            res.append(f[i])
            i += 1

            if len(f[i]) >= 2:
                res.append('{')
                res.append(f[i][0])
                res.append('}')
                res.append('{')
                res.append(f[i][1])
                res.append('}')
                if len(f[i]) > 2:
                    res.append(f[i][2:])
            else:
                res.append('{')
                res.append(f[i])
                res.append('}')
                i += 1
                res.append('{')
                res.append(f[i])
                res.append('}')
            i += 1
        else:
            res.append(f[i])
            i += 1
    return res


def clean_formula(f):
    res = []
    i = 0
    while i < len(f):
        if f[i] == "\\textstyle":
            i += 1
        elif f[i] == "\\mathrm" or f[i] == "\\mathcal":
            i += 1
            if f[i] != '{':
                res.append(f[i])
                i += 1
            else:
                st = i
                fn = index_close_bracket(f[st:])+st
                i += 1
                while i < fn:
                    res.append(f[i])
                    i += 1
                i += 1
        elif f[i] in [',', '.', '\\!', '\\.', '\\,']:
            i += 1
        elif f[i] in ["\\left\\{", "\\left\\(", "\\left\\["]:
            res.append(f[i][-1])
            i += 1
        elif f[i] in ["\\right\\{", "\\right\\(", "\\right\\["]:
            res.append(f[i][-1])
            i += 1
        elif f[i] == "\\times":
            res.append('*')
            i += 1
        else:
            res.append(f[i])
            i += 1
    return res


'''def prepare_formula(formula):
    formula = replace_spec_ch(formula)

    sep_f = re.split(r'([^a-zA-Z0-9])', formula)
    sep_f = [i for i in sep_f if i]
    sep_f = [i for i in sep_f if i.strip()]

    sep_f = unite_slash(sep_f)
    sep_f = add_bracket(sep_f)
    #sep_f = div_elems(sep_f)
    sep_f = unite_brackets(sep_f)
    sep_f = prep_frac(sep_f)
    res = clean_formula(sep_f)
    return res'''

def prepare_formula(formula):
    formula = replace_spec_ch(formula)

    sep_f = re.split(r'([^a-zA-Z0-9]|\d+)', formula)
    sep_f = [i for i in sep_f if i]
    sep_f = [i for i in sep_f if i.strip()]

    sep_f = unite_elems(sep_f)
    sep_f = sep_d(sep_f)
    sep_f = add_bracket(sep_f)
    sep_f = prep_frac(sep_f)
    res = clean_formula(sep_f)
    return res


def get_formula_M1(formula):
    return prepare_formula(formula)
#print(get_formula_M1("x^2+2x+2 = 0"))
#print(get_formula_M1("G(s)={\mathcal{N}}\{g(\theta)\}=\int_{\mathrm{a}}^{\infty}\theta^{s}g(\theta)\,{\frac{d\theta}{\theta}}"))

"""
\textstyle{\sqrt{\frac{a}{b}}}
\frac{(x+2)(x^{2}-1)}{(x+1)(x^{2}-4)}
{\frac{x-2}{x^{2}+6x+9}}-{\frac{x+2}{2x^{2}-18}}.
[2x^{2}\!\!-\!\!1]\!\!(2x^{2}\!\!+\!\!1)(4x^{4}\!\!+\!\!2x^{2}\!\!+\!\!1)(4x^{4}\!\!-\!\!2x^{2}\!\!+\!\!1)(4x^{4}\!\!-\!\!2x^{2}\!\!+\!1),
\textstyle\sum_{i=1}^{10}t_{i}
\int_{0}^{\infty}e^{-x}\,\mathrm{d}x
\textstyle\log\alpha
\ln x=\int_{1}^{t}{\frac{1}{x}}d x
{\bar{\lambda}}{\overline{{\cal S}}}
G(s)={\mathcal{N}}\{g(\theta)\}=\int_{\mathrm{a}}^{\infty}\theta^{s}g(\theta)\,{\frac{d\theta}{\theta}}
S=\int_{x}\left\{{\frac{1}{2}}\sum_{a}\partial^{\mu}\chi_{a}\partial_{\mu}\chi_{a}+V(\rho)\right\}
\exp\left[\int d^{4}x g\phi\bar{\psi}\psi\right]=\sum_{n=0}^{\infty}\frac{g^{n}}{n!}\left(\int d^{4}x\phi\bar{\psi}\psi\right)^{n}.

Process finished with exit code 0

"""
'''def fill_gaps(f):
    res = []
    for i in range(0, len(f)-1):
        if (f[i].isalpha() or f[i].isdigit() or is_id(f[i])) and (f[i+1].isalpha() or f[i+1].isdigit() or is_id(f[i+1])):
            res.append(f[i])
            res.append('*')
        elif f[i].isalpha() and f[i+1] == '(':
            res.append(f[i])
            res.append('.')
        #elif (f[i] == ')' or f[i] == '}') and (f[i+1].isalpha() or f[i+1].isdigit() or is_id(f[i+1])):
        #    res.append(f[i])
        #    res.append('*')
        elif (f[i].isdigit() or is_id(f[i])) and f[i + 1] == '(':
            res.append(f[i])
            res.append('*')
        elif (f[i] == ')') and (f[i+1] == '('):
            res.append(f[i])
            res.append('*')
        else:
            res.append(f[i])
    res.append(f[-1])
    return res'''


def fill_gaps(f):
    res = []
    for i in range(0, len(f)-1):
        if (f[i] == ')') and \
                (f[i+1] == '('):
            res.append(f[i])
            res.append('*')
        elif f[i] == 'd' and is_id(f[i+1]):
            res.append(f[i])
        elif (is_id(f[i]) or is_num(f[i])) and \
            (is_id(f[i+1]) or is_num(f[i+1])):
            res.append(f[i])
            res.append('*')
        elif is_id(f[i]) and f[i+1] =='(':
            res.append(f[i])
            res.append('.')
        else:
            res.append(f[i])
    res.append(f[-1])
    return res


def replace_frac(f):
    i = 0
    while i < len(f):
        if f[i] == "\\frac":
            f.pop(i)
            fin = index_close_bracket(f[i:]) + i
            f.insert(fin+1, '/')
        else:
            i += 1
    return f

#между int и dx ставим скобки
def prepare_int(f):
    i = 0
    while i < len(f):
        if f[i] == "\int":
            if f[i+1] == '_':
                st_down = f[i:].index('{') + i
                fin_down = index_close_bracket(f[st_down:]) + st_down

                st_up = f[fin_down:].index('{') + fin_down
                fin_up = index_close_bracket(f[st_up:]) + st_up

                i = fin_up
            f.insert(i+1, '(')
            int_index = f.index('d')
            if int_index != -1:
                int_index += 1
                while ((int_index + 1) < len(f) and f[int_index + 1] == "d"):
                    int_index += 2
                f.insert(int_index-1, ')')
        else:
            i += 1
    return f

def get_formula_M2(formula):
    return prepare_int(replace_frac(fill_gaps(prepare_formula(formula))))
    #return prepare_int(replace_frac((prepare_formula(formula))))

#f = "\int_a^b i^2*x^2 dx"
#f = "\int_a^b \int_a^b \int_a^b i^2*x^2 dx dy dz"
#print(get_formula_M2(f))
#print(get_formula_M2("\\frac 1 2 *43 + \\frac 123 + \\frac{\\frac {2^2} {3_1} }{34}"))

