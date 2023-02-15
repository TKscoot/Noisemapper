#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <random>

#include <QMainWindow>
#include <QDesktopWidget>
#include <QPushButton>
#include <QLabel>
#include <QLayout>
#include <QApplication>
#include <QStringListModel>
#include <DockManager.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class StringList : public QStringListModel
{
public:
  void append (const QString& string){
    insertRows(rowCount(), 1);
    setData(index(rowCount()-1), string);
  }
  StringList& operator<<(const QString& string){
    append(string);
    return *this;
  }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void resizeEvent(QResizeEvent* event);

public slots:
    void openFile();

private:
    Ui::MainWindow *ui;
    QLabel* l;
    int counter = 0;

    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen; // seed the generator
    std::uniform_int_distribution<> distrX; // define the range
    std::uniform_int_distribution<> distrY; // define the range

    // The main container for docking
    ads::CDockManager* m_DockManager;
};



#endif // MAINWINDOW_H
