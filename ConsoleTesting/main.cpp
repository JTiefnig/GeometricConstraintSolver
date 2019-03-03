#include <iostream>
#include <vector>
#include <stdlib.h>
#include "Matrix.h"

#include "Expression.h"
#include "ExpressionMatrix.h"


using namespace std;




int main()
{


	ModelParameter a = ModelParameter("A", 10.0);
	Constant b = 2;

	Expression d;

	d = b - a;

	cout << d.toString() << endl;

	Expression dif;

	ExpressionMatrix ex(4, 4, d); // need some kind of cool constructor for this 

	for (int i = 0; i < 100; i++)
	{

		Expression e(a);

		Expression c = a + 1;

		Expression y(c);

		d = a ^ 2;

		d = a + b - c * y - a;

		dif = d.partDif(a);
	}

	//d = a * b + c - d+y;


	//d = Log(a, d);


	//system("explorer http://google.com"); // todo c


	cout << d.toString() << endl;
	cout << "Eval: " << d.eval() << endl;

	cout << "PartDif: " << dif.toString() << endl;

	cout << "matrix: " << ex.toString() << endl;

	system("PAUSE");
	return 0;
}