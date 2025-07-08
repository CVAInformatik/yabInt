

#include "yabInt.h"
			  yabIntType::yabIntType(long long _i){
			  	long long i = _i;
			  	if( i < 0) i = -i;
			  	yabInt.clear();
			  	while (i){
			  		 yabInt.push_back(( int) (i % MODULUS) );
			  		 	i = i / MODULUS;
			  	}
			  	if(_i < 0) changeSign(yabInt);
			  }
		
				yabIntType::yabIntType(const char *c) {
				std::vector<baseType> temp ;
			  char *b = (char *)c ;
			  int sign = 0;
		    while( isspace(*b)) b++;
		    if( *b == '-') { sign  = -1; b++;}
		    else if( *b == '+') { sign  = +1; b++;};		    
		    while( isdigit(*b))	{
		    	if(yabInt.size() == 0) yabInt.push_back ((*b -'0') );
		    	else {
		    		temp = yabInt;
		    		internalDouble( yabInt); // *2
		    		internalDouble( yabInt); // *2 = *4
		    		destUniPolarAdd( yabInt, temp);; //  +1 =  *5
		    		internalDouble( yabInt); // *2 = *10
		    		yabInt[0] += (*b -'0') ;
		    	}
		    	b++;
		    }
		    while(yabInt.size() && (yabInt.back() == 0)) yabInt.pop_back();
		    
		    if( sign < 0 ) changeSign(yabInt);

		};

		
 		/* 
					 a = 2*a
		*/

		void internalDouble(std::vector<baseType> &a) {
			/* no carry addition */
			if(a.size() == 0) return;
			// we know size > 0 and stays that way.
			for ( std::size_t kx = 0 ; kx < a.size(); kx ++) a[kx] = a[kx]<<1; //a[kx] += a[kx];

			/* adjust carry */  
			baseType carry = 0;
			if( a.back() > 0)  // is positive
				{
				 for(std::size_t lx = 0; lx < a.size(); lx++){
						 a[lx]+= carry;
				     if(a[lx] & ~(yabIntType::UL-1)) { carry = 1;  ; a[lx] -= yabIntType::UL;} else carry = 0;
				    }
				  }
		  else {
				 for(std::size_t lx = 0; lx < a.size(); lx++){
	      				a[lx] += carry;
				     		if( a[lx] & ~(yabIntType::LL-1)) {  carry = -1; a[lx] -= yabIntType::LL;} else carry = 0;
				    }
				  }
		  if (carry != 0) a.push_back(carry);	
		}

		
		void changeSign(std::vector<baseType> &a){
			for( std::size_t ix = 0; ix < a.size() ; ix++)  a[ix] = -a[ix] ;
		}
			
		void mul10e3(std::vector<baseType> &a){
			if (a.size() == 0 ) return;
			std::vector<baseType> t = a;
			for( int i = 0; i < 7 ; i++)  internalDouble(a); // a = 128 *a 
			//changeSign(t);
			destBiPolarSub( a, t);  // a = 127 *a 
			destBiPolarSub( a, t);  // a = 126 *a 
			destBiPolarSub( a, t);  // a = 125 *a 
			for( int i = 0; i < 3 ; i++)  internalDouble(a);// a = 125*8 * a = 1000 *a 
		}
		
		void mul10e9(std::vector<baseType> &a){ 
  		if (a.size() == 0 ) return;
			mul10e3(a); mul10e3(a); mul10e3(a);
		}
			
			
		
		void dump(const std::vector<baseType> &a){
				for(int i = a.size(); i> 0 ;i--) printf("%d: %08X,  ", i-1, a[i-1]);
				printf("\n");
			}

		  char *iToA(const yabIntType &a ) {
		  	
			std::vector<baseType> temp ; 
			std::vector<baseType> Q ;
			std::vector<baseType> qt ;
	
			std::vector<baseType> one ; one.push_back(1);
			char *c;
			
			/* special case: 0*/
			if(a.yabInt.size() == 0)	{c = (char *) malloc(2); sprintf(c, "0");	return c;	}
			
			/* not zero */
			temp.clear();
			if(a.yabInt.back() < 0 ) 
				for(int i = 0; i< a.yabInt.size(); i++)	 temp.push_back(-a.yabInt[i]);
			else	
				for(int i = 0; i< a.yabInt.size(); i++)	 temp.push_back(a.yabInt[i]);
			/* not zero and not negative */			
		  c = (char *) malloc(5+(temp.size()*10));
		  bzero(c, 5+(temp.size()*10) );
			
			char *d = c;	
			
			do {	
				while (temp.size() > 1) {
					qt = temp;
					for(std::size_t ix = 1 ; ix < temp.size(); ix++) qt[ix-1] = qt[ix];
					qt.pop_back();
					destUniPolarAdd(Q, qt );
					mul10e9(qt);
					destBiPolarSub(temp,qt);
				} 
				
			
				while ((temp.size()>0) && (temp[0]>= 1000000000)) {
				
					destUniPolarAdd(Q, one);
					temp[0] -= 1000000000;					
				
				  if (temp.back() == 0) temp.pop_back();
				} 

				char buffer [10] ;
				sprintf(buffer,"%09d", temp[0]);
				char *c2 = buffer+8;
				for( int i = 0 ; i < 9 ; i++)	*d++ = *c2--;
				*d = '\0'; 				
				temp = Q;
				Q.clear();
		  } while (temp.size()> 0 );

		  /* tidy up  and reverse full result */
			char *d1 = c;
			char *d2 = d-1;
			while (*d2 == '0') {*d2-- = 0; }; // drop leading zeroes
			if (a.yabInt.back()< 0 ) *++d2 = '-';  // add a minus if negative 
			*(d2+1) = 0;// and zero terminate, just to be sure	
			while (d1 < d2){ char temp = *d1; *d1 = *d2; *d2 = temp; d1++; d2--;	} // reverse the full string
			//printf("strlen(c) %ld c: >%s<\n", strlen(c), c );
		  return (char *) realloc(c, 1+strlen(c));
		}

		/* 
					dest == destructive,  means it overwrites a, a += b
					uniPolar means if a and b must have the same sign, that means that no digits are 'extinguised'
					( so no need to check for leading zeroes) or sign changes.
		*/
		void  destUniPolarAdd( std::vector<baseType> &a, const std::vector<baseType> &b) {
			/* no carry addition */
			std::size_t sz = a.size();
			if(sz == 0 ){ a = b; return ;};
			if (a.size() < b.size() ){
				for ( std::size_t ix = a.size(); ix < b.size(); ix ++) a.push_back( b[ix]);
			  for ( std::size_t jx = 0 ; jx < sz; jx ++) a[jx] += b[jx];
			}
			else 
				{
			  for ( std::size_t kx = 0 ; kx < b.size(); kx ++) a[kx] += b[kx];
			  }
			/* adjust carry */  
			baseType carry = 0;
			if( a.back() > 0)  // a is positive
				{
				 for(std::size_t lx = 0; lx < a.size(); lx++){
						 a[lx]+= carry;
				     if(a[lx] >= yabIntType::UL) { carry = 1;  ; a[lx] -= yabIntType::UL;} else carry = 0;
				    }
				  }
		  else {
				 for(std::size_t lx = 0; lx < a.size(); lx++){
	      				a[lx] += carry;
				     		if( a[lx] <= yabIntType::LL) {  carry = -1; a[lx] -= yabIntType::LL;} else carry = 0;
				    }
				  }
		  if (carry != 0) a.push_back(carry);	
		}
		
		/* 
					dest == destructive  means it overwrites a, a += b
					BiPolar means a and b can have different signs, so we need to do the full monty:
					bipolar carry adjustment, leading zero dropping and sign adjustment.
		*/
		void  destBiPolarAdd( std::vector<baseType> &a, const std::vector<baseType> &b) {
			/* no carry addition */
			std::size_t sz = a.size();
			if(sz == 0 ){ a = b; return ;};
			if (a.size() < b.size() ){
				for ( std::size_t ix = a.size(); ix < b.size(); ix ++) a.push_back( b[ix]);
			  for ( std::size_t jx = 0 ; jx < sz; jx ++) a[jx] += b[jx];
			}
			else 
			  for ( std::size_t kx = 0 ; kx < b.size(); kx ++) a[kx] += b[kx];

      normalize(a);
		}

		/* 
					dest == destructive  means it overwrites a, a -= b
					BiPolar means a and b can have different signs, so we need to do the full monty:
					bipolar carry adjustment, leading zero dropping and sign adjustment.
		*/
		void  destBiPolarSub( std::vector<baseType> &a, const std::vector<baseType> &b) {
			/* no borrow subtraction */
			std::size_t sz = a.size();
			if(sz == 0 ){ 
				for(int i = 0; i < b.size(); i++) a.push_back( -b[i]);
				return ;
			};
			if (a.size() < b.size() ){
				for ( std::size_t ix = a.size(); ix < b.size(); ix ++) a.push_back( -b[ix]);
			  for ( std::size_t jx = 0 ; jx < sz; jx ++) a[jx] -= b[jx];
			}
			else 
			  for ( std::size_t kx = 0 ; kx < b.size(); kx ++) a[kx] -= b[kx];

			normalize(a);
		}
     
    /* Normalizes a number to conform to our basic format
       1) makes sure each individual digit is in ]ll....Ul] range, by pushing 
          carries up from the LSD-
       2) removes leading zeroes, so MSD is non-zero
       3) adjust signs, so all digits have the same sign as the MSD
       4) removes leading zeroes, again....
     */
		void normalize(std::vector<baseType> &a){ 
			
			/* adjust carry */  
			baseType carry = 0;
		  for(std::size_t lx = 0; lx < a.size(); lx++){
         a[lx]+= carry; // can't overflow
		     if( a[lx] >= yabIntType::UL) { carry = 1; a[lx] -= yabIntType::UL;}
				 else if( a[lx] <= yabIntType::LL) { carry = -1; a[lx] -= yabIntType::LL;} 
				 else carry = 0;
  	  }
		  if (carry != 0) a.push_back(carry);	
		  /* drop leading zeroes */	
		  while( a.size() && (a.back() == 0) ) a.pop_back();
		  /* adjust signs */
		  baseType carry1 = 0;
      if( a.size() && (a.back() > 0))
      	   for(std::size_t ix = 0 ; ix < a.size(); ix ++){
      	   	a[ix]+=carry1;
      	   	if(a[ix] < 0){  a[ix] += yabIntType::UL ; carry1 = -1; }
      	   	else carry1 = 0;
      	   }
      else 
      	   for(std::size_t ix = 0 ; ix < a.size(); ix++){
      	   	a[ix]+=carry1;
      	   	if( (a[ix]+carry1) > 0 ){ a[ix] += yabIntType::LL ; carry1 = 1; }
      	   	else carry1 = 0;
      	   }
		  /* drop leading zeroes */	
		  while( a.size() && (a.back() == 0) ) a.pop_back();
		}

