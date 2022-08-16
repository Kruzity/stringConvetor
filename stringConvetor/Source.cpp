#define _USE_MATH_DEFINES
#include<iostream>
#include<iomanip>
#include<string>
using namespace std;

float radToDeg(float rad)
{
	return rad * 180 / M_PI;
}

string functionOperationsConvert(string bracketExpression, string operation)
{
	string sAnswer;
	if (operation == "sqrt")
	{
		if (bracketExpression[0] == '-') sAnswer = "#error, negative number under the root";
		else sAnswer = to_string(sqrt(stof(bracketExpression)));
	}
	else if (operation == "cos") sAnswer = to_string(cos(radToDeg(stof(bracketExpression))));
	else if (operation == "sin") sAnswer = to_string(sin(radToDeg(stof(bracketExpression))));
	else if (operation == "tan") sAnswer = to_string(tan(radToDeg(stof(bracketExpression))));
	return sAnswer;
}

float arithmeticOperationsConvert(float leftNumber, float rightNumber, char operation)
{
	switch (operation)
	{
	case '+': return leftNumber + rightNumber;
	case '-': return leftNumber - rightNumber;
	case '*': return leftNumber * rightNumber;
	case '/': return leftNumber / rightNumber;
	}
}

string convertStringToInt(string something, float x)
{
	//cout << something << endl;
	string functionOperations[]{ "sqrt","pow","cos","sin","tan" };
	char arithmeticOperations[]{ '(','*','/','+','-'};
	
	for (size_t xPos = something.find('x'); xPos != string::npos; xPos = something.find('x'))
		something.replace(xPos, 1, to_string(x));
		
	
	
	//functionOperations
	for (string& operation : functionOperations)
	{
		for (size_t xPos = something.find(operation); xPos != string::npos; xPos = something.find(operation))
		{
			size_t leftBracket = something.find('(', xPos), rightBracket = something.find(')', xPos);
			for (int i = leftBracket + 1; i != rightBracket; i++)
			{
				if (something[i] == '(' && i < rightBracket) rightBracket = something.find(')', rightBracket + 1);
			}

			if (operation == "pow") rightBracket = something.rfind(',', rightBracket);

			string bracketExpression(something, leftBracket+1, rightBracket - leftBracket-1);

			bool isFindSubOperation = false;
			for (string& subOperation : functionOperations)
			{
				if (bracketExpression.find(subOperation)!=string::npos)
				{
					bracketExpression = convertStringToInt(bracketExpression, x);
					something.replace(leftBracket+1, rightBracket - leftBracket - 1,bracketExpression);
					isFindSubOperation = true;
					break;
				}
				
			}

			if (isFindSubOperation == true)
			{
				rightBracket = something.find(')', xPos);
				for (int i = leftBracket + 1; i != rightBracket; i++)
				{
					if (something[i] == '(' && i < rightBracket) rightBracket = something.find(')', rightBracket + 1);
				}
				if (operation == "pow") rightBracket = something.rfind(',', rightBracket);
			}
			
			if (operation == "pow")
			{
				string sAnswer;
				size_t degreeBracket;
				degreeBracket = something.find(')', rightBracket);
				string degree(something, rightBracket+1, degreeBracket - rightBracket - 1);
				sAnswer = to_string(pow(stof(bracketExpression),stof(degree)));
				something.replace(xPos, degreeBracket - xPos + 1, sAnswer);
			}
			else
				something.replace(xPos, rightBracket - xPos + 1, functionOperationsConvert(bracketExpression, operation));

			if (something.find('#')!=string::npos) return something;
		}
	}

	if (something[0] == '-') something[0] = 'i';
	//arithmeticOperations
	for (char& operation : arithmeticOperations)
	{
		for (size_t xPos = something.find(operation); xPos != string::npos; xPos = something.find(operation))
		{
			if (operation == '(')
			{
				size_t leftBracket = xPos, rightBracket = something.find(')');
				for (int i = leftBracket + 1; i != rightBracket; i++)
				{
					if (something[i] == '(' && i < rightBracket) rightBracket = something.find(')', rightBracket + 1);
				}
				string bracketExpression(something, leftBracket + 1, rightBracket - leftBracket - 1);
				something.replace(leftBracket, rightBracket - leftBracket + 1, convertStringToInt(bracketExpression, x));
			}
			else
			{
				string leftNumber, rightNumber;
				size_t leftBracket, rightBracket;
				bool isFind = false;
				for (int i = xPos-1; isFind != true; i--)
				{
					for (char& subOperation : arithmeticOperations)
					{
						if (something[i] == subOperation || i == 0) 
						{
							if (i == 0) leftNumber.assign(something, i, xPos);
							else leftNumber.assign(something, i+1, xPos-i-1); 
							isFind = true; 
							leftBracket = i;
							break; 
						}
					}
				}
				isFind = false;
				for (int i = xPos+2; isFind != true; i++)
				{
					for (char& subOperation : arithmeticOperations)
					{
						if (something[i] == subOperation || i == something.length()) 
						{ 
							rightNumber.assign(something, xPos + 1, i-xPos-1); 
							isFind = true; 
							rightBracket = i;
							break; 
						}
					}
				}

				if (rightNumber[0] == 'i') rightNumber[0] = '-';
				if (leftNumber[0] == 'i') leftNumber[0] = '-';

				float irightNumber = stof(rightNumber), ileftNumber = stof(leftNumber);
				string sAnswer = to_string(arithmeticOperationsConvert(ileftNumber, irightNumber, operation));
				if (sAnswer[0] == '-') sAnswer[0] = 'i';
				if(leftBracket==0) something.replace(leftBracket, rightBracket, sAnswer);
				else something.replace(leftBracket+1, rightBracket-leftBracket-1, sAnswer);
				//cout << ileftNumber << operation << irightNumber << "=" << answer << endl;
				
			}
			
		}
	}
	if (something[0] == 'i') something[0] = '-';
	return something;
}

int correctionExpression(string& something)
{
	for (int i = 0 ;i<something.length();i++)
	{
		if (something[i] == ' ') something.erase(something.begin()+i);
		something[i] = tolower(something[i]);
	}
	
	int amountLeftBrackets = 0, amountRightBrackets = 0;
	for (size_t leftBracket = something.find('('); leftBracket != string::npos; leftBracket = something.find('(', leftBracket+1))
		amountLeftBrackets++;
	for (size_t rightBracket = something.find(')'); rightBracket != string::npos; rightBracket = something.find(')', rightBracket+1))
		amountRightBrackets++;

	if (amountLeftBrackets != amountRightBrackets) return -1;

	return 0;
}

void correctionHandler(string& something)
{
	int error=0;
	do
	{
		error = correctionExpression(something);
		switch (error)
		{
			case -1:
			{
				cout << "Incorrect number of brackets" << endl;
				cout << "Enter again: "; getline(cin, something);
				break;
			}
		}
	} while (error != 0);
}

int main()
{
	string e("pow(x,2)");
	cout << "Enter an expression using the following functions:\npow, sqrt, sin, cos, tan \nand arithmetic operations" << endl;
	getline(cin, e);
	correctionHandler(e);
	string tmp;
	for (int i = 0; i < 1000; i++)
	{
		tmp = convertStringToInt(e, i);
		cout << "x = " << i << ". ";
		if (tmp.find('#') == string::npos)
			cout << stof(tmp) << endl;
		else
			cout << tmp << endl;
	}
	
}