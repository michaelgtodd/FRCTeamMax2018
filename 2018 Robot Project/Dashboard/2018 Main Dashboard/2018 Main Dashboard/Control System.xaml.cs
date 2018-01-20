using SharpOSC;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
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
    /// Interaction logic for Control_System.xaml
    /// </summary>
    public partial class Control_System : UserControl
    {
        private double XAxis;
        private double YAxis;
        private double ZAxis;


        public Control_System()
        {
            InitializeComponent();
        }

        private void ComboBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            ComboBoxItem Selected_Controller = (ComboBoxItem)e.AddedItems[0];
            if (Selected_Controller.Name == "Joystick")
            {
                try
                {
                    Controller_Image.Source = new BitmapImage(new Uri("Joystick.jpg", UriKind.Relative));
                }
                catch (Exception exception)
                {
                    Console.WriteLine("Yo, there was exception. Sucks to be you.");
                }
            }
            else if (Selected_Controller.Name == "Xbox_Controller")
            {
                Controller_Image.Source = new BitmapImage(new Uri("Xbox Controller.jpg", UriKind.Relative));
            }
        }

        public void UpdateControllerData(JoystickAxis AxisType, double AxisValue)
        {            
            switch (AxisType)
            {
                case (JoystickAxis.XAxis):
                    XAxis = AxisValue;
                    break;
                case (JoystickAxis.YAxis):
                    YAxis = AxisValue;
                    break;
                case (JoystickAxis.ZAxis):
                    ZAxis = AxisValue;
                    break;
                default:
                    break;
            }
            JoystickAxisGrid.MakeThingMove(XAxis, YAxis);
            JoystickZAxisBar.ChangeZAxisWidth(ZAxis);
            Z.Text = ZAxis.ToString();
            X.Text = XAxis.ToString();
            Y.Text = YAxis.ToString();
        }
    }
}
