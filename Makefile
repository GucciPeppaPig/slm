CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -MMD -MP
OBJS     = src/main.o src/model.o src/generator.o

slm: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f slm src/*.o src/*.d

.PHONY: clean
-include $(OBJS:.o=.d)
