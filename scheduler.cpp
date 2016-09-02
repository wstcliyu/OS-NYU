#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <queue>
#include <stack>
#include <algorithm>
#include <getopt.h>

#define C_RE 1
#define RE_RU 2
#define RU_B 3
#define B_RE 4
#define RU_RE 5
#define FIN 6
using namespace std;

int verbosemode=0;
int curr_time=0;
int total_time=0;
double total_cputime=0.0;
double total_turnaround_time=0.0;
double total_io_time=0.0;
double cpu_util_time=0.0;


string schedulingAlgo="";

int * random_num=NULL;;
int ofs = 0;
int num=0;
void initializeRandom(char * Fl){
  ifstream fin;
  fin.open(Fl);
  string str;
  if(fin.is_open()){
    getline(fin,str);
    num=atoi(str.c_str());
    random_num=new int[num];
    int count=0;
    while(getline(fin,str)){
      random_num[count]=atoi(str.c_str());
      count++;
    }
  }
  
     fin.close();
}
int getRandomNumber(int burst){  
int returnVal = 1 + random_num[ofs] % burst;
ofs++;
if(ofs == num)
ofs = 0;return returnVal;
}

class interval{
public:
    int start;
    int end;
    interval();
    interval(int s,int e)
    {
      start=s;end=e;
    }
    
    
};
vector <interval> interval_vector;

void insert_interval(interval temp)
{  int postion=interval_vector.size();
  for(int i=0;i<interval_vector.size();i++)
  {
    if(temp.start<interval_vector[i].start)
    {
      postion=i;break;
    }
  }
  interval_vector.insert(interval_vector.begin()+postion,temp);

}
bool compare_interval(interval temp1, interval temp2)
{ 
  return (temp1.start < temp2.start)? true: false;
}


class process{
public:
  int pid=0 ;//Process ID
  int at=0;//Arrivale Time
  int tc=0;//Total CPU time
  int cb=0;//CPUT BURST TIME
  int io=0;//IO BURST TIME
  int pt=0;//Previous Time
  int finishing_time=0,turnaround_time=0,it_time=0,cw_time=0;
  int remaining_exec_time=0,remain_cpuburst=0;
  string state;
  int static_prio=0;
  int dynamic_prio=0;
  int rand_cpuburst=0;
  process()
  {}
  process(int id,int a,int b,int c,int d)
  {
    pid=id;
    at=a;
    tc=b;
    cb=c;
    io=d;
    it_time=0;
    cw_time=0;
    remain_cpuburst=tc;  
  }
  void printprocc()
  {
      cout<<"PID->"<<pid<<" "
          <<"AT->"<<at<<" "
          <<"TC->"<<tc<<" "
          <<"CB->"<<cb<<" "
          <<"IO->"<<io<<endl;



  }
};
vector <process> procc;
class scheduler{
public:
  virtual void add_process(process inp){};
  virtual process get_next_process(){};
  vector <process> RUN_QUEUE;
  int quantum=0;
  int priority=0;
  virtual string  get_scheduler_name(){};
 
  virtual bool isEmpty() = 0;
  virtual ~scheduler(){};
  virtual void adding_expired_process(process inp)=0; //need to add virtual process as it will be used in Priority Algo

};
scheduler *sche;

class FCFS :public scheduler {
public:
  string get_scheduler_name()
  {
    return "FCFS";
  }
  process get_next_process()
  { process p;
    if (!RUN_QUEUE.empty())
    {
      p=RUN_QUEUE.front();
       RUN_QUEUE.erase(RUN_QUEUE.begin());
    
    }
      return p;
  }
  void adding_expired_process(process p)
  {

  }
  void add_process(process inp)
  {
    RUN_QUEUE.push_back(inp);
  }
  bool isEmpty()
  {
    return RUN_QUEUE.empty();
  }

  virtual ~FCFS(){};
};

class LCFS:public scheduler
{
public:
   void adding_expired_process(process p)
  {
    
  }
  bool isEmpty()
  {
    return RUN_QUEUE.empty();
  }
  void add_process(process inp)
  {
    RUN_QUEUE.push_back(inp);
  }
  string get_scheduler_name()
  {
    return "LCFS";
  }
  process get_next_process()
  { process p;
    if (!RUN_QUEUE.empty())
    {
      p=RUN_QUEUE.back();
       RUN_QUEUE.pop_back();
    
    }
      return p;
  }
 
};

