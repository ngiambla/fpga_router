# FPGA Router using Maze Routing with Search Hystersis #

This project implements a FPGA router using a Maze Routing Algorithm, optimized for searching branch-outs.


## Setup ##
To compile this project, begin a terminal process, navigate to the `src` directory and enter the following commands:

```bash
$ scl enable devtoolset-2 'bash'
$ make clean
$ make
```

The compliation should complete with few warnings. If any changes are made, ensure to execute `make clean` first, before `make`-ing the executable


## Usage ##

To run this simulation of the FPGA router, start a terminal process, navigate to the `src` directory and enter:

```bash
$ ./circutils -file filename -switch [f | w] -isParallel [T | F] -t [y | n]
```

The options provided are:

1. -file
  The FPGA router requires some netlist schematic with the related circuit proportions. These specifications are noted in the files cct1, cct2, cct3, and cct4. Choose one of these files.
2. -switch
  The switch block style is either 'fully-connected' or 'wilton', where `f` indicates the configuration for a fully connected switch, and `w` for wilton.
3. -isParallel
  This sets the option to run the router with parallel execution. `T` is for true, `F` is for false.
4. -t
  To test the minimum width of the track, you can simply supply the argument `y` to test the width, or `n` to execute the router with no min-width testing.


## Author ##

Nicholas V. Giamblanco, 2017