#include <stdio.h>
#include <getopt.h>

#include "ReadFiles.hpp"
#include "Dustmasker.hpp"

char usage[] = "./dustmasker [OPTIONS]\n"
  "\t--in FILE: input fasta file [-]\n"
  "\t--level INT: DUST level [20]\n"
  "\t--window INT: DUST window length [64]\n"
  "\t--linker INT: DUST linker [1]\n"
  "\t--outfmt STR: output format from 'interval', 'fasta' ['interval']\n"
  "\t-h: print this usage message\n"
  ;

static const char *short_options = "h" ;

static struct option long_options[] = {
  { "in", required_argument, 0, 10000,},
  { "level", required_argument, 0, 10001,},
  { "window", required_argument, 0, 10002,},
  { "linker", required_argument, 0, 10003,},
  { "outfmt", required_argument, 0, 10004,},
  { (char *)0, 0, 0, 0} 
} ;

int main(int argc, char *argv[])
{
  ReadFiles reads ;
  Dustmasker dustmasker ;

  int c, option_index ;
  option_index = 0 ;

  int outputFmt = 0 ; // 0: interval, 1: fasta 

  while (1)
  {
    c = getopt_long( argc, argv, short_options, long_options, &option_index ) ;

    if (c == -1)
      break ;

    if (c == 'h')
    {
      fprintf( stdout, "%s", usage ) ;
      return 0 ;
    }
    else if (c == 10000) // fasta file for dustmasking
    {
      reads.AddReadFile(optarg, false, false) ;
    }
    else if (c == 10001) // dustmasker threshold
    {
      dustmasker.SetThreshold(atoi(optarg)) ;
    }
    else if (c == 10002) // dustmasker window size
    {
      dustmasker.SetWindowSize(atoi(optarg)) ;
    }
    else if (c == 10003) // dustmasker linker size
    {
      dustmasker.SetLinkerSize(atoi(optarg)) ;
    }
    else if (c == 10004) // output format
    {
      if (!strcmp(optarg, "interval"))
        outputFmt = 0 ;
      else if (!strcmp(optarg, "fasta"))
        outputFmt = 1 ;
      else
      {
        fprintf(stderr, "Error: unknown output format %s\n", optarg) ;
        return 1 ;
      }
    }
  }

  if (reads.GetFileCount() == 0)
  {
    reads.AddReadFile("-", false, false) ;
  }

  dustmasker.Init("ACGT") ;
  std::vector<_dustmasker_perfect_interval> lcintervals ;
  while (reads.Next())
  {
    printf(">%s\n", reads.id) ;

    //Uppercase the sequence
    size_t n = strlen(reads.seq) ;
    for (size_t i = 0 ; i < n ; ++i)
    {
      if (reads.seq[i] >= 'a' && reads.seq[i] <= 'z')
        reads.seq[i] = reads.seq[i] - 'a' + 'A' ;
    }

    dustmasker.Mask(reads.seq, n, lcintervals) ; 

    if (outputFmt == 0) // output interval format
    {
      for (size_t i = 0 ; i < lcintervals.size() ; ++i)
      {
        printf("%lu - %lu\n", lcintervals[i].start, lcintervals[i].end) ;
      }
      continue ;
    }
    else
    {
      // Lower case the low complexity region
      for (size_t i = 0 ; i < lcintervals.size() ; ++i)
      {
        size_t start = lcintervals[i].start ;
        size_t end = lcintervals[i].end ;
        for (size_t j = start ; j <= end ; ++j)
        {
          if (reads.seq[j] >= 'A' && reads.seq[j] <= 'Z')
            reads.seq[j] = reads.seq[j] - 'A' + 'a' ;
        }
      }
      printf("%s\n", reads.seq) ;
    }
  }

  return 0 ;
}
