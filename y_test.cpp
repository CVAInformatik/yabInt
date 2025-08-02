
#include <cmath>
#include <iostream>
#include "y.h"
#include "m.h"
#include "f.h"

#include <time.h>

#include "chudnovsky.cpp"

 void testMersenneAux(unsigned int x)
{
    time_t now;
    yabIntType test("10");
		Mersenne(x, test);
	  
    
    now = time(NULL);
    printf("start: %s ", ctime(&now));

	  std::string c  = iToA(test);
	  printf("test.Mersenne(%d) %ld  %s\n", x, strlen(c.c_str()), c.c_str());
	  now = time(NULL);
	  printf("end: %s", ctime(&now));
/*
	
		yabIntType quotient = test;
		yabIntType remainder;
		yabIntType divisor("1000000000000000000000000000000000000000000000000000000000000"
												"000000000000000000000000000000"
												"000000000000000000000000000000"
												"000000000000000000000000000000"
												//"000000000000000000000000000000"
		                   );
    now = time(NULL);
    printf("start: %s", ctime(&now));
		do{
			yabIntType t = quotient;			
			DivRem(t, divisor, quotient, remainder);
		  std::string c  = iToA(remainder);
		  printf("test.Mersenne(%d) %ld  %s\n", x, strlen(c.c_str()), c.c_str());
		}
		while (!quotient.isZero()); 
	  now = time(NULL);
	  printf("end: %s", ctime(&now));
		*/	  
} 

void testDivRem()
{
			
		yabIntType b("86189793618632");
		yabIntType c("193719739186189793618632");
		
		yabIntType quot, rem;
		
		DivRem(c,b, quot, rem);
		printf(" c =     %s \n", iToA(c).c_str());
		printf(" b =     %s \n", iToA(b).c_str());
		printf(" qout =  %s \n", iToA(quot).c_str());
		printf(" rem  =  %s \n", iToA(rem).c_str());

    yabIntType check;
    check = b; 
    printf(" check=  %s \n", iToA(check).c_str());
		printf(" check*= quot  %s \n", iToA(quot).c_str());
		check *= quot ; 
		printf(" check=  %s \n", iToA(check).c_str());
		printf(" check+=rem  %s \n", iToA(rem).c_str());
		check += rem;
		printf(" check=  %s \n", iToA(check).c_str());
    printf(" c =     %s \n", iToA(c).c_str());

    printf("******************************\n");
    if( check == c ) printf("test passed\n");
    else printf("test failed\n");
    printf("******************************\n");
}

void testGCD()
{
			
		yabIntType b("86189793618632");
		yabIntType c("193719739186189793618632");
		
	
    yabIntType gcd, ab, ac;
    extendedGCD(b,c,gcd, ab,ac );
    printf(" b =     %s \n", iToA(b).c_str());
    printf(" c =     %s \n", iToA(c).c_str());
    printf(" gcd =   %s \n", iToA(gcd).c_str());
    printf(" ab =     %s \n",iToA(ab).c_str());
    printf(" ac =     %s \n",iToA(ac).c_str());

    yabIntType c1(b );
    printf(" c1 =  b     %s \n", iToA(c1).c_str());
    yabIntType c2(c );
    printf(" c2 = c     %s \n", iToA(c2).c_str());
    
    yabIntType c3((long long) 0 );
    c1 *= ab;
    printf(" c1 *=ab     %s \n", iToA(c1).c_str());
    c2 *= ac;
    printf(" c2 *=ac     %s \n", iToA(c2).c_str());
    c3 = c1;
    c3 += c2;
    printf(" c3 = c1+c2    %s \n", iToA(c3).c_str());
    
    printf("******************************\n");
    if( c3 == gcd ) printf("test passed\n");
    else printf("test failed\n");
    printf("******************************\n");
}


 void testJacobi()
 {
    for (int j = 3; j < 60; j += 2)
        for (int i = 1; i < 31; i++) {
            std::cout << "Jacobi ( ";
            yabIntType cj((long long) j);
            yabIntType ci((long long) i);
            std::cout << iToA(ci) << "/ ";
            std::cout << iToA(cj) << " ) = ";            
            std::cout << Jacobi( ci, cj) << std::endl;
        }
}




yabIntType modmult(const yabIntType &_a, const yabIntType &_b, const yabIntType &mod) {  // Compute a*b % mod
    yabIntType result;// = new pIntClass;
    yabIntType a = _a;
    std::vector<baseType> b = yabIntPeek( _b) ;
    yabIntType quot; 
    yabIntType rem;
    while (b.size()) {

        if ((b[0] & 1) == 1) {
            result += a;
            DivRem(result, mod, quot, rem); result = rem;
        }
        a.Double();
        DivRem(a, mod, quot, rem); 
        a = rem;
        internalDivPow2(1,b);
    }
    return result;
}

