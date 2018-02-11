﻿using System;
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
        //private List<string> ErrorList { get; set; }
        List<TextBlock> TaskNameList;
        List<TextBlock> TaskDurList;
        List<TextBlock> TaskFreqList;

        public AlertBox()
        {
            InitializeComponent();
            //ErrorList = new List<string>();            
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

        }

        //Public void UpdateCaution

        

        //public void UpdateError(string Error)
        //{
        //    if (ErrorList.Contains(Error))
        //    {
        //        return;

        //    }
        //    ErrorList.Add(Error);
        //    switch (ErrorList.Count)
        //    {
        //        case 1:
        //            Block1.Text = Error;
        //            break;
        //        case 2:
        //            Block2.Text = Error;
        //            break;
        //        case 3:
        //            Block3.Text = Error;
        //            break;
        //        case 4:
        //            Block4.Text = Error;
        //            break;
        //        case 5:
        //            Block5.Text = Error;
        //            break;
        //        case 6:
        //            Block6.Text = Error;
        //            break;
        //        case 7:
        //            Block7.Text = Error;
        //            break;
        //        case 8:
        //            Block8.Text = Error;
        //            break;
        //        case 9:
        //            Block9.Text = Error;
        //            break;
        //        default:
        //            break;

        //    }



        //}

        public void UpdateTaskData(TaskData DataForTasks)
        {
            for (int i = 0; i < 12; i++)
            {
                TaskNameList[i].Text = DataForTasks.NameString[i];
                TaskDurList[i].Text = DataForTasks.DurString[i];
                TaskFreqList[i].Text = DataForTasks.FreqString[i];
            }

        }

        public void UpdateCaution(string CautionType, string CautionName, bool CautionState)
        {
            //Motors    
            if (CautionType.Equals("Motor"))
            {
                if (CautionName.Equals("DriveTrainLeft1"))
                {
                    DriveMotorL1.Background = (CautionState == true) ? Brushes.Green : Brushes.Red;
                }

                if (CautionName.Equals("DriveTrainLeft2"))
                {
                    DriveMotorL2.Background = (CautionState == true) ? Brushes.Green : Brushes.Red;
                }

                if (CautionName.Equals("DriveTrainLeft3"))
                {
                    DriveMotorL3.Background = (CautionState == true) ? Brushes.Green : Brushes.Red;
                }

                if (CautionName.Equals("DriveTrainRight1"))
                {
                    DriveMotorR1.Background = (CautionState == true) ? Brushes.Green : Brushes.Red;
                }

                if (CautionName.Equals("DriveTrainRight2"))
                {
                     DriveMotorR2.Background = (CautionState == true) ? Brushes.Green : Brushes.Red;
                }

                if(CautionName.Equals("DriveTrainRight3"))
                {                   
                    DriveMotorR3.Background = (CautionState == true) ? Brushes.Green : Brushes.Red;
                }             
                
                if(CautionName.Equals("Elevator1"))
                {
                    ElevatorMotor1.Background = (CautionState == true) ? Brushes.Green : Brushes.Red;
                }

                if(CautionName.Equals(("Elevator2")))
                {                 
                    ElevatorMotor2.Background = (CautionState == true) ? Brushes.Green : Brushes.Red;
                }

                if(CautionName.Equals("ArmLeft"))
                {
                    ArmMotorL.Background = (CautionState == true) ? Brushes.Green : Brushes.Red;
                }

                if(CautionName.Equals("ArmRight"))
                {
                    ArmMotorR.Background = (CautionState == true) ? Brushes.Green : Brushes.Red;
                }

                if(CautionName.Equals("ArmLeftWheel"))
                {
                    ArmWheelMotorL.Background = (CautionState == true) ? Brushes.Green : Brushes.Red;
                }

                if(CautionName.Equals("ArmRightWheel"))
                {
                    ArmWheelMotorR.Background = (CautionState == true) ? Brushes.Green : Brushes.Red;
                }                
            }

            //Encoders
            if (CautionType.Equals("Encoder"))
            {
                if(CautionName.Equals("DriveTrainEncoderLeft"))
                {
                    DriveEncoderL.Background = (CautionState == true) ? Brushes.Green : Brushes.Red;
                }

                if(CautionName.Equals("DriveTrainRight"))
                {
                    DriveEncoderR.Background = (CautionState == true) ? Brushes.Green : Brushes.Red;
                }

                if(CautionName.Equals("Elevator"))
                {
                    ElevatorEncoder.Background = (CautionState == true) ? Brushes.Green : Brushes.Red;
                }

                if(CautionName.Equals("ArmLeft"))
                {
                    ArmEncoderL.Background = (CautionState == true) ? Brushes.Green : Brushes.Red;
                }

                if(CautionName.Equals("ArmRight"))
                {
                    ArmEncoderR.Background = (CautionState == true) ? Brushes.Green : Brushes.Red;
                }                                              
            }
            //Pressure
            if (CautionType.Equals("Pressure"))
            {
                LowPressure.Background = (CautionState == true) ? Brushes.Green : Brushes.Red;
            }
            //Tasks
            if (CautionType.Equals("Tasks"))
            {
                TaskPerformance.Background = (CautionState == true) ? Brushes.Green : Brushes.Red;
            }


        }

    }
}
