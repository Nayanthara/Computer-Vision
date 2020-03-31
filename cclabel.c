/*******************************************************************/
/* cclabel                    Label objects in an image            */
/*******************************************************************/

#include "VisXV4.h"           /* VisionX structure include file    */
#include "Vutil.h"            /* VisionX utility header files      */

VXparam_t par[] =             /* command line structure            */
{ /* prefix, value,   description                         */   
{    "if=",    0,   " input file  vtemp: local max filter "},
{    "of=",    0,   " output file "},
{     0,       0,   0}  /* list termination */
};
#define  IVAL   par[0].val
#define  OVAL   par[1].val
void setlabel(int, int, int);
Vfstruct (im);                      /* i/o image structure          */
Vfstruct (tm);                      /* temp image structure         */

main(argc, argv)
int argc;
char *argv[];
{
int        y, x, n=1;               /* index counters               */
 VXparse(&argc, &argv, par);        /* parse the command line       */
Vfread(&im, IVAL);                  /* read image file              */
Vfembed(&tm, &im, 1,1,1,1);         /* image structure with border  */
  if ( im.type != VX_PBYTE ) {      /* check image format           */
     fprintf(stderr, "vtemp: no byte image data in input file\n");
     exit(-1);
  }
  for (y = im.ylo ; y <= im.yhi ; y++) {
     for (x = im.xlo; x <= im.xhi; x++)  {
           im.u[y][x]=0;
     }
   }

  for (y = tm.ylo ; y <= tm.yhi ; y++) {
     for (x = tm.xlo; x <= tm.xhi; x++)  {
           if((tm.u[y][x]!=0)&&(im.u[y][x]==0)){
		setlabel(x, y, n);
		n++;
	   }
     }
   }

   Vfwrite(&im, OVAL);             /* write image file                */
   exit(0);
}

/* function to compute the local maximum */
void setlabel(int x, int y, int L) 
{
    im.u[y][x]=L;
    if(y+1<=im.yhi)  {
           if((tm.u[y+1][x]!=0)&&(im.u[y+1][x]==0)){
		setlabel(x, y+1, L);
	   }
     }	

    if(y-1>=im.ylo)  {
           if((tm.u[y-1][x]!=0)&&(im.u[y-1][x]==0)){
		setlabel(x, y-1, L);
	   }
     }
	
     if(x+1<=im.xhi)  {
           if((tm.u[y][x+1]!=0)&&(im.u[y][x+1]==0)){
		setlabel(x+1, y, L);
	   }
     }	
    if(x-1>=im.xlo)  {
           if((tm.u[y][x-1]!=0)&&(im.u[y][x-1]==0)){
		setlabel(x-1, y, L);
	   }
     }				
     return;
}
