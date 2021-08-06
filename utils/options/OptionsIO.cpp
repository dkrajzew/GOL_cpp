/** ************************************************************************
   @project      options library
   @file         OptionsIO.cpp
   @since        03.03.2004
   @author       Daniel Krajzewicz (daniel@krajzewicz.de)
   @copyright    Eclipse Public License v2.0 (EPL v2.0), (c) Daniel Krajzewicz 2004-2021
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
#include "OptionsCont.h"
#include "OptionsIO.h"
#include "OptionsParser.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#ifdef USE_XERCES_XML
#include "OptionsXercesHandler.h"
#include <util/PlatformUtils.hpp>
#include <sax2/SAX2XMLReader.hpp>
#include <sax2/XMLReaderFactory.hpp>
using namespace XERCES_CPP_NAMESPACE;
#endif // USE_XERCES_XML

/* -------------------------------------------------------------------------
 * (optional) memory checking
 * ----------------------------------------------------------------------- */
#ifdef DEBUG_ALLOC_NVWA
#include <debug_new.h>
#endif



/* =========================================================================
 * used namespaces
 * ======================================================================= */
using namespace std;



/* =========================================================================
 * method definitions
 * ======================================================================= */
bool
OptionsIO::parseAndLoad(OptionsCont &into, int argc, char **argv,
                        const std::string &configOptionName,
                        bool , bool ) {
    bool ok = OptionsParser::parse(into, argc, argv);
    if(ok) into.remarkUnset();
    if(ok) ok = load(into, configOptionName);
    if(ok) into.remarkUnset();
    if(ok) ok = OptionsParser::parse(into, argc, argv);
    return ok;
}


bool
OptionsIO::load(OptionsCont &into, const std::string &configOptionName) {
    if(configOptionName.length()==0) {
        return true;
    }
    if(!into.isSet(configOptionName)) {
        return true;
    }
#ifdef USE_XERCES_XML
    try {
        XMLPlatformUtils::Initialize();
    } catch(const XMLException& toCatch) {
        cerr << "Error during initialization! Message:" << std::endl
             << OptionsXercesHandler::convert(toCatch.getMessage()) << endl;
        return false;
    }
    SAX2XMLReader* parser = XMLReaderFactory::createXMLReader();
    string file = into.getString(configOptionName);
    OptionsXercesHandler handler(into, file);
    parser->setContentHandler(&handler);
    parser->setErrorHandler(&handler);
    try {
        parser->parse(file.c_str());
    } catch(const XMLException& e) {
        std::cerr << std::endl << "Error during parsing: '" << file << std::endl
             << "Exception message is:" << std::endl
             << OptionsXercesHandler::convert(e.getMessage()) << "\n" << std::endl;
        XMLPlatformUtils::Terminate();
        return false;
    } catch(...) {
        std::cerr << std::endl << "Error: Unexpected exception during parsing: '" << file << std::endl;
        XMLPlatformUtils::Terminate();
        return false;
    }
    return !handler.errorOccured();
#else
    // consume "into"
    into.contains("foo");
    return true;
#endif // USE_XERCES_XML
}


void 
OptionsIO::writeXMLConfiguration(const std::string &configName, const OptionsCont &options) {
    std::vector<std::string> optionNames = options.getSortedOptionNames();
    std::ofstream fdo(configName.c_str());
    fdo << "<configuration>" << std::endl;
    for(std::vector<std::string>::const_iterator i=optionNames.begin(); i!=optionNames.end(); ++i) {
        std::string optionName = *i;
        if(options.isSet(optionName) && !options.isDefault(optionName)) {
            fdo << "   <" << optionName << ">" << options.getValueAsString(optionName) << "</" << optionName << std::endl;
        }
    }
    fdo << "</configuration>" << std::endl;
    fdo.close();
}


void 
OptionsIO::writeXMLTemplate(const std::string &configName, const OptionsCont &options) {
    std::vector<std::string> optionNames = options.getSortedOptionNames();
    std::ofstream fdo(configName.c_str());
    fdo << "<configuration>" << std::endl;
    for(std::vector<std::string>::const_iterator i=optionNames.begin(); i!=optionNames.end(); ++i) {
        std::string optionName = *i;
        fdo << "   <" << optionName << "></" << optionName << std::endl;
    }
    fdo << "</configuration>" << std::endl;
    fdo.close();
}


