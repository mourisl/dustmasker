#include <stdio.h>
#include <getopt.h>

#include "ReadFiles.hpp"
#include "Dustmasker.hpp"

using namespace compactds ;

static const char *short_options = "f:t:w:" ;

int main(int argc, char *argv[])
{
  ReadFiles reads ;
  Dustmasker dustmasker ;

  int c, option_index ;
  option_index = 0 ;
  
  while (1)
  {
    c = getopt_long( argc, argv, short_options, NULL, &option_index ) ;

    if (c == -1)
      break ;

    if (c == 'f') // fasta file for dustmasking
    {
      reads.AddReadFile(optarg, false, false) ;
    }
    else if (c == 't') // dustmasker threshold
    {
      dustmasker.SetThreshold(atoi(optarg)) ;
    }
    else if (c == 'w') // dustmasker window size
    {
      dustmasker.SetWindowSize(atoi(optarg)) ;
    }
  }


  dustmasker.Init("ACGT") ;
  std::vector<_dustmasker_perfect_interval> lcintervals ;
  while (reads.Next())
  {
    printf(">%s\n", reads.id) ;
    dustmasker.Mask(reads.seq, strlen(reads.seq), lcintervals) ; 
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


  return 0 ;
}
