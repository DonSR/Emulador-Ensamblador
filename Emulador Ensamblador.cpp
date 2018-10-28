// Emulador Ensamblador.cpp

#include "pch.h"
#include <iostream>
#include <thread>
#include <string>

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
	//Variables que distinguen cada linea de código del lenguaje ensamblador: Código de operación, 
	//tipo de direccionamiento y dato o dirección

	int iCOP, iTD, iDIR;
};

//Variables globales que corresponden a cada uno de los registros de la computadora

int iPC, iMAR, iACC, iFR;
Linea lIR, lMDR, lInstrucciones[1000];

//Función para hacer main memory read
Linea mmRead()
{
	return lInstrucciones[iMAR];
}

//Función para hacer main memory write
void mmWrite()
{
	lInstrucciones[iMAR].setDIR(lMDR.getDIR());
}

//Ciclo de fetch
void cicloFetch()
{
	iMAR = iPC;
	lMDR = mmRead();
	iPC = iPC + 1;
	lIR = lMDR;
}

//NOP = no operación, se hace un delay de 1 segundo para demostrar que no se hizo una operación (tiempo exagerado)
void cicloNOP()
{
	std::this_thread::sleep_for(1s);
}

//CLA = limpiar acumuldor
void cicloCLA()
{
	iACC = 0;
}

//NEG = cambiar el signo del acumulador
void cicloNEG()
{
	iACC = iACC * -1;
}

//LDA = cargar al acumulador
void cicloLDA()
{
	//Selecciona el tipo de direccionamiento para ejecutar ciclos distintos
	switch (lIR.getTD())
	{
	//Direccionamiento inmediato
	case 0:
		iACC = lIR.getDIR();
		break;
	//Direccionamiento Relativo
	case 1:
		iMAR = lIR.getDIR() + iPC;
		lMDR = mmRead();
		iACC = lMDR.getDIR();
		break;
	//Direccionamiento Absoluto
	case 2:
		iMAR = lIR.getDIR();
		lMDR = mmRead();
		iACC = lMDR.getDIR();
		break;
	//Direccionamiento Indirecto
	case 3:
		iMAR = lIR.getDIR();
		lMDR = mmRead();
		iMAR = lMDR.getDIR();
		lMDR = mmRead();
		iACC = lMDR.getDIR();
		break;
	default:
		break;
	}
}

//STA = guarda un dato del acumulador en memoria
void cicloSTA()
{
	//Selecciona el tipo de direccionamiento para ejecutar ciclos distintos
	switch (lIR.getTD())
	{
	//Direccionamiento Relativo
	case 1:
		iMAR = lIR.getDIR() + iPC;
		lMDR.setDIR(iACC);
		mmWrite();
		break;
	//Direccionamiento Absoluto
	case 2:
		iMAR = lIR.getDIR();
		lMDR.setDIR(iACC);
		mmWrite();
		break;
	//Direccionamiento Indirecto
	case 3:
		iMAR = lIR.getDIR();
		lMDR = mmRead();
		iMAR = lMDR.getDIR();
		lMDR.setDIR(iACC);
		mmWrite();
		break;
	default:
		break;
	}
}

//ADD = sumar al acumulador
void cicloADD()
{
	//Selecciona el tipo de direccionamiento para ejecutar ciclos distintos
	switch (lIR.getTD())
	{
		//Direccionamiento inmediato
	case 0:
		iACC = iACC + lIR.getDIR();
		break;
		//Direccionamiento Relativo
	case 1:
		iMAR = lIR.getDIR() + iPC;
		lMDR = mmRead();
		iACC = iACC + lMDR.getDIR();
		break;
		//Direccionamiento Absoluto
	case 2:
		iMAR = lIR.getDIR();
		lMDR = mmRead();
		iACC = iACC + lMDR.getDIR();
		break;
		//Direccionamiento Indirecto
	case 3:
		iMAR = lIR.getDIR();
		lMDR = mmRead();
		iMAR = lMDR.getDIR();
		lMDR = mmRead();
		iACC = iACC + lMDR.getDIR();
		break;
	default:
		break;
	}
}

//SUB = restar al acumulador
void cicloSUB()
{
	//Selecciona el tipo de direccionamiento para ejecutar ciclos distintos
	switch (lIR.getTD())
	{
		//Direccionamiento inmediato
	case 0:
		iACC = iACC - lIR.getDIR();
		break;
		//Direccionamiento Relativo
	case 1:
		iMAR = lIR.getDIR() + iPC;
		lMDR = mmRead();
		iACC = iACC - lMDR.getDIR();
		break;
		//Direccionamiento Absoluto
	case 2:
		iMAR = lIR.getDIR();
		lMDR = mmRead();
		iACC = iACC - lMDR.getDIR();
		break;
		//Direccionamiento Indirecto
	case 3:
		iMAR = lIR.getDIR();
		lMDR = mmRead();
		iMAR = lMDR.getDIR();
		lMDR = mmRead();
		iACC = iACC - lMDR.getDIR();
		break;
	default:
		break;
	}
}

//JMP = salto a otra dirección de memoria
void cicloJMP()
{
	//Selecciona el tipo de direccionamiento para ejecutar ciclos distintos
	switch (lIR.getTD())
	{
	//Direccionamiento Relativo
	case 1:
		iPC = iPC + lIR.getDIR();
		break;
	//Direccionamiento Absoluto
	case 2:
		iPC = lIR.getDIR();
		break;
	//Direccionamiento Indirecto
	case 3:
		iMAR = lIR.getDIR();
		lMDR = mmRead();
		iPC = lMDR.getDIR();
		break;
	default:
		break;
	}
}

//JMZ = salto si la bandera es cero
void cicloJMZ()
{
	if (iFR == 1)
	{
		cicloJMP();
	}
}

//JMN = salto si la bandera es negativa
void cicloJMN()
{
	if (iFR == 2)
	{
		cicloJMP();
	}
}

//JMV = salto si la bandera es overflow
void cicloJMV()
{
	if (iFR == 3)
	{
		cicloJMP();
	}
}

//HLT = detiene la ejecución del programa
void cicloHLT()
{
	Linea end;
	end.setCOP = 999;
	end.setDIR = 999;
	end.setTD = 999;
	lInstrucciones[iPC] = end;
}

int main()
{
	string sLinea{ "" };
	cout << "Introduce tu codigo de ensamblador con el siguiente formato \"COP,TD,DIR\"" << endl;
	while (sLinea != "HLT")
	{
		cin >> sLinea;
	}
	return 0;
}
