random-real : random-real.cpp
	g++ -o random-real -std=c++11 -DDSFMT_MEXP=19937 -DHAVE_INLINE -O3 -msse2 -msse4.2 -Wall random-real.cpp dSFMT-src-2.2.1/dSFMT.c -lboost_timer -lboost_system -lgsl

random-int : random-int.cpp
	g++ -o random-int -std=c++11 -DSFMT_MEXP=19937 -DHAVE_INLINE -O3 -msse4 -Wall random-int.cpp SFMT-src-1.4/SFMT.c -lboost_timer -lboost_system -lgsl
