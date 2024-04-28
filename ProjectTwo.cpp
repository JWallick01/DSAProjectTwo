#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>

using namespace std;


// display menu
void DisplayMenu() {
    cout << "1. Load Data Structure" << endl;
    cout << "2. Print Course List" << endl;
    cout << "3. Print Course" << endl;
    cout << "9. Exit" << endl << endl;
    cout << "What would you like to do? ";
}


//structure for course with course and prereq constructors. 
struct Course {   
    string courseName;
    string courseId;
    vector<string> preReqs;

    //course constructor
    Course() {};
        Course(string aCourseId, string aCourseName) {
            courseId = aCourseId;
            courseName = aCourseName;
        }
   //course prereq constructor
        Course(string aCourseId, string aCourseName, vector<string> prereqs) {
            courseId = aCourseId;
            courseName = aCourseName;
            vector<string> preReqs = prereqs;
        }
    
};
//structure for Node with constructor
struct Node {
    Node* right;
    Node* left;
    Course course;

    //node default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }
    
    Node(Course acourse) : Node() {
        this->course = acourse;
    }
};

//BinarySearchTree class
class BinarySearchTree {
private:
    Node* root;
    void addNode(Course acourse, Node* node);
    void inOrder(Node* node);
    void printCourse(Node* node, string courseNum);

public:
    BinarySearchTree();
    void InOrder();
    void PrintCourse(string courseNum);
    bool loadData();
    bool validatePreReqs(Course course);
    void insert(Course course);

};

//default  BinarySearchTree constructor
BinarySearchTree::BinarySearchTree() {
    root = nullptr;
}

void BinarySearchTree::InOrder() {
    this->inOrder(root);
}

//passes root node to print course
void BinarySearchTree::PrintCourse(string courseNum) {
    this->printCourse(root, courseNum);
}

//inserting a course in the BinarySearchTree
void BinarySearchTree::insert(Course course) {
    //if root is empty
    if (root == nullptr) {
        root = new Node(course);
    }
    //call addNode and pass root after tree has been started
    else {
        this->addNode(course, root);
    }
}

//adding a node to BinarySearchTree
void BinarySearchTree::addNode(Course acourse, Node* node) {

    //if current node is larger than passed courseId
    if (node->course.courseId.compare(acourse.courseId) > 0) {
        //if no left node
        if (node->left == nullptr) {
            node->left = new Node(acourse);
        }
        //recurse down left side
        else {
            this->addNode(acourse, node->left);
        }
    }
    //if current node is smaller than passed courseId
    else {
        //if no right node
        if (node->right == nullptr) {
            node->right = new Node(acourse);
        }
        //recurse down right side
        else {
            this->addNode(acourse, node->right);
        }
    }
}

//Print Course in order
void BinarySearchTree::inOrder(Node* node){

    if (node != nullptr) {
        //recurse down left side
        inOrder(node->left);

        //print course information at current node
        cout << "Course Number: " << node->course.courseId;
        cout << " Course Name: " << node->course.courseName;
        cout << " Prerequisite(s): ";

        //iterate through course's prereq to print
        for (int i = 0; i < node->course.preReqs.size(); ++i) {
            cout << node->course.preReqs[i] << " ";
        }
        if (node->course.preReqs.size() == 0) {
            cout << "None";
        }
        cout << "\n";

        //recurse right side
        inOrder(node->right);
    }
    return;
}

