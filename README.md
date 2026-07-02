# dustmasker

`dustmasker` is a small C++ command-line tool that finds low-complexity regions in DNA sequences using an SDUST-style algorithm.

## Features

- Detects low-complexity intervals in DNA sequences
- Configurable DUST threshold, window size, and linker size
- Reads input from file or stdin (`-`)
- Supports interval output or masked FASTA output

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
