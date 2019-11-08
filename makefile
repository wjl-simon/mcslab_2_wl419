CXXFLAGS = -g -Wall -Wextra

OBJ = plugboard.o rotor.o reflector.o helper.o

enigma: main.cpp $(OBJ:.o=.h) $(OBJ:.o=.cpp) errors.h
	g++ $(CXXFLAGS) main.cpp $(OBJ:.o=.h) $(OBJ:.o=.cpp) errors.h -o $@

# enigma: $(OBJ) main.o 
# 	g++ $(OBJ) -o $@

# main.o: main.cpp errors.h $(OBJ:.o=.h)
# 	g++ $(CXXFLAGS) main.cpp errors.h

# #%.o: %.cpp %.h errors.h helper.h
# #	g++ $(CXXFLAGS) %.cpp %.h errors.h helper.h 

# #%.o: %.cpp %.h errors.h helper.h
# #	g++ $(CXXFLAGS) %.cpp %.h errors.h helper.h

# plugboard.o: plugboard.cpp plugboard.h errors.h
# 	g++ $(CXXFLAGS) plugboard.cpp plugboard.h errors.h helper.h

# rotor.o: rotor.cpp rotor.h errors.h helper.h
# 	g++ $(CXXFLAGS) rotor.cpp rotor.h errors.h helper.h

# reflector.o: reflector.cpp reflector.h errors.h helper.h
# 	g++ $(CXXFLAGS) reflector.cpp reflector.h errors.h helper.h

# helper.o: errors.h helper.h helper.cpp
# 	g++ $(CXXFLAGS) errors.h helper.h helper.cpp

