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

        public event EventHandler ControllerUpdateEvent;

        public void OscThreadRunner()
        {
            var listener = new UDPListener(5801);
            OscMessage messageReceived = null;
            while (true)
            {
                messageReceived = (OscMessage)listener.Receive();
                if (messageReceived != null)
                {
                    HandleOscPacket(messageReceived);
                }
                Thread.Sleep(1);
            }
        }

        public void HandleOscPacket(OscMessage message)
        {
            if (message.Address.Equals("/info"))
            {
                Console.WriteLine("Received an info packet!");
            }
            if (message.Address.Equals("/Joystick/1/XAxis"))
            {
                Application.Current.Dispatcher.BeginInvoke(
                DispatcherPriority.Background,
                new Action(() => ControlWidget.UpdateControllerData(JoystickAxis.XAxis, (double)message.Arguments[0])));
            }
            if (message.Address.Equals("/Joystick/1/YAxis"))
            {
                Application.Current.Dispatcher.BeginInvoke(
                DispatcherPriority.Background,
                new Action(() => ControlWidget.UpdateControllerData(JoystickAxis.YAxis, (double)message.Arguments[0])));
            }
            if (message.Address.Equals("/Joystick/1/ZAxis"))
            {
                Application.Current.Dispatcher.BeginInvoke(
                DispatcherPriority.Background,
                new Action(() => ControlWidget.UpdateControllerData(JoystickAxis.ZAxis, (double)message.Arguments[0])));
            }
            if (message.Address.Equals("/Error/Test"))
            {
                Application.Current.Dispatcher.BeginInvoke(
                DispatcherPriority.Background,
                new Action(() => AlertWidget.UpdateError((string)message.Arguments[0])));
            }
        }

        public MainWindow()
        {
            InitializeComponent();

            System.Threading.Thread oscThread =
                new System.Threading.Thread
                    (new System.Threading.ThreadStart(OscThreadRunner));
            oscThread.Start();
        }
    }
}
