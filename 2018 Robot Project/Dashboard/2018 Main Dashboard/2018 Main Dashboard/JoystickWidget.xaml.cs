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
    /// Interaction logic for JoystickWidget.xaml
    /// </summary>
    public partial class JoystickWidget : UserControl
    {

        public JoystickWidget()
        {
            InitializeComponent();
        }

        public double XAxis { get; private set; }
        public double YAxis { get; private set; }
        public double ZAxis { get; private set; }

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


        public void UpdateControllerData(double XAxis, double YAxis, double ZAxis)
        {
            JoystickAxisGrid.MakeThingMove(XAxis, YAxis);
            JoystickZAxisBar.ChangeZAxisWidth(ZAxis);
            Z.Text = ZAxis.ToString();
            X.Text = XAxis.ToString();
            Y.Text = YAxis.ToString();
        }
       public void UpdateJoystickWidget(ControllerData Controller)
        {
            UpdateControllerData(Controller.XAxis, Controller.YAxis, Controller.ZAxis);
            //UpdateButtonData();
        }
    }
}
