cryptopp-test: cryptopp-challenger.cpp cryptopp-keygen.cpp cryptopp-test.cpp
	g++ -O3 cryptopp-challenger.cpp -o cryptopp-challenger -lcryptopp -pthread
	g++ -O3 cryptopp-keygen.cpp -o cryptopp-keygen -lcryptopp -pthread 
	g++ -O3 cryptopp-test.cpp -o cryptopp-test -lcryptopp -pthread

clean: 
	rm -f cryptopp-challenger cryptopp-keygen cryptopp-test p pk ptsize g answer question
