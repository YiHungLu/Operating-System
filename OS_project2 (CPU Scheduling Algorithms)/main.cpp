#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <vector>
#include <time.h>

using namespace std;

/* run this program using the console pauser or add your own getch, system("pause") or input loop */
struct job{  //工作
  int n ;
	int ID;
	int cpu_burst;
	int arrival;
	int priority;
  int waiting;
  int turnaround;
};



class FCFS{
  int time ;
  int time_slice ;
  public:
  vector<int> workHis ;
  vector<job> scheduler(int t, vector<job> jobList){
    time_slice = t ;
    time = 0 ;
    workHis.clear();
    int n = 0 ;
    while(n < jobList.size()){
      if(time>=jobList[n].arrival){
        jobList[n].waiting = time - jobList[n].arrival ;
        while(jobList[n].cpu_burst!=0){
          workHis.push_back(jobList[n].ID) ;
          time++ ;
          jobList[n].cpu_burst-- ;
        }
        jobList[n].turnaround = time - jobList[n].arrival ;
        n++ ;
      }

      else{
        workHis.push_back(-1) ;
        time++ ;
      }

    }
    return jobList ;
  }
};

class RR{
  int time ;
  int time_slice ;
  public:
  vector<int> workHis ;
  vector<job> queue ;
  vector<job> scheduler(int t, vector<job> jobList){
    time_slice = t ;
    time = 0 ;
    workHis.clear();
    int n = 0 ;
    bool last = false ;
    for(int i=0 ; i<jobList.size() ; i++){
      jobList[i].waiting = 0 ;
      jobList[i].turnaround = 0 ;
    }

    while(1){
      while(time >= jobList[n].arrival && !last){         //先將到達的工作加入queue中
        jobList[n].n = n ;
        queue.push_back(jobList[n]);
        if(n+1<jobList.size())
          n++;
        else
          last = true ;
      }

      if(queue.size()!=0){
        if(time_slice!=0){
          if(queue[0].cpu_burst==0){
            jobList[queue[0].n].turnaround = time - jobList[queue[0].n].arrival ;
            queue.erase(queue.begin()) ;
            if(queue.size()>0)
              jobList[queue[0].n].waiting += (time - queue[0].arrival ) ;
            time_slice = t ;
          }
        }
        else{
          if(queue[0].cpu_burst==0){
            jobList[queue[0].n].turnaround = time - jobList[queue[0].n].arrival ;
            queue.erase(queue.begin()) ;
            if(queue.size()>0)
              jobList[queue[0].n].waiting += (time - queue[0].arrival ) ;
          }
          else{
            queue[0].arrival = time ;
            queue.push_back(queue[0]) ;
            queue.erase(queue.begin()) ;
            if(queue.size()>0)
              jobList[queue[0].n].waiting += (time - queue[0].arrival ) ;
          }
          time_slice = t ;
        }
        if(queue.size()>0){
          workHis.push_back(queue[0].ID) ;
          queue[0].cpu_burst-- ;
          time_slice-- ;
        }
        else if(!last&&queue.size()==0)
          workHis.push_back(-1);
      }

      else{
        workHis.push_back(-1);
      }

      if(n==jobList.size()-1 && queue.size()==0)
        break ;

      time++ ;
    }
    return jobList ;
  }
};

class SRTF{
  int time;
  int time_slice ;
  vector<job> queue ;

  void Insert(job newJob){
    if(queue.size() == 0)
      queue.push_back(newJob) ;
    else{
      bool find = false ;
      for(int i=0 ; i<queue.size() && !find ; i++){
        if(newJob.cpu_burst < queue[i].cpu_burst){
          queue.insert(queue.begin()+i,newJob) ;
          find = true ;
        }
        else if(newJob.cpu_burst == queue[i].cpu_burst){
          queue.insert(queue.begin()+i+1,newJob) ;
          find = true ;
        }
      }

      if(!find)
        queue.push_back(newJob) ;
    }
  }

