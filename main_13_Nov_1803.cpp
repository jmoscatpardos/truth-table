	/*
	 * File:   main.cpp
	 * Author: Jorge Moscat Pardos
	 * Assigment: Assigment 2 for Discrete Comp Struct I
	 * Created on November 6, 2010, 4:50 PM
		

/*

OBJECTIVE: Output truth table for the boolean expression entered by the user.

DESCRIPTION OF ALGORITHM:
There are two main parts in this code: the compute function and the evaluate function. 
-The compute function returns a boolean vector as a result of the logic operation provided as an argument. 
The arguments of the function are: a string that determines the operator (wee, wedge,etc) and two boolean
vectors as the operands. Inside the function there is logic to do the operation by using c++ logic operators (&&, ||, !)

-The evaluate function is a recursive function that get two arguments: the an starting index and an ending index. The program
puts each element of the formula provided by the user on a vector string for easy reference. Therefore, the evaluate function 
nows the beginning and the end of the expression that needs to be evaluated. 
	This recursive function uses a divide-and-conquer technique. Given an infix operation (\not, \wedge, etc) it determines if the 
operands are just variables or another logical expressions. Once an operand is detected the function call itself twice for the two operand.
However, if it is a "\not" it only calls itself once since there is only one operand. 
	Down in the recursion stack, once a variable is reach, the function returns a boolean vector that contains the truth table values for that
variable. This is the boot-strap condition. 
	Once all the calls are return the last step is to call compute that will compute the two expressions derived on the recursion tree. Unless, the
last operand is a "\not" that will only get one vector from the recursive call.






*/

	#include <stdlib.h>
	#include<string>

	#include <iostream>
	#include <sstream>
	#include <vector>
	#include <stdio.h>
	#include <math.h>
	#define MAX_LINE 256
	using namespace std;
	vector<bool> columns;
	vector<string> formula_vector;
	vector<string> variables;
	vector<vector<bool> > rows;
	vector<bool>null_vector;
	int num_variables = 0;
	bool findCharString (string myString, string toFindChar);
	int findVariable (string variable);
	int findClosingParent (int start );
	vector<bool>  compute (string operation, vector<bool>v1, vector<bool> v2);


//This is the evaluate function. It is recursive. It takes two arguments: the first and last index of the expression from the 
//varaible vectors. In other words the positions of the parenthesis that surrounds an expresion.
	vector<bool> evaluate ( int first_index, int last_index){
		int close = 0;
		vector<bool>exit_vector(pow(2.0,num_variables));
		vector<bool>exit_vector2(pow(2.0,num_variables));
		if (findCharString(formula_vector[first_index], "("))
			first_index++;
		int index = 0;
		//First step if to determine if first_index is either an operator (\not, \wedge...) or a variable (x, y etc)
		if (findCharString(formula_vector[first_index], "\\")) 
		{
			//if next item is a parenthesis we know there is another expression to evaluate. Thefore:
			//1. First determine if the operator is a \not, if it is then only do one recursive call since \not only takes one variable.
			//2. If it is not "\not" we need to find the end of the expression (corresponding closing parenthesis) and call the function again
			//	 with those indexes. And call evaluate another time with the remaining part of the formula. When these two calls to evaluate return
			//   the two vectors  we need to call compute with this two parts. 
			if (findCharString(formula_vector[first_index+1], "(")) 
			{
				if (formula_vector[first_index].compare("\\not") == 0){ //if next is parenthesis and current is not
					close = findClosingParent (first_index+1);
					exit_vector = evaluate (first_index+1, close);
					return compute (formula_vector[first_index],exit_vector ,null_vector);
				}

				else{
					close = findClosingParent(first_index+1);
					exit_vector = evaluate (first_index+2, close);
					if ((!findCharString(formula_vector[close +1], "\\")) && (!findCharString(formula_vector[first_index], "(")))
						exit_vector2 = evaluate (close+1, close + 1);
					else
					exit_vector2 = evaluate (close+1, last_index);

					return compute (formula_vector[first_index],exit_vector ,exit_vector2);
				}

			}

			//If next is not a parenthesis then there are 2 possibilities:
			//1. THe operator is  "\not" therefore there is only one operand and it's just a varaible
			//2. The operator is NOT "\not" therefore we know the first operand is just a variable and we need to determine
			//   if the second is an expression or just another variable
			else{

				exit_vector = evaluate(first_index+1,first_index+1);  //This recursive call will just go to the boot-strap condition and
																	  // return the truth table vector for the given variable.
				
				
				//If the second argument is an expression, call evaluate again with the start parenthesis and the closing parenthesis.
				if (findCharString(formula_vector[first_index+2], "(")){
					close = findClosingParent (first_index+2);
					exit_vector2 = evaluate (first_index+2, close);
				}
                else if (formula_vector[first_index].compare("\\not") != 0){
					exit_vector2 = evaluate(first_index+2,first_index+2);}

				//If the operator is \not then we need to only pass the first exit vector. Therefore,
				//the third argument is just the null_vector

				if (formula_vector[first_index].compare("\\not") == 0) // If the operator is "\not" we call compute with the last argument beign the a null_vector.	
					return compute (formula_vector[first_index],exit_vector ,null_vector);
				else
					return compute (formula_vector[first_index],exit_vector ,exit_vector2);
			}


		}


		else if ( first_index == last_index)// If first and last index are equal it is a variable, therefore return the truth table vector
											//that variable.
		{
			index =  findVariable (formula_vector[first_index]);
			return rows[index];
		}






	}