class SJF :public scheduler
{
public:
   void adding_expired_process(process p)
  {
    
  }
  string get_scheduler_name()
  {
    return "SJF";
  }
  process get_next_process()
  {
      process p;
      if(RUN_QUEUE.size()>0)
      {
        p=RUN_QUEUE[0];
        RUN_QUEUE.erase(RUN_QUEUE.begin());
      }
      return p;
  }
  void add_process(process inp)
  {
    int readyQ_size=RUN_QUEUE.size();
    int index=readyQ_size;
    for(int i=0;i<readyQ_size;i++)
    {
      if(inp.remain_cpuburst<RUN_QUEUE[i].remain_cpuburst)
      {
        index=i;break;
      }
    }
    RUN_QUEUE.insert(RUN_QUEUE.begin()+index,inp);
  }
  bool isEmpty()
  {
    return RUN_QUEUE.empty();
  }
  virtual ~SJF(){};
};

class RR :public scheduler 
{
public:
   void adding_expired_process(process p)
  {
    
  }
  bool isEmpty()
  {
    return RUN_QUEUE.empty();
  }
  string get_scheduler_name()
  {
    return "RR";
  }
  RR();
  RR(int quan)
  {
    this->quantum=quan;
  }
  int get_quantum()
  {
    return this->quantum;
  }
  void add_process(process inp){RUN_QUEUE.push_back(inp);}
  process get_next_process(){
    process p;
    if (!RUN_QUEUE.empty())
    {
      p=RUN_QUEUE.front();
       RUN_QUEUE.erase(RUN_QUEUE.begin());
    
    }
      return p;
  }
  virtual ~RR(){};

};
class PRIO :public scheduler
{
public:

  vector <process> a_0,a_1,a_2,a_3;
  vector <process> exp_0,exp_1,exp_2,exp_3;
  void adding_expired_process(process inp)
  { 
   if(inp.dynamic_prio==0)
      exp_0.push_back(inp);
    else if (inp.dynamic_prio==1)
      exp_1.push_back(inp);
    else if (inp.dynamic_prio==2)
      exp_2.push_back(inp);
    else if (inp.dynamic_prio==3)
      exp_3.push_back(inp);
  }
  bool isEmpty()
  {
    return RUN_QUEUE.empty();
  }
  string get_scheduler_name()
  {
    return "PRIO";
  }

  PRIO();
  PRIO(int quan)
  {
    this->quantum=quan;
  }
  int get_quantum()
  {
    return this->quantum;
  }
  void add_process(process inp)
  {
    if(inp.dynamic_prio==0)
      a_0.push_back(inp);
    else if (inp.dynamic_prio==1)
      a_1.push_back(inp);
    else if (inp.dynamic_prio==2)
      a_2.push_back(inp);
    else if (inp.dynamic_prio==3)
      a_3.push_back(inp);
  }
  process get_next_process()

  {
    process p;
    if(a_0.empty())
    {
      if(a_1.empty())
      {
        if(a_2.empty())
        {
          if(a_3.empty())
          {
            a_0.swap(exp_0);a_1.swap(exp_1);a_2.swap(exp_2);a_3.swap(exp_3);
          }
        }
      }
    }
//Get the highest priority process
    if(a_3.size()!=0)
    {
      p=a_3.front();
      a_3.erase(a_3.begin());
    }
    else if(a_2.size()!=0)
    {
      p=a_2.front();
      a_2.erase(a_2.begin());
    }
    else if(a_1.size()!=0)
    {
      p=a_1.front();
      a_1.erase(a_1.begin());
    }
    else if(a_0.size()!=0)
    {
      p=a_0.front();
      a_0.erase(a_0.begin());
    }
    return p; 

  }
  virtual ~PRIO(){};

};

class event {
public:
  int processID,time_created,time_start,state_trans,dynamic_prio,static_prio;
  event(){};
  event(int ts,int pid,int tc,int tn)
  { time_start=ts;
    processID=pid;
    time_created=tc;
    state_trans=tn;
  }

  void print_events()
  {
    cout<<"Event Details->"<<endl
        <<"Start Time:"<<time_start<<endl
        <<"Process ID:"<<processID<<endl
        <<"Time Created"<<time_created<<endl
        <<"Trans State:"<<state_trans<<endl;
  }
};

class simulator 
{

          public:
            vector<event> eventQ;
            int new_process_time=0; //When can next process run (at what time)
            int quantum=0;
                    event getEvent()
                    {
                      event temp;
                      
                      temp.processID=0;
                      temp.time_created=0;
                      temp.time_start=0;
                      temp.state_trans=0;

                      if(!eventQ.empty())
                      {
                        temp=eventQ.front();
                        eventQ.erase(eventQ.begin());
                      }

                      return temp;
                    }

