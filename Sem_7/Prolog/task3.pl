
%swipl
%consult("task3.pl").

:-['database.pl'].
:- dynamic male/1, female/1, parent/2, married/2.


father(X, Y):-parent(X, Y), male(X).
mother(X, Y):-parent(X, Y), female(X).

brother(X,Y):-
    father(Z,X), father(Z,Y),
    mother(N,X), mother(N,Y),
    male(X), dif(X, Y).

sister(X,Y):-
    father(Z,X), father(Z,Y),
    mother(N,X), mother(N,Y),
    female(X), dif(X, Y).


grandma(X, Y):-female(X), parent(X, Z), parent(Z, Y).
grandfa(X, Y):-male(X), parent(X, Z), parent(Z, Y).

great_grandma(X, Y):-female(X), parent(X, Z), parent(Z, B), parent(B, Y).
great_grandfa(X, Y):-male(X), parent(X, Z), parent(Z, B), parent(B, Y).

uncle(X, Y):- brother(X,Z), parent(Z,Y).
aunt(X, Y):- sister(X,Z), parent(Z,Y).

married_couple(X, Y):- married(X, Y), !.
married_couple(X, Y):- married(Y, X).

svekrov(X, Y):- 
    female(X),
    parent(X, Z), female(Z),
    married_couple(Z, Y).

svekr(X, Y):- 
    male(X),
    parent(X, Z), female(Z),
    married_couple(Z, Y).


husband_mother(X, Y):- 
    female(X),
    parent(X, Z), male(Z),
    married_couple(Z, Y).

husband_father(X, Y):- 
    male(X),
    parent(X, Z), male(Z),
    married_couple(Z, Y).


who_they_are(X, Y):-
    min_pаth(X, Y, Res), print_list(Res).

print_list([_]):-!.
print_list([X, Y|L]):-print_relationship(X, Y),writeln(""), print_list([Y|L]).

count_min([[_,L]], L):-!.
count_min([[_,L]|T], Min):- count_min(T, L2), Min is min(L, L2).

check_min([[Path, Min]|_], Min, Path):-!.
check_min([[_, _]|T], Min, Path):-check_min(T, Min, Path).


min_path(A, B, Res) :-
    findall([Path, L], (my_path(A, B, Path), length(Path, L)), MyList),
    count_min(MyList, Min), check_min(MyList, Min, Res).


my_path(A, A, [A]) :- !.
my_path(A, C, Path) :- my_path(A, C, Path, []).


my_path(A, B, Path, Path2) :-
    get_relationship(A, B), append(Path2, [A, B], Path), not(member(B, Path2)).

my_path(A, B, Path, Path2) :-
    get_relationship(A, Z), not(member(Z, Path2)), append(Path2, [A], Tmp),
    my_path(Z, B, Path, Tmp).


get_relationship(X, Y):- 
    get_relationship(X, Y, 0);
    get_relationship(Y, X, 1).


get_relationship(X, Y, Z):- 
    father(X, Y),!;
    mother(X, Y),!;
    (Z is 0, married_couple(X, Y)),!;
    grandma(X, Y),!;
    grandfa(X, Y),!;
    great_grandma(X, Y),!;
    great_grandfa(X, Y),!;
    sister(X, Y),!;
    brother(X, Y),!;
    uncle(X, Y),!;
    aunt(X, Y),!;
    svekrov(X, Y),!;
    svekr(X, Y),!;
    husband_mother(X, Y),!;
    husband_father(X, Y).




print_relationship(X, Y):- 
    print_relationship(X, Y, 0),!;
    print_relationship(Y, X, 1).
    
