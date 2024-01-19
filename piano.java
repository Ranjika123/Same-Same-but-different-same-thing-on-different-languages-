import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.layout.HBox;
import javafx.stage.Stage;
import javafx.scene.paint.Color;

import javax.sound.sampled.*;
import java.io.File;
import java.io.IOException;

public class VirtualPiano extends Application {
    private final String[] notes = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B", "C2"};
    private final String soundsPath = "sounds/";

    public static void main(String[] args) {
        launch(args);
    }

    @Override
    public void start(Stage primaryStage) {
        HBox keyboard = new HBox(5);

        for (String note : notes) {
            Button key = createKey(note);
            keyboard.getChildren().add(key);
        }

        Scene scene = new Scene(keyboard, 700, 120);
        primaryStage.setTitle("Virtual Piano");
        primaryStage.setScene(scene);
        primaryStage.show();
    }

    private Button createKey(String note) {
        Button key = new Button(note);
        key.setPrefSize(note.contains("#") ? 30 : 50, 100);
        key.setStyle("-fx-background-color: " + (note.contains("#") ? "black" : "white"));
        key.setOnAction(event -> playKey(note));
        return key;
    }

    private void playKey(String note) {
        try {
            File file = new File(soundsPath + note + ".wav");
            AudioInputStream audioInputStream = AudioSystem.getAudioInputStream(file);
            Clip clip = AudioSystem.getClip();
            clip.open(audioInputStream);
            clip.start();
            highlightKey(note);
        } catch (UnsupportedAudioFileException | IOException | LineUnavailableException e) {
            e.printStackTrace();
        }
    }

    private void highlightKey(String note) {
        for (Button key : getKeyboard().getChildren().toArray(new Button[0])) {
            if (key.getText().equals(note)) {
                key.setStyle("-fx-background-color: yellow");
                new Thread(() -> {
                    try {
                        Thread.sleep(300);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    key.setStyle("-fx-background-color: " + (note.contains("#") ? "black" : "white"));
                }).start();
                break;
            }
        }
    }

    private HBox getKeyboard() {
        Scene scene = primaryStage.getScene();
        if (scene != null) {
            return (HBox) scene.getRoot();
        }
        return null;
    }
}
