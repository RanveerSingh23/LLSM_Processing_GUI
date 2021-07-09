#include "matlabthreadmanager.h"
#include "mainwindow.h"




// Object that creates this thread is the parent
matlabThreadManager::matlabThreadManager(QObject *parent) :
    QThread(parent), outA(1)
{

}

matlabThreadManager::~matlabThreadManager(){
    for(auto thread : mThreads){
        if(!thread->isFinished()) thread->terminate();
    }
}

void matlabThreadManager::run(){


    // Threading and connecting signals/slots
    //mThreadManager = new matlabThreadManager(this);
    //connect(this, &MainWindow::jobStart, mThreadManager, &matlabThreadManager::onJobStart);
    //connect(mThreadManager, &matlabThreadManager::enableSubmitButton, this, &MainWindow::onEnableSubmitButton);
    //mThreadManager->start(QThread::HighestPriority);
    unsigned int mThreadID = 1;
    while(true){
    std::cout << "Ready for new job!" << std::endl;

    // Once outA is set to 0, we can create a new matlab thread for the job
    while(outA){
        sleep(1);
    }

    // Create new matlab thread
    mThreads.push_back(new matlabThread(this, funcType, outA, data, mainPath, mThreadID));
    mThreads.back()->start(QThread::TimeCriticalPriority);
    std::cout << "Matlab Job " << mThreadID << " Submitted" << std::endl;
    mThreadID++;
    outA = 1;
    data.clear();
    emit enableSubmitButton();
    }

}

// Sets data and outA (given by the GUI signal) when a job is about to start. This will let the MATLAB thread instantly start that job.
void matlabThreadManager::onJobStart(size_t &outA, std::vector<matlab::data::Array> &data, std::string &funcType, std::string &mainPath){
    std::cout << "Starting job" << std::endl;
    this->data = std::move(data);
    this->funcType = std::move(funcType);
    this->mainPath = std::move(mainPath);
    this->outA = std::move(outA);
}

