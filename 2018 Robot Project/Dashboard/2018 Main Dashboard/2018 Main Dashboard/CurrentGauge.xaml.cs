using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace _2018_Main_Dashboard
{
    /// <summary>
    /// Interaction logic for CurrentGauge.xaml
    /// </summary>
    public partial class CurrentGauge : UserControl
    {
        public CurrentGauge()
        {
            InitializeComponent();
        }

        public void ChangeWidth(double Amps)
        {
            CurrentGaugeBar.Width = Amps;
            if (Amps > 120)
            {
                CurrentGaugeBar.Fill = Brushes.Red;
            }
            else
            {
                CurrentGaugeBar.Fill = Brushes.Green;
            }
        }
    }
}
