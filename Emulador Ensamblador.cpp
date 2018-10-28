﻿// Emulador Ensamblador.cpp

#include <iostream>

using namespace std;

//Clase que se utiliza para cada linea de codigo del lenguaje ensamblador
class Linea
{
	public:
		void setCOP(int cop)
		{
			iCOP = cop;
		}
		void setTD(int td)
		{
			iTD = td;
		}
		void setDIR(int dir)
		{
			iTD = dir;
		}
		int getCOP()
		{
			return iCOP;
		}
		int getTD()
		{
			return iTD;
		}
		int getDIR()
		{
			return iDIR;
		}
	private:
		int iCOP, iTD, iDIR;
};

//Variables globales que corresponden a cada uno de los registros de la computadora

int iPC, iMAR;
Linea lACC, lIR, lMDR, lInstrucciones[1000];
char cFR;

Linea mmRead()
{
	return lInstrucciones[iPC];
}

void mmWrite()
{
	lInstrucciones[iMAR].setDIR(lACC.getDIR());
}

void cicloFetch()
{
	iMAR = iPC;
	lMDR = mmRead();
	iPC = iPC + 1;
	lIR = lMDR;
}

int main()
{

	return 0;
}
