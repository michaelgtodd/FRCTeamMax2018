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
    /// Interaction logic for AudioStuff.xaml
    /// </summary>
    public partial class AudioStuff : UserControl
    {
        SoundPlayer SanicMusic = new SoundPlayer("Music/Sanic.wav");
        SoundPlayer PokeMusic = new SoundPlayer("Music/Pokemon.wav");
        SoundPlayer DoomMusic = new SoundPlayer("Music/DOOMfeld.wav");
        SoundPlayer SinnohCenter = new SoundPlayer("Music/SinnohSong.wav");

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
            DoomMusic.Stop();
            SinnohCenter.Stop();
        }
        private void Play(object sender, RoutedEventArgs e)
        {
            if (NameCombo.SelectedIndex == 0)
            {
                SanicMusic.Load();
                SanicMusic.Play();
            }

            else if (NameCombo.SelectedIndex == 1)
            {
                DoomMusic.Load();
                DoomMusic.Play();
            }

            else if (NameCombo.SelectedIndex == 2)
            {
                PokeMusic.Load();
                PokeMusic.Play();
            }

            else if (NameCombo.SelectedIndex == 3)
            {
                SinnohCenter.Load();
                SinnohCenter.Play();
            }
        }

        private void NameCombo_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {

                MainWindow MainApplication = Application.Current.MainWindow as _2018_Main_Dashboard.MainWindow;

                if (NameCombo.SelectedIndex == 0)
                {
                    DoomMusic.Stop();
                  SanicMusic.Stop();
                SinnohCenter.Stop();
                PokeMusic.Play();
                    PokeMusic.Play();

                Application.Current.Dispatcher.BeginInvoke(
                DispatcherPriority.Background,
                new Action(() => MainApplication.UpdateBackgroundImage("GreenHill.png")));
                }
                else if (NameCombo.SelectedIndex == 1)
                {
                    SanicMusic.Stop();
                    PokeMusic.Stop();
                SinnohCenter.Stop();
                DoomMusic.Load();
                    DoomMusic.Play();
                Application.Current.Dispatcher.BeginInvoke(
                DispatcherPriority.Background,
                new Action(() => MainApplication.UpdateBackgroundImage("DOOMfeld.jpg")));
                }
            else if (NameCombo.SelectedIndex == 2)
            {
                DoomMusic.Stop();
                SanicMusic.Stop();
                SinnohCenter.Stop();
                PokeMusic.Load();
                PokeMusic.Play();
                Application.Current.Dispatcher.BeginInvoke(
                DispatcherPriority.Background,
                new Action(() => MainApplication.UpdateBackgroundImage("PokemonTown.jpg")));
            }
            else if (NameCombo.SelectedIndex == 3)
            {
                DoomMusic.Stop();
                SanicMusic.Stop();
                PokeMusic.Stop();
                SinnohCenter.Load();
               SinnohCenter.Play();
                Application.Current.Dispatcher.BeginInvoke(
                DispatcherPriority.Background,
                new Action(() => MainApplication.UpdateBackgroundImage("Sinnoh4.png")));
            }


        }       
    }
}
