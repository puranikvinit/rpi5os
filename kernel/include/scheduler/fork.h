#ifndef FORK_H
#define FORK_H

// Member APIs

int fork_process(unsigned long function, unsigned long args);

extern void return_from_fork();

#endif // !FORK_H
