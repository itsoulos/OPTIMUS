#ifndef GETOPTIONS_H
#define GETOPTIONS_H
# include <QString>
# include <unistd.h>
# include <string.h>
# include <stdio.h>
# include <stdlib.h>

extern QString  filename;
extern int      randomSeed;
extern int      threads;
extern QString  optMethod;
extern bool     debug;
extern int iterations;

#endif // GETOPTIONS_H
