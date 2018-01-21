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
        public string CurrentControllerLocal = "Joystick";

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
                CurrentControllerLocal = "Joystick";
            }
            else if (Selected_Controller.Name == "Xbox_Controller")
            {
                Controller_Image.Source = new BitmapImage(new Uri("Xbox Controller.jpg", UriKind.Relative));
                CurrentControllerLocal = "Xbox_Controller";
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

        public void UpdateButtonData(int ButtonNum, bool ButtonPressed)
        {
            Brush NewColor;
            if (ButtonPressed)
            {
                NewColor = Brushes.Green;
            }
            else
            {
                NewColor = Brushes.Black;
            }
            switch (ButtonNum)
            {
                case (1):
                    Button1.Fill = NewColor;
                    break;
                case (2):
                    Button2.Fill = NewColor;
                    break;
                case (3):
                    Button3.Fill = NewColor;
                    break;
                case (4):
                    Button4.Fill = NewColor;
                    break;
                case (5):
                    Button5.Fill = NewColor;
                    break;
                case (6):
                    Button6.Fill = NewColor;
                    break;
                case (7):
                    Button7.Fill = NewColor;
                    break;
                case (8):
                    Button8.Fill = NewColor;
                    break;
                case (9):
                    Button9.Fill = NewColor;
                    break;
                case (10):
                    Button10.Fill = NewColor;
                    break;
                case (11):
                    Button11.Fill = NewColor;
                    break;
                case (12):
                    Button12.Fill = NewColor;
                    break;
                default:
                    break;
            }
        }
    }
}
