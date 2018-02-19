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
    /// Interaction logic for AutoSwitchControl.xaml
    /// </summary>
    public partial class AutoSwitchControl : UserControl
    {
        public string Position;
        public string Goal;
        public AutoSwitchControl()
        {
            InitializeComponent();
            Postion = "Left";
            Goal = "Yes";
        }

        private void Position_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            ComboBoxItem Position_Selected = (ComboBoxItem)e.AddedItems[0];
            if (Position_Selected.Name == "AutoPositionLeft")
            {
                Position = "Left";
                /// Double Quotes = " this is inside "
                /// Why is "" his is inside ""
                /// ' is apparently a quote
            }
            else if (Position_Selected.Name == "AutoPositionMid")
            {
                Position = "Mid";
            }
            else if (Position_Selected.Name == "AutoPositionRight")
            {
                Position = "Right";
            }
        }

        private void Goal_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            ComboBoxItem Goal_Selected = (ComboBoxItem)e.AddedItems[0];
            if (Goal_Selected.Name == "No")
            {
                Goal = "No";
            }
            else if (Goal_Selected.Name == "Yes")
            {
                Goal = "Yes";
            }
            else if (Goal_Selected.Name == "Kyle")
            {
                Goal = "Kyle";
            }
        }

    }
}
