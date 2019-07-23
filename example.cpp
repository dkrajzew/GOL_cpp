/* *************************************************************************
   project:      Greyrat's Options Library
   module:       example.cpp
   purpose:      The main module
   begin:        26.06.2019
   copyright:    (C) Daniel Krajzewicz
   email:        daniel@krajzewicz.de
   *************************************************************************
   Version:
   Remarks:
   *************************************************************************
   ToDo:
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
 * definitions
 * ======================================================================= */
#define APPNAME "options_example"
#define APPVERSION "1.0.0"



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
    myOptions.setHelpHeadAndTail("Usage: options_example [option]+\n\nOptions:\n","");
    // function
    myOptions.add("name", 'n', new Option_String("World"));
    myOptions.setDescription("name", "Defines how to call the user.");
    myOptions.add("greet", 'g', new Option_String("Hello"));
    myOptions.setDescription("greet", "Defines how to greet.");
    myOptions.add("repeat", 'r', new Option_Integer());
    myOptions.setDescription("repeat", "Sets an optional number of repetitions.");
    //
    myOptions.add("version", new Option_Bool(false));
    myOptions.setDescription("version", "Prints the version.");
    myOptions.add("help", '?', new Option_Bool(false));
    myOptions.setDescription("help", "Prints this help screen.");
    // parse
    return OptionsIO::parseAndLoad(myOptions, argc, argv, "");
}


void
printAppVersion() {
    cout << APPNAME << " " << APPVERSION << endl;
}


void
printCopyrightAndContact() {
    cout << " (c) Daniel Krajzewicz" << endl;
    cout << "  http://www.krajzewicz.de/" << endl << endl;
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
            printAppVersion();
            printCopyrightAndContact();
            myOptions.printHelp(std::cout);
        } else if(myOptions.getBool("version")) {
            // print version
            printAppVersion();
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
