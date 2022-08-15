# Copyright (c) 2014-2014 Wei Song <songw@cs.man.ac.uk>
# Advanced Processor Technologies Group, School of Computer Science
# University of Manchester, Manchester M13 9PL UK
#
# This source code is free software; you can redistribute it
# and/or modify it in source code form under the terms of the GNU
# General Public License as published by the Free Software
# Foundation; either version 2 of the License, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
#
# 07/02/2014 Wei Song
#
#

INCDIRS = -I. -I/opt/local/include -L/opt/local/lib
TARGETS = saif_db.o saif_parser.o
LINK_FLAGS = -lgmp -lgmpxx

#PROFILE = -pg
CXXFLAGS = -DYYDEBUG -std=c++11 -g -O2 $(PROFILE)

BISON_EXE = /opt/local/bin/bison

all: saif.o saif_util.o $(TARGETS) test merge filter adjust_clock spa

saif.o: saif.cc saif.y saif_util.hpp saif_db.hpp
	$(CXX) $(INCDIRS) $(CXXFLAGS) -c $< -o $@

saif_util.o: saif_util.cpp saif_util.hpp saif_db.hpp
	$(CXX) $(INCDIRS) $(CXXFLAGS) -c $< -o $@

$(TARGETS): %.o:%.cpp %.hpp
	$(CXX) $(INCDIRS) $(CXXFLAGS) -c $< -o $@

test: test.cpp
	$(CXX) $< $(INCDIRS) $(CXXFLAGS) saif.o saif_util.o $(TARGETS) $(LINK_FLAGS) -o $@

filter: filter.cpp saif_db.hpp saif_db.cpp
	$(CXX) $< $(INCDIRS) $(CXXFLAGS)  saif.o saif_util.o $(TARGETS) $(LINK_FLAGS) -o $@

adjust_clock: adjust_clock.cpp saif_db.hpp saif_db.cpp
	$(CXX) $< $(INCDIRS) $(CXXFLAGS)  saif.o saif_util.o $(TARGETS) $(LINK_FLAGS) -o $@

spa: set_power_activity.cpp saif_db.hpp saif_db.cpp
	$(CXX) $< $(INCDIRS) $(CXXFLAGS)  saif.o saif_util.o $(TARGETS) $(LINK_FLAGS) -o $@

test-spa: spa
	./spa ../test6.saif 65536

filter-test: filter
	./filter ../test.saif tb_yowz tb_yowzA

filter-test-complex: filter
	./filter ../sa_tw_1.saif dut opt0 sha1 z\\\[0\\\] end dut opt0 sha1 z\\\[1\\\] end dut opt0 sha1 z\\\[2\\\] end

merge: merge.cpp saif_db.hpp saif_db.cpp
	$(CXX) $< $(INCDIRS) $(CXXFLAGS)  saif.o saif_util.o $(TARGETS) $(LINK_FLAGS) -o $@

merge-test-mismatch: merge
	./merge ../test.saif  ../test3.saif

merge-test-superset: merge
	./merge ../test.saif  ../test2.saif

merge-test-quad: merge
	./merge ../test.saif  ../test.saif ../test.saif ../test.saif

bison: saif.cc

saif.cc: saif.y
	$(BISON_EXE) $<

.PHONY: clean

clean:
	-rm -f *.o *.output test
	-rm -f saif.cc saif.hh stack.hh location.hh position.hh filter merge


