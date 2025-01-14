from ProcessFormula import get_formula_M2
from classes import is_id, is_open_bracket, is_close_bracket, is_trigonom, is_index, is_op, is_num, conv_id

class Node:
    def __init__(self, data):
        self.data = data
        self.left = self.right = None

EMPTY = "empty"

spech_op = ['\sqrt', '\sum', '\int', '\log', '\ln']

rank_2 = ['+', '-']
rank_3 = ['\\times', 'div', '*', ':', '/']


class Node:
    def __init__(self, data):
        self.data = data
        self.left = self.right = self.index_down = self.index_up = None


def get_rank(c):
    if is_open_bracket(c):
        return 0
    if c in rank_2:
        return 2
    if c in rank_3:
        return 3
    if c[0] in spech_op or is_trigonom(c) or c == '.':
        return 4
    if is_index(c):
        return 5
    # '=', 'geq', 'gt', 'leq', 'lt', 'neq'
    return 1


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


def get_int_index(formula):
    int_index = formula.index('d')
    if int_index != -1:
        int_index += 1
        while ((int_index + 3) < len(formula) and formula[int_index + 2] == "d"):
            int_index += 3

    #print("return:", int_index, formula[int_index])
    return int_index

#[name, down_index, up_index, body]
def process_int(formula):
    per = EMPTY
    int_index = get_int_index(formula)
    if int_index != -1:
        per = formula[int_index]

    if formula[1] != '_':
        return ["\int", EMPTY, EMPTY, per], 1


    st_down = formula.index('{')
    fin_down = index_close_bracket(formula[st_down:]) + st_down

    st_up = formula[fin_down:].index('{') + fin_down
    fin_up = index_close_bracket(formula[st_up:]) + st_up

    down_index = make_postfix_form(formula[st_down+1:fin_down])
    up_index = make_postfix_form(formula[st_up+1:fin_up])

    return ["\int", down_index, up_index, per], fin_up+1


def process_sqrt(formula):
    if formula[1] != '[':
        return ["\sqrt", '2', EMPTY], 1

    st_down = formula.index('[')
    fin_down = index_close_bracket(formula[st_down:]) + st_down
    down_index = make_postfix_form(formula[st_down + 1:fin_down])

    return ["\sqrt", down_index, EMPTY], fin_down+1


def process_sum(formula):
    if formula[1] != '_':
        return ["\sum", EMPTY, EMPTY, EMPTY], 1

    body = EMPTY

    st_down = formula.index('{')
    fin_down = index_close_bracket(formula[st_down:]) + st_down
    if formula[st_down+2] == '=':
        body = formula[st_down+1]
        st_down += 2


    st_up = formula[fin_down:].index('{') + fin_down
    fin_up = index_close_bracket(formula[st_up:]) + st_up

    down_index = make_postfix_form(formula[st_down + 1:fin_down])
    up_index = make_postfix_form(formula[st_up + 1:fin_up])
    return ["\sum", down_index, up_index, body], fin_up+1


def process_log(formula):
    if formula[1] != '_' and formula[1] != '^':
        return 	["\log", EMPTY, EMPTY], 1
    if formula[1] == '^':
        st_up = formula.index('{')
        fin_up = index_close_bracket(formula[st_up:]) + st_up
        up_index = make_postfix_form(formula[st_up + 1:fin_up])
        return ["\log", EMPTY, up_index], fin_up+1

    st_down = formula.index('{')
    fin_down = index_close_bracket(formula[st_down:]) + st_down
    down_index = make_postfix_form(formula[st_down + 1:fin_down])
    if formula[fin_down+1] == '^':
        st_up = formula[fin_down:].index('{') + fin_down
        fin_up = index_close_bracket(formula[st_up:]) + st_up
        up_index = make_postfix_form(formula[st_up + 1:fin_up])
        return ["\log", down_index, up_index], fin_up+1
    return ["\log", down_index, EMPTY], fin_down+1


def process_ln(formula):
    if formula[1] != '_' and formula[1] != '^':
        return ["\ln", EMPTY, EMPTY], 1
    st_up = formula.index('{')
    fin_up = index_close_bracket(formula[st_up:]) + st_up
    up_index = make_postfix_form(formula[st_up + 1:fin_up])
    return ["\ln", EMPTY, up_index], fin_up+1


