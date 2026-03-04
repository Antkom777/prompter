#include "MainWindow.h"
#include <QCloseEvent>
#include <QFile>
#include <QFileDialog>
#include <QFont>
#include <QMessageBox>
#include <QScrollBar>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), isScrolling(false), scrollSpeed(DEFAULT_SPEED),
      scrollAccumulator(0.0), currentFontSize(DEFAULT_FONT_SIZE) {
  setupUI();

  // Set window to always stay on top
  setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

  // Initialize scroll timer
  scrollTimer = new QTimer(this);
  connect(scrollTimer, &QTimer::timeout, this, &MainWindow::autoScroll);

  setWindowTitle("Prompter");
  resize(800, 600);

  // Load saved settings
  loadSettings();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
  // Create central widget and main layout
  QWidget *centralWidget = new QWidget(this);
  QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

  // Text display area
  textDisplay = new QTextEdit(this);
  textDisplay->setReadOnly(true);
  QFont font = textDisplay->font();
  font.setPointSize(currentFontSize);
  textDisplay->setFont(font);
  textDisplay->setAlignment(Qt::AlignCenter);
  mainLayout->addWidget(textDisplay);

  // Control panel
  QHBoxLayout *controlLayout = new QHBoxLayout();

  // Load file button
  loadButton = new QPushButton("Load File", this);
  connect(loadButton, &QPushButton::clicked, this, &MainWindow::loadFile);
  controlLayout->addWidget(loadButton);

  // Start/Stop button
  startStopButton = new QPushButton("Start", this);
  startStopButton->setEnabled(false);
  connect(startStopButton, &QPushButton::clicked, this,
          &MainWindow::toggleScrolling);
  controlLayout->addWidget(startStopButton);

  // Reset button
  resetButton = new QPushButton("Reset", this);
  connect(resetButton, &QPushButton::clicked, this, &MainWindow::resetScroll);
  controlLayout->addWidget(resetButton);

  // Speed control
  QLabel *speedTextLabel = new QLabel("Speed:", this);
  controlLayout->addWidget(speedTextLabel);

  speedSlider = new QSlider(Qt::Horizontal, this);
  speedSlider->setMinimum(MIN_SPEED);
  speedSlider->setMaximum(MAX_SPEED);
  speedSlider->setValue(DEFAULT_SPEED);
  speedSlider->setTickPosition(QSlider::TicksBelow);
  speedSlider->setTickInterval(5);
  speedSlider->setMinimumWidth(200);
  connect(speedSlider, &QSlider::valueChanged, this,
          &MainWindow::updateScrollSpeed);
  controlLayout->addWidget(speedSlider);

  speedLabel = new QLabel(QString::number(DEFAULT_SPEED / 10.0, 'f', 1), this);
  speedLabel->setMinimumWidth(40);
  controlLayout->addWidget(speedLabel);

  // Font size control
  QLabel *fontTextLabel = new QLabel("Font:", this);
  controlLayout->addWidget(fontTextLabel);

  fontSizeSlider = new QSlider(Qt::Horizontal, this);
  fontSizeSlider->setMinimum(MIN_FONT_SIZE);
  fontSizeSlider->setMaximum(MAX_FONT_SIZE);
  fontSizeSlider->setValue(DEFAULT_FONT_SIZE);
  fontSizeSlider->setTickPosition(QSlider::TicksBelow);
  fontSizeSlider->setTickInterval(10);
  fontSizeSlider->setMinimumWidth(150);
  connect(fontSizeSlider, &QSlider::valueChanged, this,
          &MainWindow::updateFontSize);
  controlLayout->addWidget(fontSizeSlider);

  fontSizeLabel = new QLabel(QString::number(DEFAULT_FONT_SIZE), this);
  fontSizeLabel->setMinimumWidth(30);
  controlLayout->addWidget(fontSizeLabel);

  controlLayout->addStretch();

  // Status label
  statusLabel = new QLabel("No file loaded", this);
  controlLayout->addWidget(statusLabel);

  mainLayout->addLayout(controlLayout);

  setCentralWidget(centralWidget);
}

void MainWindow::loadFile() {
  QString fileName = QFileDialog::getOpenFileName(
      this, "Open Text File", "", "Text Files (*.txt);;All Files (*)");

  if (fileName.isEmpty()) {
    return;
  }

  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QMessageBox::warning(this, "Error",
                         "Cannot open file: " + file.errorString());
    return;
  }

  QTextStream in(&file);
  QString content = in.readAll();
  file.close();

  textDisplay->setPlainText(content);
  statusLabel->setText("File loaded: " + QFileInfo(fileName).fileName());
  startStopButton->setEnabled(true);
  lastFilePath = fileName;

  // Reset scroll position
  resetScroll();

  // Save the file path
  saveSettings();
}

