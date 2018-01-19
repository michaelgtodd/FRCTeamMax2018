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
    /// Interaction logic for AxisBar.xaml
    /// </summary>
    public partial class AxisBar : UserControl
    {
        public AxisBar()
        {
            InitializeComponent();
        }

        public void MakeThingMove(double XAxis, double YAxis)
        {
            double XAxisPosition = XAxis * 25 + 25;
            double YAxisPosition = YAxis * 25 + 25;
            Thickness AxisPositionMargin = new Thickness(XAxisPosition-12, YAxisPosition-12, 0, 0);
            Ellipse1.Margin = AxisPositionMargin;
        }
    }
}
