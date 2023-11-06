#include "ConfigParser.hpp"

/*------------------------------PUBLIC METHODS ------------------------------*/

void    ConfigParser::print(void) const
{
    for (size_t i = 0; i < _serverConfigs.size(); ++i) 
    {
        std::cout << serverConfigs()[i] << "\n";
    }
}

std::vector<t_ipPort> ConfigParser::getIpPort(void)
{
    std::vector<t_ipPort>    vecIpPort;

    for (size_t i = 0; i < _serverConfigs.size(); i++)
    {
        for (size_t j = 0; j < _serverConfigs[i]._ports.size(); j++)
        {
            t_ipPort tmp;
            tmp.ip = _serverConfigs[i]._decimalIPaddress;
            tmp.port = _serverConfigs[i]._ports[j];
            vecIpPort.push_back(tmp);
        }
    }
    return (vecIpPort);
}

std::string ConfigParser::_extractLocationKey(std::string &line)
{
    size_t  startPos;
    size_t  endPos;
    std::string substr;
    std::string key;

    startPos = line.find_first_not_of(" \t", 0); // Skip leading spaces and tabs
    startPos = line.find_first_of(" ", startPos); // Skip the first word
    endPos = line.size();
    
    if (startPos != std::string::npos && endPos != std::string::npos) 
        substr = line.substr(startPos, endPos - startPos);
    // do I need an else here?

    std::istringstream lineStream(substr);
    lineStream >> key;

    return (key);
}

std::string ConfigParser::_extractLocationBlock(std::string &line,
    std::ifstream &configFile)
{
    std::stringstream locationBlock;
    size_t startPos = 0;
    size_t  endPos = 0;
    std::string substr;

    startPos = line.find_first_of("{", startPos) + 1;
    if (startPos != std::string::npos && endPos != std::string::npos) 
        substr = line.substr(startPos, endPos - startPos);
    else
    {
        // std::cout << "Missing closing bracket\n";
    }
    std::getline(configFile, line, '}');
    return (substr + line);
}

/*
Segments different blocks of server{ ... } and passes it for
furhter parsing into the parseConfigServer()
It also skips empty lines and comments (lines starting with "#")
*/
void    ConfigParser::_parseConfigFile(std::ifstream &configFile)
{
    std::map<std::string, ServerLocation> tmpVecLB;
    std::string line;
    bool insideServerBlock = false;
    std::stringstream serverBlock;

    while (std::getline(configFile, line)) 
    {
        if (line.find("#") != std::string::npos || line.empty())
            continue;
        else if (line.find("server {") != std::string::npos) 
        {
            // technically I would need to do the same here as in the location
            // block, as there could be important information on the same line
            insideServerBlock = true;
            serverBlock.str("");
            serverBlock.clear();
        }
        else if (line.find("location") != std::string::npos)
        {
            std::string key = _extractLocationKey(line);
            std::stringstream locationBlock(_extractLocationBlock(line, configFile));
            tmpVecLB.insert(std::make_pair(key, ServerLocation(locationBlock)));
            // how do I check that there is no "}" missing?
        }
        else if (line.find("}") != std::string::npos) 
        {
            insideServerBlock = false;
            ServerConfig tmp(serverBlock, _serverConfigs.size(), tmpVecLB);
            _serverConfigs.push_back(tmp);
            tmpVecLB.clear();
        }
        else if (insideServerBlock)
            serverBlock << line << "\n"; 
        else
            throw (std::runtime_error("Error, unknown keyword " + line));
    
    }

    if (insideServerBlock == true)
        std::runtime_error("Error, missing }");
}

/*----------------------------- PRIVATE METHODS -----------------------------*/

/*--------------------------- GETTERS AND SETTERS ---------------------------*/


std::vector<ServerConfig> ConfigParser::serverConfigs(void) const
{
    return (_serverConfigs);
}

/*------------------- CONSTRUCTOR, ASSIGNEMENT, DESTRUCTOR ------------------*/

ConfigParser::ConfigParser(void)
{
}

ConfigParser::ConfigParser(std::string file)
{    
    if (file.find(".conf") == std::string::npos)
        throw (std::runtime_error("Error: file extension is not .conf"));

    const char *fileName = file.c_str();
    std::ifstream configFile(fileName);
    
    if (!configFile.is_open()) 
        throw (std::runtime_error("Error: could not open file"));
	
    _parseConfigFile(configFile);
	configFile.close();
}

ConfigParser::ConfigParser(const ConfigParser& copy)
    :   _serverConfigs(copy._serverConfigs)
{
}

ConfigParser& ConfigParser::operator=(const ConfigParser& other)
{
    if (this != &other)
    {
        _serverConfigs = other._serverConfigs;
    }
    return (*this);
}

ConfigParser::~ConfigParser(void)
{
}

/*-------------------------- OPERATOR OVERLOADING ---------------------------*/