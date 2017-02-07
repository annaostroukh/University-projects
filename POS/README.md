POS (Advanced Operating Systems)
-------------
**Task for the 1st project**

Using a programming language C create a program which runs a child thread and provides a communication between child and parent threads by sending signals. 

Task of parent and child threads is one by one type letters in alphabetical order (in cycle A - Z). Alternation of parent and child thread has to be provided by sending SIGUSR1 signal - parent thread will send the signal right after it types a letter, so child thread does.  Before parent thread will type a letter, it has to show a call and wait until the keyboard button is pressed, but anyway it has to be done after accepting the SIGUSR1 signal from the child thread. For keyboard input is enough to use getchar() function and respond to the Enter button. First starts parent thread by typing letter A (instead of pid it has to show the current number of a process).

> **Example:**

> Parent (pid): 'A'
> Parent (pid): 'A'
> Child  (pid): 'A'
> Press enter...
> Parent (pid): 'B'
> Child  (pid): 'B'
> Press enter...
> Parent (pid): 'C'
> Child  (pid): 'C'
> Press enter...
> ...

If any of processes gets the SIGUSR2 signal (from the shell kill -s USR2 pid), the relevant process has to continue from the beginning of the alphabet by typing letter A.

The program has to be according the POSIX standart, without busy waiting and it will respond just to SIGUSR1 a SIGUSR2 signals. 