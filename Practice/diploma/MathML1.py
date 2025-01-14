from ProcessFormula import get_formula_M1
from classes import is_open_bracket, is_close_bracket, is_id, is_num, conv_id


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


def has_index(formula, ind = '*'):
    place = 0
    if is_open_bracket(formula[0]):
        place = index_close_bracket(formula[place:])
        place += 1
    else:
        place += 1
    if place < len(formula):
        if ind != '*':
            return formula[place] == ind
        return formula[place] in ['_', '^']
    return False


def index_place(formula):
    place = 0
    if is_open_bracket(formula[0]):
        place = index_close_bracket(formula[place:])
        place += 1
    else:
        place += 1
    return place


def next_action(formula):
    index = 0
    while index != -1 and index < len(formula):
        if is_open_bracket(formula[index]):
            index += index_close_bracket(formula[index:])
            index += 1
        elif formula[index] in ['_', '^']:
            index += 1
        elif is_num(formula[index]) or is_id(formula[index]):
            index += 1
        else:
            return index
    return -1


def process_elem(elem):
    if is_num(elem):
        return '<mn>' + elem + '</mn>'
    elif is_id(elem):
        return '<mi>' + conv_id(elem) +'</mi>'
    else:
        return '<mo>' + elem + '</mo>'


def process_frac(formula):
    st_down = formula.index('{')
    fin_down = index_close_bracket(formula[st_down:]) + st_down

    st_up = formula[fin_down:].index('{') + fin_down
    fin_up = index_close_bracket(formula[st_up:]) + st_up

    return "<mfrac>" + \
            "<mrow>" + process_expr(formula[st_down + 1:fin_down]) + "</mrow>" +\
            "<mrow>" + process_expr(formula[st_up + 1:fin_up]) + "</mrow>"+ "</mfrac>" +\
            process_expr(formula[fin_up + 1:])


def process_int(formula):
    if formula[1] != '_':
        return "<mo>&#x222b</mo>" + process_expr(formula[1:])

    st_down = formula.index('{')
    fin_down = index_close_bracket(formula[st_down:]) + st_down

    st_up = formula[fin_down:].index('{') + fin_down
    fin_up = index_close_bracket(formula[st_up:]) + st_up

    return "<msubsup><mrow><mo>&#x222b</mo></mrow>" + \
            "<mrow>" + process_expr(formula[st_down+1:fin_down]) + "</mrow>" +\
            "<mrow>" + process_expr(formula[st_up+1:fin_up]) + "</mrow>" + "</msubsup>" +\
            process_expr(formula[fin_up+1:])


def process_sqrt(formula):
    if formula[1] != '[':
        st= formula.index('{')
        fin= index_close_bracket(formula[st:]) + st
        return "<msqrt>" + process_expr(formula[st+1:fin]) + "</msqrt>" +\
                process_expr(formula[fin+1:])

    st_ind = formula.index('[')
    fin_ind = index_close_bracket(formula[st_ind:]) + st_ind

    st_up = formula[fin_ind:].index('{')+fin_ind
    fin_up = index_close_bracket(formula[st_up:]) + st_up

    return "<mroot>" +\
            "<mrow>" + process_expr(formula[st_up+1:fin_up]) + "</mrow>" +\
            "<mrow>" + process_expr(formula[st_ind+1:fin_ind]) + "</mrow>" +\
            "</mroot>" + process_expr(formula[fin_up+1:])


def process_sum(formula):
    if formula[1] != '_':
        return 	"<mo>&#x2211</mo>" + process_expr(formula[1:])

    st_down = formula.index('{')
    fin_down = index_close_bracket(formula[st_down:]) + st_down

    st_up = formula[fin_down:].index('{') + fin_down
    fin_up = index_close_bracket(formula[st_up:]) + st_up

    return "<msubsup><mrow><mo>&#x2211</mo></mrow>" +\
            "<mrow>" + process_expr(formula[st_down+1:fin_down]) + "</mrow>" +\
            "<mrow>" + process_expr(formula[st_up+1:fin_up]) + "</mrow>" + "</msubsup>" +\
            process_expr(formula[fin_up+1:])


def process_log(formula):
    if formula[1] != '_' and formula[1] != '^':
        return 	"<mi>log</mi>" + process_expr(formula[1:])

    if formula[1] == '^':
        st_up = formula.index('{')
        fin_up = index_close_bracket(formula[st_up:]) + st_up

        return "<msup><mrow><mi>log</mi></mrow>" + \
            "<mrow>" + process_expr(formula[st_up + 1:fin_up]) + "</mrow>" + "</msup>" + \
            process_expr(formula[fin_up + 1:])


    st_down = formula.index('{')
    fin_down = index_close_bracket(formula[st_down:]) + st_down

    if formula[fin_down+1] == '^':
        st_up = formula[fin_down:].index('{') + fin_down
        fin_up = index_close_bracket(formula[st_up:]) + st_up

        return "<msubsup><mrow><mi>log</mi></mrow>" +\
                "<mrow>" + process_expr(formula[st_down+1:fin_down]) + "</mrow>" +\
                "<mrow>" + process_expr(formula[st_up+1:fin_up]) + "</mrow>" + "</msubsup>" +\
                process_expr(formula[fin_up+1:])
    return "<msub><mrow><mi>log</mi></mrow>" +\
            "<mrow>" + process_expr(formula[st_down+1:fin_down]) + "</mrow>" + "</msub>" +\
            process_expr(formula[fin_down+1:])


