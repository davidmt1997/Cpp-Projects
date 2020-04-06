#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <fcntl.h>
#include <wordexp.h>
#include <regex.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <wordexp.h>
#include <fstream>
#include <csignal> 

using namespace std;

#define DELIMS " \r\t\n"
#define WRITE   1
#define READ  0
string prompt = ">>> ";

void my_path(string p, string arr[]);

vector<string> split_input(char *input, int del){
        vector<string> list;
        char *split;
        const char *delim;
        switch(del){
            case 1: delim = ";"; break;     //break multi line assignments down
            case 2: delim = DELIMS; break;  //break assignment into bite size piece
            case 3: delim = ":"; break;     //break paths down
            case 4: delim = "|"; break; //break multi pipes down
        }
        split = strtok(input, delim);
        while(split != NULL){
                list.push_back(split);
                split = strtok(NULL, delim);
        }
        return list;
}

vector<string> concat(vector<string> list_in, string cmd){
  vector<string> list_out;
  string thecat;
  while(!list_in.empty()){
    thecat = list_in.at(0)+"/"+cmd;
    list_out.push_back(thecat);
    list_in.erase(list_in.begin());
  }
  return list_out;
}

void mySystem(vector<string> argl){
  int status;
  pid_t pid = fork();
    if(pid == 0){
    vector<string> list, cmdL;
    char *env = getenv("PATH");
    char *env_p = new char[strlen(env)];
      env_p = strcpy(env_p,env);
    list = split_input(env_p,3);
    list = concat(list,argl.at(0));
    int i;
    char *args[argl.size()+1];
    args[argl.size()]='\0';
    while(!list.empty()){
      if(strstr((char*) argl.at(0).c_str(),"/") != NULL)
        args[0]=(char *) argl.at(0).c_str();
      else
        args[0]=(char *) list.at(0).c_str();

      i = 1;
      while(args[i] != NULL){
        args[i]=(char *) argl.at(i).c_str();
        i++;
      }
      if(execv(args[0], args) < 0){
        list.erase(list.begin());
        continue;
      }else exit(EXIT_SUCCESS);
  }
  printf("Youre wrong, no such command\n");
  exit(EXIT_FAILURE);
  }else if (pid < 0) perror("fork");
  else while (wait(&status) != -1);
}
void redirect(char *dir, char *file, int io){
  int status;
  pid_t pid = fork();
    if(pid == 0){
    int fd;
    vector<string> list, cmdL, argl;
    argl = split_input(dir, 2);
    if(io == WRITE){
      fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
      if(fd < 0) {
        perror("open");
        exit(1);
      }
      if(dup2(fd,1) < 0) perror("dup2 for writing");
    }else{
      fd = open(file, O_RDONLY);
      if(dup2(fd,0) < 0) perror("dup2 for reading");
    }
    char *env = getenv("PATH");
    char *env_p = new char[strlen(env)];
      env_p = strcpy(env_p,env);
    list = split_input(env_p,3);
    list = concat(list,argl.at(0));
    int i;
    char *args[argl.size()+1];
    args[argl.size()]='\0';
    while(!list.empty()){
      if(strstr((char*) argl.at(0).c_str(),"/") != NULL)
        args[0]=(char *) argl.at(0).c_str();
      else
        args[0]=(char *) list.at(0).c_str();
      i = 1;
      while(args[i] != NULL){
        args[i]=(char *) argl.at(i).c_str();
        i++;
      }
      if(execv(args[0], args) < 0){
        list.erase(list.begin());
        continue;
      }else exit(EXIT_SUCCESS);
  }
  printf("Youre wrong, no such command\n");
  exit(EXIT_FAILURE);
  }else if (pid < 0) perror("fork");
  else while (wait(&status) != -1);
}
string wordexp(string var, int flags = 0){
    wordexp_t p;
    if(!wordexp(var.c_str(), &p, flags)){
      if(p.we_wordc && p.we_wordv[0])
        var = p.we_wordv[0];
      wordfree(&p);
    }
    return var;
}
string change_dir(char *n_dir){
  if(chdir(n_dir) < 0) perror("chdir");
  if(strcmp(n_dir, "..")==0){
    string HOME = wordexp("~");
    char *my_path = getenv("PWD");
    //cout << my_path << endl;
    string path = string(my_path);
    if(path.find(HOME)==0)
      path.replace(0, HOME.size(), "~");
    return path;
  }
  
  string HOME = wordexp("~");
  char *my_path = getenv("PWD");
  string path = string(my_path);
  if(path.find(HOME)==0)
    path.replace(0, HOME.size(), "~");
  path += "/"+string(n_dir);
  
  return path;
}