yabIntType  modpow(const yabIntType & _a, const yabIntType & _b, const yabIntType & mod) {  // Compute a^b % mod
    yabIntType  result;
    yabIntType   a = _a;
    yabIntType   b = _b;
		std::vector<baseType> braw = yabIntPeek(b);

    yabIntType  t;
  //  yabIntType quot; 
	//  yabIntType rem;

    ++(result);// result = 1;
#if 0
    while (!b.isZero()) {
        if ((b[0] & 1) == 1) {
            t = modmult(result, a, mod);
            result = t;
        }
        a = modmult(a, a, mod);
        b.DivPow2(1);
    }
#else
		baseType bb = 0;
    for(int it = 0; it < (braw.size()-1); it++){
    	baseType ib = 1;
    	bb = braw[it];    	
	    while(ib < yabIntType::UL){
        if ((bb & ib) == 1) {
            t = modmult(result, a, mod);
            result = t;
        }
        a = modmult(a, a, mod);
        ib = ib<<1 ;
			}
    }
   	bb = braw.back();//[braw.size()-1];
   	while (bb){
        if ((bb & 1) == 1) {
            t = modmult(result, a, mod);
            result = t;
        }
        a = modmult(a, a, mod);
        bb = bb>>1;
		}    
#endif

    return result;
}



 
void PollardRho(const char *s) 
{
	std::cout << "PR 1 " <<  s <<  std::endl;
	yabIntType N(s);
	
	std::cout << "PollardRho: " << iToA(N) << std::endl;
	yabIntType g;

#define LIM 20000000	
		

	yabIntType seed("1");
	
  yabIntType nk1("1");
  yabIntType nk2(nk1);
  yabIntType diff;
//#define BRENT
#ifdef BRENT  
#define CL 128
	yabIntType acc((const long long )1) ;
	int counter = 0;
#endif
	for(int i = 0; i < LIM ; i++)	{
		 nk1 *= nk1; //modmult(nk1,nk1, N);
		 nk1 = modmult(nk1,nk1, N);
		 nk1 *= nk1; //modmult(nk1,nk1, N);
		 nk1 = modmult(nk1,nk1, N);
		 nk1 = modmult(nk1,nk1, N);
		 nk1 += 1;
		 nk2 *= nk2; //modmult(nk2,nk2, N);
		 nk2  =  modmult(nk2,nk2, N);
		 nk2 *= nk2; // modmult(nk2,nk2, N);
		 nk2 = modmult(nk2,nk2, N);
		 nk2 = modmult(nk2,nk2, N);
		 nk2 += 1;
		 nk2 *= nk2; //modmult(nk2,nk2, N);
		 nk2 = modmult(nk2,nk2, N);
		 nk2 *= nk2; //modmult(nk2,nk2, N);
		 nk2 = modmult(nk2,nk2, N);
		 nk2 = modmult(nk2,nk2, N);
		 nk2 += 1;
 		 //std::cout << "       PollardRho: nk1  " << iToA(nk1) << "  nk2  " << iToA(nk2) << std::endl;
#ifdef BRENT
     diff = nk2  ;
	   diff  -= nk1;
	   if(diff.isNegative()) diff.ChangeSign();
		 acc = 	modmult( acc, diff, N)	;
		 if(counter++ == CL){
				 //std::cout << "counter " << counter << std::endl	 ;
	   		 GCD( acc, N, g);
		  	 if( !g.isOne() ){
	  	 		std::cout << "       PollardRho: N  " << iToA(N) << std::endl;
	   			std::cout << i << "  PollardRho: gcd  " << iToA(g) << std::endl;
	   			break;
	   		 }
	   		 else{
	   		 	counter = 0;
	   		 	acc = 1;
	   		 }	   		 	
		 }

#else
     diff = nk2  ;
	   diff  -= nk1;
	   if(diff.isNegative()) diff.ChangeSign();
	   GCD( diff, N, g);
  	 if( !g.isOne() ){
	   		std::cout << "       PollardRho: N  " << iToA(N) << std::endl;
	   		std::cout << i << "  PollardRho: gcd  " << iToA(g) << std::endl;
	   		break;
	   }	   
#endif
	}
}


void MontPollardRho(const char *s) 
{
	yabIntType N(s);
	int counter = 0;
	std::cout << "MontPollardRho: " << iToA(N) << std::endl;
	
#define LIM1 2000000000	

	yabIntType seed("1");
	//std::cout << "MontPollardRho  seed: " << iToA(seed) << std::endl;
  montType nk1(N);
	//std::cout << "MontPollardRho  N: " << iToA(nk1.toYabInt()) << std::endl;
  montType nk2(nk1);
	montType one(nk1); 
	one.fromYabInt(seed);
  //std::cout << "       PollardRho: one  " << iToA(one.toYabInt()) << std::endl;
  montType nkdiff(nk1);
  nk1.fromYabInt(seed);
  //std::cout << "       PollardRho: nk1  " << iToA(nk1.toYabInt()) << std::endl;
  nk2 = nk1 ;
  //std::cout << "       PollardRho: nk2  " << iToA(nk2.toYabInt()) << std::endl;
	for(int i = 0; i < LIM1 ; i++)	{
		 nk1 *= nk1;
		 //std::cout << "1       PollardRho: nk1  " << iToA(nk1.toYabInt()) << std::endl;
		 nk1 += one;
		 //std::cout << "2       PollardRho: nk1  " << iToA(nk1.toYabInt()) << std::endl;
		 nk2 *= nk2; 
 		 //std::cout << "3       PollardRho: nk2  " << iToA(nk2.toYabInt()) << std::endl;
		 nk2 += one;
 		 //std::cout << "4       PollardRho: nk2  " << iToA(nk2.toYabInt()) << std::endl;
		 nk2 *= nk2; //modmult(nk2,nk2, N);
 		 //std::cout << "5       PollardRho: nk2  " << iToA(nk2.toYabInt()) << std::endl;
		 nk2 += one;
 		 //std::cout << "6       PollardRho: nk2  " << iToA(nk2.toYabInt()) << std::endl;
		 nkdiff = nk2;
		 nkdiff -= nk1;
 		 //std::cout << "7       PollardRho: nkdiff  " << iToA(nkdiff.toYabInt()) << std::endl;
   	 //std::cout << "       PollardRho: nk1  " << iToA(nk1.toYabInt()) << std::endl;
   	 // std::cout << "       PollardRho: nk2  " << iToA(nk2.toYabInt()) << std::endl;
		 if( 0 == (++counter % 8))
		 	{
		 		counter = 0;
		 		yabIntType diff = nkdiff.toYabInt();
		 		yabIntType g ;
		 		yabIntType b1 ;
		 		yabIntType b2 ;
		 		//extendedGCD(diff, N, g, b1, b2);
		 		GCD( diff, N, g);
  	 		if( !g.isOne() ){
  	 		 std::cout << "*******************************************" << std::endl;
	   		 std::cout << "       PollardRho: N  " << iToA(N) << std::endl;
	   		 std::cout << "       PollardRho: diff  " << iToA(diff) << std::endl;
	   		 std::cout << i << "  PollardRho: g     " << iToA(g) << std::endl;
	   		 yabIntType q;
	   		 yabIntType r;
	   		 DivRem(N, g, q,r);
	   		 std::cout << "  PollardRho: N/g  " << iToA(q)<< " r " << iToA(r) << std::endl;
	   		 DivRem(diff, g, q,r);
	   		 std::cout << "  PollardRho: diff/g  " << iToA(q)<< " r " << iToA(r) << std::endl;
				 std::cout << "*******************************************" << std::endl<< std::endl;	   		

	   		return ;
		 	  }
		 	}
	}

}