void Mersenne(const unsigned int N, yabIntType &a)
{
	std::cout << "Mersenne "  <<std::endl;
	unsigned int _N = N;
	a.yabInt.clear();
	while (_N >= 30) { a.yabInt.push_back(MODULUS-1); _N-= 30; }

	std::cout << "Mersenne "  << _N << std::endl;
	
	baseType  temp =0;
	while( _N-- >0) {
		   temp = temp+temp;
		   temp  += 1;
	}
	std::cout << "Mersenne "  << iToA(a)  << std::endl;

	if(temp) a.yabInt.push_back(temp);
	
}
/*

			calculates a *= b 
*/
void  destBiPolarMul( std::vector<baseType> &a, const std::vector<baseType> &b)
{
	
	  if(b.size() == 0) a.clear();
	  if(a.size() == 0)	   return;
	  /* no zero in factors here */
	  std::vector<baseType> t1 = a;
	  std::vector<baseType> t2 = b;
#if  0
	  a.clear();
		/* calculates a *= b by the "Russian Peasant" method*/
	  if(t1.back() < 0 ) { changeSign(t1); changeSign(t2);}
	  for( int i = 0 ; i < t1.size()-1; i++ ){
	  	for( int j = 1 ; j < MODULUS; j = j+j ){
	  	   if(j & t1[i]) destUniPolarAdd(a, t2);
	  	   internalDouble(t2);
	  	}
	  }	  	
	  /* the last one is special we don't want to do a lot of 
	     superfluous doublings of a potentially very large number....
	  */
	  for( int j = 1 ; j <= t1.back(); j = j+j ){
	  	   if(j & t1.back()) destUniPolarAdd(a, t2);
	  	   internalDouble(t2);
	  }
#else

    int sign = 1;
 	  a.clear();

    if( t1.back() < 0) { sign = -sign; changeSign(t1);}
    if( t2.back() < 0) { sign = -sign; changeSign(t2);}
    
    for( size_t i = 0; i < ( t1.size() + t2.size() + 1); i++) a.push_back(0);
    	
    for( size_t i1 = 0; i1 < t1.size(); i1++)
    {
 
       for( size_t i2 = 0; i2 < t2.size(); i2++)
       {
			  //  printf("t1: %d t2: %d \n",t1[i1],t2[i2]);
			    long long acc = ((long long) t1[i1]) *((long long) t2[i2]);
			  //  printf("acc: %lld\n",acc);
			    a[i2 + i1] += ( acc & (yabIntType::UL-1));
			  //  printf("a[%ld]: %d\n",i2+i1, (int) ( acc & (UL-1)));
   				a[i2 + i1+1] += ( acc >> 30 );
			  //  printf("a[%ld]: %d\n",i2+i1+1,(int) (acc>>30));
		   
			 /* adjust carry */
		   		int carry = 0;
		   		if( a[i2+i1] >= yabIntType::UL)
		   		{
       	  	a[i2+i1] -= yabIntType::UL;
       	  	a[i2+i1+1]++;
		   		}
		   		if( a[i2+i1+1] >= yabIntType::UL)
		   		{
   	  	 		a[i2+i1+1] -= yabIntType::UL;
			   		carry = 1;
			 		}
       		for( size_t i3 = i2+i1+2 ; carry && (i3 < a.size()) ; i3++ )
       		{
       	  	a[i3] += carry ; 
       	  	carry = 0;
       	  	if( a[i3] >= yabIntType::UL)
       	  	{
       	  		carry = 1;
       	  		a[i3] -= yabIntType::UL;
       	  	}
       		}      
       }
//   	  dump(a);

     }
    while((a.size() > 0) && (a.back() == 0)) a.pop_back(); 
    if( sign < 0) changeSign(a);
 	  //dump(a);

#endif
}

