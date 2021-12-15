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

# Bit Fields:

In my experience, these are rarely used, but you might see them out there from time to time, especially in lower-level applications that pack bits together into larger spaces.

Let’s take a look at some code to demonstrate a use case:

```c
#include <stdio.h>

struct foo {
    unsigned int a;
    unsigned int b;
    unsigned int c;
    unsigned int d;
};

int main(void)
{
    printf("%zu\n", sizeof(struct foo));
}
```

For me, this prints 16. Which makes sense, since unsigneds are 4 bytes on my system.

But what if we knew that all the values that were going to be stored in a and b could be stored in 5 bits, and the values in c, and d could be stored in 3 bits? That’s only a total 16 bits. Why have 128 bits reserved for them if we’re only going to use 16?

Well, we can tell C to pretty-please try to pack these values in. We can specify the maximum number of bits that values can take (from 1 up the size of the containing type).

```c
struct foo {
    unsigned int a:5;
    unsigned int b:5;
    unsigned int c:3;
    unsigned int d:3;
};
```

Now when I ask C how big my struct foo is, it tells me 4! It was 16 bytes, but now it’s only 4. It has “packed” those 4 values down into 4 bytes, which is a four-fold memory savings.

The tradeoff is, of course, that the 5-bit fields can only hold values from 0-31 and the 3-bit fields can only hold values from 0-7. But life’s all about compromise, after all.

Source: https://beej.us/guide/bgc/html/split/structs-ii-more-fun-with-structs.html#bit-fields

# What is Unicode?

Back in the day, it was popular in the US and much of the world to use a 7-bit or 8-bit encoding for characters in memory. This meant we could have 128 or 256 characters (including non-printable characters) total. That was fine for a US-centric world, but it turns out there are actually other alphabets out there—who knew? Chinese has over 50,000 characters, and that’s not fitting in a byte.

So people came up with all kinds of alternate ways to represent their own custom character sets. And that was fine, but turned into a compatibility nightmare.

To escape it, Unicode was invented. One character set to rule them all. It extends off into infinity (effectively) so we’ll never run out of space for new characters. It has Chinese, Latin, Greek, cuneiform, chess symbols, emojis… just about everything, really! And more is being added all the time!

## Unicode in C

Before I get into encoding in C, let’s talk about Unicode from a code point standpoint. There is a way in C to specify Unicode characters and these will get translated by the compiler into the execution character.

So how do we do it?

How about the euro symbol, code point 0x20AC. (I’ve written it in hex because both ways of representing it in C require hex.) How can we put that in our C code?

Use the \u escape to put it in a string, e.g. "\u20AC" (case for the hex doesn’t matter). You must put exactly four hex digits after the \u, padding with leading zeros if necessary.

Here’s an example:

```c
char *s = "\u20AC1.23";

printf("%s\n", s);  // €1.23
```

So \u works for 16-bit Unicode code points, but what about ones bigger than 16 bits? For that, we need capitals: \U.

For example:

```c
char *s = "\U0001D4D1";

printf("%s\n", s);  // Prints a mathematical letter "B"
```

It’s the same as \u, just with 32 bits instead of 16. These are equivalent:

```
\u03C0
\U000003C0
```

Again, these are translated into the execution character set during compilation. They represent Unicode code points, not any specific encoding. Furthermore, if a Unicode code point is not representable in the execution character set, the compiler can do whatever it wants with it.

Now, you might wonder why you can’t just do this:

```c
char *s = "€1.23";

printf("%s\n", s);  // €1.23
```

And you probably can, given a modern compiler. The source character set will be translated for you into the execution character set by the compiler. But compilers are free to puke out if they find any characters that aren’t included in their extended character set, and the € symbol certainly isn’t in the basic character set.

Caveat from the spec: you can’t use \u or \U to encode any code points below 0xA0 except for 0x24 ($), 0x40 (@), and 0x60 (\`) —yes, those are precisely the trio of common punctuation marks missing from the basic character set. Apparently this restriction is relaxed in the upcoming version of the spec.

Finally, you can also use these in identifiers in your code, with some restrictions. But I don’t want to get into that here. We’re all about string handling in this chapter.

And that’s about it for Unicode in C.

Source: https://beej.us/guide/bgc/html/split/unicode-wide-characters-and-all-that.html#what-is-unicode
