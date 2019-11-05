CXXFLAGS = -g -Wall -Wextra

enigma: *.cpp *.h
	g++ $(EXXFLAGS) *.cpp *.h -o $@
