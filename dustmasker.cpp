#include <stdio.h>

#include "ReadFiles.hpp"
#include "Dustmasker.hpp"

using namespace compactds ;

int main(int argc, char *argv[])
{
  ReadFiles reads ;
  reads.AddReadFile(argv[1], false, false) ;
  Dustmasker dustmasker ;

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
