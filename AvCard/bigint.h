#pragma once
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "typedef.h"
#define MAX_LEN 32
#define MAX_VALUE ((1ULL << 32) - 1)

class BigInteger {
public:
	size_t length;
	uint32 data[1 + MAX_LEN];


	void LevelUp();


public:
	bool getBit(const uint32 &at) const;
	BigInteger();

	void reduce(uint32 t) {
		for (; t < length; ++t) this->data[t] = 0U;
	}
	BigInteger(byte *data, int length) ;
	BigInteger(uint32 t);
	BigInteger(const BigInteger &another) {
		if (this != &another) {
			memcpy(this->data, another.data, sizeof this->data);
			this->length = another.length;
		}
	}

	byte* getData() const;
	int getLength() const;

	BigInteger& operator=(const BigInteger &another);


	friend bool operator==(const BigInteger &a, const BigInteger &b) ;
	friend bool operator >(const BigInteger &a, const BigInteger &b) ;
	friend bool operator <(const BigInteger &a, const BigInteger &b) ;

	friend bool operator <=(const BigInteger &a, const BigInteger &b);

	friend bool operator >=(const BigInteger &a, const BigInteger &b) ;
	BigInteger& operator+=(const BigInteger& another) ;
	//works only if this >= another, must be checked on higher level
	BigInteger& operator-=(const BigInteger& another);

	BigInteger& operator*=(const uint32& b) ;
	BigInteger& operator *= (const BigInteger& another);

	BigInteger& operator<<=(const uint32 &n);
	BigInteger &operator>>=(const uint32 &n);
	friend BigInteger operator <<(const BigInteger &a, const uint32 &n) {
		BigInteger ret = a;
		ret <<= 1;
		return ret;
	}



	BigInteger& operator/=(const BigInteger& another);

	BigInteger& operator %= (const BigInteger &another) ;

	friend BigInteger operator % (const BigInteger &a, const BigInteger &b) ;

	friend BigInteger operator/ (const BigInteger &a, const BigInteger &b);

	friend BigInteger operator+(const BigInteger &a, const BigInteger& b) ;

	friend BigInteger operator-(const BigInteger &a, const BigInteger& b) ;

	friend BigInteger operator *(const BigInteger &a, const BigInteger& b) ;
	friend BigInteger operator *(const BigInteger &a, const uint32 b) ;

	BigInteger pow(uint32 n) ;

	friend BigInteger powMod(BigInteger a, uint32 n, const BigInteger &mod);

	friend BigInteger powMod(BigInteger a, BigInteger n,  const BigInteger &mod) {
		BigInteger zero(0), one(1);
		BigInteger ret(1);
		while (n > zero) {
			if (n.data[0] & 1) {
				ret *= (a) %= mod;
				n-=one;
			} else {
				(a *= a) %= mod;
				n >>= 1;
			}
		}
		return ret;
	}

	uint32 getBitsLength() const {
		uint32 ret = MAX_LEN << 5;
		while (ret <= (MAX_LEN << 5) && !getBit(ret)) --ret;
		if (ret  > MAX_LEN << 5) ret = 0;
		return ret;
	}
};

