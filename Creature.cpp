#include <iostream>
#include "Creature.h"

Creature::Creature(xml_node<> * node){

	string tempString;

		for(xml_node<> *rnodes = node->first_node(); rnodes; rnodes=rnodes->next_sibling()){


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

			   else if(tempString.compare("description") == 0)
			   {
				   description = rnodes->value();
			   }

			   else if(tempString.compare("vulnerability") == 0)
			  {
				   vulnerability.push_back(rnodes->value());
			  }

			   else if(tempString.compare("trigger") == 0)
			   {
			   		  		   Trigger strigger(rnodes);
			   		  		   trigger.push_back(strigger);
			   }

			   else if(tempString.compare("attack") == 0)
			   {
			  			   //Attack cAttack;
			  			   string c_tempstring;

			  			 Condition tcondition;

			  			 for(xml_node<> *cnodes = rnodes->first_node(); cnodes; cnodes=cnodes->next_sibling()){

			  				 c_tempstring = cnodes->name();
			  				std::cout << "		"<< c_tempstring << "  " << cnodes->value() <<std::endl;

			  				 if(c_tempstring.compare("condition") == 0)
			  				 {

									 for(xml_node<> *cdnodes = cnodes->first_node(); cdnodes; cdnodes=cdnodes->next_sibling())
									 {

										 c_tempstring = cdnodes->name();

										 std::cout << "			"<< c_tempstring << "  " << cdnodes->value() <<std::endl;
										 if(c_tempstring.compare("owner") == 0)
										 {
											tcondition.owner = cdnodes->value();
										 }
										 else if(c_tempstring.compare("status") == 0)
										 {
											tcondition.status = cdnodes->value();
										 }

										else if(c_tempstring.compare("has") == 0)
										{
											tcondition.has = cdnodes->value();
										}
										else if(c_tempstring.compare("object") == 0)
										{
											tcondition.object = cdnodes->value();
										}
									 }
			  				attack.condition = tcondition;
			  				 }
			  				else if(c_tempstring.compare("action") == 0)
			  				{
			  					attack.actions.push_back(cnodes->value());
			  				}
			  				else if(c_tempstring.compare("print") == 0)
			  				{
			  					attack.print = cnodes->value();
			  				}
			  			 }


			  			 }

			   }

		}


Creature::~Creature(){

}
