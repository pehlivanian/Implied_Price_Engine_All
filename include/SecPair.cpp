#include "SecPair.hpp"

void
SecPair::normalize() {

  // Degenerate case
  if ((f_ < 0) and (b_ < 0)) {
    f_ = -1;
    b_ = -1;
    mult_ = -1;
    return;
  }

  // Degenerate case
  if (mult_ == 0) {
    f_ = -1;
    b_ = -1;
    mult_ = -1;
    return;
  }

  if (f_ < 0) {
    f_ = b_;
    b_ = -1;
    mult_ *= -1;
  }
  else if (b_ < 0) {
    b_ = -1;
  }
  else {
    if (f_ == b_) {
      f_ = -1;
      b_ = -1;
      mult_ = -1;
    }
    else if (f_ > b_) {
      std::swap(f_,b_);
      mult_ *= -1;
    }
    else { return; }
  }
}

bool
SecPair::operator==(const SecPair& rhs) const
{

  if ( (f_ * mult_== rhs.mult() * rhs.leg0()) &&
       (b_ * mult_== rhs.mult() * rhs.leg1()) )
    {
      return true;
    }
  else
    {
      return false;
    }
}

bool
SecPair::operator!=(const SecPair& rhs) const
{
  return !(rhs == *this);
}

SecPair&
SecPair::operator=(const SecPair& rhs)
{

  f_ = rhs.leg0();
  b_ = rhs.leg1();
  mult_ = rhs.mult();
  normalize();

  return *this;    
}

inline void error(const std::string& s) { throw std::runtime_error(s); }

SecPair&
SecPair::operator+=(const SecPair& rhs) {

  if (rhs.isTrivialSpread()) {
    return *this;
  }

  if (this->isTrivialSpread()) {
    f_ = rhs.leg0();
    b_ = rhs.leg1();
    mult_ = rhs.mult();
    normalize();
    return *this;
   }

  auto f_r = rhs.leg0();
  auto b_r = rhs.leg1();
  auto m_r = rhs.mult();

  if ((f_ == f_r) && (b_ == b_r)) {
    mult_ += m_r;
    normalize();
    return *this;
  }
  else if ( mult_ * m_r > 0) {
    if ((f_ * mult_) == (m_r * b_r)) {
      f_ = f_r;
      normalize();
      return *this;
    }
    else if ((mult_ * b_) == (m_r * f_r)) {
      b_ = b_r;
      normalize();
      return *this;
    }
  }
  else if ( mult_ * m_r < 0)  {
    if (( mult_ * f_) == (-1 * m_r * f_r)) {
      f_ = std::min( b_, b_r);
      b_ = std::max( b_, b_r);
      mult_ = (f_ == b_r) ? -1 * m_r : -1 * mult_;
      return *this;
    }
    else if ((mult_ * b_) == (-1 * m_r * b_r)) {
      auto f_tmp = std::min( f_, f_r);
      b_ = std::max( f_, f_r);
      f_ = f_tmp;
      mult_ = (f_ == f_r) ? m_r : mult_;
      return *this;
    }
  }
  else {
    throw SecPairException();
  }

  throw SecPairException();

}

SecPair
SecPair::operator+(const SecPair& rhs) const
{
  SecPair SecPair_tmp = *this;
  SecPair_tmp += rhs;
  return SecPair_tmp;
}

SecPair&
SecPair::operator*=(int m)
{
  if (isTrivialSpread()) { return *this; }

  mult_ *= m;
  normalize();
  return *this;
}

SecPair
SecPair::operator*(int m) const
{
  SecPair SecPair_tmp(*this);
  SecPair_tmp *= m;
  return SecPair_tmp;
}

void operator*(int m, SecPair& rhs) { rhs*=m; }

SecPair&
SecPair::operator-=(const SecPair& rhs)
{
  SecPair rhs_tmp = SecPair(rhs);
  rhs_tmp *= -1;
  *this = *this + rhs_tmp;

  normalize();

  return *this;
  
}

SecPair
SecPair::operator-(const SecPair& rhs) const
{
  SecPair SecPair_tmp(*this);
  SecPair_tmp -= rhs;
  return SecPair_tmp;
}

bool
SecPair::operator<(const SecPair& rhs) const
{
  // Arbitrary
  return ((mult_ * f_) < (rhs.mult() * rhs.leg0()));
}

SecPair
SecPair::abs() const
{
  return SecPair(f_, b_, 1);
}

bool
SecPair::isPos() const
{
  return (abs() == *this);
}

bool
SecPair::isLeg() const
{
    return ((f_ >= 0) && (b_ < 0));
}

bool
SecPair::isTrivialSpread() const
{
  return ((f_ < 0) && (b_ < 0)) || mult_ == 0;
}
int
SecPair::leg0() const
{
    return f_;
}

int
SecPair::leg1() const
{
    return b_;
}

int
SecPair::mult() const
{
    return mult_;
}

bool
SecPair::isSpread() const {
  return (f_ >= 0) && (b_ >= 0);
}

bool
SecPair::isOutright() const {
  return (f_ < 0) || (b_ < 0);
}

std::ostream& operator<<(std::ostream& out, const SecPair& p)
{
  if ((p.leg0() < 0) && (p.leg1() < 0))
    {
      out << "0";
      return out;
    }
  else if ((p.leg0() >= 0) && (p.leg1() < 0))
    {
      std::string parity = "l";
      if (p.mult() == -1)
	parity = "neg_l";
      out << parity << p.leg0();
      return out;
    }
  else
    {
      std::string parity = "s";
      if (p.mult()== -1)
	parity = "neg_s";
      out << parity << p.leg0() << "_" << p.leg1();
      return out;
    }
}

