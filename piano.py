import tkinter as tk
from pygame import mixer

class VirtualPiano:
    def __init__(self, root):
        self.root = root
        self.root.title("Virtual Piano")
        self.root.geometry("800x200")

        self.notes = ["C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B", "C2"]
        self.sounds = {note: mixer.Sound(f"sounds/{note}.mp3") for note in self.notes}

        self.create_keyboard()

    def play_key(self, note):
        self.sounds[note].play()

    def create_keyboard(self):
        for note in self.notes:
            key = tk.Button(self.root, text=note, width=5, height=2, command=lambda n=note: self.play_key(n))
            key.pack(side=tk.LEFT, padx=2)

if __name__ == "__main__":
    mixer.init()
    root = tk.Tk()
    app = VirtualPiano(root)
    root.mainloop()
