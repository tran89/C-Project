#include <iostream>
#include "Item.h"

Item::Item(xml_node<> * node){

	string tempString;

		for(xml_node<> *rnodes = node->first_node(); rnodes; rnodes=rnodes->next_sibling())
		{

			   tempString = rnodes->name();
			   std::cout << "	" << tempString << "  "<< rnodes->value() <<std::endl;

			   if(tempString.compare("name") == 0)
			   {
				   name = rnodes->value();
			   }

			   else if(tempString.compare("status") == 0)
			   {
				   status = rnodes->value();
			   }

			   else if(tempString.compare("writing") == 0)
			   {
				   writing = rnodes->value();
			   }

			   else if(tempString.compare("description") == 0)
			   {
				   description = rnodes->value();
			   }

			   else if(tempString.compare("trigger") == 0)
			   {
					Trigger strigger(rnodes);
				    trigger.push_back(strigger);
			   }

			   else if(tempString.compare("turnon") == 0)
			   {
				   
				   string b_tempstring;
				   for(xml_node<> *bnodes = rnodes->first_node(); bnodes; bnodes=bnodes->next_sibling())
				   {

						 b_tempstring = bnodes->name();//fixed
						 std::cout << "	" << b_tempstring << "  " << bnodes->value() <<std::endl;

						 if(b_tempstring.compare("print") == 0)
						 {
							 turnon.print= bnodes->value();
						 }
						 else if(b_tempstring.compare("action") == 0)
						 {
							turnon.actions.push_back(bnodes->value());
						 }
				  }
			   }

		}








}

Item::~Item( ){

}


