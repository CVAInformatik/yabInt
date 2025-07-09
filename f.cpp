


#include <string>
#include "y.h"
#include "f.h"

//#define ALIGN 1  Right now I can see why it doesn't work
// as I expected, will leave it alone for now 
yabFloatType::yabFloatType(const char *s, unsigned int pf)
{
	
#define ERROR 10
	
	bool neg = false;
	
	std::string myS(s);
		
	std::string tempI;
	std::string tempF;
	
	unsigned int state = 0;
		
  for( size_t i = 0; i < myS.size(); i++)
  {
			switch (state){
  		case 0: if(std::iswspace( myS[i])) state = 0;
  			    else if(myS[i] == '+') state = 1;
  			    else if(myS[i] == '-') { state = 1; neg = true;}
  			    else if(std::isdigit(myS[i]) ){ state = 2; tempI.push_back(myS[i] );}
  			    else state = ERROR;	
  			    break;
  			    
  		case 1: if(std::isdigit(myS[i]) ){ state = 2; tempI.push_back(myS[i] );}
  			    else state = ERROR;	
  			    break;
  			    
  		case 2:  if(std::isdigit(myS[i]) ) tempI.push_back(myS[i] );
  			    else if(myS[i] == '.') {
  			    	 yabIntType t(tempI);
  			    	 val = t ;// the integer part is now in val;
  			    	 state = 3;
  			    }
  			    else 
  			    	state = ERROR;	
  			    break;
  			    
  		case 3:
  	        if(std::isdigit(myS[i]) ){ 
  			    	tempF.push_back(myS[i] );
  			    	state = 3;
  			    }
  			    else 
  			    	state = ERROR;	
            break;
  			    
  		case ERROR:
  			    break;
  	}
  }

 	if(state == ERROR) { val = 0; a = 0; }
 	else if (state == 1) a= 0 ; //integer
 	else if (state == 2) { 
    yabIntType t(tempI);
  	val = t ;// the integer part is now in val;
   	a = 0;
  }
  else if (state == 3) { //fractional part present, maybe...
  	std::vector<unsigned char> n ;
  	a = 0;
		
  	for(int i = 0; i<tempF.size(); i++)  {
  		n.push_back( tempF[i] - '0' );
  	}
 		while( n.size() && n.back() == 0 ) n.pop_back();
  	while(( a < ((pf *tempF.size())+ yabIntType::DIGITSIZE)) && n.size()> 0){
  			for ( int j = 0 ; j< n.size(); j++) 
  			{
  				n[j] += n[j];
  			}


  			int c = 0;
  			for( int j = n.size(); j > 0 ; j--){
  				 n[j- 1] += c; c = 0;
  				 if(n[j- 1] > 9) {n[j- 1] -= 10; c = 1;}
  			}
  			val <<= 1;
  			val += c ;
  			a++;
  			if( n.back() == 0 ) n.pop_back();
  	}
  }

  
  /* put the 'binary' point on a 'digit'- border  */
#ifdef ALIGN  
  
  yabIntType testval(val);
  unsigned long long testa = a;
  while((testa %  yabIntType::DIGITSIZE) != 0)   	{
  std::cout <<" a " << a << "  dump of val " ; dump(val.yabInt);
  std::cout <<" testa " << testa << "  dump of testval " ; dump(testval.yabInt);
  			testval <<= 1;
  			testa++;  		
  } 
  a = testa;
  val = testval;
#else
  //std::cout << "bitsize "  << BitSize(val)  <<" a " << a << "  dump of val " ; dump(val.yabInt);
#endif
  if( neg ) val.ChangeSign();
}


