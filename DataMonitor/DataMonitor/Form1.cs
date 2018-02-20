using SharpOSC;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Threading;

namespace DataMonitor
{
    public partial class Form1 : Form
    {

        public void tryupdate(System.Windows.Forms.DataVisualization.Charting.Chart chart, string series, double value)
        {
            Random random = new Random();
            try
            {
                if (chart.Series[series].Points.Count > 300)
                    chart.Series[series].Points.RemoveAt(0);
            }
            catch { }
            try
            {
                chart.Series[series].Points.AddY(value);
            }
            catch
            {
                Console.WriteLine("Boo");
            }
        }


        public void OscReceiveRunner()
        {
            try
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
                    }
                    catch
                    {
                        Console.WriteLine("Unhandled Exception");
                    }
                }
            }
            catch { }
        }

        public void HandleOscPacket(OscMessage message)
        {
            if (message.Address.Contains("lefttargetvel"))
            {
                chart1.Invoke((MethodInvoker)delegate {
                    // Running on the UI thread
                    tryupdate(chart1,"Series1",(double) message.Arguments[0]);
                });
            }
            else if (message.Address.Contains("righttargetvel"))
            {
                chart2.Invoke((MethodInvoker)delegate {
                    // Running on the UI thread
                    tryupdate(chart2, "Series1", (double)message.Arguments[0]);
                });

            }
            else if (message.Address.Contains("leftactualvel"))
            {
                chart1.Invoke((MethodInvoker)delegate {
                    // Running on the UI thread
                    tryupdate(chart1, "Series2", (double)message.Arguments[0]);
                });
            }
            else if (message.Address.Contains("rightactualvel"))
            {
                chart1.Invoke((MethodInvoker)delegate {
                    // Running on the UI thread
                    tryupdate(chart2, "Series2", (double)message.Arguments[0]);
                });
            }
            else if (message.Address.Equals("/twist"))
            {
                chart1.Invoke((MethodInvoker)delegate {
                    // Running on the UI thread
                    tryupdate(chart3, "Series1", (double)message.Arguments[0]);
                });
            }
            else if (message.Address.Equals("/power/0"))
            {
                chart1.Invoke((MethodInvoker)delegate {
                    // Running on the UI thread
                    tryupdate(chart4, "Series1", (double)message.Arguments[0]);
                });
            }
            else if (message.Address.Equals("/power/1"))
            {
                chart1.Invoke((MethodInvoker)delegate {
                    // Running on the UI thread
                    tryupdate(chart5, "Series1", (double)message.Arguments[0]);
                });
            }
            else if (message.Address.Equals("/power/2"))
            {
                chart1.Invoke((MethodInvoker)delegate {
                    // Running on the UI thread
                    tryupdate(chart6, "Series1", (double)message.Arguments[0]);
                });
            }
            else if (message.Address.Equals("/power/15"))
            {
                chart1.Invoke((MethodInvoker)delegate {
                    // Running on the UI thread
                    tryupdate(chart7, "Series1", (double)message.Arguments[0]);
                });
            }
            else if (message.Address.Equals("/power/14"))
            {
                chart1.Invoke((MethodInvoker)delegate {
                    // Running on the UI thread
                    tryupdate(chart8, "Series1", (double)message.Arguments[0]);
                });
            }
            else if (message.Address.Equals("/power/13"))
            {
                chart1.Invoke((MethodInvoker)delegate {
                    // Running on the UI thread
                    tryupdate(chart9, "Series1", (double)message.Arguments[0]);
                });
            }
        }

        public Form1()
        {
            InitializeComponent();

            System.Threading.Thread OscReceive =
                new System.Threading.Thread
                    (new System.Threading.ThreadStart(OscReceiveRunner));
            OscReceive.Start();
        }

        private void chart2_Click(object sender, EventArgs e)
        {
        }

        private void chart1_Click(object sender, EventArgs e)
        {

        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }
    }
}
