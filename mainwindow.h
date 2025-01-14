#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "matlabthreadmanager.h"
#include "matlaboutputwindow.h"
#include "datapath.h"
#include <QMainWindow>
#include <QCloseEvent>
#include <QFileDialog>
#include <QDir>
#include <QFileInfo>
#include <QThread>
#include <QMessageBox>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// Class to hold advanced options from the user
class GUIvals{
    public:
        // Main Advanced Settings
        double skewAngle;
        double xyPixelSize;
        bool Reverse;
        bool sCMOSCameraFlip;

        // DSR Advanced Settings
        bool BKRemoval;
        double LowerLimit;
        QString resampleType;
        bool resampleEnabled;
        std::vector<double> resample;
        bool zarrFile;
        bool saveZarr;
        bool save3DStack;

        // Decon Advanced Settings
        QString cppDeconPath;
        QString loadModules;
        QString cudaDeconPath;
        QString OTFGENPath;
        QString RLMethod;
        bool fixIter;
        double errThresh;
        bool debug;
        bool gpuJob;
        QString largeMethod;

        // Job Advanced Settings
        bool largeFile;
        QString jobLogDir;
        QString uuid;
        uint64_t maxTrialNum;
        uint64_t unitWaitTime;
        uint64_t minModifyTime;
        uint64_t maxModifyTime;
        uint64_t maxWaitLoopNum;

        GUIvals() : skewAngle{32.45},
                    xyPixelSize{.108},
                    Reverse{true},
                    sCMOSCameraFlip{false},
                    BKRemoval{false},
                    LowerLimit{.4},
                    resampleType{"isotropic"},
                    resampleEnabled{false},
                    resample{1.0,1.0,1.0},
                    zarrFile{false},
                    saveZarr{false},
                    save3DStack{true},
                    cppDeconPath{"/global/home/groups/software/sl-7.x86_64/modules/RLDecon_CPU/20200718/build-cluster/cpuDeconv"},
                    loadModules{"module load gcc/4.8.5; module load fftw/3.3.6-gcc; module load boost/1.65.1-gcc; module load libtiff/4.1.0; "},
                    cudaDeconPath{"/global/home/groups/software/sl-7.x86_64/modules/cudaDecon/bin/cudaDeconv"},
                    OTFGENPath{"/global/home/groups/software/sl-7.x86_64/modules/cudaDecon/bin/radialft"},
                    RLMethod{"simplified"},
                    fixIter{false},
                    errThresh{.00000001},
                    debug{false},
                    gpuJob{false},
                    largeMethod{"inmemory"},
                    largeFile{false},
                    jobLogDir{"../job_logs"},
                    uuid{""},
                    maxTrialNum{3},
                    unitWaitTime{1},
                    minModifyTime{1},
                    maxModifyTime{10},
                    maxWaitLoopNum{10}
        {}
};

// Class to hold advanced options for Sim Recon
class simReconVals{
    public:
        // Main Advanced Settings
        double skewAngle;
        double xyPixelSize;
        bool Reverse;

        // Recon Advanced Settings
        bool islattice;
        double NA_det;
        double NA_ext;
        double nimm;
        double wvl_em;
        double wvl_ext;
        double w;
        bool normalize_orientations;
        QString resultsDirName;
        bool perdecomp;
        bool edgeTaper;
        double edgeTaperVal;
        double intThresh;
        double occThresh;
        bool useGPU;
        QString gpuPrecision;
        double Overlap;
        std::vector<double> ChunkSize;
        double reconBatchNum;

        // Job Advanced Settings
        QString jobLogDir;
        QString uuid;
        uint64_t maxTrialNum;
        uint64_t unitWaitTime;
        uint64_t parPoolSize;
        uint64_t maxModifyTime;

        simReconVals(): skewAngle{32.45},
                        xyPixelSize{.108},
                        Reverse{true},
                        islattice{true},
                        NA_det{1.0},
                        NA_ext{0.55},
                        nimm{1.33},
                        wvl_em{0.605},
                        wvl_ext{0.560},
                        w{5e-3},
                        normalize_orientations{false},
                        resultsDirName{"sim_recon"},
                        perdecomp{true},
                        edgeTaper{false},
                        edgeTaperVal{0.1},
                        intThresh{1},
                        occThresh{0.8},
                        useGPU{true},
                        gpuPrecision{"single"},
                        Overlap{128},
                        ChunkSize{32,32,32},
                        reconBatchNum{5},
                        jobLogDir{"../job_logs"},
                        uuid{""},
                        maxTrialNum{3},
                        unitWaitTime{2},
                        parPoolSize{24},
                        maxModifyTime{10}

        {}
};

// Class to hold advanced options for Sim Recon
class configFileVals{
public:
    // Config File Locations
    QString configFile;
    QString gpuConfigFile;

    // Config File Settings
    QString BashLaunchStr;
    bool GNUparallel;
    QString MCCMasterStr;
    QString MCRParam;
    double MemPerCPU;
    QString SlurmParam;
    uint64_t jobTimeLimit;
    bool masterCompute;
    uint64_t minCPUNum;
    uint64_t maxCPUNum;
    uint64_t maxJobNum;
    bool wholeNodeJob;

    configFileVals(): configFile{""},
        gpuConfigFile{""},
        BashLaunchStr(""),
        GNUparallel(true),
        MCCMasterStr(""),
        MCRParam(""),
        MemPerCPU(.01),
        SlurmParam(""),
        jobTimeLimit(12),
        masterCompute(true),
        minCPUNum(1),
        maxCPUNum(1),
        maxJobNum(5000),
        wholeNodeJob(false)

