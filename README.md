# Vehicle Position Challenge

## Task
The binary data file _positions.dat_ contains a position for each of 2 million vehicles. Your task is to write a program that finds the nearest vehicle 
positions in the data file to each of the 10 co-ordinates provided in the table below. Your program should be able to find the position ID's of all 10 closest positions in less time than the benchmark approach that simply loops through each of the 2 million positions and keeps the closest to each of the 10 co-ordinates. The challenge is to optimize for speed and improve on the benchmark to the best of your ability.

## My Solution
The approach I took involved sorting the vehicles by latitude, followed by a loop through all 2 million that only considered the vehicles within a close latitude range of the test co-ordinate. The main assumption I make is that the shortest distance to the test co-ordinate will be within a small range of latitude as well. This allows my solution to skip over the calculation of ditances for all but a few vehicles that fall into this range. The initial rnge I choose is 0.05. The alogorithm proceeds as follows:
  * Sort all vehicles read from file by latitude (using qsort)
  * Consider a test co-ordinate {32, -100}, this will only consider vehicles in range (31.95,32.05)
  * If no vehicles fall in this range, it is doubled and the loop now considers vehicles in range (31.90,32.10)
  * This repeats to (31.80, 32.20), (31.60, 32.40) and so on until at least one vehicle in the latitude range is present.
  * The calculation of distance is only done within the subset of vehicles that meet the latitude criteria. 

By limiting the subset of vehicles, the number of distance calculations and update of closest vehicle is reduced, such that the search time per test co-ordinate is reduced. However, there is one main flaw. The initial range of 0.05 means that in an extreme case, this solution may fail to produce the correct output. For example, using the same co-ordinate above {32, -100}, a vehicle at {32.04,-100.5} (distance=0.2516) would be found as the closest over another at {32.06,-100.1} (distance=0.0136). 