void 
OptionsIO::printSetOptions(std::ostream &os, const OptionsCont &options, bool includeSynonyms, bool shortestFirst, bool skipDefault) {
    std::vector<std::string> optionNames = options.getSortedOptionNames();
    for(std::vector<std::string>::const_iterator i=optionNames.begin(); i!=optionNames.end(); ++i) {
        std::string optionName = *i;
        if(!options.isSet(optionName)) {
            continue;
        }
        if(skipDefault && options.isDefault(optionName)) {
            continue;
        }
        std::vector<std::string> synonyms = options.getSynonyms(optionName);
        if(shortestFirst) {
            std::reverse(synonyms.begin(), synonyms.end());
        }
        std::vector<std::string>::const_iterator j = synonyms.begin();
        os << (*j++);
        if(includeSynonyms) {
            if(j!=synonyms.end()) {
                os << " (";
                while(j!=synonyms.end()) {
                    os << (*j++);
                    if(j!=synonyms.end()) {
                        os << ", ";
                    }
                }
                os << ")";
            }
        }
        os << ": " << options.getValueAsString(optionName);
        if(options.isDefault(optionName)) {
            os << " (default)";
        }
        os << std::endl;
    }
}


void
OptionsIO::printHelp(std::ostream &os, const OptionsCont &options, size_t maxWidth, size_t optionIndent, size_t divider, size_t sectionIndent) {
    std::vector<std::string> optionNames = options.getSortedOptionNames();
    const std::string &helpHead = options.getHelpHead();
    const std::string &helpTail = options.getHelpTail();
    // compute needed width
    size_t optMaxWidth = 0;
    for(std::vector<std::string>::const_iterator i=optionNames.begin(); i!=optionNames.end(); ++i) {
        std::string optNames = getHelpFormattedSynonyms(options, *i);
        optMaxWidth = optMaxWidth<optNames.length() ? optNames.length() : optMaxWidth;
    }
    // build the indent
    std::string optionIndentSting, sectionIndentSting;
    for(size_t i=0; i<optionIndent; ++i) {
        optionIndentSting += " ";
    }
    for(size_t i=0; i<sectionIndent; ++i) {
        sectionIndentSting += " ";
    }
    // 
    if(helpHead.length()!=0) {
        os << helpHead << std::endl;
    }
    std::string lastSection;
    for(std::vector<std::string>::const_iterator i=optionNames.begin(); i!=optionNames.end(); ++i) {
        std::string name = *i;
        // check whether a new section starts
        std::string optSection = options.getSection(name);
        if(lastSection!=optSection) {
            lastSection = optSection;
            os << sectionIndentSting << lastSection << std::endl;
        }
        // write the option
        std::string optNames = getHelpFormattedSynonyms(options, name);
        // write the divider
        os << optionIndentSting << optNames;
        size_t owidth = optNames.length();
        // write the description
        size_t beg = 0;
        std::string desc = options.getDescription(name);
        size_t offset = divider+optMaxWidth-owidth;
        size_t startCol = divider+optMaxWidth+optionIndent;
        while(beg<desc.length()) {
            for(size_t j=0; j<offset; ++j) {
                os << " ";
            }
            if(maxWidth-startCol>=desc.length()-beg) {
                os << desc.substr(beg);
                beg = desc.length();
            } else {
                size_t end = desc.rfind(' ', beg+maxWidth-startCol);
                os << desc.substr(beg, end);
                beg = end;
                os << std::endl;
            }
            startCol = divider+optMaxWidth+optionIndent+1; // could "running description indent"
            offset = startCol;
        }
        os << std::endl;
    }
    if(helpTail.length()!=0) {
        os << helpTail << std::endl;
    }
}


std::string 
OptionsIO::getHelpFormattedSynonyms(const OptionsCont &options, const std::string &optionName) {
    OptionsCont::compareByLength c;
    std::vector<std::string> synonyms = options.getSynonyms(optionName);
    std::sort(synonyms.begin(), synonyms.end(), c);
    std::ostringstream oss;
    for(std::vector<std::string>::const_iterator j=synonyms.begin(); j!=synonyms.end(); ++j) {
        // consider the - / --
        if((*j).length()==1) {
            oss << '-';
        } else {
            oss << "--";
        }
        oss << (*j);
        if(j!=synonyms.end()-1) {
            oss << ", ";
        }
    }
    /*
    std::string semType = option->getSemanticType();
    if(semType!="") {
        oss << " " << semType;
    }
    */
    return oss.str();
}





// *************************************************************************

