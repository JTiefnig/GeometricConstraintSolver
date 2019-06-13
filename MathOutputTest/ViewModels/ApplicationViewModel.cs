using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CASCwrapper;
using System.ComponentModel;
using System.Collections.ObjectModel;



namespace MathOutputTest.ViewModels
{
    public class ApplicationViewModel : INotifyPropertyChanged
    {

        public ObservableCollection<Equation> Equations { get; set; }

        public event PropertyChangedEventHandler PropertyChanged;

        public void OnPropertyChanged(string Name) => PropertyChanged(this, new PropertyChangedEventArgs(Name)); 


        public ApplicationViewModel()
        {
            Equations = new ObservableCollection<Equation>();
        }

        


    }




    public class Equation
    {
        public String Name { get; set; }
        public String LateX { get; set; }
    }

}