  public:
  vector<int> workHis ;
  vector<job> scheduler(int t, vector<job> jobList){
    time = 0 ;
    workHis.clear();
    int n = 0 ;
    bool last = false ;

    for(int i=0 ; i<jobList.size() ; i++){  //init
      jobList[i].waiting = 0 ;
      jobList[i].turnaround = 0 ;
    }

    while(1){
      while(time >= jobList[n].arrival && !last){         //先將到達的工作加入queue中
        jobList[n].n = n ;
        Insert(jobList[n]) ;
        if(n+1<jobList.size())
          n++;
        else
          last = true ;
      }

      if(queue.size()!=0){
        for(int i=1 ; i<queue.size() ; i++)
          jobList[queue[i].n].waiting ++ ;

        workHis.push_back(queue[0].ID);
        queue[0].cpu_burst-- ;
        if(queue[0].cpu_burst == 0){
          jobList[queue[0].n].turnaround =  time - jobList[queue[0].n].arrival + 1;
          queue.erase(queue.begin()) ;
        }
          
      }

      else{
        workHis.push_back(-1);
      }
      time++ ;

      if(queue.size()==0 && last)
        return jobList ;
    }


  }
};

class PPRR{
  int time;
  int time_slice ;
  int ts ;
  vector<job> queue ;

  void Insert(job newJob){
    if(queue.size() == 0)
      queue.push_back(newJob) ;
    else{
      bool find = false ;
      for(int i=0 ; i<queue.size() && !find ; i++){
        if(newJob.priority < queue[i].priority){
          queue.insert(queue.begin()+i,newJob) ;
          find = true ;
        }
        else if(newJob.priority == queue[i].priority && i+1 < queue.size() && newJob.priority < queue[i+1].priority){
          queue.insert(queue.begin()+i+1,newJob) ;
          find = true ;
        }
      }

      if(!find)
        queue.push_back(newJob) ;
    }
  }

  public:
  vector<int> workHis ;
  vector<job> scheduler(int t, vector<job> jobList){
    time = 0 ;
    ts = t ;
    workHis.clear();
    int n = 0 ;
    bool last = false ;
    time_slice = t ;
    for(int i=0 ; i<jobList.size() ; i++){  //init
      jobList[i].waiting = 0 ;
      jobList[i].turnaround = 0 ;
    }

    bool rst = false ;
    job nowJob ;
    bool haveJob = false ;

    while(1){
      while(time >= jobList[n].arrival && !last){         //先將到達的工作加入queue中
        jobList[n].n = n ;
        Insert(jobList[n]) ;
        if(n+1<jobList.size())
          n++;
        else
          last = true ;
      }

      if(queue.size() != 0 || haveJob){
        if(haveJob == false){
          nowJob = queue[0] ;
          queue.erase(queue.begin()) ;
          haveJob = true ;
        }

        else if(haveJob && queue.size() != 0){
          if(nowJob.priority > queue[0].priority){
            job temp = nowJob ;
            nowJob = queue[0] ;
            queue.erase(queue.begin()) ;
            Insert(temp) ;
          }
        }
        workHis.push_back(nowJob.ID);
        for(int i=0 ; i<queue.size() ; i++)
          jobList[queue[i].n].waiting ++ ;
        nowJob.cpu_burst-- ;
        time_slice-- ;

        if(nowJob.cpu_burst == 0){
          jobList[nowJob.n].turnaround =  time - jobList[nowJob.n].arrival + 1;
          haveJob = false ;
          time_slice = t ;
        }

        else if(time_slice == 0){
          Insert(nowJob) ;
          haveJob = false ;
          time_slice = t ;
        }
      }

      else
        workHis.push_back(-1);

      if(queue.size()==0 && !haveJob && last)
        return jobList ;

      time++ ;
    }
  }
};

class HRRN{
  int time ;
  int time_slice ;
  public:
  vector<int> workHis ;
  vector<job> queue ;

