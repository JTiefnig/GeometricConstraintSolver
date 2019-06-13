#include <iostream>
#include <vector>
#include <stdlib.h>
#include "Matrix.h"
#include "Expression.h"
#include "ExpressionMatrix.h"


using namespace std;




int main()
{
	ModelParameter a("A", 10.0);
	ModelParameter b("B", 12);

	Expression x("W");

	Expression d = b;


	for (int i = 0; i < 20; i++)
	{
		d = a+b+d;
	}

	cout << "before " << d.toString() << endl << endl;
	
	d = d.substitute(a, Sin(b));
	d = d.partDif(b);
	auto dd = d.substitute(Sin(b), Cos(x));

	cout << "after " << dd.toString() << endl << endl;

	//d = a * b + c - d+y;


	//d = Log(a, d);


	//system("explorer http://google.com"); // todo c // so cool, dass ich daf�r eine l�sung gefunden hab hihi, johannes
	
	cout << "Eval: " << d.eval() << endl;



	system("PAUSE");
	return 0;
}