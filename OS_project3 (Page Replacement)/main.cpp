#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <vector>
#include <time.h>

using namespace std;

string name ;
FILE *fout = NULL;
int page_frame ;
vector<int> page_ref ;

bool Read(){  // 讀入input
	cout<<endl<<"請輸入檔名 : " ;
	cin>>name ;
	string fileName = name + ".txt" ;
	FILE *fPtr = NULL;
	fPtr=fopen(fileName.c_str(),"r");
  page_ref.clear() ;

  char ch ;
	if(fPtr==NULL)
		cout<<"can't find this file"<<endl ;
	else{
    fscanf(fPtr, "%d", &page_frame) ;
    fscanf(fPtr, "%c", &ch) ;    //換行
		while(fscanf(fPtr, "%c", &ch)!=EOF){
      if(ch != '\n')
        page_ref.push_back(int(ch)-48) ;
    }

    fclose(fPtr);
	  return true ;
  }
  return false ;
}

void FIFO() {
  int fault = 0 ;
  int replaces = 0 ;
  vector<int> buffer ;
  buffer.clear() ;

  fprintf( fout, "--------------FIFO-----------------------\n" ) ;

  for (int i = 0 ; i<page_ref.size() ; i++){
    bool f = false ;
    bool stored = false ;
    for ( int j = 0 ; j<buffer.size() ;j++ )
      if ( buffer[j] == page_ref[i] )
        stored = true ;
    if ( !stored ) {
      f = true ;
      fault++ ;
      if ( buffer.size() < page_frame ) {
        buffer.insert(buffer.begin(), page_ref[i]) ;
      }

      else {
        replaces++ ;
        buffer.pop_back() ;
        buffer.insert(buffer.begin(), page_ref[i]) ;
      }
    }

    fprintf( fout, "%d\t", page_ref[i] ) ;
    for(int x = 0 ; x < buffer.size() ; x++)
      fprintf( fout, "%d", buffer[x] ) ;
    if ( f )
      fprintf( fout, "\tF" ) ;
    fprintf( fout, "\n" ) ;
  }
  fprintf( fout, "Page Fault = %d  Page Replaces = %d  Page Frames = %d\n", fault, replaces ,page_frame ) ;
}

void LRU() {
  int fault = 0 ;
  int replaces = 0 ;
  vector<int> buffer ;
  buffer.clear() ;

  fprintf( fout, "\n--------------LRU-----------------------\n" ) ;

  for (int i = 0 ; i<page_ref.size() ; i++){
    bool f = false ;
    bool stored = false ;
    int index ;
    for ( int j = 0 ; j<buffer.size() ;j++ )
      if ( buffer[j] == page_ref[i] ) {
        stored = true ;
        index = j ;
      }

    if ( !stored ) {
      f = true ;
      fault++ ;
      if ( buffer.size() < page_frame ) {
        buffer.insert(buffer.begin(), page_ref[i]) ;
      }

      else {
        replaces++ ;
        buffer.pop_back() ;
        buffer.insert(buffer.begin(), page_ref[i]) ;
      }
    }

    else {
      buffer.erase(buffer.begin()+index) ;
      buffer.insert(buffer.begin(), page_ref[i]) ;
    }

    fprintf( fout, "%d\t", page_ref[i] ) ;
    for(int x = 0 ; x < buffer.size() ; x++)
      fprintf( fout, "%d", buffer[x] ) ;
    if ( f )
      fprintf( fout, "\tF" ) ;
    fprintf( fout, "\n" ) ;
  }

  fprintf( fout, "Page Fault = %d  Page Replaces = %d  Page Frames = %d\n", fault, replaces ,page_frame ) ;
}

vector<int> MakeCounter() {
  int max = -1 ;
  for (int i = 0 ; i<page_ref.size() ; i++)
    if (max<page_ref[i])
      max = page_ref[i] ;
  vector<int> temp ;
  for(int i = 0 ; i<= max ; i++)
    temp.push_back(0) ;
  return temp ;
}

