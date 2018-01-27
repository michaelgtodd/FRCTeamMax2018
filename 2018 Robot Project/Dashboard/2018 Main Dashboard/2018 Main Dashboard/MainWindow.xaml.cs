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
using System.Windows.Threading;

namespace _2018_Main_Dashboard
{

    public class DashboardData
    {
        public string CurrentController { get; set; }
        public string CurrentDriveMode { get; set; }
    }

    public class ControllerData
    {
        public List<double>AxisList { get; set; }
        public List<bool>ButtonList { get; set; }
        public ControllerData()
        {
            AxisList = new List<double>
            {
                0.0, 0.0, 0.0, 0.0, 0.0, 0.0
            };
            ButtonList = new List<bool>
            {
                false, false, false, false, false, false, false, false, false, false, false, false
            };    
        }
    }

    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        DashboardData CurrentDashboardData = new DashboardData();
        ControllerData ControllerData0 = new ControllerData();
        ControllerData ControllerData1 = new ControllerData();
        ControllerData ControllerData2 = new ControllerData();
        ControllerData ControllerData3 = new ControllerData();

        public void OscReceiveRunner()
        {
            var listener = new UDPListener(5801);
            OscMessage messageReceived = null;
            while (true)
            {
                try
                {
                    messageReceived = (OscMessage)listener.Receive();
                    if (messageReceived != null)
                    {
                        HandleOscPacket(messageReceived);
                    }
                    Thread.Sleep(1);
                }
                catch
                {
                    Console.WriteLine("Unhandled Exception");
                }
            }
        }

        public void OscSendRunner()
        {
            UDPSender Sender = new SharpOSC.UDPSender("10.10.71.2", 5801);
            OscBundle ToSend = new SharpOSC.OscBundle(Utils.DateTimeToTimetag(DateTime.Now));
            OscMessage ControllerMessage = new OscMessage("/Dashboard/ControllerMessage/", CurrentDashboardData.CurrentController);
            ToSend.Messages.Add(ControllerMessage);
            OscMessage DriveModeMessage = new OscMessage("/Dashboard/DriveModeMessage/", CurrentDashboardData.CurrentDriveMode);
            ToSend.Messages.Add(DriveModeMessage);
            Sender.Send(ToSend);
            Thread.Sleep(1);
        }

        public void HandleOscPacket(OscMessage message)
        {
            if (message.Address.Equals("/Error/Test"))
            {
                Application.Current.Dispatcher.BeginInvoke(
                DispatcherPriority.Background,
                new Action(() => AlertWidget.UpdateError((string)message.Arguments[0])));
            }
            if (message.Address.Contains("Joystick"))
            {
                string[] JoystickStringArray = message.Address.Split('/');
                if(JoystickStringArray[2].Equals("0"))
                {
                    SetControllerData(ControllerData0, JoystickStringArray, message);
                }
                if (JoystickStringArray[2].Equals("1"))
                {
                    SetControllerData(ControllerData1, JoystickStringArray, message);
                }
                if (JoystickStringArray[2].Equals("2"))
                {
                    SetControllerData(ControllerData2, JoystickStringArray, message);
                }
                if (JoystickStringArray[2].Equals("3"))
                {
                    SetControllerData(ControllerData3, JoystickStringArray, message);
                }
            }
        }
        public void SetControllerData(ControllerData CurrentControllerData, string[] StringArray, OscMessage Message)
        {
            if (StringArray[3] == "Axis")
            {
                int AxisNumber = int.Parse(StringArray[4]);
                CurrentControllerData.AxisList[AxisNumber] = (double)Message.Arguments[0];
            }
            if (StringArray[3] == "Button")
            {
                int ButtonNumber = int.Parse(StringArray[4]);
                CurrentControllerData.ButtonList[ButtonNumber] = (int)Message.Arguments[0] == 1 ? true : false;
            }
        }

        public void DashboardDataRunner()
        {
            while (true)
            {
                Application.Current.Dispatcher.BeginInvoke(
                DispatcherPriority.Background,
                new Action(() => CurrentDashboardData.CurrentController = ControlWidget.ControllerCombobox.Text));
                Application.Current.Dispatcher.BeginInvoke(
               DispatcherPriority.Background,
               new Action(() => CurrentDashboardData.CurrentDriveMode = (ControlWidget.JoystickWidget.TankCheckbox.IsChecked == true) ? "Tank" : "Arcade"));
                Thread.Sleep(1);
            }
        }

        public void JoystickRunner()
        {
            while (true)
            {
                Application.Current.Dispatcher.BeginInvoke(
                DispatcherPriority.Background,
                new Action(() => ControlWidget.JoystickWidget.UpdateJoystickWidget(ControllerData0)));
                Thread.Sleep(1);
            }
        }

        public MainWindow()
        {
            InitializeComponent();

            System.Threading.Thread OscReceive =
                new System.Threading.Thread
                    (new System.Threading.ThreadStart(OscReceiveRunner));
            OscReceive.Start();

            System.Threading.Thread OscSend =
                new System.Threading.Thread
                    (new System.Threading.ThreadStart(OscSendRunner));
            OscSend.Start();

            System.Threading.Thread UpdateDashboardData =
                new System.Threading.Thread
                    (new System.Threading.ThreadStart(DashboardDataRunner));
            UpdateDashboardData.Start();

            System.Threading.Thread UpdateJS =
    new System.Threading.Thread
        (new System.Threading.ThreadStart(JoystickRunner));
            UpdateJS.Start();
        }
    }
}
