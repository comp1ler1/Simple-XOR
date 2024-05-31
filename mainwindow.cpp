#include "mainwindow.h"
#include "ui_mainwindow.h"


void MainWindow::updateTimer(){
    std::cout << "Time is over" << std::endl;

    on_pushButton_clicked();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineEdit_3->setValidator( new QIntValidator);
    //ui->lineEdit_4->setValidator( new QIntValidator );
    ui->lineEdit_4->setInputMask("hhhhhhhhhhhhhhhh");

    timer = new QTimer(this);
    //timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTimer()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


std::wstring MainWindow::getCounterName(std::wstring input){

    std::wregex rgx(L"^.*([(][0-9]*[)][.])");
    //std::wregex rgx2(L"[(][0-9]*[)][.]");
    //std::wregex rgx3(L"^.*([.*][(][0-9]*[)])");
    std::wregex rgx4(L"^.*([.].*)");

    std::wcmatch temp;
    std::regex_search(input.c_str(), temp, rgx);
    std::wstring output;
    if(!temp.empty()){
        std::wcout << temp[temp.size() - 1] << std::endl;

        auto number = stoi(temp[temp.size() - 1].str().substr(1, temp[temp.size() - 1].str().size() - 3));

        std::wcout << number << std::endl;

        auto numberPrev = number;
        number += 1;
        output = input;

        std::wstring wnumber = std::to_wstring(numberPrev);
        wnumber.insert(0, L"(");
        wnumber.append(L").");

        std::wstring outputResult = std::regex_replace(output, rgx, wnumber);

        auto inputLen = outputResult.length();

        wnumber = std::to_wstring(number);
        wnumber.insert(0, L"(");
        wnumber.append(L").");

        outputResult = std::regex_replace(output, rgx, wnumber);


        output.erase(output.size() - inputLen, output.size() - 1);
        output.append(outputResult);

        std::wcout << output << std::endl;
    }else{
        std::regex_search(input.c_str(), temp, rgx4);
        std::wcout << temp[temp.size() - 1] << std::endl;
        output = input;
        output.insert(output.length() - temp[temp.size() - 1].length(), L"(1)");
        std::wcout << output << std::endl;
    }
    return output;
}


std::vector<std::wstring> MainWindow::getFiles(std::wstring input_mask){

    std::vector<std::wstring> files;

    WIN32_FIND_DATAW findFileData;
    HANDLE hFind = FindFirstFileW(input_mask.c_str(), &findFileData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (findFileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY) {
                //std::wcout << findFileData.cAlternateFileName << std::endl;
                files.push_back(findFileData.cFileName);
            }
        } while (FindNextFile(hFind, &findFileData) != 0);
        FindClose(hFind);
    }

    return files;
}


void MainWindow::cipherFiles(){

    std::vector<std::wstring> files = getFiles(mask);

    for(size_t i = 0; i < files.size(); i++){
        //std::ifstream in(files[i].c_str());

        HANDLE fh;
        fh = CreateFileW( files[i].c_str(), GENERIC_WRITE, 0, NULL,
                         CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

        if (ERROR_SHARING_VIOLATION != GetLastError())
        {
           //std::wcout << L"File " << files[i] << L" is not open" << std::endl;

           CloseHandle(fh);

           union buf a;

           std::wstring out_file = files[i];
           std::ifstream in;
           std::ofstream out;

           std::wstring tempString = output_path;
           if(ui->comboBox->currentIndex() == 1){

               tempString.append(files[i]);
               std::wcout << tempString << std::endl;

               while(1){
                   std::vector<std::wstring> temp_vector = getFiles(tempString);
                   std::wstring tempPath = output_path;

                   if(temp_vector.empty()){
                        break;
                       //std::wcout << tempPath << std::endl;

                       //std::filesystem::copy_file(files[i].c_str(), tempString.c_str());
                   }else{
                       //tempPath.append(files[i]);
                       std::wstring newFile = getCounterName(temp_vector[temp_vector.size() - 1]);

                       tempPath.append(newFile);
                       tempString = tempPath;
                       std::cout << "Oops" << std::endl;
                   }
               }
               std::wcout << tempString << std::endl;
               //std::filesystem::CopyFileW(files[i], tempString);

               in.open(files[i].c_str(), std::ios::binary);
               out.open(tempString.c_str(), std::ios::binary);

               while(!in.eof()){
                    //in.get(a.array, n);
                   int j = 0;
                   for (;j < 8 && in.tellg() != -1; ++j){
                        in.read(a.array + j, 1);
                        a.array[j] ^= scalar.array[7 - j];
                   }

                    if(!in.eof())
                        out.write(a.array, j);
                    else
                        out.write(a.array, j - 1);
               }

               in.close();
               out.close();
           }
           else{
               tempString = files[i];
               std::fstream file(files[i].c_str(), std::ios::in | std::ios::out | std::ios::binary);

               unsigned long long count = 0;
               while(!file.eof()){
                    //in.get(a.array, n);
                   int j = 0;
                   for (;j < 8 && file.tellg() != -1; ++j){
                        file.read(a.array + j, 1);
                        a.array[j] ^= scalar.array[7 - j];
                   }

                    if(!file.eof()){
                        count += j;
                        file.seekp(count - j);
                        file.write(a.array, j);
                    }
                    else{
                        file.clear();
                        count += j;
                        file.seekp(count - j);
                        file.write(a.array, j - 1);
                        break;
                    }
               }

               file.close();
               //continue;

           }

            if(ui->checkBox->checkState()){
                const std::string s2(files[i].begin(), files[i].end());
                remove(s2.c_str());
            }

        }

    }
}

void MainWindow::on_pushButton_clicked()
{
    std::cout << "Button is clicked" << std::endl;
    std::cout << ui->comboBox_2->currentIndex() << std::endl;
    if(ui->comboBox_2->currentIndex()){
        timer->setInterval(1000 * time_in_seconds);
        timer->start();
    }else{
        timer->stop();
    }

    if(output_path.empty()){
        output_path = std::filesystem::current_path();
        //std::wcout << output_path << std::endl;
        output_path.append(L"\\\\");
    }

    //std::filesystem::copy_file("test.txt", "test1.txt");

    //std::wcout << std::filesystem::current_path() << std::endl;
    //std::wcout << output_path << std::endl;

    cipherFiles();
}


void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    std::cout << arg1.toStdString() << std::endl;
    mask = arg1.toStdWString();
}


void MainWindow::on_lineEdit_5_textChanged(const QString &arg1)
{
    std::cout << arg1.toStdString() << std::endl;
    output_path = arg1.toStdWString();
}


void MainWindow::on_lineEdit_4_textChanged(const QString &arg1)
{
    //bool flag;
    //scalar = arg1.toUInt(&flag, 10);
    scalar.number = arg1.toULongLong(&flag, 16);
    std::cout << arg1.toStdString() << std::endl;
    std::cout << scalar.number << std::endl;
    //for(int i = 0; i < 8; i++){
    //    std::cout << scalar.array[i] << std::endl;
    //}
}


void MainWindow::on_lineEdit_3_textChanged(const QString &arg1)
{
    //bool flag;
    time_in_seconds = arg1.toUInt(&flag, 10);
    std::cout << time_in_seconds << std::endl;
}


void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    std::cout << "Current index: " << index << std::endl;
    std::cout << ui->comboBox->currentIndex() << std::endl;
}



void MainWindow::on_pushButton_2_clicked()
{
    timer->stop();
}


void MainWindow::on_pushButton_3_clicked()
{
    timer->start();
}

