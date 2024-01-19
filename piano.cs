using System;
using System.Collections.Generic;
using System.IO;
using System.Media;
using System.Windows.Forms;

namespace VirtualPiano
{
    public partial class VirtualPianoForm : Form
    {
        private Dictionary<string, SoundPlayer> sounds = new Dictionary<string, SoundPlayer>();

        public VirtualPianoForm()
        {
            InitializeComponent();
            LoadSounds();
            CreateKeyboard();
        }

        private void LoadSounds()
        {
            string[] notes = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B", "C2" };

            foreach (var note in notes)
            {
                string soundPath = Path.Combine("sounds", $"{note}.wav");
                sounds[note] = new SoundPlayer(soundPath);
            }
        }

        private void CreateKeyboard()
        {
            string[] notes = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B", "C2" };

            foreach (var note in notes)
            {
                Button key = new Button
                {
                    Text = note,
                    Height = 100,
                    Width = note.Contains("#") ? 30 : 50,
                    BackColor = note.Contains("#") ? System.Drawing.Color.Black : System.Drawing.Color.White,
                    ForeColor = System.Drawing.Color.Black,
                    Cursor = Cursors.Hand
                };

                key.Click += (sender, e) => PlayKey(note);
                keyboardPanel.Controls.Add(key);
            }
        }

        private void PlayKey(string note)
        {
            if (sounds.TryGetValue(note, out var player))
            {
                player.Stop();
                player.Play();
            }
        }

        private void VirtualPianoForm_KeyDown(object sender, KeyEventArgs e)
        {
            string key = e.KeyData.ToString().ToUpper();
            string note = Array.Find(new[] { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B", "C2" },
                                     n => n.StartsWith(key));

            if (note != null)
            {
                PlayKey(note);
                Button keyButton = keyboardPanel.Controls.Find(note, false)[0] as Button;
                keyButton.BackColor = System.Drawing.Color.Yellow;
                keyButton.Refresh();

                Timer timer = new Timer();
                timer.Interval = 300;
                timer.Tick += (s, ev) =>
                {
                    keyButton.BackColor = note.Contains("#") ? System.Drawing.Color.Black : System.Drawing.Color.White;
                    keyButton.Refresh();
                    timer.Stop();
                };

                timer.Start();
            }
        }
    }
}
