
# Fuzzing Program Slices

The purpose is to investigate fuzzing program slices. There are a few
possible applications of this including targeting new code added to a 
source tree or a certain piece of a parser. I believe there are 
multiple approaches that could be taken to do this and some closely
related work, but this here is just my experiments. Feel free to include
or contribute work to this project; my intention is to just explore this
space some, so the more explorers the merrier :D.

## Idea in abstract

A program P takes some file as an input. The file is parsed for some reason
and so we want to target the parser(s) acting on the file for fuzz testing.
Say this file's format is such in which that it can have some common header
and that header has a field that indicates some formatting of the rest of the
file. So, perhaps there is a common parse_header() and then from that it would
look at a field indicating the type for the rest of the file's format. So 
depending on that type, it may call parse_sub_A() or parse_sub_B() or something.
Each of these being their own parser for the rest of the file. The goal here is
to target not the entire program P, but a specific parser acting on a certain 
type of input. Why? For a few reasons we may want to do this... including it's a
new type of file to be parsed and so only want to fuzz test the new code.


So the idea is we take P and perform some analysis on it, based on some input
that reaches the target parser. That is, perhaps some dependency graph 
generation is performed or control flow trace from execution. This is used
as inputs to a slicing algorithm. This (r) slicing algorithm will produce
a trimmed version of the program P in favor of fuzzing the certain, targeted
parser. This trimmed version should be in a state where it can still be used
by a fuzzing system, eg afl-fuzz. The fuzzing done on the program slice will
possibly generate a bunch of input samples that can crash this slice. One must
then take these samples and attempt to see if they will crash the original
or if they are ``easily mapped'' to samples that do crash the original.

So in a way, we have:

```
Program P. Input I.
A <- Analysis(P, I)
P' <- Slice(P, I, A)
{C'} <- Fuzz(P', I)
{C} <- MapCrash(P, P', {C'})
```

So, the real goal is that you can slice in such a way that MapCrash() is the 
identity on some/most of the inputs. This is not really realistic. So the 
secondary goal is that you can generate crashes that are, under some 
measure, a small distance from a sample that will crash the original P. What
that metric is, I think, can depend on how you view the problem and the MapCrash
function.


## Naive work

Some [naive work](https://github.com/roachspray/acsac17wip)  has been implemented
and at this point only weakly tested. There are some other things I would like to do
with that code, such as instead of branch removal, branch to exit()'s, and cascade 
fuzzing (described in the ACSAC slides).


## Exit blocks

An improvement for the mapping phase, but perhaps not from other
perspectives, is to leave branches to blocks you don't want to reach, but
insert exit(0) calls there. This reduces the fuzz space, as desired, and
lets any inputs map back to the original program. The issue is you still
have the branches. The code in exitblocks/ shows how this works, using the
same weak trace input as naive. 


