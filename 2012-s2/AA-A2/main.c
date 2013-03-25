/***************************************************************************
 *
 *   File        : main.c
 *   Student Id  : s3295715
 *   Name        : Chao Ruan
 *
 ***************************************************************************/
#include "libutil.h"
#include "libset.h"
#include "sl_list.h"
#include "libintersect.h"

#include <sys/time.h>
#include <math.h>

#define NANOSECONDS 1000000
#define GETOPT_FINISHED -1
#define INIT_SET_SIZE 128

extern hrtime_t gethrtime();

enum mode_t {
   LIN,
   BIN,
   INP,
   EXP,
   BLK
};

enum imode_t {
   SVS,
   SEQ,
   ADP
};

typedef struct {
   list_t *data_files;
   int runs;
   int verbose;
   enum mode_t mode;
   enum imode_t imode;
} clo_t;

static void
print_usage(const char *program)
{
   fprintf(stderr, "USAGE: %s [options] test1.dat test2.dat ...\n", program);
   fprintf(stderr, "  -m search method [linear, binary, interpolation, ");
   fprintf(stderr, "exponential]\n");
   fprintf(stderr, "  -i intersection method [svs, sequential, adaptive]\n");
   fprintf(stderr, "  -r runs\n");
   fprintf(stderr, "  -v verbose\n");
   fprintf(stderr, "  -h Display usage information\n");
   fprintf(stderr, "\n");
   fprintf(stderr, "EXAMPLE: %s -m linear -i svs -r 5 test1.dat test2.dat\n",
		    program);
   fprintf(stderr, "\n");
   return;
}

/** Initialize resources for the command line options **/
clo_t*
init_clo(void)
{
   clo_t* rv = NULL;
   rv = (clo_t *) safe_malloc(sizeof(clo_t));

   rv->data_files = NULL;	/* list_create(); */
   rv->runs = 1;
   rv->verbose = FALSE;
   rv->mode = LIN;
   rv->imode = SVS;

   return (rv);
}

/** Free all resources allocated for the command line options **/
void
destroy_clo(clo_t * noret)
{
   if (!noret) {
      fprintf(stderr, "WARNING: command line options does not exist!\n");
      return;
   }
   if (noret->data_files) {
      list_free(noret->data_files);
   }

   free(noret);
}

clo_t *
parse_args(int argc, char **argv)
{
   int opt = GETOPT_FINISHED;
   clo_t *iopts = init_clo();

   if (argc <= 1) {
      print_usage(argv[0]);
      exit(EXIT_FAILURE);
   }

   while ((opt = getopt(argc, argv, "i:m:r:vh")) != GETOPT_FINISHED) {
      switch (opt) {
         case 'i':
            if (strcmp(optarg, "svs") == 0) {
               iopts->imode = SVS;
            } else if (strcmp(optarg, "sequential") == 0) {
                      iopts->imode = SEQ;
                   }else if (strcmp(optarg, "adaptive") == 0) {
		         iopts->imode = ADP;
                         }else {
                              fprintf(stderr, "ERROR: imode <%s> unknown!\n", optarg);
                              exit(EXIT_FAILURE);
                           }
         break;
         case 'm':
            if (strcmp(optarg, "linear") == 0) {
               iopts->mode = LIN;
            } else if (strcmp(optarg, "binary") == 0) {
                      iopts->mode = BIN;
                 } else if (strcmp(optarg, "interpolation") == 0) {
                           iopts->mode = INP;
                       } else if (strcmp(optarg, "exponential") == 0) {
                                 iopts->mode = EXP;
                              } else if (strcmp(optarg, "block") == 0) {
                                        iopts->mode = BLK;
			             } else {
				             fprintf(stderr, "ERROR: mode <%s> unknown!\n", optarg);
                                             exit(EXIT_FAILURE);
			}
         break;
         case 'r':
            iopts->runs = atoi(optarg);
         break;
         case 'v':
            iopts->verbose = TRUE;
         break;
         case 'h':
         default:
            print_usage(argv[0]);
            exit(EXIT_FAILURE);
	}
   }

	/* TODO READ NAMES OF DATA SETS */

   return (iopts);
}

int
main(int argc, char **argv)
{
   int i;
   float average, stddev;
   hrtime_t *runtimes;
   hrtime_t start;
   hrtime_t end;

   clo_t   *iopts = parse_args(argc, argv);

   /* select interstion algorithm */
   switch (iopts->imode) {
      case SVS:
         fprintf(stdout, "ALGORITHM : SvS\n");
         break;
      case SEQ:
         fprintf(stdout, "ALGORITHM : Sequential\n");
         break;
      case ADP:
      fprintf(stdout, "ALGORITHM : Adaptive\n");
         break;
      default:
         fprintf(stdout, "ALGORITHM : Unknown\n");
   return (EXIT_FAILURE);
   }

   /* select fsearch algorithm */
   switch (iopts->mode) {
      case LIN:
         fprintf(stdout, "F-SEARCH : Linear\n");
         break;
      case BIN:
         fprintf(stdout, "F-SEARCH : Binary\n");
         break;
      case INP:
         fprintf(stdout, "F-SEARCH : Interpolation\n");
         break;
      case EXP:
         fprintf(stdout, "F-SEARCH : Exponential\n");
         break;
      default:
         fprintf(stdout, "F-SEARCH : Unknown\n");
         return (EXIT_FAILURE);
   }


   runtimes = safe_malloc(sizeof(hrtime_t) * iopts->runs);

   /* perform intersection and run timings */
   for (i = 0; i < iopts->runs; i++) {
      start = gethrtime();
      /* TODO INTERSECT HERE */
      end = gethrtime();
      runtimes[i] = end - start;
   }

   if (iopts->verbose == TRUE) {
   /* Dump the intersect list */
      fprintf(stdout, "RESULT: ");

   /* TODO OUTPUT INTERSECTION RESULT */
      fprintf(stdout, "\n");
   }

   average = calculate_avg(runtimes, iopts->runs);
   stddev = calculate_stddev(runtimes, iopts->runs);
   fprintf(stdout, "RUNS : %d\n", iopts->runs);
   fprintf(stdout, "TIME : %.5f +/- %.5f s\n", average, stddev);

   /* Clean up the mess * */
   destroy_clo(iopts);
   if (runtimes)
      free(runtimes);

   return (EXIT_SUCCESS);
}
