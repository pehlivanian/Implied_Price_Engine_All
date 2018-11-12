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

SecPair&
SecPair::operator+=(const SecPair& rhs) {

  if (rhs.isTrivialSpread()) {
    return *this;
  }

  if ((f_ == rhs.leg0()) && (b_ == rhs.leg1())) {
    mult_ += rhs.mult();
    normalize();
    return *this;
  }
  else if ( mult_ * rhs.mult() > 0) {
    if ((f_ * mult_) == (rhs.mult() * rhs.leg1())) {
      f_ = rhs.leg0();
      normalize();
      return *this;
    }
    else if ((mult_ * b_) == (rhs.mult() * rhs.leg0())) {
      b_ = rhs.leg1();
      normalize();
      return *this;
    }
  }
  else if ( mult_ * rhs.mult() < 0)  {
    if (( mult_ * f_) == (-1 * rhs.mult() * rhs.leg0())) {
      f_ = std::min( b_, rhs.leg1());
      b_ = std::max( b_, rhs.leg1());
      mult_ = (f_ == rhs.leg1()) ? -1 * rhs.mult() : -1 * mult_;
      return *this;
    }
    else if ((mult_ * b_) == (-1 * rhs.mult() * rhs.leg1())) {
      f_ = std::min( f_, rhs.leg0());
      b_ = std::max( f_, rhs.leg0());
      mult_ = (f_ == rhs.leg0()) ? rhs.mult() : mult_;
      return *this;
    }
  }
  else {
    throw SecPairException();
  }

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
SecPair::operator*(int m)
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

