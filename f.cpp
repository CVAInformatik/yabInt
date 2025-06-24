


#include <string>
#include "y.h"
#include "f.h"


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
  if( neg ) val.ChangeSign();
}


std::vector<baseType> maskOff(unsigned int bits, const std::vector<baseType> &a)
{
	  std::vector<baseType> res;
	  	
	  if ( bits > (a.size()* yabIntType::DIGITSIZE) ){
	  	res = a;
	  	while( bits > (res.size()* yabIntType::DIGITSIZE) ) 
	  		res.push_back(0);
	  }
	  else {
	  	
	  	
	  		
	  }
	  
	  return res;
}

std::string fToA(const yabFloatType &a ){
	
	static yabIntType intPart;
	bool       neg = false ;
	

	intPart = a.val ;
	if(intPart.isNegative()) { neg = true; intPart.ChangeSign() ;}
	
	if( a.a)  intPart >>= a.a ;

	if(neg)intPart.ChangeSign() ; 

	std::string in( iToA(intPart))	;

	if(a.a == 0) return in ; // we have an integer

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
   if(neg)frac.ChangeSign() ; 
   if(shift ) frac<<= (yabIntType::DIGITSIZE- shift);
   
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
  	if (fraction.length())
	  	return in + "." + fraction;
	  else
	  	return in ;
}