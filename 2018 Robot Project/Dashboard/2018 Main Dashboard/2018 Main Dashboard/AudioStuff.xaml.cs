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

namespace _2018_Main_Dashboard
{
    /// <summary>
    /// Interaction logic for AudioStuff.xaml
    /// </summary>
    public partial class AudioStuff : UserControl
    {
        SoundPlayer Music = new SoundPlayer("Music/Sanic.wav");
        

        public AudioStuff()
        {
            InitializeComponent();
            Music.Load();
            Music.Play();
        }

        private void Mute(object sender, RoutedEventArgs e)
        {
            Music.Stop();
        }

        private void Play(object sender, RoutedEventArgs e)
        {
            Music.Play();
        }
    }
}