void check_files(const char* pattern){
  int status;
  pid_t pid = fork();
  if(pid == 0){
    wordexp_t p;
    char **w;

    if(wordexp(pattern,&p,0) < 0) perror("wordexp");
    w = p.we_wordv;
    for (int i = 0; i < p.we_wordc; i++)
      printf("%s\n", w[i]);
    wordfree(&p);
    exit(EXIT_SUCCESS);
  }else if (pid < 0) perror("fork");
  else while (wait(&status) != -1);
}

void signal_handler( int signal_num ) { 
   cout << "Interrupt signal is (" << signal_num << ").\n";
} 

int main(){
  vector<string> aList, cList, bList, command;
  char *input, *split, *file, *dir, *word;
  string my_path;
  bool first = false, die = false;
  int i, j, k, io;

  const char* env_p = getenv("PATH");
  string epath = env_p;

  printf("\nWelcome. The \"q\" command will exit the shell\n");
  using_history();
  if(read_history("/home/dmiranda/.hist")!=0)cout << "read history error" << endl;
  while(!die){
    signal(SIGINT, signal_handler);
    input = readline( prompt.c_str());
    add_history(input);
    if(write_history("/home/dmiranda/.hist")!=0)cout << "write history error" << endl;
      
    aList = split_input(input, 1);

    while(!aList.empty()){
      char *cmd = (char *) aList.at(0).c_str();
      if(strstr(cmd,"*") != NULL){
        command = split_input(input, 2);
        string res = (char *)command.at(1).c_str();
        check_files((const char*) res.c_str());
        break;
      }
      if(strstr(cmd,"$") != NULL){
        cout << cmd << endl;
        split = strtok(cmd,"$");
        word = strtok(NULL,"\r");
        if(getenv(word)==NULL){
          cout << "Variable entered not found in the current environment" << endl;
          break;
        }
        cList = split_input(split,2);
        cList.push_back(getenv(word));
        mySystem(cList);
        break;
      }
      if(strstr(cmd,"=") != NULL){
        word = strtok(cmd,"=");
        split = strtok(NULL,"\r");
        if(strcmp(word,"PS1") == 0){
          string newstr = split;
          prompt=newstr;
          break;
          }else{
          if(setenv(word,split,1) < 0) perror("setenv");
          break;
        }
      }
      if(strstr(cmd,">") != NULL){
        dir = strtok(cmd,">");
        file = strtok(NULL," ");
        redirect(dir,file,WRITE);
        break;
      }
      if(strstr(cmd,"<") != NULL){
        dir = strtok(cmd,"<");
        file = strtok(NULL," ");
        redirect(dir,file,READ);
        break;
      }
      else{
        cList = split_input(cmd, 2);
        aList.erase(aList.begin());
        char *args[cList.size()+1];
        args[cList.size()] = '\0';
        j = 0;
        while(args[j] != '\0'){
          args[j]=(char *) cList.at(j).c_str()+NULL;
          j++;
        }            
        if((strlen(args[0]) == 1) && (strcmp(args[0], "q") == 0)){
                die = true;
                break;
        }else if(strcmp(args[0], "cd") == 0) {
          if(sizeof(args) > 1) my_path = change_dir(args[1]);
          cout << "my path " << my_path << endl;
          continue;
        }
        mySystem(cList);
      }

    }
  }
  return 0;
}

