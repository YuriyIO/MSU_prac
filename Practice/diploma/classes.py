



id = ['\\alpha', '\Alpha',
    '\\beta', '\Beta',
    '\gamma', '\Gamma',
    '\pi', '\Pi',
    '\phi', '\Phi',
    '\\varphi', '\\varPhi',
    '\mu',
    "\\theta",
    '\lambda',
    '\\tau',
    '\omega', '\Omega',
    "\infty"]

id_in_mathML = ['&#x03b1', '&#x0391',#alpha
                '&#x03b2', '&#x0392',#beta
                '&#x03b3', '&#x0393',#gamma
                '&#x03c0', '&#x03a0',#pi
                '&#x03c6', '&#x03a6',#phi
                '&#x03c6', '&#x03a6',#varphi
                '&#x03bc',#mu
                '&#x03b8',#theta
                '&#x03bb',#lambda
                '&#x03c4',#tau
                '&#x03c9', '&#x03a9',#omega
                '&#x221E'#infty
                ]

action = ['+', '-', ':', '/', '\\times',
          '=', '<', '\leq', '>', '\geq',
          '!']

bracket = ['(', ')', '[', ']', '{', '}',
           "\\right(", "\\right[", "\\right{", '\\left)', '\\left]', '\\left}']

open_bracket = ['(',  '[',  '{', "\\right(", "\\right[", "\\right{"]

close_bracket = [')', ']', '}', '\\left)', '\\left]', '\\left}']

trigonometry = ['\sin', '\cos', '\\tan', '\cot',
                '\\arcsin', '\\arccos', '\\arctan', '\\arccot']

indexes = ['^', '_']

op = ['\\frac', '\sqrt', '\sum', '\int', '\log', '\ln']

def is_op(c):
    return c in op

def is_index(c):
    return c in indexes


def is_id(c):
    return c in id or c.isalpha() or c[0] == "\\" and not is_op(c)

def conv_id(c):
    if c[0] != "\\":
        return c
    if c not in id:
        return c[1:]
    return id_in_mathML[id.index(c)]


def is_num(c):
    z = c.find(',')
    if z == -1:
        return c.isdigit()
    return c[0:z].isdigit() and c[z+1:].isdigit()


def is_open_bracket(c):
    return c in open_bracket


def is_close_bracket(c):
    return c in close_bracket


def is_trigonom(c):
    return c in trigonometry
