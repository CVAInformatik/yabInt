
#pragma once

#include <string>
#include <iostream>
#include "y.h"

/*
    An experiment in making a unlimited precision floating point type,
    based on yabIntType .
*/
class yabFloatType;

/*
    the digits paramater control number of digits after the decimal point
    AND the format, 
    digits = n  ( n >= 0)  gives from 0 to n decimal digits
    digits = -n gives always n decimal digits, with 0 padding if necessary
    
    Rounding is "towards nearest integer"   
    
        5.76 rounded to 1 digit after the decimal point becomes 5.8
        5.75 rounded to 1 digit after the decimal point becomes 5.8
        5.74 rounded to 1 digit after the decimal point becomes 5.7
        -5.76 rounded to 1 digit after the decimal point becomes -5.8
        -5.75 rounded to 1 digit after the decimal point becomes -5.8
        -5.74 rounded to 1 digit after the decimal point becomes -5.7
     
		(No particular reason for this choice, apart from convenience.)
    
*/
std::string fToA(const yabFloatType &a, int digits = 10 ) ; 
yabFloatType  reciprocal(const yabFloatType& f);
yabFloatType  reciprocalSquareRoot(const yabFloatType& f);
yabFloatType  SquareRoot(const yabFloatType& f);

class yabFloatType
{
public:
	
	friend int Compare( const std::vector<baseType> &a, const std::vector<baseType> &b);

	yabFloatType(){ val = 0 ; a = 0 ;};

  /* format {+|-}nnnnnn.nnnnn  for now */
 	yabFloatType(const char *s, unsigned int pf = 1 );

	yabFloatType( const yabFloatType &f){ 		 
		 val = f.val;
		 a   = f.a ;
		 };	

   /* constructing a float from components */
	yabFloatType( const yabIntType &i, unsigned long long _a){ 		 
		 val = i;
		 a   = _a ;
		 };	
		 
	~yabFloatType() {val = 0 ;} ;

	yabFloatType& operator=(const yabFloatType& f){ if (&f == this) return *this;
		                                       	else {
		                                       		val = f.val ; 
		                                       		a   = f.a;
		                                       		return *this;
		                                       	};
																					}
																					
	yabFloatType& operator+=( const yabFloatType& f){
		      yabIntType t ; 
		      if( a < f.a ) { //
				      t =  val;		      
		      	  t <<= (f.a - a);
		      	  t += f.val;
		      	  a =   f.a;
		      	}
		      else if( f.a < a ) {
				      t =  f.val;		      
		      	  t <<= (a - f.a);
		      	  t += val;
		      	}
		      else { 
		      	t = val; 
		      	t += f.val;
		      } 
      	  val = t ;
      	  return *this;
	};
	
	yabFloatType& operator-=( const yabFloatType& f){
		      yabIntType t ; 
		      if( a < f.a ) { //
				      t =  val;		      
		      	  t <<= (f.a - a);
		      	  t -= f.val;
		      	  a =   f.a;
		      	}
		      else if( f.a < a ) {
				      t =  f.val;		      
		      	  t <<= (a - f.a);
		      	  t.ChangeSign();
		      	  t += val;
		      	}
		      else { 
		      	t = val; 
		      	t -= f.val;
		      } 
      	  val = t ;
      	  return *this;
	};

	yabFloatType& operator*=( const yabFloatType& f) {
		                 val *= f.val;
		                 a += f.a ;
// performance  optimization
//std::cout << " bitsize(val), a  " <<	BitSize(val) << "  " << a << std::endl;
// adjust for performance, it works without, but gets slow for som
#define MAXMULT 4000    	
#ifdef MAXMULT
                       	{
                       		while (( a > MAXMULT) && (BitSize(val) > MAXMULT/2)){
                       			{
                       			    val >>= yabIntType::DIGITSIZE ;
                       			    a    -= yabIntType::DIGITSIZE ;
                       			}
                       		}
                       	}
#endif
		                 return *this;
	};

	yabFloatType& operator/=( const yabFloatType& f) {
		                 yabFloatType rec(reciprocal(f));		                 
		                 val *= rec.val;
		                 a += rec.a ;
		                 return *this;
	};

  void ChangeSign(){ 
		val.ChangeSign(); 
	};
	
	inline bool  isZero()     const  { return val.isZero(); } 
  inline bool  isNegative() const  { return val.isNegative(); }
  inline bool  isPositive() const  { return val.isPositive(); }

		
	friend std::string fToA(const yabFloatType &a, int digits ) ; 
	friend int yabFloatCompare(const yabFloatType &a, const yabFloatType &b );
	friend yabFloatType  guess(const yabFloatType& a);
	friend yabFloatType  Wish(const yabFloatType& a);
	
private:
	
	yabIntType val;
	unsigned long long a ;
};