//Printing a course
void BinarySearchTree::printCourse(Node* node, string courseNum) {

    while (node != nullptr) {
        //if match is found
        if (node->course.courseId.compare(courseNum) == 0) {
            cout << "Course Number: " << node->course.courseId;
            cout << "  Course Name: " << node->course.courseName;
            cout << "  Prerequisite(s): ";

            //iterate through prereq to print
            for (int i = 0; i < node->course.preReqs.size(); ++i) {
                cout << node->course.preReqs[i] << " ";
            }

            //if no prereqs
            if (node->course.preReqs.size() == 0) {
                cout << "None";
            }
            cout << "\n";
            return;
        }
        //if current course number is smaller than the one passed in
        else if (node->course.courseId.compare(courseNum) > 0 && node->left != nullptr) {
            node = node->left;
        }
        //if current course number is larger than the one passed in
        else if (node->course.courseId.compare(courseNum) < 0 && node->right != nullptr) {
            node = node->right;
        }
        else {
            cout << "Course not found. Are you sure you entered the right course number? \n";
            return;
        }
    }
    //error if tree is not loaded
    if (root == nullptr) {
        cout << "Error loading data. =[ \n";
        return;
    }
}

//loading a file into the BinarySearchTree
bool BinarySearchTree::loadData() {

    //ifstream to open file
    ifstream file;
    file.open("Courses.txt");

    if (file.is_open()) {
        //while end of file is not reached
        while (!file.eof()) {
            //create a vector to hold line items
            vector<string> courseLine;
            // parse line into vector
            string line;

            //get line from file and assign to 'line'
            getline(file, line);
            //while line is not empty
            while (line.length() > 0) {
                unsigned int delim = line.find(',');
                //limits data size to 99 or less characters
                if (delim < 100) {
                    //add item to courseLine, then delete
                    courseLine.push_back(line.substr(0, delim));
                    line.erase(0, delim + 1);
                }
                //add item following last comma, then clear line
                else {
                    courseLine.push_back(line.substr(0, line.length()));
                    line = "";
                }
            }

            
            Course course;
            //assign course number and name

            course.courseId = courseLine[0];
            course.courseName = courseLine[1];
            //push items 3+ into prereq vector
            for (unsigned int i = 2; i < courseLine.size(); i++) {
                course.preReqs.push_back(courseLine[i]);
            }
            //insert the course into binarySearchTree
            insert(course);
        }
        return true;
    }
    if (!file) {
        cout << "Something went wrong and the file didn't open.\n";
        return false;
    }
    file.close();
}

//validating prerequisite courses
bool BinarySearchTree::validatePreReqs(Course course) {
    //create local node and set to root
    Node* current = root;
    if (current->course.preReqs.size() > 0) {
        //iterate through prerequisite vector
        for (int i = 0; i < course.preReqs.size(); i++) {
            while (current != nullptr) {
                //if match is found
                if (course.preReqs[i] == current->course.courseId) {
                    return true;
                }
                //if course number is smaller than prerequisite
                else if (course.preReqs[i].compare(current->course.courseId) > 0 && current->left != nullptr) {
                    current = current->left;
                }
                //if course number is larger than prerequisite
                else if (course.preReqs[i].compare(current->course.courseId) < 0 && current->right != nullptr) {
                    current = current->right;
                }
                //if corresponding course number is not found
                else {
                    cout << "Prerequisite not found. \n";
                    return false;
                }
            }
        }
    }
    else {
        //no prerequisites
        return true;
    }
}

//Main function
int main() {

    BinarySearchTree* binarySearchTree = new BinarySearchTree();
    string courseId;
    int userInput = 0; //takes user choice for the menu set to 0
    do {
        std::cout << "Welcome to the course planner.\n";
        DisplayMenu();
        cin >> userInput;

        switch (userInput) {  //switch to go through menu options depending on user Input
        case 1: { 
            binarySearchTree->loadData();
            break; };

        case 2: { 
            binarySearchTree->InOrder();
            cout << "\n";
            break; };
        case 3: { 
            cout << "What course would you like to know about? " << endl;
            cin >> courseId;
            binarySearchTree->PrintCourse(courseId);
            break; };


        case 9: {
            cout << "Thank you for using our course planner!" << endl;
            break;
        }
        default: {
            cout << userInput << " is not a valid option. Please try again" << endl;
            break;
        }

        }
    } while (userInput != 9); //continues program as long as user doesnt select 9
}