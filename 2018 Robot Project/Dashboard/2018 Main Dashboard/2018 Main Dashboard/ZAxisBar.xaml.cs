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
    /// Interaction logic for ZAxisBar.xaml
    /// </summary>
    public partial class ZAxisBar : UserControl
    {
        public ZAxisBar()
        {
            InitializeComponent();
        }
        public void ChangeZAxisWidth(double ZAxis, string Controller)
        {
            double ZAxisWidth;
            if (Controller == "Xbox Controller")
            {
                ZAxisWidth = ZAxis * 50;
                if (ZAxisWidth < 0)
                    ZAxisWidth = 0;
                ZAxisIndicator.Width = ZAxisWidth;
            }
            else if (Controller == "Joystick")
            {
                ZAxisWidth = ZAxis * 25 + 25;
                ZAxisIndicator.Width = ZAxisWidth;
            }
        }

    }
}
