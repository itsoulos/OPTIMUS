# OPTIMUS
This projects proposes a programming tool written in ANSI C+ for global optimization problems. The main components of the software are: a) Coding of the objective problem in a high level language such as ANSI C++ b) Incorporation of many global optimization techniques to tackle the objective problem c)Parameterization of global optimization methods using user-defined parameters.
## Building instructions ##
### Requirements ###
 - A C++ compiler (GNU GCC is recommended).
 - The GNU make utility
 - The OpenMP programming library
 - The QT programming library.
### Installation ###
The following steps are required in order to build the tool:
  * <b>Download</b> and install the QT programming library from https://qt.io
  * <b>Download</b> the software from https://github.com/itsoulos/OPTIMUS.git
  * <b>Set</b> the OPTIMUSPATH environment variable pointing at the installation directory of OPTIMUS e.g.
     OPTIMUSPATH=/home/user/OPTIMUS/
  * <b> Set</b> the LD_LIBRAPY_PATH to include the OPTIMUS/lib subdirectory e.g. LD_LIBRAPY_PATH=$LD_LIBRAPY_PATH:$OPTIMUSPATH/lib/:
  * <b> Issue</b> the command:  cd $OPTIMUSPATH
  * <b> Execute</b> the compilation script: ./compile.sh
After the compilation the executable OptimusApp will be placed under bin subdirectory.

## Contents ##
The software has the following subdirectories
    - bin: After compilation of the package, it will contain the executable OptimusApp
    - Bfgs: the source code of the Bfgs optimization method
    - De: the source code for the Differential Evolution optimization method

    - doc: This directory contains the documentation of the package
    - PROBLEMS: A directory that contains some test functions.
    - compile.sh: The main compilation script.

## Example of optimization function
## Compile an optimization function
## The executable OptimusApp ##
## A typical run ##
A full working command for the Rastrigin problem using the utility program OptimusApp is shown below

./OptimusApp --filename=librastrigin.so --opt_method=Pso --pso_particles=100 --pso_generations=10
The parameters for the above command line are as follows:

1. The argument of --filename determines the objective problem in shared library format.

2. The argument of --opt_method sets the used global optimization procedure.

3. The argument of --pso_particles sets the number of particles of the PSO optimizer.

4. The argument of --pso_generations sets the maximum number of generations allowed.

5. The argument of --localsearch_method sets the used local optimization procedure.
The output of the previous command is as follows:

Generation     1 value:   -1.7464048

Generation     2 value:   -1.8619942

Generation     3 value:   -1.8852439

Generation     4 value:   -1.9490074

Generation     5 value:   -1.9490074

Generation     6 value:   -1.9490074

Generation     7 value:   -1.9490074

Generation     8 value:   -1.9775267

Generation     9 value:   -1.9972928

Generation    10 value:   -1.9977027

Minimum:       -2.0000000000  Function calls:    1028

Full documentation at [manual](
https://raw.githack.com/itsoulos/OPTIMUS/master/MANUAL/docs/html/index.html)