def process_ln(formula):
    if formula[1] != '^':
        return 	"<mi>ln</mi>" + process_expr(formula[1:])


    st_up = formula.index('{')
    fin_up = index_close_bracket(formula[st_up:]) + st_up

    return "<msup><mrow><mi>ln</mi></mrow>" +\
            "<mrow>" + process_expr(formula[st_up+1:fin_up]) + "</mrow>" + "</msup>" +\
            process_expr(formula[fin_up+1:])


def process_bracket(formula):
    open_index = 0
    close_index = index_close_bracket(formula)
    if close_index < 0:
        close_index = len(formula)
    if close_index - open_index == 2 and formula[open_index] == '{':
        return process_expr(formula[1:close_index]) + process_expr(formula[close_index+1:])
    return "<mo>(</mo>" + process_expr(formula[1:close_index]) + "<mo>)</mo>" +\
        process_expr(formula[close_index+1:])


def process_index(formula):
    next_act = next_action(formula)
    if next_act < 0:
        next_act = len(formula)
    place = index_place(formula)

    st_down = formula[place:].index('{') + place
    fin_down = index_close_bracket(formula[st_down:]) + st_down

    #new
    while(formula[next_act-1] != "}"):
        next_act -= 1
    #print(formula[st_down:next_act])
    # new

    if formula[place] == '_':
        if has_index(formula[place + 1:], '^'):
            place2 = index_place(formula[place + 1:]) + place + 1
            st_up = formula[place2:].index('{') + place2
            fin_up = index_close_bracket(formula[st_down:]) + st_up
            return "<msubsup>" + process_expr(formula[:place]) + \
                "<mrow>" + process_expr(formula[st_down + 1:fin_down]) + "</mrow>" + \
                "<mrow>" + process_expr(formula[st_up + 1:fin_up]) + "</mrow></msubsup>" + \
                process_expr(formula[next_act:])
        return "<msub><mrow>" + process_expr(formula[:place])+"</mrow>" + \
            "<mrow>" + process_expr(formula[st_down:next_act]) + "</mrow></msub>" + \
            process_expr(formula[next_act:])

    return "<msup><mrow>" + process_expr(formula[:place])+"</mrow>" + \
        "<mrow>" + process_expr(formula[st_down:next_act]) + "</mrow></msup>" + \
        process_expr(formula[next_act:])


def process_expr(formula):
    #print(formula)
    if len(formula) == 0:
        return ""
    elif formula[0] == "\\frac":
        return process_frac(formula)
    elif formula[0] == "\int":
        return process_int(formula)
    elif formula[0] == "\sqrt":
        return process_sqrt(formula)
    elif formula[0] == "\sum":
        return process_sum(formula)
    elif formula[0] == "\log":
        return process_log(formula)
    elif formula[0] == "\ln":
        return process_ln(formula)
    elif has_index(formula):
        return process_index(formula)
    elif is_open_bracket(formula[0]):
        return process_bracket(formula)
    else:
        return process_elem(formula[0]) + process_expr(formula[1:])


def get_MathML_Presentation(formula):
    #print(formula)
    sep = get_formula_M1(formula)
    #print(get_formula_M1(formula))
    res = process_expr(sep)
    #print(res)
    #print(process_expr(process_expr(get_formula_M1(formula))))
    return '<math xmlns="http://www.w3.org/1998/Math/MathML">' + \
        res + '</math>'

#f_int = "\int \int_1^2 x dx dx"
#print(process_expr(get_formula_M1(f_int)))
#f_frac = "\\frac 1 2 *43 + \\frac 123 + \\frac{\\frac {2^2} {3_1} }{34}"
#print(process_expr(get_formula_M1(f_frac)))
#f_sqrt = "\sqrt [10^12] {34_2+x} + \sqrt{124}"
#print(process_expr(get_formula_M1(f_sqrt)))
#f_sum = "\sum_{i=1^2}^{10} t + i"
#print(process_expr(get_formula_M1(f_sum)))
#f_bracket = "x + (1 + (3 + 3) + (9 + 1))"
#print(process_expr(get_formula_M1(f_int)))
#f_indexes = "k_{i=1}^10 + x^a_1^2^2 = 0"
#f_indexes = "x^2^x+1"
#print(process_expr(get_formula_M1(f_int)))
#f_log = "\log_3^{4+1} x"#
#f_log = "\log_3^{4+1} x"
#print(get_formula_M1(f_log))
#print(process_expr(get_formula_M1(f_log)))
#print(process_expr(get_formula_M1(f_log)))
#f_ln = "\ln^{4+1} x"
#print(process_expr(get_formula_M1(f_ln)))

#f = "a^{2^{4^{3+1}}}+1"
#f="a^{2^{4^{3+1}}}ads+1"
#f = "a^{x+1}^4^{3+1}+1"
#f = "a_2_3_4"
#print(get_formula_M1(f))
#print(process_expr(get_formula_M1(f)))


f = "\sum_{i=1^2}^{10} t + i"
f = "\log^2 x + 1"
f = "(\frac {\sum_{i=1}^10 \int_a_b i^2*x^2^2 dx}{y^{x+1}+\sqrt[n]{9+y^r}})^25"

#f = "\sum_{j=2}^{n}(j-1)={\frac{n(n-1)}{2}}"
print(get_MathML_Presentation(f))
#f = "\int_a^b i^2*x^2 dx"
#f = "\int_a^b i^2*x^2^2 dx"
#f = "x^2"
#print(get_formula_M1(f))
#print(process_expr(get_formula_M1(f)))
#print(get_MathML_Presentation(f))

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


