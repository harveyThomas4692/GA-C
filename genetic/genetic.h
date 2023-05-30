/******************************************************************************************/
/* genetic.h - a realisation of genetic algorithms */
/******************************************************************************************/
/* libraries */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

/******************************************************************************************/
/* constants */
/******************************************************************************************/

#define MAXLEN 500        /* maximal length of a bitlist */
#define MAXPOP 500        /* maximal population size */
#define MAXGEN 500        /* maximal number of generations */
#define MAXCUTS 5         /* maximal number of cuts in a crossing */
#define IMAXDIM 128       /* maximal dimension of arrays for integer state vectors */
#define FMAXDIM 32        /* maximial dimension of arrays for float state vectors */
#define RANKING 0         /* macro for ranking method to select breeding pairs */
#define ROULETTE 1        /* macro for roulette method to select breading pairs */
#define STDMUTRATE 0.01   /* standard mutation rate of 1% */
#define STDALPHA 3.0      /* standard value of alpha parameter in selection probability */
#define STDNUMCUTS 1      /* standard value for the number of cuts at crossing */
#define KEEPFITEST 1      /* copy fitest individual into next population */
#define DONTKEEPFITEST 0  /* don't copy over fitest individual */
#define MONITORON 1       /* terminal monitor on */
#define MONITOROFF 0      /* terminal monitor off */

/******************************************************************************************/
/* structures */
/******************************************************************************************/

struct bitlist
{
  int len;           /* number of bits in bitlist */
  int bits[MAXLEN];  /* the actual bitlist */
  float fitness;     /* fitness of bitlist */
  int terminal;      /* terminal or not */
};


struct population
{
  int size;                   /* size of population */
  float maxfitness;           /* maximal fitness in population */
  float avfitness;            /* average fitness in population */
  int nterm;                  /* number of terminal states in population */
  struct bitlist bl[MAXPOP];  /* the actual population */
};  


/******************************************************************************************/
/* prototypes for bitlist functions */
/******************************************************************************************/

/* generate a random bitlist of lenght len */
struct bitlist randombitlist(int len);

/* generate a random integer in a range from min to max */
int randomint(int min, int max);

/* generate a random float in a range from min to max */
float randomfloat(float min, float max);

/* generate a random choice for integers 0,...,len-1 for a probability distribution p */
int randomchoice(float p[MAXLEN], int len);

/* print an integer array vec with length len to terminal, with final character termchar */
void printlist(int vec[IMAXDIM], int len, char termchar);

/* write an integer array vec with length len to a file with file pointer fp, with final character termchar */
void fprintlist(FILE *fp, int vec[IMAXDIM], int len, char termchar);

/* print a bitlist bl to terminal */
void printbitlist(struct bitlist bl, char termchar);

/* write a bitlist bl to a file with file pointer fp */
void fprintbitlist(FILE *fp, struct bitlist bl, char termchar);

/* read a bitlist bl from a file with file pointer fp */
int fscanbitlist(FILE *fp, struct bitlist *bl);

/* joins bitlists bl1 and bl2 to form another bitlist */
struct bitlist joinbitlists(struct bitlist bl1, struct bitlist bl2);

/* flips bit in position pos for a bitlist bl */
void flipbit(struct bitlist *bl, int pos);

/* copies the bits in bitlist bl from position pos1 to pos2-1 into a new bitlist */
struct bitlist copybitlist(struct bitlist bl, int pos1, int pos2);

/* pastes bitlist blpaste into bitlist bl at position pos, overwriting previous content in bl */
void pastebitlist(struct bitlist *bl, struct bitlist blpaste, int pos);

/* a simple insertion sort of an integer array into ascending order */
void isort(int arr[], int len);

/* crosses bitlists bl1 and bl2, with numcuts number of cuts at positions specified in array cuts */
void crossbitlists(struct bitlist *bl1, struct bitlist *bl2, int numcuts, int cuts[MAXCUTS]);

/* convert an integer num into a bitlist, using numbits bits, after taking off min */
struct bitlist int2bits(int num, int numbits, int min);

/* converts a bitlist bl into an integer plus min */
int bits2int(struct bitlist bl, int min);

/* converts on integer array vev with length len into a bitlist, using numbits bits for every integer, after subtracting min */
struct bitlist intvec2bits(int vec[IMAXDIM], int len, int numbits, int min);

/* converts a bitlist bl into an integer array vec, using numbits bits per integer and adding min */
void bits2intvec(int vec[IMAXDIM], struct bitlist bl, int numbits, int min);

/* converts a float x in range min to max into a bitlist bl with numbits bits */
struct bitlist float2bitlist(float x, float min, float max, int numbits);

/* converts a bitlist bl into a float f in range min to max */
float bitlist2float(struct bitlist bl, float min, float max);

/* converts a float array x, with ranges in arrays min to max into a bitlist bl, with number of bits given in array numbits */
struct bitlist floatvec2bitlist(float x[FMAXDIM], int xlen, float min[FMAXDIM], float max[FMAXDIM], int numbits[FMAXDIM]);

/* converts a bitlist bl into aan array x of floats, with ranges given by arrays min and max */
void bitlist2floatvec(float x[FMAXDIM], struct bitlist bl, float min[FMAXDIM], float max[FMAXDIM], int numbits[FMAXDIM]);

/* the fitness function - to be realised in the envirnment */
void fitness(struct bitlist *bl);

/* the randomstate function - to be realised in the environment */
struct bitlist randomstate();

/* compare two bistlist by their fitness */
int compbitlist(const void *p1, const void *p2);

/* decide if two bistlist are identical */
int bitlistsequal(struct bitlist bl1, struct bitlist bl2);
  
/******************************************************************************************/
/* prototypes for population functions */
/******************************************************************************************/

/* generate a random population of size popsize */
struct population randompop(int popsize);

/* print a population */
void printpop(struct population *pop);

/* write population to a file */
void fprintpop(FILE *fp, struct population *pop);

/* read population from a file */
int fscanpop(FILE *fp, struct population *pop);

/* sort a population by fitness */
void sortpop(struct population *pop);

/* mutate a population */
void mutatepop(struct population *pop, float mutrate);

/* find fitest in a population */
struct bitlist fitestinpop(struct population *pop);

/* find average fitness of a population */
float avfitness(struct population *pop);

/* find number of terminal state in population */
int nterminalpop(struct population *pop);

/* generate the next population from a given one */
void nextpop(struct population pop, struct population *newpop, int meth, int numcuts, int keepfitest,
	     float mutrate, float alpha);


/******************************************************************************************/
/* prototypes for evolution functions */
/******************************************************************************************/

/* genetically evolve a population */
struct population * evolvepop(struct population initialpop, int numgen, int meth, int numcuts,
			      int keepfitest, float mutrate, float alpha, int monitor);

/* monitor evolution of a population */
void monitorevol(int gen, struct population *pop);

/* select terminal states from a population */
struct bitlist * termstates(struct population *evol, int numgen, int *numterm);

/* remove redundqncy in list of bitlists */
void removeredundancy(struct bitlist *bl, int *len);

/* select terminal states from a population and remove redundancy */
struct bitlist * termstatesred(struct population *evol, int numgen, int *numterm);

/* repeated evolution of a random initial population, extracting terminal states */
struct bitlist * searchenv(int numevol, int numgen, int popsize, int *nterm, int *saturate, int meth, int numcuts,
			   int keepfitest, float mutrate, float alpha, int monitor);
