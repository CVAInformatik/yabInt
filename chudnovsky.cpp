
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

/* 
   highly specialised SquareRoot function for this application
   not a general solutin !!
   
   we are using the fact that we know the argument is
   10005 followed by a lot of zeroes 
   
   it means, broadly the first digits in the Squareroot are either
   
      10002 if the numbers of zeroes after the 5 is even
      31630 if the numbers of zeroes after the 5 is odd
   
   Actually in our code the number of zeroes is alwayse even,
   so I don't implement the odd version
   parameter 'one' is a power of ten, and we kan find the power based on the number
   of trailing zeroes (because 10^P = (2x5)^P = 2^P x 5^P )
   ( by counting how many times we can right shift 'one' to the right)
   
*/

yabIntType IntSquareRoot1( const yabIntType &n, const yabIntType &one )
{
   std::vector<baseType>  one_bits = yabIntPeek(one);
   yabIntType temp_one(one)	;
   int power = 0 ;
   while(temp_one.isEven())	 {
   	temp_one >>= 1; power++ ;
   		power++;
   		temp_one >>= 1;
   }
   yabIntType candidate;
   unsigned int bias = 0;
   yabIntType candidate0("10002499687578100594479218787636");
   yabIntType candidate1("1000249968757810059447921878763577780015950243686963146571355115696509678538643042923111879484999732");
   if (power < 4000 ){
   	  candidate = candidate0;
   	  bias = 29;
   	}
   else {
   	  candidate = candidate1;
   	  bias = 97;
   	}
   //std::cout << "power " << power << std::endl;
   for(int i = 0 ; i < ((power/2)-bias) ; i++){
   	  yabIntType t1 = candidate;
   	  yabIntType t2 = candidate;
   	  t1 <<= 1;
   	  t2 <<= 3;
   	  candidate = t1;
   	  candidate += t2;
   }	
   yabIntType  tempCand(candidate);
   //std::cout << "candidate " << iToA(tempCand) << std::endl;
   tempCand *= candidate;
   //std::cout << "candidate * candidate " << iToA(tempCand) << std::endl;
   //std::cout << " n                    " << iToA(n) << std::endl;
   /* now we have a 'good'  starting value  in candidate */
//#define NEWTONRAPHSON
//#define BINARYSEARCHSQUAREROOT
#define INVERSESQUAREROOT

#ifdef NEWTONRAPHSON   
    std::cout << "NEWTONRAPHSON " << std::endl;
   	yabIntType dummy;
	  yabIntType x;
	  yabIntType n_one(n);
	  x = candidate;
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
#endif

#ifdef BINARYSEARCHSQUAREROOT
    std::cout << "BINARYSEARCHSQUAREROOT " << std::endl;
	  yabIntType n_one(n); n_one *= one;
	  yabIntType x((int)1); x <<= BitSize(n_one)/2 ;
	  yabIntType ul(x) ;
	  yabIntType ll(x) ;
	  yabIntType t1;
	  ul >>= 4;
	  do{
	  	ul <<= 1;
	  	t1 = ul ;
	  	t1 *= ul;
	  }	  
	  while( t1 < n_one);

	  ll <<= 4;
	  do{
	  	 ll >>= 1;
	  	 t1 = ll ;
	   	 t1 *= ll;
	  }	  
	  while( t1 > n_one);
  		
	  yabIntType temp;
	  yabIntType test;
	  yabIntType oldtest;
	  while(1){
    	  temp = ll ;
	  	  temp += ul; 
	  	  temp >>= 1;
	  	  test = temp;	  	  
	  	  test *= temp ;
	      if( test < n_one )  ll = temp;
	      else if ( test > n_one ) ul = temp;
        if( oldtest == test ) break;
        oldtest = test;	
	  }	
    return temp ;
#endif
#ifdef  INVERSESQUAREROOT
    std::cout << "INVERSERSQUAREROOT " << std::endl;
	  yabIntType n_one(n); n_one *= one;
	  yabIntType x((int)1); x <<= BitSize(n_one)/2 ;
	  yabIntType ul(x) ;
	  yabIntType t;
	  ul >>= 4;
	  do{
	  	ul <<= 1;
	  	t = ul ;
	  	t *= ul;
	  }	  
	  while( t < n_one);
    // ul  is a guess for squareroot n_one
    yabIntType C((int) 1) ;
    unsigned int c = BitSize( n_one) * 2 ;
    C <<= c ;
    yabIntType newX((int) 0 ), oldX, dummy;
    yabIntType threeCC((int) 3);
    threeCC <<= c; 
    threeCC <<= c; 
    // threeCC is 3*(C**2)    
    DivRem(C,ul, oldX, dummy );
    // oldX is an approximation to C/squareroot(n_one)
    yabIntType t1,t2 ; // a work variable
    while (1) {
    	t1 = oldX ;
    	t1 *= oldX;
    	t1 *= n_one;
    	t1.ChangeSign();
    	t1 += threeCC;
    	t1 *= oldX ;
    	newX = t1 ;
    	newX >>= 1;
    	newX >>= c;	
    	newX >>= c;
    	if( newX == oldX ) break ;
    	oldX = newX;	
    }
    yabIntType res(n_one);
    res *= oldX ;// res =  n_one * C/squareroot(n_one)    
    res >>= c ;
    return res;	
#endif
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
#define R 120
	std::cout << " R "  << R << std::endl ;
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
	
	//std::cout << " t9 " << iToA(t9)<< std::endl ;
	std::cout << "IntSquareRoot begin" << std::endl;
	yabIntType t10 = IntSquareRoot1( t9, one);
	std::cout << "IntSquareRoot done" << std::endl;
	//std::cout << " squareroot t9 " << iToA(t10) << std::endl;	
	//return;
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
#define R1 100
  for( int i = 0 ; i < R1; i++) one *= _one;

  yabIntType val("10005");
  val *= one;
  yabIntType res;
  res = IntSquareRoot1(val, one);
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
