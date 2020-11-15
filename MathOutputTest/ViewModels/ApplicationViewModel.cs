using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CASCwrapper;
using System.ComponentModel;
using System.Collections.ObjectModel;
using System.Windows.Input;



namespace MathOutputTest.ViewModels
{
    public class ApplicationViewModel : INotifyPropertyChanged
    {

        public ObservableCollection<Equation> Equations
        {
            get;
            private set;
        }

        public event PropertyChangedEventHandler PropertyChanged;


        public AlgebraSystem Algebra { get;  }

        public void OnPropertyChanged(string Name) => PropertyChanged(this, new PropertyChangedEventArgs(Name)); 


        public ApplicationViewModel()
        {
            Equations = new ObservableCollection<Equation>();

            Algebra = new AlgebraSystem();
        }




        #region Commands

        public ICommand TestCommand => new RelayCommand(TestExecute);

        void TestExecute()
        {
            Algebra.TestSetup();
        }

        #endregion


    }




    public class Equation
    {
        public String Name { get; set; }
        public String LateX { get; set; }
    }

}