/*
  returns 0 if equal 
  returns  1 if a > b
  returns  -1 if a < b
  
*/
int compare( const std::vector<baseType> &a, const std::vector<baseType> &b)
{
	 if(a.size() == 0){
	 	  if (b.size() == 0) return 0;
	 	  if (b.back() < 0 ) return 1 ;
	 	  if (b.back() > 0 ) return -1 ;
	 }
	 if(b.size() == 0){
	 	  if (a.size() == 0) return 0;
	 	  if (a.back() > 0 ) return 1 ;
	 	  if (a.back() < 0 ) return -1 ;
	 }
	 /* both a and b not zero */
	 if((a.back() > 0 ) && (b.back() <0 )) return 1;
	 if((a.back() < 0 ) && (b.back() >0 )) return -1;
	 /* same sign */
	 if( a.back() > 0){
	 	  if(a.size() > b.size() ) return 1;
	 	  if(a.size() < b.size() ) return -1;
	 }
	 else {
	 	  if(a.size() > b.size() ) return -1;
	 	  if(a.size() < b.size() ) return 1;
	 }
	 /* same size, same sign */
	 for( int i = a.size() ; i > 0; i--)
	     if( a[i-1]> b[i-1]) return 1;
	     else if(a[i-1]< b[i-1]) return -1;
	 return 0;
}			

