# A-Star-Pathfinding
A* algorithm C++ implementation for pathfinding

## Context :
- Original developement date : 02/2018
- Haven't done much C++ in recent months
- Developped as part of a Computer Games Programming coursework at HWU.
  - Limited development time (single day)
  - Very limited value as part of the coursework : 2 points out of 20.

## Features 
- Support for diagonal movements and special terrain elements (walls, slower cells).
- Arbitrary grid size (ASCII form)
- Heavy use of pointers
- Customizable start / end points.


## Self analysis as of 28/07/2018 :
  - Need more comments
  - Some code sections could be more explicit (e.g. 'GetNeighbours' when testing for adjacent cells)
  - Some vectorizable code (e.g. 'NodeIn')
  - Possible performance improvements (most likely - code not profiled)
    - Replace the ClosedList (i.e. 'visited') type from std::vector to std::unordered_set for faster 'exists in' checks.
    - Smarter / faster map creation (i.e. replace loops by... maybe something from Boost + smarter special cells distribution)

## Overall : 
- Stability    : OK
- Readability  : OK, but missing comments
- Code quality : MOK, some sections to be more explicit / less "loopy".
- Performance  : MOK (good enough for smallish grid sizes)
