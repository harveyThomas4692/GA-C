/******************************************************************************************/
/* population functions */
/******************************************************************************************/
/* generate a random population of size popsize */

struct population randompop(int popsize)
{

  int size, i;
  struct population pop;
  struct bitlist blfitest;

  size=((popsize>MAXPOP) ? MAXPOP : popsize);
  for (i=0; i<size; i++) pop.bl[i]=randomstate();
  pop.size=size;

  /* compute average and maximal fitness for random population */
  pop.avfitness=avfitness(&pop);    /* average fitness in population */
  blfitest=fitestinpop(&pop);
  pop.maxfitness=blfitest.fitness;  /* maximal fitness in population */
  pop.nterm=nterminalpop(&pop);     /* number of terminal state in mutated population */

  return pop;
  
}  


/******************************************************************************************/
/* print a population */

void printpop(struct population *pop)
{

  int i;

  printf("<|\"Size\"->%i,\"MaxFitness\"->%f,\"AvFitness\"->%f,\"NTerm\"->%i,\"Pop\"->{\n",
	 pop->size,pop->maxfitness,pop->avfitness,pop->nterm);
  for (i=0; i<pop->size-1; i++) printbitlist((pop->bl)[i],',');
  printbitlist((pop->bl)[pop->size-1],'}');
  printf("|>\n");

}  


/******************************************************************************************/
/* write population to a file */

void fprintpop(FILE *fp, struct population *pop)
{

  int i;

  fprintf(fp,"<|\"Size\"->%i,\"MaxFitness\"->%f,\"AvFitness\"->%f,\"NTerm\"->%i,\"Pop\"->{\n",
	  pop->size,pop->maxfitness,pop->avfitness,pop->nterm);
  for (i=0; i<pop->size-1;i++) fprintbitlist(fp,(pop->bl)[i],',');
  fprintbitlist(fp,(pop->bl)[pop->size-1],'}');
  fprintf(fp,"|>\n");

} 

  
/******************************************************************************************/
/* read population from a file */


int fscanpop(FILE *fp, struct population *pop)
{

  int i, eof;

  eof=fscanf(fp,"<|\"Size\"->%i,\"MaxFitness\"->%f,\"AvFitness\"->%f,\"NTerm\"->%i,\"Pop\"->{\n",
        	 &(pop->size),&(pop->maxfitness),&(pop->avfitness),&(pop->nterm));
  
  i=0;
  for (i=0; i<pop->size; i++) fscanbitlist(fp,&((pop->bl)[i]));
  fscanf(fp,"|>\n");

  return eof;
    
}  


/******************************************************************************************/
/* sort a population by fitness */

void sortpop(struct population *pop)
{

  qsort(pop->bl,pop->size,sizeof(struct bitlist),compbitlist);

}


/******************************************************************************************/
/* mutate a population */

void mutatepop(struct population *pop, float mutrate)
{

  int len, nbits, nmut, i, k, pos, ipos, bpos;
  struct bitlist blfitest;

  len=((pop->bl)[0]).len;     /* length of bitlist, assume they are the same across population */
  nbits=len*(pop->size);      /* total number of bits in population */
  nmut=round(nbits*mutrate);  /* number of bits to be mutated */

  /* run over mutations */
  for (i=0; i<nmut; i++) {
    pos=randomint(0,nbits-1);     /* random bit position in entire population */
    ipos=pos/len;             /* individual where mutation occurs */
    bpos=pos%len;             /* bit position in individual */
    flipbit(&((pop->bl)[ipos]),bpos);
  }


  /* update fitness for mutated population */
   for (k=0; k<pop->size; k++) fitness(&((pop->bl)[k]));
   pop->avfitness=avfitness(pop);      /* average fitness in mutated population */
   blfitest=fitestinpop(pop);
   pop->maxfitness=blfitest.fitness;   /* maximal fitness in mutated population */
   pop->nterm=nterminalpop(pop);       /* number of terminal state in mutated population */
  
}  


/******************************************************************************************/
/* find fitest in a population */

struct bitlist fitestinpop(struct population *pop)
{

  float fmax;
  int pos, i;

  pos=0;
  if (pop->size>0) {
    fmax=((pop->bl)[0]).fitness; 
    for (i=1; i<pop->size; i++)
      if (((pop->bl)[i]).fitness>fmax) {pos=i; fmax=((pop->bl)[i]).fitness;}
  }

  return (pop->bl)[pos];
  
}  
      

/******************************************************************************************/
/* find average fitness of a population */

float avfitness(struct population *pop)
{

  float fsum=0.;
  int i;

  for (i=0; i<pop->size; i++) fsum=fsum+((pop->bl)[i]).fitness;

  return fsum/pop->size;

}  
  

/******************************************************************************************/
/* find number of terminal state in population */

int nterminalpop(struct population *pop)
{

  int i, nterm;

  nterm=0;
  for (i=0; i<pop->size; i++)
    if (((pop->bl)[i]).terminal) nterm++;

  return nterm;

}  


/******************************************************************************************/
/* generate the next population from a given one by selection, crossing and mutation */

void nextpop(struct population pop, struct population *newpop, int meth, int numcuts, int keepfitest,
	     float mutrate, float alpha)
{

  struct bitlist blfitest, bl1, bl2;
  float fmax, fav, df, p[MAXPOP];
  int n, len, k, i, i1, i2, j, cutpos[MAXCUTS];

   n=pop.size;             /* size of population */
   len=((pop.bl)[0]).len;  /* length of bitlist - assume the same across population */
   if (n>1) {

     /* find fitest in population */
     blfitest=fitestinpop(&pop);
  
    /* fix parameters if out of range */
    if (numcuts<1 || numcuts>MAXCUTS) numcuts=1;
    if (alpha<=1.) alpha=2.;
 

    /* compute selection probabilities p */
    /* ranking method */
    if (meth==RANKING) {
      sortpop(&pop);
      for (k=0; k<n; k++) p[k]=2*(1+(n-k-1)/(n-1)*(alpha-1))/(1+alpha)/n;
    }  
    else {
      /* roulette method */
      fmax=blfitest.fitness;
      fav=avfitness(&pop);
      df=fmax-fav;
      if (df<=0) for (k=0; k<n; k++) p[k]=1./n;
      else for (k=0; k<n; k++) p[k]=((alpha-1)*(((pop.bl)[k]).fitness-fav)+df)/df/n;
    }

    /* select and cross pairs of individuals */
    i=0;
    while (i<n) {
      i1=randomchoice(p,n); i2=randomchoice(p,n);             /* positions of two individuals selected */
      bl1=pop.bl[i1]; bl2=pop.bl[i2];                         /* copy two selected individuals */
      for (j=0; j<numcuts; j++) cutpos[j]=randomint(0,len-1); /* determine random cut positions */
      crossbitlists(&bl1,&bl2,numcuts,cutpos);                /* cross the two individuals */
      (newpop->bl)[i]=bl1; (newpop->bl)[i+1]=bl2;             /* copy the crossed individuals into new population */
      i=i+2;
    }
    newpop->size=n;

    /* mutate new population */
    mutatepop(newpop,mutrate);

    /* if fitest individual is kept copy to new population */
    if (keepfitest) (newpop->bl)[0]=blfitest;

 }  

} 
