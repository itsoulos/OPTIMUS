# ifndef __GETOPTIONS__H
# define __GETOPTIONS__H

extern int 	genome_count;
extern int 	genome_length;
extern double 	selection_rate;
extern double 	mutation_rate;
extern void   	parse_cmd_line(int argc,char **argv);
extern int  	random_seed;
extern char outputfile[1024];
# endif
