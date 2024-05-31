#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <string>
#include <filesystem>
#include <windows.h>
#include <vector>
#include <fstream>
#include <regex>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

union buf{unsigned long long number; char array[8];};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    //void updateTime();

    void on_lineEdit_textChanged(const QString &arg1);

    //void on_lineEdit_2_textChanged(const QString &arg1);

    //void on_lineEdit_5_textChanged(const QString &arg1);

    void on_lineEdit_2_textChanged(const QString &arg1);

    void on_lineEdit_5_textChanged(const QString &arg1);

    void on_lineEdit_4_textChanged(const QString &arg1);

    void on_lineEdit_3_textChanged(const QString &arg1);

    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    bool flag;

    unsigned long long time_in_seconds;
    union buf scalar;
    std::wstring mask, output_path;

    void cipherFiles();
    std::vector<std::wstring> getFiles(std::wstring input_mask);

    std::wstring getCounterName(std::wstring input);

};
#endif // MAINWINDOW_H
