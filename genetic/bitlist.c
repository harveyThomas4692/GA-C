/******************************************************************************************/
/* bitlist functions */
/******************************************************************************************/
/* generate a random bitlist of lenght len */
 
struct bitlist randombitlist(int len)
{

  int len1, i;
  struct bitlist bl;

  len1=((len > MAXLEN) ? MAXLEN : len);
  srand(clock());srand(clock());
  for(i=0; i<len1; i++) bl.bits[i]=randomint(0,1);
  bl.len=len1; bl.fitness=0.; bl.terminal=0;
  
  return bl;
}

/******************************************************************************************/
/* generate a random integer in a range from min to max */

int randomint(int min, int max)
{

  return round((1.* rand())/RAND_MAX*(max-min))+min;

}  


/******************************************************************************************/
/* generate a random float in a range from min to max */

float randomfloat(float min, float max)
{

  return (1.* rand())/RAND_MAX*(max-min)+min;

}


/******************************************************************************************/
/* generate a random choice for integers 0,...,len-1 for a probability distribution p */

int randomchoice(float p[MAXPOP], int len)
{

  float psum, P, fran;
  int i;
  /* determine normalisation */
  psum=0;
  for (i=0; i<len;i++ ) psum=psum+p[i];

  /* generate random float in range [0,1] */
  fran=(1.*rand())/RAND_MAX;
  P=p[0]/psum; i=1;
  while (P<fran && i<len) {P=P+p[i]/psum; i++;}

  return i-1;
}  


/******************************************************************************************/ 
/* print an integer array vec with length len to terminal, with final character termchar */
 
void printlist(int vec[IMAXDIM], int len, char termchar)
{

  int i;

  if (len>IMAXDIM) len=IMAXDIM;
  if (len<=0) printf("{}");
  else {
    printf("{");
    for (i=0; i<(len-1); i++) printf("%i,",vec[i]);
    printf("%1i}",vec[len-1]);
  }
  printf("%c",termchar);
}


/******************************************************************************************/ 
/* write an integer array vec with length len to a file with file pointer fp, with final character termchar */

void fprintlist(FILE *fp, int vec[IMAXDIM], int len, char termchar)
{

  int i;

  if (len>IMAXDIM) len=IMAXDIM;
  if (len<=0) fprintf(fp,"{}");
  else {
    fprintf(fp,"{");
    for (i=0; i<(len-1); i++) fprintf(fp,"%i,",vec[i]);
    fprintf(fp,"%1i}",vec[len-1]);
  }
  fprintf(fp,"%c",termchar);
}


/******************************************************************************************/ 
/* print a bitlist bl to terminal */

void printbitlist(struct bitlist bl, char termchar)
{
  printf("<|\"Len\"->%i,\"Bits\"->",bl.len);
  printlist(bl.bits,bl.len,',');
  printf("\"Fitness\"->%f",bl.fitness);
  printf(",\"Terminal\"->%i|>%c",bl.terminal,termchar);
}  


/******************************************************************************************/ 
/* write a bitlist bl to a file with file pointer fp */

void fprintbitlist(FILE *fp, struct bitlist bl, char termchar)
{
  fprintf(fp,"<|\"Len\"->%i,\"Bits\"->",bl.len);
  fprintlist(fp,bl.bits,bl.len,',');
  fprintf(fp,"\"Fitness\"->%f",bl.fitness);
  fprintf(fp,",\"Terminal\"->%i|>%c",bl.terminal,termchar);
}


/******************************************************************************************/ 
/* read a bitlist bl from a file with file pointer fp */

int fscanbitlist(FILE *fp, struct bitlist *bl)
{
  
  int i, eof;
  char c;
     
  eof=fscanf(fp,"<|\"Len\"->%i,\"Bits\"->{",&(bl->len));
  if (eof != EOF) {
    if (bl->len>0) {
      for (i=0; i<bl->len-1; i++) fscanf(fp,"%i,",&(bl->bits[i]));
      fscanf(fp,"%i},",&(bl->bits[bl->len-1]));
    }
    fscanf(fp,"\"Fitness\"->%f,",&(bl->fitness));
    fscanf(fp,"\"Terminal\"->%i|>%c\n",&(bl->terminal),&c);
  }
  else {bl->len=0; bl->fitness=0.; bl->terminal=0;}

  return eof;
}  


/******************************************************************************************/
/* joins bitlists bl1 and bl2 to form another bitlist */

struct bitlist joinbitlists(struct bitlist bl1, struct bitlist bl2)
{
  struct bitlist bl;
  int i, len2;

