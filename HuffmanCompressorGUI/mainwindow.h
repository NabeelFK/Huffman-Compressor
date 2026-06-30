#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    void setSelectedFile(const QString &filePath);
    void updateButtons();

    Ui::MainWindow *ui;

    QString inputFilePath;
    QString outputFolderPath;
};

#endif // MAINWINDOW_H