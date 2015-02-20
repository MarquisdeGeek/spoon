# Spoon

The esoteric language 'Spoon'. Created in 1998 by Steven Goodwin. (Who's, strangely, still proud of it!)

It has been untouched since then. I'm including it in Github for whatever historical amusement may be found!

## Purpose
  'Spoon' is probably the most obsfucated language available. 
  It is certainly the one with the fewest number of tokens - two! 
  It also lets you hide messages inside ASCII graphics, change 
  the tokens themselves, and convert to BrainF***.

## Background
  
The first lesson in most computer courses tells us that there are only states
inside a computers' memory - 1 and 0. Thing is, how many actually get to 
program in 1's and 0's? I therefore decided it was time to write a language
that consisted solely of those two tokens. 

   As I was going to be dealing with two tokens, I thought it might be a 
nice idea to use compression (since each token might concievably by quite 
large).	My current thoughts were to take a subset of 'C', and tokenise the
oft used tokens using Huffman encoding. I was working on a useable subset
when I discovered BrainF***.

   To be correct, I *re*discovered BrainF*** (known henceforth as simply 
'Brain'). I thought wow! I thought oh! I thought damn - somebody's beaten me
to it. But on second glance I believed I still contribute this extension. I
used the operations present in Brain (there are only 8, []<>+-.,) and collated
a number of examples. I ran this through my Huffman program to determine the
best token, based on probability. I got the following table:
```
      1   +
    000	  -
    010	  >
    011	  <
   0011	  ]
  00100	  [
 001010	  .
0010110	  ,         Note: Huffman actually returned '001011', but to save confusion
                          for those quickly scanning the file for token conversions
                          I elected to mis-quote it (see later tokens DEBUG & EXIT
                          for the reasons).
```
   The next task was to write a converter between the two languages, and a
stand alone interpreter. Those are the files present in this directory. The next
thought that entered my head was, 'Why not change the tokens?'. Good idea! I
then added a small feature to permit this. Now, instead of Spoon looking like:

```
00101010010, it could be
AABABABAABA, or
HHiHiHiHHiH, or (for those with a perverse slant on life)
11010101101
```

   So, as well as being the minimalist token language, it's also the most
token-customisable! This facility lends itself to being hidden inside ASCII
graphics (as my examples demonstrate). It is also possible to hardcode the tokens
for '0' and '1 into the interpreter to create a language consisting of only tabs
and carriage returns! Or spaces and tab! Or control codes!


   I then added two additional tokens. The mneumonics for 'DEBUG' and 'EXIT' I
thought were necessary. 'DEBUG' could be used to output the stack, whilst 'EXIT'
would just quit the interpreter. My reasoning was simple, these tokens could be
converted into binary very simply (for ultimate compression), but it might
contain some non-sensence tokens if the code length was not divisible into whole
bytes. I choose two arbitary tokens, namely:
```
00101110   DEBUG
00101111   EXIT
```

   As you can see, it is an obstuse little language. Unfortunately, for those
that have already seen Brain, it offers little.

## Programming with Spoon

For those who have used esoteric languages like Spoon before, here are a few
programming notes.

   Spoon uses a linear address space, initialised to zero at startup. There is
also an address pointer. Only data at this location may be changed. 

   The instructions are:
```
Spoon     Brain    Operation                                 C
       1    +      Increment the number at the ptr           a[p]++
     000    -      Increment the number at the ptr           a[p]--
     010    >      Move the address pointer on 1 space       p++
     011    <      Move the address pointer back 1 space     p--
    0011    ]      End loop - terminates when address!=0     }
   00100    [      Start loop (pre-check)					 while(a[p]){
  001010    .      Prints character at address space         putchar(a[p]);
 0010110    ,      Get char from keyboard, and write to mem  a[p]=getchar()
00101110    #      Outputs stack information
00101111   <eof>   Terminates the program					 exit(0);
```
   Spoon is free form (spaces, tabs and newlines are ignored), and any
characters that are not '1' or '0' are treated as comments and are ignored.

## Next Steps

There isn't one! You can not create a free form language with fewer tokens. It
is possible, of course, to use the following table:
```
       1   +
      11   -
     111   >
    1111   <
   11111   ]
  111111   [
 1111111   .
11111111   ,
```
  ...which uses fewer tokens (1), but then you *need* a delimiter between them.

   I hope to have some free time in order to write a Windows GUI for Spoon, but
that seems unlikely, given my current workload.

   I would love to see some of your examples of Spoon, particularly those
embedded in ASCII art (please remember to send me details of the command line if
you don't use '1' and '0' as your tokens). 


## Challenges


1. Write a program that compiles with:
	spoon file.sp -00 -11    AND
	spoon file.sp -01 -10

2. Repeat 1, but producing identical output

3. Write a self-producing program

4. Produce a readable program, written in Spoon


I have no idea if any of the above are possible. If you manage it, you know my
email...

P.S. A palindromic program is trivial, since EXIT can be embedded into the centre
of the code, and the rest filled with junk that will be never be used. However,
if all the tokens had to feature in the program, it might be worthy of study...


