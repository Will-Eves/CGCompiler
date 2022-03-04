#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#define WINDOWS_SUPPORT
#define MAC_SUPPORT
#define LINUX_SUPPORT
#include "platform.h"

void Compile(std::string ifilename, std::string ofilename){
    std::string code;
    unsigned int codeLength;

    std::stringstream codeStream;
    std::string line;
    std::ifstream ifile(ifilename.c_str());
    while(std::getline(ifile, line)){
        codeStream << line << std::endl;
    }
    code = codeStream.str();
    codeLength = code.length();

    std::stringstream outCode;

    outCode << "#include <iostream>\n";
    outCode << "#define r return\n";
    outCode << "void p(int input){ std::cout << input; }\n";
    outCode << "void pc(int input){ char cinp = input; std::cout << cinp; }\n";
    outCode << "void ps(std::string input){ std::cout << input; }\n";
    outCode << "void pb(){ std::cout << std::endl; }\n";
    outCode << "void i(int& input){ std::cin >> input; }\n";

    // Splice to c++
    unsigned int index = 0;
    // 0 : none, 1 : function, 2 : number, 3 : array, 4 : loop
    unsigned int mode = 0;
    unsigned int depth = 0;
    unsigned int functionDepth = 0;
    bool varsDone = true;
    bool inComment = false;
    bool inString = false;
    int cppModeOut = 0;
    bool cppMode = false;
    unsigned int loopCount = 0;
    while(index < codeLength){

        if(code[index] == '/' && index+1<codeLength && code[index+1] == '*') inComment = true;
        if(code[index-2] == '*' && code[index-1] == '/') inComment = false;
        if(inComment){
            index++;
            continue;
        }

        if(code[index] + "" == "'" || code[index] == '"') inString = !inString;
        if(inString){
            outCode << code[index];
            index++;
            continue;
        }

        if(code[index] == '{'){
            outCode << '(';
            if(mode == 1 && !varsDone && index+1<codeLength && code[index+1] != '}'){
                outCode << "int ";
            }
        }
        else if(code[index] == '}'){
            varsDone = true;
            outCode << ')';
        }
        else if(code[index] == '<'){ outCode << '{'; depth++; }
        else if(code[index] == '>'){
            if(depth-1 == functionDepth){
                outCode << "return 0;";
                mode = 0;
            }
            outCode << '}';
            depth--;
        }
        else if(code[index] == 'f' && index+1<codeLength && code[index+1] == ' '){ varsDone = false; mode = 1; outCode << "int"; functionDepth = depth; }
        else if(code[index] == 'n' && index+1<codeLength && code[index+1] == ' '){ mode = 2; outCode << "int"; }
        else if(code[index] == ';' && mode == 2){ mode = 0; outCode << " = 0;"; }
        else if(code[index] == ','){
            if(mode == 1 && !varsDone && index+1<codeLength && code[index+1] != '}'){
                outCode << ", int ";
            }else{
                outCode << ',';
            }
        }
        else if(code[index] == '+' && index+1<codeLength && code[index+1] == ' ') outCode << "+=";
        else if(code[index] == '-' && index+1<codeLength && code[index+1] == ' ') outCode << "-=";
        else if(code[index] == '*' && index+1<codeLength && code[index+1] == ' ') outCode << "*=";
        else if(code[index] == '/' && index+1<codeLength && code[index+1] == ' ') outCode << "/=";
        else if(code[index] == 'i' && index+1<codeLength && code[index+1] == 's'){ index++; outCode << "=="; }
        else if(code[index] == '['){
            std::string start;
            std::string end;

            bool a = false;

            index++;
            while(index<codeLength&&code[index] != ':'){
                if(code[index] == '.') a = !a;
                else if(a){
                    end += code[index];
                }else{
                    start += code[index];
                }
                index++;
            }
            outCode << "for(int LOOP_INDEX" + std::to_string(loopCount) + " = " + start + "; LOOP_INDEX" + std::to_string(loopCount) + " < " + end + "; LOOP_INDEX" + std::to_string(loopCount) + "++){";
            loopCount++;
        }
        else if(code[index] == ']') outCode << "}";
        else outCode << code[index];
        index++;
    }

    std::ofstream tfile;
    tfile.open("temp.cpp"); 
    tfile << outCode.str() << std::endl;
    tfile.close();

    std::string cmd = "g++ temp.cpp -o " + ofilename;
    system(cmd.c_str());
#ifdef WINDOWS
    cmd = "start " + ofilename;
#elif LINUX
    cmd = "./" + ofilename;
#elif MAC
    cmd = "./" + ofilename;
#endif
    system(cmd.c_str());
}

int main( int argc, char* argv[] ){
    if(argc == 3){
        std::string infile = argv[1];
        std::string outfile = argv[2];
        Compile(infile, outfile);
        return 0;
    }

    std::cout << "Welcome to .cg compiler!" << std::endl;
    std::cout << "Type 'run filename' to run a .exe file." << std::endl;
    std::cout << "Type 'compile infilename outfilename' to compile a .cg program to a .exe file." << std::endl;
    std::cout << "Type 'exit' to leave the program." << std::endl;

    bool running = true;
    while(running){
        std::string cmd;
        std::cin >> cmd;

        if(cmd == "exit"){
            running = false;
        }else if(cmd == "run"){
            std::string filename;
            std::cin >> filename;
            std::string out = "start " + filename;
            system(out.c_str());
        }else if(cmd == "compile"){
            std::string ifilename;
            std::cin >> ifilename;
            std::string ofilename;
            std::cin >> ofilename;
            
            Compile(ifilename, ofilename);
        }
    }

    return 0;
}