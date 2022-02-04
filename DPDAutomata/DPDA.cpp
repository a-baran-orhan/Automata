#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <set>
#include <sstream>
#include <fstream>

using namespace std;

typedef struct myDpda{
    string state_first; //inital state of the states
    char input_char;
    char top_of_stack;
    string latest_stat;  //will provide next state
    char latest_stack_up; //character will written to stack
} myDpda_t;

class Automata
{
public:
    Automata(const string &given_file, string basicString);
    void writing_and_operations(string strr, const string& given_file);
private:
    string present_state;
    deque<char> stack;
    set<char> our_alphabet;
    set<char> our_stack_alp;
    set<string> states;
    string initial_state;
    set<string> agreed_states; //final states
    vector<myDpda_t> transitions; };

string  eraseDemo(string str) //STRING OPERATION FOR THE Automata FILE STRINGS
{
    str.erase(0, 5);
    return str;
}

string splitter(string split_line,string delim){ //STRING OPERATION FOR THE Automata FILE STRINGS
    string s = split_line;
    string delimiter = delim;
    auto start = 0U;
    auto end = s.find(delimiter);
    while (end != string::npos)
    {

        start = end + delimiter.length();
        end = s.find(delimiter, start);
    }
    return s.substr(start,end);

}
int main(int argc, char* argv[]) {
    // GETTING FILES FROM COMMAND LINE ARGUMENT
    string input_file;
    string string_input;
    string output_file;
    if(argc == 4) {
        input_file = argv[1];
        string_input = argv[2];
        output_file = argv[3];
    }
    fstream f;
    f.open(output_file,fstream::out | fstream::trunc); //FOR CLEAN THE OUTPUT FILE


    Automata dpda(input_file, output_file);
    //FOR THE INPUT FILE
    string line;
    ifstream myfile (string_input);
    int i = 0;
    if (myfile.is_open()) //FOR THE EMPTY LINES IT GIVES ACCEPT
    {
        while (getline (myfile, line))
        {
            ++i;
            dpda.writing_and_operations(line,output_file);
            if (line.length() == 0) {
            }
        }
    }
}



Automata::Automata(const string &given_file, string output_file_name) { //FUNCTION FOR THE Automata FILE
    ifstream ifstream;
    ifstream.open(given_file);
    string line;

    // ################################## states ##################################
    getline(ifstream, line);
    if(line.back() == '\r'){
        line.pop_back();
    }
    line = splitter(line,":");

    string s = line;
    string delimiter = " => ";
    auto start = 0U;
    auto end = s.find(delimiter);
    while (end == string::npos)
    {

        start = end + delimiter.length();
        end = s.find(delimiter, start);
    }
    string line2 = s.substr(start,end);

    istringstream ssLine4(line2);
    while(ssLine4) {
        string a;
        if(!getline(ssLine4,a,',')) {
            break;
        }

        states.insert(a);
    }

    //################################## start state #############################################

    string line3 = splitter(line," => ");

    istringstream ss(line3);
    string token;
    getline(ss, token, ',');
    int i = 0;
    while(i < token.size())
    {
        if(token[i] == '(' || token[i] == ')')
        {
            token.erase(i,1);
        }
        i++;
    }

    initial_state = token;
    string line4 = eraseDemo(line3);

    const string unwanted = "[]" ;
    string line5  ;
    for( char c : line4 ) if( unwanted.find(c) == string::npos ) line5 += c ;
    istringstream ssLine6(line5);
    while(ssLine6) {
        string a;
        if (!getline(ssLine6,a,',')){
            break;
        }

        agreed_states.insert(a);
    }
    // ################################## input SYMBOLS ############################
    getline(ifstream, line);
    if(line.back() == '\r'){
        line.pop_back();
    }

    line = splitter(line,":");


    istringstream ssLine2(line);
    while(ssLine2) {
        string a;
        if(!getline(ssLine2,a,',')) {
            break;
        }

        our_alphabet.insert(a[0]);
    }

    //################################## stack alphabett #####################################
    getline(ifstream, line);
    if(line.back() == '\r'){
        line.pop_back();
    }

    line = splitter(line,":");

    istringstream ssLine3(line);
    while(ssLine3) {
        string a;
        if(!getline(ssLine3,a,',')) {
            break;
        }
        our_stack_alp.insert(a[0]);
    }


    // ################################## transitions ###################
    while(ifstream) {
        if(!getline(ifstream,line)) {
            break;
        } else {
            if(line.back() == '\r') {
                line.pop_back();
            }
            line = splitter(line,":");
            istringstream ssTransition(line);
            string First_State_Name, Input_Sym, Stack_Up, Latest_State, Latest_Stack_Up;
            getline(ssTransition, First_State_Name, ',');
            getline(ssTransition, Input_Sym, ',');
            getline(ssTransition, Stack_Up, ','); //will be ,
            getline(ssTransition, Latest_State, ',');
            getline(ssTransition, Latest_Stack_Up);
            //################################## CHECKING FOR THE RIGHT INPUT OR NOT
            if( Input_Sym[0] != 'e' && !our_alphabet.count(Input_Sym[0]) || Stack_Up[0] != 'e' && !our_stack_alp.count(Stack_Up[0]) || !states.count(First_State_Name) || !states.count(Latest_State) || Latest_Stack_Up[0] != 'e' && !our_stack_alp.count(Latest_Stack_Up[0]) ){
                ofstream writer2;
                writer2.open(output_file_name);
                writer2<<"Error [1]:DPDA description is invalid!"<<endl;
                exit(3);
            }
            myDpda_t r = {First_State_Name, Input_Sym[0], Stack_Up[0], Latest_State, Latest_Stack_Up[0]};
            transitions.push_back(r);
        }
    }
    ifstream.close();
    present_state = initial_state;
}

