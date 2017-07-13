#
# Makefile -- featsel's Makefile
#
#    This file is part of the featsel framework
#    Copyright (C) 2016  Marcelo S. Reis
#
#
#    If you use featsel in your publication, we kindly ask you to acknowledge us
#    by citing the paper that describes this framework:
#
#    M.S. Reis, G. Estrela, C.E. Ferreira and J. Barrera
#    "featsel: A Framework for Benchmarking of
#    Feature Selection Algorithms and Cost Functions"
#    https://github.com/msreis/featsel
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

IDIR = ../

CXXFLAGS = -fopenmp -O2 -g -Wall -fmessage-length=0

OBJS =		src/Element.o src/ElementSet.o src/ElementSubset.o \
      src/ROBDD.o src/Vertex.o src/Partition.o \
      src/PartitionNode.o src/functions/PartCost.o \
# %template_class%
      src/Collection.o src/Solver.o src/CostFunction.o \
      src/algorithms/UCurveToolBox.o \

TOBJS =		test/ElementTest.o test/ElementSetTest.o \
      test/ROBDDTest.o test/PartitionTest.o \
      test/PartitionNodeTest.o test/functions/PartCostTest.o \
      test/algorithms/UCurveToolBoxTest.o \
# %template_test%
      test/ElementSubsetTest.o test/CollectionTest.o \
      test/functions/MeanConditionalEntropyMock.o \
      test/algorithms/SFFSMock.o test/algorithms/SBFSMock.o \

LIBS = -lm

TARGET =	bin/featsel

TEST =		bin/featselTest

$(TARGET):	featsel.o $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) src/featsel.o $(OBJS) $(LIBS) \
			src/parsers/XmlParser.cpp src/parsers/XmlScanner.cpp \
			src/parsers/XmlParserDriver.cpp \
			src/parsers/DatParserDriver.cpp

$(TEST):	featselTest.o $(TOBJS) $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TEST) test/featselTest.o $(TOBJS) $(OBJS) $(LIBS) \
			src/parsers/XmlParser.cpp src/parsers/XmlScanner.cpp \
			src/parsers/XmlParserDriver.cpp \
			src/parsers/DatParserDriver.cpp


featsel.o:
	$(CXX) $(CXXFLAGS)	-c -o src/featsel.o src/featsel.cpp

featselTest.o:
	$(CXX) $(CXXFLAGS)	-c -o test/featselTest.o test/featselTest.cpp

help:
	groff -man -Tascii docs/featsel.1 > docs/featsel.txt

bison:
	bison	-o src/parsers/XmlParser.cpp src/parsers/XmlParser.yy

flex:
	flex	-o src/parsers/XmlScanner.cpp src/parsers/XmlScanner.ll


all:	bison flex featsel.cpp $(TARGET) featselTest.cpp $(TEST) help


featsel.cpp:
	bin/build_featsel_main_file.pl

featselTest.cpp:
	bin/build_featselTest_main_file.pl


test:	$(TEST)

.PHONY: clean

clean:
	rm -f $(OBJS) $(TARGET)
	rm -f $(TOBJS) $(TEST)
	find . -type f -name '*.o' -exec rm {} +
	rm -f input/tmp/*
	rm -f output/*
