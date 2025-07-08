/*
		 yet another big Integer:    yabInt
*/

#include <ctype.h>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <random>

typedef int baseType;

// the digits are 2s complement integers in the openrange ]-MODULUS...MODULUS[
// this range must leave a one-bit head room at least, but gives a lot of other advantages.

#define MODULUS 0x40000000

class  yabIntType;
/*
     returns Qout and Rem, such that b = ( a * Quot) + Rem, Rem >= 0
*/
/* friend */
int Compare( const std::vector<baseType> &a, const std::vector<baseType> &b);
void DivRem(const yabIntType &a, const yabIntType &b,yabIntType &Qoutient, yabIntType &Rem );
void extendedGCD(const yabIntType &a, const yabIntType &b, yabIntType &gcd, yabIntType &am, yabIntType &bm  );
void GCD(const yabIntType &a, const yabIntType &b, yabIntType &gcd);
int Jacobi(const yabIntType& a, const yabIntType& b);
void Mersenne(const unsigned int N, yabIntType &a);
/* Montgomery related friends */
yabIntType mont_R( const yabIntType &a);
void REDC(  const yabIntType &R, const yabIntType &N, const yabIntType &Ninv, yabIntType &a);

/* not a friend  */
const yabIntType Exp(const yabIntType &a, unsigned long long exponent);
void  destBiPolarSub( std::vector<baseType> &a, const std::vector<baseType> &b);
void  destUniPolarAdd( std::vector<baseType> &a, const std::vector<baseType> &b);
void  destBiPolarAdd( std::vector<baseType> &a, const std::vector<baseType> &b);
void  destBiPolarMul( std::vector<baseType> &a, const std::vector<baseType> &b);	
void  normalize(std::vector<baseType> &a);
void mul10e9(std::vector<baseType> &a);
void mul10e3(std::vector<baseType> &a);
void internalDouble(std::vector<baseType> &a);			
void changeSign(std::vector<baseType> &a);
void dump(const std::vector<baseType> &a);			
		
class yabIntType 
{

	public:
		static const baseType UL = MODULUS;
		static const baseType LL = (-MODULUS);
		
		yabIntType() {yabInt.clear();};
		yabIntType(const std::vector<baseType> &a){ yabInt= a;}
		yabIntType(const yabIntType &a) {yabInt.clear(); yabInt = a.yabInt ;};
		~yabIntType() {yabInt.clear();};  
			
		yabIntType(const char *c);
		yabIntType(const long long i);

 		inline void  Double() { internalDouble( yabInt);};
 		inline void  DivPow2( unsigned int p) { internalDivPow2( p,yabInt);};
 		inline void  MulPow2( unsigned int p) { internalMulPow2( p,yabInt);};
 		
		inline void  ChangeSign() {changeSign(yabInt);};
 		inline yabIntType& operator*=( const yabIntType &b ){destBiPolarMul(yabInt, b.yabInt); return *this; };
 		inline yabIntType& operator+=( const yabIntType &b ){destBiPolarAdd(yabInt, b.yabInt); return *this; };
 		inline yabIntType& operator-=( const yabIntType &b ){destBiPolarSub(yabInt, b.yabInt); return *this; };
		inline yabIntType&  operator=( const yabIntType &a) { if(!( &a == this)) yabInt = a.yabInt; return *this; };
		yabIntType&  operator++();
    inline baseType operator[](unsigned int index) {if (index < yabInt.size() ) return yabInt[index]; else return UL;};
		bool  operator==( const yabIntType &b) { return 0 == Compare(yabInt, b.yabInt); }
		bool  operator>( const yabIntType &b) {  return 0 < Compare(yabInt, b.yabInt); }
		bool  operator<( const yabIntType &b) {  return 0 > Compare(yabInt, b.yabInt); }
		int   compare( const yabIntType &b) const   { return Compare(yabInt, b.yabInt); }
    inline int   bitSize() { return internalBitSize(yabInt);  }
    inline int   size() const { return yabInt.size() ;  }
    inline bool  isZero() const  { return yabInt.size() == 0; } 
    inline bool  isOne() const  { return ( yabInt.size() == 1) && (yabInt[0]==1); } 
    inline bool  isNegative() const  { return (yabInt.size() != 0) && (yabInt.back() < 0 ); }
    inline bool  isPositive() const  { return (yabInt.size() != 0) && (yabInt.back() > 0 ); }
    
		friend char *iToA(const yabIntType &a) ;
    friend void Mersenne(const unsigned int N, yabIntType &a);
 		friend void DivRem(const yabIntType &a, const yabIntType &b, yabIntType &Qoutient, yabIntType &Rem );
		friend void extendedGCD(const yabIntType &a, const yabIntType &b, yabIntType &gcd, yabIntType &am, yabIntType &bm  );
 		friend void GCD(const yabIntType &a, const yabIntType &b, yabIntType &gcd);
		friend int Jacobi(const yabIntType& a, const yabIntType& b);	
		friend yabIntType Mersenne(unsigned int N);
		friend yabIntType mont_R( const yabIntType &a);
		friend void REDC( const yabIntType &R, const yabIntType &N, const yabIntType &Ninv, yabIntType &a);
		

  protected: 
		std::vector<baseType> yabInt;
			
	private:
 		/* private stuff */		
    inline int   internalBitSize(const std::vector<baseType> &a) ;
		void internalDivPow2( unsigned int p, std::vector<baseType> &a) ;
		void internalMulPow2( unsigned int p, std::vector<baseType> &a) ;
	
};


yabIntType  modpow(const yabIntType & _a, const yabIntType & _b, const yabIntType & mod);  // Compute a^b % mod
yabIntType  modmult(const yabIntType &_a, const yabIntType &_b, const yabIntType &mod);  // Compute a*b % mod

class RandomYabIntType
{
public:
	RandomYabIntType();
	~RandomYabIntType();
	yabIntType rand(yabIntType &a);
	
private:
	
	unsigned int  _Rand(); // 
	std::random_device rd;
	std::uniform_int_distribution<unsigned int>* dist;

};

class m1Type : public yabIntType {
	
public:
		   m1Type(const yabIntType &N);
		   m1Type(const m1Type  &T);
		   
		   m1Type operator=(const m1Type a);
		   m1Type operator=(const yabIntType a);
   		 inline m1Type& operator*=( const m1Type &b ){destBiPolarMul(yabInt, b.yabInt); return *this; };
 		   inline m1Type& operator+=( const m1Type &b ){destBiPolarAdd(yabInt, b.yabInt); return *this; };
 		   inline m1Type& operator-=( const m1Type &b ){destBiPolarSub(yabInt, b.yabInt); return *this; };

		   yabIntType toYabIntType();
};