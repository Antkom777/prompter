#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QSettings>
#include <QSlider>
#include <QString>
#include <QTextEdit>
#include <QTimer>
#include <QVBoxLayout>

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(const QString &executableName = "prompter",
                      QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void loadFile();
  void toggleScrolling();
  void updateScrollSpeed(int value);
  void updateFontSize(int value);
  void autoScroll();
  void resetScroll();

private:
  void setupUI();
  void updateSpeedLabel();
  void updateFontSizeLabel();
  void saveSettings();
  void loadSettings();
  void closeEvent(QCloseEvent *event) override;

  // UI Components
  QTextEdit *textDisplay;
  QPushButton *loadButton;
  QPushButton *startStopButton;
  QPushButton *resetButton;
  QSlider *speedSlider;
  QLabel *speedLabel;
  QSlider *fontSizeSlider;
  QLabel *fontSizeLabel;
  QLabel *statusLabel;

  QString lastFilePath;
  QString executableName; // Name of the executable (for settings isolation)

  // Scrolling control
  QTimer *scrollTimer;
  bool isScrolling;
  double scrollSpeed;       // pixels per scroll
  double scrollAccumulator; // for smooth fractional scrolling
  int currentFontSize;

  // Constants
  static constexpr int MIN_SPEED = 1;
  static constexpr int MAX_SPEED = 20;
  static constexpr int DEFAULT_SPEED = 5;
  static constexpr int MIN_FONT_SIZE = 12;
  static constexpr int MAX_FONT_SIZE = 22;
  static constexpr int DEFAULT_FONT_SIZE = 16;
  static constexpr int TIMER_INTERVAL = 20; // milliseconds
};

#endif // MAINWINDOW_H
