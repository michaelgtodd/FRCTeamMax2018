using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace _2018_Main_Dashboard
{
    /// <summary>
    /// Interaction logic for JoystickWidget.xaml
    /// </summary>
    public partial class JoystickWidget : UserControl
    {
        List<Border> ButtonList;
        List<Border> SecondaryButtonList;
        public JoystickWidget()
        {
            InitializeComponent();
            ButtonList = new List<Border>
            {
                Button1, Button2, Button3, Button4, Button5, Button6, Button7, Button8, Button9, Button10, Button11, Button12
            };
            SecondaryButtonList = new List<Border>
            {
                SecondaryButton1, SecondaryButton2, SecondaryButton3, SecondaryButton4, SecondaryButton5, SecondaryButton6, SecondaryButton7, SecondaryButton8, SecondaryButton9, SecondaryButton10, SecondaryButton11, SecondaryButton12
            };
        }

        public void UpdateButtonData(ControllerData DataForButtons0, ControllerData DataForButtons1)
        {
            for (int i = 0; i < 12; i++)
            {
                if (DataForButtons0.ButtonList[i])
                {
                    ButtonList[i].Background = Brushes.Green;
                }
                else
                {
                    ButtonList[i].Background = Brushes.Black;
                }

                if (DataForButtons1.ButtonList[i])
                {
                    SecondaryButtonList[i].Background = Brushes.Green;
                }
                else
                {
                    SecondaryButtonList[i].Background = Brushes.Black;
                }
            }
        }


        public void UpdateControllerData(ControllerData DataForController0, ControllerData DataForController1)
        {
            JoystickAxisGrid.MakeThingMove(DataForController0.AxisList[0], DataForController0.AxisList[1]);
            JoystickZAxisBar.ChangeZAxisWidth(DataForController0.AxisList[2], ControllerCombobox.Text);
            if (ControllerCombobox.Text == "Joystick")
            {
                SecondaryGrid.MakeThingMove(DataForController1.AxisList[0], DataForController1.AxisList[1]);
                SecondaryAxisBar.ChangeZAxisWidth(DataForController1.AxisList[2], "Joystick");
            }
            else if (ControllerCombobox.Text == "Xbox Controller")
            {
                SecondaryGrid.MakeThingMove(DataForController0.AxisList[4], DataForController0.AxisList[5]);
                SecondaryAxisBar.ChangeZAxisWidth(DataForController0.AxisList[3], "Xbox Controller");
            }
        }
        public void UpdateJoystickWidget(ControllerData Controller0, ControllerData Controller1)
        {
            UpdateControllerData(Controller0, Controller1);
            UpdateButtonData(Controller0, Controller1);
        }

        private void TankCheckboxChecked(object sender, RoutedEventArgs e)
        {
            try
            {
                ArcadeCheckbox.IsChecked = false;
                ShowController();
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
                ShowController();
            }
            catch (NullReferenceException)
            {

            }
        }

        private void ShowController()
        {
            if (ControllerCombobox.SelectedIndex == 0)
            {
                if (ArcadeCheckbox.IsChecked == true)
                {
                    SecondaryXY.Visibility = Visibility.Hidden;
                    SecondaryZ.Visibility = Visibility.Hidden;
                    SecondaryGrid.Visibility = Visibility.Hidden;
                    SecondaryButtonGrid.Visibility = Visibility.Hidden;
                    SecondaryAxisBar.Visibility = Visibility.Hidden;
                    SecondaryButtons.Visibility = Visibility.Hidden;
                }
                else if (TankCheckbox.IsChecked == true)
                {
                    SecondaryXY.Visibility = Visibility.Visible;
                    SecondaryZ.Visibility = Visibility.Visible;
                    SecondaryGrid.Visibility = Visibility.Visible;
                    SecondaryButtonGrid.Visibility = Visibility.Visible;
                    SecondaryAxisBar.Visibility = Visibility.Visible;
                    SecondaryButtons.Visibility = Visibility.Visible;
                }
            }
            else if (ControllerCombobox.SelectedIndex ==1)
            {
                SecondaryXY.Visibility = Visibility.Visible;
                SecondaryZ.Visibility = Visibility.Visible;
                SecondaryGrid.Visibility = Visibility.Visible;
                SecondaryButtonGrid.Visibility = Visibility.Hidden;
                SecondaryAxisBar.Visibility = Visibility.Visible;
                SecondaryButtons.Visibility = Visibility.Hidden;
            }
        }

        private void TankCheckboxUnchecked(object sender, RoutedEventArgs e)
        {
            try
            {
                ArcadeCheckbox.IsChecked = true;
                ShowController();
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
                ShowController();
            }
            catch (NullReferenceException)
            {

            }
        }

        private void ControllerComboboxSelectionChanged(object sender, SelectionChangedEventArgs e)
        {
           try
            {
                ShowController();
            }
            catch
            {

            }
        }

        public void UpdateSwitchesName(string NewName)
        {
            DriverControllerSelection.Text = NewName;
        }
    }
}