                    void putEvent (event temp)
                    {
                      int size=eventQ.size();
                      for (int i=0;i <eventQ.size();i++)
                      {
                        if(temp.time_start<eventQ[i].time_start)
                        {
                          size=i;
                          break;
                        }
                      }
                      eventQ.insert(eventQ.begin()+size,temp);
                    }

                    void addtoEventQ()
                    { ///CODE THE FUNCTION TO ADD ALL NEW PROCESSES TO EVENT QUEUE
                      for(int i=0;i<procc.size();i++)
                      { 
                        event eve(procc[i].at,procc[i].pid,procc[i].at,C_RE); 
                        this->putEvent(eve);
                        procc[i].static_prio=getRandomNumber(4);
                        procc[i].dynamic_prio=procc[i].static_prio-1;

                      }
                    }

                    // 


                    void main_execution()
                    {
                     // cout<<"smain_exec";
                      curr_time=procc[0].at;
                      event temp_event(0,0,0,0);

                      while(!eventQ.empty())
                      {
                        temp_event=this->getEvent();
                        int processID=temp_event.processID;
                        if(curr_time<temp_event.time_start)
                          curr_time=temp_event.time_start;
                        if(temp_event.state_trans== C_RE)
                        {
                          //this->created_ready(temp_event.processID,curr_time);
                          
                          procc[processID].pt=curr_time;
                        sche->add_process(procc[processID]);
                        event temp(procc[processID].at,processID,procc[processID].at,RE_RU);
                        this->putEvent(temp);                        
                        }
                        else if(temp_event.state_trans== RE_RU)
                        {
                          if(temp_event.time_start<new_process_time)
                          {
                            temp_event.time_start=new_process_time;
                            this->putEvent(temp_event);
                            continue;
                          }
                          //this->ready_running(temp_event,temp_event.processID);
                          event curr_event=temp_event;
                          int procID=temp_event.processID;
                        process p=sche->get_next_process();
                                procID=p.pid;
                               curr_event.time_created=procc[procID].pt;
                              int total_cpu_time=((procc[procID].cw_time+curr_time)- curr_event.time_created);
                              procc[procID].cw_time=total_cpu_time;
                              if(sche->get_scheduler_name()=="RR"||sche->get_scheduler_name()=="PRIO")
                                {
                                                if(procc[procID].rand_cpuburst==0)
                                                {
                                                  procc[procID].rand_cpuburst=getRandomNumber(procc[procID].cb);
                                                }

                                                if(quantum> procc[procID].rand_cpuburst)
                                                {
                                                    if(procc[procID].rand_cpuburst<procc[procID].remain_cpuburst)
                                                    {
                                                      event temp(curr_time+procc[procID].rand_cpuburst,procID,curr_time,RU_B);
                                                      new_process_time=curr_time+procc[procID].rand_cpuburst;
                                                      procc[procID].remain_cpuburst -= procc[procID].rand_cpuburst;
                                                      procc[procID].rand_cpuburst=0;
                                                      this->putEvent(temp);
                                                    }
                                                    else
                                                    {
                                                      event temp(curr_time+procc[procID].remain_cpuburst,procID,curr_time,FIN);
                                                      new_process_time=curr_time+procc[procID].remain_cpuburst;
                                                      procc[procID].remain_cpuburst=0;
                                                      procc[procID].rand_cpuburst=0;
                                                      this->putEvent(temp);
                                                    }
                                                }
                                                else if(quantum<=procc[procID].rand_cpuburst)
                                                {
                                                  if(quantum<procc[procID].remain_cpuburst)
                                                          {
                                                                    if(procc[procID].rand_cpuburst!=quantum)
                                                                    {
                                                                      event temp(curr_time+quantum,procID,curr_time,RU_RE);
                                                                    new_process_time=curr_time+quantum;
                                                                    procc[procID].remain_cpuburst= procc[procID].remain_cpuburst-quantum;
                                                                    procc[procID].rand_cpuburst=procc[procID].rand_cpuburst-quantum;
                                                                    this->putEvent(temp);
                                                                    }
                                                                    else
                                                                    {
                                                                    event temp(curr_time+procc[procID].rand_cpuburst,procID,curr_time,RU_B);
                                                                    new_process_time=curr_time+procc[procID].rand_cpuburst;
                                                                    procc[procID].remain_cpuburst= procc[procID].remain_cpuburst-procc[procID].rand_cpuburst;
                                                                    procc[procID].rand_cpuburst=0;
                                                                    this->putEvent(temp);

                                                                    }
                                                            }
                                                  else
                                                  {
                                                    event temp(curr_time+procc[procID].remain_cpuburst,procID,curr_time,FIN);
                                                    new_process_time=curr_time+procc[procID].remain_cpuburst;
                                                    procc[procID].remain_cpuburst=0;
                                                    procc[procID].rand_cpuburst=0;
                                                    this->putEvent(temp);
                                                  }
                                                }
                                              
                              }
                              else
                              {
                                        //cout << endl << "YOLO 1:" << procc[procID].cb << endl;
                                        int random_val=getRandomNumber(procc[procID].cb);
                                        procc[procID].rand_cpuburst=random_val;
                                        if(procc[procID].remain_cpuburst<=random_val)
                                        {
                                          int rem_cpu=procc[procID].remain_cpuburst;
                                          procc[procID].remain_cpuburst=0;
                                          event y(curr_time+rem_cpu,procID,curr_time,FIN);
                                          this->putEvent(y);
                                          new_process_time=curr_time+rem_cpu;
                                        }
                                        else
                                        {
                                          procc[procID].remain_cpuburst=procc[procID].remain_cpuburst- random_val;
                                          event z(curr_time+random_val,procID,curr_time,RU_B);
                                          this->putEvent(z);
                                          new_process_time=curr_time+random_val;
                                        }
                             }    
                        }
                        else if(temp_event.state_trans== RU_B)
                        {
                          //this->running_blocked(temp_event.processID);
                          
                        int random = getRandomNumber(procc[processID].io);
                      event temp(curr_time+random,processID,curr_time,B_RE);
                      procc[processID].it_time+=random;
                      this->putEvent(temp);
                      
                      interval tempzz(curr_time,curr_time+random);
                      insert_interval(tempzz);
                        }
                        else if(temp_event.state_trans== B_RE)
                        {   
                          //this->blocked_ready(temp_event.processID);
                        procc[processID].dynamic_prio=procc[processID].static_prio-1;
                      procc[processID].pt=curr_time;
                      sche->add_process(procc[processID]);
                      event temp(curr_time,processID,curr_time,RE_RU);
                      this->putEvent(temp);
                        }
                        else if(temp_event.state_trans==RU_RE)
                        {
                         // this->running_ready(temp_event.processID);
                        procc[processID].pt=curr_time;
                      event e(curr_time,processID,curr_time,RE_RU);
                      if(sche->get_scheduler_name()=="PRIO")
                      {
                          procc[processID].dynamic_prio=procc[processID].dynamic_prio-1;
                          if(procc[processID].dynamic_prio==-1)
                          {
                            procc[processID].dynamic_prio=procc[processID].static_prio-1;
                            sche->adding_expired_process(procc[processID]);
                            this->putEvent(e);
                          }
                          else
                          {
                            this->putEvent(e);
                            sche->add_process(procc[processID]);
                          }
                          }
                      else
                      {
                        this->putEvent(e);
                        sche->add_process(procc[processID]);
                      }
                        }
                        else if(temp_event.state_trans==FIN)
                        {
                          //this->finished(temp_event.processID);
                         procc[processID].finishing_time=curr_time;
                      procc[processID].turnaround_time=procc[processID].finishing_time-procc[processID].at;

                        }
                      }

              }

