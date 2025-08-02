
#include "f.h"

yabIntType guess( const yabIntType &n)
{
	  std::vector<baseType> t = yabIntPeek(n);
	  yabIntType res (n)	;
	  if(t.size() == 1 ) {
	  	 t[0] = (int) sqrt((double ) t[0]);
	  	 res = yabIntType(t);
	  }
	  else {
	  	  unsigned int sz = BitSize( res);

	  	  std::cout << "sz " << sz << " sz%30 " << (sz%30)   << std::endl;

	  	  t.clear();
	  	  for(int i = 0; i < sz/60; i++){
	  	  	t.push_back(0);
	  	  }
	  	  t.push_back(1);
	  	  res = yabIntType(t);
	  	  for(int i = 0; (i) < (sz%30) ; i++){
	  	  	res <<= 1;
	  	  }	  	
	  	  std::cout << "sz " << sz << " bitsize(res) " << BitSize(res)   << std::endl;
    }
	  return res;	
		
}
/* SLOW ! */
yabIntType IntSquareRoot( const yabIntType &n, const yabIntType &one )
{
	  yabIntType dummy;
	  yabIntType x;
	  yabIntType n_one(n);
	  x = guess(n);
	  n_one *= one; 
	  while(1) {
	  	yabIntType x_old = x;
		  yabIntType t(n_one);
	  	DivRem( t, x_old, t, dummy);
		  x += t;
	  	x >>= 1;
	  	if( x == x_old) break;
	  }
	  return x;

}


yabIntType guess1( const yabIntType &n,const yabIntType &one )
{
        yabIntType t(n);
        yabIntType res;
        // here we do some guessing
        t *= one;
        std::vector<baseType> temp = yabIntPeek(n)	;
	      int bitsize = BitSize(temp);

	  return res;	
}

yabIntType IntReciprocalSquareRoot1( const yabIntType &n, const yabIntType &one )
{
	  yabIntType dummy;
	  yabIntType x;
	  yabIntType n_one(n);
	  yabIntType three_one((int)3);
	  x = guess(n);
	  n_one *= one; 
	  three_one *= one;
	  while(1) {
	  	yabIntType x_old = x;
		  yabIntType t(n_one);
		  t *= x_old ;
		  t *= x_old ;
		  x = three_one;
		  x  -= t ;
		  x  *= x_old;
	  	x >>= 1;
	  	if( x == x_old) break;
	  }
	  return x;

}

yabIntType IntSquareRoot1( const yabIntType &n, const yabIntType &one )
{
	  yabIntType n_one(n);
	  n_one *= one; 
	 yabFloatType nf( n_one,0);
	 std::cout << "IntSquareRoot1 " <<  fToA(nf, 100000)<< std::endl;
   yabFloatType res = SquareRoot(nf);
   	return res.fToYabInt();
}


void Chudnovsky()
{
	yabIntType k((int) 1);
	yabIntType _one("10000000000" "0000000000"
	                "0000000000" "0000000000"
	                "0000000000" "0000000000"
	                "0000000000" "0000000000"
	                "0000000000" "0000000000"
	            );
  yabIntType one ((int) 1);
#define R 21
  for( int i = 0 ; i < R; i++) one *= _one;
	yabIntType a_k(one);
	yabIntType a_sum(one);
	yabIntType b_sum((int) 0);
	yabIntType c("640320");
	yabIntType c3(c);
	c3 *= c;
	c3 *= c;

	yabIntType c3_over_24 ;
	yabIntType dummy;
	DivRem(c3,yabIntType((int)24), c3_over_24, dummy) ;
	//std::cout << "c3_over_24  "  << iToA(c3_over_24 )<< std::endl ;
	while (1){
		yabIntType t1(k); t1 *= 6 ;
		yabIntType t2(k); t2 *= 2 ;
		yabIntType t3(t1); 
		t1 -= 5;  t2 -= 1; t3 -= 1; ;
		
		a_k *= t1 ; 
		a_k *= t2 ; 
		a_k *= t3 ; 
		a_k.ChangeSign();
		
		yabIntType t4(k); 
		t4 *= k; t4 *= k; t4 *=c3_over_24;
		
		if(a_k.isNegative()) {
			a_k.ChangeSign();
			DivRem( a_k, t4, a_k, dummy);
			a_k.ChangeSign();
		}else
			DivRem( a_k, t4, a_k, dummy);
		
		a_sum += a_k;
		
		yabIntType t5(a_k); t5 *= k;
		b_sum += t5;
		
		++k;
		
		if (a_k.isZero()) break;			
	}
	yabIntType total ;
	yabIntType c6("13591409");
	yabIntType t6(a_sum); t6 *= c6;
	yabIntType c7("545140134");
	yabIntType t7(b_sum); t7 *= c7;
	total += t6;
	total += t7;
	yabIntType t8("426880");
	yabIntType t9("10005");
	t9 *= one;
	std::cout << " t9 " << iToA(t9)<< std::endl ;
	std::cout << "IntSquareRoot begin" << std::endl;
	yabIntType t10 = IntSquareRoot( t9, one);
	std::cout << "IntSquareRoot done" << std::endl;
	t10 *= t8;
	t10 *= one;
	yabIntType pi;
	DivRem( t10, total, pi, dummy);
	std::cout << " pi " << iToA(pi)<< std::endl ;
		
}

void TestSquareRoots()
{
	yabIntType _one("10000000000" "0000000000"
	                 "0000000000" "0000000000"
	                 "0000000000" "0000000000"
	                 "0000000000" "0000000000"
	                 "0000000000" "0000000000"
	            );
  yabIntType one ((int) 1);
#define R1 18
  for( int i = 0 ; i < R1; i++) one *= _one;

  yabIntType val("10005");
  val *= one;
  yabIntType res;
  res = IntSquareRoot(val, one);
  //res = IntSquareRoot1(val, one);
  yabIntType test(res);
  test *= res ;
  std::cout << "one " << iToA(one)<< std::endl;
  std::cout << "val " << iToA(val)<< std::endl;
  std::cout << "res " << iToA(res)<< std::endl;
  std::cout << "test " << iToA(test)<< std::endl;
  yabIntType test1	;
  yabIntType dummy;
  DivRem(test, one, test1, dummy);
  std::cout << "test1 " << iToA(test1)<< std::endl;
  	
}
