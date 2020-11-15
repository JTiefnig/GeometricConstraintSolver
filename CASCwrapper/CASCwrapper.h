#pragma once

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Collections::ObjectModel;
using namespace System::ComponentModel;


#include "EquationProperty.h"
#include "Model.h"




namespace CASCwrapper {

	public ref class AlgebraSystem : INotifyPropertyChanged
	{
	public:


		


		AlgebraSystem();

		void TestSetup();


		property ObservableCollection<EquationProperty^>^ Equations
		{
			ObservableCollection<EquationProperty^>^ get()
			{
				return _Equations;
			}

			void set(ObservableCollection<EquationProperty^>^ nl)
			{
				this->_Equations = nl;
				OnPropertyChanged("Equations");
			}
		}

		virtual ~AlgebraSystem()
		{
			delete model;
		}

		// Geerbt über INotifyPropertyChanged
		virtual event System::ComponentModel::PropertyChangedEventHandler ^ PropertyChanged;


	private:

		ObservableCollection<EquationProperty^>^ _Equations;

		Model* model;

		void OnPropertyChanged(String^ info)
		{
			PropertyChanged(this, gcnew PropertyChangedEventArgs(info));
		}

	};
}
