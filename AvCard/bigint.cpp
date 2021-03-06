#include "BigInt.h"
#include "helpers.h"

bool BigInteger::getBit(const uint32 &at) const {
	return ::getBit(this->data, at);
}

void BigInteger::LevelUp()
{
	for (int i = length ;i>=1;i--)
		data[i] = data[i-1];
	if (data[length]) ++length;
}

BigInteger::BigInteger(uint32 t) {

	memset(this->data, 0x00,sizeof this->data);
	this->length = 1;
	this->data[0] = t;

}

BigInteger::BigInteger() {
	int once = 0;
	if (!once) {
		once = 1;
	}
	length = 1;	memset(this->data, 0x00,sizeof this->data);

}

int BigInteger ::getLength() const{
	return this->length;
}

byte *BigInteger::getData() const {
	byte *ret = new byte[MAX_LEN << 2];
	for (size_t at = 0; at < MAX_LEN; ++at) {
		uint32 t = this->data[at];
		byte *q = (byte*)&t;
		change_endian(q);
		for (size_t jj = 0; jj < 4; ++jj) ret[(at << 2) + jj] = q[jj];
	}
	return ret;
}


BigInteger::BigInteger(byte *data, int length) {

	memset(this->data, 0x00,sizeof this->data);
	this->length = 0;
	memset(this->data, 0, sizeof this->data);
	for (int at = 0; at < length; at += 4) {
		for (int j = at; j < min(at + 4, length); ++j) {
			this->data[this->length] <<= (sizeof byte) << 3;
			this->data[this->length] |= data[j];
		}
		++this->length;
	}		
	while (this->length > 1 && !this->data[this->length - 1]) --this->length;
}

BigInteger& BigInteger::operator=(const BigInteger &another) {
	if (this != &another) {
		memset(this->data, 0, sizeof this->data);
		for (size_t at = 0; at < another.length; this->data[at] = another.data[at++]);
		this->length = another.length;
	}
	return *this;
}


bool operator==(const BigInteger &a, const BigInteger &b) {
	if (a.length != b.length) return false;
	for (int i = a.length - 1; i >= 0; --i) if (a.data[i] != b.data[i]) return false;
	return true;
}
bool operator >(const BigInteger &a, const BigInteger &b) {
	if (a.length!=b.length) return a.length>b.length;
	for (int i=a.length-1;i>=0;i--)
	{
		if (a.data[i]!=b.data[i])
			return a.data[i]>b.data[i];
	}
	return false;
}
bool operator <(const BigInteger &a, const BigInteger &b) {
	if (a.length!=b.length) return a.length<b.length;
	for (int i=a.length-1;i>=0;i--)
	{
		if (a.data[i]!=b.data[i])
			return a.data[i]<b.data[i];
	}
	return false;
}

bool operator <=(const BigInteger &a, const BigInteger &b) {
	if (a.length!=b.length) return a.length<b.length;
	for (int i=a.length-1;i>=0;i--)
	{
		if (a.data[i]!=b.data[i])
			return a.data[i]< b.data[i];
	}
	return true;
}

bool operator >=(const BigInteger &a, const BigInteger &b) {
	if (a.length!=b.length) return a.length<b.length;
	for (int i=a.length-1;i>=0;i--)
	{
		if (a.data[i]!=b.data[i])
			return a.data[i]> b.data[i];
	}
	return true;
}

BigInteger& BigInteger:: operator+=(const BigInteger& another) {
	int carry = 0;
	for (size_t at = 0; at < another.length || carry; ++at) {
		if (at == this->length) ++this->length;
		unsigned long long actual = this->data[at]; actual += carry;
		if (at < another.length)	actual += another.data[at];
		if (actual > MAX_VALUE) {
			carry = 1;
		}
		this->data[at] = actual;
	}
	return *this;
}
//works only if this >= another, must be checked on higher level
BigInteger& BigInteger::operator-=(const BigInteger& another) {
	unsigned long long carry = 0;
	for (size_t at = 0; at < another.length || carry; ++at) {
		unsigned long long actualA = this->data[at], actualB = carry;
		if (at < another.length) actualB += another.data[at];
		if (actualA < actualB) {
			actualA += MAX_VALUE;
			actualA -= actualB;
			this->data[at] = actualA;
			carry = 1;				
		} else {
			this->data[at] -= actualB;
			carry = 0;
		}
	}
	while (this->length > 1 && this->data[this->length - 1] == 0) --this->length;
	return *this;
}

BigInteger& BigInteger::operator*=(const uint32& b) {
	unsigned long long carry = 0, actual;
	for (size_t at = 0; at < this->length || carry; ++at) {
		actual = 1ULL * this->data[at];
		actual *= b;
		actual += carry;
		this->data[at] = actual;
		carry = actual >> sizeof ( uint32 ) * 8;
	}
	while (this->length > 1 && this->data[this->length - 1] == 0) --this->length;
	return *this;
}
BigInteger& BigInteger::operator *= (const BigInteger& another) {
	uint32 *temp = new uint32[this->length];
	memcpy(temp, this->data, (sizeof uint32) * this->length);
	size_t len = this->length;
	size_t i , j ;
	uint32 carry=0 ;
	uint64 mul ;
	uint32 t[MAX_LEN+1];
	memset(t, 0x00, sizeof t);
	for (i = 0 ; i < len; ++i) {
		carry = 0;
		for (j = 0; j < another.length || carry; ++j) {
			mul = t[i + j];
			mul += ((1ULL) * temp[i]) * (j < another.length ? another.data[j] : 0ULL);
			mul += carry;
			t[i + j] = mul;
			carry = mul >> 32;
		}
	}

	this->length <<= 1;
	memcpy(this->data, t, length << 2);
	while (this->length > 1 && this->data[this->length - 1] == 0) --this->length;
	delete temp;
	return *this;
}