def process_op(formula):
    if formula[0] == "\int":
        return process_int(formula) #["\int", down_index, up_index, ?] + place
    elif formula[0] == "\sqrt":
        return process_sqrt(formula)#["\int", down_index, _, body] + place
    elif formula[0] == "\sum":
        return process_sum(formula)#["\int", down_index, _, body] + place
    elif formula[0] == "\log":
        return process_log(formula)#["\int", down_index, _, body] + place
    elif formula[0] == "\ln":
        return process_ln(formula)#["\int", down_index, _, body] + place
    return "", 1


def make_postfix_form(formula):
    answer = []
    stack = []
    i = 0

    while i < len(formula):
        if is_num(formula[i]) or is_id(formula[i]):
            answer.append(formula[i])
            i += 1
        elif formula[i] in spech_op:
            long_op, add = process_op(formula[i:])
            #удаляем элемент по которому берется d
            #print(formula)
            if formula[i] == "\int":
                int_index = get_int_index(formula[i:])+i
                if int_index != -1:
                    #print(formula[int_index])
                    formula.pop(int_index-1)
                    formula.pop(int_index - 1)
                    #print(formula)
            if len(stack) == 0 or get_rank(long_op) > get_rank(stack[-1]):
                stack.append(long_op)
            else:
                while len(stack) != 0 and get_rank(long_op) < get_rank(stack[-1]):
                    answer.append(stack.pop())
                stack.append(long_op)
            i += add
        else:
            if is_close_bracket(formula[i]):
                while not is_open_bracket(stack[-1]):
                    answer.append(stack.pop())
                stack.pop()
            elif is_open_bracket(formula[i]):
                stack.append(formula[i])
            elif len(stack) == 0 or get_rank(formula[i]) > get_rank(stack[-1]):
                stack.append(formula[i])
            else:
                if formula[i] == '.':
                    while len(stack) != 0 and get_rank(formula[i]) < get_rank(stack[-1]):
                        answer.append(stack.pop())
                else:
                    while len(stack) != 0 and get_rank(formula[i]) <= get_rank(stack[-1]):
                        answer.append(stack.pop())
                stack.append(formula[i])
            i += 1
    while len(stack) != 0:
        answer.append(stack.pop())

    return answer

#f = "x^2*\sin x^2"
#f = "\int_a^b i^2*x^2 dx"
#f = "\log (x^2) + \sqrt[10]{12+2} + 5"
#f = "1+\log_3^2 x + 1"
#f = "\int_a^b ((x^2) dx) + 1"
#f = "x^2 + 2x -1 = 0"
#f = "1 + \sum \log x = 3"
#f = "\int \log_3 x = 1"
#f = "1 + \int x dx = 3"
#f = "\log_3 \log x  = 3"
#print(get_formula_M2(f))
#print(get_formula_M2(f))
#f = "\sum_{i=1}^100 a_i"
#f = "\sin f(x)*(35+1)"
#f = "\sin f(x)*35+1"
#f = "a+b*c=0"
#f = "y +2+ f(x) = 0"
#print(get_formula_M2(f))
#print(make_postfix_form(get_formula_M2(f)))


def has_one_param(c):
    return c[0] in spech_op or is_trigonom(c)



def add_elem_in_tree(root, elem):
    if root is None:
        return Node(elem)
    elif root.data[0] not in spech_op:
        if root.data.isalpha() or root.data.isdigit() or is_id(root.data):
            return None
    tmp = add_elem_in_tree(root.right, elem)
    if tmp is not None:
        root.right = tmp
        return root
    elif not has_one_param(root.data):
        tmp = add_elem_in_tree(root.left, elem)
        if tmp is None:
            return None
        root.left = tmp
        return root
    else:
        return None


def build_tree(formula):
    root = None
    i = len(formula) - 1
    while i >= 0:
        root = add_elem_in_tree(root, formula[i])
        i -= 1
    return root


def print_tree(root):
    if root is None:
        return
    print_tree(root.left)
    print_tree(root.right)
    print(root.data)

#print_tree((build_tree(make_postfix_form(get_formula_M2(f)))))


def process_member(c):
    if is_num(c):
        return '<cn>' + c + '</cn>'
    else:
        return '<ci>' + conv_id(c)+ '</ci>'


#spech_op = ['\sqrt', '\sum', '\int', '\log', '\ln']


