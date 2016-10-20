#include "cat2_visitor.hpp"

void 
cat2_visitor::generate(DataElement* d)
{
  Graph *g = dynamic_cast<Graph*>(d);

  const int n = g->numVertices();
  pred_.assign(n, -1);
  dist_.assign(n, std::numeric_limits<int>::max());
  size_.assign(n, std::numeric_limits<int>::max());
  dist_[s_] = 0;
  BinaryHeap pq(n);

    int v, u;
    long newLen;
    size_t newSize;
    Graph::Graph_iterator ci, last;
    for(int u = 0; u < n; u++) { pq.insert(u, dist_[u]); }

  // Find vertex in ever shrinking set, V-S, whose dist_[] 
  // is smallest. Recompute potential new paths to update all shotest paths.
  while(!pq.isEmpty())
    {
      u = pq.smallest(); // this also pops

      ci = g->begin(u);
      last = g->end(u);

        for(;
	    ci != last;
	    ++ci)
	    {
	        v = ci->first;
	        newLen = dist_[u];
	        newLen += (ci->second).first;
	        newSize = std::min(size_[u], (ci->second).second);
	        if (newLen < dist_[v])
            {
	            pq.decreaseKey(v, newLen);
	            dist_[v] = newLen;
	            size_[v] = newSize;
	            pred_[v] = u;
	        }
	    }
    }
}
