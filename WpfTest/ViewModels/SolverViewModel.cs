using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SolverAdapter;
using System.Windows.Input;



namespace WpfTest
{
    public class SolverViewModel
    {

        public Solver NummericSolver { get; private set; }

        public List<ParameterBridge> Parameters
        {
            get
            {
                return NummericSolver.getParameterList();
            }
        }

        public SolverViewModel()
        {
            NummericSolver = new Solver();
        }

        /// <summary>
        /// 
        /// </summary>
        public ICommand SolveCommand => new RelayCommand(NummericSolver.solve);

    }
}