def procc_op(c):
    if c == '=':
        return '<eq/>'
    elif c == '+':
        return '<plus/>'
    elif c == '-':
        return '<minus/>'
    elif c == '*':
        return '<times/>'
    elif c == '/':
        return '<divide/>'
    elif c == '^':
        return '<power/>'
    elif c == '_':
        return '<power/>'
    elif is_trigonom(c):
        return '<' + c + '/>'
    elif c == ".":
        return""
    elif c[0] == '\sqrt':
        return '<root/>' +\
            '<degree><cn>' + get_mathML_for_index(c[1]) + '</cn></degree>'
    elif c[0] == '\int':
        s1 = s2 = s3 = ""
        if c[1] != EMPTY:
            s1 = '<lowlimit>' + get_mathML_for_index(c[1]) + '</lowlimit>'
        if c[2] != EMPTY:
            s2 = '<uplimit>' + get_mathML_for_index(c[2]) + '</uplimit>'
        if c[3] != EMPTY:
            s3 = "<bvar><ci>"+c[3]+"</ci></bvar>"
        return '<int/>'+s1+s2+s3
    elif c[0] == '\sum':
        s1 = s2 = s3 = ""
        if c[1] != EMPTY:
            s1 = '<lowlimit>' + get_mathML_for_index(c[1]) + '</lowlimit>'
        if c[2] != EMPTY:
            s2 = '<uplimit>' + get_mathML_for_index(c[2]) + '</uplimit>'
        if c[3] != EMPTY:
            s3 = "<bvar><ci>"+c[3]+"</ci></bvar>"
        return '<sum/>'+s1+s2+s3
    elif c[0] == '\log':#может быть up limit и low limit
        s1 = s2 = ""
        if c[1] != EMPTY:
            s1 = '<lowlimit>' + get_mathML_for_index(c[1]) + '</lowlimit>'
        if c[2] != EMPTY:
            s2 = '<uplimit>' + get_mathML_for_index(c[2]) + '</uplimit>'
        return '<log/>'+s1+s2
    elif c[0] == '\ln':
        s1 = s2 = ""
        if c[1] != EMPTY:
            s1 = '<lowlimit>' + get_mathML_for_index(c[1]) + '</lowlimit>'
        if c[2] != EMPTY:
            s2 = '<uplimit>' + get_mathML_for_index(c[2]) + '</uplimit>'
        return '<ln/>' + s1 + s2




def goThroughTheTree(root):
    if root.left is not None or root.right is not None:
        root.data = procc_op(root.data)
        res ='<apply>' + root.data
    else:
        root.data = process_member(root.data)
        res = root.data
    if root.left is not None:
        res += goThroughTheTree(root.left)
    if root.right is not None:
        res += goThroughTheTree(root.right)
        res += '</apply>'
    return res

def get_mathML_for_index(c):
    return goThroughTheTree(build_tree(make_postfix_form(c)))

def get_MathML_Content(formula):
    sep = get_formula_M2(formula)
    pstf_form = make_postfix_form(sep)
    sem_tree = build_tree(pstf_form)
    res = goThroughTheTree(sem_tree)
    return '<math xmlns="http://www.w3.org/1998/Math/MathML">' + \
        res + '</math>'

f = "a+b*c=0"
#f = "y +2+ f(x) = 0"
#f = "x^2^(y+1)+x=0"
#f = "\int_a^b i^2*x^2 dx"
#f = "\log_3 \int_a^b \int_a^b \int_a^b i^2*x^2 dx dy dz"
#print(make_postfix_form(get_formula_M2(f)))
#print(get_MathML_Content(f))
f = '\sum_{j=2}^{n}(j-1)={\frac{n(n-1)}{2}}'

#print(make_postfix_form(get_formula_M2(f)))
#print(get_MathML_Content(f))

"""
\textstyle{\sqrt{\frac{a}{b}}}
\frac{(x+2)(x^{2}-1)}{(x+1)(x^{2}-4)}
{\frac{x-2}{x^{2}+6x+9}}-{\frac{x+2}{2x^{2}-18}}.
\textstyle\sum_{i=1}^{10}t_{i}
\int_{0}^{\infty}e^{-x}\,\mathrm{d}x
\ln x=\int_{1}^{t}{\frac{1}{x}}d x
\textstyle\int_{a}^{b}f(x)d x=F(b)-\ F(a)
\sum_{j=2}^{n}j={\frac{n(n+1)}{2}}-1
\sum_{j=2}^{n}(j-1)={\frac{n(n-1)}{2}}
"""
