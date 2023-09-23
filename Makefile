CXX=g++
CXXFLAGS=-g -Wall -std=c++11

all: twitter

# Finish this target by adding other dependencies and 
# the action to compile the final twitter executable
twitter: twitter.o user.o tweet.o datetime.o twiteng.o handler.o cmdhandler.o util.o
	$(CXX) $(CXXFLAGS) twitter.o user.o tweet.o datetime.o twiteng.o handler.o cmdhandler.o util.o -o twitter

# Complete	
twitter.o: twitter.cpp twiteng.h cmdhandler.h util.h
	$(CXX) $(CXXFLAGS) -c twitter.cpp -o twitter.o

# Add other rules/targets
twiteng.o : twiteng.cpp twiteng.h util.h user.h tweet.h
	$(CXX) $(CXXFLAGS) -c twiteng.cpp -o twiteng.o

datetime.o : datetime.cpp datetime.h
	$(CXX) $(CXXFLAGS) -c datetime.cpp -o datetime.o

tweet.o : tweet.cpp tweet.h twiteng.h user.h tweet.h
	$(CXX) $(CXXFLAGS) -c tweet.cpp -o tweet.o

user.o : user.cpp user.h twiteng.h tweet.h
	$(CXX) $(CXXFLAGS) -c user.cpp -o user.o

handler.o : handler.cpp handler.h
	$(CXX) $(CXXFLAGS) -c handler.cpp -o handler.o

cmdhandler.o : cmdhandler.cpp cmdhandler.h util.h handler.h
	$(CXX) $(CXXFLAGS) -c cmdhandler.cpp -o cmdhandler.o

util.o : util.cpp util.h
	$(CXX) $(CXXFLAGS) -c util.cpp -o util.o

clean:
	rm -f *.o twitter *.feed


# You should assume only source files (not compiled object code) exists when you run the make command.

# Each .cpp should be compiled to a .o file.
# All the .o files can be linked to an exectuable.
# Each of these requires a target/rule.

# Dependencies simply say:  if this (dependency) file changes,
# then we need to rerun this action/rule (to produce this output). 

# If user.h changed, what .cpp files would you want to recompile to their corresponding .o files? 
# Certainly user.cpp but also any .cpp  that #includes user.h.
#  Thus, all those .o  (cpp) rules should be dependent on user.h