/*
     returns the bitsize for the numerical value of a, |a|
*/
#define BTYPESIZE 30 
inline int   yabIntType::internalBitSize(const std::vector<baseType> &a) {
		baseType temp;
		if (a.size() == 0) return 0;
		temp = a.back() ;
		if( temp < 0 ) temp = -temp;
		int count = 0;
		while (temp > 0) { count++ ; temp = temp >> 1;}
		return ((a.size() - 1) *BTYPESIZE ) + count;
}

void yabIntType::internalDivPow2(unsigned int _p, std::vector<baseType> &a) 
{
	 unsigned int p = _p;
   if ( a.size() == 0 )	 return ;
   	
   baseType  sign = a.back();
   if( sign< 0) changeSign(a);

	 unsigned int digits = p/BTYPESIZE ; // no of digits to drop
   if (digits && (a.size() >= digits)) {
   	   for (unsigned int i = digits; i < a.size(); i++) a[i-digits] = a[digits];
   	   for (unsigned int i = 0; i < digits; i++) a.pop_back();
   	}

   p = p - digits *BTYPESIZE;
   while ( p--){
   		for(int i = 1 ; i< a.size(); i++) if (a[i] & 1) a[i-1] += MODULUS ;
   		for(int i = 0 ; i< a.size(); i++ ) a[i] = a[i]>>1 ;
 	 }
   while (a.size() && (a.back() == 0) ) a.pop_back() ;   
   
   if( a.size() && (sign < 0)) changeSign(a);
}

