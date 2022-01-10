# Implied_Price_Engine_All
Fast implied price server for energy complex. An algorithm is presented for the computation of implied quotes in a hypothetical market closely resembling CME WTI Crude (CL) futures, ICE/IPE Brent futures (B), or any one of the US crude product or natural gas markets. The algorithm produces a full set of implied quote prices and sizes using only user-entered quote information. Each leg quote (size, price) is computed on its own execution thread. We exploit this structure to test a cpu-distributed version, using a consumer-producer queue with worker tasks consuming a user quote and generating multiple downstream implied quotes per input. The algorithm computes the entire complex of n implied inside bids and asks in <img src="https://latex.codecogs.com/svg.image?\mathcal{O}\(n^2t\)" title="\mathcal{O}\(n^2\log{n}\)" /> while consuming n user-entered bids and asks.

### Normalized 12-market setup for fast computation
![plot](https://github.com/pehlivanian/Implied_Price_Engine_All/blob/master/docs/test_case.jpg?raw=true)

### Algorithm runtimes as function of n
![plot](https://github.com/pehlivanian/Implied_Price_Engine_All/blob/master/docs/algo_complexity.jpg?raw=true)
