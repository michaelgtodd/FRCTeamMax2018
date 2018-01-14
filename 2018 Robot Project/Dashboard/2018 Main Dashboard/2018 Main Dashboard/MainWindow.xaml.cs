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
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public static void HandleOscPacket(OscMessage message)
        {
            if(message.Address.Equals("/info"))
            {
                Console.WriteLine("Received an info packet!");
            }
        }

        public static void OscThreadRunner()
        {
            var listener = new UDPListener(5801);
            OscMessage messageReceived = null;
            while (messageReceived == null)
            {
                messageReceived = (OscMessage)listener.Receive();
                HandleOscPacket(messageReceived);
                Thread.Sleep(1);
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