/*
yabIntType Exp( const yabIntType &a, const yabIntType &N)
{
	  yabIntType result(1);	
	  std::vector<baseType> n = yabIntPeek(N);

		if (n.size() == 0) return result;

	  yabIntType aa ;
		baseType nn = 0;
		aa = a;
    for(int it = 0; it < (n.size()-1); it++){
    	std::cout << " Exp 1 result " << iToA(result) << std::endl;;
    	baseType in = 1;
    	nn = n[it];    	
	    while(in < yabIntType::UL){
        if ((nn & in) == 1) result *= aa ;        
        aa *= aa;
        in = in<<1 ;
			}
    }
   	nn = n[n.size()-1];
   	while (nn){
	    	std::cout << " Exp 2 nn " << nn << std::endl;;
	    	std::cout << " Exp 3 result " << iToA(result) << std::endl;;
        if ((nn & 1) == 1) result *=aa;        
        aa *= aa;
        nn = nn>>1;
		}    
	  return result;
}
*/
void testExponent()
{
	yabIntType a;
	yabIntType b;
	yabIntType N;	
	yabIntType n;	
	yabIntType result;
	yabIntType witness;
	
	a = 6 ; N = 5 ; result = Exp(a, N);
	std::cout << " a**N; where a " << iToA(a)<< " and N "<< iToA(N) << " = "
		                << iToA(result) << std::endl;
  n = result ;               	
  b = 7  ;            
  witness = Exp(b, n)            ;
  std::cout << " p1" << std::endl;
  witness -= b;
  std::cout << " p2" << std::endl;
  yabIntType g;
  std::cout << " p3" << std::endl;
  GCD(witness, n, g);
  std::cout << " p4" << std::endl;;
	std::cout << " GCD( " << iToA(witness)<< ", " << iToA(n) <<" ) = " <<
		                 iToA(g) << std::endl;
  /*
	N = 101 ; result = Exp(a, N);
	std::cout << " a**N; where a " << iToA(a)<< " and N "<< iToA(N) << " = "
		                << iToA(result) << std::endl;
              	
  result -= a;
  GCD(result, N, g);
	std::cout << " GCD( " << iToA(result)<< ", " << iToA(N) <<" ) = " <<
		                 iToA(g) << std::endl;
  */		                	
}


yabIntType ModularExp( const yabIntType &m, const yabIntType &a, const yabIntType &N)
{
	  yabIntType result(1);	
	  std::vector<baseType> n = yabIntPeek(N);

		if (n.size() == 0) yabIntType((int) 1);

		montType res(m);	 
			std::cout << " Exp 0 res " << iToA(res.toYabInt()) << std::endl;;
    montType aa(res) ; aa.fromYabInt(a) ;
    res.fromYabInt(yabIntType( (int ) 1)) ;
			std::cout << " Exp 0 res " << iToA(res.toYabInt()) << std::endl;;
    
		baseType nn = 0;
    for(int it = 0; it < (n.size()-1); it++){
  /*  	std::cout << " Exp 1 result " << iToA(result) << std::endl;;
			std::cout << " Exp 1 res " << iToA(res.toYabInt()) << std::endl;;
	    std::cout << " Exp 1 aa  " << iToA(aa.toYabInt()) << std::endl;;
   */         		
    	baseType in = 1;
    	nn = n[it];    	
	    while(in < yabIntType::UL){
        if ((nn & in) == 1) res *= aa ;        
        aa *= aa;
	   /* 	std::cout << " Exp 1.1 res " << iToA(res.toYabInt()) << std::endl;;
	    	std::cout << " Exp 1.2 aa  " << iToA(aa.toYabInt()) << std::endl;;
     */   in = in<<1 ;
			}
    }
   	nn = n[n.size()-1];
   	while (nn){
	    	//std::cout << " Exp 2 nn " << nn << std::endl;;
	    	//std::cout << " Exp 3 res " << iToA(res.toYabInt()) << std::endl;;
        if ((nn & 1) == 1) res *=aa;        
        aa *= aa;
        nn = nn>>1;
		}    

	  return res.toYabInt();
}