  int GetHR(float t){
    float maxR ;
    float r ;
    int index ;
    for(int i = 0 ; i<queue.size() ; i++){
      float waiting = time - queue[i].arrival ;
      r = 1 + waiting/queue[i].cpu_burst ;
      if(r>maxR){
        maxR = r ;
        index = i ;
      }

      else if(r == maxR){
        if(queue[i].arrival < queue[index].arrival){
          maxR = r ;
          index = i ;
        }

        else if(queue[i].arrival == queue[index].arrival ){
          if(queue[i].ID < queue[index].ID){
            maxR = r ;
            index = i ;
          }
        }
      }
    }

    return index ;
  }

  vector<job> scheduler(int t, vector<job> jobList){
    time_slice = t ;
    time = 0 ;
    workHis.clear();
    int n = 0 ;
    bool last = false ;
    for(int i=0 ; i<jobList.size() ; i++){
      jobList[i].waiting = 0 ;
      jobList[i].turnaround = 0 ;
    }

    while(1){
      while(time >= jobList[n].arrival && !last){         //先將到達的工作加入queue中
        jobList[n].n = n ;
        queue.push_back(jobList[n]) ;
        if(n+1<jobList.size())
          n++;
        else
          last = true ;
      }

      if(queue.size()!=0){
        int index = GetHR(time) ;
        job nowJob = queue[index] ;
        queue.erase(queue.begin()+index) ;
        jobList[nowJob.n].waiting = time - nowJob.arrival ;
        for(int i = 0 ; i<nowJob.cpu_burst ; i++){
          workHis.push_back(nowJob.ID) ;
          time++ ;
          for(int j=0 ; j<queue.size() ; j++)
            queue[j].waiting++ ;
        }
        jobList[nowJob.n].turnaround = time - nowJob.arrival ;
      }

      else{
        workHis.push_back(-1) ;
        time++ ;
      }

      if(queue.size()==0 && last)
        return jobList ;

    }
    return jobList ;
  }
};

class proj2{
  vector<job> jobList ;
  int time_slice ;
  int model ;
  string name ;
  vector< vector<job> > result ;
  vector< vector<int> > workHis ;
  public:
	bool Read(){  // 讀入input
    jobList.clear();
		job temp ;
		string str ;
		char ch ;
		int num ;
		cout<<endl<<"請輸入檔名 : " ;
		cin>>name ;
		string fileName = name + ".txt" ;
		FILE *fPtr = NULL;
		fPtr=fopen(fileName.c_str(),"r");
		if(fPtr==NULL)
			cout<<"error file"<<endl ;
		else{
      fscanf(fPtr, "%d", &model) ;
			fscanf(fPtr, "%d", &time_slice) ;
      fscanf(fPtr, "%c", &ch) ;
      ch = '0';
			while(ch!='\n')
				fscanf(fPtr, "%c", &ch) ;
			while(fscanf(fPtr, "%d", &num)!=EOF){
				temp.ID = num ;
        fscanf(fPtr, "%d", &num);
        temp.cpu_burst = num;
        fscanf(fPtr, "%d", &num);
        temp.arrival = num ;
        fscanf(fPtr, "%d", &num);
        temp.priority = num ;
        jobList.push_back(temp);
      }


      fclose(fPtr);
			return true ;
		}
		return false ;
  }

  vector<job> BubbleSort(vector<job> jobList){
    int n = jobList.size();
    job temp ;
    for(int i=0 ; i<n ; i++){
      for(int j=i+1 ; j<n ; j++){
        if(jobList[i].arrival > jobList[j].arrival){
          temp = jobList[j] ;
          jobList[j] = jobList[i];
          jobList[i] = temp;
        }

        else if(jobList[i].arrival == jobList[j].arrival){
          if(jobList[i].ID > jobList[j].ID){
            temp = jobList[j] ;
            jobList[j] = jobList[i];
            jobList[i] = temp;
          }
        }
      }
    }
    return jobList ;
  }

