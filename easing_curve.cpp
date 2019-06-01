
// Easing Curves Calculator
// Lawrence France
// Started 30/03/19

/* Description

	Output easing curve data given curve parameters and input data from user

	Input: 	Desired easing curve and associated parameters
				These may be:
					Linear
					InQuad
					OutQuad
					InOutQuad
					(information at http://doc.qt.io/qt-5/qeasingcurve.html#Type-enum)
			Time value at which easing curve should be evaluated

			Program deals with malformed inputs, e.g. incomplete variables
			Also performs validation checks, e.g. non-number inputs
			These are discarded until user enters valid input
			

	Output:		Output easing curve details back to user
				Value of easing curve at user given time value 

	Calculation details:

		Using notation:
			diff 	= x(duration) - x(0)

		Example easing curve parameters:
			x(0)		= 100
			x(duration)	= 200
			d 			= 1.0
			diff 		= 100

	
		In general, Output = x(0) + diff * (t/d)^n
												^ "easing factor"


		For linear, n=1:
			f(t) = x(0) + diff * t/d
		Example inputs and their outputs:
				t = 0.2 --> f(t) = 120
				t = 0.5 --> f(t) = 150
				t = 1.0 --> f(t) = 200

		For InQuad, n=2:
			f(t) = x(0) + diff * (t/d)^2
		Example inputs and their outputs:
				t = 0.2 --> f(t) = 104
				t = 0.5 --> f(t) = 125
				t = 1.0 --> f(t) = 200

		For OutQuad, n=2 with an inversion:
			f(t) = x(duration) - diff * (1 - (1 - t/d)^2)
		Example inputs and their outputs:
				t = 0.2 --> f(t) = 136
				t = 0.5 --> f(t) = 175
				t = 1.0 --> f(t) = 200

		For InOutQuad:

			if t between 0<->d/2, use InQuad, e.g.  x(0) = 100, x(duration) = 150 :
				t = 0.2 --> f(t) = 108
				t = 0.4 --> f(t) = 132

			if t between d/2<->d, use OutQuad, e.g. x(0) = 150, x(duration) = 200 :
				t = 0.7 --> f(t) = 168
				t = 1.0 --> f(t) = 200

			if t == d/2, we know f(0.5) = x(0) + diff/2 = 150 :
				f(t) = x(0) + diff / 2

*/


#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <sstream>


using namespace std;


/* GLOBAL FUNCTION DEFINITIONS */

bool ValidDbl(string& s) {

	/*	Purpose: Check string input can be converted to a valid dbl
				 Throw exception if it cannot
				 Return true if string can be converted, false if not

		Method:	 stod method				 
	*/
	
	double TestDouble = 0.0; 

	try {	// Is a number
		TestDouble = stod(s);			
		return true;
	}
	catch (...) {	// Is not a number
		return false;
	}
}
bool ValidInt(string& s) {

	/*	Purpose: Check string input can be converted to a valid int
				 Throw exception if it cannot
				 Return true if string can be converted, false if not

		Method:	 ceil-floor check method
				 stringstream method not suitable
				 stoi method not suitable as double will be converted to int
	*/

	try {		
		
		if ( abs(ceil(stod(s)) - floor(stod(s))) < 0.0001 ) { // Integer			
			return true;
		}
		else {		// Not an integer			
			return false;
		}
	}
	catch (...) {	// Not a number		
		return false;
	}

	
}
bool ValidInput(vector<string>& v) {

	/*	Purpose: Check strings input by user are valid 				 

		Method:	 Check:
					There are 4 elements.
					EC choice is one of the available ECs.
					Lower and Upper bounds are not negative integers.
					Upper > Lower.
					Duration is a positive double.
				 Throw exception any conditions are not met.
				 Return true if input is valid, false if not.

				 Not a method of EC class as do not want to pass
				 invalid inputs to object storage.
	*/

	
	// Check there are 4 elements
	if (v.size() != 4) {		
		cout << "There are not four valid elements. Please try again:" << endl;
		return false;
	}
	
	// Check string is valid
	if (
			v[0] != "Linear" 
		&&	v[0] != "InQuad" 
		&&	v[0] != "OutQuad" 
		&&	v[0] != "InOutQuad"
		//  Further ECs here
	) {
		cout <<		"Curve type is invalid, must be 'Linear',"
					"'InQuad', 'OutQuad' or 'InOutQuad'\n"
					". Please try again:" 
		<< endl;

		return false;
	}
	
	// Check integers are integers and Upper>Lower
	//		Call ValidInt function
	//		Check Upper>Lower
	if (!ValidInt(v[1])) {
		cout << "Lower bound is not an integer. Please try again:" << endl;
		return false;
	}
	else if (stoi(v[1]) < 0) {
		cout << "Lower bound must not be negative. Please try again:" << endl;
		return false;
	}
	else if (!ValidInt(v[2])) {
		cout << "Upper bound is not an integer. Please try again:" << endl;
		return false;
	}
	else if (stoi(v[2]) < 0) {
		cout << "Upper bound must not be negative. Please try again:" << endl;
		return false;
	}
	else if (stoi(v[2]) <= stoi(v[1])) {	// Check Upper>Lower
		cout << "Upper bound must be greater than Lower bound. Please try again:" << endl;
		return false;
	}
	
	// Check double is a double
	//		Call ValidDbl function
	if (!ValidDbl(v[3])) {
		cout << "Duration value is not a double. Please try again:" << endl;
		return false;
	}
	else if (stoi(v[3]) <= 0) {
		cout << "Duration must be greater than 0. Please try again:" << endl;
		return false;
	}

	

	return true;

}