void testModExponent()
{
	yabIntType a;
	yabIntType b;
	yabIntType N;	
	yabIntType n;	
	yabIntType witness;
	
	
	/*
	yabIntType result;
	a = 7 ;  N = 5 ; result = Exp(a, N);
	std::cout << " a**N; where a " << iToA(a)<< " and N "<< iToA(N) << " = "
		                << iToA(result) << std::endl;
  
  */
	yabIntType result("115792089237316195423570985008687907853269984665640564039457584007913129639937");
  
   //yabIntType result("11579208923731");
   yabIntType  iot ;
   yabIntType  piot ;
   montType    miot(result);
   for(int i = 1 ; i < 16807 ; i++){
   	   
   	   iot = (1 *i);
   	   miot.fromYabInt(iot);
   	   piot = miot.toYabInt();
			 std::cout << " (iot, piot)= ( " << iToA(iot) << ", " << iToA(piot) << " )" << std::endl;
   }
  
  
   
  n = result ;
  n -= 1;               	
  b = 7  ;       
       
  witness = ModularExp(result, b, n)            ;
  
  std::cout << " p1 witness "<< iToA(witness) << std::endl;
  witness -= b;
  std::cout << " p2 witness "<< iToA(witness) << std::endl;
  yabIntType g;
  std::cout << " p3" << std::endl;
  GCD(witness, result, g);
  std::cout << " p4" << std::endl;;
	std::cout << " GCD( " << iToA(witness)<< ", " << iToA(n) <<" ) = " <<
		                 iToA(g) << std::endl;
 
	N = 101 ; result = Exp(a, N);
	std::cout << " a**N; where a " << iToA(a)<< " and N "<< iToA(N) << " = "
		                << iToA(result) << std::endl;
              	
  result -= a;
  GCD(result, N, g);
	std::cout << " GCD( " << iToA(result)<< ", " << iToA(N) <<" ) = " <<
		                 iToA(g) << std::endl;

}

void testMontgomery()
{
#define APRIME  65537
/*
  montType  t(APRIME);
  
  for(int i = 0; i < (APRIME+10); i++){
  	yabIntType a(i);
  	yabIntType b;
  	yabIntType c(i % APRIME);
  	t.fromYabInt(a);
  	b = t.toYabInt();
  	if ( c  != b)
  		std::cout << "error " << iToA(a) << " != "<< iToA(b) << std::endl;
  }
  
  montType  t1(APRIME);
  montType  t2(t1);
  montType  t3(t1);
  for(int i1 = 0; i1 < (APRIME+10); i1 += 3){
  	for(int i2 = i1+1; i2 < (APRIME+10); i2+=7){
  	yabIntType a(i1);
  	yabIntType b(i2);
  	yabIntType c( (i1+i2)% APRIME );
  	t1.fromYabInt(a);
  	t2.fromYabInt(b);
  	t3.fromYabInt(c);
  	t1 += t2;
  	std::cout << " "  << i1 << " " << i2 << std::endl;
  	if ( t3  != t1)
  		std::cout << "error " << iToA(c) << " != "<< iToA(b) << std::endl;
  		}
  }

  montType  t11(APRIME);
  montType  t22(t11);
  montType  t33(t11);
  for(int i1 = 0; i1 < (APRIME+10); i1 += 3){
  	for(int i2 = i1+1; i2 < (APRIME+10); i2+=7){
  	yabIntType a(i1);
  	yabIntType b(i2);
  	unsigned long long ct = (i1*i2)% APRIME;
  	yabIntType c( ct );
  	t11.fromYabInt(a);
  	t22.fromYabInt(b);
  	t33.fromYabInt(c);
  	t11 *= t22;
  	std::cout << " "  << ct << " "  << iToA(t11) << std::endl;
  	if ( t33  != t11)
  		std::cout << "error " << iToA(c) << " != "<< iToA(b) << std::endl;
  		}
  }
*/ 
  MontPollardRho("649037107316853453566312041152511");//M109
  MontPollardRho("2535301200456458802993406410751");//M101
  MontPollardRho("803469022129495137770981046170581301261101496891396417650687");//M199
  MontPollardRho("115792089237316195423570985008687907853269984665640564039457584007913129639937");//F8
  //MontPollardRho("340282366920938463463374607431768211457");//F7
  MontPollardRho("13407807929942597099574024998205846127479365820592393377723561443721764"
  "0300735469768018742981669034276900318581864860508537538828119465699464336"
	"49006084097");
}

/* 
   calculate x^y (mod N). 
   
   we assume y >= 0 
   
*/
yabIntType ModPow( const yabIntType &N, const yabIntType &x, const yabIntType &y )
{
	montType _x(N);
	montType _p(_x);
	_x.fromYabInt(x);
	_p.fromYabInt(yabIntType((int) 1));
	if(y.isZero()) return _p.toYabInt();
	
	std::cout << " ModPow " <<  iToA(_p.toYabInt()) << std::endl;
		
	/* y is at least one digit */	
  std::vector<baseType> ybits;
  ybits = yabIntPeek(y);	
	while( ybits.size() > 0){
		 baseType _y = ybits.back(); 
		 for(baseType it = (yabIntType::UL >> 1) ; it> 0; it = (it >> 1))
		 {
		 		_p *= _p;
		 		if (it & _y)  _p *= _x;		 		
				std::cout << " ModPow " <<  iToA(_p.toYabInt()) << std::endl;
				std::cout << " ModPow _y it " <<  _y  << " " << it << std::endl;
		 }
		 ybits.pop_back();
	}
	/* now done */
	return _p.toYabInt();
}

