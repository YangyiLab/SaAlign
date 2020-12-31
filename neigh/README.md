# neigh
Neighbour Joining algorithm for the creation of Phylogenetic Trees, implemented 
in C.

## Building
The first step to build `neigh` is to obtain the source code:

```
$ git clone https://github.com/EliaCereda/neigh.git
```

`neigh` depends on a few external projects, which must be installed to compile 
it:

 * CMake, to portably generate the configuration files for a wide range of build 
   systems. 
 * gengetopt, to automatically generate the C code to parse command line 
   arguments.
 * Graphviz, to produce PDF visualizations of the generated
   phylogenetic trees **(optional)**.

This dependencies can be installed with the following command on **Ubuntu**:

```
$ sudo apt-get install cmake gengetopt libgraphviz-dev
```

On **macOS**, the following command can be used to install the dependencies with
the [Homebrew](brew.sh) package manager:

```
$ brew install cmake gengetopt graphviz
```

`neigh` can now be build with the following commands:

```
$ cd path/to/source/dir/

$ mkdir build/
$ cd build/

$ cmake .
$ make
```

## Input Format
The input format accepted by `neigh` is based on the popular _PHYLIP_ format:

 * an integer on the first line, representing the number of species
 * one line for each species:
   * starting with the name of the species
   * followed by the values of distance matrix, separated with spaces
   * optionally, an asterisk '*' marking the end of the line

The format differs from _PHYLIP_ in that only the _lower triangular_ form is 
supported.

From `examples/INGI2368.in`:

```
6
A 
B 2 
C 4 4
D 6 6 6
E 6 6 6 4
F 8 8 8 8 8
```

## Usage
This is the basic form to invoke `neigh`, it prints to the terminal output the
phylogenetic tree generated from the provided distances:

```
$ neigh path/to/input/file
```

This is the tree generated from `examples/INGI2368.in`:

```
Processing file 'examples/INGI2368.in'...

                                   |--1.00-- A
                        |--1.00-- C_1
                        |          |--1.00-- B
             |--1.00-- C_2
             |          |--2.00-- C
  |--2.50-- C_4
  |          |          |--2.00-- D
  |          |--1.00-- C_3
  |                     |--2.00-- E
C_5
  |--2.50-- F
```

Alternatively, `neigh` is able to visualize the generated phylogenetic trees in 
PDF format using Graphviz. The command to use is the following:

```
$ neigh path/to/input/file --graphviz-trees=final --output-dir=path/to/output/
```

The complete list of command line options supported by `neigh` can be printed 
with:

```
$ neigh --help
```

## Contributors

This project has been developed by Giorgia Adorni and Elia Cereda.
