# Vehicle Position Challenge

## My Solution
The approach I took involved sorting the vehicles by latitude, followed by a loop through all 2 million that only considered the vehicles within a close latitude range of the test co-ordinate. The main assumption I make is that the shortest distance to the test co-ordinate will be within a small range of latitude as well. This allows my solution to skip over the calculation of ditances for all but a few vehicles that fall into this range. The initial rnge I choose is 0.05. The alogorithm proceeds as follows:
  * Sort all vehicles read from file by latitude (using qsort)
  * Consider a test co-ordinate {32, -100}, this will only consider vehicles in range (31.95,32.05)
  * If no vehicles fall in this range, it is doubled and the loop now considers vehicles in range (31.90,32.10)
  * This repeats to (31.80, 32.20), (31.60, 32.40) and so on until at least one vehicle in the latitude range is present.
  * The calculation of distance is only done within the subset of vehicles that meet the latitude criteria. 

By limiting the subset of vehicles, the number of distance calculations and update of closest vehicle is reduced, such that the search time per test co-ordinate is reduced. However, there is one main flaw. The initial range of 0.05 means that in an extreme case, this solution may fail to produce the correct output. For example, using the same co-ordinate above {32, -100}, a vehicle at {32.04,-100.5} (distance=0.2516) would be found as the closest over another at {32.06,-100.1} (distance=0.0136). 

## Task
The binary data file _positions.dat_ contains a position for each of 2 million vehicles. Your task is to write a program that finds the nearest vehicle 
positions in the data file to each of the 10 co-ordinates provided in the table below. Your program should be able to find the position ID's of all 10 closest positions in less time than the benchmark approach that simply loops through each of the 2 million positions and keeps the closest to each of the 10 co-ordinates. The challenge is to optimize for speed and improve on the benchmark to the best of your ability.

## Specifics
  * Use Git to clone the project template to your PC
  * Use C language and GCC compiler toolchain (recommend MinGW/MSYS for Windows users)
  * Your code should be compatible with 32 and 64 bit architectures
  * Add your code to _solution.c_
  * Complete _makefile_ with rules to compile and clean
     * Compile: Should only recompile code when any source code changed
       * Disable optimization
       * Disable generation of debug information (for final submission)
       * Enable all general warnings
       * Enable conversion warnings
     * Clean: Should not show error if files does not exist
  * Add code to time execution of the content of the _main()_ function with millisecond accuracy. This includes reading of the file.
  * You are allowed to use any means possible to speed up execution - this is not limited to code/algorithm optimization.

## How to submit your solution
 * Create a __PRIVATE__ repository on Github (free)            <--- Very important: Ensure that the repository is __PRIVATE__
 * Update the README.md file:
   * Explain your problem solving approach (design decisions) and your method
   * Specify any assumptions made
 * Add the following contributors to your repository:
   * https://github.com/ErnestL
   * https://github.com/CarloPutter
   * https://github.com/winstonarendse
   * https://github.com/GordonJ
 * Create a pull request for your solution
 * Notify us of completion with an email containing your repository URL
   * Nix.Stephens@mixtelematics.com
   * Ernest.Lotter@mixtelematics.com
   * Carlo.Putter@mixtelematics.com
   * Winston.Arendse@mixtelematics.com
   * Gordon.Jefferies@mixtelematics.com

## Additional information

### Structure of the objects within _position.dat_:

Field               |Data Type
--------------------|--------------------------------------------------------
Position ID         |32 bit integer
Vehicle Registration|Null terminated string (ASCII)
Latitude            |32 bit floating point
Longitude           |32 bit floating point
UTC Timestamp       |64 bit unsigned integer (number of seconds since Epoch)

### 10 co-ordinates:

Position  |1          | 2        | 3         | 4        | 5        | 6         | 7         | 8        | 9        | 10
----------|-----------|----------|-----------|----------|----------|-----------|-----------|----------|----------|------------
Latitude  |34.544909  |32.345544 |33.234235  |35.195739 |31.895839 |32.895839  |34.115839  |32.335839 |33.535339 |32.234235
Longitude |-102.100843|-99.123124|-100.214124|-95.348899|-97.789573|-101.789573|-100.225732|-99.992232|-94.792232|-100.222222
