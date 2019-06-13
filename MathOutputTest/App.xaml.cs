using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;
using MathOutputTest.ViewModels;

namespace MathOutputTest
{
    /// <summary>
    /// Interaktionslogik für "App.xaml"
    /// </summary>
    public partial class App : Application
    {



        protected override void OnStartup(StartupEventArgs e)
        {
            base.OnStartup(e);

            var appvm = new ApplicationViewModel();

            var mainWin = new MainWindow(appvm);
            mainWin.Show();

        }

    }
}
