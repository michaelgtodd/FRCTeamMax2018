using System.Windows.Controls;
using System.Windows.Media;

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
