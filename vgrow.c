/*******************************************************************/
/* vgrow                     Region growing using a range          */
/*******************************************************************/

#include "VisXV4.h"           /* VisionX structure include file    */
#include "Vutil.h"            /* VisionX utility header files      */

VXparam_t par[] =             /* command line structure            */
{ /* prefix, value,   description                         */   
{    "if=",    0,   " input file  vtemp: local max filter "},
{    "of=",    0,   " output file "},
{    "r=",    0,   " range "},
{    "-p",    0,   " label type "},
{     0,       0,   0}  /* list termination */
};
#define  IVAL   par[0].val
#define  OVAL   par[1].val
#define  RVAL   par[2].val
#define  PVAL   par[3].val
void setlabel(int, int, int);
Vfstruct (im);                      /* i/o image structure          */
Vfstruct (tm);                      /* temp image structure         */
int range;			    /* range                        */
int first; 			    /* first pixel in range         */
int min;                            /* minimum pixel value in range       */
int max;			    /* minimum pixel value in range       */

main(argc, argv)
int argc;
char *argv[];
{
  int        y, x, n=0;               /* index counters               */
  VXparse(&argc, &argv, par);         /* parse the command line       */
  Vfread(&im, IVAL);                  /* read image file              */
  Vfembed(&tm, &im, 1,1,1,1);         /* image structure with border  */
  if ( im.type != VX_PBYTE ) {        /* check image format           */
     	fprintf(stderr, "vtemp: no byte image data in input file\n");
     	exit(-1);
  }

  if (RVAL)range=atoi(RVAL);
  if((range<0)||(range>255)){
  	fprintf(stderr, "invalid range! using range 0\n");
  	range=0;
  }
  for (y = im.ylo ; y <= im.yhi ; y++) {
     	for (x = im.xlo; x <= im.xhi; x++)  {
           	im.u[y][x]=0;
     	}
  }

  for (y = tm.ylo ; y <= tm.yhi ; y++) {
     	for (x = tm.xlo; x <= tm.xhi; x++)  {
           	if((tm.u[y][x]!=0)&&(im.u[y][x]==0)){
			first=tm.u[y][x];
			min=tm.u[y][x];
			if (n==255) n=1;
			else n++;
			if (PVAL) setlabel(x, y, first);
			else setlabel(x, y, n);
	  	 }
    	}
   }

   Vfwrite(&im, OVAL);             /* write image file                */
   exit(0);
}

/* function to set label */
void setlabel(int x, int y, int L) 
{
    im.u[y][x]=L;
	//check range based on first pixel
	if((tm.u[y+1][x]!=0)&&(abs(tm.u[y+1][x]-first)<=range)&&(im.u[y+1][x]==0)){
		setlabel(x, y+1, L);
	   }
	if((tm.u[y-1][x]!=0)&&(abs(tm.u[y-1][x]-first)<=range)&&(im.u[y-1][x]==0)){
		setlabel(x, y-1, L);
	   }
	if((tm.u[y][x+1]!=0)&&(abs(tm.u[y][x+1]-first)<=range)&&(im.u[y][x+1]==0)){
		setlabel(x+1, y, L);
	   }
	if((tm.u[y][x-1]!=0)&&(abs(tm.u[y][x-1]-first)<=range)&&(im.u[y][x-1]==0)){
		setlabel(x-1, y, L);
	   }

	//Check range based on min-max

	/*if((tm.u[y+1][x]!=0)&&(abs(tm.u[y+1][x]-min)<=range)&&(abs(tm.u[y+1][x]-max)<=range)&&(im.u[y+1][x]==0)){
		if(tm.u[y+1][x]<min)min=tm.u[y+1][x];
		if(tm.u[y+1][x]>max)max=tm.u[y+1][x];
		setlabel(x, y+1, L);
	   }
	if((tm.u[y-1][x]!=0)&&(abs(tm.u[y-1][x]-min)<=range)&&(abs(tm.u[y-1][x]-max)<=range)&&(im.u[y-1][x]==0)){
		if(tm.u[y-1][x]<min)min=tm.u[y-1][x];
		if(tm.u[y-1][x]>max)max=tm.u[y-1][x];
		setlabel(x, y-1, L);
	   }
	if((tm.u[y][x+1]!=0)&&(abs(tm.u[y][x+1]-min)<=range)&&(abs(tm.u[y][x+1]-max)<=range)&&(im.u[y][x+1]==0)){
		if(tm.u[y][x+1]<min)min=tm.u[y][x+1];
		if(tm.u[y][x+1]>max)max=tm.u[y][x+1];
		setlabel(x+1, y, L);
	   }
	if((tm.u[y][x-1]!=0)&&(abs(tm.u[y][x-1]-min)<=range)&&(abs(tm.u[y][x-1]-max)<=range)&&(im.u[y][x-1]==0)){
		if(tm.u[y][x-1]<min)min=tm.u[y][x-1];
		if(tm.u[y][x-1]>max)max=tm.u[y][x-1];
		setlabel(x-1, y, L);
	   }*/			
     return;
}