  bl=bl1;
  len2=((bl1.len+bl2.len) > MAXLEN ? MAXLEN-bl1.len : bl2.len);
  for (i=0; i<len2; i++) bl.bits[bl1.len+i]=bl2.bits[i];
  bl.len=bl1.len+len2;
  
  return bl;
}  

 
/******************************************************************************************/
/* flips bit in position pos for a bitlist bl */

void flipbit(struct bitlist *bl, int pos)
{
  if ((pos>=0) && (pos<(bl->len))) (bl->bits)[pos]=((bl->bits)[pos]+1) % 2;
}  


/******************************************************************************************/
/* copies the bits in bitlist bl from position pos1 to pos2-1 into a new bitlist */

struct bitlist copybitlist(struct bitlist bl, int pos1, int pos2)
{

  struct bitlist blcopy;
  int i, p1, p2;

  /* make sure positions are viable */
  if (pos1<=pos2) {p1=pos1; p2=pos2;}
  else {p1=pos2; p2=pos1;}
  if (p1<0) p1=0;
  if (p2<0) p2=0;
  if (p1>bl.len) p1=bl.len;
  if (p2>bl.len) p2=bl.len;

  /* copy section of bit list */
  if (p1<p2) {
    for (i=0; i<(p2-p1); i++) blcopy.bits[i]=bl.bits[p1+i];
    blcopy.len=p2-p1;
  }
  else blcopy.len=0;
  blcopy.fitness=0.; blcopy.terminal=0;

  return blcopy;
}  


/******************************************************************************************/
/* pastes bitlist blpaste into bitlist bl at position pos, overwriting previous content in bl */

void pastebitlist(struct bitlist *bl, struct bitlist blpaste, int pos)
{

  int len, i;

  /* make sure position and length are ok */
  if (pos<0) pos=0;
  if (pos>=bl->len) pos=bl->len-1;
  len=((pos+blpaste.len<=bl->len) ? blpaste.len : bl->len-pos);

  /*copy b1paste into bl */
  if (len>0) {
    for (i=0; i<len; i++) (bl->bits)[pos+i]=blpaste.bits[i];
  }
}

									  
/******************************************************************************************/
/* a simple insertion sort of an integer array into ascending order */

