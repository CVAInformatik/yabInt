
CC = g++
CFLAGS = -g 
CPPFLAGS =  -O1  

%.o  :  %.cpp
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@


clean:
	rm *.o

y_ssmul.o : y_ssmul.cpp y_ssmul.h

y.o : y.cpp  y.h 

m.o : m.cpp m.h y.cpp y.h

f.o : f.cpp f.h y.cpp  y.h 

y_test.o : y_test.cpp  y.cpp y.h y_ssmul.cpp y_ssmul.h  m.cpp m.h  f.cpp f.h 

y :  y_test.o y.o  m.o f.o y_ssmul.o


