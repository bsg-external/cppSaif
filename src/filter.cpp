
/*
* SAIF Filter Function
*
* taylor-bsg
*/

#include <iostream>
#include <fstream>

#include "saif_parser.hpp"
#include "saif_db.hpp"
#include <list>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>


int main(int argc, char* argv[]) {
  if (argc == 1) {
    printf("Usage: filter <input .saif file> [ filter_list ] [ end filter_list ]*\n");
    printf("filter_list is a sequence of hierarchical instance names within the path to keep signals for\n");
    printf("note that ports, nets and instances at the higher levels will not be filtered\n");
     exit(-1);
  } 
  
   saif::SaifParser parser(argv[1]);
   saif::SaifDB db;
   
   fprintf(stderr,"Start Parsing....\n");
   parser.parse(&db);
  
   int fileNum = 0;
   int curArg = 2;
 
   while (curArg < argc)
     {
      pid_t pid;
      int status=0;

      std::list <std::string> filter_list;

      for (; curArg < argc; curArg++)
       {
         if (strcmp("end",argv[curArg]) == 0) 
           { curArg++; break; }

         std::string filter = std::string(argv[curArg]);
         filter_list.push_back(filter);
       }
 
      std::cerr << "filter list:";
      for (std::string x: filter_list) {
       std::cerr << x << ' ';
      }
      std::cerr << "\n";

      fprintf(stderr,"Start Filter....\n");

      if (fork() == 0)
       {
         std::ofstream myfile;
         char filename[500];
         sprintf(filename,"%s_%d",argv[1],fileNum);
         myfile.open(filename);

         db.top->filter(filter_list);
   
         myfile << db << std::endl;
         myfile.close();
         std::cerr << "Generated " << filename << "\n";
         exit(0);
       }
      else
       wait(&status);
     
      fileNum++;
     
      return 0;
     }
}