void LFU() {
  vector<int> counter ;
  counter = MakeCounter() ;

  int fault = 0 ;
  int replaces = 0 ;
  vector<int> buffer ;
  buffer.clear() ;

  fprintf( fout, "\n--------------Least Frequently Used Page Replacement-----------------------\n" ) ;

  for (int i = 0 ; i<page_ref.size() ; i++){
    bool f = false ;
    bool stored = false ;
    int index ;
    for ( int j = 0 ; j<buffer.size() ;j++ )
      if ( buffer[j] == page_ref[i] ) {
        stored = true ;
        index = j ;
      }

    if ( !stored ) {
      f = true ;
      fault++ ;
      if ( buffer.size() < page_frame ) {
        buffer.insert(buffer.begin(), page_ref[i]) ;
      }

      else {
        replaces++ ;
        counter[page_ref[i]] = 0 ;
        int min = counter[buffer[0]] ;
        int minIndex = 0;
        for ( int j = 0 ; j < buffer.size() ; j++ )
          if ( counter[buffer[j]] <= min ) {
            min = counter[buffer[j]] ;
            minIndex = j ;
          }

        buffer.erase(buffer.begin()+minIndex) ;
        buffer.insert(buffer.begin(), page_ref[i]) ;
      }
    }

    counter[page_ref[i]]++ ;

    fprintf( fout, "%d\t", page_ref[i] ) ;
    for(int x = 0 ; x < buffer.size() ; x++)
      fprintf( fout, "%d", buffer[x] ) ;
    if ( f )
      fprintf( fout, "\tF" ) ;
    fprintf( fout, "\n" ) ;
  }

  fprintf( fout, "Page Fault = %d  Page Replaces = %d  Page Frames = %d\n", fault, replaces ,page_frame ) ;
}

void MFU() {
  vector<int> counter ;
  counter = MakeCounter() ;

  int fault = 0 ;
  int replaces = 0 ;
  vector<int> buffer ;
  buffer.clear() ;

  fprintf( fout, "\n--------------Most Frequently Used Page Replacement -----------------------\n" ) ;


  for (int i = 0 ; i<page_ref.size() ; i++){
    bool f = false ;
    bool stored = false ;
    int index ;
    for ( int j = 0 ; j<buffer.size() ;j++ )
      if ( buffer[j] == page_ref[i] ) {
        stored = true ;
        index = j ;
      }

    if ( !stored ) {
      f = true ;
      fault++ ;
      if ( buffer.size() < page_frame ) {
        buffer.insert(buffer.begin(), page_ref[i]) ;
      }

      else {
        replaces++ ;
        counter[page_ref[i]] = 0 ;
        int max = counter[buffer[0]] ;
        int maxIndex = 0;
        for ( int j = 0 ; j < buffer.size() ; j++ )
          if ( counter[buffer[j]] >= max ) {
            max = counter[buffer[j]] ;
            maxIndex = j ;
          }

        buffer.erase(buffer.begin()+maxIndex) ;
        buffer.insert(buffer.begin(), page_ref[i]) ;
      }
    }

    counter[page_ref[i]]++ ;

    fprintf( fout, "%d\t", page_ref[i] ) ;
    for(int x = 0 ; x < buffer.size() ; x++)
      fprintf( fout, "%d", buffer[x] ) ;
    if ( f )
      fprintf( fout, "\tF" ) ;
    fprintf( fout, "\n" ) ;
  }

  fprintf( fout, "Page Fault = %d  Page Replaces = %d  Page Frames = %d\n", fault, replaces ,page_frame ) ;
}

