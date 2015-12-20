#include "statisticspage.h"
#include "ui_statisticspage.h"
#include "main.h"
#include "wallet.h"
#include "init.h"
#include "base58.h"
#include "clientmodel.h"
#include "bitcoinrpc.h"
#include <sstream>
#include <string>

using namespace json_spirit;


StatisticsPage::StatisticsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatisticsPage)
{
    ui->setupUi(this);
    setFixedSize(400, 420);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerCountDown()));
    timer->start(1000);


}

int heightPrevious = -1;
int connectionPrevious = -1;
int volumePrevious = -1;
double rewardPrevious = -1;
double netPawratePrevious = -1;
double pawratePrevious = -1;
double hardnessPrevious = -1;
double hardnessPrevious2 = -1;
int stakeminPrevious = -1;
int stakemaxPrevious = -1;
QString stakecPrevious = "";
int inc1 = 10;


void StatisticsPage::updateStatistics()
{
    double pHardness = GetDifficulty();
    double pHardness2 = GetDifficulty(GetLastBlockIndex(pindexBest, true));
    int pPawrate = GetPoWMHashPS();
    double pPawrate2 = 0.000;
    int nHeight = pindexBest->nHeight;
    double nSubsidy = nHeight;
    uint64_t nMinWeight = 0, nMaxWeight = 0, nWeight = 0;
    pwalletMain->GetStakeWeight(*pwalletMain, nMinWeight, nMaxWeight, nWeight);
    uint64_t nNetworkWeight = GetPoSKernelPS();
    int64_t volume = ((pindexBest->nMoneySupply)/100000000);
    int peers = this->model->getNumConnections();
    pPawrate2 = (double)pPawrate;
    QString height = QString::number(nHeight);
    QString stakemin = QString::number(nMinWeight);
    QString stakemax = QString::number(nNetworkWeight);
    QString phase = "";
    QString LastPoWBlock = QString::number(LAST_POW_BLOCK);

    ui->labelPhasePoW->setText("PoW = FINISHED");
    ui->labelPhasePoS->setText("PoS = 5% YEARLY");


    if (pindexBest->nHeight < LAST_POW_BLOCK)
    {
        phase = "Proof of Work";
        nSubsidy = nHeight ;
    }
    else
    {
        phase = "Proof of Stake";
        nSubsidy = 0 ;
    }

    QString subsidy = QString::number(nSubsidy, 'f', 6);
    QString hardness = QString::number(pHardness, 'f', 6);
    QString hardness2 = QString::number(pHardness2, 'f', 6);
    QString pawrate = QString::number(pPawrate2, 'f', 3);
    QString Qlpawrate = model->getLastBlockDate().toString();

    QString QPeers = QString::number(peers);
    QString qVolume = QString::number(volume);

    if(nHeight > heightPrevious)
    {
        ui->heightBox->setText("<b><font color=\"white\">" + height + "</font></b>");
    } else {
    ui->heightBox->setText("<font color=\"white\">" + height + "</font>");
    }

    if(0 > stakeminPrevious)
    {
        ui->minBox->setText("<b><font color=\"white\">" + stakemin + "</font></b>");
    } else {
    ui->minBox->setText("<font color=\"white\">" + stakemin + "</font>");
    }
    if(0 > stakemaxPrevious)
    {
        ui->maxBox->setText("<b><font color=\"white\">" + stakemax + "</font></b>");
    } else {
    ui->maxBox->setText("<font color=\"white\">" + stakemax + "</font>");
    }

    if(phase != stakecPrevious)
    {
        ui->cBox->setText("<b><font color=\"white\">" + phase + "</font></b>");
    } else {
    ui->cBox->setText("<font color=\"white\">" + phase + "</font>");
    }

    
    if(nSubsidy < rewardPrevious)
    {
        ui->rewardBox->setText("<b><font color=\"white\">" + subsidy + "</font></b>");
    } else {
    ui->rewardBox->setText("<font color=\"white\">" + subsidy + "</font>");
    }
    
    if(pHardness > hardnessPrevious)
    {
        ui->diffBox->setText("<b><font color=\"white\">" + hardness + "</font></b>");        
    } else if(pHardness < hardnessPrevious) {
        ui->diffBox->setText("<b><font color=\"white\">" + hardness + "</font></b>");
    } else {
        ui->diffBox->setText("<font color=\"white\">" + hardness + "</font>");        
    }

    if(pHardness2 > hardnessPrevious2)
    {
        ui->diffBox2->setText("<b><font color=\"white\">" + hardness2 + "</font></b>");
    } else if(pHardness2 < hardnessPrevious2) {
        ui->diffBox2->setText("<b><font color=\"white\">" + hardness2 + "</font></b>");
    } else {
        ui->diffBox2->setText("<font color=\"white\">" + hardness2 + "</font>");
    }
    
    if(pPawrate2 > netPawratePrevious)
    {
        ui->pawrateBox->setText("<b><font color=\"white\">" + pawrate + " MH/s</font></b>");
    } else if(pPawrate2 < netPawratePrevious) {
        ui->pawrateBox->setText("<b><font color=\"white\">" + pawrate + " MH/s</font></b>");
    } else {
        ui->pawrateBox->setText("<font color=\"white\">" + pawrate + " MH/s</font>");
    }

    if(Qlpawrate != pawratePrevious)
    {
        ui->localBox->setText("<b><font color=\"white\">" + Qlpawrate + "</font></b>");
    } else {
    ui->localBox->setText("<font color=\"white\">" + Qlpawrate + "</font>");
    }
    
    if(peers > connectionPrevious)
    {
        ui->connectionBox->setText("<b><font color=\"white\">" + QPeers + "</font></b>");             
    } else if(peers < connectionPrevious) {
        ui->connectionBox->setText("<b><font color=\"white\">" + QPeers + "</font></b>");        
    } else {
        ui->connectionBox->setText("<font color=\"white\">" + QPeers + "</font>");  
    }

    if(volume > volumePrevious)
    {
        ui->volumeBox->setText("<b><font color=\"white\">" + qVolume + " DRZ" + "</font></b>");
    } else if(volume < volumePrevious) {
        ui->volumeBox->setText("<b><font color=\"white\">" + qVolume + " DRZ" + "</font></b>");
    } else {
        ui->volumeBox->setText("<font color=\"white\">" + qVolume + " DRZ" + "</font>");
    }
    updatePrevious(nHeight, nMinWeight, nNetworkWeight, phase, nSubsidy, pHardness, pHardness2, pPawrate2, Qlpawrate, peers, volume);
}

void StatisticsPage::updatePrevious(int nHeight, int nMinWeight, int nNetworkWeight, QString phase, double nSubsidy, double pHardness, double pHardness2, double pPawrate2, QString Qlpawrate, int peers, int volume)
{
    heightPrevious = nHeight;
    stakeminPrevious = nMinWeight;
    stakemaxPrevious = nNetworkWeight;
    stakecPrevious = phase;
    rewardPrevious = nSubsidy;
    hardnessPrevious = pHardness;
    hardnessPrevious2 = pHardness2;
    netPawratePrevious = pPawrate2;
    pawratePrevious = Qlpawrate;
    connectionPrevious = peers;
    volumePrevious = volume;
}

void StatisticsPage::setModel(ClientModel *model)
{
    updateStatistics();
    this->model = model;

}


StatisticsPage::~StatisticsPage()
{
    delete ui;
}

void StatisticsPage::timerCountDown()
{
    inc1 = inc1 - 1;
    ui->labelCountDown->setText(QString::number(inc1) + "s");
    if (inc1 == 0)
    {
        updateStatistics();
        inc1 = 10;
    }
}


