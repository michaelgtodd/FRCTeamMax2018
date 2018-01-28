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
        List<Border> ButtonList;
        public JoystickWidget()
        {
            InitializeComponent();
            ButtonList = new List<Border>
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
                    ButtonList[i].Background = Brushes.Green;
                }
                else
                {
                    ButtonList[i].Background = Brushes.Black;
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

        private void TankCheckboxChecked(object sender, RoutedEventArgs e)
        {
            try
            {
                ArcadeCheckbox.IsChecked = false;
            }
            catch (NullReferenceException)
            {

            }
        }

        private void ArcadeCheckboxChecked(object sender, RoutedEventArgs e)
        {
            try
            {
                TankCheckbox.IsChecked = false;
            }
            catch (NullReferenceException)
            {

            }
        }

        private void TankCheckboxUnchecked(object sender, RoutedEventArgs e)
        {
            try
            {
                ArcadeCheckbox.IsChecked = true;
            }
            catch (NullReferenceException)
            {

            }
        }

        private void ArcadeCheckboxUnchecked(object sender, RoutedEventArgs e)
        {
            try
            {
                TankCheckbox.IsChecked = true;
            }
            catch (NullReferenceException)
            {

            }
        }
    }
}
