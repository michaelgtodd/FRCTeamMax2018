using System.Media;
using System.Windows;
using System.Windows.Controls;

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
    }
}