void yabIntType::internalMulPow2(unsigned int _p, std::vector<baseType> &a) 
{
	 unsigned int p = _p;
   if ( a.size() == 0 )	 return ;
   	
   baseType  sign = a.back();
   if( sign< 0) changeSign(a);

	 unsigned int digits = p/BTYPESIZE ; // no of digits to drop
   if (digits && (a.size() >= digits)) {
   	   a.push_back( 0 ); // Allocate extra space, 
   	   for (unsigned int i = a.size()-1  ; i > 0 ; i) a[i] = a[i-1];
   	   a[0] = 0;
   	}

   p = p - digits *BTYPESIZE;
   while ( p--) internalDouble(yabInt);

   while (a.size() && (a.back() == 0) ) a.pop_back() ;   
   
   if( sign < 0 ) changeSign(a);
}

yabIntType& yabIntType::operator++()
{
	if (yabInt.size() == 0) yabInt.push_back(1);
	else {
		yabInt[0]++;
		normalize(yabInt);
	}
	return *this;
}

/*
     return quotient and Remainder, such that
     
     a = m * Quotient + Remainder, a,m != 0,  Quotient = (a div m),  Remainder >= 0.
     
pIntClass RemQuotient(const pIntClass& A, const pIntClass& M, pIntClass* Quotient)
{
	pIntClass  _Quotient;
    pIntClass Rem;// = new pIntClass();
	//int counter = 0;

	if (M.IsZero()) {
		std::cout << "divison by zero" << std::endl;
		return Rem;
	}
	else if ((A.value.size() == 1) && (M.value.size() == 1)) {
		/* small numbers both less than RMOD 
		if (Quotient != NULL)
		{
			Quotient->value.clear();
			Quotient->value.push_back(A.value[0] / M.value[0]);
		}

		Rem.value.push_back( A.value[0] % M.value[0]);
        Rem.normalize(Rem.value);
		return Rem;
	}
	else {
        pIntClass  _dividend(A);
        pIntClass  _divisor(M);
        int     reciprocal = pIntClass::MODULUS / (2 + _divisor.value.back());
        int     shift = (int)M.value.size();
		pIntClass  _reciprocal(reciprocal);

        Rem = reciprocal;
		Rem *= _dividend;

		if (!Rem.IsZero())  
            for (int i = 0; i < shift;i++)	Rem.DivModulus();

		while (1)
		{
			_Quotient = Rem;
			Rem *= _divisor;
            Rem.ChSignBit();
			Rem += _dividend;

			if (_divisor.IsBiggerNummerically(Rem)) 	
                break;

			Rem *= _reciprocal;
			for (int i = 0; i < shift;i++) {
				Rem.DivModulus();
			}
			if (Rem.IsZero())
				Rem = 1;
			Rem += _Quotient;
		}
		
		if (Rem.IsNeg())
		{
			Rem += _divisor;
			_Quotient += -1;
		}
		if (Quotient)  *Quotient = _Quotient;
		return Rem;
	}
}

     
*/
void DivRem(const yabIntType &a, const yabIntType &m,yabIntType &Quotient, yabIntType &Remainder )
{
	if (m.yabInt.size() == 0) {
		Quotient.yabInt.clear();
		Remainder.yabInt.clear();
		return;
	}

	if( (a.yabInt.size() == 1 ) && (m.yabInt.size() == 1 )) // two small numbers
	{
			Quotient.yabInt.clear();
			Quotient.yabInt.push_back( a.yabInt[0]/m.yabInt[0]);
			if(Quotient.yabInt[0] == 0 )  Quotient.yabInt.clear();
			Remainder.yabInt.clear();
			Remainder.yabInt.push_back( a.yabInt[0] % m.yabInt[0]);
			if(Quotient.yabInt[0] == 0 )  Quotient.yabInt.clear();
			if(Remainder.yabInt[0] == 0 ) Remainder.yabInt.clear();
			return;
	}
	
	// at least on big number
	yabIntType _dividend(a);
	yabIntType _divisor(m);
	int reciprocal = m.UL/(2 + m.yabInt.back() );
	int shift = (int) m.yabInt.size();
	yabIntType _reciprocal(reciprocal);
	
	Remainder = _reciprocal;

	Remainder *= _dividend;

	if (!Remainder.isZero())  
      for (int i = 0; i < shift; i++) {
      	  switch(Remainder.yabInt.size() ){
      	  case 0: break;
      	  default :
       					for( int j =0 ; j < (Remainder.yabInt.size()-1); j++)
     		  			     Remainder.yabInt[j] = Remainder.yabInt[j+1];
     						Remainder.yabInt.pop_back();
     						break;
     			}
		  }
						
	while(1){
			Quotient = Remainder;

		  Remainder *= _divisor;

		  Remainder.ChangeSign();

		  Remainder += _dividend;

      if(Remainder.isNegative()){
      	 yabIntType r(Remainder);
      	 r.ChangeSign();
      	 if ( _divisor > r ) break;
      } else
      	 if ( _divisor > Remainder ) break;
      	
 			Remainder *= _reciprocal;

      for (int i = 0; i < shift;i++) {
      	  switch(Remainder.yabInt.size() ){
      	  case 0: break;
      	  default :
       					for( int j =0 ; j < (Remainder.yabInt.size()-1); j++)
     		  			     Remainder.yabInt[j] = Remainder.yabInt[j+1];
     						Remainder.yabInt.pop_back();
     						break;
     			}
		  }

		  if(Remainder.isZero()) Remainder = 1;
		  Remainder += Quotient;
	}					
	
	/* clean up */					
	if(Remainder.isNegative()){
		 Remainder += _divisor;
		 Quotient  += -1;
	}		
		
}




