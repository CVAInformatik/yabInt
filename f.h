
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
		 
	~yabFloatType() {val = 0 ;} ;

	yabFloatType& operator=(yabFloatType& f){ if (&f == this) return *this;
		                                       	else {
		                                       		val = f.val ; 
		                                       		a   = f.a;
		                                       		return *this;
		                                       	};
																					}
																					
	yabFloatType& operator+=( const yabFloatType& f){
		      yabIntType t(f.val);		      
		      if( a < f.a ) {
		      	  val <<= (f.a - a);
		      	  a =   f.a;
		      	};
		      if( f.a < a ) {
		      	  val <<= (a - f.a);
		      	};
      	  val += t ;
      	  return *this;
	};
	
	yabFloatType& operator-=( const yabFloatType& f){
		      yabIntType t(f.val);		      
		      if( a < f.a ) {
		      	  val <<= (f.a - a);
		      	  a =   f.a;
		      	};
		      if( f.a < a ) {
		      	  val <<= (a - f.a);
		      	};
      	  val -= t ;
      	  return *this;
	};

	yabFloatType& operator*=( const yabFloatType& f) {
		                 val *= f.val;
		                 a += f.a ;
		                 return *this;
	};
	
	friend std::string fToA(const yabFloatType &a, int digits ) ; 
		
private:
	
	yabIntType val;
	unsigned long long a ;
};



