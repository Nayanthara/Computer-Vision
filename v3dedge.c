/****************************************************************/
/* v3dedge    Compute function on a 3D image structure          */
/****************************************************************/

#include "VisXV4.h"          /* VisionX structure include file       */
#include "Vutil.h"           /* VisionX utility header files         */

VXparam_t par[] =            /* command line structure               */
{
{    "if=",    0,   " input file  v3dedge: detect edge"},
{    "of=",    0,   " output file "},
{     0,       0,    0}
};
#define  IVAL   par[0].val
#define  OVAL   par[1].val

int
main(argc, argv)
int argc;
char *argv[];
{
V3fstruct (im);
V3fstruct (tm);
int        x,y,z;               /* index counters                 */
int        xx,yy,zz;            /* window index counters          */
int        sx,sy,sz;
    VXparse(&argc, &argv, par); /* parse the command line         */

    V3fread( &im, IVAL);        /* read 3D image                  */
    if ( im.type != VX_PBYTE || im.chan != 1) { /* check  format  */
       fprintf (stderr, "image not byte type or single channel\n");
       exit (1);
    }   
   
    V3fembed(&tm, &im, 1,1,1,1,1,1); /* temp image copy with border */

    int sobelx[3][3][3]=   
        {
            {
            {-1, 0, 1},
            {-2, 0, 2},
            {-1, 0, 1}
            },
            {
            {-2, 0, 2},
            {-4, 0, 4},
            {-2, 0, 2}
            },
            {
            {-1, 0, 1},
            {-2, 0, 2},
            {-1, 0, 1}
            },
        };
    int sobely[3][3][3]=   
        {
            {
            {1, 2, 1},
            {0, 0, 0},
            {-1, -2, -1}
            },
            {
            {2, 4, 2},
            {0, 0, 0},
            {-2, -4, -2}
            },
            {
            {1, 2, 1},
            {0, 0, 0},
            {-1, -2, -1}
            },
        };
    int sobelz[3][3][3]=   
        {
            {
            {1, 2, 1},
            {2, 4, 2},
            {1, 2, 1}
            },
            {
            {0, 0, 0},
            {0, 0, 0},
            {0, 0, 0}
            },
            {
            {-1, -2, -1},
            {-2, -4, -2},
            {-1, -2, -1}
            },
        };

    for (z = im.zlo; z <= im.zhi; z++) {/* for all pixels */
      for (y = im.ylo; y <= im.yhi; y++) {
        for (x = im.xlo; x <= im.xhi; x++) {
             sx= 0;
             sy= 0;
             sz= 0;
             for (zz = -1; zz <= 1; zz++) {/* compute the function */
               for (yy = -1; yy <= 1; yy++) {
                 for (xx = -1; xx <= 1; xx++) {
                     sx = sx + tm.u[z + zz][y + yy][x + xx]*sobelx[1 + xx][1 + yy][1 + zz];
                     sy = sy + tm.u[z + zz][y + yy][x + xx]*sobely[1 + xx][1 + yy][1 + zz];
                     sz = sz + tm.u[z + zz][y + yy][x + xx]*sobelz[1 + xx][1 + yy][1 + zz];
                 }   
               }   
             }   
             im.u[z][y][x] = (int)(sqrt(sx*sx+sy*sy+sz*sz)/3.47);
        }
      }
   }
   V3fwrite (&im, OVAL);
   exit(0);
}
