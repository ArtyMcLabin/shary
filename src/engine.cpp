/* BSD 3-Clause License
 *
 *	 Copyright (c) 2016, Arty McLabin
 *	 All rights reserved.
 *
 * 	 Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions are met:
 *
 *	 * Redistributions of source code must retain the above copyright notice, this
 *	   list of conditions and the following disclaimer.
 *
 *	 * Redistributions in binary form must reproduce the above copyright notice,
 *	   this list of conditions and the following disclaimer in the documentation
 *	   and/or other materials provided with the distribution.
 *
 *	 * Neither the name of the copyright holder nor the names of its
 *	   contributors may be used to endorse or promote products derived from
 *	   this software without specific prior written permission.
 *
 *	 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *	 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *	 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *	 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 *	 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *	 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *	 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *	 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 *	 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *	 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include"engine.h"
#include<boost/filesystem.hpp>


Command toCommand(string str)
{
    if(str == "get"){
        return Command::get;
    }else if(str == "give"){
        return Command::give;
    }else if(str == "add"){
        return Command::add;
    }else if(str == "remove"){
        return Command::remove;
    }else if(str == "help" || str == "-h" || str =="--help"){
        return Command::help;
    }else if(str == "list"){
        return Command::list;
    }else{
        return Command::bad_command;
    }
}

void add(string name, IP ip)
{
    IP old_ip;
    //if entry exists already
    if( !(old_ip=getIP(name)).empty() ){
        cerr << "\"" << name << "\" is enlisted under \"" << old_ip << "\" already." << endl;
        exit(-1);

//+     replace by new ip:
//
//        ifstream old_config(config_file_path);
//            if(!old_config){
//                cerr << "\"" << config_file_path << "\" is in use by another instance or program." << endl;
//                exit(-1);
//            }
//
//        ofstream new_config(config_file_path+".temp");
//            if(!new_config){
//                cerr << "\"" << config_file_path << ".temp\" is in use by another instance or program." << endl;
//                exit(-1);
//            }
//

    }else{
        //create new entry
            ofstream config(config_file_path, std::ios_base::app);
                if(!config){
                    cerr << "add(): \"" << config_file_path << "\":"<< strerror(errno) << endl;
                    exit(-1);
                }
            config << name << ":" << ip << endl;

    }

}

void remove(string name)
{
    //if config file doesn't exist, the contact list is empty.
    if(!boost::filesystem::exists(config_file_path)){
        cerr << "no such contact: \"" << name << "\"." << endl;
        exit(-1);
    }

    //shary.conf
    ifstream old_config(config_file_path);
        if(!old_config){
            cerr << "\"" << config_file_path << "\" is in use by another instance or program." << endl;
            exit(-1);
        }

    //shary.conf.temp
    ofstream new_config(config_file_path+".temp");
        if(!new_config){
            cerr << "\"" << config_file_path << ".temp\" is in use by another instance or program." << endl;
            exit(-1);
        }

    //search for the name pending deletion
    string line;
    while(getline(old_config, line))
    {
        if(line.substr(0,line.find_first_of(':'))==name){
            continue;
        }else{
            new_config << line << endl;
        }
    }

    //replace old by new
    boost::filesystem::rename(config_file_path+".temp",config_file_path);
}


bool give(IP ip, const vector<string> &filenames)
{
    string filenames_together;
    for(string filename : filenames){
        filenames_together += filename;
        filenames_together += " ";
    }
    system(("zip data.shary.zip "+filenames_together).c_str()); //@
    system(("nc -w 5 "+ip+" "+port+" < data.shary.zip").c_str());
    system("rm data.shary.zip");


    return true;
}

IP getIP(string name)
{
    if(!boost::filesystem::exists(config_file_path)){
        return ""; //empty ip (serves as null)
    }

    ifstream config(config_file_path);
        if(!config){
            cerr << "getIP(): " << strerror(errno) << endl;
            cerr << "\"" << config_file_path << "\" is in use by another instance or program." << endl;
            exit(-1);
        }

    string line;
    while(getline(config,line))
    {
        if(name == line.substr(0,line.find(":"))){
            return (IP)line.substr(line.find(":")+1); //+ check on add() -> name that contains ':'
        }
    }

    //didn't find fitting entry - return empty ip (serves as null)
    return "";
}

bool isValidCommand(Command command, int argc)
{
    switch(command)
    {
        case Command::get: return argc==2;
        case Command::give: return argc>3;
        case Command::add: return argc==4;
        case Command::remove: return argc==3;
        case Command::list: return argc==2;
        case Command::help: return argc==2;
        default: return false;
    }
}

void displaySyntax()
{
    cout << "correct syntax is: " << endl
         << "\t"    << "shary <command> <parameters...>" << endl
         << "\t"    << "shary --help" << endl;
}

void displayContacts()
{
    ifstream config(config_file_path);
        if(!config){
            cerr << "\"" << config_file_path << "\": " << strerror(errno) << endl;
            exit(-1);
        }
    string buffer;
    //print all the contacts
    while(!config.eof()){
        getline(config, buffer);
        cout << buffer << endl;
    }
}

void displayHelp()
{
    cout << endl;
    cout << "version " << VERSION << endl;
    cout << "port: " << port << endl;
    cout << endl;

    cout << "edit contacts:" << endl;
    cout << tab  << "(desktop):" << tab << "shary add laptop 192.168.0.2" << endl;
    cout << tab  << "(desktop):" << tab << "shary remove laptop" << endl;
    cout << "transfer files:" << endl;
    cout << tab  << "(laptop):" << tab << "shary get" << endl;
    cout << tab  << "(desktop):" << tab << "shary give laptop foo.txt *.jpg" << endl;
    cout << "helper commands:" << endl;
    cout << tab  << "(desktop):" << tab << "shary list" << endl;
    cout << tab  << " >outputs list of all contacts and their IP's" << endl;
}





