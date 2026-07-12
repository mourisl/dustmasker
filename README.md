# dustmasker

`dustmasker` is a small C++ command-line tool that finds low-complexity regions in DNA sequences using an SDUST-style algorithm.

## Features

- Detects low-complexity intervals in DNA sequences
- Configurable DUST threshold, window size, and linker size
- Reads input from file or stdin (`-`)
- Supports interval output or masked FASTA output
- Includes a self-contained `Dustmasker.hpp` header for embedding the algorithm in other C++ projects

## Build

Requirements:

- `g++`
- `make`
- `zlib` development library

Build with:

```bash
make
```

This produces the `dustmasker` executable in the repository root.

## Using `Dustmasker.hpp`

The repository also provides a header-only `Dustmasker` class in `Dustmasker.hpp` for direct use in C++ code.

Basic steps:

1. Include `Dustmasker.hpp`
2. Construct a `Dustmasker` instance
3. Initialize the alphabet with `Init("ACGT")`
4. Optionally adjust parameters with `SetWindowSize`, `SetThreshold`, and `SetLinkerSize`
5. Call `Mask` on a sequence and inspect the returned intervals

Example:

```cpp
#include <iostream>
#include <vector>
#include "Dustmasker.hpp"

int main() {
  const char *seq = "ACGTACGTAAAAAAAAAACGTNNNNACGT";

  Dustmasker dustmasker;
  dustmasker.Init("ACGT");
  //dustmasker.SetWindowSize(64);
  //dustmasker.SetThreshold(20);
  //dustmasker.SetLinkerSize(1);

  std::vector<_dustmasker_perfect_interval> regions;
  dustmasker.Mask(seq, strlen(seq), regions);

  for (const auto &region : regions) {
    std::cout << region.start << " - " << region.end << '\n';
  }

  return 0;
}
```

Notes:

- `Mask` writes low-complexity intervals into the result vector as 0-based inclusive `start`/`end` coordinates.
- `Init("ACGT")` configures the DNA alphabet; characters not in the alphabet are treated as non-specific characters.
- `Mask` clears the output vector before filling it.
- For sequences shorter than 3 bases, no intervals are returned.

## Usage

```bash
./dustmasker [OPTIONS]
```

Options:

- `--in FILE` input FASTA file (default: `-`, stdin)
- `--level INT` DUST level/threshold (default: `20`)
- `--window INT` DUST window length (default: `64`)
- `--linker INT` linker size for merging nearby intervals (default: `1`)
- `--outfmt STR` output format: `interval` or `fasta` (default: `interval`)
- `-h` print help

## Output formats

### `interval` (default)

For each sequence, prints:

1. FASTA header line (`>sequence_id`)
2. One line per low-complexity interval as `start - end`, where both `start` and `end` are 0-based inclusive coordinates

### `fasta`

For each sequence, prints:

1. FASTA header line (`>sequence_id`)
2. Sequence where low-complexity intervals are converted to lowercase

## Example

```bash
./dustmasker --in input.fa --outfmt interval
./dustmasker --in input.fa --outfmt fasta --level 20 --window 64 --linker 1
```
