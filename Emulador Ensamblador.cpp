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
		iDIR = dir;
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
	Linea()
	{
		iCOP = 0;
		iTD = 0;
		iDIR = 0;
	}
private:
	//Variables que distinguen cada linea de código del lenguaje ensamblador: Código de operación, 
	//tipo de direccionamiento y dato o dirección

	int iCOP, iTD, iDIR;
};

//Variables globales que corresponden a cada uno de los registros de la computadora

int iPC{ 0 }, iMAR{ 0 }, iACC{ 0 }, iFR{ 0 };
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
	iPC++;
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

//Seleciona el ciclo a Ejecutar
void elegirCiclo()
{
	switch (lIR.getCOP())
	{
	case 0:
		cicloNOP();
		break;
	case 1:
		cicloCLA();
		break;
	case 2:
		cicloNEG();
		break;
	case 10:
		cicloLDA();
		break;
	case 11:
		cicloSTA();
		break;
	case 20:
		cicloADD();
		break;
	case 21:
		cicloSUB();
		break;
	case 30:
		cicloJMP();
		break;
	case 31:
		cicloJMZ();
		break;
	case 32:
		cicloJMN();
		break;
	case 33:
		cicloJMV();
		break;
	default:
		break;
	}
}

void escribirLinea(int iCont, string sLinea)
{
	int cop, td, dir, cont{ 0 };
	for (int iVeces = 0; iVeces < 3; iVeces++)
	{
		string sValor{ "" };
		for (cont; sLinea[cont] != ',' && sLinea[cont] != '.'; cont++)
		{
			sValor += sLinea[cont];
		}
		cont++;
		switch (iVeces)
		{
		case 0:
			cop = stoi(sValor);
			lInstrucciones[iCont].setCOP(cop);
			break;
		case 1:
			td = stoi(sValor);
			lInstrucciones[iCont].setTD(td);
			break;
		case 2:
			dir = stoi(sValor);
			lInstrucciones[iCont].setDIR(dir);
			break;
		default:
			break;
		}
	}
}

string convertirLinea(string sLinea)
{
	for (int cont = 0; sLinea[cont] != '\0'; cont++)
	{
		sLinea[cont] = toupper(sLinea[cont]);
	}
	int cont = 0;
	for (int iVeces = 0; iVeces < 2; iVeces++)
	{
		string sValor{ "" };
		for (cont; sLinea[cont] != ',' && sLinea[cont] != '.'; cont++)
		{
			sValor += sLinea[cont];
		}
		if (sValor == "CLA")
		{
			sLinea[0] = '0';
			sLinea[1] = '0';
			sLinea[2] = '1';
		}
		else if (sValor == "HLT")
		{
			sLinea[0] = '0';
			sLinea[1] = '9';
			sLinea[2] = '9';
		}
		else if (sValor == "NOP")
		{
			sLinea[0] = '0';
			sLinea[1] = '0';
			sLinea[2] = '0';
		}
		else if (sValor == "LDA")
		{
			sLinea[0] = '0';
			sLinea[1] = '1';
			sLinea[2] = '0';
		}
		else if (sValor == "STA")
		{
			sLinea[0] = '0';
			sLinea[1] = '1';
			sLinea[2] = '1';
		}
		else if (sValor == "ADD")
		{
			sLinea[0] = '0';
			sLinea[1] = '2';
			sLinea[2] = '0';
		}
		else if (sValor == "SUB")
		{
			sLinea[0] = '0';
			sLinea[1] = '2';
			sLinea[2] = '1';
		}
		else if (sValor == "NEG")
		{
			sLinea[0] = '0';
			sLinea[1] = '0';
			sLinea[2] = '2';
		}
		else if (sValor == "JMP")
		{
			sLinea[0] = '0';
			sLinea[1] = '3';
			sLinea[2] = '0';
		}
		else if (sValor == "JMZ")
		{
			sLinea[0] = '0';
			sLinea[1] = '3';
			sLinea[2] = '1';
		}
		else if (sValor == "JMN")
		{
			sLinea[0] = '0';
			sLinea[1] = '3';
			sLinea[2] = '2';
		}
		else if (sValor == "JMV")
		{
			sLinea[0] = '0';
			sLinea[1] = '3';
			sLinea[2] = '3';
		}
		else if (sValor == "I")
		{
			sLinea[4] = '0';
		}
		else if (sValor == "R")
		{
			sLinea[4] = '1';
		}
		else if (sValor == "A")
		{
			sLinea[4] = '2';
		}
		else if (sValor == "D")
		{
			sLinea[4] = '3';
		}
		cont++;
	}
	return sLinea;
}

int main()
{
	string sLinea{ "  " };
	int iCont{ 0 }, iLineas;
	cout << "Introduce tu codigo de ensamblador con el siguiente formato \"COP,TD,DIR.\"" << endl;
	while (sLinea[0] != '9' && sLinea[1] != '9')
	{
		cout << iCont << " ";
		cin >> sLinea;
		sLinea = convertirLinea(sLinea);
		escribirLinea(iCont, sLinea);
		iCont++;
	}
	for (int iX = 0; iX < iCont; iX++)
	{
		if (iACC == 0)
		{
			iFR = 1;
		}
		else if (iACC < 0)
		{
			iFR = 2;
		}
		else if (iACC > 100000000)
		{
			iFR = 3;
		}
		else
		{
			iFR = 0;
		}
		cicloFetch();
		elegirCiclo();
	}
	cout << "Selecciona cuantas lineas de codigo quieres ver en pantalla: " << endl;
	cin >> iLineas;
	cout << "Tu codigo quedo de la siguiente manera: " << endl;
	for (iCont = 0; iCont < iLineas; iCont++)
	{
		cout << iCont << "; " << lInstrucciones[iCont].getCOP() << ", " << lInstrucciones[iCont].getTD() << ", " << lInstrucciones[iCont].getDIR() << "." << endl;
	}
	return 0;
}