void testModPow(){
	
	  for(int i = 1 ; i < 16 ; i++){
     	yabIntType x((int) i);
     	yabIntType y((int) 10460353202);
     	//yabIntType m((int) 10460353203);
     	yabIntType m((int) 524287 );// 
     	yabIntType res;
     	
     	res = ModPow(m, x,y);
     	std::cout << " x : " << iToA(x) ;
     	std::cout << " y : " << iToA(y) ;
     	std::cout << " m : " << iToA(m) << std::endl << std::endl;
      std::cout << " res : " << iToA(res) << std::endl ;

     	yabIntType res1;
      yabIntType one((int)1) ;	
      res -= one ;
      if (res.isPositive()) GCD( 	res, m, res1);
      std::cout << " res1 : " << iToA(res1) << std::endl << std::endl;
      
      	
    }
}

void debugFloat(){
	yabFloatType a("1.25");
  std::cout << " a " << fToA(a) << std::endl ;
	yabFloatType b("1.0");
  std::cout << " b " << fToA(b) << std::endl ;
	yabFloatType a1("0.000025");
  std::cout << " a1 " << fToA(a1) << std::endl ;
	yabFloatType b1 = a;
	             b1 += a1;
  std::cout << " n1  " << fToA(b1) << std::endl ;
	             b1 = a;
	             b1 -= a1;
  std::cout << " b1  " << fToA(b1) << std::endl ;
	             b1 = a1;
	             b1 += a;
  std::cout << " b1  " << fToA(b1) << std::endl ;
	             b1 = a1;
	             b1 -= a;
  std::cout << " b1  " << fToA(b1) << std::endl ;

}

void testFloat(){

/*	
	yabFloatType a("46682632");
	
  std::cout << " a " << fToA(a) << std::endl ;
	yabFloatType b("46682632.2", 40);
  std::cout << " b " << fToA(b) << std::endl ;
	yabFloatType c("1024.75", 20);
  std::cout << " c " << fToA(c) << std::endl ;
	yabFloatType d("1024.25", 20);
  std::cout << " d " << fToA(d) << std::endl ;

	yabFloatType an("-46682632");
  std::cout << " an " << fToA(an) << std::endl ;
	yabFloatType bn("-46682632.2", 40);
  std::cout << " bn " << fToA(bn) << std::endl ;
	yabFloatType cn("-1024.75", 20);
  std::cout << " cn " << fToA(cn) << std::endl ;
	yabFloatType dn("-1024.25", 20);
  std::cout << " dn " << fToA(dn) << std::endl ;
*/
	yabFloatType e("1.2");
  std::cout << " e  " << fToA(e) << std::endl << std::endl;
	yabFloatType e1("1.2", 20);
  std::cout << " e1 " << fToA(e1) << std::endl << std::endl ;
	yabFloatType e2("1.2", 100);
  std::cout << " e2 " << fToA(e2) << std::endl<< std::endl ;
	yabFloatType e3("1.125", 40);
  std::cout << " e3 " << fToA(e3) << std::endl<< std::endl ;
	yabFloatType e4("0.125", 40);
  std::cout << " e4 " << fToA(e4) << std::endl<< std::endl ;
  	
  yabFloatType add(e3)	; add+=(e4);
	std::cout << " add " << fToA(add) << std::endl<< std::endl ;

  yabFloatType sub(e3)	; sub-=(e4);
	std::cout << " sub " << fToA(sub) << std::endl<< std::endl ;

  yabFloatType prod(e3)	; prod *=(e4);
	std::cout << " prod " << fToA(prod) << std::endl<< std::endl ;
  
  yabFloatType e5(e1);
  yabFloatType f1("5");
  e5 *= f1;
	std::cout << " e5 " << fToA(e5) << std::endl ;
	std::cout << " e5 " << fToA(e5, -10) << std::endl ;
	std::cout << " e5 " << fToA(e5,20) << std::endl ;
	std::cout << " e5 " << fToA(e5,-20) << std::endl ;
	std::cout << " e5 " << fToA(e5,30) << std::endl ;
	std::cout << " e5 " << fToA(e5,-30) << std::endl ;
	std::cout << " e5 " << fToA(e5,40) << std::endl<< std::endl ;
	std::cout << " e5 " << fToA(e5,-40) << std::endl<< std::endl ;
	for( int i = 1; i <50 ; i++){
	std::cout << " e5 " << fToA(e5,i) << std::endl ;
	std::cout << " e5 " << fToA(e5,-i) << std::endl ;
	}	
  yabFloatType f2("-5");

  yabFloatType e6(e2);
  e6 *= f2;
	for( int i = 0 ; i <60 ; i++){
	std::cout << " e6 " << fToA(e6,i) << std::endl ;
	std::cout << " e6 " << fToA(e6,-i) << std::endl ;
	}	
	
  yabFloatType e7(e4);
	std::cout << " e7 " << fToA(e7) << std::endl ;
	std::cout << " e7 " << fToA(e7, -10) << std::endl ;
  e7 += e4;
	std::cout << " e7 " << fToA(e7) << std::endl ;
	std::cout << " e7 " << fToA(e7, -10) << std::endl ;
  e7 += e4;
	std::cout << " e7 " << fToA(e7) << std::endl ;
	std::cout << " e7 " << fToA(e7, -10) << std::endl ;
  e7 += e4;
	std::cout << " e7 " << fToA(e7) << std::endl ;
	std::cout << " e7 " << fToA(e7, -10) << std::endl ;
  e7 += e4;
	std::cout << " e7 " << fToA(e7) << std::endl ;
	std::cout << " e7 " << fToA(e7, -10) << std::endl ;
  e7 += e4;
	std::cout << " e7 " << fToA(e7) << std::endl ;
	std::cout << " e7 " << fToA(e7, -10) << std::endl ;
  e7 += e4;
	std::cout << " e7 " << fToA(e7) << std::endl ;
	std::cout << " e7 " << fToA(e7, -10) << std::endl ;
  e7 += e4;
	std::cout << " e7 " << fToA(e7) << std::endl ;
	std::cout << " e7 " << fToA(e7, -10) << std::endl ;

  yabFloatType f3("0.05");
	std::cout << " f3 " << fToA(f3) << std::endl ;
	std::cout << " f3 " << fToA(f3, -10) << std::endl ;

  yabFloatType f4("0.05",64);
	std::cout << " f4 " << fToA(f4) << std::endl ;
	std::cout << " f4 " << fToA(f4, -10) << std::endl ;
		
	for( int i = 0 ; i <60 ; i++){
	std::cout << " f4 " << fToA(f4,i) << std::endl ;
	std::cout << " f4 " << fToA(f4, -i) << std::endl ;
	}	

	
}


 void testFastItoAaux(unsigned int x)
{
    time_t now;
    yabIntType test("10");
		Mersenne(x, test);
	  //test.ChangeSign();
    now = time(NULL);
    printf("start: %s", ctime(&now));
    std::string temp; temp = iToA( test);

		std::cout << "Mersenne(" << x << ") length " << temp.length() << std::endl;
		std::cout << temp << std::endl;	
	
	  now = time(NULL);
	  printf("end: %s", ctime(&now));
		  
} 