void isort(int arr[], int len)
{
    int i, key, j;
    for (i = 1; i < len; i++)
    {
        key = arr[i];
        j = i - 1;

        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}
									  

/******************************************************************************************/
/* crosses bitlists bl1 and bl2, with numcuts number of cuts at positions specified in array cuts */

void crossbitlists(struct bitlist *bl1, struct bitlist *bl2, int numcuts, int cuts[MAXCUTS])
{

  int i, minlen, start, end, cuts1[MAXCUTS+2];
  struct bitlist blpart1, blpart2;

  /* set lengths of bitlists to common minimum */
  minlen=((bl1->len < bl2->len) ? bl1->len : bl2->len);
  bl1->len=minlen; bl2->len=minlen;

  /* check range of cut positions */
  cuts1[0]=0; cuts1[numcuts+1]=minlen;
  for (i=0; i<numcuts; i++) {
    cuts1[i+1]=cuts[i];
    if (cuts[i]<0) cuts1[i+1]=0;
    if (cuts[i]>=minlen) cuts1[i+1]=minlen-1;
  }  
  
  /* sort cut positions in ascending order */
  isort(cuts1,numcuts+2);
 
  /* swap the relevant parts of the two bitlists */
  for (i=0; i<=numcuts; i=i+2) {
    start=cuts1[i]; end=cuts1[i+1];
    blpart1=copybitlist(*bl1,start,end);
    blpart2=copybitlist(*bl2,start,end);
    pastebitlist(bl1,blpart2,start);
    pastebitlist(bl2,blpart1,start);
  }  
}  

 
/******************************************************************************************/
/* convert an integer num into a bitlist, using numbits bits, after taking off min */

struct bitlist int2bits(int num, int numbits, int min)
{

  int num1, pmax, p, bit, i;
  struct bitlist bl;

  /* bring integer into correct range */
  pmax=pow(2,numbits);
  num1=(num-min) % pmax;

  /* convert number into bits */
  for (i=0; i<numbits; i++) {
    p=pow(2,numbits-i-1);
    bit=num1/p;
    bl.bits[i]=bit;
    num1=num1-bit*p;
  }
  bl.len=numbits; bl.fitness=0.; bl.terminal=0;

  return bl;
}  


/******************************************************************************************/
/* converts a bitlist bl into an integer plus min */

int bits2int(struct bitlist bl, int min)
{
  int i, num;

  num=0;
  for (i=0; i<bl.len; i++)
    if (bl.len-i <=15) num=num+bl.bits[i]*pow(2,bl.len-i-1);

  return num+min;
}  


/******************************************************************************************/
/* converts on integer array vev with length len into a bitlist, using numbits bits for every integer, after subtracting min */

struct bitlist intvec2bits(int vec[IMAXDIM], int len, int numbits, int min)
{

  int len1, i;
  struct bitlist blint, bl;

  /* truncate array if it overruns bitlist */
  if (len*numbits <= MAXLEN) len1=len;
  else len1=MAXLEN/numbits;

  bl.len=numbits*len1; bl.fitness=0.; bl.terminal=0;
  for (i=0; i<len1; i++) {
    blint=int2bits(vec[i],numbits,min);
    pastebitlist(&bl,blint,i*numbits);
  }

  return bl;
}  


/******************************************************************************************/
/* converts a bitlist bl into an integer array vec, using numbits bits per integer and adding min */

void bits2intvec(int vec[IMAXDIM], struct bitlist bl, int numbits, int min)
{

  int i, len;
  struct bitlist blpart;

  len=bl.len/numbits;
  for (i=0; i<len; i++) {
    blpart=copybitlist(bl,i*numbits,(i+1)*numbits);
    vec[i]=bits2int(blpart,min);
  }
}  


/******************************************************************************************/
/* converts a float x in range min to max into a bitlist bl with numbits bits */

struct bitlist float2bitlist(float x, float min, float max, int numbits)
{
  
  int num, npts;
  float dx;

  /* use defaults if values are unreasonable */
  if (max<=min) max=min+1; if (numbits<1) numbits=8;
  if (x<min) x=min; if (x>max) x=max;

  /* compute integer num which marks discrete point in interval [min.max] */
  npts=pow(2,numbits); dx=(max-min)/(npts-1.);
  num=round((x-min)/dx);

  /* convert this integer into bitlist */
  return int2bits(num,numbits,0);
}  


/******************************************************************************************/
/* converts a bitlist bl into a float in range min to max */

float bitlist2float(struct bitlist bl, float min, float max)
{  

  int num, npts;
  float dx;
  
  /* use defaults if values are unreasonable */
  if (max<=min) max=min+1;

  num=bits2int(bl,0);
  npts=pow(2,bl.len); dx=(max-min)/(npts-1.);

  return min+num*dx;
}  


/******************************************************************************************/
/* converts a float array x, with ranges in arrays min to max into a bitlist bl, with number of bits given in array numbits */

struct bitlist floatvec2bitlist(float x[FMAXDIM], int xlen, float min[FMAXDIM], float max[FMAXDIM], int numbits[FMAXDIM])
{

  struct bitlist bl, blfloat;
  int i, pos;

  pos=0;
  for (i=0; i<xlen; i++) {
    bl.len=pos+numbits[i];
    blfloat=float2bitlist(x[i],min[i],max[i],numbits[i]);
    pastebitlist(&bl,blfloat,pos);
    pos=pos+numbits[i];
  }
  bl.fitness=0.; bl.terminal=0;

  return bl;

} 
  

/******************************************************************************************/
/* converts a bitlist bl into aan array x of floats, with ranges given by arrays min and max */

void bitlist2floatvec(float x[FMAXDIM], struct bitlist bl, float min[FMAXDIM], float max[FMAXDIM], int numbits[FMAXDIM])
{

  int i, pos;
  struct bitlist blfloat;

  i=0; pos=0; 
  while (pos<bl.len) {
    blfloat=copybitlist(bl,pos,pos+numbits[i]);
    x[i]=bitlist2float(blfloat,min[i],max[i]);
    pos=pos+numbits[i]; i++;
  }
}  


/******************************************************************************************/
/* compare two bistlist by their fitness */

int compbitlist(const void *p1, const void *p2)
{

  const struct bitlist *bl1=p1, *bl2=p2;

  if (bl1->fitness < bl2->fitness) return 1;
  else if  (bl1->fitness > bl2->fitness) return -1;
  else return 0;

}  


/******************************************************************************************/
/* decide if two bistlist are identical */

int bitlistsequal(struct bitlist bl1, struct bitlist bl2)
{

  int i, equal;
  
  if (bl1.len != bl2.len) return 0;
  else {
    equal=1; i=0;
    while (equal && i<bl1.len) {
      equal = equal && (bl1.bits[i]==bl2.bits[i]);
      i++;
    }

    return equal;
  }
    
}
