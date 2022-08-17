/*
 *  Authored by taylor-bsg
 */

/*
* set_power_activity
*
*
*/

#include "saif_parser.hpp"
#include "saif_db.hpp"
#include <list>


saif::SaifDB *db_global;

unsigned long long clock_cycles;
int mode; // genus = 1; opensta = 0

void print_string_list(std::list<std::string> string_list)
{
  for (std::string x: string_list) {
    std::cout << x << '/';
  }
}

void spa_SaifRecord(std::list<std::string> path_list,  std::string portname, boost::shared_ptr<saif::SaifRecord> into)
{
  if (into != NULL)
    {
      double activity = into->TC.get_d()/clock_cycles;
      if (activity > 2.0){
        std::cout << "# warning: rounding from " << activity << " to 2.0\n";
        activity = 2.0;
      }

      if (mode) {
        std::cout << "set_db pin:";
        print_string_list(path_list);
        std::cout << portname << " .lp_asserted_toggle_rate " << activity << ";";
        std::cout << "set_db pin:";
        print_string_list(path_list);
        std::cout << portname << " .lp_asserted_probability " << (into->T0.get_d() / (into->T0.get_d() + into->T1.get_d())) << ";\n";
      } else {
        std::cout << "set_power_activity -pins ";
        print_string_list(path_list);

        std::cout << portname << " -activity " << activity << " -duty " << (into->T0.get_d() / (into->T0.get_d() + into->T1.get_d()));
        std::cout << "\n";
      }
    }
}


void spa_SaifSignal(std::list<std::string> path_list, std::string portname, boost::shared_ptr<saif::SaifSignal> into)
{
  spa_SaifRecord(path_list,portname, into->data);
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

  std::map<std::string,boost::shared_ptr<saif::SaifSignal> >::iterator itr;

  /*  while (itr != instance1->signals.end()) {
      spa_SaifSignal(path_list,itr->second);
      itr++;
    }
  */
  itr = instance1->ports.begin();
  while (itr != instance1->ports.end()) {
    spa_SaifSignal(path_list,itr->first,itr->second);
    itr++;
  }
}


int main(int argc, char* argv[]) {
  if (argc < 3){
    printf("Usage:  spa <.saif file> <1=genus; 0=opensta> <divisor>\n");
    exit(-1);
  }
  std::list<std::string> path_list;
  std::cerr << "Parsing " << argv[1] << std::endl;
  sscanf(argv[2],"%d",&mode);
  sscanf(argv[3],"%lld",&clock_cycles);
  saif::SaifParser parser(argv[1]);
  saif::SaifDB db;
  parser.parse(&db);
  db_global = &db;
  
  db.top->visit(path_list,apply);
   
  //std::cout << db << std::endl;

  return 0;
}
