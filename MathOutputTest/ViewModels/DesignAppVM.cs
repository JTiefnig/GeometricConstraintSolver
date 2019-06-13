using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MathOutputTest.ViewModels
{
    class DesignAppVM : ApplicationViewModel
    {

        public DesignAppVM()
            :base()
        {

            TestInit();

        }


        private void TestInit()
        {
            Equations.Add(new Equation() { Name = "First Test Equation", LateX = @"\left(x^2 + 2 \cdot x + 2\right) = 0" });

            Equations.Add(new Equation() { Name = "Second Test Equation", LateX = @"F(x) = \int^a_b \frac{1}{3}x^3" });


        }


    }
}