void testFastItoA()
{
		testFastItoAaux( 862);
	  testFastItoAaux( 3320);
	  testFastItoAaux( 8624);
	  testFastItoAaux( 23200); 	      
	  testFastItoAaux( 86243);
	  testFastItoAaux( 132049); 
	  //testFastItoAaux( 216091);
}

void testMersenne()
{
	  testMersenneAux( 862);
	  testMersenneAux( 3320);
	  testMersenneAux( 8624);
	  testMersenneAux( 86243);
	
}



void testDivision()
{
	/*
	
	  yabFloatType a0("1");
	  
	  yabFloatType a1("10");
	  yabFloatType b1("0.1");
	  yabFloatType a2("100");
	  yabFloatType b2("0.01");
	  yabFloatType a3("1000");
	  yabFloatType b3("0.001");
	  yabFloatType a4("10000");
	  yabFloatType b4("0.0001");
	  yabFloatType a5("100000");
	  yabFloatType b5("0.00001");
	  yabFloatType a6("1000000");
	  yabFloatType b6("0.000001");
	  yabFloatType a7("10000000");
	  yabFloatType b7("0.0000001");
	  
	  yabFloatType c(a);
	  c *= b;
	  for(int i = 10; i < 50 ; i++){
		std::cout << i << " a       " << fToA(a, i) << std::endl ;  
		std::cout << i << " b       " << fToA(b, i) << std::endl ;  
		std::cout << i << " a*b = c " << fToA(c, i) << std::endl ;  	
		}
*/	 
	  yabFloatType a("500", 32);
		std::cout << " a         " << fToA(a, 20) << std::endl ;  
		std::cout << " 1/a       " << fToA(reciprocal(a), 20) << std::endl ;  
			
	  yabFloatType a0("1000", 32);	  
		std::cout << " a0         " << fToA(a0, 20) << std::endl ;  
		std::cout << " 1/a0       " << fToA(reciprocal(a0), 20) << std::endl ;  

	  yabFloatType a1("2000", 32);
		std::cout << " a1         " << fToA(a1, 20) << std::endl ;  
		std::cout << " 1/a1       " << fToA(reciprocal(a1), 20) << std::endl ;  

	  yabFloatType a2("2500", 32);
		std::cout << " a2         " << fToA(a2, 20) << std::endl ;  
		std::cout << " 1/a2       " << fToA(reciprocal(a2), 20) << std::endl ;  

	  yabFloatType a20("2800", 32);
		std::cout << " a20         " << fToA(a20, 20) << std::endl ;  
		std::cout << " 1/a20       " << fToA(reciprocal(a20), 20) << std::endl ;  


	  yabFloatType a3("3000", 32);
		std::cout << " a3         " << fToA(a3, 20) << std::endl ;  
		std::cout << " 1/a3       " << fToA(reciprocal(a3), 20) << std::endl ;  

	  yabFloatType a40("0.001", 32);
		std::cout << " a40         " << fToA(a40, 20) << std::endl ;  
		std::cout << " 1/a40       " << fToA(reciprocal(a40), 20) << std::endl ;  


	  yabFloatType a4("0.003", 32);
		std::cout << " a4         " << fToA(a4, 20) << std::endl ;  
		std::cout << " 1/a4       " << fToA(reciprocal(a4), 20) << std::endl ;  
		    
	  yabFloatType a5("0.004", 32);
		std::cout << " a5         " << fToA(a5, 20) << std::endl ;  
		std::cout << " 1/a5       " << fToA(reciprocal(a5), 20) << std::endl ;  
			
		yabFloatType m1("9472394.792")	;
		yabFloatType m2("8977.04174")	;
		std::cout << " m1 "  << fToA(m1, 20) << std::endl;
		std::cout << " m2 "  << fToA(m2, 20) << std::endl;
		yabFloatType m3;
		m3 = m1 ;
		m3 /= m2 ;
		std::cout << " m1/m2 "  << fToA(m3, -40) << std::endl;
		m3 = m1 ;
		std::cout << " m3    "  << fToA(m3, -80) << std::endl;
		m3 /= m1 ;
		std::cout << " m1/m1 "  << fToA(m3, -80) << std::endl;
		m3 = m2 ;
		std::cout << " m3    "  << fToA(m3, -80) << std::endl;
		m3 /= m2 ;
		std::cout << " m2/m2 "  << fToA(m3, -80) << std::endl;
}

