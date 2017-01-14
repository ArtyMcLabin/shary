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

/* Project Glossary:
 *	//+		add
 *	//-		remove
 *	//#		fix
 *	//!		very important (on short term)
 *	//?		non checked as working / needed yet
 *	//%		potential bug in future
 *	//@		implement error handling
 *	rome overload : all overloads lead to rome
 */

#include<iostream>
#include"engine.h"

using namespace std;
string config_file_path; //extern in engine.h



int main(int argc, char* argv[])
{
    //+ reimplement config dir with XDG_CONFIG_HOME
    config_file_path = CONFIG_PATH_ROOT+string("shary")+CONFIG_FILE_EXTENSION;
    Command command;
    IP ip;

    if(argc==1){
        displaySyntax();
        return -1;
    }

    command = toCommand(argv[1]);
    if(!isValidCommand(command,argc)){ //if not appropriate parameters count
        displaySyntax();
        return -1;
    }

    switch(command)
    {
        case Command::get:
            #ifdef _WIN32
                system(("nc -l "+port+" > data.shary.zip && unzip data.shary.zip && rm data.shary.zip").c_str());
            #else
                system(("nc -l -p "+port+" > data.shary.zip && unzip data.shary.zip && rm data.shary.zip").c_str());
            #endif
            break;
        case Command::give:
            {
                ip = getIP(argv[2]);
                vector<string> filenames;
                for(int i=3; i<argc; i++){
                    //supply rest of the arguments (which are filenames)
                    filenames.push_back(string(argv[i]));
                }
                give(ip, filenames);
                break;
            }
        case Command::add:
            add(argv[2],argv[3]); //@ ensure good order
            break;
        case Command::remove:
            remove((string)argv[2]);
            break;
        case Command::help:
            displayHelp();
            break;
        case Command::list:
            displayContacts();
            break;
        default: break; //never called because eliminated in the check block before switch (remove compiler warning)
    }
    return 0;
}