//This is the compute function. Given a string with the operator (\not, \wedge, \vee...) and two boolean vectors
//it return a boolean vector with the result.
	vector<bool>  compute (string operation, vector<bool>v1, vector<bool> v2){
		vector<bool> result;
		size_t found;
		bool a = false;
		bool b = true;
		result.resize(pow(2.0,num_variables));
		//If the second operand is empty that means that we need to perform a \not.
		if (v2.empty()){
			for (int i = 0; i < pow(2.0,num_variables); i++)
				result[i] = !v1[i];
		}

		else{
				if(operation.compare("\\vee") == 0){
					for (int i = 0; i < pow(2.0,num_variables); i++){
						result[i] = v1[i] || v2[i];}}
                                else if (operation.compare("\\wedge")== 0){
					for (int i = 0; i < pow(2.0,num_variables); i++){
						result[i] = v1[i] && v2[i];}}
                                else if (operation.compare("\\rightarrow")== 0){
					for (int i = 0; i < pow(2.0,num_variables); i++){
						result[i] = (v1[i]&&v2[i]) ||((!v1[i])&& v2[i]) ||((!v1[i])&&(!v2[i]));}}
                                else if (operation.compare("\\leftrightarrow")==0){
					for (int i = 0; i < pow(2.0,num_variables); i++){
						result[i] = (v1[i]&&v2[i])||((!v1[i])&&(!v2[i]));}}
			}

			return result;
		}


	int findVariable (string variable)
	{
		for(int i = 0; i < num_variables ; i++)
		{
			if (variable.compare(variables[i]) == 0)
				return i;
		}
	}

	bool findCharString (string myString, string toFindChar)
	{

		size_t found;
		found = myString.find_first_of(toFindChar);
		if (found == string::npos)
			return false;
		else
			return true;


	}



//This fucntion return the index in the vector formula of the correspoding closing parenthesis.
	int findClosingParent (int start )
	{
		int open = 0;
		int close_p = 0;
		int counter = start;


		while (counter<formula_vector.size())
		{
			if (formula_vector[counter].compare(")") == 0)
				close_p++;
			else if (formula_vector[counter].compare("(") == 0)
				open++;
			if (open == close_p)
				return counter;
			counter++;

		}

	}

//Given the number of variables, this function updates the rows ( boolean vector of boolean vectors) 
// and creates and displays the truth table. 

	void createTruthTables_2(int number_variables)
	{
		int num_2_fill = 0;
		rows.resize(number_variables);
		columns. resize (pow(2.0,number_variables));
		rows.assign(number_variables,columns);

		//First columns then rows
		for (int i=0; i<number_variables; i++){
			num_2_fill = pow(2.0, (number_variables -1)-i);
			for (int j = 0; j<pow(2.0,number_variables); j=j+2*num_2_fill){
				for (int k = j; k<num_2_fill+j; k++)
					rows[i][k] = true;
			}
		}


		//Loop to display the truth table 
				for(int i=0;i<rows.size(); i++) {
					cout<<variables[i]<<": ";
				   for (int j=0;j<rows[i].size(); j++)
					 cout <<rows[i][j] << " ";
					 cout << endl;
		}


	}




	int main() {

		int size_of_formula=0;
		char buf [MAX_LINE];
		char buf2 [MAX_LINE];
		char variables_in [MAX_LINE];
		char formula [MAX_LINE];
		string buffer;
		char * pch;
		int counter = 0;
		int count_elements = 0;
		



		cout<< "Please enter the number of variables in your formula"<<endl;
		fgets(buf, sizeof(buf),stdin);
		num_variables = atoi (buf);
		variables.resize(num_variables);

		cout<< "Please enter the variables separeted by spaces"<<endl;
		fgets(variables_in, sizeof(variables_in),stdin);


		cout<< "Now please enter your formula"<<endl;
		fgets(formula, sizeof(formula),stdin);



	 //   strncpy(buf,formula.c_str(), formula.size());

//Putting each variable in a string vector. This vector will be used
//later in the recursions calls.
		strcpy (buf2,variables_in);
		pch = strtok (buf2, " \n");
		while ((pch != NULL)){
			buffer = pch;
			variables[counter]= pch;
			counter++;

			pch = strtok (NULL, " \n");

		}




//Putting each element of in the formula in a string vector. This vector will be used
//later in the recursions calls.

		strcpy (buf,formula);
		pch = strtok (buf, " ");
		while ((pch != NULL)){
			count_elements++;
			pch = strtok (NULL, " \n");

		}

		formula_vector.resize(count_elements);
		count_elements = 0;
		strcpy (buf,formula);
		pch = strtok (buf, " ");
		while ((pch != NULL)){
			formula_vector[count_elements]  = pch;
			pch = strtok (NULL, " \n");
			count_elements++;

		}

		//Generating and displaying the truth table:
		 createTruthTables_2(num_variables);



		 vector<bool>result(pow(2.0,num_variables));
		 int last = formula_vector.size();

		 result = evaluate ( 0, (last -1 )	);

		//Displaying the result of the operation
		 cout<<endl<<"F: ";
		 for (int i =0; i <pow(2.0,num_variables); i++)
		 {
			 cout<<result[i]<<" ";

		 }
		 cout<<endl;



		return (0);
	}