BigInteger& BigInteger::operator>>=(const uint32 &n) {
	if (n == 0) return *this;
	int nInts = n >> 5;
	int nBits = n & 0x1F;
	uint32 *mag = new uint32 [this->length];
	memcpy(mag, this->data, this->length * sizeof this->data[0]);
	int magLen = this->length;
	memset(this->data, 0, sizeof this->data);
	if (nInts >= magLen) {
		this->length = 1;
		return *this;
	}
	if (nBits == 0) {
		int newMagLen = magLen - nInts;
		for (int i=0; i<newMagLen; i++)
			this->data[i] = mag[i];
		this->length = newMagLen;
	} else {
		int newMagLength = 0;
		int i = 0;
		int highBits = mag[0] >> nBits;
		if (highBits != 0) {
			newMagLength =  magLen - nInts;
			this->data[i++] = highBits;
		} else {
			newMagLength = magLen - nInts -1;
		}

		int nBits2 = 32 - nBits;
		int j=0;
		while (j < magLen - nInts - 1)
			this->data[i++] = (mag[j++] << nBits2) | (mag[j] >> nBits);
		this->length = newMagLength;
	}
	return *this;
}

BigInteger& BigInteger::operator<<=(const uint32& n) {
	if (n == 0) return *this;
	int nInts = n >> 5;
	int nBits = n & 0x1F;
	uint32 *mag = new uint32 [this->length];
	memcpy(mag, this->data, this->length * sizeof this->data[0]);
	int magLen = this->length;
	memset(this->data, 0, sizeof this->data);
	if (nBits == 0) {
		this->length = magLen + nInts;
		for (int i=0; i<magLen; i++)
			this->data[i] = mag[i];
	} else {
		int i = 0;
		int newMagLength;
		int nBits2 = 32 - nBits;
		int highBits = mag[0] >> nBits2;
		if (highBits != 0) {
			newMagLength = magLen + nInts + 1;
			this->data[i++] = highBits;
		} else {
			newMagLength = magLen + nInts;
		}
		int j=0;
		while (j < magLen-1)
			this->data[i++] = mag[j++] << nBits | mag[j] >> nBits2;
		this->data[i] = mag[j] << nBits;
		this->length  = newMagLength;
	}
	return *this;
}


BigInteger& BigInteger::operator/=(const BigInteger& another) {
	BigInteger cur1, cur2;
	uint32 *digits = new uint32[this->length];
	memcpy(digits, this->data, this->length * (sizeof this->data[0]));
	memset(this->data, 0, sizeof this->data);
	int len = this->length;
	for (int i = len - 1; i>=0; --i) {
		cur1.LevelUp();
		cur1.data[0] = digits[i];
		uint64 x = 0, l = 0, r = MAX_VALUE;
		while (l <= r) {
			uint32 m = l + ((r - l) >> 1);
			cur2 = another * m;
			if (cur2 <= cur1) {
				x = m;
				l = m;
				++l;
			} else {
				r = m;--r;
			}
		}
		this->data[i] = x;
		cur1 = cur1 - another * x;
	}
	while (this->length > 1 && !this->data[this->length - 1]) --this->length;
	return *this;
}

BigInteger& BigInteger::operator%= (const BigInteger &another) {
	BigInteger act = (*this) / another;
	act *= another;
	(*this) -= act;
	return *this;
}

BigInteger operator % (const BigInteger &a, const BigInteger &b) {
	BigInteger ret = a;
	ret %= b;
	return ret;
}

BigInteger operator/ (const BigInteger &a, const BigInteger &b) {
	BigInteger ret = a;
	ret /= b;
	return ret;
}

BigInteger operator+(const BigInteger &a, const BigInteger& b) {
	BigInteger ret = a;
	ret += b;
	return ret;
}

BigInteger operator-(const BigInteger &a, const BigInteger& b) {
	BigInteger ret = a;
	ret -= b;
	return ret;
}

BigInteger operator *(const BigInteger &a, const BigInteger& b) {
	BigInteger ret = a;
	ret *= b;
	return ret;
}
BigInteger operator *(const BigInteger &a, const uint32 b) {
	BigInteger ret = a;
	ret *= b;
	return ret;
}

BigInteger BigInteger::pow(uint32 n) {
	BigInteger ret;
	ret.data[0] = 1;
	BigInteger t = (*this);
	while (n) {
		if (n & 1) {ret *= t, --n;}
		else { t *= t; n >>= 1;}
	}	
	return ret;
}

BigInteger powMod(BigInteger a, uint32 n, const BigInteger &mod) {
	BigInteger ret;
	ret.data[0] = 1;
	BigInteger t = a;
	while (n) {
		if (n & 1) { (ret *= t) %= mod, --n;}
		else { (t *= t ) %= mod; n >>= 1;}
	}	
	return ret;	
}
