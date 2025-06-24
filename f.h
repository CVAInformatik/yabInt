
#pragma once

#include <string>
#include <iostream>
#include "y.h"

/*
    An experiment in making a unlimited precision floating point type,
    based on yabIntType .
*/
class yabFloatType;


std::string fToA(const yabFloatType &a ) ; 

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
	
	friend std::string fToA(const yabFloatType &a ) ; 
		
private:
	
	yabIntType val;
	unsigned long long a ;
};



