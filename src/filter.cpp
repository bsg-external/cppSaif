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
* SAIF Filter Function
*
*
*/

#include "saif_parser.hpp"
#include "saif_db.hpp"
#include <list>

int main(int argc, char* argv[]) {
  saif::SaifParser parser(argv[1]);

  std::list <std::string> filter_list;

  for (int i = 2; i < argc; i++)
    {
      std::string filter = std::string(argv[i]);
      filter_list.push_back(filter);
    }
  /*
  std::cout << "filter list:";
  for (std::string x: filter_list) {
    std::cout << x << ' ';
  }
  std::cout << "\n";
  */
  saif::SaifDB db;
  
  parser.parse(&db);

  db.top->filter(filter_list);
  
  std::cout << db << std::endl;

  return 0;
}
