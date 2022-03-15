function f1
def
enddef
return ci0
end

function f2 pi1
def
var vi1
vet va2 size ci30
var vi3
enddef
return ci-1
end

function f3 pa1 pi2
def
vet va1 size ci10
vet va2 size ci20
var vi3
enddef
return ci5
end

function f4 pi1
def
var vi1
var vi2
enddef
vi1 = ci1
vi2 = vi1
vi1 = pi1 + vi2
vi2 = vi1 * ci-5
return vi1
end

function f5 pi1 pa2
def
enddef
return pi1
end

function f6 pa1
def
var vi1
var vi2
vet va3 size ci30
enddef
vi1 = ci1
vi2 = call f5 vi1 va3
vi2 = call f5 ci5 pa1
return vi2
end

function f7 pi1 pa2
def
var vi1
vet va2 size ci10
enddef
vi1 = pi1 + ci1
set va2 index ci5 with ci2
set pa2 index ci0 with vi1
get va2 index ci8 to vi1
return pi1
end

function f8
def
var vi1
vet va2 size ci10
enddef
vi1 = ci3
if vi1
vi1 = ci0
endif
if vi1 
get va2 index ci8 to vi1
end
if vi1
return vi1
endif
return ci-1
end