#if 0
char *yabIntType::iToA2() {
		std::vector<baseType> temp = yabInt ; 
		std::vector<baseType> Q ;
		std::vector<baseType> qt ;
			
		
		std::vector<baseType> one ; one.push_back(1);
		char *c;
			
		/* special case: 0*/
		if(temp.size() == 0)	{c = (char *) malloc(2); sprintf(c, "0");	return c;	}
		
		/* not zero */
		if(temp.back() < 0 ) changeSign(temp);
			
		/* not zero and not negative */			
	  c = (char *) malloc(5+(temp.size()*10));
	  bzero(c, 5+(temp.size()*10) );
			
		char *d = c;	
			
		do {	
			while (temp.size() > 1) {
				qt = temp;
				for(std::size_t ix = 1 ; ix < temp.size(); ix++) qt[ix-1] = qt[ix];
				qt.pop_back();
				destUniPolarAdd(Q, qt );
				mul10e9(qt);
				destBiPolarSub(temp,qt);
			} 
			
			while (temp.size() && (temp[0]>= 1000000000)) {
				destUniPolarAdd(Q, one);
				temp[0] -= 1000000000;					
			  if (temp.back() == 0) temp.pop_back();
			} 
			char buffer [10] ;
			sprintf(buffer,"%09d", temp[0]);
			char *c2 = buffer+8;
			for( int i = 0 ; i < 9 ; i++)	{	*d++ = *c2--;}
			*d = '\0'; 				
			temp = Q;
			Q.clear();
	  } while (temp.size()> 0 );
		/* tidy up  and reverse full result */
		char *d1 = c;
		char *d2 = d-1;
		while (*d2 == '0') {*d2-- = 0;}; // drop leading zeroes  and zeroterminate
		if (yabInt.back()< 0 ) *++d2 = '-';  // add a minus if negative 
		*(d2+1) = '\0'; 				
		while (d1 < d2){ char temp = *d1; *d1 = *d2; *d2 = temp; d1++; d2--;	} // reverse the full string
		return (char *) realloc(c, 1+strlen(c));
}
#endif