void MainWindow::toggleScrolling() {
  if (isScrolling) {
    // Stop scrolling
    scrollTimer->stop();
    isScrolling = false;
    startStopButton->setText("Start");
    statusLabel->setText("Paused");
  } else {
    // Start scrolling
    scrollTimer->start(TIMER_INTERVAL);
    isScrolling = true;
    startStopButton->setText("Stop");
    statusLabel->setText("Scrolling...");
  }
}

void MainWindow::updateScrollSpeed(int value) {
  // Convert slider value (1-20) to actual speed (0.1-2.0 pixels per tick)
  scrollSpeed = value / 10.0;
  updateSpeedLabel();
  saveSettings();
}

void MainWindow::updateSpeedLabel() {
  speedLabel->setText(QString::number(scrollSpeed, 'f', 1));
}

void MainWindow::updateFontSize(int value) {
  currentFontSize = value;
  QFont font = textDisplay->font();
  font.setPointSize(currentFontSize);
  textDisplay->setFont(font);
  updateFontSizeLabel();
  saveSettings();
}

void MainWindow::updateFontSizeLabel() {
  fontSizeLabel->setText(QString::number(currentFontSize));
}

void MainWindow::autoScroll() {
  QScrollBar *scrollBar = textDisplay->verticalScrollBar();
  int currentValue = scrollBar->value();
  int maxValue = scrollBar->maximum();

  if (currentValue < maxValue) {
    // Accumulate fractional scrolling for smooth movement
    scrollAccumulator += scrollSpeed;
    int scrollAmount = static_cast<int>(scrollAccumulator);

    if (scrollAmount > 0) {
      scrollBar->setValue(currentValue + scrollAmount);
      scrollAccumulator -= scrollAmount;
    }
  } else {
    // Reached the end
    toggleScrolling();
    statusLabel->setText("Finished");
  }
}

void MainWindow::resetScroll() {
  QScrollBar *scrollBar = textDisplay->verticalScrollBar();
  scrollBar->setValue(0);
  scrollAccumulator = 0.0;

  if (isScrolling) {
    toggleScrolling();
  }

  if (!textDisplay->toPlainText().isEmpty()) {
    statusLabel->setText("Ready");
  }
}

void MainWindow::saveSettings() {
  QSettings settings("Prompter", "PrompterApp");

  // Save window geometry
  settings.setValue("geometry", saveGeometry());
  settings.setValue("windowState", saveState());

  // Save last file
  if (!lastFilePath.isEmpty()) {
    settings.setValue("lastFile", lastFilePath);
  }

  // Save speed and font size
  settings.setValue("scrollSpeed", speedSlider->value());
  settings.setValue("fontSize", currentFontSize);
}

void MainWindow::loadSettings() {
  QSettings settings("Prompter", "PrompterApp");

  // Restore window geometry
  if (settings.contains("geometry")) {
    restoreGeometry(settings.value("geometry").toByteArray());
  }
  if (settings.contains("windowState")) {
    restoreState(settings.value("windowState").toByteArray());
  }

  // Restore speed
  if (settings.contains("scrollSpeed")) {
    int speed = settings.value("scrollSpeed").toInt();
    speedSlider->setValue(speed);
    scrollSpeed = speed / 10.0;
    updateSpeedLabel();
  }

  // Restore font size
  if (settings.contains("fontSize")) {
    int fontSize = settings.value("fontSize").toInt();
    currentFontSize = fontSize;
    fontSizeSlider->setValue(fontSize);
    QFont font = textDisplay->font();
    font.setPointSize(currentFontSize);
    textDisplay->setFont(font);
    updateFontSizeLabel();
  }

  // Restore last file
  if (settings.contains("lastFile")) {
    QString fileName = settings.value("lastFile").toString();
    QFile file(fileName);
    if (file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text)) {
      QTextStream in(&file);
      QString content = in.readAll();
      file.close();

      textDisplay->setPlainText(content);
      statusLabel->setText("File loaded: " + QFileInfo(fileName).fileName());
      startStopButton->setEnabled(true);
      lastFilePath = fileName;
    }
  }
}

void MainWindow::closeEvent(QCloseEvent *event) {
  saveSettings();
  event->accept();
}
