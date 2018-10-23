#ifndef CONTAINER_H
#define CONTAINER_H

#include <iostream>
#include <vector>
#include "Trigger.h"
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"

using namespace std;
using namespace rapidxml;

class Container{
public:
   Container(xml_node<> * node);
   virtual ~Container();

   string name;
   string status;
   string description;

   std::vector<string> accept;
   std::vector<string> item;
   std::vector<Trigger> trigger;

};

#endif /* CONTAINER_H */
