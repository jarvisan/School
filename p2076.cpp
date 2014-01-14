//  Andrew Jarvis

#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <unordered_map>
using namespace std;


int main(int argc, const char * argv[])
{
    ifstream input (argv[1]);
    int availableSpace, thickness;
    string command, title;
    vector<pair<string,int> > shelf;
    pair<string,int> book;
    unordered_map<string, int> masterList;

    input >> availableSpace;

    while (input >> command) {
        input >> std::ws;
        //ADD
        if(command == "ADD") {
            getline(input, title);
            auto pos = title.find_last_of(' ');
            thickness = atoi(title.substr(pos, title.size()).c_str());
            title = title.substr(0, pos);
            for(unsigned int i = 0; i < title.length(); i++){
                if(title.find_last_of(' ') == title.length()-1){
                    title = title.substr(0, title.size()-1);
                }
            }
            book.first = title;
            book.second = thickness;
            masterList.insert(book);
            if(thickness <= availableSpace){
                shelf.push_back(book);
                availableSpace = availableSpace - thickness;
            }
            else{
                while(availableSpace < thickness){
                    availableSpace = availableSpace + shelf.front().second;
                    shelf.erase(shelf.begin());
                }
                shelf.push_back(book);
                availableSpace = availableSpace - thickness;
            }
        }
        //CHECKOUT
        else if (command == "CHECKOUT") {
            getline(input, title);
            for(unsigned int i = 0; i < shelf.size(); i++){
                if(shelf[i].first.compare(title) == 0){
                    availableSpace = availableSpace + shelf[i].second;
                    shelf.erase(shelf.begin()+i);
                }
            }
        }
        //RETURN
        else if (command == "RETURN") {
            getline(input, title);
            thickness = masterList.find(title)->second;
            book.second = thickness;
            book.first = title;
            if(thickness <= availableSpace){
                shelf.push_back(book);
                availableSpace = availableSpace - thickness;
            }
            else{
                while(availableSpace < thickness){
                    availableSpace = availableSpace + shelf.front().second;
                    shelf.erase(shelf.begin());
                }
                shelf.push_back(book);
                availableSpace = availableSpace - thickness;
            }
        }
        //PRINT
        else {
            for(int i = shelf.size()-1; i > -1; i--)
                cout <<setw(30)<< shelf[i].first << setw(14) << shelf[i].second << endl;
            cout << "AVAILABLE SHELF SPACE:"<< setw(21) << availableSpace << endl;
            cout << "\n";
        }
    }
    return 0;
}