std::string Round( int _digits, std::string &in, std::string &frac )
{
	int digits = _digits ;
	if( _digits < 0) digits = -digits ;
		
	if(frac.length() < digits ) {
		std::string temp = frac ;
		
    if( _digits < 0 ){
    	if(temp.length() == 0)  temp = "." ;
    	else temp = "." + frac;
    			
    	while (temp.length() < digits+1 ) temp.push_back('0'); 
    }
    else 
    	if(temp.length() > 0) temp =  "." + frac;
    	
		return in + temp;
	}
	else {
		int carry = 0;
		std::string temp = frac.substr(0, digits+1);
    switch (temp.back()){
    	case '5': 
    	case '6': 
    	case '7':
    	case '8':
    	case '9': carry = 1; temp.pop_back(); break;
    	default: temp.pop_back(); break;
    };
    while (temp.length() && carry ) {
    	switch (temp.back()){
    		case '9': carry = 1; temp.pop_back(); 
    			break;    		
    		default:  carry = 0; temp.back() = temp.back()+1 ; 
    			break;    	
    	}			
    }
    // back fill 
    if( _digits < 0 )
    	while (temp.length() < digits ) temp.push_back('0'); 
    /* handle integer part */
    for (std::string::reverse_iterator rit=in.rbegin(); rit!=in.rend(); ++rit)
    {
    	if( carry == 0) break;
    	switch (*rit){
    		case '9': carry = 1; *rit = '0'; 
    			break;    		
    		default:  carry = 0; *rit = *rit +1 ; 
    			break;    	
    	}			    	
    }
    /* integer part done */
    if( temp.length() ) return in + "." + temp;
    else return in;
	}	    
	return std::string("test");
}	

std::string fToA(const yabFloatType &a, int digits){
	
	static yabIntType intPart;
	std::string sign = "" ;

	intPart = a.val ;
	if(intPart.isNegative()) { sign = "-" ; intPart.ChangeSign() ;}
	
	if( a.a)  intPart >>= a.a ;

	std::string in( iToA(intPart))	;
	

	if(a.a == 0) return sign + in ; // we have an integer

   /* now we need convert the fractional part */
   /* we shift it LEFT, so the MSB (the first after the binary point)
      is in position DIGITSIZE-1
      Then we remove the integer part bits, and prepend with digits '1''0''0'******
      (to keep the yabIntFormat no leading zeroes).
      multiplying with 10 will produce '10''0''0d'********,
      where d is the first digit after the decimal expansion and the *** parts
      has changed, (it will always appear in the same position)
      we save the d and replace '10''0''0d' with '1''0''0'*******,retaining the new **** part
      and repeat until the ***** part is all zero.
   */   
   unsigned int shift = a.a % yabIntType::DIGITSIZE ;
   unsigned int dpos  	 =  (a.a/yabIntType::DIGITSIZE)+1 ;
   yabIntType frac(a.val);
   if(a.val.isNegative() )frac.ChangeSign() ; 
//#ifndef ALIGN   	
   if(shift ) frac<<= (yabIntType::DIGITSIZE- shift);
//#endif   
   std::string fraction ;
   std::vector<baseType> rawBits = yabIntPeek(frac)	;
   std::vector<baseType> rawFrac ;
   	while (1){
	   		baseType  counter = 0;
	   		rawFrac.clear();
  		 	for(int i =0 ; i < dpos; i++) {
   		 		counter |= rawBits[i];
   		 		rawFrac.push_back(rawBits[i])	;
   			};
   			if (counter == 0) break;
   			rawFrac.push_back( 0);
				rawFrac.push_back( 0);
				rawFrac.push_back( 1);
    		yabIntPoke(rawFrac, frac);
        frac *= 10;
        rawBits = yabIntPeek(frac)	;
        fraction.push_back( rawBits[dpos] + '0');    
  	}
  	return sign + Round(digits, in, fraction);
}


	/*
	         if a == b -> 0
	         if a > b ->  1
	         if a < b ->  -1
	*/
	int yabFloatCompare(const yabFloatType &a, const yabFloatType &b )	{
		int res = 0;
		yabFloatType t(a);
		
		t -= b ;
		
		if ( t.isZero() ) res = 0;
		else if( t.isPositive() ) res = 1;
		else res = -1;
			
		return res;
	}


		
