#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <sstream>
#include <iomanip>


using namespace std;



//Rachit Mehrotra
//rm4149@nyu.edu



int No=0;
void printMemoryTable(int value,string error);
void pass1 (char* inp);
void pass2 (char* inp);
void symbolt();
void checkforrule4();
int symbol_exists(string sym);
bool is_digit(const std::string& s);
void __parseerror(int errcode, int linenum, int lineoffset) {
    static char* errstr[] = {
    "NUM_EXPECTED", // Number expect
    "SYM_EXPECTED", // Symbol Expected
    "ADDR_EXPECTED", // Addressing Expected
    "SYM_TOLONG", // Symbol Name is to long
    "TO_MANY_DEF_IN_MODULE", // > 16
    "TO_MANY_USE_IN_MODULE", // > 16
    "TO_MANY_INSTR" // total num_instr exceeds memory size (512)
    };
    printf("Parse Error line %d offset %d: %s\n", linenum, lineoffset, errstr[errcode]);
}
struct sym_table {
  string name;
  int val;
  int used_flag=0;
  int module;
};
sym_table arr[512];
string symbolerr[512];
  int arr_index=0;

int main ( int argc, char *argv[] )
{  
  if (argc == 2)
  {
               pass1(argv[1]);
  }
  else 
  {
    cout << "Invalid Number of arguments";
  }
  return 0;
}
bool is_digit(const std::string& s)
{
    string::const_iterator it = s.begin();
    while (it != s.end() && isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

void pass1 ( char* inp)
{
  ifstream ifs(inp);
  char c;
  ifs.get(c);
  int lineoffset=1;
  int line=1;
  int wordcount=0;
  int no_of_module=1;
  int list_type=1;
  int memoryoffset=0;
  int line3count=0;
  int  prevInstOffset=0;
  string prev_word="";
  int variablelength=0;


  while(!ifs.eof())
  {         if(list_type==3)
              {
                line3count=0;
              }

              string temp_words="";
               while ((c==' ' || c=='\n' || c== '\t') && !ifs.eof())
                {
                  if (c=='\n')
                  {
                    line++;
                    lineoffset=1;
                    ifs.get(c);
                  }
                  else
                  {
                    lineoffset++;
                    ifs.get(c);
                  }
                }
                while (c != ' ' && c != '\n' && c != '\t') 
                {
                    temp_words+=c;
                    lineoffset++;
                    ifs.get(c);
                    variablelength++;
                }
                wordcount++;
                    if(list_type==3)
                    {
                memoryoffset+=atoi(temp_words.c_str());
                  line3count++;
                  if(memoryoffset>512)
                  {
                    __parseerror(6, line, (lineoffset-variablelength));
                        exit(EXIT_FAILURE);
                  }
              }
        
          if(!temp_words.empty())
            {      
             //CHECK FOR RULE 5
         if(list_type==3)
          for(int i=0;i<arr_index;i++)
          {
            if(arr[i].module==no_of_module)
            {  //cout<<"Debug->Module no"<<no_of_module<<" arr module->"<<arr[i].module<<"arr val"<<arr[i].val<<" value of size->"<<atoi(temp_words.c_str())<<endl;
              if(arr[i].val>memoryoffset-1)
              {
                cout << "Warning: Module " << no_of_module << ": " << arr[i].name << " to big "<< arr[i].val << " (max=" << memoryoffset-1 << ") assume zero relative"<< endl;
                    arr[i].val=0;
              }
            }
          } 


                if(!is_digit(temp_words.c_str()))
                    { //cout<<"Error is "<<temp_words;
                        __parseerror(0, line, (lineoffset-variablelength));
                        exit(EXIT_FAILURE);
                    }
            
              if(list_type==1)        
            if(atoi(temp_words.c_str())>16)
            {
              __parseerror(4, line, (lineoffset-variablelength));
                        exit(EXIT_FAILURE);
            }
            if(list_type==2)
              if(atoi(temp_words.c_str())>16)
            {
              __parseerror(5, line, (lineoffset-variablelength));
                        exit(EXIT_FAILURE);
            }


            }
            if(temp_words==" ")
            {
              lineoffset++;
              continue;
            }      



            variablelength=0;

      int no_variable=atoi(temp_words.c_str());

              // cout <<endl<<" Word-list count: "<<temp_words<<" Line Offset: "<<lineoffset<<" Line: "<<line<<" Words: " <<wordcount<<" List type: "<<list_type<<" Modules: "<<no_of_module<<endl;
        
         int var_index=1;
         
     if (list_type==1||list_type==3)
       no_variable*=2;



        while (var_index<=no_variable)
        {  temp_words="";
              while ((c==' ' || c=='\n' || c== '\t') && !ifs.eof())
                {
                  if (c=='\n')
                  {
                    line++;
                     prevInstOffset=lineoffset;
                    lineoffset=1;
                    ifs.get(c);
                  }
                  else
                  {
                    lineoffset++;
                    ifs.get(c);
                  }
                }

          while (c != ' ' && c != '\n' && c != '\t') 
          {   variablelength++;
              temp_words+=c;
              lineoffset++;
              ifs.get(c);

              //CHeck error for symbol name too long > 16 !!! 
          }

          if(temp_words.empty())
          {
           //CHecking for parse error if Any of the lists are incomplete
        if(list_type==3)
        {
          if (no_variable!=var_index-1)
          {
            __parseerror(2, line-1, (prevInstOffset));
            exit(EXIT_FAILURE);
          }

          

        }

              if(list_type==1)
              {
                if(!is_digit(temp_words.c_str()))
                    {       
                        __parseerror(1, line-1, (prevInstOffset));
                        exit(EXIT_FAILURE);
                    }
              }
             

          }

           
                             if(list_type==2)
                                          { 
                                            if(is_digit(temp_words.c_str()))
                                            {
                                             __parseerror(1, line, (lineoffset-variablelength));
                                                    exit(EXIT_FAILURE); 
                                            }
                                          }


              
            
          if(wordcount%2==1)
          {prev_word=temp_words;
              if(prev_word.length()>16)
              {
                {
                                             __parseerror(3, line, (lineoffset-variablelength));
                                                    exit(EXIT_FAILURE); 
                                            }
              }
           } 
/*
            if(list_type==3)
            {   cout<<"Offsett is"<<lineoffset<<endl;
                cout<<"Word is"<<temp_words<<endl;
            
            }*/
                
            if(list_type==1)    
            if(wordcount%2==0)
            {

              if(symbol_exists(prev_word)!=-1)
                  {
                      symbolerr[symbol_exists(prev_word)]="Error: This variable is multiple times defined; first value used";
                  }
              else
              {
              arr[arr_index].name=prev_word;
              arr[arr_index].val=atoi(temp_words.c_str())+memoryoffset;
              arr[arr_index].module=no_of_module;
             // cout<<endl<<"Memory offsettt--->>"<<memoryoffset<<endl;
              arr_index++;
              }
            }
            wordcount++;

          if(list_type==1)
            {
              if(wordcount%2==1)
              {
                 if(!is_digit(temp_words.c_str()))
            {     //cout<<"Error is "<<temp_words;
                __parseerror(0, line, (lineoffset-variablelength));
                exit(EXIT_FAILURE);
            }

              }
              else
              {
                 if(is_digit(temp_words.c_str()))
            {     //cout<<"Error2 is "<<temp_words;
                __parseerror(1, line, (lineoffset-variablelength));
                exit(EXIT_FAILURE);
            }
              }
            }
            
            // Error check ADDR_Expected
            if (list_type==3)
            {
                if(line3count%2==0)
                {
                                    if(!is_digit(temp_words.c_str()))
                              {     //cout<<"Error is "<<temp_words;
                                  __parseerror(2, line, (lineoffset-variablelength));
                                  exit(EXIT_FAILURE);
                              }
                                    
                }

            }




            variablelength=0;
                var_index++;
              //cout <<endl<<"Prev word->"<<prev_word<<" Word: "<<temp_words<<" Line Offset: "<<lineoffset<<" Line: "<<line<<" Words: " <<wordcount<<" List type: "<<list_type<<" Modules: "<<no_of_module<<endl;
        }

                


            if(list_type<=2)
            {
              list_type++;
              

            }
            else
            {  wordcount=0;
              no_of_module++;  
              list_type=1;


           

            }

  }
   symbolt();
   pass2(inp);

}

void symbolt()
{  cout<<"Symbol Table"<<endl;
  for(int i=0;i<arr_index;i++)
  {
    cout<<arr[i].name<<"="<<arr[i].val<<" "<<symbolerr[i]<<endl;
  }
  cout<<"Memory Map"<<endl;
}


void pass2 ( char* inp)
{
  ifstream ifs(inp);
  char c;
  ifs.get(c);
  int lineoffset=0;
  int line=1;
  int wordcount=0;
  int no_of_module=1;
  int list_type=1;
  int memoryoffset=0;
  int memoryoffset2=0;
  int l3c=0;
  int l2c=0;
  string prev_word="";

  string uselist[16][2];
  int uselist_index=0;

  while(!ifs.eof())
  {        string temp_words="";
               while ((c==' ' || c=='\n' || c== '\t') && !ifs.eof())
                {
                  if (c=='\n')
                  {
                    line++;
                    lineoffset=0;
                    ifs.get(c);
                  }
                  else
                  {
                    lineoffset++;
                    ifs.get(c);
                  }
                }
                while (c != ' ' && c != '\n' && c != '\t') 
                {
                    temp_words+=c;
                    lineoffset++;
                    ifs.get(c);
                }
                wordcount++;
                    if(list_type==3)
               { memoryoffset+=atoi(temp_words.c_str());
                memoryoffset2=atoi(temp_words.c_str());
                  l3c++;
               }
      int no_variable=atoi(temp_words.c_str());

               
         int var_index=1;
     if (list_type==1||list_type==3)
       no_variable*=2;
     int uselistvar;
     if (list_type==2)
     {   
       uselistvar=atoi(temp_words.c_str());
      
       }
       

        while (var_index<=no_variable)
          {      
                          temp_words="";
                              while ((c==' ' || c=='\n' || c== '\t') && !ifs.eof())
                                {
                                  if (c=='\n')
                                  {
                                    
                                    lineoffset=0;
                                    ifs.get(c);
                                  }
                                  else
                                  {
                                    lineoffset++;
                                    ifs.get(c);
                                  }
                                }

                          while (c != ' ' && c != '\n' && c != '\t') 
                          {
                              temp_words+=c;
                              lineoffset++;
                              ifs.get(c);
                          }
                           
                            if(list_type==2)
                            {  
                              if(no_variable==0)
                                {l2c=0;
                                }
                                else
                              {
                              l2c++;
                              }

                              uselist[uselist_index][0]=temp_words;
                              uselist[uselist_index][1]="0";
                              uselist_index++;

                            //  cout<<"Debug-->L2c"<<l2c<<"of module"<<no_of_module<<endl;
                            }

                            if(list_type==3)
                            {  l3c++;
                              if(l3c%2==0)
                              prev_word=temp_words;
                              else
                              {
                                
                                if(prev_word=="R")
                                {
                                  if(atoi(temp_words.c_str())>9999)
                                    printMemoryTable(9999,"Error: Illegal opcode; treated as 9999");
                                  else if (atoi(temp_words.c_str())%1000>no_variable)
                                    printMemoryTable(((((atoi(temp_words.c_str())/ 1000) * 1000) + memoryoffset)-memoryoffset2), "Error: Relative address exceeds module size; zero used");
                                  else
                                  printMemoryTable((atoi(temp_words.c_str())+memoryoffset)-memoryoffset2," ");
                                  
                                }
                                else if(prev_word=="I")
                                {  
                                  if (atoi(temp_words.c_str())>9999)
                                    printMemoryTable(9999,"Error: Illegal immediate value; treated as 9999");
                                  else
                                  printMemoryTable(atoi(temp_words.c_str())," ");
                                  
                                }
                                else if(prev_word=="E")
                                {
                                  
                                  string ind=temp_words.substr(1,3);
                                  int index_for_E=atoi(ind.c_str());
                                    //cout<<"Debug->"<<index_for_E<<"->"<<uselist_index<<endl;

                                  if(index_for_E+1>uselist_index)
                                  {
                                    printMemoryTable(atoi(temp_words.c_str()),"Error: External address exceeds length of uselist; treated as immediate"); 
                                  }
                                  else
                                  {

                                  string symb=uselist[index_for_E][0];
                                  int check_if_symb_exist=0;
                                 //cout<<"Debug--->"<<l2c<<"Symbol->"<<symb<<"-->index_for_E-->"<<index_for_E<<"-->>USelist value-->"<<uselist[index_for_E][1]<<endl;
                                  for(int  i=0;i<arr_index;i++)
                                  {
                                    if (arr[i].name==symb)
                                    {  check_if_symb_exist=1;
                                      arr[i].used_flag=1;
                                        uselist[index_for_E][1]="1";
                                      printMemoryTable((atoi(temp_words.substr(0,1).c_str())*1000)+arr[i].val," ");
                                      arr[i].used_flag=1;
                                    }

                                  }
                                    if (check_if_symb_exist==0)
                                    {
                                      printMemoryTable((atoi(temp_words.substr(0,1).c_str())*1000),"Error: " + symb+ " is not defined; zero used");
                                      uselist[index_for_E][1]="1";
                                    }

                                }
        
         
                                }
                                else if(prev_word=="A") 
                                {  
                                    if (atoi(temp_words.c_str())>9999)
                                      printMemoryTable(9999,"Error: Illegal opcode; treated as 9999");
                                    else if (atoi(temp_words.c_str())%1000>512)
                                      printMemoryTable(((atoi(temp_words.c_str()) / 1000) * 1000),"Error: Absolute address exceeds machine size; zero used");
                                    else
                                    printMemoryTable(atoi(temp_words.c_str())," ");
                                
                                }
                              }


                            }


                            wordcount++;

                                var_index++;


         }
          
  
         
       //Check for RULE 7 
            if(list_type==3)
              {
/*
                for (int i=0;i<=uselist_index;i++)
                       {
                        cout<<" "<<uselist[i][0]<<" "<<uselist[i][1]<<" "<<no_of_module<<" "<<uselistvar;
                       } */
                          
                          for (int i=0;i<uselistvar;i++)
                      {
                        if(atoi(uselist[i][1].c_str())==0)
                          cout << "Warning: Module " << no_of_module << ": " <<uselist[i][0]<< " appeared in the uselist but was not actually used"<< endl;
                      }
              uselistvar=0;
              }
              l3c=0;
              l2c=0;
         
            
            if(list_type<=2)
            {
              list_type++;
              

            }
            else
            {  wordcount=0;
              no_of_module++;  
              list_type=1;
              uselist_index=0; 
              
            }
           

  }

  checkforrule4();

}

void checkforrule4()
{

for(int i=0;i<arr_index;i++)
{  //cout<<endl<<"Variable->"<<arr[i].name<<"<-Used Flag-->"<<arr[i].used_flag<<endl;
  if(arr[i].used_flag==0)
  { 
    cout << "Warning: Module " << arr[i].module << ": " << arr[i].name<< " was defined but never used" << endl;
}
}
}
void printMemoryTable(int value, string error) {
cout << setw(3) << setfill('0') << No << ": " << setw(4) << setfill('0')<< value;
if (error != " ")
{
    cout << " " << error;
}
cout << endl;
No++;
}
int symbol_exists(string sym) {
  for (int i = 0; i < arr_index; i++) 
  {
    if (sym == arr[i].name)
    return i;
  }
  return -1;
}
