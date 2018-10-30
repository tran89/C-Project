#ifndef ITEM_H
#define ITEM_H


#include <iostream>
#include <vector>
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"
#include "Trigger.h"

using namespace std;
using namespace rapidxml;

struct Turnon{
	string print;
	std::vector<string> actions;
};



class Item{
public:
   Item(xml_node<> * node);
   virtual ~Item();


   string name;
   string status;
   string description;
   string writing;



   std::vector<Trigger> trigger;

};

#endif /* ITEM_H */
