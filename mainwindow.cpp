#include "mainwindow.h"
#include "glviewportwidget.h"
#include "ui_mainwindow.h"
#include <QScreen>
#include <QDebug>
#include <QListView>
#include <QScrollArea>
#include <QFileDialog>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create the dock manager after the ui is setup. Because the
    // parent parameter is a QMainWindow the dock manager registers
    // itself as the central widget as such the ui must be set up first.
    m_DockManager = new ads::CDockManager(this);

    ////ui->tabWidget->setGeometry(0, 0, 5, this->size().height());
    ////ui->mainSplitter->setChildrenCollapsible(false);

    GLViewportWidget* viewportWidget = new GLViewportWidget();
    viewportWidget->setObjectName("viewportWidget");
    viewportWidget->setAccessibleName("viewportWidget");

    //ui->mainSplitter->insertWidget(1, viewportWidget);
    //ui->mainSplitter->setStretchFactor(0, 1);
    //ui->mainSplitter->setStretchFactor(1, 1);
    //ui->mainSplitter->setSizes(QList<int>() << this->size().width() * 0.2f << this->size().width() * 0.8f);

    QObject::connect(ui->openFileButton, SIGNAL(clicked()), this, SLOT(openFile()));

    // Create a dock widget with the title Label 1 and set the created label
    // as the dock widget content
    ads::CDockWidget* DockWidget = new ads::CDockWidget("Inspector");
    DockWidget->setWidget(ui->fieldsWidget);

    ads::CDockWidget* DockWidget1 = new ads::CDockWidget("Viewport");
    DockWidget1->setWidget(viewportWidget);

    //// Add the toggleViewAction of the dock widget to the menu to give
    //// the user the possibility to show the dock widget if it has been closed
    ui->windowsMenu->addAction(DockWidget->toggleViewAction());
    ui->windowsMenu->addAction(DockWidget1->toggleViewAction());

    // Add the dock widget to the top dock widget area
    m_DockManager->addDockWidget(ads::LeftDockWidgetArea, DockWidget);

    m_DockManager->addDockWidget(ads::RightDockWidgetArea, DockWidget1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

}

void MainWindow::openFile()
{
    QString filePath =  QFileDialog::getOpenFileName(
          this,
          "Open Document",
          QDir::currentPath(),
          "All files (*.*) ;; Document files (*.doc *.rtf);; PNG files (*.png)");

    if(filePath.isNull())
    {

      return;
    }

    qWarning() << "selected file path : " << filePath.toUtf8();

    QFileInfo fileInfo(filePath);
    ui->fileLabel->setText(fileInfo.fileName());

}