void testSquareRoot()
{

		yabFloatType a("4000000000000000000", 32);
		std::cout << " a         " << fToA(a, 20) << std::endl ;  
		std::cout << " 1/<squareroot of a>       " << fToA(reciprocalSquareRoot(a), 20) << std::endl ;  
	
		yabFloatType a1("500", 32);
		std::cout << " a1         " << fToA(a1, 20) << std::endl ;  
		std::cout << " 1/<squareroot of a1>       " << fToA(reciprocalSquareRoot(a1), 20) << std::endl ;  

		yabFloatType a2("0.05", 32);
		std::cout << " a2         " << fToA(a2, 20) << std::endl ;  
		std::cout << " 1/<squareroot of a2>       " << fToA(reciprocalSquareRoot(a2), 20) << std::endl ;  

		yabFloatType a3("9591651650165.6159759157", 32);
		std::cout << " a3         " << fToA(a3, 20) << std::endl ;  
		yabFloatType a31 = reciprocalSquareRoot(a3);
		std::cout << " 1/<squareroot of a3>       " << fToA(a31, 20) << std::endl ;  
		yabFloatType a32 = a3;
		a32 *= a31;
		std::cout << " a3 * 1/<squareroot of a3>       " << fToA(a32, 20) << std::endl ;  
    a32 *= a32;			
		std::cout << " square(a3 * 1/<squareroot of a3>)       " << fToA(a32, 20) << std::endl ;  


		yabFloatType a4("959165314463116113131131331650165.6154654619759157", 32);
		std::cout << " a4                                " << fToA(a4, 100) << std::endl ;  
		yabFloatType a41 = reciprocalSquareRoot(a4);
		std::cout << " 1/<squareroot of a4>       " << fToA(a41, 100) << std::endl ;  

		 
		yabFloatType a42 = a4;
		a42 *= a41;
		std::cout << " a4 * 1/<squareroot of a4>       " << fToA(a42, 100) << std::endl ;  
    a42 *= a42;			
		std::cout << " square(a4 * 1/<squareroot of a4>) " << fToA(a42, 100) << std::endl ;  

		yabFloatType a5("959165314463116113131131331650165.6154654619759157", 32);
		std::cout << " a5         " << fToA(a5, 20) << std::endl ;  
		yabFloatType a51 = SquareRoot(a5);
		std::cout << " a5 = squareroot of a5       " << fToA(a51, 20) << std::endl ;  
		 a51 *= a51;
		std::cout << " a5 * a5    " << fToA(a5, 20) << std::endl ;  
			
}

/*
       Gauss-Legendre Iteration for Pi
*/
void pi(int iterations)
{
		yabFloatType a0("1", 32);
		std::cout << " a0 " << fToA(a0, 100) << std::endl ;  
		yabFloatType b2("2", 32);
		yabFloatType  b0 = reciprocalSquareRoot(b2);
		std::cout << " b0 " << fToA(b0, 100) << std::endl ;  
		yabFloatType  p0("1");
		yabFloatType  t0("0.25");

  	yabFloatType anext;
  	yabFloatType b1;
	  yabFloatType bnext;
	  yabFloatType pnext;
	  yabFloatType tnext;
	  yabFloatType diff ;
	  
	  for(int i = 0; i < iterations; i++){
	  	anext = a0;
	  	b1    = a0;
	  	anext += b0;
	  	anext /= b2;
	  	std::cout << "anext  "<<fToA(anext, 100) <<std::endl;

	  	b1 *= b0;
 			std::cout << " b1 " << fToA(b1, 100) << std::endl ;  
	  	std::cout << "now squareroot "<< std::endl;
	  	bnext = SquareRoot( b1);
	  	std::cout << "after squareroot "<< std::endl;
	  	pnext = p0;
	  	pnext *= b2;
	  	std::cout << "pnext  "<<fToA(pnext, 100) <<std::endl;
	  	tnext = t0 ;
	  	diff  = anext;
	  	std::cout << "1 diff  "<<fToA(diff, 100) <<std::endl;
	  	diff -= a0;
	  	std::cout << "2 diff  "<<fToA(diff, 100) <<std::endl;
	  	diff *= diff ;
	  	std::cout << "3 diff  "<<fToA(diff, 100) <<std::endl;
	  	diff *= p0;
	  	std::cout << "4 diff  "<<fToA(diff, 100) <<std::endl;
	  	tnext -= diff;
	  	std::cout << "tnext  "<<fToA(tnext, 100) <<std::endl;
	  	a0 = anext;
	  	b0 = bnext;
	  	p0 = pnext;
	  	t0 = tnext;
	  yabFloatType sum;
	  sum =  a0;
	  sum += b0;
	  sum *= sum;
	  yabFloatType div;
	  div  = t0;
	  div += div;
	  div += div;
	  yabFloatType pi;
	  pi = sum;
	  pi /=  div;
	  std::cout << std::endl << " iterations " << i << " PI " << fToA(pi, 500) << std::endl << std::endl ;  
	  	
	  }	
	  /* print result */
	  yabFloatType sum;
	  sum =  a0;
	  sum += b0;
	  sum *= sum;
	  yabFloatType div;
	  div  = t0;
	  div += div;
	  div += div;
	  yabFloatType pi;
	  pi = sum;
	  pi /=  div;
	  std::cout << " iterations " << iterations << " PI " << fToA(pi, 500) << std::endl ;  
	

}

