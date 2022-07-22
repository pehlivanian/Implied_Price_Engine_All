# Implied_Price_Engine_All
Implementation of fast implied order book server modeled on CME, ICE
energy complex. The market modeled closely resembles CME WTI Crude
(CL) futures, ICE/IPE Brent (B) futures, CME Heating Oil, RBOB, ICE
Gasoil, etc. The main engine ingests streaming bid offer price &
size data for outright leg months and calendar spreads, and gives as
output 3 markets:

- User quotes   [BBO for outrights (legs) based on only user inputs in
outright markets]
- Impled quotes [BBO for implied outrights (legs) based on user inputs
in outright, calendar spread markets]
- Merged quotes [BBO for merged User + Implied quotes]


### Underlying graph for 12-month market setup, optimized for
efficient computation
![plot](https://github.com/pehlivanian/Implied_Price_Engine_All/blob/master/docs/test_case.jpg?raw=true)

### Algorithm runtimes as function of n [Merged BBO runtimes are
O(n^2*log(n)) in the number of markets]
![plot](https://github.com/pehlivanian/Implied_Price_Engine_All/blob/master/docs/algo_complexity.jpg?raw=true)
