/*********************************************************************/
/* vits:  Threshold image between average of regions on either side  */
/*********************************************************************/

#include "VisXV4.h"          /* VisionX structure include file       */
#include "Vutil.h"           /* VisionX utility header files         */

VXparam_t par[] =            /* command line structure               */
{
{  "if=",   0,   " input file, vtpeak: threshold between hgram peaks"},
{  "of=",   0,   " output file "},
{  "thresh=",    0,   " initial threshold"},
{  "-v",    0,   "(verbose) print threshold information"},
{   0,      0,   0} /* list termination */
};
#define  IVAL   par[0].val
#define  OVAL   par[1].val
#define  TVAL   par[2].val
#define  VFLAG  par[3].val

int hist[256];                 /* histogram bins                 */
int thresh;                    /* threshold                      */

main(argc, argv)
int argc;
char *argv[];
{

    Vfstruct (im);                 /* input image structure          */
    int y,x;                       /* index counters                 */
    int i;
    int rmax=255;                      /* upper limit of region          */
    int rmin=0;                      /* lower limit of region          */
    int avg1=0, avg2=0;                /* average gray levels of region 1 and 2         */
    int old1=0, old2=0;                /* old average gray levels of region 1 and 2     */
			     
    VXparse(&argc, &argv, par);     /* parse the command line         */
   while( Vfread( &im, IVAL)){

    if ( im.type != VX_PBYTE ) {
    	fprintf (stderr, "error: image not byte type\n");
        exit (1);
    }

    /* clear the histogram */
    for (i = 0; i < 256; i++) hist[i] = 0;
 
    /* compute the histogram */
    for (y = im.ylo; y <= im.yhi; y++)
        for (x = im.xlo; x <= im.xhi; x++)
       		hist[im.u[y][x]]++;

    if (TVAL) thresh = atoi(TVAL);  /* if thresh= was specified, get value */
    else thresh = getAvg(rmin,rmax);    /* set default threshold to average gray level*/
    if (thresh < 0 || thresh > 255) {
	fprintf(stderr, "thresh= must be between 0 and 255\nUsing thresh=%d\n",thresh);
    }
    
    int converged= 0; /*false*/
    i=1000; /*loop counter*/
    while((!converged)&&(i>0)){
   	/* find average of region 1 */
    	rmin=0;
    	rmax=thresh-1;
    	avg1 = getAvg(rmin,rmax); 

	/* find average of region 2 */
    	rmin=thresh+1; /* Should this be thresh or thresh+1? */
    	rmax=255;
    	avg2 = getAvg(rmin,rmax); 

        if((avg1==old1)&&(avg2==old2)){
		converged=1;/*true*/
	}
	else{
		old1=avg1;
		old2=avg2;
		thresh=(avg1+avg2)/2;
		i--;
	}
	//fprintf(stderr, "avg1 = %d avg2 = %d thresh = %d\n", avg1, avg2, thresh);
    }
  
    if(VFLAG)
       fprintf(stderr, "avg1 = %d avg2 = %d thresh = %d\n", avg1, avg2, thresh);
    /* apply the threshold */
    for (y = im.ylo; y <= im.yhi; y++) {
       for (x = im.xlo; x <= im.xhi; x++) {
       		if (im.u[y][x] >= thresh) im.u[y][x] = 255;
                else                      im.u[y][x] = 0;
       }
    }
  
    Vfwrite( &im, OVAL);
}/*for each frame*/
    exit(0);
}


int getAvg(int min,int max){

    float sum=0;
    float count=0;
    int i=0;
    for (i=min;i<=max;i++){
	sum+=i*hist[i];
	count+=hist[i];
    }
    if (count!=0) return (int)(sum/count);
    else return 0;

}