vector<string> UserInputEC() {

	/*	Purpose: Take user input for EC parameters

		Method:	User input collected into a string using getline.
				This is then delimited "," by getline.
				The first entry is pushed into a results vector.
				The resulting 3 strings are processed character-by-character
				according to:
					Characters considered: those following an "="
					Other characters are discarded.					
				The user's input is repeated back to them.
				Returns vector of processed strings ready for SetValues function.

				original input			"Linear,x_0=100,x_max=200,dur=1.0"
				processed characters	 		0000111	000000111 0000111
				output vector		   ["Linear",  "100",    "200",  "1.0"]
		
					
				Invalid input examples:
					e.g.				
						
						original input			"Linr,x_0=-100,x_max=-200,dur=-1.0"							
						processed characters		  00001111 0000001111 00001111
					
						There are several problems with this input:
							EC curve type is misspelled.
							Lower is negative.
							Upper is negative and Upper < Lower.
							dur is negative.
						The validation function ValidInput will catch all of these
						invalidities and report the first it encounters to the user.


				Malformed input examples:
					e.g.				"Linear,x_0=100,x_max=200,dur=1.0,a"
		
						The user has entered an extra comma and character.
						This function will then process this as an extra entry by
						appending the result vector with an (empty) extra string.
						The ValidInput function will then determine this to be an invalid
						input and prompt the user to enter it again.
		
					e.g.				"Linear,x_0=100,x_max=200,dur=1.0,dur=1.0"
		
						The user has duplicated the final entry.
						This function will then process this extra entry and
						produce a vector with 5 elements.
						The ValidInput function will then determine this to be an invalid
						input and prompt the user to enter it again.

				 
				Not a method of EC class as it is not directly related
				to the class members
	*/

	cout << "Please enter Easing Curve details, in the following format:\nLinear,x_t0=100,x_tmax=200,duration=1\n" << endl;

	bool InputCheck = false;
	while (!InputCheck) {

		string UserInput;						// String to contain user input line
		vector<string> Inputs;					// Vector of strings to contain processed inputs

		cin.clear();
		getline(cin, UserInput);				// Setup getline method
		istringstream iss(UserInput);			
		string entry; vector<string> entries;	// Temporary variables to hold each entry
												// and split entries

		while (getline(iss, entry, ',')) {		// Parse for ","					
			entries.push_back(entry);			// Push string into vector	
		}
				
		Inputs.push_back(entries[0]);			// Retrieve EC type

		for (unsigned int i = 1; i < entries.size(); i++) { // Iterate through each entry string, for i>0
			
			string temp = ""; bool EqualsFound = false;
			for (int j = 0; j < entries[i].size(); j++) { // Iterate through each character of entry string
				
				string s(1, entries[i][j]);		// Convert char to to string

				if (s == "=") {					// Only consider characters after an equals has been found
					EqualsFound = true; 
				}	
				else if (EqualsFound) {			// Append result string with character after "="
					temp.push_back(entries[i][j]); 
				}				
			}
			Inputs.push_back(temp);	// Push result string into result vector of strings, our processed input			
		}
		
		/*for (int i = 0; i < Inputs.size(); i++) {
			cout << Inputs[i] << "\t";
		}*/

		if (ValidInput(Inputs)) {		// Validate inputs
			cout << UserInput << endl;	// Repeat user's input back to them
			return Inputs;
			InputCheck = true;
		}
	}
}


/*  CLASS TO CONTAIN EASING CURVE DETAILS  */
/*	OOP was chosen as many similar parameters are created
	replaced sequentially, and the purpose of the task 
	relates to them only. */

class EasingCurve {
	
	// Private Variables, curve parameters obtained from user input
private:
	int lower, upper, diff, mid, order;	
	double dur, factor;
	string curve_type;

	

	
	// Public 
public:	

	int result; double time;
		// Class function declarations

	
	void SetValues(vector<string>&); // Pass user input variables to object

	double UserInputTime();	// Retrieve user input for time

	int Calculate();		// Calculate easing factors ready for results

	void Linear();			// Calcualte easing curve result for Linear EC
	void InQuad();			// Calcualte easing curve result for Inward Quadratic EC
	void OutQuad();			// Calculate easing curve result for Outward Quadratic EC
	void InOutQuad();		// Calcualte easing curve result for Inward-Outward Quadratic EC

};


/* CLASS FUNCTION DEFINITIONS */



void	EasingCurve::SetValues(vector<string>& v) {

	/*	Purpose: Take user input and pass the data to the class above.
				 Performs basic operations as well as assigning the
				 order of the curve to be used later.
	*/

	curve_type	= v[0];
	lower		= stoi(v[1]);
	upper		= stoi(v[2]);
	dur			= stod(v[3]);

	diff = upper - lower;
	mid  = lower + diff / 2;	
}