void LFU_LRU() {
  vector<int> counter ;
  counter = MakeCounter() ;

  int fault = 0 ;
  int replaces = 0 ;
  vector<int> buffer ;
  buffer.clear() ;

  fprintf( fout, "\n--------------Least Frequently Used LRU Page Replacement-----------------------\n" ) ;

  for (int i = 0 ; i<page_ref.size() ; i++){
    bool f = false ;
    bool stored = false ;
    int index ;
    for ( int j = 0 ; j<buffer.size() ;j++ )
      if ( buffer[j] == page_ref[i] ) {
        stored = true ;
        index = j ;
      }

    if ( !stored ) {
      f = true ;
      fault++ ;
      if ( buffer.size() < page_frame ) {
        buffer.insert(buffer.begin(), page_ref[i]) ;
      }

      else {
        counter[page_ref[i]] = 0 ;
        replaces++ ;
        int min = counter[buffer[0]] ;
        int minIndex = 0;
        for ( int j = 0 ; j < buffer.size() ; j++ )
          if ( counter[buffer[j]] <= min ) {
            min = counter[buffer[j]] ;
            minIndex = j ;
          }

        buffer.erase(buffer.begin()+minIndex) ;
        buffer.insert(buffer.begin(), page_ref[i]) ;
      }
    }

    else {
      buffer.erase(buffer.begin()+index) ;
      buffer.insert(buffer.begin(), page_ref[i]) ;
    }

    counter[page_ref[i]]++ ;

    fprintf( fout, "%d\t", page_ref[i] ) ;
    for(int x = 0 ; x < buffer.size() ; x++)
      fprintf( fout, "%d", buffer[x] ) ;
    if ( f )
      fprintf( fout, "\tF" ) ;
    fprintf( fout, "\n" ) ;
  }

  fprintf( fout, "Page Fault = %d  Page Replaces = %d  Page Frames = %d\n", fault, replaces ,page_frame ) ;
}

void MFU_LRU() {
  vector<int> counter ;
  counter = MakeCounter() ;

  int fault = 0 ;
  int replaces = 0 ;
  vector<int> buffer ;
  buffer.clear() ;

  fprintf( fout, "\n--------------Most Frequently Used LRU Page Replacement -----------------------\n" ) ;

  for (int i = 0 ; i<page_ref.size() ; i++){
    bool f = false ;
    bool stored = false ;
    int index ;
    for ( int j = 0 ; j<buffer.size() ;j++ )
      if ( buffer[j] == page_ref[i] ) {
        stored = true ;
        index = j ;
      }

    if ( !stored ) {
      f = true ;
      fault++ ;
      if ( buffer.size() < page_frame ) {
        buffer.insert(buffer.begin(), page_ref[i]) ;
      }

      else {
        replaces++ ;
        counter[page_ref[i]] = 0 ;
        int max = counter[buffer[0]] ;
        int maxIndex = 0;
        for ( int j = 0 ; j < buffer.size() ; j++ )
          if ( counter[buffer[j]] >= max ) {
            max = counter[buffer[j]] ;
            maxIndex = j ;
          }

        buffer.erase(buffer.begin()+maxIndex) ;
        buffer.insert(buffer.begin(), page_ref[i]) ;
      }
    }

    else {
      buffer.erase(buffer.begin()+index) ;
      buffer.insert(buffer.begin(), page_ref[i]) ;
    }

    counter[page_ref[i]]++ ;

    fprintf( fout, "%d\t", page_ref[i] ) ;
    for(int x = 0 ; x < buffer.size() ; x++)
      fprintf( fout, "%d", buffer[x] ) ;
    if ( f )
      fprintf( fout, "\tF" ) ;
    fprintf( fout, "\n" ) ;
  }

  fprintf( fout, "Page Fault = %d  Page Replaces = %d  Page Frames = %d\n", fault, replaces ,page_frame ) ;
}

int main(int argc, char** argv) {
  int cmd ;
  cout << "輸入指令(0:Quit, 1:執行) : " ;
  cin >> cmd ;
  while(cmd != 0) {
    if(Read() && cmd == 1 ) {
      string fileName = "output_" + name + ".txt" ;
		  fout = fopen( fileName.c_str(), "w" ) ;
      FIFO() ;
      LRU() ;
      LFU() ;
      MFU() ;
      LFU_LRU() ;
      MFU_LRU() ;
      fclose( fout ) ;
    }

    cout << "\n輸入指令(0:Quit, 1:執行) : " ;
    cin >> cmd ;
  }

	return 0;
}