void testSSMul(){
	yabIntType a("600");
	yabIntType b("100000000");
	yabIntType res((int) 0);
#if 1
	std::cout << " SchStrMultiply " <<  iToA(a) << std::endl ;  
	std::cout << " SchStrMultiply " <<  iToA(b) << std::endl ;  
	std::cout << " SchStrMultiply " <<  iToA(res) << std::endl ;  
	res = SchStrMultiply(a,b);
	std::cout << " SchStrMultiply " <<  iToA(res) << std::endl ; 
	for(int i = 0; i < 200 ; i++){ 
		//std::cout << i << " SchStrMultiply 1 " <<  iToA(res) << std::endl ; 
		//std::cout << i << " SchStrMultiply 2 " <<  iToA(b) << std::endl ;  
		a = SchStrMultiply(res,b);
   	std::cout << i << " SchStrMultiply 5 " <<  iToA(a) << std::endl ;  
		res = a;
	}
#else
	std::string qstr("6000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
	const char *cqstr = "6000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
	yabIntType q(qstr);
	yabIntType cq(cqstr);
	std::cout<<" qstr " << qstr << std::endl ;
	//std::cout<<" iToA(q) " << iToA(q) << std::endl ;
	std::cout<<" cqstr " << cqstr << std::endl ;
	//std::cout<<" iToA(cq) " << iToA(cq) << std::endl ;
	a =	SchStrMultiply(q, b);
	std::cout<<" a " << iToA(a) << std::endl ;
	std::cout<<" a " << iToA(SchStrMultiply(q, b)) << std::endl ;
#endif
}

void testSSMul2(){
	
	std::string astr("1");
	std::string bstr("6");
	yabIntType a(astr);
	yabIntType b(bstr);
	yabIntType res;
	for(int i = 0; i < 100 ; i++){
		astr = astr + "0";
		yabIntType a(astr);
	  res =	SchStrMultiply(a, b);
		std::cout<< i << " res:  " << iToA(res) << std::endl;
	}
}

void testItoA(){
	std::string a1str("11111111111111");
	std::string a2str("22222222222222");
	std::string a3str("33333333333333");
	std::string a4str("44444444444444");
	std::string a5str("55555555555555");
	std::string a6str("66666666666666");
	std::string a7str("77777777777777");
	std::string a8str("88888888888888");
	std::string a9str("99999999999999");
	std::string a0str("00000000000000");
	std::string astr("");
	astr += a1str; 
	yabIntType a(astr); std::cout << iToA(a) << std::endl;
	astr += a2str;
	yabIntType b(astr);  std::cout << iToA(b) << std::endl;
	astr += a3str;
	yabIntType c(astr);  std::cout << iToA(c) << std::endl;
	astr += a4str;
	yabIntType d(astr);  std::cout << iToA(d) << std::endl;
	astr += a5str;
	yabIntType e(astr);  std::cout << iToA(e) << std::endl;
	astr += a6str;
	yabIntType f(astr);  std::cout << iToA(f) << std::endl;
	astr += a7str;
	yabIntType g(astr);  std::cout << iToA(g) << std::endl;
	astr += a8str;
	yabIntType h(astr);   std::cout << iToA(h) << std::endl;
	astr += a9str;
	yabIntType i(astr);   std::cout << iToA(i) << std::endl;
	astr += a0str;
	yabIntType j(astr);
  std::cout << iToA(j) << std::endl;
  for(int ix =0; ix< 30; ix++){
  	astr += a0str;
  	yabIntType temp(astr);
  	std::cout<< " ix "<< ix<< "  " << iToA(temp) << std::endl;
  }
	
}

int main(int argc, char **argv)
{
    //testDivRem();
    //testGCD();
    //testJacobi();
    
		/*
		yabIntType xx("10");
		for(int i = 0 ; i < 8; i++)
		{
			std::cout << iToA(xx) << std::endl;
				xx *= xx;
			std::cout << iToA(xx) << std::endl;
		}	*/
    
		//testMontgomery();		
		//testModExponent();
		//testModPow();
		//testFloat();
		//testMersenne();
		//debugFloat();
		//testDivision();
		//testSquareRoot();
    //pi(10);
    Chudnovsky();
    //testSSMul();
    //testSSMul2();
    //testItoA();
    //TestSquareRoots();
	  return 0;
	  
}