void Automata::writing_and_operations(string strr,const string& given_file) {

    ofstream writer;
    writer.open(given_file,ios_base::app );
    // START STATE TO CURRENT STATE
    present_state = initial_state;
    stack.clear();
    int a = 0;
    while(a < strr.size())
    {
        if(strr[a] == ',' )
        {
            strr.erase(a,1);
        }
        a++;
    }
    bool checker;
    int i = 0;
    //X WILL BE THE INPUT STRINGS
    while( i < (int)strr.size() ) {
        int myDpda;
        checker = false;
        for (myDpda = 0; myDpda < (int)transitions.size(); myDpda++) {
            if (present_state == transitions[myDpda].state_first && ( strr[i] == transitions[myDpda].input_char || transitions[myDpda].input_char == 'e' )){
                checker = true;
                break;
            }
        }
        if (!checker){
            writer << "REJECT" << endl << endl;
            return;
        }

        // ReadING INPUT SYMBOL AND THEN GOING OTHER INPUT
        if (transitions[myDpda].input_char != 'e' ) {
            i++;
        }

        if (transitions[myDpda].top_of_stack != 'e') {
            if ( !stack.empty() ) {
                stack.pop_back();
            }
        }

        present_state = transitions[myDpda].latest_stat;
        // PUSHING THE VALUE IN TRANSITOTNS LAST ELEMENT TO STACK
        if (transitions[myDpda].latest_stack_up != 'e'  ) {
            stack.push_back(transitions[myDpda].latest_stack_up);

        }
        //PRINTING OPERATIONS
        writer  << transitions[myDpda].state_first << ","
                << transitions[myDpda].input_char  << ","
                << transitions[myDpda].top_of_stack     << " => "
                << transitions[myDpda].latest_stat     << ","
                <<transitions[myDpda].latest_stack_up  << " [STACK]:";
        string delim = "";
		if (stack.empty()  ) {
            
            writer << "\n";
            writer
                    << transitions[myDpda].state_first << ","
                    << transitions[myDpda].input_char << ","
                    << transitions[myDpda].top_of_stack << " => "
                    << transitions[myDpda].latest_stat << ","
                    << transitions[myDpda].latest_stack_up << " [STACK]:";
        }
        //FOR THE COMMA IN OUR PROGRAM
            for (auto i : stack) {
                writer << delim << i;
                delim = ",";
            }
        
		
        writer << endl;
        if(transitions[myDpda].latest_stack_up == 'e' && stack.size() == 1 && stack.front() == '$'){
            myDpda++;
            writer
                    << transitions[myDpda].state_first << ","
                    << transitions[myDpda].input_char << ","
                    << transitions[myDpda].top_of_stack << " => "
                    << transitions[myDpda].latest_stat << ","
                    << transitions[myDpda].latest_stack_up << " [STACK]:$"
                    << "\n";
        }
    }
	
    if (stack.empty() || (stack.front()=='$'&& stack.size()==1) || agreed_states.find(present_state) != agreed_states.end() || stack.size()==0  ) {
        writer << "ACCEPT" << endl << endl;
    }
    else {
        writer << "REJECT" << endl << endl;
    }
    writer.close();
}
