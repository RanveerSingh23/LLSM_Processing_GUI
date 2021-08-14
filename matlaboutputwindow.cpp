#include <QLabel>
#include <QPushButton>
#include <QCloseEvent>
#include <QScrollArea>
#include <iostream>
#include "matlaboutputwindow.h"
#include "ui_matlaboutputwindow.h"
#include "jobtext.h"


matlabOutputWindow::matlabOutputWindow(std::unordered_map<int,std::string> &jobLogPaths, std::unordered_map<int,std::string> &jobNames, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::matlabOutputWindow)
{
    ui->setupUi(this);

    this->jobLogPaths = &jobLogPaths;
    this->jobNames = &jobNames;

    mOWThread = new matlabOutputWindowThread(jobLogPaths,this);
    connect(mOWThread, &matlabOutputWindowThread::updateOutputForm, this, &matlabOutputWindow::onUpdateOutputForm);
    mOWThread->start(QThread::NormalPriority);

    QWidget* containerMain = new QWidget(this);
    mainBox = outputBox(new QScrollArea(this),containerMain,new QVBoxLayout(containerMain));

    mainBox.container->setLayout(mainBox.vBox);
    mainBox.scrollArea->setWidget(mainBox.container);

    ui->verticalLayout_2->addWidget(mainBox.scrollArea);

}

matlabOutputWindow::~matlabOutputWindow()
{
    delete ui;
    mOWThread->terminate();
}

void matlabOutputWindow::closeEvent(QCloseEvent *event){
    event->ignore();
}

void matlabOutputWindow::onUpdateOutputForm(std::map<int,std::map<std::string,std::string>> *fNames, QMutex *fileNamesLock){
    fileNamesLock->lock();
    for(auto &path : *fNames){

        outputBox nBox;
        bool addLayout = false;
        if(buttons.find(path.first) == buttons.end()){
        addLayout = true;
        QWidget* container = new QWidget(this);
        nBox = outputBox(new QScrollArea(this),container,new QVBoxLayout(container));
        buttons.emplace(path.first,std::make_pair(nBox,std::unordered_map<std::string,QPushButton*>()));
        nBox.vBox->addStretch();
        }
        else nBox = buttons.at(path.first).first;

        for(auto &subPath : path.second){
           if(buttons.at(path.first).second.find(subPath.second) == buttons.at(path.first).second.end()){
               QPushButton* button = new QPushButton(this);
               QFileInfo filePath(QString::fromStdString(subPath.second));
               button->setText(filePath.fileName());
               button->setObjectName(filePath.absoluteFilePath());
               buttons.at(path.first).second.emplace(button->objectName().toStdString(),button);
               connect(button,&QPushButton::clicked,this,&matlabOutputWindow::onJobButtonClicked);
               nBox.vBox->insertWidget(nBox.vBox->count()-1,button);
           }
        }

        if(addLayout){
        nBox.container->setLayout(nBox.vBox);
        nBox.scrollArea->setWidget(nBox.container);

        mainBox.vBox->addWidget(new QLabel("<b>"+QString::fromStdString(jobNames->at(path.first))+"</b>",this));
        mainBox.vBox->addWidget(nBox.scrollArea);
        }
    }
    fileNamesLock->unlock();

}

void matlabOutputWindow::onJobButtonClicked(){
    QString button = ((QPushButton*)sender())->objectName();
    //QDesktopServices::openUrl(button);
    jobText *jText = new jobText(button,this);
    jText->setModal(false);
    jText->show();

}