/* given a returns the value for initial guess of the reciprocal */
	yabFloatType  guess(const yabFloatType& a){
	      std::vector<baseType> temp = yabIntPeek(a.val)	;
	      unsigned long long ain = a.a ;
	      int bitsize = BitSize(temp);
	      //std::cout << " bitsize " << bitsize << std::endl;
	      yabIntType aVal(temp);	
	      if(ain > bitsize){
	      	   // number is smaller than 1
	      	   aVal = 1;
	      	   aVal <<= (ain - bitsize) +1 ;
	      	   // ain is kept as is
	      }
	      else { // a <= bitsize
		  	     //aVal is kept as is
		  	     ain =  bitsize+10-ain  ;
		  	}
		  	yabFloatType c1("1.20");// best value 1.5 ?
		  	yabFloatType _c1("1.10");
		  	yabFloatType c2("0.80");// best value 0.75 ?
		  	yabFloatType _c2("0.9");
		  	yabFloatType res(aVal,ain);
		  	do {
           yabFloatType test;
           test = res;
           test *= a; 
           int l1 = yabFloatCompare( c1,test);//  1 if c1 > test
           int l2 = yabFloatCompare( c2,test);// -1 if c2 < test
		  	
		  	   if( (l1 * l2 ) == -1) break; 	// c1 > test && c2 < test 	  	
		  	   if (l1 < 0 ) res *= _c2;
           if (l2 > 0)  res *= _c1;
		  	} while(true);
		  	
		  	return res;
	}


  
	yabFloatType  reciprocal(const yabFloatType& f)
	{
		if(f.isZero()){
			std::cout << " division by zero !!!!!!! " << std::endl;
			return yabFloatType("0");
		}
		
		yabFloatType Xnow(guess(f));
		yabFloatType _Two("2");
		yabFloatType  t1;

		for(int i = 0; i < 10; i++) { // iterate 10 times for now
          t1 = f ;
          t1 *= Xnow;
          t1.ChangeSign();
          t1 += _Two;
          t1 *= Xnow ;
			    Xnow = t1;
		};
		
		return Xnow;
	}
	
	/*
	        returns 1/<the squareroot of f>
	        
	        iteration step  is 
	        
	        Xnew = (Xold/2)(3 - f * Xold * Xold)
	*/
	yabFloatType  Wish(const yabFloatType& a){

        yabIntType t((long) 3);
        // here we do some guessing
        std::vector<baseType> temp = yabIntPeek(a.val)	;
	      unsigned long long ain = a.a ;
	      int bitsize = BitSize(temp);
	      
	      //std::cout << " bitsize " << bitsize << " ain " << ain <<std::endl;
	      if (bitsize > ain) // f >= 1
	      	{
	      	  ain = ((bitsize - ain)/2) +1 ;
	      	}
	      else {
	      	t <<= ((ain-bitsize)/2) -1 ;
	      	ain = 0;
	      }
        // we have a guess
		  	yabFloatType res(t,ain);
		  	
	      //std::cout << " res " << fToA(res,20) << std::endl;

		  	return res;

		
	}
	
	yabFloatType  reciprocalSquareRoot(const yabFloatType& f)
	{
		if(f.isZero()){
			std::cout << " division by zero !!!!!!! " << std::endl;
			return yabFloatType("0");
		}
		yabFloatType Xold;
		yabFloatType three("3");
		yabFloatType half("0.5");
		
		Xold = Wish(f);
		
		for( int i = 0 ; i < 10 ; i++ ){
			 yabFloatType t;
			 t = Xold ;
			 t *= Xold;
			 t *= f ;
			 t.ChangeSign();
			 t += three;
			 t *= Xold;
			 t *= half;
			 Xold = t;
		}
		if (Xold.isNegative()) Xold.ChangeSign();
		return Xold;
	}