    {}
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool kill;

public slots:
    void onEnableSubmitButton();

signals:
    //void jobStart(size_t &outA, std::vector<matlab::data::Array> &data, QString &funcType, std::tuple<QString, QString, bool> &mPathJNameParseCluster, std::unordered_map<int,std::pair<QString,QDateTime>> &jobLogPaths);
    void jobStart(std::string &args, QString &funcType, std::tuple<QString, QString, bool> &mPathJNameParseCluster, std::unordered_map<int,std::pair<QString,QDateTime>> &jobLogPaths, bool isMcc, const std::string &pathToMatlab);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    // Using compiled scripts or matlab
    bool isMcc;
    std::string pathToMatlab;

    void writeSettings();
    void readMatlabPathSettings();
    void readConfigSettings();
    void readSettings();
    matlabThreadManager *mThreadManager;
    matlabOutputWindow *mOutputWindow;
    matlabOutputWindowThread *mOutputWindowThread;
    QMutex outputLock;
    GUIvals guiVals;
    std::vector<dataPath> dPaths;
    std::vector<std::pair<QLabel*,QCheckBox*>> channelWidgets;

    // Config Vals
    configFileVals cFileVals;

    // Sim Recon
    simReconVals simreconVals;
    std::vector<QString> simReconPsfFullPaths;
    std::vector<dataPath> simReconDPaths;
    std::vector<std::pair<QLabel*,QCheckBox*>> simReconChannelWidgets;

    // Crop
    std::vector<dataPath> cropDPaths;
    std::vector<std::pair<QLabel*,QCheckBox*>> cropChannelWidgets;

    // FFT Analysis
    std::vector<dataPath> fftAnalysisDPaths;

    // FSC Analysis
    std::vector<dataPath> fscAnalysisDPaths;
    std::vector<std::pair<QLabel*,QCheckBox*>> fscAnalysisChannelWidgets;

    // mipGenerator
    std::vector<dataPath> mipGeneratorDPaths;
    std::vector<std::pair<QLabel*,QCheckBox*>> mipGeneratorChannelWidgets;

    // psfDetectionAnalysis
    std::vector<dataPath> psfDetectionAnalysisDPaths;
    std::vector<std::pair<QLabel*,QCheckBox*>> psfDetectionAnalysisChannelWidgets;

    // tiffZarrConverter
    std::vector<dataPath> tiffZarrConverterDPaths;
    std::vector<std::pair<QLabel*,QCheckBox*>> tiffZarrConverterChannelWidgets;

    std::vector<QString> lsImagePaths;
    std::vector<QString> backgroundPaths;
    std::vector<QString> psfFullPaths;

    std::unordered_map<int,std::pair<QString,QDateTime>> jobLogPaths;
    std::unordered_map<int,QString> jobNames;

    bool loadSettings;
    void checkLoadPrevSettings();
    QString mostRecentDir;

private slots:
    void on_dsrAdvancedSettingsButton_clicked();

    void on_deconAdvancedSettingsButton_clicked();

    void on_jobAdvancedSettingsButton_clicked();

    void on_submitButton_clicked();

    void on_resultsDirBrowseButton_clicked();

    void on_resultsDirLineEdit_textChanged(const QString &arg1);

    void on_mainAdvancedSettingsButton_clicked();

    void on_deskewCheckBox_stateChanged(int arg1);

    void on_rotateCheckBox_stateChanged(int arg1);

    void on_deskewAndRotateCheckBox_stateChanged(int arg1);

    void on_stitchCheckBox_stateChanged(int arg1);

    void on_mainNextButton_clicked();

    void on_dsrPreviousButton_clicked();

    void on_dsrNextButton_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_objectiveScanCheckBox_stateChanged(int arg1);

    void on_stitchPreviousButton_clicked();

    void on_stitchNextButton_clicked();

    void on_deconPreviousButton_clicked();

    void on_deconNextButton_clicked();

    void on_jobPreviousButton_clicked();

    void on_addPathsButton_clicked();

    void on_imageListFullPathsBrowseButton_clicked();

    void on_imageListFullPathsLineEdit_textChanged(const QString &arg1);

    void on_boundBoxCheckBox_stateChanged(int arg1);

    void on_llffCorrectionCheckBox_stateChanged(int arg1);

    void on_lsImageAddPathsButton_clicked();

    void on_backgroundAddPathsButton_clicked();

    void on_psfFullAddPathsButton_2_clicked();

    void on_deconOnlyCheckBox_stateChanged(int arg1);

    void on_customPatternsCheckBox_stateChanged(int arg1);

    void on_simReconSubmitButton_clicked();

    void on_cropSubmitButton_clicked();

    void selectFolderPath();

    void on_simReconPsfFullAddPathsButton_clicked();

    void on_simReconCustomPatternsCheckBox_stateChanged(int arg1);

    void on_simReconMainAdvancedSettingsButton_clicked();

    void on_simReconReconAdvancedSettingsButton_clicked();

    void on_simReconJobAdvancedSettingsButton_clicked();

    void on_simReconMainNextButton_clicked();

    void on_simReconReconPreviousButton_clicked();

    void on_simReconReconNextButton_clicked();

    void on_simReconJobPreviousButton_clicked();

    void on_parallelRsyncSubmitButton_clicked();

    void on_parallelRsyncSourceBrowseButton_clicked();

    void on_parallelRsyncDestBrowseButton_clicked();

    void on_fftAnalysisSubmitButton_clicked();

    void on_fftAnalysisAddPathsButton_clicked();

    void on_fscAnalysisSubmitButton_clicked();

    void on_psfDetectionAnalysisSubmitButton_clicked();

    void on_tiffZarrConverterSubmitButton_clicked();

    void on_mipGeneratorSubmitButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
