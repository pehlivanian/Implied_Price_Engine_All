# Implied_Price_Engine_All
Implementation of fast implied order book server modeled on CME, ICE energy complex. The market modeled closely resembles CME WTI Crude (CL) futures, ICE/IPE Brent (B) futures, CME Heating Oil, RBOB, ICE Gasoil, etc. The main engine ingests streaming bid offer price & size data for outright leg months and calendar spreads, and gives as output 3 markets:

- User quotes   [BBO for outrights (legs) based on only user inputs in outright markets]
- Impled quotes [BBO for implied outrights (legs) based on user inputs
in outright, calendar spread markets]
- Merged quotes [BBO for merged User + Implied quotes]


### Underlying graph for 12-month market setup, optimized for efficient computation
![plot](https://github.com/pehlivanian/Implied_Price_Engine_All/blob/master/docs/test_case.jpg?raw=true)

### Algorithm runtimes as function of n [Merged BBO runtimes are
O(n^2*log(n)) in the number of markets]
![plot](https://github.com/pehlivanian/Implied_Price_Engine_All/blob/master/docs/algo_complexity.jpg?raw=true)

### Sample Simulator output, 12 serial markets (front 12 in CME CL), User, Implied, Merged BBO:

```
 : ===========
 : USER PRICES
 : ===========
           leg_0            leg_1            leg_2            leg_3            leg_4            leg_5            leg_6            leg_7            leg_8            leg_9           leg_10           leg_11 
================ ================ ================ ================ ================ ================ ================ ================ ================ ================ ================ ================ 
     4592 x (13)      4614 x (25)      4607 x (25)       4622 x (2)      4618 x (21)      4636 x (17)       4639 x (7)      4626 x (19)       4634 x (9)      4647 x (11)      4640 x (16)       4637 x (8) 
     4591 x (14)       4610 x (2)       4599 x (2)       4609 x (8)       4608 x (6)      4614 x (13)      4614 x (22)       4623 x (9)       4624 x (2)      4617 x (11)      4631 x (21)      4636 x (19) 
================ ================ ================ ================ ================ ================ ================ ================ ================ ================ ================ ================ 
 : ==============
 : IMPLIED PRICES
 : ==============
           leg_0            leg_1            leg_2            leg_3            leg_4            leg_5            leg_6            leg_7            leg_8            leg_9           leg_10           leg_11 
================ ================ ================ ================ ================ ================ ================ ================ ================ ================ ================ ================ 
     4597 x (12)      4613 x (13)       4604 x (8)       4617 x (8)       4613 x (4)      4630 x (13)       4632 x (2)       4628 x (4)       4636 x (7)       4639 x (8)       4645 x (8)      4639 x (13) 
     4589 x (18)       4609 x (2)       4601 x (6)      4612 x (11)      4607 x (19)       4621 x (1)      4621 x (14)       4617 x (4)       4619 x (7)       4623 x (8)      4627 x (19)      4626 x (14) 
================ ================ ================ ================ ================ ================ ================ ================ ================ ================ ================ ================ 
 : =============
 : MERGED PRICES
 : =============
           leg_0            leg_1            leg_2            leg_3            leg_4            leg_5            leg_6            leg_7            leg_8            leg_9           leg_10           leg_11 
================ ================ ================ ================ ================ ================ ================ ================ ================ ================ ================ ================ 
     4592 x (13)      4613 x (13)       4604 x (8)       4617 x (8)       4613 x (4)      4630 x (13)       4632 x (2)      4626 x (19)       4634 x (9)       4639 x (8)      4640 x (16)       4637 x (8) 
     4591 x (14)       4610 x (2)       4601 x (6)      4612 x (11)       4608 x (6)       4621 x (1)      4621 x (14)       4623 x (9)       4624 x (2)       4623 x (8)      4631 x (21)      4636 x (19) 
================ ================ ================ ================ ================ ================ ================ ================ ================ ================ ================ ================ 

```