# cmpe300

Bogazici University(Turkey) CMPE300 (Algorithm Analysis) course project implementation. Game of Life project using OpenMPI in C++.

# implementation
grid core map and periodic neighboring (like toroid) <br>
at "sourceCode/game.cpp" 

# testcases
use "testCases/textoutputs/rand_000.txt" as input and "rand_020.txt" as output (20 is the default number of iterations)

or you may change the number of iterations in the "sourceCode/runner.sh" 

# running

use "sourceCode/runner.sh" as bash script to run code. it is easy to understand or you may check the assignment pdf.
as default it is running with 144(slave) + 1(master) cores. My setup contains 8 logical cores and with 145 cores and 20 iteration, it takes apprx. 2 seconds.
