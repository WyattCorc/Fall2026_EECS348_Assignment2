#include <iostream>//C++ basic lib for I/O
#include <string>//Required to use strings in objects
#include <vector>//Type of array in C++
#include <sstream>// Allows strings to be used in i/o

struct Email {//Makes a structure, similar to a class
    std::string sender, subject, date;


    int priorityScore() const {//funct that outputs and int, reads the "sender" str and returns weight vals
        if (sender == "Boss") return 5;//Boss returns the highest
        if (sender == "Subordinate") return 4;//Subordanate returns second highest
        if (sender == "Peer") return 3;//peer returns 3rd
        if (sender == "ImportantPerson") return 2;//important is in 4th
        return 1; // all others are output to the lowest prio
    }

    // Uses the date var as an aray and converts it to yyyymmdd to be more easily worked with
    std::string formattedDate() const {
        return date.substr(6, 4) + date.substr(0, 2) + date.substr(3, 2);
    }

    bool operator<(const Email& other) const {
        if (priorityScore() != other.priorityScore())//Runs a comparason, returning true if current has a lower prio
            return priorityScore() < other.priorityScore();
        return formattedDate() > other.formattedDate(); //If they are the same and the if block is bypassed, the newest takes precident
    }
};

class MaxHeap {//Object that stores and sorts the emails
    std::vector<Email> heap;

    void siftUp(int i) {//fuct, uses void to signify it has no value and will not return anything
        while (i > 0 && heap[(i - 1) / 2] < heap[i]) {//The while keeps it running till index[i] reaches the root
            std::swap(heap[i], heap[(i - 1) / 2]);//Looks at the child, uses the formula to find the parent, swaps if it is smaller
            i = (i - 1) / 2;//updates the index if they are swapped
        }
    }

    void siftDown(int i) {//void so it will not return anything
        int maxIdx = i, left = 2 * i + 1, right = 2 * i + 2, size = heap.size();//finds the parent, assuming it is the largest
        if (left < size && heap[maxIdx] < heap[left]) maxIdx = left;//Checks the left child updates maxid if its bigger
        if (right < size && heap[maxIdx] < heap[right]) maxIdx = right;//Checks the right child updates maxid if its bigger
        if (i != maxIdx) {//If maxid was changed it swaps the largest child with the parent
            std::swap(heap[i], heap[maxIdx]);//physically does the swap
            siftDown(maxIdx);//recursion to call itself till the whole tree is sorted
        }
    }

public://Makes the functs below accessable by all instances of the classes
    void push(const Email& email) {//reads the email, requires no return since there is a void
        heap.push_back(email);//Pushes the new email to the back of the array
        siftUp(heap.size() - 1);//Uses the sift up to move up until it is in the correct position
    }

    void pop() {//basically removes or pops off the top of the list after its in this case read
        if (heap.empty()) return;//fixes edge case in which you try to pop and empty list
        heap[0] = heap.back();//moves the one at the bottom to the top
        heap.pop_back();//removes the one at the bottom
        siftDown(0);//uses siftdown to recursivley sort the whole thing and make it return to its right place
    }

    Email top() const { return heap.front(); }//returns the email at the top of the heap
    size_t size() const { return heap.size(); }//basically a len for the heap, looks for the total amount of items
    bool empty() const { return heap.empty(); }//checks if the heap is empty
};

int main() {//starts the main funct

    MaxHeap inbox;//creates the Maxheap with the variblename inbox
    std::string line;//does the I/O from the commandline to find the file

    while (std::getline(std::cin, line)) {//Looks through the .txt and runs the functions based on the keywords given
        if (line.rfind("EMAIL ", 0) == 0) {//if the keyword/commmand is EMAIL
            std::stringstream ss(line.substr(6));
            Email email;//Creates and email struct, grabbing the following information
            if (std::getline(ss, email.sender, ',')&&//sender
            std::getline(ss, email.subject, ',')&&//subject
            std::getline(ss, email.date)){//date
            //Following 2 lines clean the strings specifically trimming leading spaces
            if (!email.subject.empty() && email.subject[0] == ' ') email.subject.erase(0, 1);
            if (!email.date.empty() && email.date[0] == ' ') email.date.erase(0, 1);

            inbox.push(email);//adds it to the heap, named inbox, pushes to the bottom
            }
        } 
        else if (line == "COUNT") {//runs the count command, shows the total number of emails/objects left in inbox
            std::cout << "There are " << inbox.size() << " emails to read.\n";
        } 
        else if (line == "NEXT") {//runs the next command
            if (!inbox.empty()) {//Handles the is empty edgecase
                Email e = inbox.top();
                std::cout << "Next email:\nSender: " << e.sender //Handles printing the information from the struct
                          << "\nSubject: " << e.subject 
                          << "\nDate: " << e.date << "\n";
            }
            else{//Added if user hits the edgecase
                std::cout << "Inbox is empty\n";
            }
        } 
        else if (line == "READ") {//Read function pops of the top after checking if the inbox is not empty
            if (!inbox.empty()) inbox.pop();//checks if it is not empty, if so pops off the top item
            if (inbox.empty()) std::cout <<"Inbox is empty\n";//added feedback if the user hits an edgecase
        }
    }
    return 0;//ends main()
}