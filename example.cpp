/** ************************************************************************
   @project      options library
   @since		 26.06.2019
   @copyright    (c) Daniel Krajzewicz 2019-2021
   @author       Daniel Krajzewicz  
   @email        daniel@krajzewicz.de
   @license      Eclipse Public License v2.0 (EPL v2.0) 
   @brief        A very basic example for using the options.
   It's basically a "Hello World"-application
   that allows you to give a name and a greet on the command line. 
   The default for the name is "World" by default, but may be defined using 
   the option --name <NAME>, or -n <NAME> for short. The default for the 
   greet is "Hello" by default, but may be defined using the option 
   --greet <GREET>, or -g <GREET> for short. Additionally, you may 
   optionally change the number of times "<GREET> <NAME>!" is printed using 
   the option --repeat (or -r for short).
   *********************************************************************** */

/* =========================================================================
 * global configuration import
 * ======================================================================= */
#if defined _MSC_VER || defined __BORLANDC__
#include <windows_config.h>
#else
#include <config.h>
#endif



/* =========================================================================
 * included modules
 * ======================================================================= */
#include <iostream>
#include <string>
#include <cassert>
#include <fstream>

#include <utils/options/OptionsCont.h>
#include <utils/options/Option.h>
#include <utils/options/OptionsIO.h>

/* -------------------------------------------------------------------------
 * (optional) memory checking
 * ----------------------------------------------------------------------- */
#ifdef WANT_MEMCHECK
#include <foreign/nvwa/debug_new.h>
#endif // WANT_MEMCHECK



/* =========================================================================
 * used namespaces
 * ======================================================================= */
using namespace std;



/* =========================================================================
 * static member definitions
 * ======================================================================= */
OptionsCont myOptions;



/* =========================================================================
 * enum definitions
 * ======================================================================= */
enum ReturnCodes {
    STAT_OK = 0,
    STAT_MISSING_OPTION = 1,
    STAT_FALSE_OPTION = 2,
    STAT_READ_COMMENT = 3
};



/* =========================================================================
 * method definitions
 * ======================================================================= */
bool
getOptions(int argc, char *argv[]) {
    myOptions.setHelpHeadAndTail("Usage: example [option]+\n\nOptions:","");
    // function
    myOptions.add("name", 'n', new Option_String("World"));
    myOptions.setDescription("name", "Defines how to call the user.");
    myOptions.add("greet", 'g', new Option_String("Hello"));
    myOptions.setDescription("greet", "Defines how to greet.");
    myOptions.add("repeat", 'r', new Option_Integer());
    myOptions.setDescription("repeat", "Sets an optional number of repetitions.");
    //
    myOptions.add("help", '?', new Option_Bool());
    myOptions.setDescription("help", "Prints this help screen.");
    // parse
    return OptionsIO::parseAndLoad(myOptions, argc, argv, "");
}


int
main(int argc, char *argv[]) {
    ReturnCodes ret = STAT_OK;
    // print how to get the help screen
    try {
        // parse options
        if(!getOptions(argc, argv)) {
            throw std::runtime_error("Please check your options.");
        }
        // check for additional (meta) options
        if(myOptions.getBool("help")) {
            // print the help screen
            myOptions.printHelp(std::cout);
        } else {
            // do something
            std::string name = myOptions.getString("name");
            std::string greet = myOptions.getString("greet");
            int number = 1;
            if(myOptions.isSet("repeat")) {
                number = myOptions.getInteger("repeat");
            }
            for (int i=0; i<number; ++i) {
                std::cout << greet << " " << name << "!" << std::endl;
            }
        }
    } catch(std::runtime_error &e) {
        cerr << e.what() << endl;
        ret = STAT_READ_COMMENT;
    }
    if(ret!=STAT_OK) {
        cerr << "Quitting (on error)." << endl;
    }
    return ret;
}


// *************************************************************************
