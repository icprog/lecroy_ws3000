

dbpf "$(P)$(R).OFMT" "ASCII"
dbpf "$(P)$(R).IFMT" "Hybrid"
dbpf "$(P)$(R).TMOD" "Write/Read"

dbpf "$(P)$(R).AOUT" "*IDN?"
epicsThreadSleep 2

echo In another window run caget -S $(P)$(R).BINP and confirm that the IDN string was read.

dbpr "$(P)$(R)" 1

