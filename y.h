/*
		 yet another big Integer:    yabInt
*/

#pragma once
#include <climits>
#include <ctype.h>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <random>

typedef int baseType;

// the digits are 2s complement integers in the openrange ]-MODULUS...MODULUS[
// this range must leave a one-bit head room, at least, but gives other advantages.

#define MOD  30
#define MODULUS (1 << MOD )


class  yabIntType;
	
const yabIntType Exp( const yabIntType &x, const yabIntType &y );
const yabIntType Exp(const yabIntType &a, unsigned long long exponent);
void  destBiPolarSub( std::vector<baseType> &a, const std::vector<baseType> &b);
void  destUniPolarAdd( std::vector<baseType> &a, const std::vector<baseType> &b);
void  destBiPolarAdd( std::vector<baseType> &a, const std::vector<baseType> &b);
void  destBiPolarMul( std::vector<baseType> &a, const std::vector<baseType> &b);	
void  normalize(std::vector<baseType> &a);
void  mul10e9(std::vector<baseType> &a);
void  mul10e3(std::vector<baseType> &a);
void  internalDouble(std::vector<baseType> &a);			
void  changeSign(std::vector<baseType> &a);
void  dump(const std::vector<baseType> &a);			
int   Compare(const std::vector<baseType> &a, const std::vector<baseType> &b);		
void  internalDivPow2( unsigned int p, std::vector<baseType> &a) ;
void  internalMulPow2( unsigned int p, std::vector<baseType> &a) ;
std::string fastItoA( yabIntType Int);
std::string iToA(const yabIntType &a) ;
void _GCD(const yabIntType &a, const yabIntType &b, yabIntType &gcd);	
 
class yabIntType 
{

	public:
		static const baseType UL = MODULUS;
		static const baseType LL = (-MODULUS);
		static const baseType DIGITSIZE = MOD;
		
		yabIntType() {yabInt.clear();};
		yabIntType(const std::vector<baseType> &a){ yabInt= a;}
		yabIntType(const yabIntType &a) {yabInt.clear(); yabInt = a.yabInt ;};
		~yabIntType() {yabInt.clear();};  
			
		yabIntType(const char *c);
		yabIntType(const long long i);
		yabIntType(const std::string s): yabIntType(s.c_str()){};

 		inline void  Double() { internalDouble( yabInt);};
 		inline void  DivPow2( unsigned int p) { internalDivPow2( p,yabInt);};
 		inline void  MulPow2( unsigned int p) { internalMulPow2( p,yabInt);};
 		inline yabIntType& operator>>=( unsigned int p) { internalDivPow2( p,yabInt);return *this;};
 		inline yabIntType& operator<<=( unsigned int p) { internalMulPow2( p,yabInt);return *this;};
 		
		inline void  ChangeSign() {changeSign(yabInt);};
 		inline yabIntType& operator*=( const yabIntType &b ){destBiPolarMul(yabInt, b.yabInt); return *this; };
 		inline yabIntType& operator+=( const yabIntType &b ){destBiPolarAdd(yabInt, b.yabInt); return *this; };
 		inline yabIntType& operator-=( const yabIntType &b ){destBiPolarSub(yabInt, b.yabInt); return *this; };
		inline yabIntType&  operator=( const yabIntType &a) { if(!( &a == this)) yabInt = a.yabInt; return *this; };
		yabIntType&  operator=( long long  i);
		yabIntType&  operator++();
		yabIntType&  operator--();
		    	
		bool  operator==( const yabIntType &b) const { return 0 == Compare(yabInt, b.yabInt); }
		bool  operator!=( const yabIntType &b) const { return 0 != Compare(yabInt, b.yabInt); }
		bool  operator>( const yabIntType &b) const {  return 0 < Compare(yabInt, b.yabInt); }
		bool  operator<( const yabIntType &b) const {  return 0 > Compare(yabInt, b.yabInt); }
		int   compare( const yabIntType &b) const   { return Compare(yabInt, b.yabInt); }
    inline bool  isZero() const  { return yabInt.size() == 0; } 
    inline bool  isOne() const  { return ( yabInt.size() == 1) && (yabInt[0]==1); } 
    inline bool  isNegative() const  { return (yabInt.size() != 0) && (yabInt.back() < 0 ); }
    inline bool  isPositive() const  { return (yabInt.size() != 0) && (yabInt.back() > 0 ); }
    inline bool  isEven() const { return ( yabInt.size() == 0) || ((yabInt[0] & 1) == 0);}
    
		friend std::string _iToA(const yabIntType &a) ;
                friend void Mersenne(const unsigned int N, yabIntType &a);
 		friend void DivRem(const yabIntType &a, const yabIntType &b, yabIntType &Qoutient, yabIntType &Rem );
		friend void extendedGCD(const yabIntType &a, const yabIntType &b, yabIntType &gcd, yabIntType &am, yabIntType &bm  );
 		friend void GCD(const yabIntType &a, const yabIntType &b, yabIntType &gcd);
		friend int Jacobi(const yabIntType& a, const yabIntType& b);	
		friend yabIntType Mersenne(unsigned int N);
		friend std::vector<baseType>  yabIntPeek(const yabIntType& a);
		friend void  yabIntPoke(std::vector<baseType> s, yabIntType& a);
		/* Montgomery related friends */
		friend void REDC(  const yabIntType &R, const yabIntType &N, const yabIntType &Ninv, yabIntType &a);
                friend int  BitSize(const yabIntType &a);
			
	private:

		std::vector<baseType> yabInt;
	
};





