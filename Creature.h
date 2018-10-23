#ifndef CREATURE_H
#define CREATURE_H

#include <iostream>
#include <vector>
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"

#include "Trigger.h"
using namespace std;

using namespace rapidxml;
struct Attack{
	Condition condition;
	string print;
	vector <string> actions;
};

class Creature {
public:
   Creature(xml_node<> * node);
   virtual ~Creature();

   string name;
   string status;
   string description;


   std::vector<string> vulnerability;
   std::vector<Trigger> trigger;

   Attack attack;


};

#endif /* CREATURE_H */
