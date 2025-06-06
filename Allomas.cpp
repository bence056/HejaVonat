#include "Allomas.h"
#include <string>
#include <fstream>
#include <sstream>
#include "memtrace.h"

void Allomas::DelAllomas()  {
    if (this->nextNode != nullptr) {
        this->nextNode->DelAllomas();
    }
    delete this;
}


std::string Allomas::getAllomasNev() const
{
    return Nev;
}

double Allomas::getX() const
{
    return Szelesseg;
}

double Allomas::getY() const
{
    return Magassag;
};

bool Allomas::getWc() const
{
    return Wc;
};

bool Allomas::getBufe() const
{
    return Bufe;
}
Allomas *Allomas::getNextNode()
{
    return nextNode;
}

void Allomas::setNextNode(Allomas *nextNode) {
    this->nextNode = nextNode;
}

void Allomas::operator+(Allomas *newAllomas)
{
    if (newAllomas == nullptr)
        throw "Previous or new Allomas does not exist";

    Allomas *tmp = this;
    while (tmp->getNextNode() != nullptr)
    {
        tmp = tmp->getNextNode();
    }
    tmp->nextNode = newAllomas;
}
Allomas* Allomas::AllomasDelete(Allomas *AllomasToDelete, Allomas *ElsoAllomas) {
    if (AllomasToDelete == ElsoAllomas) {
        Allomas *tmp = ElsoAllomas;
        ElsoAllomas = ElsoAllomas->getNextNode();
        delete tmp;

        return ElsoAllomas;
    }
    Allomas *tmp = ElsoAllomas;
    Allomas *prevNode = ElsoAllomas;
    while (tmp != AllomasToDelete) {
        if (tmp == nullptr) {
            throw "AllomasToDelete does not exist!";
        }
        prevNode = tmp;
        tmp = tmp->getNextNode();
    }
    prevNode->setNextNode(tmp->getNextNode());
    delete tmp;
    return this;

}

Allomas* Allomas::InitAllomas() {
    int ReadIncrement = 0;
    std::string Name;
    double Szelesseg;
    double Magassag;
    bool Wc;
    bool Bufe;
    bool FirstAllomasExists = false;
    Allomas* FirstAllomas = nullptr;
    std::ifstream file("Allomas.dat");
    if (!file.is_open()) {
        throw "Allomas file could not be opened!";
    }
    std::string line;
    std::string tmp;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        while (std::getline(ss, tmp, ';')) {
            if (ReadIncrement == 0) {
                Name = tmp;
                ReadIncrement++;
            }else if (ReadIncrement == 1) {
                Szelesseg = std::stod(tmp);
                ReadIncrement++;
            }else if (ReadIncrement == 2) {
                Magassag = std::stod(tmp);
                ReadIncrement++;
            }else if (ReadIncrement == 3) {
                Wc = std::stoi(tmp) == 1;
                ReadIncrement++;
            }else if (ReadIncrement == 4) {
                Bufe = std::stoi(tmp) == 1;
                ReadIncrement = 0;
            }
        }
        if (!FirstAllomasExists) {
            FirstAllomas = new Allomas(Name,Szelesseg,Magassag,Wc,Bufe);
            FirstAllomasExists = true;
        } else if (FirstAllomas != nullptr) {
            Allomas* NextAllomas = new Allomas(Name,Szelesseg,Magassag,Wc,Bufe);
            FirstAllomas->operator+(NextAllomas);
        }
    }
    if (FirstAllomas == nullptr) {
        throw "FirstAllomas was not initialised!";
    }
    file.close();
    return FirstAllomas;

}
Allomas * Allomas::FindAllomasByName(const std::string &Nev) {
    Allomas* tmp = this;
    while (tmp != nullptr) {
        if (tmp->getAllomasNev() == Nev) {
            return tmp;
        }
        tmp = tmp->getNextNode();
    }
    throw "Allomas with that Name does not exist!";
}
