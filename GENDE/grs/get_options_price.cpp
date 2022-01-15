# include <unistd.h>
# include <string.h>
# include <stdio.h>
# include <stdlib.h>

int genome_count;
int genome_length;
double selection_rate;
double mutation_rate;
int random_seed;
char outputfile[1024];
const char *short_options="hp:c:l:s:m:r:o:";

void	print_usage()
{
	printf("\t-h	Print this help screen.\n"
			"\t-c	Specify population size.\n"
			"\t-l	Specify genome length.\n"
			"\t-s	Specify selection rate.\n"
			"\t-r	Specify random seed.   \n"
			"\t-o	Specify output file.   \n"
			"\t-m	Specify	mutation  rate.\n");
}

void	parse_cmd_line(int argc,char **argv)
{
	int next_option;
	char	genome_count_string[100];
	strcpy(genome_count_string,"10");
	char	genome_length_string[100];
	strcpy(genome_length_string,"200");
	char	selection_rate_string[100];
	strcpy(selection_rate_string,"0.1");
	char	mutation_rate_string[100];
	strcpy(mutation_rate_string,"0.05");
	char random_seed_string[100];
	strcpy(random_seed_string,"0");

	do
	{
              next_option=getopt(argc,argv,short_options);
	      switch(next_option)
	      {
		      case 'c':
			      strcpy(genome_count_string,optarg);
			      break;
		      case 'l':
			      strcpy(genome_length_string,optarg);
			      break;
		      case 's':
			      strcpy(selection_rate_string,optarg);
			      break;
		      case 'm':
			      strcpy(mutation_rate_string,optarg);
			      break;
		      case 'r':
			      strcpy(random_seed_string,optarg);
			      break;
		      case 'o':
			      strcpy(outputfile,optarg);
			      break;
		      case 'h':
			      print_usage();
			      exit(0);
			      break;
		      case -1:
			      break;
		      case '?':
			      print_usage();
			      exit(1);
			      break;
		      default:
			      print_usage();
			      exit(1);
			      break;
	      }
	}while(next_option!=-1);
	genome_count=atoi(genome_count_string);
	genome_length=atoi(genome_length_string);
	selection_rate=atof(selection_rate_string);
	mutation_rate=atof(mutation_rate_string);
	random_seed=atoi(random_seed_string);
}
