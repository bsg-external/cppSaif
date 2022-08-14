/*
 *  Authored by taylor-bsg
 */

/*
* SAIF Adjust Clock Function
*
* Say you simulated the design at 50 MHz but you want to time at 1 GHz.
* What do you do?  multiply up the toggle count field! 
*
*
*/

#include "saif_parser.hpp"
#include "saif_db.hpp"
#include <list>


saif::SaifDB *db_global;

long clock_adjust;

void print_string_list(std::list<std::string> string_list)
{
  for (std::string x: string_list) {
    std::cout << x << ' ';
  }
  std::cout << '\n';
}


void adjust_clock_SaifRecord(std::list<std::string> path_list, boost::shared_ptr<saif::SaifRecord> into)
{
  if ((into != NULL))
    into->TC = into->TC * clock_adjust;
}

void adjust_clock_SaifSignal(std::list<std::string> path_list, boost::shared_ptr<saif::SaifSignal> into)
{
    std::map<int,boost::shared_ptr<saif::SaifSignal> >::iterator itr2,itr = into->bits.begin();

    while (itr != into->bits.end()) {
      int bitnum = itr->first;
      adjust_clock_SaifSignal(path_list,itr->second);
      itr++;
    }

    adjust_clock_SaifRecord(path_list,into->data);
}

void apply(std::list<std::string> path_list)
{
  // lookup instance
  boost::shared_ptr<saif::SaifInstance> instance1  = db_global->top->lookup(path_list);

  if (path_list.empty())
    return;
  
  if (instance1 == NULL) {
    std::cout << "ERROR: Mismatching path:"; print_string_list(path_list);
    exit(-1);
  }

  std::map<std::string,boost::shared_ptr<saif::SaifSignal> >::iterator itr2,itr = instance1->signals.begin();

  while (itr != instance1->signals.end()) {
      adjust_clock_SaifSignal(path_list,itr->second);
      itr++;
    }

  itr = instance1->ports.begin();
  while (itr != instance1->ports.end()) {
    adjust_clock_SaifSignal(path_list,itr->second);
    itr++;
  }
}



int main(int argc, char* argv[]) {
  if (argc < 2){
    printf("Usage:  adjust_clock <.saif file> <clock multiplier>\n");
    exit(-1);
  }
  std::list<std::string> path_list;
  std::cerr << "Parsing " << argv[1] << std::endl;
  saif::SaifParser parser(argv[1]);
  saif::SaifDB db;
  parser.parse(&db);
  db_global = &db;

  sscanf(argv[2],"%ld",&clock_adjust);
  
  db.top->visit(path_list,apply);
   
  std::cout << db << std::endl;

  return 0;
}
