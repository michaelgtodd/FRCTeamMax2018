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
        List<Rectangle> ButtonList;
        public JoystickWidget()
        {
            InitializeComponent();
            ButtonList = new List<Rectangle>
            {
                Button1, Button2, Button3, Button4, Button5, Button6, Button7, Button8, Button9, Button10, Button11, Button12
            };
        }

        public void UpdateButtonData(ControllerData DataForButtons)
        {
            for (int i = 0; i < 12; i++)
            {
                if (DataForButtons.ButtonList[i])
                {
                    ButtonList[i].Fill = Brushes.Green;
                }
                else
                {
                    ButtonList[i].Fill = Brushes.Black;
                }
            }
        }


        public void UpdateControllerData(ControllerData DataForController)
        {
            JoystickAxisGrid.MakeThingMove(DataForController.AxisList[0], DataForController.AxisList[1]);
            JoystickZAxisBar.ChangeZAxisWidth(DataForController.AxisList[2]);
            Z.Text = DataForController.AxisList[2].ToString();
            X.Text = DataForController.AxisList[0].ToString();
            Y.Text = DataForController.AxisList[1].ToString();
        }
       public void UpdateJoystickWidget(ControllerData Controller)
        {
            UpdateControllerData(Controller);
            UpdateButtonData(Controller);
        }
    }
}
