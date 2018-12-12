#include <iostream>
#include <fstream>
#include <istream>
#include <ostream>
#include <string>
#include <cstring>
#include <dirent.h>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <sstream> 
using namespace std;

//FIXME - If no file path is set inside of the NoterwFilePath, Alert the user to somehow set a filepath before using the program
//FIXME - When a file is being read, it is duplicating the last letter in the file. Not totally sure why

//12/1/18 - The note path isn't setting correctly for some reason. It works sometimes but other times it doesn't. Not really sure why. Go to list notes to see what you can do. Some how the directory 
//string is getting messed up which keeps the file from being read. Occassionally it has an extra s. Maybe you need to clear the input buffer at some point in the code

char const FILEPATH_DOCUMENT[40] = "/usr/lib/Noterw/NoterwFilePath"; //holds the name of the filePath text file name constant

string GetFilePath();
void LaunchMainMenu(string* notes_FilePath_Poiner);
void ReadNote(bool fromMainMenu, char* argv = 0); //Opens the text file sent in as an argument
void SetFilePath(bool fromMainMenu, int& userInput, string* notes_FilePath_Pointer); //Sets the path for text file (note) retrieval.
void ListNotes(bool fromMainMenu, string* notes_FilePath_Pointer);
void WriteNote(bool fromMainMenu, string* notes_FilePath_Pointer);
void DisplayHelpScreen(bool fromMainMenu); //Displays the help screen 

int main(int argc, char* argv[]) {

	bool modifier = false; //Tells whether the command line input contains a modifier
	char fpOption[4] = "-fp"; //Used to set the filepath; Leave one space for the null terminator
	char filePathOption[11] = "--filePath"; //Same as fp but in extended form
	char lnOption[4] = "-ln"; //Used to list the notes in the filePath file
	char listNotesOption[12] = "--listNotes";
	char hOption[3] = "-h";
	char helpOption[7] = "--help";
	int exitVariable = 6; //Holds the command value to make the program exit after completing a command using an option
	string notes_FilePath = ""; //Holds the string representation of the filePath set by the user where Notes are located

	notes_FilePath = GetFilePath();
	string* notes_FilePath_Pointer = &notes_FilePath;

	if (argc > 1) {
		for (int i = 0; i < argc; ++i) {
			if (strcmp(fpOption, argv[i]) == 0 || strcmp(filePathOption, argv[i]) == 0) { //You are comparing to cstrings here, not regular string variables. NOTE -> RETURNS 0 IF EQUAL NOT 1
				SetFilePath(false, exitVariable, notes_FilePath_Pointer); //Send in a 6 to make the program exit automatically after execution
				modifier = true;
			}
			else if (strcmp(lnOption, argv[i]) == 0 || strcmp(listNotesOption, argv[i]) == 0) {
				ListNotes(false, notes_FilePath_Pointer);
				modifier = true;
			}
			else if (strcmp(hOption, argv[i]) == 0 || strcmp(helpOption, argv[i]) == 0) {
				DisplayHelpScreen(false);
				modifier = true;
			}
		}
	} else if (argc <= 1) { //If no modifiers or files are input, launch the main menu
		LaunchMainMenu(notes_FilePath_Pointer);
	}

	
	if (!modifier && argc > 1) { //If there is no modifier and there is another argument (a notepad file)
		ReadNote(false, argv[1]);
	}

	return 0;
}

string GetFilePath(){

	string nfp = "";
	char curChar = ' '; //holds the current char being read from the file. MUST use a char instead of string or will prevent noskipws from working correctly
	ifstream instream(FILEPATH_DOCUMENT);

	if (!instream) {
		cout << endl << "ERROR 1: UNABLE TO FIND FILEPATH" << endl;
		cout << endl << "Please make sure the file path is set correctly. ";
		cout << endl << "If this is your first time using the program, make sure to set a file path." << endl;
		cout << "If errors persist, please use the '--filePath' modifier to set the read path." << endl;
		cout.flush(); //Don't forget to flush!
		exit(1); //Exits the program
	}

	//FIXME - .eof() method checks whether the end of a file is reached based on the LAST operation, not based on the next. This method is causing the duplication of the last letter in a 
	//filePath. It is corrected in the loop below, but not sure if this patch is the best approach

	while (!instream.eof()) { //FIXME - pre-emptively prevent the stream from loading in the 'newline' character
		instream >> noskipws >> curChar;
		nfp = nfp + curChar;
	}

	while (nfp[nfp.length()-1] == 10 || nfp[nfp.length()-1] == 32 || nfp[nfp.length() -1] == nfp[nfp.length() - 2]){ //10 represents the 'new line' character. Some how it is getting into the string. 																 //Probably from the while loop directly above.
									//32 is asc 2 value for space
		nfp.erase(nfp.begin() + (nfp.length() - 1), nfp.begin() + nfp.length()); 
	}

	instream.close(); //Close the filestream

	return nfp;
}

