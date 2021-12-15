# minitalk

The purpose of this project is to code a small data exchange program using UNIX signals.

Thanks to [0x00Jeff](https://github.com/0x00Jeff) for the great help ❤️.

# What is signal?

A signal is an event which is generated to notify a process or thread that some important situation has arrived. When a process or thread has received a signal, the process or thread will stop what its doing and take some action. Signal may be useful for inter-process communication.

## Standard Signals

The signals are defined in the header file signal.h as a macro constant. Signal name has started with a “SIG” and followed by a short description of the signal. So, every signal has a unique numeric value. Your program should always use the name of the signals, not the signals number. The reason is signal number can differ according to system but meaning of names will be standard.

The macro NSIG is the total number of signal defined. The value of NSIG is one greater than the total number of signal defined (All signal numbers are allocated consecutively).

Source: https://linuxhint.com/signal_handlers_c_programming_language/

# Signal Handling

The signal() call takes two parameters: the signal in question, and an action to take when that signal is raised.

The action can be one of three things:

- A pointer to a handler function.
- SIG_IGN to ignore the signal.
- SIG_DFL to restore the default handler for the signal.

Let’s write a program that you can’t CTRL-C out of. (Don’t fret—in the following program, you can also hit RETURN and it’ll exit.)

```c
#include <stdio.h>
#include <signal.h>

int main(void)
{
    char s[1024];

    signal(SIGINT, SIG_IGN);    // Ignore SIGINT, caused by ^C

    printf("Try hitting ^C... (hit RETURN to exit)\n");

    // Wait for a line of input so the program doesn't just exit
    fgets(s, sizeof s, stdin);
}
```

Check out line 8—we tell the program to ignore SIGINT, the interrupt signal that’s raised when CTRL-C is hit. No matter how much you hit it, the signal remains ignored. If you comment out line 8, you’ll see you can CTRL-C with impunity and quit the program on the spot.

## Writing Signal Handlers

I mentioned you could also write a handler function that gets called with the signal is raised.

These are pretty straightforward, are also very capability-limited when it comes to the spec.

Before we start, let’s look at the function prototype for the signal() call:

```
void (*signal(int sig, void (*func)(int)))(int);
```

Pretty easy to read, right?

WRONG! :)

Let’s take a moment to take it apart for practice.

signal() takes two arguments: an integer sig representing the signal, and a pointer func to the handler (the handler returns void and takes an int as an argument), highlighted below:

```
                sig          func
              |-----|  |---------------|
void (*signal(int sig, void (*func)(int)))(int);
```

Basically, we’re going to pass in the signal number we’re interesting in catching, and we’re going to pass a pointer to a function of the form:

```
void f(int x);
```

that will do the actual catching.

Now—what about the rest of that prototype? It’s basically all the return type. See, signal() will return whatever you passed as func on success… so that means it’s returning a pointer to a function that returns void and takes an int as an argument.

```
returned
function    indicates we're              and
returns     returning a                  that function
void        pointer to function          takes an int
|--|        |                                   |---|
void       (*signal(int sig, void (*func)(int)))(int);
```

Also, it can return SIG_ERR in case of an error.

Let’s do an example where we make it so you have to hit CTRL-C twice to exit.

I want to be clear that this program engages in undefined behavior in a couple ways. But it’ll probably work for you, and it’s hard to come up with portable non-trivial demos.

```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int count = 0;

void sigint_handler(int signum)
{
    // The compiler is allowed to run:
    //
    //   signal(signum, SIG_DFL)
    //
    // when the handler is first called. So we reset the handler here:
    signal(SIGINT, sigint_handler);

    (void)signum;   // Get rid of unused variable warning

    count++;                       // Undefined behavior
    printf("Count: %d\n", count);  // Undefined behavior

    if (count == 2) {
        printf("Exiting!\n");      // Undefined behavior
        exit(0);
    }
}

int main(void)
{
    signal(SIGINT, sigint_handler);

    printf("Try hitting ^C...\n");

    for(;;);  // Wait here forever
}
```

One of the things you’ll notice is that on line 14 we reset the signal handler. This is because C has the option of resetting the signal handler to its SIG_DFL behavior before running your custom handler. In other words, it could be a one-off. So we reset it first thing so that we handle it again for the next one.

We’re ignoring the return value from signal() in this case. If we’d set it to a different handler earlier, it would return a pointer to that handler, which we could get like this:

```
// old_handler is type "pointer to function that takes a single
// int parameter and returns void":

void (*old_handler)(int);

old_handler = signal(SIGINT, sigint_handler);
```

That said, I’m not sure of a common use case for this. But if you need the old handler for some reason, you can get it that way.

Quick note on line 16—that’s just to tell the compiler to not warn that we’re not using this variable. It’s like saying, “I know I’m not using it; you don’t have to warn me.”

And lastly you’ll see that I’ve marked undefined behavior in a couple places. More on that in the next section.

Source: https://beej.us/guide/bgc/html/split/signal-handling.html

# Take your knowledge to the next level:

- https://elixir.bootlin.com/linux/latest/source/include/linux/signal.h