  vector<job> BubbleSortByID(vector<job> jobList){
    int n = jobList.size();
    job temp ;
    for(int i=0 ; i<n ; i++){
      for(int j=i+1 ; j<n ; j++){
        if(jobList[i].ID > jobList[j].ID){
          temp = jobList[j] ;
          jobList[j] = jobList[i];
          jobList[i] = temp;
        }
      }
    }
    return jobList ;
  }

	void MakeFile() {
		FILE *fout = NULL;
    string fileName = "output_" + name + ".txt" ;
		fout = fopen( fileName.c_str(), "w" ) ;
    if(model == 1){
		  fprintf( fout, "FCFS\n" ) ;
      for(int i = 0 ; i<workHis[0].size() ; i++){
        if(workHis[0][i]>=10){
          char ch = 'A' + workHis[0][i] - 10 ;
          fprintf( fout, "%c", ch ) ;
        }

        else if(workHis[0][i]<0)
          fprintf( fout, "-" ) ;
        else
          fprintf( fout, "%d", workHis[0][i] ) ;
        
      }
      fprintf( fout, "\n===========================================================\n" ) ;
      fprintf( fout, "\nwaiting\n" ) ;
      fprintf( fout, "ID	FCFS\n" ) ;
      fprintf( fout, "===========================================================\n" ) ;
      for(int x=0 ; x<jobList.size() ; x++){
        fprintf( fout, "%d\t%d\n", result[0][x].ID, result[0][x].waiting ) ;
      }
      fprintf( fout, "===========================================================\n\n" ) ;
      fprintf( fout, "Turnaround Time\n" ) ;
      fprintf( fout, "ID	FCFS\n" ) ;
      fprintf( fout, "===========================================================\n" ) ;
      for(int x=0 ; x<jobList.size() ; x++){
        fprintf( fout, "%d\t%d\n", result[0][x].ID, result[0][x].turnaround ) ;
      }
    }

    else if(model == 2){
		  fprintf( fout, "RR\n" ) ;
      for(int i = 0 ; i<workHis[0].size() ; i++){
        if(workHis[0][i]>=10){
          char ch = 'A' + workHis[0][i] - 10 ;
          fprintf( fout, "%c", ch ) ;
        }

        else if(workHis[0][i]<0)
          fprintf( fout, "-" ) ;
        else
          fprintf( fout, "%d", workHis[0][i] ) ;
      }
      fprintf( fout, "\n===========================================================\n" ) ;
      fprintf( fout, "\nwaiting\n" ) ;
      fprintf( fout, "ID	RR\n" ) ;
      fprintf( fout, "===========================================================\n" ) ;
      for(int x=0 ; x<jobList.size() ; x++){
        fprintf( fout, "%d\t%d\n", result[0][x].ID, result[0][x].waiting ) ;
      }
      fprintf( fout, "===========================================================\n\n" ) ;
      fprintf( fout, "Turnaround Time\n" ) ;
      fprintf( fout, "ID	RR\n" ) ;
      fprintf( fout, "===========================================================\n" ) ;
      for(int x=0 ; x<jobList.size() ; x++){
        fprintf( fout, "%d\t%d\n", result[0][x].ID, result[0][x].turnaround ) ;
      }
    }

    else if(model == 3){
		  fprintf( fout, "SRTF\n" ) ;
      for(int i = 0 ; i<workHis[0].size() ; i++){
        if(workHis[0][i]>=10){
          char ch = 'A' + workHis[0][i] - 10 ;
          fprintf( fout, "%c", ch ) ;
        }

        else if(workHis[0][i]<0)
          fprintf( fout, "-" ) ;
        else
          fprintf( fout, "%d", workHis[0][i] ) ;
      }
      fprintf( fout, "\n===========================================================\n" ) ;
      fprintf( fout, "\nwaiting\n" ) ;
      fprintf( fout, "ID	SRTF\n" ) ;
      fprintf( fout, "===========================================================\n" ) ;
      for(int x=0 ; x<jobList.size() ; x++){
        fprintf( fout, "%d\t%d\n", result[0][x].ID, result[0][x].waiting ) ;
      }
      fprintf( fout, "===========================================================\n\n" ) ;
      fprintf( fout, "Turnaround Time\n" ) ;
      fprintf( fout, "ID	SRTF\n" ) ;
      fprintf( fout, "===========================================================\n" ) ;
      for(int x=0 ; x<jobList.size() ; x++){
        fprintf( fout, "%d\t%d\n", result[0][x].ID, result[0][x].turnaround ) ;
      }
    }

    else if(model == 4){
		  fprintf( fout, "PPRR\n" ) ;
      for(int i = 0 ; i<workHis[0].size() ; i++){
        if(workHis[0][i]>=10){
          char ch = 'A' + workHis[0][i] - 10 ;
          fprintf( fout, "%c", ch ) ;
        }

        else if(workHis[0][i]<0)
          fprintf( fout, "-" ) ;
        else
          fprintf( fout, "%d", workHis[0][i] ) ;
      }
      fprintf( fout, "\n===========================================================\n" ) ;
      fprintf( fout, "\nwaiting\n" ) ;
      fprintf( fout, "ID	PPRR\n" ) ;
      fprintf( fout, "===========================================================\n" ) ;
      for(int x=0 ; x<jobList.size() ; x++){
        fprintf( fout, "%d\t%d\n", result[0][x].ID, result[0][x].waiting ) ;
      }
      fprintf( fout, "===========================================================\n\n" ) ;
      fprintf( fout, "Turnaround Time\n" ) ;
      fprintf( fout, "ID	PPRR\n" ) ;
      fprintf( fout, "===========================================================\n" ) ;
      for(int x=0 ; x<jobList.size() ; x++){
        fprintf( fout, "%d\t%d\n", result[0][x].ID, result[0][x].turnaround ) ;
      }
    }

    else if(model == 5){
		  fprintf( fout, "HRRN\n" ) ;
      for(int i = 0 ; i<workHis[0].size() ; i++){
        if(workHis[0][i]>=10){
          char ch = 'A' + workHis[0][i] - 10 ;
          fprintf( fout, "%c", ch ) ;
        }

        else if(workHis[0][i]<0)
          fprintf( fout, "-" ) ;
        else
          fprintf( fout, "%d", workHis[0][i] ) ;
      }
      fprintf( fout, "\n===========================================================\n" ) ;
      fprintf( fout, "\nwaiting\n" ) ;
      fprintf( fout, "ID	HRRN\n" ) ;
      fprintf( fout, "===========================================================\n" ) ;
      for(int x=0 ; x<jobList.size() ; x++){
        fprintf( fout, "%d\t%d\n", result[0][x].ID, result[0][x].waiting ) ;
      }
      fprintf( fout, "===========================================================\n\n" ) ;
      fprintf( fout, "Turnaround Time\n" ) ;
      fprintf( fout, "ID	HRRN\n" ) ;
      fprintf( fout, "===========================================================\n" ) ;
      for(int x=0 ; x<jobList.size() ; x++){
        fprintf( fout, "%d\t%d\n", result[0][x].ID, result[0][x].turnaround ) ;
      }
    }

    else if(model == 6){
		  fprintf( fout, "All\n" ) ;
      for(int i=0 ; i<5 ; i++){
        if(i==0)
          fprintf( fout, "==        FCFS==\n" ) ;
        else if(i==1)
          fprintf( fout, "\n==          RR==\n" ) ;
        else if(i==2)
          fprintf( fout, "\n==        SRTF==\n" ) ;
        else if(i==3)
          fprintf( fout, "\n==        PPRR==\n" ) ;
        else if(i==4)
          fprintf( fout, "\n==        HRRN==\n" ) ;
        for(int j = 0 ; j<workHis[i].size() ; j++){
          if(workHis[i][j]>=10){
            char ch = 'A' + workHis[i][j] - 10 ;
            fprintf( fout, "%c", ch ) ;
          }

          else if(workHis[i][j]<0)
            fprintf( fout, "-" ) ;
          else
            fprintf( fout, "%d", workHis[i][j] ) ;
        }
      }
      fprintf( fout, "\n===========================================================\n" ) ;
      fprintf( fout, "\nwaiting\n" ) ;
      fprintf( fout, "ID	FCFS	RR	SRTF	PPRR	HRRN\n" ) ;
      fprintf( fout, "===========================================================\n" ) ;
      for(int x=0 ; x<jobList.size() ; x++){
        fprintf( fout, "%d\t%d\t%d\t%d\t%d\t%d\n", result[0][x].ID, result[0][x].waiting, result[1][x].waiting, result[2][x].waiting, result[3][x].waiting, result[4][x].waiting  ) ;
      }
      fprintf( fout, "===========================================================\n\n" ) ;
      fprintf( fout, "Turnaround Time\n" ) ;
      fprintf( fout, "ID	FCFS	RR	SRTF	PPRR	HRRN\n" ) ;
      fprintf( fout, "===========================================================\n" ) ;
      for(int x=0 ; x<jobList.size() ; x++){
        fprintf( fout, "%d\t%d\t%d\t%d\t%d\t%d\n", result[0][x].ID, result[0][x].turnaround, result[1][x].turnaround, result[2][x].turnaround, result[3][x].turnaround, result[4][x].turnaround  ) ;
      }
      fprintf( fout, "===========================================================\n\n" ) ;
	  }

    fclose( fout ) ;
  }






