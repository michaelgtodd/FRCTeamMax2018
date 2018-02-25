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
        public string DriverController { get; set; }
        public string DriverMode { get; set; }
        public string AutoPosition { get; set; }
        public string AutoGoal { get; set; }
        public string SwitchesController { get; set; }
        public string SwitchesMode { get; set; }
    }

    public class ControllerData
    {
        public List<double> AxisList { get; set; }
        public List<bool> ButtonList { get; set; }
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


    public class TaskData
    {
        public List<string> NameString { get; set; }
        public List<string> DurString { get; set; }
        public List<string> FreqString { get; set; }
        public TaskData()
        {
            NameString = new List<string>
            {
                "", "", "", "", "", "", "", "", "","", "", ""
            };

            DurString = new List<string>
            {
                "", "", "", "", "", "", "", "", "","", "", ""
            };
            FreqString = new List<string>
            {
                "", "", "", "", "", "", "", "", "","", "", ""
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
        TaskData CurrentTaskData = new TaskData();


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
                        Thread.Sleep(1);
                    }
                    catch
                    {
                        Console.WriteLine("Unhandled Exception");
                    }
                }
            }
            catch { }
        }

        public void OscSendRunner()
        {
            while(true)
            { 
                UDPSender Sender = new SharpOSC.UDPSender("10.10.71.2", 5801);
                OscBundle ToSend = new SharpOSC.OscBundle(Utils.DateTimeToTimetag(DateTime.Now));
                OscMessage DriverController = new OscMessage("/Dashboard/DriverController/", CurrentDashboardData.DriverController);
                ToSend.Messages.Add(DriverController);
                OscMessage DriveMode = new OscMessage("/Dashboard/DriveMode/", CurrentDashboardData.DriverMode);
                ToSend.Messages.Add(DriveMode);
                OscMessage AutoPositionMessage = new OscMessage("/Dashboard/AutoPositionMessage/", CurrentDashboardData.AutoPosition);
                ToSend.Messages.Add(AutoPositionMessage);
                OscMessage AutoGoalMessage = new OscMessage("/Dashboard/AutoSwitchPriority/", CurrentDashboardData.AutoGoal);
                ToSend.Messages.Add(AutoGoalMessage);
                OscMessage SwitchesController = new OscMessage("/Dashboard/SwitchesController/", CurrentDashboardData.SwitchesController);
                ToSend.Messages.Add(SwitchesController);
                OscMessage SwitchesMode = new OscMessage("/Dashboard/SwitchesMode/", CurrentDashboardData.SwitchesMode);
                ToSend.Messages.Add(SwitchesMode);
                Sender.Send(ToSend);
                Thread.Sleep(50);
            }
        }


        public void HandleOscPacket(OscMessage message)
        {
            if (message.Address.Contains("Caution"))
            {
                string[] CautionArray = message.Address.Split('/');
                
                

                int CautionInteger = (int)message.Arguments[0];
                bool CautionState = (CautionInteger == 1) ? true : false;

                 Application.Current.Dispatcher.BeginInvoke(
                DispatcherPriority.Background,
                new Action(() => AlertWidget.UpdateCaution(CautionArray[2],CautionArray[3],CautionState)));
            }
            

            if (message.Address.Contains("Joystick"))
            {
                string[] JoystickStringArray = message.Address.Split('/');
                if (JoystickStringArray[2].Equals("0"))
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
            if (message.Address.Contains("TaskStats"))
            {
                string[] TaskStatsStringArray = message.Address.Split('/');
                int TaskNumber = int.Parse(TaskStatsStringArray[2]) - 1;

                if (TaskStatsStringArray[3].Equals("Name"))
                {
                    CurrentTaskData.NameString[TaskNumber] = (string)message.Arguments[0];
                }
                if (TaskStatsStringArray[3].Equals("Duration"))
                {
                    CurrentTaskData.DurString[TaskNumber] = ((int)message.Arguments[0]).ToString();
                }
                if (TaskStatsStringArray[3].Equals("Period"))
                {
                    CurrentTaskData.FreqString[TaskNumber] = ((int)message.Arguments[0]).ToString();
                }

            }
            if (message.Address.Equals("/Power/Total/"))
            {
                Application.Current.Dispatcher.BeginInvoke(
               DispatcherPriority.Background,
               new Action(() => CurrentGaugeWidget.ChangeWidth((double)message.Arguments[0])));
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
                try
                {
                    Application.Current.Dispatcher.BeginInvoke(
                    DispatcherPriority.Background,
                    new Action(() => CurrentDashboardData.DriverController = DriverWidget.ControllerCombobox.Text));
                    Application.Current.Dispatcher.BeginInvoke(
                    DispatcherPriority.Background,
                    new Action(() => CurrentDashboardData.DriverMode = (DriverWidget.TankCheckbox.IsChecked == true) ? "Tank" : "Arcade"));
                    Application.Current.Dispatcher.BeginInvoke(
                    DispatcherPriority.Background,
                    new Action(() => CurrentDashboardData.AutoGoal = AutoSwitch.Goal));
                    Application.Current.Dispatcher.BeginInvoke(
                    DispatcherPriority.Background,
                    new Action(() => CurrentDashboardData.AutoPosition = AutoSwitch.Position));
                    Application.Current.Dispatcher.BeginInvoke(
                    DispatcherPriority.Background,
                    new Action(() => CurrentDashboardData.SwitchesController = SwitchesWidget.ControllerCombobox.Text));
                    Application.Current.Dispatcher.BeginInvoke(
                    DispatcherPriority.Background,
                    new Action(() => CurrentDashboardData.SwitchesMode = (SwitchesWidget.TankCheckbox.IsChecked == true) ? "Tank" : "Arcade"));
                    Thread.Sleep(1);
                }
                catch (NullReferenceException) { }
            }
        }

        public void TaskRunner()
        {
            while (true)
            {
                try
                {
                    Application.Current.Dispatcher.BeginInvoke(
                    DispatcherPriority.Background,
                    new Action(() => AlertWidget.UpdateTaskData(CurrentTaskData)));
                    Thread.Sleep(1);
                }
                catch(NullReferenceException) {}
            }

        }

        public void JoystickRunner()
        {
            while (true)
            {
                    Application.Current.Dispatcher.BeginInvoke(
                  DispatcherPriority.Background,
                  new Action(() => DriverWidget.UpdateJoystickWidget(ControllerData0, ControllerData1)));
                    Application.Current.Dispatcher.BeginInvoke(
                  DispatcherPriority.Background,
                  new Action(() => SwitchesWidget.UpdateJoystickWidget(ControllerData2, ControllerData3)));
                    Thread.Sleep(1);
            }
        }

        public void UpdateBackgroundImage(string BackgroundImagePath)
        {
            Uri BackgroundImageData = new Uri(BackgroundImagePath, UriKind.Relative);
            BitmapImage BackgroundImage = new BitmapImage(BackgroundImageData);
            ThemeImage.Source = BackgroundImage;
        }

        public MainWindow()
        {
            InitializeComponent();
            Application.Current.Dispatcher.BeginInvoke(
               DispatcherPriority.Background,
               new Action(() => SwitchesWidget.UpdateSwitchesName("Switches Controller Selection")));

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
                new System.Threading.Thread(new System.Threading.ThreadStart(JoystickRunner));
            UpdateJS.Start();

            System.Threading.Thread UpdateAlerts =
                new System.Threading.Thread(new System.Threading.ThreadStart(TaskRunner));
            UpdateAlerts.Start();
        }
    }
}
