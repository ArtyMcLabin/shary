compile = g++ -Wall -std=c++1z -O3
link_boost = -lboost_system -lboost_filesystem
	
deploy: link
	cp shary /usr/bin/
	
link: main.o engine.o
	$(compile) engine.o main.o $(link_boost) -o shary
	
main.o: main.cpp
	$(compile) main.cpp -c

engine.o: engine.h engine.cpp
	$(compile) engine.cpp -c
	
clean:
	rm -f *.o && rm -f shary && rm -f shary.exe && rm -f shary.zip
	
zip:
	zip makefile main.cpp engine.h engine.cpp

remake: clean deploy
	
