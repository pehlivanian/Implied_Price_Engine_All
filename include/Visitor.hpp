#ifndef __VISITOR_HPP__
#define __VISITOR_HPP__

template<template<typename> class Element>
class Visitor
{
public:
  template<typename Intrinsic>
  inline void visit(Element<Intrinsic>* d) { generate(d); }
  virtual ~Visitor() {}
private:
  template<typename Intrinsic>
  void generate(Element<Intrinsic>*);
protected:
  Visitor() {}
};

#endif