/* binary GCD AKA Stein's algorithm 

from wikipedia (in pseudocode with parallel assignment !) 

function extended_gcd(a, b)
    (old_r, r) := (a, b)
    (old_s, s) := (1, 0)
    (old_t, t) := (0, 1)
    
    while r ? 0 do
        quotient := old_r div r
        (old_r, r) := (r, old_r - quotient × r)
        (old_s, s) := (s, old_s - quotient × s)
        (old_t, t) := (t, old_t - quotient × t)
    
    output "Bézout coefficients:", (old_s, old_t)
    output "greatest common divisor:", old_r
    output "quotients by the gcd:", (t, s)
*/

void extendedGCD(const yabIntType &a, const yabIntType &b, yabIntType &gcd, yabIntType &am, yabIntType &bm  )
{
	yabIntType old_r(a);
	yabIntType r(b);
	yabIntType old_s((long long) 1);
	yabIntType s((long long) 0);
	yabIntType old_t((long long) 0);
	yabIntType t((long long) 1);
	
	while(!r.isZero()){
		 yabIntType quotient ;
		 yabIntType rem;
		 DivRem( old_r, r, quotient, rem);
		 { // (old_r, r) := (r, old_r - quotient × r)
		 	  yabIntType temp_oldr(old_r);
		 	  old_r = r;
		 	  r *= quotient ;
		 	  r.ChangeSign();
		 	  r += temp_oldr;		 	  
		 }
		 { // (old_s, s) := (s, old_s - quotient × s)
		 	 yabIntType temp_olds(old_s);
		 	 old_s = s;
		 	 s *= quotient;
		 	 s.ChangeSign();
		 	 s +=  temp_olds;
		 }
		 { //  (old_t, t) := (t, old_t - quotient × t)
		 	  yabIntType temp_oldt(old_t);
		 	  old_t = t;
		 	  t *= quotient;
		 	  t.ChangeSign();
		 	  t += temp_oldt;
		 }		
	}
  gcd = old_r;
	am =  old_s;
	bm =  old_t;	
}

/*

   Currently only works for a and b 0 or larger....

*/
void GCD(const yabIntType &a, const yabIntType &b, yabIntType &gcd)
{
	  if(a.isZero()) { gcd.yabInt.clear(); return ;}
	  if(b.isZero()) { gcd.yabInt.clear(); return ;}
	  
	  unsigned int shift = 0;
	  yabIntType ac(a);
	  yabIntType bc(b);
	  
	  while ( ((ac[0] | bc[0]) & 1) == 0){
	  	 ac.DivPow2(1);
	  	 bc.DivPow2(1);
	  	 shift++;
	  }

	  while ((ac[0] & 1 ) == 0) ac.DivPow2(1);
    while (bc.isPositive() ){
	 			 while ((bc.yabInt[0] & 1 ) == 0) bc.DivPow2(1);

	 			 if( ac > bc ){
	 			 	yabIntType swap(ac);
	 			 	ac = bc;
	 			 	bc = swap;
	 			 }
	 			 bc -= ac;
	 			 bc.DivPow2(1);
		}
		ac.MulPow2(shift);
		gcd = ac;
}

const yabIntType  Exp(const yabIntType &a, unsigned long long exponent)
{
	  yabIntType res(1);
	  if(exponent == 0 ) return res;
	  if(a.isZero())	   return res;
	  /* no zero in factors here */
	  yabIntType t1(a);
	  if(t1.isNegative() ) t1.ChangeSign();
	  	
	  for( unsigned long long j = 1 ; j <= exponent ; j = j+j ){
	  	   if(j & exponent) res *= t1;
	  	   t1 *= t1;
	  }
	  if(a.isNegative() && (exponent & 1 )) res.ChangeSign();

	  return res;
}


