# Cache Simulator
Cache Simulator in C++.

**Cache Types**:
- Direct Mapped (*DM*)
- Set Associative (*SA*)
- Fully Associative (*FA*)

**Block Replacement Policies**:
- Least Recently Used (*LRU*)
- Static Re-Reference Interval Prediction (*SRRIP*)

## Inputs

- Text file containing the memory addresses in the order they are accesses in decimal form
  - Maximum value: 2<sup>64</sup> - 1 (64-bit unsigned integers)
- Cache Size (in blocks)
- Cache Type
- Number of ways (only if *SA* is selected)
- Block Replacement Policy (only if *SA* or *FA* is selected)
- Re-Reference Prediction Value (*RRPV*) (only if *SRRIP* is selected)

## Output

Text file containing the following information:
- Final contents of the cache for each set and way
- Total Number of Cache Queries
- Total Number of Unique Addresses
- Total Number of Misses
- Total Number of Hits
- Miss Rate
- Hit Rate
