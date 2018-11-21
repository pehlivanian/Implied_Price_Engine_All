#ifndef __SECPAIR_HPP__
#define __SECPAIR_HPP__

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <exception>

const unsigned MAX_LEGS = 48;

struct SecPairException : public std::exception {
    const char *what() const noexcept override {
        return "Invalid SecPair";
    }
};

class SecPair;
std::ostream& operator<<(std::ostream&, const SecPair&);
void operator*(int, SecPair&);

class SecPair {
public:
    friend void _swap( SecPair& lhs, SecPair& rhs) {
        std::swap(lhs.f_, rhs.f_);
        std::swap(lhs.b_, rhs.b_);
        std::swap(lhs.mult_, rhs.mult_);
    }

    SecPair() : f_(-1), b_(-1), mult_(-1) {}
    SecPair(int f, int b, int mult) : f_(f), b_(b), mult_(mult) { normalize(); }
    SecPair(const SecPair& rhs) : f_(rhs.leg0()), b_(rhs.leg1()), mult_(rhs.mult()) { normalize(); }
    SecPair(SecPair&& rhs) = default;

    SecPair abs()  const;
    bool isPos()   const;
    bool isLeg()   const;

    int leg0() const;
    int leg1() const;
    int mult() const;

    SecPair& operator*=(int);
    SecPair operator*(int) const;
    bool operator==(const SecPair&) const;
    bool operator!=(const SecPair&) const;
    SecPair& operator=(const SecPair&);
    SecPair& operator+=(const SecPair&);
    SecPair operator+(const SecPair&) const;
    SecPair& operator-=(const SecPair&);
    SecPair operator-(const SecPair&) const;
    bool operator<(const SecPair&) const;

    void normalize();
    bool isSpread()   const;
    bool isOutright() const;
    bool isTrivialSpread() const;

private:
    mutable bool valid_;
    int f_, b_, mult_;
};


#endif
