#ifndef TRIGGER_H
#define TRIGGER_H


#include <iostream>
#include <vector>
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"

using namespace std;
using namespace rapidxml;

struct Condition{
	string owner;
	string status;
	string has;
	string object;
};


class Trigger{
public:
   Trigger(xml_node<> * node);
   virtual ~Trigger();

   string type; //single or permanent
   //string command;
   string print;

   std::vector<string> commands;
   std::vector<Condition> condition;
   std::vector<string> actions;



};

#endif /* TRIGGER_H */
