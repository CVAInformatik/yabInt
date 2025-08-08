/*
   Schoenhage-Strassen Multiplication
   
*/

#include "y_ssmul.h"


#include "FFTBase.h"
#include "simpleRadix2IOIPFFTindex.h"
#include "simple32NIOIPFFT.h"
#include "FFTClass.h"


#define RMOD3 1024

static void LoadFFT(const std::vector<baseType>& A, double* Buffer)
{
	int carry = 0;
	int FFTIndex = 0;
	for (size_t ix = 0; ix < A.size(); ix++) {
		int temp = (A[ix] >= 0) ? A[ix] : -A[ix];   // temp is  radix 10^9 
		for (int i = 0; i < 3; i++) {
			int tmp = (temp % RMOD3);// tmp is  radix 2^10
			temp = temp / RMOD3;
			tmp = tmp + carry; carry = 0;
			if (tmp >=(RMOD3 / 2)) {  // tmp is 'balanced' radix 2^10 
				tmp = tmp - RMOD3;
				carry++;
			}

			Buffer[FFTIndex++] = (double)tmp;
		}
	}
	if (carry)
		Buffer[FFTIndex] = 1.0;
}


static void Carry(int64_t size, FFTType *Buffer)
{
	int64_t carry = 0;  // these must be 64 bit integers
	int64_t tmp = 0;

	/* conversion from 'balanced' Radix 2^10 double  to unbalanced Radix 2^10 double */
	for (size_t i = 0; i < size; i++)
	{
		tmp = (int64_t)std::round(Buffer[i]);

		tmp += carry; carry = 0;
		while (tmp < (-1 * (RMOD3 / 2))) { tmp += RMOD3; carry--; }
		while (tmp >= (RMOD3 / 2)) { tmp -= RMOD3; carry++; }
		Buffer[i] = (double)tmp;
	}
	carry = 0;
	/* carry we are still in Radix 2^10 double*/
	for (size_t i = 0; i < size; i++)
	{
		tmp = (int64_t)std::round(Buffer[i]);
		tmp += carry;  carry = 0;
		if (tmp < 0) { tmp += RMOD3; carry--; }
		Buffer[i] = double(tmp);
	}

}

/*

		we convert each radix 2^30 digit to 3 Radix 2^10 digits
		put one on factor in real part, the other in the imaginary part.
		convert each radix 2^10 digit to 'balanced representation
	  (so they are in the range [-512..511] in order to minimize noise.
		perform the FFT
		untangle the two spectras and multiply them point by point
		perform the inverse FFT (which will give a pure real result)
		handle interdigit carry for the 2^10 bit values.
		convert back to radix 30.
		we need a factor 6 larger FFT than the largets factorsize, 
		a factor 3 to handle the Radix 30 to Radix 10 conversion
		and a factor 2 to make a linear convolution from a circular one
		without overlap.
*/

yabIntType  SchStrMultiply(const yabIntType& a, const yabIntType& b)
{
	FFTClass  *fft = new FFTClass(); 

	yabIntType res((int) 0);
	
	int aSign = a.yabInt.back() >= 0 ? 1 : -1;
	int bSign = b.yabInt.back() >= 0 ? 1 : -1;

  /* sign noted for now, we will ignore the sign and multiply
     the numbers as positive numbers, then update the sign at 
     the end 
  */

	u32 max = a.yabInt.size();
	if( max < b.yabInt.size()) max = b.yabInt.size() ;

// convenient to have a sligthly large buffer, 
// when we convert back	 to radix 30
#define SLACK 2
	u32 length = fft->FindLength ( max *6)	;

	fft->Length(length);
	u32 N = fft->Status();
	if( N){
		 FFTType *re = new FFTType[N+SLACK];
		 FFTType *im = new FFTType[N+SLACK];
	 
		 for(int ix = 0 ; ix < (N+SLACK); ix++) 
		        re[ix] = im[ix] = 0.0;
		 
		 
		 
  	 LoadFFT(a.yabInt, re);
		 
		 LoadFFT(b.yabInt, im);
		 
		 fft->ForwardFFT(re, im);

    /* 
    more space efficient, we work in-place 
    processing entry [i] and entry [N-i] simultaneous.
    */
		for (u32 i = 1; i < N/2; i++)
		{
			FFTType X01Real = re[i];
			FFTType X01Imag = im[i];
			FFTType X02Real = re[N - i];
			FFTType X02Imag = im[N - i];
			FFTType X1RealiN = (X01Real + X02Real) / 2.0;
			FFTType X1Imagi = (X01Imag - X02Imag) / 2.0;
			FFTType X1ImagN = -X1Imagi;//(X02Imag - X01Imag) / 2.0;
			FFTType X2Imagi = -1 * (X01Real - X02Real) / 2;
			FFTType X2ImagN = -X2Imagi ;;//-1 * (X02Real - X01Real) / 2;
			FFTType X2RealiN = (X01Imag + X02Imag) / 2;
			FFTType X3Reali = X1RealiN * X2RealiN - X1Imagi * X2Imagi;
			FFTType X3Imagi = X1RealiN * X2Imagi + X1Imagi * X2RealiN;
			FFTType X3RealN = X2RealiN * X1RealiN - X2ImagN * X1ImagN;
			FFTType X3ImagN = X2RealiN * X1ImagN + X2ImagN * X1RealiN;
			re[i] = X3Reali;
			im[i] = X3Imagi;
			re[N-i] = X3RealN;
			im[N-i] = X3ImagN;
		}
		re[0] *= im[0];
		im[0] 	= 0.0;
		re[N/2] *= im[N/2] ;
		im[N/2] = 0.0;
						
		fft->InverseFFT(re, im);

		Carry(N, re);

		res.yabInt.clear();
		
		for (int i = 0; i < N; i += 3)// can do this, thanks to SLACK
		{
			int t0 = (int) re[i];
			int t1 = RMOD3 * (int)re[i + 1];       // these values are 0 when we reach
			int t2 = RMOD3 * RMOD3 * (int)re[i + 2];// the end of buffer, due to
			res.yabInt.push_back(t0 + t1 + t2);         // SLACK
		}

		while (res.yabInt.size() && (res.yabInt.back() == 0)) res.yabInt.pop_back();
		
		if((aSign * bSign) < 0) /* negative result*/
			res.ChangeSign();
				
		delete [] re;
		delete [] im;
		delete fft;
	}
	
	return res; 
}
