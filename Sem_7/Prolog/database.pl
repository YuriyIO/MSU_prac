male(kosty).
male(tomas).
male(bob).
male(alex).
male(petr).
male(max).
male(andrey).
male(dave).
male(steve).
male(arnold).
male(mark).
male(nick).

female(zina).
female(ira).
female(alice).	
female(kate).
female(eva).
female(marge).
female(olga).
female(jane).
female(kira).
female(sveta).
female(liza).


married(zina,kosty).
married(ira,tomas).
married(alice,bob).
married(eva,petr).
married(marge,max).
married(olga,andrey).
married(sveta, mark).


parent(zina,alice).
parent(kosty,alice).

parent(ira,bob).
parent(tomas,bob).
parent(ira,alex).
parent(tomas,alex).
parent(ira,kate).
parent(tomas,kate).

parent(alice,petr).
parent(bob,petr).
parent(alice,max).
parent(bob,max).
parent(alice,olga).
parent(bob,olga).

parent(eva,jane).
parent(petr,jane).
parent(eva, liza).
parent(petr, liza).

parent(marge,dave).
parent(max,dave).
parent(marge,kira).
parent(max,kira).

parent(olga,steve).
parent(andrey,steve).
parent(olga,sveta).
parent(andrey,sveta).
parent(olga,arnold).
parent(andrey,arnold).



parent(sveta, nick).
parent(mark,nick).




get_relationsship(X, Y):- 
    get_relationsship(X, Y, 0);
    get_relationsship(Y, X, 1).


get_relationsship(X, Y, Z):- 
    father(X, Y);
    mother(X, Y);
    (Z is 0, married_couple(X, Y));
    grandma(X, Y);
    grandfa(X, Y);
    great_grandma(X, Y);
    great_grandfa(X, Y);
    sister(X, Y);
    brother(X, Y);
    uncle(X, Y);
    aunt(X, Y);
    svekrov(X, Y);
    svekr(X, Y);
    husband_mother(X, Y);
    husband_father(X, Y).


min_pаth(A, B, [A, B]):- 
    get_relationsship(A, B),!.
min_pаth(A, B, [A, Z, B]):- 
    get_relationsship(A, Z), get_relationsship(Z, B),!.
min_pаth(A, B, [A, Z, D, B]):- 
    get_relationsship(A, Z), get_relationsship(Z, D), 
    get_relationsship(D, B),!.
min_pаth(A, B, [A, Z, D, M, B]):- 
    get_relationsship(A, Z), get_relationsship(Z, D), 
    get_relationsship(D, M), get_relationsship(M, B),!.
min_pаth(A, B, [A, Z, D, M, K, B]):- get_relationsship(A, Z), get_relationsship(Z, D), 
    get_relationsship(D, M), get_relationsship(M, K),
    get_relationsship(K, B),!.
min_pаth(A, B, [A, Z, D, M, K, L, B]):- get_relationsship(A, Z), get_relationsship(Z, D), 
    get_relationsship(D, M), get_relationsship(M, K),
    get_relationsship(K, L),get_relationsship(L, B),!.


min_pаth2(A, B, [A, B]):- 
    get_relationsship(A, B).
min_pаth2(A, B, [A, Z, B]):- 
    get_relationsship(A, Z), get_relationsship(Z, B).
min_pаth2(A, B, [A, Z, D, B]):- 
    get_relationsship(A, Z), get_relationsship(Z, D), 
    get_relationsship(D, B).
min_pаth2(A, B, [A, Z, D, M, B]):- 
    get_relationsship(A, Z), get_relationsship(Z, D), 
    get_relationsship(D, M), get_relationsship(M, B).
min_pаth2(A, B, [A, Z, D, M, K, B]):- get_relationsship(A, Z), get_relationsship(Z, D), 
    get_relationsship(D, M), get_relationsship(M, K),
    get_relationsship(K, B).
min_pаth2(A, B, [A, Z, D, M, K, L, B]):- get_relationsship(A, Z), get_relationsship(Z, D), 
    get_relationsship(D, M), get_relationsship(M, K),
    get_relationsship(K, L),get_relationsship(L, B).




gget_relationsship(X, Y):- 
    gget_relationsship(X, Y, 0);
    gget_relationsship(Y, X, 1).

gget_relationsship(X, Y, Z):- 
    father(X, Y);
    mother(X, Y);
    (Z is 0, married_couple(X, Y));
    sister(X, Y);
    brother(X, Y);
    uncle(X, Y);
    aunt(X, Y);
    svekrov(X, Y);
    svekr(X, Y);
    husband_mother(X, Y);
    husband_father(X, Y).


min_pаth3(A, B, [A, B]):- 
    gget_relationsship(A, B),!.
min_pаth3(A, B, [A, Z, B]):- 
    gget_relationsship(A, Z), gget_relationsship(Z, B),!.
min_pаth3(A, B, [A, Z, D, B]):- 
    gget_relationsship(A, Z), gget_relationsship(Z, D), 
    gget_relationsship(D, B),!.
min_pаth3(A, B, [A, Z, D, M, B]):- 
    gget_relationsship(A, Z), gget_relationsship(Z, D), 
    gget_relationsship(D, M), gget_relationsship(M, B),!.
min_pаth3(A, B, [A, Z, D, M, K, B]):- gget_relationsship(A, Z), gget_relationsship(Z, D), 
    gget_relationsship(D, M), gget_relationsship(M, K),
    gget_relationsship(K, B),!.
min_pаth3(A, B, [A, Z, D, M, K, L, B]):- gget_relationsship(A, Z), gget_relationsship(Z, D), 
    gget_relationsship(D, M), gget_relationsship(M, K),
    gget_relationsship(K, L),gget_relationsship(L, B),!.


ggget_relationsship(X, Y):- 
    ggget_relationsship(X, Y, 0);
    ggget_relationsship(Y, X, 1).

ggget_relationsship(X, Y, Z):- 
    father(X, Y);
    mother(X, Y);
    (Z is 0, married_couple(X, Y));
    sister(X, Y);
    brother(X, Y);
    grandma(X, Y);
    grandfa(X, Y);
    uncle(X, Y);
    aunt(X, Y);
    svekrov(X, Y);
    svekr(X, Y);
    husband_mother(X, Y);
    husband_father(X, Y).


min_pаth4(A, B, [A, B]):- 
    ggget_relationsship(A, B),!.
min_pаth4(A, B, [A, Z, B]):- 
    ggget_relationsship(A, Z), ggget_relationsship(Z, B),!.
min_pаth4(A, B, [A, Z, D, B]):- 
    ggget_relationsship(A, Z), ggget_relationsship(Z, D), 
    ggget_relationsship(D, B),!.
min_pаth4(A, B, [A, Z, D, M, B]):- 
    ggget_relationsship(A, Z), ggget_relationsship(Z, D), 
    ggget_relationsship(D, M), ggget_relationsship(M, B),!.
min_pаth4(A, B, [A, Z, D, M, K, B]):- ggget_relationsship(A, Z), ggget_relationsship(Z, D), 
    ggget_relationsship(D, M), ggget_relationsship(M, K),
    ggget_relationsship(K, B),!.
min_pаth4(A, B, [A, Z, D, M, K, L, B]):- ggget_relationsship(A, Z), ggget_relationsship(Z, D), 
    ggget_relationsship(D, M), ggget_relationsship(M, K),
    ggget_relationsship(K, L),ggget_relationsship(L, B),!.