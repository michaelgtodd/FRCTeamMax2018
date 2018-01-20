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
    /// Interaction logic for AxisGrid.xaml
    /// </summary>
    public partial class AxisGrid : UserControl
    {
        public AxisGrid()
        {
            InitializeComponent();
        }

        public void MakeThingMove(double XAxis, double YAxis)
        {
            double XAxisPosition = XAxis * 45;
            double YAxisPosition = YAxis * 45;
            Thickness AxisPositionMargin = new Thickness(XAxisPosition, YAxisPosition, 0, 0);
            Ellipse1.Margin = AxisPositionMargin;
        }
    }
}