  void Do(){
    result.clear() ;
    jobList = BubbleSort(jobList) ;
    workHis.clear() ;
    if(model==1){
      FCFS model1 ;
      result.push_back( BubbleSortByID( model1.scheduler(time_slice,jobList) ) );
      workHis.push_back(model1.workHis) ;
    }
    else if(model==2){
      RR model2 ;
      result.push_back( BubbleSortByID( model2.scheduler(time_slice,jobList) ) );
      workHis.push_back(model2.workHis) ;
    }
    else if(model==3){
      SRTF model3 ;
      result.push_back( BubbleSortByID( model3.scheduler(time_slice,jobList) ) );
      workHis.push_back(model3.workHis) ;
    }
    else if(model==4){
      PPRR model4 ;
      result.push_back( BubbleSortByID( model4.scheduler(time_slice,jobList) ) );
      workHis.push_back(model4.workHis) ;
    }
    else if(model==5){
      HRRN model5 ;
      result.push_back( BubbleSortByID( model5.scheduler(time_slice,jobList) ) );
      workHis.push_back(model5.workHis) ;
    }
    else if(model==6){
      FCFS model1 ;
      RR model2 ;
      SRTF model3 ;
      PPRR model4 ;
      HRRN model5 ;
      result.push_back( BubbleSortByID( model1.scheduler(time_slice,jobList) ) );
      result.push_back( BubbleSortByID( model2.scheduler(time_slice,jobList) ) );
      result.push_back( BubbleSortByID( model3.scheduler(time_slice,jobList) ) );
      result.push_back( BubbleSortByID( model4.scheduler(time_slice,jobList) ) );
      result.push_back( BubbleSortByID( model5.scheduler(time_slice,jobList) ) );
      workHis.push_back(model1.workHis) ;
      workHis.push_back(model2.workHis) ;
      workHis.push_back(model3.workHis) ;
      workHis.push_back(model4.workHis) ;
      workHis.push_back(model5.workHis) ;
    }

    MakeFile() ;
  }
};

int main(int argc, char** argv) {
  proj2 p ;
  p.Read() ;
  p.Do() ;
	return 0;
}
