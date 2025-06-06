#include "JaratWrapper.h"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include "memtrace.h"


JaratWrapper::~JaratWrapper() {}

void JaratWrapper::DelJaratWrapper() {
    if (this->nextNode != nullptr) {
        this->nextNode->DelJaratWrapper();
    }
    delete this->currJarat;
    delete this;
}

void JaratWrapper::PrintJarat() {
    JaratWrapper* tmp = this;
    while (tmp != nullptr) {
        tmp->currJarat->PrintJarat();
        tmp->currJarat->getFirstMenetrend()->printAllInfo();
        tmp->currJarat->getJVonat()->printVonatStat();

        tmp = tmp->nextNode;
    }
}

Jarat * JaratWrapper::getJarat() {
    return currJarat;
}

JaratWrapper * JaratWrapper::getNextNode() {
    return nextNode;
}

JaratWrapper * JaratWrapper::InitJaratWrapper(Allomas* FirstAllomas, Vonat** VonatArray) {
    JaratWrapper* ElsoJaratWrapper = nullptr;
    bool ElsoJaratWrapperExists = false;
    std::string JaratNev;
    int VonatSzam;
    int MenetrendID;
    std::string AllomasNev;
    int ErkezOra;
    int ErkezPerc;
    int IndulOra;
    int IndulPerc;


    bool ElsoMenetrendExist = false;
    int MenetrendReadIncrement = 0;
    std::ifstream file("JaratWrapper.dat");
    if (!file.is_open()) {
        throw "JaratWrapper file could not be opened!";
    }
    std::string line;
    std::string tmp;
    while (std::getline(file, line)) {

        Menetrend* ELsoMenetrend;
        int ReadIncrement = 0;
        std::stringstream ss(line);
        while (getline(ss, tmp, ';')) {
            if (ReadIncrement == 0) {
                JaratNev = tmp;
                ReadIncrement++;
            } else if (ReadIncrement == 1) {
                VonatSzam = std::stoi(tmp);
                ReadIncrement++;
            } else if (ReadIncrement == 2) {
                if (MenetrendReadIncrement == 0) {
                    MenetrendID = std::stoi(tmp);
                    MenetrendReadIncrement++;
                }else if (MenetrendReadIncrement == 1) {
                    AllomasNev = tmp;
                    MenetrendReadIncrement++;
                } else if (MenetrendReadIncrement == 2) {
                    ErkezOra = std::stoi(tmp);
                    MenetrendReadIncrement++;
                } else if (MenetrendReadIncrement == 3) {
                    ErkezPerc = std::stoi(tmp);
                    MenetrendReadIncrement++;
                } else if (MenetrendReadIncrement == 4) {
                    IndulOra = std::stoi(tmp);
                    MenetrendReadIncrement++;
                } else if (MenetrendReadIncrement == 5) {
                    IndulPerc = std::stoi(tmp);
                    if (!ElsoMenetrendExist) {
                        Ido *ErkezIdo = new Ido(ErkezOra,ErkezPerc);
                        Ido *IndulIdo = new Ido(IndulOra,IndulPerc);
                        ELsoMenetrend = new Menetrend(FirstAllomas->FindAllomasByName(AllomasNev),MenetrendID,ErkezIdo, IndulIdo);
                        ElsoMenetrendExist = true;
                    } else {
                        Ido *ErkezIdo = new Ido(ErkezOra,ErkezPerc);
                        Ido *IndulIdo = new Ido(IndulOra,IndulPerc);
                        Menetrend* NextMenetrend = new Menetrend(FirstAllomas->FindAllomasByName(AllomasNev),MenetrendID,ErkezIdo, IndulIdo);
                        ELsoMenetrend->operator+(NextMenetrend);

                    }
                    MenetrendReadIncrement = 0;
                }
            }
        }
        if (!ElsoJaratWrapperExists) {
            Jarat * JaratToAdd = new Jarat(JaratNev,Vonat::FindVonatByNumber(VonatArray, VonatSzam),ELsoMenetrend);
            ElsoJaratWrapper = new JaratWrapper(JaratToAdd);
            ElsoJaratWrapperExists = true;
        } else {
            Jarat * JaratToAdd = new Jarat(JaratNev,Vonat::FindVonatByNumber(VonatArray, VonatSzam), ELsoMenetrend);
            JaratWrapper* NextJaratWrapper = new JaratWrapper(JaratToAdd);
            ElsoJaratWrapper->operator+(NextJaratWrapper);

        }
        ElsoMenetrendExist = false;
    }
    file.close();
    return ElsoJaratWrapper;
}



void JaratWrapper::operator+(JaratWrapper *newJaratWrapper) {
        if (newJaratWrapper == nullptr)
            throw "Previous or new JaratWrapper does not exist";
        JaratWrapper *tmp = this;
        while (tmp->getNextNode() != nullptr)
        {
            tmp = tmp->getNextNode();
        }
        tmp->nextNode = newJaratWrapper;
}
Jarat * JaratWrapper::FindJaratByName(const std::string &Nev) {
    JaratWrapper* tmp = this;
    while (tmp != nullptr) {
        if (tmp->getJarat()->getJaratName() == Nev) {
            return tmp->getJarat();
        }
        tmp = tmp->getNextNode();
    }
    throw "Jarat with that Name does not exist!";
}

void JaratWrapper::AddKeses() {
    std::string JNev;
    std::cout << "Jarat Neve:\n";
    std::cin >> JNev;
    try {
        FindJaratByName(JNev);
    }catch (const char* e) {
        std::cout << e;
    }
    std::string SPerc;
    std::cout << "Keses (Perc):";
    std::cin >> SPerc;
    int Keses = stoi(SPerc);
    if (Keses <= 0 || Keses > 60) {
        throw "Invalid number for Keses!";
    }
    FindJaratByName(JNev)->getFirstMenetrend()->setKesesToALl(Keses);
}