              void printingResults()
              {
                //merging IO utilization to get final IO_UTIL
                //merge code reference from google
               // cout<<"Entering Printing";
                int x=0;
                stack<interval> s;
                
                 sort(interval_vector.begin(),interval_vector.end(),compare_interval);
                 if(interval_vector.size()>0)
                 s.push(interval_vector[0]);

                for(int i=1;i<interval_vector.size();i++)
                {
                  interval temp=s.top();
                  if(temp.end<interval_vector[i].start)
                    s.push(interval_vector[i]);
                  else if (temp.end<interval_vector[i].end)
                  {
                    temp.end=interval_vector[i].end;
                    s.pop();
                    s.push(temp);

                  }
                }
                while (!s.empty())
                {
                  interval t=s.top();
                  x=x+t.end-t.start;
                  s.pop();
                }
                int io_util;
                if(interval_vector.size()>0)
                  io_util=x;
                else
                  io_util=0;
                // cout<<"IO_UTIL is: "<<io_util<<endl;
                string schedulername=sche->get_scheduler_name();

                cout<<schedulername;
                if(sche->get_scheduler_name()=="RR"||sche->get_scheduler_name()=="PRIO")
                {
                  cout<<" "<<sche->quantum;
                }

                cout<<endl;

                for(int i=0;i<procc.size();i++)
                {
                  printf("%04d: %4d %4d %4d %4d %d |  %4d  %4d  %4d  %4d\n", procc[i].pid, procc[i].at, procc[i].tc, procc[i].cb, procc[i].io, procc[i].static_prio,  procc[i].finishing_time, procc[i].turnaround_time , procc[i].it_time, procc[i].cw_time);
                  if(procc[i].finishing_time>total_time)
                  {
                    total_time=procc[i].finishing_time;
                  }
                  total_cputime+=procc[i].cw_time;
                  total_turnaround_time+=procc[i].turnaround_time;
                  cpu_util_time+=procc[i].tc;
                }
                //cout<<endl<<"Debug stuff   ";
                //cout<<total_time<<"   "<<io_util<<endl;
                printf("SUM: %d %.2lf %.2lf %.2lf %.2lf %.3lf\n", total_time, (cpu_util_time*100)/total_time, (((float)io_util*100)/(float)total_time), total_turnaround_time/procc.size(), total_cputime/procc.size(), ((double)procc.size()*100)/total_time);


                }

} ;




