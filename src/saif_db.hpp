/*
* Copyright (c) 2014-2014 Wei Song <songw@cs.man.ac.uk>
* Advanced Processor Technologies Group, School of Computer Science
* University of Manchester, Manchester M13 9PL UK
*
* This source code is free software; you can redistribute it
* and/or modify it in source code form under the terms of the GNU
* General Public License as published by the Free Software
* Foundation; either version 2 of the License, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
*/

/*
* A saif database
* 06/02/2014 Wei Song
*
*
*/

#ifndef SAIF_DB_H_
#define SAIF_DB_H_

#include <list>
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <gmpxx.h>
#include <boost/shared_ptr.hpp>


extern void print_string_list(std::list<std::string> string_list);
namespace saif {

  // a switch record
  class SaifRecord {
  public:
    SaifRecord();
    mpz_class T0;
    mpz_class T1;
    mpz_class TX;
    mpz_class TZ;
    mpz_class TC;
    mpz_class IG;
    mpz_class TB;

    virtual std::ostream& streamout ( std::ostream& ) const;
  };
  
  inline std::ostream& operator<< ( std::ostream& os, const SaifRecord& rhs) {
    return rhs.streamout(os);
  }

  // a signal (single or multi bits)
  class SaifSignal {
  public:
    std::map<int, boost::shared_ptr<SaifSignal> > bits;
    boost::shared_ptr<SaifRecord> data;

    virtual std::ostream& streamout ( std::ostream& ) const;
    virtual std::ostream& streamout ( std::ostream&, const std::string&, const std::string&, unsigned int indent = 0) const;
  };

  inline std::ostream& operator<< ( std::ostream& os, const SaifSignal& rhs) {
    return rhs.streamout(os);
  }

  // instance
  class SaifInstance {
  public:
    std::map<std::string, boost::shared_ptr<SaifSignal> > signals;
    std::map<std::string, boost::shared_ptr<SaifSignal> > ports;
    std::map<std::string, boost::shared_ptr<SaifInstance> > instances;
    std::string module_name;

    virtual std::ostream& streamout ( std::ostream&, const std::string&, unsigned int indent = 0) const;
    virtual std::ostream& streamout ( std::ostream& ) const;


    boost::shared_ptr<SaifInstance> lookup(std::list<std::string> path_list)
    {
      std::map<std::string, boost::shared_ptr<SaifInstance> >::iterator it;
					      
      boost::shared_ptr<SaifInstance> curInstance = NULL;

      for (std::string x: path_list) {
	if (curInstance == NULL) {
	  it = instances.find(x);
	  if (it != instances.end())
	    curInstance = instances[x];
	  else {
	    // std::cout << "WARNING: failed to find" << x << " in "; print_string_list(path_list);
	    return NULL;
	  }
	}
	else {
	  it = curInstance->instances.find(x);
	  if (it != curInstance->instances.end())
	    curInstance = (curInstance->instances[x]);
	  else {
	    // std::cout << "WARNING: failed to find" << x << " in "; print_string_list(path_list);
	    return NULL;
	  }
	}
      }

      return curInstance;
    }
    
    void visit(std::list<std::string> path_list, void (*fn)(std::list<std::string>))
    {
      std::map<std::string,boost::shared_ptr<SaifInstance> >::iterator itr = instances.begin();

      // visit all of the children
      while (itr != instances.end()) {
	path_list.push_back((*itr).first);
	(*itr).second->visit(path_list,fn);
	path_list.pop_back();
	++itr;
      }
      fn (path_list);
    }
    
    void filter(std::list<std::string> filter_list)
    {
      std::map<std::string,boost::shared_ptr<SaifInstance> >::iterator itr = instances.begin();
      /*
      std::cout << "filtering ";
      for (std::string x: filter_list) {
	std::cout << x << ' ';
      }

      std::cout << "\n";
      */
      //std::cout << instances;
      
      if (filter_list.empty())
	return;
      
      while (itr != instances.end()) {
	if ((*itr).first == filter_list.front()) {
	  //	  std::cout << "natched " << (*itr).first << "\n";
	  std::string temp = filter_list.front();
	    filter_list.pop_front();
	    (*itr).second->filter(filter_list);
	    filter_list.push_front(temp);
	    ++itr;
	} else {
	  //std::cout << "nuke " << (*itr).first << " vs " << filter_list.front() << "\n";
	  itr = instances.erase(itr);
	}
      }
    }
  };

  inline std::ostream& operator<< ( std::ostream& os, const SaifInstance& rhs) {
    return rhs.streamout(os);
  }

  // database
  class SaifDB {
  public:
    std::string version;
    std::string direction;
    std::string date;
    std::string vendor;
    std::string program_name;
    std::string tool_version;
    std::string divider;
    mpz_class duration;
    std::pair<mpz_class, std::string> timescale;

    boost::shared_ptr<SaifInstance> top;
    std::string top_name;

    virtual std::ostream& streamout ( std::ostream& ) const;
  };

  inline std::ostream& operator<< ( std::ostream& os, const SaifDB& rhs) {
    return rhs.streamout(os);
  }

}

#endif
