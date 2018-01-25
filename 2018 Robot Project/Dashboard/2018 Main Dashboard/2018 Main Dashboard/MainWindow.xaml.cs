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
    }

    public class ControllerData
    {
        public double XAxis { get; set; }
        public double YAxis { get; set; }
        public double ZAxis { get; set; }
    }

    public enum JoystickAxis
    {
        XAxis,
        YAxis,
        ZAxis,
        Throttle,
        Undefined
    }

    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public static double XAxis;
        public static double YAxis;
        public static double ZAxis;
        DashboardData CurrentDashboardData = new DashboardData();
        ControllerData CurrentControllerData = new ControllerData();

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
            Sender.Send(ToSend);
            Thread.Sleep(1);
        }

        public void HandleOscPacket(OscMessage message)
        {
            if (message.Address.Equals("/info"))
            {
                Console.WriteLine("Received an info packet!");
            }
            if (message.Address.Equals("/Joystick/1/XAxis"))
            {
                CurrentControllerData.XAxis = (double) message.Arguments[0];
            }
            if (message.Address.Equals("/Joystick/1/YAxis"))
            {
                CurrentControllerData.YAxis = (double)message.Arguments[0];
            }
            if (message.Address.Equals("/Joystick/1/ZAxis"))
            {
                CurrentControllerData.ZAxis = (double)message.Arguments[0];
            }
            if (message.Address.Equals("/Error/Test"))
            {
                Application.Current.Dispatcher.BeginInvoke(
                DispatcherPriority.Background,
                new Action(() => AlertWidget.UpdateError((string)message.Arguments[0])));
            }
            if (message.Address.Equals("/Joystick/1/B1"))
            {
                bool ButtonPressed;
                    ButtonPressed = (int)message.Arguments[0] == 1 ? true : false;
                Application.Current.Dispatcher.BeginInvoke(
                DispatcherPriority.Background,
                new Action(() => ControlWidget.JoystickWidget.UpdateButtonData(1, ButtonPressed)));
            }
            if (message.Address.Equals("/Joystick/1/B2"))
            {
                bool ButtonPressed;
                ButtonPressed = (int)message.Arguments[0] == 1 ? true : false;
                Application.Current.Dispatcher.BeginInvoke(
                DispatcherPriority.Background,
                new Action(() => ControlWidget.JoystickWidget.UpdateButtonData(2, ButtonPressed)));
            }
            if (message.Address.Equals("/Joystick/1/B3"))
            {
                bool ButtonPressed;
                ButtonPressed = (int)message.Arguments[0] == 1 ? true : false;
                Application.Current.Dispatcher.BeginInvoke(
                DispatcherPriority.Background,
                new Action(() => ControlWidget.JoystickWidget.UpdateButtonData(3, ButtonPressed)));
            }
            if (message.Address.Equals("/Joystick/1/B4"))
            {
                bool ButtonPressed;
                ButtonPressed = (int)message.Arguments[0] == 1 ? true : false;
                Application.Current.Dispatcher.BeginInvoke(
                DispatcherPriority.Background,
                new Action(() => ControlWidget.JoystickWidget.UpdateButtonData(4, ButtonPressed)));
            }
            if (message.Address.Equals("/Joystick/1/B5"))
            {
                bool ButtonPressed;
                ButtonPressed = (int)message.Arguments[0] == 1 ? true : false;
                Application.Current.Dispatcher.BeginInvoke(
                DispatcherPriority.Background,
                new Action(() => ControlWidget.JoystickWidget.UpdateButtonData(5, ButtonPressed)));
            }
            if (message.Address.Equals("/Joystick/1/B6"))
            {
                bool ButtonPressed;
                ButtonPressed = (int)message.Arguments[0] == 1 ? true : false;
                Application.Current.Dispatcher.BeginInvoke(
                DispatcherPriority.Background,
                new Action(() => ControlWidget.JoystickWidget.UpdateButtonData(6, ButtonPressed)));
            }
            if (message.Address.Equals("/Joystick/1/B7"))
            {
                bool ButtonPressed;
                ButtonPressed = (int)message.Arguments[0] == 1 ? true : false;
                Application.Current.Dispatcher.BeginInvoke(
                DispatcherPriority.Background,
                new Action(() => ControlWidget.JoystickWidget.UpdateButtonData(7, ButtonPressed)));
            }
            if (message.Address.Equals("/Joystick/1/B8"))
            {
                bool ButtonPressed;
                ButtonPressed = (int)message.Arguments[0] == 1 ? true : false;
                Application.Current.Dispatcher.BeginInvoke(
                DispatcherPriority.Background,
                new Action(() => ControlWidget.JoystickWidget.UpdateButtonData(8, ButtonPressed)));
            }
            if (message.Address.Equals("/Joystick/1/B9"))
            {
                bool ButtonPressed;
                ButtonPressed = (int)message.Arguments[0] == 1 ? true : false;
                Application.Current.Dispatcher.BeginInvoke(
                DispatcherPriority.Background,
                new Action(() => ControlWidget.JoystickWidget.UpdateButtonData(9, ButtonPressed)));
            }
            if (message.Address.Equals("/Joystick/1/B10"))
            {
                bool ButtonPressed;
                ButtonPressed = (int)message.Arguments[0] == 1 ? true : false;
                Application.Current.Dispatcher.BeginInvoke(
                DispatcherPriority.Background,
                new Action(() => ControlWidget.JoystickWidget.UpdateButtonData(10, ButtonPressed)));
            }
            if (message.Address.Equals("/Joystick/1/B11"))
            {
                bool ButtonPressed;
                ButtonPressed = (int)message.Arguments[0] == 1 ? true : false;
                Application.Current.Dispatcher.BeginInvoke(
                DispatcherPriority.Background,
                new Action(() => ControlWidget.JoystickWidget.UpdateButtonData(11, ButtonPressed)));
            }
            if (message.Address.Equals("/Joystick/1/B12"))
            {
                bool ButtonPressed;
                ButtonPressed = (int)message.Arguments[0] == 1 ? true : false;
                Application.Current.Dispatcher.BeginInvoke(
                DispatcherPriority.Background,
                new Action(() => ControlWidget.JoystickWidget.UpdateButtonData(12, ButtonPressed)));
            }
        }

        public void DashboardDataRunner()
        {
            while (true)
            {
                Application.Current.Dispatcher.BeginInvoke(
                DispatcherPriority.Background,
                new Action(() => CurrentDashboardData.CurrentController = ControlWidget.ControllerCombobox.Text));
                Thread.Sleep(1);
            }
        }

        public void JoystickRunner()
        {
            while (true)
            {
                Application.Current.Dispatcher.BeginInvoke(
                DispatcherPriority.Background,
                new Action(() => ControlWidget.JoystickWidget.UpdateJoystickWidget(CurrentControllerData)));
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