void input_get (int argc, char **argv)
{
      int index=0;
      int c=0;

      opterr = 0;
      while ((c = getopt (argc, argv, "vs:")) != -1)
        switch (c)
          {
          case 'v':
            verbosemode = 1;
            break;
          case 's':
            schedulingAlgo = optarg;
            break;
            default:
            break;
          }

          for (index = optind; index < argc; index++)
          {
                          if(index==argc-2)
                          { ifstream infile(argv[index]);
                        int a, b,c,d;
                        int i=0;
                      while (infile >> a >> b >> c >> d)
                      {
                          procc.push_back(process(i,a,b,c,d));
                          i++;
                      }
                          //printf ("Non-option argument 1 %s\n", argv[index]);
                          }
                          if (index==argc-1)
                          {
                            initializeRandom(argv[index]);
                          // printf ("Non-option argument 2 %s\n", argv[index]);
                          }
          }
  simulator simulation ;
            for (int i=0;i<schedulingAlgo.length();i++)
            {
              if (schedulingAlgo.at(i)=='F')
              {
               // cout<<"FCFS"<<endl;
                sche= new FCFS();

              }
              else if (schedulingAlgo.at(i)=='L')
              {
                //cout << "LCFS"<<endl;
                sche=new LCFS();
              }
              else if (schedulingAlgo.at(i)=='S')
              {
               // cout <<"SJF"<<endl;
                sche=new SJF();
              }
              else if (schedulingAlgo.at(i)=='R')
              {
                //cout<<"RR"<<"\t"<<schedulingAlgo.substr(i+1)<<endl;
                simulation.quantum=atoi(schedulingAlgo.substr(i+1).c_str());
                sche=new RR(atoi(schedulingAlgo.substr(i+1).c_str()));
              }
              else if (schedulingAlgo.at(i)=='P')
              {
                //cout<<"PRIO"<<"\t"<<schedulingAlgo.substr(i+1)<<endl;
                simulation.quantum=atoi(schedulingAlgo.substr(i+1).c_str());
                sche=new PRIO(atoi(schedulingAlgo.substr(i+1).c_str()));
              }
            }
            //cout<<"Starting add to event"<<endl;
  simulation.addtoEventQ();
  //cout<<"Printing events la"<<endl;
  // for(int i=0;i<simulation.eventQ.size();i++)
  // {
  //   simulation.eventQ[i].print_events();
  // } 
 // cout<<"starting main exec"<<endl;
  simulation.main_execution();
 // cout<<"printing"<<endl;
  simulation.printingResults();


}



int main(int argc , char **argv)
{
    input_get(argc,argv);
   // cout << "verbosemode is-->" << verbosemode << "  schedulingAlgo is-->" << schedulingAlgo << "  Vector size"<<procc.size()<< endl;


  //   for(int i=0; i<procc.size(); ++i)
  // {  
  //   procc[i].printprocc();
  //  }

    //cout<<endl<<endl;

     // for(int i=0;i<=40000;i++)
     // {
     //   cout<<randomnumbers[i]<<endl;
     // }

  return 0;
}