print_relationship(X, Y, _):- 
    father(X, Y),write(X),write(" is father of "), write(Y),!;
    mother(X, Y),write(X),write(" is mother of "), write(Y),!;
    married_couple(X, Y),write(X),write(" is married with "), write(Y),!;
    grandma(X, Y),write(X),write(" is grandma of "), write(Y),!;
    grandfa(X, Y),write(X),write(" is grandfa of "), write(Y),!;
    great_grandma(X, Y),write(X),write(" is great_grandma of "), write(Y),!;
    great_grandfa(X, Y),write(X),write(" is great_grandfa of "), write(Y),!;
    sister(X, Y),write(X),write(" is sister of "), write(Y),!;
    brother(X, Y),write(X),write(" is brother of "), write(Y),!;
    uncle(X, Y),write(X),write(" is uncle of "), write(Y),!;
    aunt(X, Y),write(X),write(" is aunt of "), write(Y),!;
    svekrov(X, Y),write(X),write(" is svekrov of "), write(Y),!;
    svekr(X, Y),write(X),write(" is svekr of "), write(Y),!;
    husband_mother(X, Y),write(X),write(" is mother of "), write(Y), write("'s husband"),!;
    husband_father(X, Y),write(X),write(" is father of "), write(Y), write("'s husband").



have_person(X):-female(X), !.
have_person(X):-male(X).

add_person(X, mother, Y):-not(have_person(X)), have_person(Y),
                        assert(female(X)), assert(parent(X, Y)), write("done"), !.
add_person(X, mother, _):-have_person(X), write("mother exist already"), !.
add_person(_, mother, _):-write("Don't have the child"), !.

add_person(X, father, Y):-not(have_person(X)), have_person(Y),
                        assert(male(X)), assert(parent(X, Y)), write("done"), !.
add_person(X, father, _):-have_person(X), write("father exist already"), !.
add_person(_, father, _):-write("Don't have the child"), !.

add_person(X, married, Y):-not(have_person(X)), male(Y),
                        assert(female(X)), assert(married(X, Y)), write("done"), !.
add_person(X, married, Y):-not(have_person(X)), female(Y),
                        assert(male(X)), assert(married(X, Y)), write("done"), !.
add_person(X, married, _):-have_person(X), write("first person exist already"), !.
add_person(_, married, _):-write("Don't have the second person"), !.



add_person(X, son, Y):-not(have_person(X)), have_person(Y),
                        married_couple(Y, Z),
                        assert(male(X)), assert(parent(Y, X)), assert(parent(Z, X)),write("done"), !.
add_person(X, son, _):-have_person(X), write("first person exist already"), !.
add_person(_, son, _):-write("Don't have the second person"), !.


add_person(X, daughter, Y):-not(have_person(X)), have_person(Y),
                        married_couple(Y, Z),
                        assert(female(X)), assert(parent(Y, X)),assert(parent(Z, X)), write("done"), !.
add_person(X, daughter, _):-have_person(X), write("first person exist already"), !.
add_person(_, daughter, _):-write("Don't have the second person"), !.

add_person(X, sister, Y):-not(have_person(X)), have_person(Y),
    					parent(Z, Y), married_couple(F, Z),
                        assert(female(X)), assert(parent(Z, X)), assert(parent(F, X)), write("done"), !.
add_person(X, sister, _):-have_person(X), write("sister exist already"), !.
add_person(_, sister, _):-write("Don't have the sibling"), !.

add_person(X, brother, Y):-not(have_person(X)), have_person(Y),
    					parent(Z, Y), married_couple(F, Z),
                        assert(male(X)), assert(parent(Z, X)), assert(parent(F, X)),write("done"), !.
add_person(X, brother, _):-have_person(X), write("brother exist already"), !.
add_person(_, brother, _):-write("Don't have the sibling"), !.




run:- 
    write("\n"),
    write("Enter a num to perfom an action:\n"),
    write("1 - find the certain relatives of known person\n"),
    write("2 - get relationships between known people\n"),
    write("3 - add new person\n"),
    write("0 - finish the programm\n\n"),
    write('Enter a num: '), read(C), write("\n"), run(C), !.

run(0):- write('Goodbye\n').

run(1):-
    write("Enter a known person"), read(X),
    write("Enter relationship: "), read(Z),
    run(1, X, Z),write("\n"),
    run, !.

run(2):-
    write("Enter the first person"), read(X),
    write("Enter the second person"), read(Y),
    who_they_are(X, Y), write("\n"),
    run, !.

run(3):-
    write("Enter a new person"), read(X),
    write("Enter relationship"), read(Z),
    write("Enter a known person"), read(Y),
    add_person(X, Z, Y),write("\n"),
    run, !.


run(C):- 
    write("choose a number dif from"), write(C),
    write("\n"), 
    run, !.

