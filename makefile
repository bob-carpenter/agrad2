test:	test.cpp $(wildcard src/stan/diff/rev/*.hpp)
	clang++ -O3 -Wall -I src test.cpp -o test

etest:	etest.cpp $(wildcard src/stan/diff/rev/*.hpp)
	clang++ -O3 -Wall -I src etest.cpp -o etest

clean:	
	rm test
	rm etest