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
using System.Windows.Shapes;

namespace _2018_Main_Dashboard
{
    /// <summary>
    /// Interaction logic for TasksMonitor.xaml
    /// </summary>
    public partial class TasksMonitor : Window
    {
        List<TextBlock> TaskNameList;
        List<TextBlock> TaskDurList;
        List<TextBlock> TaskFreqList;

        public TasksMonitor()
        {
            InitializeComponent();
            TaskNameList = new List<TextBlock>
            {
                Task1Name, Task2Name, Task3Name, Task4Name, Task5Name, Task6Name, Task7Name, Task8Name, Task9Name, Task10Name, Task11Name, Task12Name
            };
            TaskDurList = new List<TextBlock>
            {
                Task1Dur, Task2Dur, Task3Dur, Task4Dur, Task5Dur, Task6Dur, Task7Dur, Task8Dur, Task9Dur, Task10Dur, Task11Dur, Task12Dur
            };
            TaskFreqList = new List<TextBlock>
            {
                Task1Freq, Task2Freq, Task3Freq, Task4Freq, Task5Freq, Task6Freq, Task7Freq, Task8Freq, Task9Freq, Task10Freq, Task11Freq, Task12Freq
            };

            InitializeComponent();
           
        }
        public void UpdateTaskData(TaskData DataForTasks)
        {
            for (int i = 0; i< 12; i++)
            {
                TaskNameList[i].Text = DataForTasks.NameString[i];
                TaskDurList[i].Text = DataForTasks.DurString[i];
                TaskFreqList[i].Text = DataForTasks.FreqString[i];
            }

        }

    }
}
