#include <thread>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <vector>

using namespace std;
//mode one makes list up to X
void listloop();
//mode two checks values 2 to X for prime numbers 
void limitloop();
//thread/pipe creation for mode 2
void primec(int num, int fd);
//trhead/pipe creation for mode 1
void primelim(int pri, int fdr);

//total number of primes found so far used in mode 1
int primecount = 1;
//number of prmes desired used in mode 1
int nprime;
//step mode
char on;
//select mode
int main(){
    int mo;
    cout << "Mode 1(list) or 2(limit): ";
    scanf("%i", &mo);
    getchar();
    if(mo == 1){
	cout << "step mode on? (y) or (n)";
	cin >> on;
	if(on != 'y' && on != 'n'){
	  cout << "invalid selection.\n";
	  exit (0);
	}
        listloop();
    }
    if(mo == 2){
	cout << "step mode on? (y) or (n)";
	cin >> on;
	if(on != 'y' && on != 'n'){
	  cout << "invalid selection.\n";
	  exit (0);
	}
        limitloop();
    }
    else cout << "invalid selection.\n";
}
//mode one makes list up to X
void listloop(){
    //list of numbers to be passed along
    vector<int> numlist;
    //contains initial pipe read/write
    int fd[2];
    cout << "Enter number of primes: ";
    scanf("%i", &nprime);
    //Reaches resource limit
    if(nprime > 500){
      cout << "Warining may not finish too many requested.\n";
      cin.get();
    }
    if(nprime <= 0){
      cout << "Nothing to calculate\n";
      exit (0);
    }
    getchar();
    //pipe for comunicating with first thread
    pipe(fd);
    //make list of numbers
    for(int i = nprime*100; i > 2; i--){
	numlist.push_back(i);
    }
    
    cout << "Prime: 2, this is built in.\n";
    //check if number is multiple of 2
    for(unsigned int i = 0; i < numlist.size(); i++){
	if(numlist[i]%2==0)
	  numlist.erase(numlist.begin()+i);
    }
    //writes list to pipe
    write(fd[1], &numlist, sizeof(numlist));
    //spawns thread
    if(on == 'y'){
      cout << "first thread spawns next\n";
      cin.get();
    }
    thread next(primelim, numlist.back(), fd[0]);
    //wait for threads
    next.join();
}
//mode two checks values 2 to X for prime numbers 
void limitloop(){
    //list of numbers to be passed along
    vector<int> numlist;
    //ending number for list
    int limit;
    //contains initial pipe read/write
    int fd[2];
    cout << "Enter ending value: ";
    scanf("%i", &limit); 
    getchar();
    //Reaches resource limit
    if(limit > 3000){
      cout << "Warining may not finish large request.\n";
      cin.get();
    }
    if(limit <= 1){
      cout << "Nothing to calculate\n";
      exit (0);
    }
    //pipe for comunicating with first thread
    pipe(fd);
    
    for(int i = limit; i > 2; i--){
	numlist.push_back(i);
    }
    
    cout << "Prime: 2, this is built in.\n";
    if(limit == 2){
     cout << "done\n";
     exit(0);
    }
    //check if number is multiple of 2
    for(unsigned int i = 0; i < numlist.size(); i++){
	if(numlist[i]%2==0)
	  numlist.erase(numlist.begin()+i);
    }
    //writes list to pipe
    write(fd[1], &numlist, sizeof(numlist));
    //spawns thread
    if(on == 'y'){
      cout << "first thread spawns next\n";
      cin.get();
    }
    thread next(primec, numlist.back(), fd[0]);
    //wait for threads
    next.join();
}
//thread/pipe creation for mode 2
void primec(int pri, int fdr){
    //where the list to be checked/sent is stored
    vector<int> newlist;
    //pipe info for each thread
    int fd[2];
    //read list from previous thread
    read(fdr, &newlist, sizeof(newlist));
    //check to see if done
    if(newlist.size() == 0){
      cout << "List Complete\n";
      exit(0);
    }
    //print out prime and thread id responsible for that prime
    cout << "Prime: " << pri << ", ";
    cout << "Thread id: " << this_thread::get_id() << "\n";
    //remove prime that was just printed
    newlist.pop_back();
    //remove values that are multiples of current prime
    for(unsigned int i = 0; i < newlist.size(); i++){
	if(newlist[i]%pri==0)
	  newlist.erase(newlist.begin()+i);
    }
    //open pipe for writing
    pipe(fd);
    //pass list to pipe
    write(fd[1], &newlist, sizeof(newlist));
    //spawn new thread
    if(on == 'y'){
      cout << "New thread next\n";
      cin.get();
    }
    thread next(primec,newlist.back(), fd[0]);
    //wait
    next.join();
}
//trhead/pipe creation for mode 1
void primelim(int pri, int fdr){
    //where the list to be checked/sent is stored
    vector<int> newlist;
    //pipe info for each thread
    int fd[2];
    //read list from previous thread
    read(fdr, &newlist, sizeof(newlist));
    //check to see if done
    if(primecount == nprime){
      cout << "List Complete\n";
      exit(0);
    }
    //print out prime and thread id responsible for that prime
    cout << "Prime: " << pri << ", ";
    primecount++;
    cout << "Thread id: " << this_thread::get_id() << "\n";
    //remove prime that was just printed
    newlist.pop_back();
    //remove values that are multiples of current prime
    for(unsigned int i = 0; i < newlist.size(); i++){
	if(newlist[i]%pri==0)
	  newlist.erase(newlist.begin()+i);
    }
    //open pipe for writing
    pipe(fd);
    //pass list to pipe
    write(fd[1], &newlist, sizeof(newlist));
    //spawn new thread
    if(on == 'y'){
      cout << "New thread spawns\n";
      cin.get();      
    }
    thread next(primelim,newlist.back(), fd[0]);
    //wait
    next.join();
  
}