void LaunchMainMenu(string* notes_FilePath_Pointer){
	
	int userInput = 0; //Holds the user's selection

	do {
		cout << endl << "*********" << endl << "MAIN MENU" << endl << "*********" << endl;
		cout << endl << "1) Set File Path";
		cout << endl << "2) List Notes";
		cout << endl << "3) Write new note";
		cout << endl << "4) Read note";
		cout << endl << "5) Help";
		cout << endl << "6) Exit";
		cout << endl;
		
		cout << endl << "What would you like to do?: "; cin >> userInput;

		switch (userInput){
			case 1: SetFilePath(true, userInput, notes_FilePath_Pointer); break;
			case 2: ListNotes(true, notes_FilePath_Pointer); break;
			case 3: WriteNote(true, notes_FilePath_Pointer); break;
			case 4: ReadNote(true); break;
			case 5: DisplayHelpScreen(true); break;
			case 6: break;
			default: cout << endl << "I'm sorry, I didn't get that." << endl; break;
		} 
		
	} while (userInput != 6);

	return;
}

void ReadNote(bool fromMainMenu, char* argv) { //Note - Default implementation only needs to be stated in the function declaration

	string nfp = GetFilePath();
	char curChar = ' '; //holds the current char being read from the file. MUST use a char instead of string or will prevent noskipws from working correctly
	string fileName = " ";

	if (fromMainMenu){ 
		cout << endl << "Please type the name of the file you would like to read: ";
		cin.ignore(1);
		getline(cin, fileName);
	}
	
	else {
		ostringstream oss; //Create an output string stream to write to
		oss << argv; //Write the filename to the output stream
		fileName = oss.str(); //Copy it into a string variable
	}

	ifstream instream(nfp + "/" + fileName);

	if (!instream) {
		cout << endl << "ERROR 2: UNABLE TO OPEN FILE " << endl;
		cout << endl << "Troubleshooting step 1: Make sure the name of the file is spelled correctly.";
		cout << endl << "Troubleshooting step 2: Ensure the file is located inside the folder set by the File Path. " << endl;
		cout << endl << "If errors persist, please use '--help' modifier to investigate further." << endl;
		cout.flush(); //Don't forget to flush!
		exit(1); //Exits the program
	}

	cout << endl; //New line just for cleaninless and readability;

	while (!instream.eof()) {
		instream >> noskipws >> curChar;
		cout << curChar;
	}

	cout << endl; //New line just for cleanliness and readability;

	return;
}

void SetFilePath(bool fromMainMenu, int& userInput, string* notes_FilePath_Pointer) {

/* If the user navigates to this method from the main menu, the boolean fromMainMenu will be equal to true. This 
 * tells the method to allow the user to navigate back. This functionality allows the user to use the -fp option
 * and have the program instantly exit when the option is applied rather than taking the user back to the main menu  
 */

	if (fromMainMenu) {cin.ignore(1);} 	//You have to ignore one value because the cin is registering an input when 'enter' is pressed
						//to execute the command that navigates to the SetFilePath() function from the main menu.
						//This navigation causes the buffer to have an '\n' in it by the time it arrives to getline()
	
	cout << endl << "Please enter a file path: ";

	string nfp = " "; //File path input by user
	ofstream outStream(FILEPATH_DOCUMENT);
	
	getline(cin, nfp); //Get user input filepath
	outStream << nfp;
	outStream.close(); //Make sure you close the stream!

	*notes_FilePath_Pointer = GetFilePath();

	cout << endl << "File Path set to: " << nfp << endl;
	
	return;
}

void ListNotes(bool fromMainMenu, string* notes_FilePath_Pointer){

	cout << endl; //Print a new line for astetics
	string nfp = *notes_FilePath_Pointer;
	DIR* directory; //pointer to a function that opens a directory: DIR *opendir(const char *dirname)
	struct dirent* entry; //returns a pointer to a structure representing the directory entry at the current position in teh directory stream. struct dirent *readdir (DIR *dirp)

	while (nfp.back() == '\n'){ 
		//FIXME - For some reason there is are extra '\n' characters at the end of the string when it is passed into the function. It may come from earlier code I'm not 				sure. This removes it. I think it comes from converting the string to a c-string which is tacking on nulls at the end of the string. 
		nfp.pop_back(); 
	}

	//cout << nfp;

	directory = opendir (nfp.c_str());

	if (directory != NULL){
	
		vector<string> files;

		while ((entry = readdir(directory)) != NULL){ //While the entry being read from the directory is not null. readdir = read directory
			
			string fileName = entry->d_name;
			files.push_back(fileName);			
		}

		sort(files.begin(), files.end());
		closedir(directory);
		for (int i = 0; i< files.size(); ++i){
			cout << files.at(i) << endl;
		}
	} else {
		//FIXME - Add an error statement
	}
	
	return;
}

void WriteNote(bool fromMainMenu, string* notes_FilePath_Pointer){
	
	//This will open the command line text editor "nano". I assume it will be stored in the same location on every file system, though there is a chance that it won't be.
	//FIXME - If for some reason the user has moved the location of the nano file, it will need to send an error if no nano file is found

	std::system("/bin/nano");
	return;
}

void DisplayHelpScreen(bool fromMainMenu) {

	cout << endl << "DESCRIPTION" << endl;
	cout << endl << "Prints the contents of a text file into the command line to allow easy access to personal notes " << endl;
	cout << endl << "USAGE" << endl;
	cout << endl << "Command [OPTION] Filename" << endl;
	cout << endl << "OPTIONS" << endl;
	cout << endl << "-fp, --filePath " << endl << "     " << "set the location (directory) where personal notes are stored" << endl;
	cout << endl << "-ln, --listNotes " << endl << "     " << "list the notes located in the directory" << endl;
	cout << endl << "-h, --help " << endl << "     " << "display this help and exit" << endl;

	return;
}
