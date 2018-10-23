#include <iostream>
#include "Trigger.h"

Trigger::Trigger(xml_node<> * node)
{


	string tempString;

	for(xml_node<> *rnodes = node->first_node(); rnodes; rnodes=rnodes->next_sibling()){


	tempString = rnodes->name();
	std::cout << "		 " << tempString << "  "<< rnodes->value() <<std::endl;

		   if(tempString.compare("type") == 0)
		   {
			  type = rnodes->value();
		   }

		   else if(tempString.compare("command") == 0)
		   {
			   commands.push_back(rnodes->value());
		   }

		   else if(tempString.compare("print") == 0)
		   {
			   print = rnodes->value();
		   }

		   else if(tempString.compare("action") == 0)
		  {
		   		 		  	actions.push_back(rnodes->value());
		  }


		   else if(tempString.compare("condition") == 0)
		   {
		  			   Condition tcondition;
		  			   string c_tempstring;

		  			 for(xml_node<> *cnodes = rnodes->first_node(); cnodes; cnodes=cnodes->next_sibling()){

		  				 c_tempstring = cnodes->name();
		  				 std::cout << "			" << c_tempstring << "  " << cnodes->value() <<std::endl;

		  				 if(c_tempstring.compare("owner") == 0)
		  				 {
		  					tcondition.owner = cnodes->value();
		  				 }
		  				 else if(c_tempstring.compare("status") == 0)
		  				 {
		  					tcondition.status = cnodes->value();
		  				 }

		  				else if(c_tempstring.compare("has") == 0)
		  				{
		  					tcondition.has = cnodes->value();
		  				}
		  				else if(c_tempstring.compare("object") == 0)
		  				{
		  					tcondition.object = cnodes->value();
		  				}
		  			 }

		  			 condition.push_back(tcondition);

		   }



	}


}

Trigger::~Trigger( ) {

}



