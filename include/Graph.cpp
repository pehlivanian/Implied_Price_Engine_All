#include "Graph.hpp"

Graph::Graph(const Graph& rhs)
{
  SERIALIZE_WRITES;
  n_ = rhs.n_;
  directed_ = rhs.directed_;
  std::copy(rhs.vertices_.begin(), rhs.vertices_.end(), vertices_.begin());
}

Graph::Graph(Graph&& rhs) noexcept :
  n_(rhs.n_),
  directed_(rhs.directed_),
  vertices_(std::move(rhs.vertices_))
{}

Graph&
Graph::operator=(Graph&& rhs) noexcept
{
  return rhs;
}	

inline bool
Graph::isEdge(int v1, int v2) const
{
  SERIALIZE_READS;
  for(VertexIterator ei = vertices_[v1].begin(),
	ee = vertices_[v1].end();
      ei != ee;
      ++ei)
    {
      if (ei->first == v2)
	return true;
    }
  return false;
}

inline bool
Graph::isEdge(int v1, int v2, int& w) const
{
  SERIALIZE_READS;
  for(VertexIterator ei = vertices_[v1].begin(),
	ee = vertices_[v1].end();
      ei != ee;
      ++ei)
    {
      if (ei->first == v2)
	{
	  w = ei->second;
	  return true;
	}
    }
  return false;
}

inline int
Graph::edgeWeight(int v1, int v2) const
{
  SERIALIZE_READS;
    for(VertexIterator ei = vertices_[v1].begin(),
	ee = vertices_[v1].end();
      ei != ee;
      ++ei)
    {
      if (ei->first == v2)
	return ei->second;
    }

}

void 
Graph::load (std::string filename) {
  FILE *fp = fopen (filename.c_str(), "r");
  int nv, ne;

  // sufficient space
  char buf[4096];

  char* i_ = fgets (buf, 4096, fp);

  int nr = fscanf (fp, "%d %d %s\n", &nv, &ne, buf);
  if (nr != 3) {
    std::cout << "Invalid file format " << filename << "\n";
    n_=0;
    return;
  }

  if (!strcmp (buf, "directed")) {
    directed_ = true;
  } else {
    directed_ = false;
  }

  // vertices_.resize(nv);
  for(int i=0; i<nv; ++i)
    {
      arena<POOL_SIZE> ar;
      vertices_.emplace_back(A<IPair, POOL_SIZE>(ar));
    }
  n_ = nv;

  while (ne-- > 0) {
    int src, tgt, weight;

    int nr = fscanf (fp, "%d,%d,%d\n", &src, &tgt, &weight);
    if (nr == 2) {
      addEdge (src, tgt);
    } else if (nr == 3) {
      addEdge (src, tgt, weight);
    }
  }

  fclose(fp);
}
  
void
Graph::addEdge(int v1, int v2, int w)
{
  // No need to SERIALIZE as we are assuming this is only called on init_()
  vertices_[v1].emplace_front(v2, w);
  
  // undirected have both.
  if (!directed_) 
    vertices_[v2].emplace_front(v1,w);
}

void
Graph::addEdge(int v1, int v2)
{
  // No need to SERIALLIZE as we are assuming this is only called on init_()
  addEdge(v1, v2, 1);
}

bool
Graph::removeEdge(int v1, int v2)
{
  // No need to SERIALIZE as we are assuming this is only called on init_()
  bool found = false;
  for( VertexIterator ei = vertices_[v1].begin(),
	 ee = vertices_[v2].end();
       ei != ee;
       ++ei)
    {
      if (ei->first == v2)
	{
	  vertices_[v1].remove(*ei);
	  found = true;
	  break;
	}
    }
  if (!found) return false;

  if (!directed_) {
    for (VertexList::const_iterator ei = vertices_[v2].begin();
	 ei != vertices_[v2].end(); 
	 ++ei) {
      if (ei->first == v1) {
	vertices_[v2].remove(*ei);
	break;
      }
    }
  }
  return true;
}

// This function will assuredly be hot
bool
Graph::updateEdgeWeight(int v1, int v2, int w)
{
  SERIALIZE_WRITES;
  VertexIterator eb = vertices_[v1].before_begin();
  for( VertexIterator ei = vertices_[v1].begin(),
	 ee = vertices_[v1].end();
       ei != ee;
       ++ei)
    {
      if ((ei->first) == v2)
	{
	  // Remove edge
	  ei = vertices_[v1].erase_after(eb);
	  // Add new edge
	  vertices_[v1].emplace_front(v2, w);
	  return true;
	}
      eb = ei;
    }
  return false;
}
