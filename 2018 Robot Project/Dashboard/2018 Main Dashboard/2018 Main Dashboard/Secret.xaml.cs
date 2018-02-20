using System;
using System.Collections.Generic;
using System.Linq;
using System.Media;
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
using System.Windows.Threading;

namespace _2018_Main_Dashboard
{
    /// <summary>
    /// Interaction logic for Secret.xaml
    /// </summary>
    public partial class Secret : UserControl
    {
        SoundPlayer SecretThing = new SoundPlayer("Music/SecretSong.wav");
        public Secret()
        {
            InitializeComponent();
        }

        private void SecretClick(object sender, RoutedEventArgs e)
        {
            
            SecretThing.Load();
            SecretThing.Play();
        }

        private void SecretButt_Click_1(object sender, RoutedEventArgs e)
        {

        }
    }
}
