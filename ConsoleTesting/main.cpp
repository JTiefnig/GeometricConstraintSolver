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


	for (int i = 0; i < 4; i++)
	{
		d = Sin(Log(a,b)) + d + cos(b);
	}

	cout << "before " << d.toString() << endl << endl;
	d = d.partDif(b);

	auto dd = d.substitute(Sin(b), x);

	cout << "after " << dd.toString() << endl << endl;

	//d = a * b + c - d+y;


	//d = Log(a, d);


	//system("explorer http://google.com"); // todo c


	
	cout << "Eval: " << d.eval() << endl;



	system("PAUSE");
	return 0;
}