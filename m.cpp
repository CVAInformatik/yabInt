
#include "y.h"
#include "m.h"

	yabIntType montType::toYabInt() const
	{
		yabIntType temp(val);
		//std::cout << " temp " << iToA(temp) << std::endl;
	  yabIntType quotient;
	  yabIntType res;
		temp *= Rinv;
	  DivRem(temp, N, quotient, res);
	  return res;
	}
	
 	montType   &montType::fromYabInt(const yabIntType &a){
 		  yabIntType quotient;
 		  yabIntType temp(a);
 		  val = 0;
 		  temp <<= ((Rdigits - 1) * yabIntType::DIGITSIZE) ;
 		  DivRem(temp, N, quotient, val);
 		  return *this;
 	}

	void REDC(  const yabIntType &R, const yabIntType &N, const yabIntType &Ninv,yabIntType &a){
		yabIntType m(a);
		//std::cout << "REDC m " << iToA(m ) << std::endl;
		/* m mod R */ 
		while( m.yabInt.size() > (R.yabInt.size()-1)) m.yabInt.pop_back();
		while( m.yabInt.size() && (m.yabInt.back() == 0)) m.yabInt.pop_back();
		m *= Ninv;
		//std::cout << "REDC m*Ninv " << iToA(m ) << std::endl;
		while( m.yabInt.size() > (R.yabInt.size()- 1) ) m.yabInt.pop_back();
		while( m.yabInt.size() && (m.yabInt.back() == 0)) m.yabInt.pop_back();
		//std::cout << "REDC (m*Ninv) mod R " << iToA(m ) << std::endl;
		yabIntType t = N;
		t *= m;
		t += a;
    a.yabInt.clear();
    for(int i = R.yabInt.size()-1; i < t.yabInt.size(); i++)		
      a.yabInt.push_back(t.yabInt[i]);
		while( a.yabInt.size() && (a.yabInt.back() == 0)) a.yabInt.pop_back();
		//std::cout << "REDC a " << iToA(a ) << std::endl;
		//std::cout << "REDC N " << iToA(N ) << std::endl;
		if( N == a) a = 0;
		else if ( N< a) a -= N;
		//std::cout << "REDC a " << iToA(a ) << std::endl;
	}

std::string iToA(const montType &a ) {
	  yabIntType t = a.toYabInt();
	  return iToA( t);
}


