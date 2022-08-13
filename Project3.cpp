#include <Python.h>
#include <iostream>
#include <Windows.h>
#include <cmath>
#include <string>
#include <fstream>

using namespace std;

/* Function links to and calls desired function in Python code
@Parameter: String (in quotes) corresponding to the desired function in Python
@Returns to C++ from the Python function a list with items counted */
void CallProcedure(string pName){
	char* procname = new char[pName.length() + 1];
	std::strcpy(procname, pName.c_str());

	Py_Initialize();
	PyObject* my_module = PyImport_ImportModule("PythonCode");
	PyErr_Print();
	PyObject* my_function = PyObject_GetAttrString(my_module, procname);
	PyObject* my_result = PyObject_CallObject(my_function, NULL);
	Py_Finalize();

	delete[] procname;
}

/* Function links to and calls desired function in Python code
@Parameter: String (in quotes) corresponding to the desired function in Python, and another string such as item (to search for)
@Returns the frequency of specified item to C++ */
int callIntFunc(string proc, string param){
	char* procname = new char[proc.length() + 1];
	std::strcpy(procname, proc.c_str());

	char* paramval = new char[param.length() + 1];
	std::strcpy(paramval, param.c_str());


	PyObject* pName, * pModule, * pDict, * pFunc, * pValue = nullptr, * presult = nullptr;
	// Initialize the Python Interpreter
	Py_Initialize();
	// Build the name object
	pName = PyUnicode_FromString((char*)"PythonCode");
	// Load the module object
	pModule = PyImport_Import(pName);
	// pDict is a borrowed reference 
	pDict = PyModule_GetDict(pModule);
	// pFunc is also a borrowed reference 
	pFunc = PyDict_GetItemString(pDict, procname);
	if (PyCallable_Check(pFunc)){
		pValue = Py_BuildValue("(z)", paramval);
		PyErr_Print();
		presult = PyObject_CallObject(pFunc, pValue);
		PyErr_Print();
	}
	else{
		PyErr_Print();
	}
	//printf("Result is %d\n", _PyLong_AsInt(presult));
	Py_DECREF(pValue);
	// Clean up
	Py_DECREF(pModule);
	Py_DECREF(pName);
	// Finish the Python Interpreter
	Py_Finalize();

	// clean 
	delete[] procname;
	delete[] paramval;


	return _PyLong_AsInt(presult);
}

/*Function prints menu for user
@no parameters
@returns no values, displays user menu on screen */
void menuDisplay() {

	cout << endl;
	cout << "        Please select an option:" << endl;
	cout << " 1 - Display all items and sales data" << endl;
	cout << " 2 - Display specific item sales data" << endl;
	cout << " 3 - All items with histogram of sales data" << endl;
	cout << " 4 - Exit" << endl;
	cout << " Please enter selection: ";
}

/* main program
@no parameters
@no return, calls other functions and displays output to user */
int main(){
	const string menuChoice1 = "1";
	const string menuChoice2 = "2";
	const string menuChoice3 = "3";
	const string menuChoice4 = "4";
	string selection = "0";
	string userWord;
	string item;
	string itemName;
	int count;
	int i;
	ifstream fileInput;

	menuDisplay();
	cin >> selection;
	cout << endl;

	while (selection != menuChoice4) {
		if (selection == menuChoice1) {
			system("Color 06"); //changes text color
			CallProcedure("determineInventory"); //uses function in C++ to call "determineInventory" in Python code
		}
		else if (selection == menuChoice2) {
			cout << "Type item to find: ";
			cin >> item;

			//Change item entered to have uppercase first letter and rest of word lowercase
			if (!item.empty()) {
				item[0] = std::toupper(item[0]);
				for (std::size_t i = 1; i < item.length(); ++i) {
					item[i] = std::tolower(item[i]);
				}
			}
			system("Color 05"); //changes text color

			//uses function in C++ to call function in Python code
			int count = callIntFunc("determineFrequency", item);
			if (count >= 0) {
				cout << item << ": " << count;
				cout << endl;
			}
			else {
				cout << "Item does not exist" << endl; //prints if user item is not in sales data list
			}
		}
		else if (selection == menuChoice3) {
			CallProcedure("output"); //Calls function in C++ to call function in Python that writes to a data file
			fileInput.open("frequency.dat"); //opens data file
			fileInput >> itemName >> count;

			while (!fileInput.fail()) { //continues through each line of the file until "fail" or end of file
				system("Color 0F"); //changes output text color
				cout << itemName;
				for (i = 0; i < count; i++) { //loops through data file and replaces number of times item sold with stars for histogram
					system("Color 02"); //changes output text color
					cout << "*";
				}
				cout << endl;
				fileInput >> itemName >> count;
				continue;
			}
			fileInput.close(); //close data file when selection has finished histogram
		}
		else { //validation of user menu selection, requests user to enter a valid choice
			system("Color 04"); //changes output text color
			cout << "-------------------------------------------------" << endl;
			cout << "*** Please enter a valid menu option 1 - 4!!! ***" << endl;
			cout << "-------------------------------------------------" << endl;
		}
		menuDisplay();
		cin >> selection;
		cout << endl;
		system("Color 0F"); //changes output text color

		if (selection == menuChoice4) { //user selection closes program
			cout << "Have a great day!!! Goodbye!" << endl;
			system("pause");
		}
		continue;
		return 0;
	}
}