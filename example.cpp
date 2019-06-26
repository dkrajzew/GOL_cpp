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

#include <utils/options/OptionCont.h>
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
OptionCont myOptions;



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
 * help text
 * ======================================================================= */
char *help[] = {
    "Usage: options_example [option]+",
    " ",
    "Options:",
    " File Options:",
    "   -i, --input <PATH>[;<PATH>]*    The files to read; Paths are accepted, too",
    "   -f, --fromformat <FORMAT>       Which input format is assumed",
    "   -o, --output <PATH>             The name of the output file",
    "   -t, --toformat <FORMAT>         Which format shall be used for output",
    "   -r, --recurse                   Enable recursion into subfolders",
    " Image Export Options:",
    "   -w, --cell-width <PIXEL>        The width of a cell in the image (default:1)",
    "   -h, --cell-height <PIXEL>       The height of a cell in the image (default:1)",
    "   --columns <INTEGER>             The number of columns; a flat line if not given",
    " CSS Export Options:",
    "   --css.name <FORMAT_STRING>      A format string that defines how the names of",
    "                                    the css-entries are generated",
    "   --css.attribute <NAME>          The name of the attribute to assign the color to",
    " JavaScript Export Options:",
    "   --js.name <FORMAT_STRING>       A format string that defines how the variable name",
    "   --js.type <NAME>                The wanted representation type ['hex', 'triplet']",
    " Operators:",
    "   -m, --max-colors <INTEGER>      Limits the palette to this size",
    "   -n, --name <STRING>             Names the palette",
    " Report Options:",
    "   -v, --verbose                   Enable verbose reporting",
    "   --print-files                   Prints the files that are processed",
    "   --print-files.prefix <STRING>   Defines the prefix to be used",
    "   --print-files.divider <STRING>  Defines the divider between files",
    "   --print-set-options             Prints option values before processing",
    "   --print-summary                 Prints a summary after processing.",
    " Help and Version Options:",
    "   -?, --help                      Prints this screen",
    "   --version                       Prints the version",
    "",
    "Format String Placeholders:",
    "   %f: input file name (without extension)",
    "   %n: palette name (from the --name option, the information within the file, or",
    "        the filename (without the extension)",
    "   %i: current entry's index",
    "   ",
    0
};



/* =========================================================================
 * method definitions
 * ======================================================================= */
bool
getOptions(int argc, char *argv[]) {
    // files
    myOptions.add("input", 'i', new Option_FileName()); // !!! filename
    myOptions.add("recurse", 'r', new Option_Bool(false));
    myOptions.add("fromformat", 'f', new Option_String(""));
    myOptions.add("output", 'o', new Option_FileName()); // !!! filename
    myOptions.add("toformat", 't', new Option_String(""));
    // css
    myOptions.add("css.name", new Option_String(".pal_%n_%i"));
    myOptions.add("css.attribute", new Option_String("background-color"));
    // JavaScript
    myOptions.add("js.name", new Option_String("%f"));
    myOptions.add("js.type", new Option_String("hex"));
    // images
    myOptions.add("cell-width", 'w', new Option_Integer(1));
    myOptions.add("cell-height", 'h', new Option_Integer(1));
    myOptions.add("columns", new Option_Integer());
    // processing
    myOptions.add("max-colors", 'm', new Option_Integer());
    myOptions.add("name", 'n', new Option_String());
    //
    myOptions.add("print-files", new Option_Bool(false));
    myOptions.add("print-files.prefix", new Option_String("#"));
    myOptions.add("print-files.divider", new Option_String(";"));
    myOptions.add("verbose", 'v', new Option_Bool(false));
    myOptions.add("print-set-options", new Option_Bool(false));
    myOptions.add("print-summary", new Option_Bool(false));
    //
    myOptions.add("version", new Option_Bool(false));
    myOptions.add("help", '?', new Option_Bool(false));
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
    if(argc==1) {
        printAppVersion();
        printCopyrightAndContact();
        cerr << "Error: No options given." << endl;
        cerr << " Use --help for further information." << endl;
        ret = STAT_MISSING_OPTION;
    } else {
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
                //HelpPrinter::print(help);
            } else if(myOptions.getBool("version")) {
                // print version
                printAppVersion();
            } else {
                // run
                if(myOptions.getBool("print-set-options")) {
                    cout << "Currently set options:" << endl;
                    cout << myOptions << endl;
                    cout << "------------------------------------------------------------------" << endl;
                }
                // do something
            }
        } catch(std::runtime_error &e) {
            cerr << e.what() << endl;
            ret = STAT_READ_COMMENT;
        }
    }
    if(ret!=STAT_OK) {
        cerr << "Quitting (on error)." << endl;
    }
    return ret;
}


// *************************************************************************
