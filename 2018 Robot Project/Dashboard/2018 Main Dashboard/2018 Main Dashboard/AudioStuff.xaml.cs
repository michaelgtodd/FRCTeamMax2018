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
        SoundPlayer SanicMusic = new SoundPlayer("Sanic.wav");
        SoundPlayer PokeMusic = new SoundPlayer("Pokemon.wav");

        public AudioStuff()
        {
            InitializeComponent();
            
            SanicMusic.Load();
            SanicMusic.Play();
        }

        private void Mute(object sender, RoutedEventArgs e)
        {
            PokeMusic.Stop();
            SanicMusic.Stop();
        }

        private void Play(object sender, RoutedEventArgs e)
        {
            if (NameCombo.SelectedIndex == 2)
            {
                PokeMusic.Play();
            }
            else if (NameCombo.SelectedIndex == 0)
            {
                SanicMusic.Play();
            }
            
        }

        private void Sanic_Selected(object sender, RoutedEventArgs e)
        {
           // Music.SoundLocation = "Sanic.wav";

        }

        private void DOOM_Selected(object sender, RoutedEventArgs e)
        {

        }

        private void Pokemon_Selected(object sender, RoutedEventArgs e)
        {



        }

        private void NameCombo_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (NameCombo.SelectedIndex == 2)
            {
                SanicMusic.Stop();
                PokeMusic.Play();
                PokeMusic.Play();     
            }
            else if (NameCombo.SelectedIndex == 0)
            {
                PokeMusic.Stop();
                SanicMusic.Load();
                SanicMusic.Play();
            }
        }       
    }
}