pl_lst([]).
pl_lst([A|B]):-write(A), write("\n"), pl_lst(B).


run(1, X, father):-
    findall([Y], (father(X, Y)), MyList),
    write(X), write(" is father of:\n"),
    pl_lst(MyList).


run(1, X, mother):-
    findall([Y], (mother(X, Y)), MyList),
    write(X), write(" is mother of:\n"),
    pl_lst(MyList).


run(1, X, child):-
    findall([Y], (parent(Y, X)), MyList),
    write(X), write(" is child of:\n"),
    pl_lst(MyList).


run(1, X, sister):-
    findall([Y], (sister(X, Y)), MyList),
    write(X), write(" is sister of:\n"),
    pl_lst(MyList).


run(1, X, brother):-
    findall([Y], (brother(X, Y)), MyList),
    write(X), write(" is brother of:\n"),
    pl_lst(MyList).


run(1, X, married):-
    findall([Y], (married_couple(X, Y)), MyList),
    write(X), write(" is father of:\n"),
    pl_lst(MyList).

run(1, X, aunt):-
    findall([Y], (aunt(X, Y)), MyList),
    write(X), write(" is aunt of:\n"),
    pl_lst(MyList).


run(1, X, uncle):-
    findall([Y], (uncle(X, Y)), MyList),
    write(X), write(" is uncle of:\n"),
    pl_lst(MyList).

run(1, X, niece):-
    findall([Y], (aunt(Y, X)), MyList1),
    findall([Y], (uncle(Y, X)), MyList2),
    write(X), write(" is niece of:\n"),
    pl_lst(MyList1), pl_lst(MyList2).



run(1, X, grandma):-
    findall([Y], (grandma(X, Y)), MyList),
    write(X), write(" is grandma of:\n"),
    pl_lst(MyList).

run(1, X, grandfa):-
    findall([Y], (grandfa(X, Y)), MyList),
    write(X), write(" is grandfa of:\n"),
    pl_lst(MyList).


run(1, X, grandson):-
    findall([Y], (grandfa(Y, X)), MyList1),
    findall([Y], (grandma(Y, X)), MyList2),
    write(X), write(" is grandson of:\n"),
    pl_lst(MyList1), pl_lst(MyList2).

run(1, X, granddaughter):-
    findall([Y], (grandfa(Y, X)), MyList1),
    findall([Y], (grandma(Y, X)), MyList2),
    write(X), write(" is granddaughter of:\n"),
    pl_lst(MyList1), pl_lst(MyList2).
    

run(1, X, great_grandma):-
    findall([Y], (great_grandma(X, Y)), MyList),
    write(X), write(" is great_grandma of:\n"),
    pl_lst(MyList).

run(1, X, great_grandfa):-
    findall([Y], (great_grandfa(X, Y)), MyList),
    write(X), write(" is great_grandfa of:\n"),
    pl_lst(MyList).

run(1, X, great_grandson):-
    findall([Y], (great_grandfa(Y, X)), MyList1),
    findall([Y], (great_grandma(Y, X)), MyList2),
    write(X), write(" is great_grandson of:\n"),
    pl_lst(MyList1), pl_lst(MyList2).

run(1, X, great_granddaughter):-
    findall([Y], (great_grandfa(Y, X)), MyList1),
    findall([Y], (great_grandma(Y, X)), MyList2),
    write(X), write(" is great_granddaughter of:\n"),
    pl_lst(MyList1), pl_lst(MyList2).

run(1, X, svekrov):-
    findall([Y], (svekrov(X, Y)), MyList),
    write(X), write(" is svekrov of:\n"),
    pl_lst(MyList).

run(1, X, svekr):-
    findall([Y], (grandma(X, Y)), MyList),
    write(X), write(" is svekr of:\n"),
    pl_lst(MyList).

run(1, X, husband_mother):-
    findall([Y], (husband_mother(X, Y)), MyList),
    write(X), write(" is husband_mother of:\n"),
    pl_lst(MyList).


run(1, X, husband_father):-
    findall([Y], (husband_father(X, Y)), MyList),
    write(X), write(" is husband_father of:\n"),
    pl_lst(MyList).