double	EasingCurve::UserInputTime() {

	/*	Purpose: Check time value input by user is valid

		Method:	 Use getline method
				 Check:
					Input is a number
					Is 0 <= time <= duration
				 Throw exception any conditions are not met
				 Return true if input is valid, false if not

				 Method of EC class because comparison with duration is required				 
	*/
	
	//cout << "Please enter time values to evaluate against the Easing Curve:" << endl;

	bool InputCheck = false;
	while (!InputCheck) {

		cin.clear();
		string UserTime;
		getline(cin, UserTime);

		if (!ValidDbl(UserTime)) {
			cout << "Time entered must be a number" << endl;
		}
		else if (stod(UserTime) < 0 || stod(UserTime) > dur) {
			cout << "Time must be between 0 and " << dur << endl;
		}
		else {
			return stod(UserTime);
		}
	}
}

int		EasingCurve::Calculate() {

	/*	Purpose: Call EC functions according to which has been chosen by the user

		Method:	 Check curve_type member
				 Call EC function corresponding to this value

		Extensibility: Further ECs can be added if needed in future
	*/

	
	if (curve_type == "Linear") {
		Linear();
	}
	else if (curve_type == "InQuad") {
		InQuad();
	}
	else if (curve_type == "OutQuad") {
		OutQuad();
	}
	else if (curve_type == "InOutQuad") {
		InOutQuad();
	}
	/*	else if (*new curve type*) {

			// FURTHER EASING CURVES MAY GO HERE IN FUTURE

		}
	*/	

	return result;
}

void	EasingCurve::Linear() {

	/*	Purpose: Use the Linear easing curve 

		Method: Use f(t) = x(0) + diff * t/d
	*/

	order = 1;
	double temp = lower + diff * pow(time / dur, order);

	string StringInt(4, temp);
	if (ValidInt(StringInt)) {
		int temp2 = static_cast<int>(temp);
		result = temp;
	}
	else {
		result = static_cast<int>(temp);
		cout << "" << endl;
	}


}
void	EasingCurve::InQuad() {

	/*	Purpose: Use the Inward Quadratic easing curve

		Method: Use f(t) = x(0) + diff * (t/d)^2
	*/
	
	order = 2;
	result = lower + diff * pow(time / dur, order);
}
void	EasingCurve::OutQuad() {

	/*	Purpose: Use the Outward Quadratic easing curve

		Method: f(t) = x(duration) - diff * (1 - t/d)^2
	*/
	 
	order = 2;
	result = upper - diff * pow(1 - time / dur, order);
}
void	EasingCurve::InOutQuad() {

	/*	Purpose: Use the Inward-Outward Quadratic easing curve

		Method:	 if t between 0<->d/2, use InQuad with upper = mid
				 if t between d/2<->d, use OutQuad with lower = mid
				 if t == d/2, we know f(t) = mid
	*/

	diff = mid - lower;
	order = 2;

	if (time <= dur / 2) {		// InOutQuad, left segment 
		result = lower + diff * pow(2 * time / dur, order);
	}
	else if (time >= dur / 2) {	// InOutQuad, right segment
		result = upper - diff * pow(1 - (time - dur / 2) / (dur*dur / 2), order);
	}
	else {						// InOutQuad, midpoint case
		result = mid;
	}
}


/* MAIN PROGRAM */

int main()
{    
	/*	Purpose: To outline the functionality of the above class and associated functions
				 for easing functions calculations.

		Method:	 Take user input.
				 Determine what easing curve is to be used.
				 Evaluate easing curve values for subsequent progress values.
				 Output results to console.

		Input:	 This supports two user inputs:
					Easing Curve details:
						Linear,x_t0=100,x_tmax=200,duration=1
					Time values for evaluation:
						0.2
						0.5
						1.0
		
		Output:	 The program is specified to repeat the curve details to the user when 
				 they are input successfully:
						Linear,x_t0=100,x_tmax=200,duration=1
				 And report subsequent calculation values to the user:
						120
						150
						200	
				 Further examples at top of this program.
	
	*/
	
	/* USER INPUT */	
	
	// Take user input, process and validate inputs
	vector<string> UserInputs = UserInputEC();
		
	string Curve	= UserInputs[0];
	int Lower		= stoi(UserInputs[1]);
	int Upper		= stoi(UserInputs[2]);
	double Time		= stod(UserInputs[3]);	
	
	// Store input in object
	EasingCurve EC;	
	/*EC.SetValues(Lower, Upper, UserInputs[0], Time);*/
	EC.SetValues(UserInputs);
	

	/* CALCULATION */

	//Repeat calculation as many times as user desires
	bool CalcsDone = false;
	while (!CalcsDone) {

		
		// Take user input for time
		EC.time = EC.UserInputTime();


		/* OUTPUT */
		//cout << "f(t = " << EC.time << ") = " << EC.Calculate() << endl;
		cout << EC.Calculate() << endl;
	}

	return 0;
}

