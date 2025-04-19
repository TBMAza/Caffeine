#ifndef AUTOMATA
#define AUTOMATA

State nat_automaton(char target, Bool reset);
State nint_automaton(char target, Bool reset);
State nreal_automaton(char target, Bool reset);
State preal_automaton(char target, Bool reset);
State word_automaton(char target, Bool reset);

#endif