int Jacobi(const yabIntType& a, const yabIntType& b)
{

	yabIntType A(a);
	yabIntType M(b);
	int ResSign = 1;
	if (M.isZero())
		return 1;
	else {
		yabIntType rem, quotient;

	  DivRem(A, M, quotient, rem); A = rem;
		if (A.isZero()) {  //A|M 
			return 0;
		}
		else {
			while (!A.isZero()) {
				while (!A.isZero() && ((A.yabInt[0] & 1) == 0))
				{
					A.DivPow2(1);
					switch (M.yabInt[0] & 0x7)
					{
					case 3: 
          case 5:   
               ResSign = -ResSign;
						break;
					default:
						break;
					}
				}
				yabIntType temp(A);
				A = M;
				M = temp;
				if (!A.isZero() && !M.isZero() && (3 == (A.yabInt[0] & 0x3)) && (3 == (M.yabInt[0] & 0x3))) 
                    ResSign = -ResSign;
        DivRem(A, M, quotient, rem); A = rem;

			}
			if ((M.yabInt.size() == 1) && (M.yabInt[0] == 1))
				return ResSign;
			else
				return 0;
		}
	}
};



yabIntType modmult(const yabIntType &_a, const yabIntType &_b, const yabIntType &mod) {  // Compute a*b % mod
    yabIntType result;// = new pIntClass;
    yabIntType a = _a;
    yabIntType b = _b;
    yabIntType quot; 
    yabIntType rem;
    while (!b.isZero()) {

        if ((b[0] & 1) == 1) {
            result += a;
            DivRem(result, mod, quot, rem); result = rem;
        }
        a.Double();
        DivRem(a, mod, quot, rem); 
        a = rem;
        b.DivPow2(1);
    }
    return result;
}

yabIntType  modpow(const yabIntType & _a, const yabIntType & _b, const yabIntType & mod) {  // Compute a^b % mod
    yabIntType  result;
    yabIntType   a = _a;
    yabIntType   b = _b;
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
    for(int it = 0; it < (b.size()-1); it++){
    	baseType ib = 1;
    	bb = b[it];    	
	    while(ib < yabIntType::UL){
        if ((bb & ib) == 1) {
            t = modmult(result, a, mod);
            result = t;
        }
        a = modmult(a, a, mod);
        ib = ib<<1 ;
			}
    }
   	bb = b[b.size()-1];
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


yabIntType   Mersenne(unsigned int N)
{
	yabIntType res;
	res.yabInt.clear();
	while (N >= 30) { res.yabInt.push_back(MODULUS-1); N-= 30; }
	
	baseType  temp =0;
	while( N-- >0) {
		   temp = temp+temp;
		   temp  += 1;
	}
	if(temp) res.yabInt.push_back(temp);
	
	return res ;
}




RandomYabIntType::RandomYabIntType(){
	dist = new std::uniform_int_distribution<unsigned int>(0, yabIntType::UL - 1);
}

RandomYabIntType::~RandomYabIntType(){
	delete dist;
}



yabIntType RandomYabIntType::rand(yabIntType &a){
  
  std::vector<baseType> res;
	
	while( res.size() < a.size())
			res.push_back(_Rand()); 
 
  if ((res.size() > 0) && ( res.back() > a[a.size()-1])) { 
  	baseType t = res.back() % a[a.size()-1];
  	res.pop_back();
  	res.push_back(t);  	
  }
  while(res.size() && ( res.back() == 0)) res.pop_back();
  
  yabIntType r(res);
  
  return r;	
}

unsigned RandomYabIntType::_Rand(){
	unsigned  int ix2 = dist->operator()(rd);
	return ix2 % yabIntType::UL ;
}


yabIntType mont_R( const yabIntType &a)
{
	yabIntType res ;
	res.yabInt.clear();
	while(res.yabInt.size() < a.yabInt.size()) res.yabInt.push_back(0);
	res.yabInt.push_back(1);
	return res;
}
