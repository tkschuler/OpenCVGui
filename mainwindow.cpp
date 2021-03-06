#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    timer = new QTimer(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_open_webcam_clicked()
{
    cap.open(0);

    if(!cap.isOpened())  // Check if we succeeded
    {
        cout << "camera is not open" << endl;
    }
    else
    {
        cout << "camera is open" << endl;

        connect(timer, SIGNAL(timeout()), this, SLOT(update_window()));
        timer->start(20);
    }
}

void MainWindow::on_pushButton_close_webcam_clicked()
{
    disconnect(timer, SIGNAL(timeout()), this, SLOT(update_window()));
    cap.release();

    Mat image = Mat::zeros(frame.size(),CV_8UC3);

    qt_image = QImage((const unsigned char*) (image.data), image.cols, image.rows, QImage::Format_RGB888);

    ui->label->setPixmap(QPixmap::fromImage(qt_image));

    ui->label->resize(ui->label->pixmap()->size());

    cout << "camera is closed" << endl;
}

void MainWindow::update_window()
{
    cap >> frame;

    cvtColor(frame, frame, CV_BGR2RGB);

    if (crosshair){
    line(frame, Point((frame.cols / 2) - 50, frame.rows / 2), Point((frame.cols / 2) + 50, frame.rows / 2), Scalar(0, 0, 255), 3);
    line(frame, Point(frame.cols / 2, (frame.rows / 2) - 50), Point(frame.cols / 2, (frame.rows / 2) + 50), Scalar(0, 0, 255), 3);
    }

    qt_image = QImage((const unsigned char*) (frame.data), frame.cols, frame.rows, QImage::Format_RGB888);

    ui->label->setPixmap(QPixmap::fromImage(qt_image));

    ui->label->resize(ui->label->pixmap()->size());
}

void MainWindow::on_pushButton_crosshair_clicked()
{
    if (crosshair){
        crosshair = false;
        cout << "Crosshair Enabled." << endl;
    }
    else{
        crosshair = true;
        cout << "Crosshair Disabled." << endl;
    }
}
