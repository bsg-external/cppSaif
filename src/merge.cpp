/*
 *  Authored by taylor-bsg
 */

/*
* SAIF Filter Function
*
*
*/

#include "saif_parser.hpp"
#include "saif_db.hpp"
#include <list>


saif::SaifDB *db_global;
saif::SaifDB *db2_global;


void print_string_list(std::list<std::string> string_list)
{
  for (std::string x: string_list) {
    std::cout << x << ' ';
  }
  std::cout << '\n';
}


void merge_SaifRecord(std::list<std::string> path_list, std::string name, boost::shared_ptr<saif::SaifRecord> into, boost::shared_ptr<saif::SaifRecord> from)
{
  if ((into != NULL))
    if (from == NULL)
      {
	std::cout << "ERROR: missing saif record" << name; print_string_list(path_list);
	return;
      }

  into->T0 += from->T0;
  into->T1 += from->T1;
  into->TX += from->TX;
  into->TZ += from->TZ;
  into->TC += from->TC;
  into->IG += from->IG;
  into->TB += from->TB;
}

void merge_SaifSignal(std::list<std::string> path_list, std::string name, boost::shared_ptr<saif::SaifSignal> into, boost::shared_ptr<saif::SaifSignal> from)
{
    std::map<int,boost::shared_ptr<saif::SaifSignal> >::iterator itr2,itr = into->bits.begin();

    while (itr != into->bits.end()) {
      int bitnum = itr->first;
      itr2 = from->bits.find(bitnum);
      if (itr2 == from->bits.end())
	{
	  std::cout << "ERROR: missing bit " << bitnum << " in " << name << " "; print_string_list(path_list);
	  continue;
	}
      merge_SaifSignal(path_list,name,itr->second, itr2->second);
      itr++;
    }

    merge_SaifRecord(path_list,name,into->data, from->data);
}

void apply(std::list<std::string> path_list)
{
  //std::cout << "apply:"; print_string_list(path_list);

  // lookup instance
  boost::shared_ptr<saif::SaifInstance> instance1  = db_global->top->lookup(path_list);
  boost::shared_ptr<saif::SaifInstance> instance2  = db2_global->top->lookup(path_list);

  if (path_list.empty())
    return;
  
  if (instance1 != NULL) {
    if (instance2 == NULL) {
      // this is a fatal error, because when we merge we will be increasing the time
      // but if there are missing instances, then we will not be proportionally increasing
      // the time of the missing instances. This is a feature that could be supported
      // but should require an explicit flag
      std::cout << "ERROR: Mismatching path:"; print_string_list(path_list);
      exit(-1);
      return;
    }
    
    // print out wires
    std::map<std::string,boost::shared_ptr<saif::SaifSignal> >::iterator itr2,itr = instance1->signals.begin();
    while (itr != instance1->signals.end()) {
      std::string name = itr->first;
      itr2 = instance2->signals.find(name);
      if (itr2 == instance2->signals.end())
	{
	  std::cout << "ERROR: mismatching wire:" << name; print_string_list(path_list);
	  continue;
	}
      merge_SaifSignal(path_list,name,itr->second, itr2->second);
      itr++;
    }

    // print out ports
    itr = instance1->ports.begin();
    while (itr != instance1->ports.end()) {
      std::string name = itr->first;
      itr2 = instance2->ports.find(name);
      if (itr2 == instance2->ports.end())
	{
	  std::cout << "ERROR: mismatching port:" << name; print_string_list(path_list);
	  continue;
	}
      merge_SaifSignal(path_list,name,itr->second, itr2->second);
      itr++;
    }
  } else {
    // this is a fatal error, because when we merge we will be increasing the time
    // but if there are missing instances, then we will not be proportionally increasing
    // the time of the missing instances. This is a feature that could be supported
    // but should require an explicit flag
    std::cout << "ERROR: mismatching instance "; print_string_list(path_list);
    std::cout << "\n";
    exit(-1);
    return;
  }
}


// this will merge data from different runs of the same hardware
//
// usage: merge <merge into> <merge from #1> <merge from #2> <merge from #3> ....
//
//

int main(int argc, char* argv[]) {
  if (argc < 2){
    printf("Usage:  merge <dest .saif file> <src .saif file 1> <src .saif file 2> .. <src .saif file N>\n");
    printf("  Adds activity data from src files into dest files; and increases time scale accordingly\n");
    printf("  note: if a signal is in the src .saif files but not in the dest, it is ommitted; it is assume this signal is not of interest.\n");
    printf("        it is an error if a signal is in the dest file but not in the source file because it would correspond to missing data");
    exit(-1);
  }

  saif::SaifParser parser(argv[1]);
  saif::SaifDB db;
  parser.parse(&db);
  db_global = &db;

  
  for (int i = 2; i < argc; i++)
    {
      saif::SaifParser parser2(argv[i]);
      saif::SaifDB db2;
      db2_global = &db2;
      parser2.parse(&db2);
      //std::cout << db2 << std::endl;
      std::list<std::string> path_list;
      db.top->visit(path_list,apply);
      db.duration += db2.duration;
    }
  
  std::cout << db << std::endl;

  return 0;
}
