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
    /// Interaction logic for AlertBox.xaml
    /// </summary>
    public partial class AlertBox : UserControl
    {
        private List<string> ErrorList { get; set; }

        public AlertBox()
        {
            InitializeComponent();
            ErrorList = new List<string>();
        }

        private void Tasks_Button_Click(object sender, RoutedEventArgs e)
        {
            TasksMonitor ew = new TasksMonitor();
            ew.Name = "ActiveTaskMonitor";
            ew.Show();
        }

        private void Swap_Button_Click(object sender, RoutedEventArgs e)
        {
            ErrorsWindow ew = new ErrorsWindow();
            ew.Show();
        }

        public void UpdateError(string Error)
        {
            if (ErrorList.Contains(Error))
            {
                return;

            }
            ErrorList.Add(Error);
            switch (ErrorList.Count)
            {
                case 1:
                    Block1.Text = Error;
                    break;
                case 2:
                    Block2.Text = Error;
                    break;
                case 3:
                    Block3.Text = Error;
                    break;
                case 4:
                    Block4.Text = Error;
                    break;
                case 5:
                    Block5.Text = Error;
                    break;
                case 6:
                    Block6.Text = Error;
                    break;
                case 7:
                    Block7.Text = Error;
                    break;
                case 8:
                    Block8.Text = Error;
                    break;
                case 9:
                    Block9.Text = Error;
                    break;
                default:
                    break;

            }


        }
    }
}
