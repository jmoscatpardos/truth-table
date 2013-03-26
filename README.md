truth-table
===========

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
