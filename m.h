

#pragma once

#include <iostream>
#include "y.h"



class montType
{
public:
	
	friend int Compare( const std::vector<baseType> &a, const std::vector<baseType> &b);

	montType( const yabIntType &_N)
	{
		 val = 0 ;
		 N = _N;
		 R =  1; Rdigits = 1;
  	 while ( N > R) { 
   	 	R<<= yabIntType::DIGITSIZE; 
  	 	Rdigits++;
  	 	}
		 yabIntType gcd ;

		 extendedGCD(R, N, gcd, Rinv, Ninv);
/*
		std::cout << "1 montType()  : N " << iToA(N) << std::endl;
		std::cout << "2 montType()  : R " << iToA(R) << std::endl;
		std::cout << "2 montType()  : Rdigits " << Rdigits << std::endl;
		std::cout << "3 montType()  : Rinv " << iToA(Rinv) << std::endl;
		std::cout << "4 montType()  : Ninv " << iToA(Ninv) << std::endl;
		std::cout << "5 montType()  : gcd " << iToA(gcd) << std::endl;
*/
		 if( Rinv.isNegative()) Rinv  += N;
		 //if( Ninv.isNegative() ) Ninv += R;
     Ninv.ChangeSign();
/*
		std::cout << "11 montType()  : N " << iToA(N) << std::endl;
		std::cout << "22 montType()  : R " << iToA(R) << std::endl;
		std::cout << "22 montType()  : Rdigits " << Rdigits << std::endl;
		std::cout << "33 montType()  : Rinv " << iToA(Rinv) << std::endl;
		std::cout << "44 montType()  : Ninv " << iToA(Ninv) << std::endl;
*/    
    yabIntType t1(N) ;
    t1 *= Ninv;
    yabIntType t2(R) ;
    t2 *= Rinv;
    yabIntType t3(t2);
    t3 -= t1 ;
    std::cout << " R x Rinv : " << iToA(t2) << std::endl;
    std::cout << " N x Ninv : " << iToA(t1) << std::endl;
    std::cout << " (R x Rinv)-(N x Ninv): " << iToA(t3) << std::endl;
    	
		 if ( !gcd.isOne()) 
		 	std::cout << " R and N not prime !! R: " 
		 		      <<  iToA(R) << " N: " << iToA(N) << std::endl;		 		 
	} 

	montType( const montType &m){ 		 
		 val = m.val;
		 N = m.N; 
		 R = m.R;
		 Rinv = m.Rinv;
		 Rdigits = m.Rdigits;
		 Ninv = m.Ninv;
		 };	
		 
	~montType() {} ;

	montType& operator=(montType& b){ if (&b == this) return *this;
		                                       else {
		                                       	val = b.val ; 
		                                       	N = b.N;
		                                       	R = b.R;
		                                       	Rdigits = b.Rdigits;
		                                       	Ninv = b.Ninv;
		                                       	Rinv = b.Rinv;
		                                       	return *this;};
																					 }
	montType& operator+=( const montType& b){ val += b.val ;
		                                        if(N == val) val = 0;
		                                        if(N < val) val -= N;
		                                        return *this;};
	montType& operator-=( const montType& b){ val -= b.val ;
		                                        if(val.isNegative()) val += N ;
		                                        return *this;};
	montType& operator*=( const montType& b){ val *= b.val ; 
				//std::cout << "1 operator *=  : val " << iToA(val) << std::endl;
																					  REDC(R, N, Ninv, val);
				//std::cout << "2 operator *=  : val " << iToA(val) << std::endl;
																						return *this;};
	bool  operator==( 	const montType& b){
		  	return (val == b.val) && (R ==b.R) && (N == b.N);
	}																				
	
	bool  operator!=( 	const montType& b){
		  	return (val != b.val) || (R != b.R) || (N != b.N);
	}																				
	
	yabIntType toYabInt() const ;
 	montType   &fromYabInt(const yabIntType &a);
 	
	
		
private:
	
	yabIntType val;
	yabIntType N ;
	yabIntType R ;
	unsigned int Rdigits;
	yabIntType Ninv ;
	yabIntType Rinv ;
};


std::string  iToA(const montType &a ) ; 
