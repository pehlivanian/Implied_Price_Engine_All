#ifndef __DATA_ELEMENT_HPP__
#define __DATA_ELEMENT_HPP__

#include "Visitor.hpp"

template<class Intrinsic>
class DataElement
{
public:
  inline void accept(Visitor<DataElement>* v) { v->visit(this); }
  virtual ~DataElement() {}
protected:
  DataElement() {}
};

#endif
