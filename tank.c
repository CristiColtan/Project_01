#define _CRT_SECURE_NO_WARNINGS 

#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>
#include <stdlib.h>
#include <wincon.h>
#include <stdbool.h>

/*
HOW TO PLAY ==============================================
FIECARE TANC ARE UN COMBUSTIBIL.
SE MISCA STANGA DE PE A, RESPECTIV DREAPTA DE PE D.
DUPA CE COMBUSTIBILUL S A TERMINAT PUTETI SETA PUTEREA.

PENTRU SETAREA PUTERII APASATI P(CRESTE) O(SCADE)

APASATI SPACE DACA ATI AJUNS LA VALOAREA DORITA.

PENTRU SETAREA UNGHIULUI APASATI U(CRESTE) Y(SCADE)
AVETI DE ALES INTRE 4 UNGHIURI (0,30,60,90)
0 trage orizontal(bara este goala), 90 vertical(bara este plina)

APASATI SPACE DACA ATI AJUNS LA VALOAREA DORITA

PENTRU SETAREA MUNITIEI APASATI M(CRESTE) N(SCADE)
BARA GOALA = PROIECTIL MIC (DAMAGE REDUS (DACA NIMERESTE TANCUL -40))
BARA 1 CARACTER = PROIECTIL MARE (DAMAGE RIDICAT (DACA NIMERESTE TANCUL -80))
BARA 2 CARACTERE = ATAC AERIAN (DAMAGE FOARTE REDUS (DACA NIMERESTE -20))

APASATI SPACE DACA ATI AJUNS LA VALOARE DORITA PENTRU A TRAGE.

ATENTIE!
DACA UTILIZATI ATACUL AERIAN, APASATI ENTER PENTRU A TRAGE!(INTRODUCETI COLOANA PE CARE SA FIE LANSATA RACHETA)

ATENTIE!
PUTEREA TREBUIE SA FIE MINIM 1, ALTFEL TANCUL EXPLODEAZA.
*/

#define DIM_X 50
#define DIM_Y 197

#define ANSI_COLOR_BLUE "\033[0;34m"
#define ANSI_COLOR_YELLOW "\033[0;33m"
#define ANSI_COLOR_RESET "\033[0;37m"

#define CHR_STANGA_SUS 201
#define CHR_STANGA_JOS 200
#define CHR_DREAPTA_SUS 187
#define CHR_DREPTA_JOS 188
#define CHR_BARA_VERTICALA 186
#define CHR_BARA_ORIZONTALA 205
#define CHR_BIT_GRADAT 178
#define CHR_BIT_PLIN 219
#define ROATA_TANK 79
#define CORP_TANK 219
#define TEAVA_TUN 205
#define CAP_TANK 220
#define PROIECTIL 111

//variabile globale

char scena[DIM_X][DIM_Y];
int player1_life = 99;
int player2_life = 99;
int putere = 0;
int coloana = -1;
int ammo_x = 54;
int damage = 0;
int ammo_y = 40;
int unghi = 0;
int shoots = 0;
int air_attack_x = 0;
int air_attack_y = 0;
int ammo = 0;
int player1_fuel = 7;
int player2_fuel = 5;
int roata_tank1_fata_x = DIM_X - 2;
int roata_tank1_fata_y = 1;
int tank1_x[2][3] = { 0 };
int tank1_y[2][3] = { 0 };
int tank2_x[2][3] = { 0 };
int tank2_y[2][3] = { 0 };
int putere_x = 56;
int putere_y = 1;
int unghi_x = 54;
int unghi_y = 1;
int proiectil1_x;
int proiectil1_y;
int proiectil1_x;
int proiectil1_y;

///

bool joc_pierdut(int x, int y)
{
	if (player1_life < 1 || player2_life < 1)
		return true;
	return false;
}
void gotoXY(int x, int y)
{
	COORD coord = { y, x };

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}//am inversat x cu y, deoarece coordonatele erau inversate atunci cand apelam functia

/// <summary>
/// /TANK 1
/// </summary>
void initializare_tank1()
{
	tank1_x[0][0] = DIM_X - 3;//cap
	tank1_x[0][1] = DIM_X - 3;//corp
	tank1_x[0][2] = DIM_X - 3;//tun
	tank1_x[1][0] = DIM_X - 2;//roata
	tank1_x[1][1] = DIM_X - 2;//roata
	tank1_x[1][2] = DIM_X - 2;//roata

	tank1_y[0][0] = 1;//cap
	tank1_y[0][1] = 2;//corp
	tank1_y[0][2] = 3;//tun
	tank1_y[1][0] = 1;//roata
	tank1_y[1][1] = 2;//roata
	tank1_y[1][2] = 3;//roata
}
void pozitioneaza_tank1()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 18);
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 3; j++)
		{
			if (i == 0 && j == 0)
			{
				gotoXY(tank1_x[i][j], tank1_y[i][j]);
				{
					scena[tank1_x[i][j]][tank1_y[i][j]] = CAP_TANK;
					printf("%c", CAP_TANK);
				}
			}
			if (i == 0 && j == 1)
			{
				gotoXY(tank1_x[i][j], tank1_y[i][j]);
				{
					scena[tank1_x[i][j]][tank1_y[i][j]] = CORP_TANK;
					printf("%c", CORP_TANK);
				}
			}
			if (i == 0 && j == 2)
			{
				gotoXY(tank1_x[i][j], tank1_y[i][j]);
				{
					scena[tank1_x[i][j]][tank1_y[i][j]] = TEAVA_TUN;
					printf("%c", TEAVA_TUN);
				}
			}
			if (i == 1)
			{
				gotoXY(tank1_x[i][j], tank1_y[i][j]);
				{
					scena[tank1_x[i][j]][tank1_y[i][j]] = ROATA_TANK;
					printf("%c", ROATA_TANK);
				}
			}
		}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}//am parcurs matricea fiecarui tank, apoi am printat partile lui dupa schema de la initializare
void miscare_stanga_tank1()
{
	if (scena[tank1_x[1][2] + 1][tank1_y[1][2] - 1] == (char)CHR_BIT_GRADAT)
	{
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 3; j++)
			{
				tank1_x[i][j]++;
				tank1_y[i][j]--;
				if (i == 0 && j == 0)
				{
					gotoXY(tank1_x[i][j], tank1_y[i][j]);
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 18);
						scena[tank1_x[i][j]][tank1_y[i][j]] = CAP_TANK;
						printf("%c", CAP_TANK);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
				}
				if (i == 0 && j == 1)
				{
					gotoXY(tank1_x[i][j], tank1_y[i][j]);
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 18);
						scena[tank1_x[i][j]][tank1_y[i][j]] = CORP_TANK;
						printf("%c", CORP_TANK);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
				}
				if (i == 0 && j == 2)
				{
					gotoXY(tank1_x[i][j], tank1_y[i][j]);
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 18);
						scena[tank1_x[i][j]][tank1_y[i][j]] = TEAVA_TUN;
						printf("%c", TEAVA_TUN);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
				}
				if (i == 1)
				{

					gotoXY(tank1_x[i][j], tank1_y[i][j]);
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 18);
						scena[tank1_x[i][j]][tank1_y[i][j]] = ROATA_TANK;
						printf("%c", ROATA_TANK);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
				}


			}
		gotoXY(tank1_x[0][2], tank1_y[0][2] + 1);
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
			scena[tank1_x[0][2] + 1][tank1_y[0][2] + 1] = CHR_BIT_GRADAT;
			printf("%c", CHR_BIT_GRADAT);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		}
		gotoXY(tank1_x[0][2] - 1, tank1_y[0][2] + 1);
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
			scena[tank1_x[0][2] - 1][tank1_y[0][2] + 1] = CHR_BIT_GRADAT;
			printf("%c", CHR_BIT_GRADAT);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		}
		gotoXY(tank1_x[0][2] - 1, tank1_y[0][2]);
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
			scena[tank1_x[0][2] - 1][tank1_y[0][2]] = CHR_BIT_GRADAT;
			printf("%c", CHR_BIT_GRADAT);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		}
		gotoXY(tank1_x[0][2] - 1, tank1_y[0][2] - 1);
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
			scena[tank1_x[0][2] - 1][tank1_y[0][2] - 1] = CHR_BIT_GRADAT;
			printf("%c", CHR_BIT_GRADAT);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		}
	}
	else
	{
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 3; j++)
			{
				tank1_y[i][j]--;
				if (i == 0 && j == 0)
				{
					gotoXY(tank1_x[i][j], tank1_y[i][j]);
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 18);
						scena[tank1_x[i][j]][tank1_y[i][j]] = CAP_TANK;
						printf("%c", CAP_TANK);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
				}
				if (i == 0 && j == 1)
				{
					gotoXY(tank1_x[i][j], tank1_y[i][j]);
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 18);
						scena[tank1_x[i][j]][tank1_y[i][j]] = CORP_TANK;
						printf("%c", CORP_TANK);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
				}
				if (i == 0 && j == 2)
				{
					gotoXY(tank1_x[i][j], tank1_y[i][j]);
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 18);
						scena[tank1_x[i][j]][tank1_y[i][j]] = TEAVA_TUN;
						printf("%c", TEAVA_TUN);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
				}
				if (i == 1)
				{
					gotoXY(tank1_x[i][j], tank1_y[i][j]);
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 18);
						scena[tank1_x[i][j]][tank1_y[i][j]] = ROATA_TANK;
						printf("%c", ROATA_TANK);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
				}
			}
		gotoXY(tank1_x[0][2], tank1_y[0][2] + 1);
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
			scena[tank1_x[0][2]][tank1_y[0][2] + 1] = CHR_BIT_GRADAT;
			printf("%c", CHR_BIT_GRADAT);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		}
		gotoXY(tank1_x[1][2], tank1_y[1][2] + 1);
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
			scena[tank1_x[1][2]][tank1_y[1][2] - 1] = CHR_BIT_GRADAT;
			printf("%c", CHR_BIT_GRADAT);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		}
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}
void miscare_dreapta_tank1()
{
	if (scena[tank1_x[1][2]][tank1_y[1][2] + 1] == (char)CHR_BIT_PLIN)
	{
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 3; j++)
			{
				tank1_x[i][j]--;
				tank1_y[i][j]++;
				if (i == 0 && j == 0)
				{
					gotoXY(tank1_x[i][j], tank1_y[i][j]);
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 18);
						scena[tank1_x[i][j]][tank1_y[i][j]] = CAP_TANK;
						printf("%c", CAP_TANK);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
				}
				if (i == 0 && j == 1)
				{
					gotoXY(tank1_x[i][j], tank1_y[i][j]);
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 18);
						scena[tank1_x[i][j]][tank1_y[i][j]] = CORP_TANK;
						printf("%c", CORP_TANK);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
				}
				if (i == 0 && j == 2)
				{
					gotoXY(tank1_x[i][j], tank1_y[i][j]);
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 18);
						scena[tank1_x[i][j]][tank1_y[i][j]] = TEAVA_TUN;
						printf("%c", TEAVA_TUN);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
				}
				if (i == 1)
				{

					gotoXY(tank1_x[i][j], tank1_y[i][j]);
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 18);
						scena[tank1_x[i][j]][tank1_y[i][j]] = ROATA_TANK;
						printf("%c", ROATA_TANK);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
				}


			}
		gotoXY(tank1_x[1][0] + 1, tank1_y[1][0] + 1);
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
			scena[tank1_x[1][0] + 1][tank1_y[1][0] + 1] = CHR_BIT_GRADAT;
			printf("%c", CHR_BIT_GRADAT);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		}
		gotoXY(tank1_x[1][0] + 1, tank1_y[1][0]);
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
			scena[tank1_x[1][0] + 1][tank1_y[1][0]] = CHR_BIT_GRADAT;
			printf("%c", CHR_BIT_GRADAT);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		}
		gotoXY(tank1_x[1][0] + 1, tank1_y[1][0] - 1);
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
			scena[tank1_x[1][0] + 1][tank1_y[1][0] - 1] = CHR_BIT_GRADAT;
			printf("%c", CHR_BIT_GRADAT);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		}
		gotoXY(tank1_x[1][0], tank1_y[1][0] - 1);
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
			scena[tank1_x[1][0]][tank1_y[1][0] - 1] = CHR_BIT_GRADAT;
			printf("%c", CHR_BIT_GRADAT);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		}
	}
	else
	{
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 3; j++)
			{
				tank1_y[i][j]++;
				if (i == 0 && j == 0)
				{
					gotoXY(tank1_x[i][j], tank1_y[i][j]);
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 18);
						scena[tank1_x[i][j]][tank1_y[i][j]] = CAP_TANK;
						printf("%c", CAP_TANK);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
				}
				if (i == 0 && j == 1)
				{
					gotoXY(tank1_x[i][j], tank1_y[i][j]);
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 18);
						scena[tank1_x[i][j]][tank1_y[i][j]] = CORP_TANK;
						printf("%c", CORP_TANK);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
				}
				if (i == 0 && j == 2)
				{
					gotoXY(tank1_x[i][j], tank1_y[i][j]);
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 18);
						scena[tank1_x[i][j]][tank1_y[i][j]] = TEAVA_TUN;
						printf("%c", TEAVA_TUN);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
				}
				if (i == 1)
				{
					gotoXY(tank1_x[i][j], tank1_y[i][j]);
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 18);
						scena[tank1_x[i][j]][tank1_y[i][j]] = ROATA_TANK;
						printf("%c", ROATA_TANK);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
				}
			}
		gotoXY(tank1_x[0][0], tank1_y[0][0] - 1);
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
			scena[tank1_x[0][0]][tank1_y[0][0] - 1] = CHR_BIT_GRADAT;
			printf("%c", CHR_BIT_GRADAT);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		}
		gotoXY(tank1_x[1][0], tank1_y[1][0] - 1);
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
			scena[tank1_x[1][0]][tank1_y[1][0] - 1] = CHR_BIT_GRADAT;
			printf("%c", CHR_BIT_GRADAT);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		}
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}
//===============================

/// <summary>
/// /TANK 2
/// </summary>
void initializare_tank2()
{
	tank2_x[0][0] = DIM_X - 3;//tun
	tank2_x[0][1] = DIM_X - 3;//corp
	tank2_x[0][2] = DIM_X - 3;//cap
	tank2_x[1][0] = DIM_X - 2;//roata
	tank2_x[1][1] = DIM_X - 2;//roata
	tank2_x[1][2] = DIM_X - 2;//roata

	tank2_y[0][0] = DIM_Y - 4;//tun
	tank2_y[0][1] = DIM_Y - 3;//corp
	tank2_y[0][2] = DIM_Y - 2;//cap
	tank2_y[1][0] = DIM_Y - 4;//roata
	tank2_y[1][1] = DIM_Y - 3;//roata
	tank2_y[1][2] = DIM_Y - 2;//roata
}
void pozitioneaza_tank2()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 20);
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 3; j++)
		{
			if (i == 0 && j == 0)
			{
				gotoXY(tank2_x[i][j], tank2_y[i][j]);
				{
					scena[tank2_x[i][j]][tank2_y[i][j]] = TEAVA_TUN;
					printf("%c", TEAVA_TUN);
				}
			}
			if (i == 0 && j == 1)
			{
				gotoXY(tank2_x[i][j], tank2_y[i][j]);
				{
					scena[tank2_x[i][j]][tank2_y[i][j]] = CORP_TANK;
					printf("%c", CORP_TANK);
				}
			}
			if (i == 0 && j == 2)
			{
				gotoXY(tank2_x[i][j], tank2_y[i][j]);
				{
					scena[tank2_x[i][j]][tank2_y[i][j]] = CAP_TANK;
					printf("%c", CAP_TANK);
				}
			}
			if (i == 1)
			{
				gotoXY(tank2_x[i][j], tank2_y[i][j]);
				{
					scena[tank2_x[i][j]][tank2_y[i][j]] = ROATA_TANK;
					printf("%c", ROATA_TANK);
				}
			}
		}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}
void miscare_stanga_tank2()
{
	if (scena[tank2_x[1][0]][tank2_y[1][0] - 1] == (char)CHR_BIT_PLIN)
	{
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 3; j++)
			{
				tank2_x[i][j]--;
				tank2_y[i][j]--;
				if (i == 0 && j == 0)
				{
					gotoXY(tank2_x[i][j], tank2_y[i][j]);
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 20);
						scena[tank2_x[i][j]][tank2_y[i][j]] = TEAVA_TUN;
						printf("%c", TEAVA_TUN);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
				}
				if (i == 0 && j == 1)
				{
					gotoXY(tank2_x[i][j], tank2_y[i][j]);
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 20);
						scena[tank2_x[i][j]][tank2_y[i][j]] = CORP_TANK;
						printf("%c", CORP_TANK);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
				}
				if (i == 0 && j == 2)
				{
					gotoXY(tank2_x[i][j], tank2_y[i][j]);
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 20);
						scena[tank2_x[i][j]][tank2_y[i][j]] = CAP_TANK;
						printf("%c", CAP_TANK);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
				}
				if (i == 1)
				{

					gotoXY(tank2_x[i][j], tank2_y[i][j]);
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 20);
						scena[tank2_x[i][j]][tank2_y[i][j]] = ROATA_TANK;
						printf("%c", ROATA_TANK);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
				}


			}
		gotoXY(tank2_x[1][2], tank2_y[1][2] + 1);
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
			scena[tank2_x[1][2]][tank2_y[1][2] + 1] = CHR_BIT_GRADAT;
			printf("%c", CHR_BIT_GRADAT);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		}
		gotoXY(tank2_x[1][2] + 1, tank2_y[1][2] + 1);
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
			scena[tank2_x[1][2] + 1][tank2_y[1][2] + 1] = CHR_BIT_GRADAT;
			printf("%c", CHR_BIT_GRADAT);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		}
		gotoXY(tank2_x[1][2] + 1, tank2_y[1][2]);
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
			scena[tank2_x[1][2] + 1][tank2_y[1][2]] = CHR_BIT_GRADAT;
			printf("%c", CHR_BIT_GRADAT);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		}
		gotoXY(tank2_x[1][2] + 1, tank2_y[1][2] - 1);
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
			scena[tank2_x[1][2] + 1][tank2_y[1][2] - 1] = CHR_BIT_GRADAT;
			printf("%c", CHR_BIT_GRADAT);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		}
	}
	else
	{
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 3; j++)
			{
				tank2_y[i][j]--;
				if (i == 0 && j == 0)
				{
					gotoXY(tank2_x[i][j], tank2_y[i][j]);
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 20);
						scena[tank2_x[i][j]][tank2_y[i][j]] = TEAVA_TUN;
						printf("%c", TEAVA_TUN);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
				}
				if (i == 0 && j == 1)
				{
					gotoXY(tank2_x[i][j], tank2_y[i][j]);
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 20);
						scena[tank2_x[i][j]][tank1_y[i][j]] = CORP_TANK;
						printf("%c", CORP_TANK);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
				}
				if (i == 0 && j == 2)
				{
					gotoXY(tank2_x[i][j], tank2_y[i][j]);
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 20);
						scena[tank2_x[i][j]][tank2_y[i][j]] = CAP_TANK;
						printf("%c", CAP_TANK);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
				}
				if (i == 1)
				{
					gotoXY(tank2_x[i][j], tank2_y[i][j]);
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 20);
						scena[tank2_x[i][j]][tank2_y[i][j]] = ROATA_TANK;
						printf("%c", ROATA_TANK);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
				}
			}
		gotoXY(tank2_x[0][2], tank2_y[0][2] + 1);
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
			scena[tank2_x[0][2]][tank2_y[0][2] + 1] = CHR_BIT_GRADAT;
			printf("%c", CHR_BIT_GRADAT);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		}
		gotoXY(tank2_x[1][2], tank2_y[1][2] + 1);
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
			scena[tank2_x[1][2]][tank2_y[1][2] + 1] = CHR_BIT_GRADAT;
			printf("%c", CHR_BIT_GRADAT);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		}
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}
void miscare_dreapta_tank2()
{
	if (scena[tank2_x[1][0] + 1][tank2_y[1][0] + 1] == (char)CHR_BIT_GRADAT)
	{
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 3; j++)
			{
				tank2_x[i][j]++;
				tank2_y[i][j]++;
				if (i == 0 && j == 0)
				{
					gotoXY(tank2_x[i][j], tank2_y[i][j]);
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 20);
						scena[tank2_x[i][j]][tank2_y[i][j]] = TEAVA_TUN;
						printf("%c", TEAVA_TUN);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
				}
				if (i == 0 && j == 1)
				{
					gotoXY(tank2_x[i][j], tank2_y[i][j]);
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 20);
						scena[tank2_x[i][j]][tank2_y[i][j]] = CORP_TANK;
						printf("%c", CORP_TANK);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
				}
				if (i == 0 && j == 2)
				{
					gotoXY(tank2_x[i][j], tank2_y[i][j]);
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 20);
						scena[tank2_x[i][j]][tank2_y[i][j]] = TEAVA_TUN;
						printf("%c", CAP_TANK);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
				}
				if (i == 1)
				{

					gotoXY(tank2_x[i][j], tank2_y[i][j]);
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 20);
						scena[tank2_x[i][j]][tank2_y[i][j]] = ROATA_TANK;
						printf("%c", ROATA_TANK);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
				}


			}
		gotoXY(tank2_x[0][0] - 1, tank2_y[0][0] + 1);
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
			scena[tank2_x[0][0] - 1][tank2_y[0][0] + 1] = CHR_BIT_GRADAT;
			printf("%c", CHR_BIT_GRADAT);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		}
		gotoXY(tank2_x[0][0] - 1, tank2_y[0][0]);
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
			scena[tank2_x[0][0] - 1][tank2_y[0][0]] = CHR_BIT_GRADAT;
			printf("%c", CHR_BIT_GRADAT);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		}
		gotoXY(tank2_x[0][0] - 1, tank2_y[0][0] - 1);
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
			scena[tank2_x[0][0] - 1][tank2_y[0][0] - 1] = CHR_BIT_GRADAT;
			printf("%c", CHR_BIT_GRADAT);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		}
		gotoXY(tank2_x[0][0], tank2_y[0][0] - 1);
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
			scena[tank2_x[0][0]][tank2_y[0][0] - 1] = CHR_BIT_GRADAT;
			printf("%c", CHR_BIT_GRADAT);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		}
	}
	else
	{
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 3; j++)
			{
				tank2_y[i][j]++;
				if (i == 0 && j == 0)
				{
					gotoXY(tank2_x[i][j], tank2_y[i][j]);
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 20);
						scena[tank2_x[i][j]][tank2_y[i][j]] = TEAVA_TUN;
						printf("%c", TEAVA_TUN);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
				}
				if (i == 0 && j == 1)
				{
					gotoXY(tank2_x[i][j], tank2_y[i][j]);
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 20);
						scena[tank2_x[i][j]][tank2_y[i][j]] = CORP_TANK;
						printf("%c", CORP_TANK);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
				}
				if (i == 0 && j == 2)
				{
					gotoXY(tank2_x[i][j], tank2_y[i][j]);
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 20);
						scena[tank2_x[i][j]][tank2_y[i][j]] = CAP_TANK;
						printf("%c", CAP_TANK);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
				}
				if (i == 1)
				{
					gotoXY(tank2_x[i][j], tank2_y[i][j]);
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 20);
						scena[tank2_x[i][j]][tank2_y[i][j]] = ROATA_TANK;
						printf("%c", ROATA_TANK);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
				}
			}
		gotoXY(tank2_x[0][0], tank2_y[0][0] - 1);
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
			scena[tank2_x[0][0]][tank2_y[0][0] - 1] = CHR_BIT_GRADAT;
			printf("%c", CHR_BIT_GRADAT);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		}
		gotoXY(tank2_x[1][0], tank2_y[1][0] - 1);
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
			scena[tank2_x[1][0]][tank2_y[1][0] - 1] = CHR_BIT_GRADAT;
			printf("%c", CHR_BIT_GRADAT);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		}
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}
//================================

//SCENA===========================
void fullscreen()
{
	keybd_event(VK_MENU, 0x38, 0, 0);
	keybd_event(VK_RETURN, 0x1c, 0, 0);
	keybd_event(VK_RETURN, 0x1c, KEYEVENTF_KEYUP, 0);
	keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0);
}
void hidecursor()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}
void initializare_scena()
{
	for (int i = 0; i < DIM_X; i++)
		for (int j = 0; j < DIM_Y; j++)
			scena[i][j] = CHR_BIT_GRADAT;

	int margine_st = 0;
	int margine_dr = DIM_Y;

	for (int i = DIM_X - 1; i >= DIM_X - 12; i--)
	{

		for (int j = margine_st; j < margine_dr; j++)
		{
			scena[i][j] = CHR_BIT_PLIN;
		}
		margine_st += 8;
		margine_dr -= 8;
	}

}
void afisare_scena()
{
	system("cls");
	for (int i = 0; i < DIM_X; i++, printf("\n"))
		for (int j = 0; j < DIM_Y; j++)
		{
			if (scena[i][j] == (char)CHR_BIT_GRADAT)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
				printf("%c", scena[i][j]);
			}
			else if (scena[i][j] == (char)CHR_BIT_PLIN)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
				printf("%c", scena[i][j]);

			}
		}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}
//================================

//PROIECTIL=======================
void initializare_proiectil1()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31);
	gotoXY(tank1_x[0][2], tank1_y[0][2] + 1);
	scena[tank1_x[0][2]][tank1_y[0][2] + 1] = PROIECTIL;
	printf("%c", PROIECTIL);
	proiectil1_x = tank1_x[0][2];
	proiectil1_y = tank1_y[0][2];
}//am plasat proiectilul in fata tunului
void calculeaza_pozitie_proiectil1()
{
	if (unghi == 0)//miscare pe drept
	{
		for (int g = 1; g <= putere * 25; g++)
		{
			proiectil1_y++;
			if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_PLIN)
			{
				gotoXY(proiectil1_x, proiectil1_y);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
				scena[proiectil1_x][proiectil1_y] = CHR_BIT_GRADAT;
				printf("%c", CHR_BIT_GRADAT);
				break;
			}
			else if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_GRADAT)
			{
				gotoXY(proiectil1_x, proiectil1_y);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31);
				scena[proiectil1_x][proiectil1_y] = PROIECTIL;
				printf("%c", PROIECTIL);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				gotoXY(proiectil1_x, proiectil1_y - 1);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
				scena[proiectil1_x][proiectil1_y] = CHR_BIT_GRADAT;
				printf("%c", CHR_BIT_GRADAT);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			}
			Sleep(85.25);
		}
		gotoXY(proiectil1_x, proiectil1_y - 1);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
		scena[proiectil1_x][proiectil1_y] = CHR_BIT_GRADAT;
		printf("%c", CHR_BIT_GRADAT);
	}
	if (unghi == 1)//30 grade
	{
		for (int g = 1; g <= putere * 12; g++)
		{
			proiectil1_y++;
			if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_GRADAT)
			{
				gotoXY(proiectil1_x, proiectil1_y);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31);
				scena[proiectil1_x][proiectil1_y] = PROIECTIL;
				printf("%c", PROIECTIL);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

				gotoXY(proiectil1_x, proiectil1_y - 1);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
				scena[proiectil1_x][proiectil1_y] = CHR_BIT_GRADAT;
				printf("%c", CHR_BIT_GRADAT);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			}
			proiectil1_y++;
			if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_GRADAT)
			{
				gotoXY(proiectil1_x, proiectil1_y);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31);
				scena[proiectil1_x][proiectil1_y] = PROIECTIL;
				printf("%c", PROIECTIL);

				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				gotoXY(proiectil1_x, proiectil1_y - 1);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
				scena[proiectil1_x][proiectil1_y] = CHR_BIT_GRADAT;
				printf("%c", CHR_BIT_GRADAT);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			}
			if (g % 2 == 0)
			{
				proiectil1_x--;
				if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_GRADAT)
				{
					gotoXY(proiectil1_x, proiectil1_y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31);
					scena[proiectil1_x][proiectil1_y] = PROIECTIL;
					printf("%c", PROIECTIL);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

					gotoXY(proiectil1_x + 1, proiectil1_y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
					scena[proiectil1_x + 1][proiectil1_y] = CHR_BIT_GRADAT;
					printf("%c", CHR_BIT_GRADAT);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				}
			}
			Sleep(91.25);
		}
		for (int g = 1; g <= putere * 12; g++)
		{
			proiectil1_y++;
			if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_PLIN && proiectil1_x == tank2_x[0][1] && proiectil1_y == tank2_y[0][1])
				player2_life -= damage;
			if (scena[proiectil1_x][proiectil1_y] == (char)TEAVA_TUN || scena[proiectil1_x][proiectil1_y] == (char)CAP_TANK)
				player2_life -= damage;
			for (int oo = 0; oo < 2; oo++)
				for (int jj = 0; jj < 3; jj++)
					if (tank2_x[oo][jj] == proiectil1_x && tank2_y[oo][jj] == proiectil1_y)
					{
						player2_life -= damage;
						break;
					}
			if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_PLIN)
			{
				gotoXY(proiectil1_x, proiectil1_y);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
				scena[proiectil1_x][proiectil1_y] = CHR_BIT_GRADAT;
				printf("%c", CHR_BIT_GRADAT);
				break;
			}
			if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_GRADAT)
			{
				gotoXY(proiectil1_x, proiectil1_y);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31);
				scena[proiectil1_x][proiectil1_y] = PROIECTIL;
				printf("%c", PROIECTIL);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

				gotoXY(proiectil1_x, proiectil1_y - 1);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
				scena[proiectil1_x][proiectil1_y - 1] = CHR_BIT_GRADAT;
				printf("%c", CHR_BIT_GRADAT);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			}
			proiectil1_y++;
			for (int oo = 0; oo < 2; oo++)
				for (int jj = 0; jj < 3; jj++)
					if (tank2_x[oo][jj] == proiectil1_x && tank2_y[oo][jj] == proiectil1_y)
					{
						player2_life -= damage;
						break;
					}
			if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_PLIN)
			{
				gotoXY(proiectil1_x, proiectil1_y);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
				scena[proiectil1_x][proiectil1_y] = CHR_BIT_GRADAT;
				printf("%c", CHR_BIT_GRADAT);
				break;
			}
			if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_GRADAT)
			{
				gotoXY(proiectil1_x, proiectil1_y);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31);
				scena[proiectil1_x][proiectil1_y] = PROIECTIL;
				printf("%c", PROIECTIL);

				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				gotoXY(proiectil1_x, proiectil1_y - 1);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
				scena[proiectil1_x][proiectil1_y - 1] = CHR_BIT_GRADAT;
				printf("%c", CHR_BIT_GRADAT);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			}
			if (g % 2 == 0)
			{
				proiectil1_x++;
				for (int oo = 0; oo < 2; oo++)
					for (int jj = 0; jj < 3; jj++)
						if (tank2_x[oo][jj] == proiectil1_x && tank2_y[oo][jj] == proiectil1_y)
						{
							player2_life -= damage;
							break;
						}
				if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_PLIN)
				{
					gotoXY(proiectil1_x, proiectil1_y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
					scena[proiectil1_x][proiectil1_y] = CHR_BIT_GRADAT;
					printf("%c", CHR_BIT_GRADAT);
					break;
				}
				if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_GRADAT)
				{
					gotoXY(proiectil1_x, proiectil1_y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31);
					scena[proiectil1_x][proiectil1_y] = PROIECTIL;
					printf("%c", PROIECTIL);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

					gotoXY(proiectil1_x - 1, proiectil1_y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
					scena[proiectil1_x - 1][proiectil1_y] = CHR_BIT_GRADAT;
					printf("%c", CHR_BIT_GRADAT);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				}
			}
			Sleep(91.25);
		}
		gotoXY(proiectil1_x - 1, proiectil1_y);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
		scena[proiectil1_x - 1][proiectil1_y] = CHR_BIT_GRADAT;
		printf("%c", CHR_BIT_GRADAT);
		gotoXY(proiectil1_x, proiectil1_y - 1);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
		scena[proiectil1_x][proiectil1_y - 1] = CHR_BIT_GRADAT;
		printf("%c", CHR_BIT_GRADAT);
	}
	if (unghi == 2)//60 grade
	{
		for (int g = 1; g <= putere * 8; g++)
		{
			proiectil1_y++;
			if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_GRADAT)
			{
				gotoXY(proiectil1_x, proiectil1_y);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31);
				scena[proiectil1_x][proiectil1_y] = PROIECTIL;
				printf("%c", PROIECTIL);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

				gotoXY(proiectil1_x, proiectil1_y - 1);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
				scena[proiectil1_x][proiectil1_y] = CHR_BIT_GRADAT;
				printf("%c", CHR_BIT_GRADAT);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			}
			proiectil1_x--;
			if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_GRADAT)
			{
				gotoXY(proiectil1_x, proiectil1_y);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31);
				scena[proiectil1_x][proiectil1_y] = PROIECTIL;
				printf("%c", PROIECTIL);

				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				gotoXY(proiectil1_x + 1, proiectil1_y);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
				scena[proiectil1_x + 1][proiectil1_y] = CHR_BIT_GRADAT;
				printf("%c", CHR_BIT_GRADAT);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			}

			proiectil1_x--;
			if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_GRADAT)
			{
				gotoXY(proiectil1_x, proiectil1_y);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31);
				scena[proiectil1_x][proiectil1_y] = PROIECTIL;
				printf("%c", PROIECTIL);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

				gotoXY(proiectil1_x + 1, proiectil1_y);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
				scena[proiectil1_x + 1][proiectil1_y] = CHR_BIT_GRADAT;
				printf("%c", CHR_BIT_GRADAT);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			}
			Sleep(185.25);
		}
		for (int g = 1; g <= putere * 8; g++)
		{
			proiectil1_y++;
			if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_PLIN && proiectil1_x == tank2_x[0][1] && proiectil1_y == tank2_y[0][1])
				player2_life -= damage;
			if (scena[proiectil1_x][proiectil1_y] == (char)TEAVA_TUN || scena[proiectil1_x][proiectil1_y] == (char)CAP_TANK)
				player2_life -= damage;
			for (int oo = 0; oo < 2; oo++)
				for (int jj = 0; jj < 3; jj++)
					if (tank2_x[oo][jj] == proiectil1_x && tank2_y[oo][jj] == proiectil1_y)
					{
						player2_life -= damage;
						break;
					}
			if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_PLIN)
			{
				gotoXY(proiectil1_x, proiectil1_y);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
				scena[proiectil1_x][proiectil1_y] = CHR_BIT_GRADAT;
				printf("%c", CHR_BIT_GRADAT);
				break;
			}
			if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_GRADAT)
			{
				gotoXY(proiectil1_x, proiectil1_y);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31);
				scena[proiectil1_x][proiectil1_y] = PROIECTIL;
				printf("%c", PROIECTIL);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

				gotoXY(proiectil1_x, proiectil1_y - 1);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
				scena[proiectil1_x][proiectil1_y] = CHR_BIT_GRADAT;
				printf("%c", CHR_BIT_GRADAT);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			}
			proiectil1_x++;
			for (int oo = 0; oo < 2; oo++)
				for (int jj = 0; jj < 3; jj++)
					if (tank2_x[oo][jj] == proiectil1_x && tank2_y[oo][jj] == proiectil1_y)
					{
						player2_life -= damage;
						break;
					}
			if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_PLIN)
			{
				gotoXY(proiectil1_x, proiectil1_y);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
				scena[proiectil1_x][proiectil1_y] = CHR_BIT_GRADAT;
				printf("%c", CHR_BIT_GRADAT);
				break;
			}
			if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_GRADAT)
			{
				gotoXY(proiectil1_x, proiectil1_y);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31);
				scena[proiectil1_x][proiectil1_y] = PROIECTIL;
				printf("%c", PROIECTIL);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

				gotoXY(proiectil1_x - 1, proiectil1_y);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
				scena[proiectil1_x - 1][proiectil1_y] = CHR_BIT_GRADAT;
				printf("%c", CHR_BIT_GRADAT);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			}
			proiectil1_x++;
			for (int oo = 0; oo < 2; oo++)
				for (int jj = 0; jj < 3; jj++)
					if (tank2_x[oo][jj] == proiectil1_x && tank2_y[oo][jj] == proiectil1_y)
					{
						player2_life -= damage;
						break;
					}
			if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_PLIN)
			{
				gotoXY(proiectil1_x, proiectil1_y);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
				scena[proiectil1_x][proiectil1_y] = CHR_BIT_GRADAT;
				printf("%c", CHR_BIT_GRADAT);
				break;
			}
			if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_GRADAT)
			{
				gotoXY(proiectil1_x, proiectil1_y);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31);
				scena[proiectil1_x][proiectil1_y] = PROIECTIL;
				printf("%c", PROIECTIL);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

				gotoXY(proiectil1_x - 1, proiectil1_y);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
				scena[proiectil1_x - 1][proiectil1_y] = CHR_BIT_GRADAT;
				printf("%c", CHR_BIT_GRADAT);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			}

			Sleep(185.25);
		}
		gotoXY(proiectil1_x - 1, proiectil1_y);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
		scena[proiectil1_x - 1][proiectil1_y] = CHR_BIT_GRADAT;
		printf("%c", CHR_BIT_GRADAT);
	}
	if (unghi == 3)//90 grade
	{
		gotoXY(proiectil1_x, proiectil1_y + 1);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
		scena[proiectil1_x][proiectil1_y + 1] = CHR_BIT_GRADAT;
		printf("%c", CHR_BIT_GRADAT);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

		for (int g = 1; g <= putere * 4; g++)
		{
			proiectil1_x--;
			if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_GRADAT)
			{
				gotoXY(proiectil1_x, proiectil1_y);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31);
				scena[proiectil1_x][proiectil1_y] = PROIECTIL;
				printf("%c", PROIECTIL);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

				gotoXY(proiectil1_x + 1, proiectil1_y);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
				scena[proiectil1_x + 1][proiectil1_y] = CHR_BIT_GRADAT;
				printf("%c", CHR_BIT_GRADAT);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			}
			Sleep(185.25);
		}
		gotoXY(tank1_x[0][2], tank1_y[0][2]);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 18);
		printf("%c", TEAVA_TUN);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		for (int g = 1; g <= putere * 4; g++)
		{
			proiectil1_x++;
			if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_PLIN && proiectil1_x == tank1_x[0][1] && proiectil1_y == tank1_y[0][1])
				player1_life -= damage;
			if (scena[proiectil1_x][proiectil1_y] == (char)TEAVA_TUN || scena[proiectil1_x][proiectil1_y] == (char)CAP_TANK)
				player1_life -= damage;
			for (int oo = 0; oo < 2; oo++)
				for (int jj = 0; jj < 3; jj++)
					if (tank1_x[oo][jj] == proiectil1_x && tank1_y[oo][jj] == proiectil1_y)
					{
						player1_life -= damage;
						break;
					}
			if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_PLIN)
			{
				gotoXY(proiectil1_x, proiectil1_y);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
				scena[proiectil1_x][proiectil1_y] = CHR_BIT_GRADAT;
				printf("%c", CHR_BIT_GRADAT);
				break;
			}
			if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_GRADAT)
			{
				gotoXY(proiectil1_x, proiectil1_y);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31);
				scena[proiectil1_x][proiectil1_y] = PROIECTIL;
				printf("%c", PROIECTIL);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

				gotoXY(proiectil1_x - 1, proiectil1_y);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
				scena[proiectil1_x - 1][proiectil1_y] = CHR_BIT_GRADAT;
				printf("%c", CHR_BIT_GRADAT);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			}
			Sleep(185.25);
		}
		gotoXY(proiectil1_x, proiectil1_y);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
		scena[proiectil1_x][proiectil1_y] = CHR_BIT_GRADAT;
		printf("%c", CHR_BIT_GRADAT);
	}

}
void initializare_proiectil2()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31);
	gotoXY(tank2_x[0][0], tank2_y[0][0] - 1);
	scena[tank2_x[0][0]][tank2_y[0][0] - 1] = PROIECTIL;
	printf("%c", PROIECTIL);
	proiectil1_x = tank2_x[0][0];
	proiectil1_y = tank2_y[0][0];
}
void calculeaza_pozitie_proiectil2()
{

	{
		if (unghi == 0)//miscare pe drept
		{
			for (int g = 1; g <= putere * 25; g++)
			{
				proiectil1_y--;
				if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_PLIN)
				{
					gotoXY(proiectil1_x, proiectil1_y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
					scena[proiectil1_x][proiectil1_y] = CHR_BIT_GRADAT;
					printf("%c", CHR_BIT_GRADAT);
					break;
				}
				else if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_GRADAT)
				{
					gotoXY(proiectil1_x, proiectil1_y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31);
					scena[proiectil1_x][proiectil1_y] = PROIECTIL;
					printf("%c", PROIECTIL);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					gotoXY(proiectil1_x, proiectil1_y + 1);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
					scena[proiectil1_x][proiectil1_y] = CHR_BIT_GRADAT;
					printf("%c", CHR_BIT_GRADAT);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				}
				Sleep(85.25);
			}
			gotoXY(proiectil1_x, proiectil1_y + 1);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
			scena[proiectil1_x][proiectil1_y + 1] = CHR_BIT_GRADAT;
			printf("%c", CHR_BIT_GRADAT);
		}
		if (unghi == 1)//30 grade
		{
			for (int g = 1; g <= putere * 12; g++)
			{
				proiectil1_y--;
				if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_GRADAT)
				{
					gotoXY(proiectil1_x, proiectil1_y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31);
					scena[proiectil1_x][proiectil1_y] = PROIECTIL;
					printf("%c", PROIECTIL);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

					gotoXY(proiectil1_x, proiectil1_y + 1);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
					scena[proiectil1_x][proiectil1_y + 1] = CHR_BIT_GRADAT;
					printf("%c", CHR_BIT_GRADAT);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				}
				proiectil1_y--;
				if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_GRADAT)
				{
					gotoXY(proiectil1_x, proiectil1_y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31);
					scena[proiectil1_x][proiectil1_y] = PROIECTIL;
					printf("%c", PROIECTIL);

					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					gotoXY(proiectil1_x, proiectil1_y + 1);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
					scena[proiectil1_x][proiectil1_y + 1] = CHR_BIT_GRADAT;
					printf("%c", CHR_BIT_GRADAT);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				}
				if (g % 2 == 0)
				{
					proiectil1_x--;
					if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_GRADAT)
					{
						gotoXY(proiectil1_x, proiectil1_y);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31);
						scena[proiectil1_x][proiectil1_y] = PROIECTIL;
						printf("%c", PROIECTIL);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

						gotoXY(proiectil1_x + 1, proiectil1_y);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
						scena[proiectil1_x + 1][proiectil1_y] = CHR_BIT_GRADAT;
						printf("%c", CHR_BIT_GRADAT);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
				}
				Sleep(91.25);
			}
			for (int g = 1; g <= putere * 12; g++)
			{
				proiectil1_y--;
				if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_PLIN && proiectil1_x == tank2_x[0][1] && proiectil1_y == tank2_y[0][1])
					player1_life -= damage;
				if (scena[proiectil1_x][proiectil1_y] == (char)TEAVA_TUN || scena[proiectil1_x][proiectil1_y] == (char)CAP_TANK)
					player1_life -= damage;
				for (int oo = 0; oo < 2; oo++)
					for (int jj = 0; jj < 3; jj++)
						if (tank1_x[oo][jj] == proiectil1_x && tank1_y[oo][jj] == proiectil1_y)
						{
							player1_life -= damage;
							break;
						}
				if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_PLIN)
				{
					gotoXY(proiectil1_x, proiectil1_y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
					scena[proiectil1_x][proiectil1_y] = CHR_BIT_GRADAT;
					printf("%c", CHR_BIT_GRADAT);
					break;
				}
				if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_GRADAT)
				{
					gotoXY(proiectil1_x, proiectil1_y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31);
					scena[proiectil1_x][proiectil1_y] = PROIECTIL;
					printf("%c", PROIECTIL);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

					gotoXY(proiectil1_x, proiectil1_y + 1);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
					scena[proiectil1_x][proiectil1_y + 1] = CHR_BIT_GRADAT;
					printf("%c", CHR_BIT_GRADAT);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				}
				proiectil1_y--;
				for (int oo = 0; oo < 2; oo++)
					for (int jj = 0; jj < 3; jj++)
						if (tank1_x[oo][jj] == proiectil1_x && tank1_y[oo][jj] == proiectil1_y)
						{
							player1_life -= damage;
							break;
						}
				if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_PLIN)
				{
					gotoXY(proiectil1_x, proiectil1_y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
					scena[proiectil1_x][proiectil1_y] = CHR_BIT_GRADAT;
					printf("%c", CHR_BIT_GRADAT);
					break;
				}
				if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_GRADAT)
				{
					gotoXY(proiectil1_x, proiectil1_y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31);
					scena[proiectil1_x][proiectil1_y] = PROIECTIL;
					printf("%c", PROIECTIL);

					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					gotoXY(proiectil1_x, proiectil1_y + 1);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
					scena[proiectil1_x][proiectil1_y + 1] = CHR_BIT_GRADAT;
					printf("%c", CHR_BIT_GRADAT);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				}
				if (g % 2 == 0)
				{
					proiectil1_x++;
					for (int oo = 0; oo < 2; oo++)
						for (int jj = 0; jj < 3; jj++)
							if (tank1_x[oo][jj] == proiectil1_x && tank1_y[oo][jj] == proiectil1_y)
							{
								player1_life -= damage;
								break;
							}
					if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_PLIN)
					{
						gotoXY(proiectil1_x, proiectil1_y);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
						scena[proiectil1_x][proiectil1_y] = CHR_BIT_GRADAT;
						printf("%c", CHR_BIT_GRADAT);
						break;
					}
					if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_GRADAT)
					{
						gotoXY(proiectil1_x, proiectil1_y);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31);
						scena[proiectil1_x][proiectil1_y] = PROIECTIL;
						printf("%c", PROIECTIL);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

						gotoXY(proiectil1_x - 1, proiectil1_y);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
						scena[proiectil1_x - 1][proiectil1_y] = CHR_BIT_GRADAT;
						printf("%c", CHR_BIT_GRADAT);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
				}
				Sleep(91.25);
			}
			gotoXY(proiectil1_x - 1, proiectil1_y);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
			scena[proiectil1_x - 1][proiectil1_y] = CHR_BIT_GRADAT;
			printf("%c", CHR_BIT_GRADAT);
			gotoXY(proiectil1_x, proiectil1_y + 1);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
			scena[proiectil1_x][proiectil1_y + 1] = CHR_BIT_GRADAT;
			printf("%c", CHR_BIT_GRADAT);
		}
		if (unghi == 2)//60 grade
		{
			for (int g = 1; g <= putere * 8; g++)
			{
				proiectil1_y--;
				if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_GRADAT)
				{
					gotoXY(proiectil1_x, proiectil1_y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31);
					scena[proiectil1_x][proiectil1_y] = PROIECTIL;
					printf("%c", PROIECTIL);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

					gotoXY(proiectil1_x, proiectil1_y + 1);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
					scena[proiectil1_x][proiectil1_y + 1] = CHR_BIT_GRADAT;
					printf("%c", CHR_BIT_GRADAT);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				}
				proiectil1_x--;
				if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_GRADAT)
				{
					gotoXY(proiectil1_x, proiectil1_y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31);
					scena[proiectil1_x][proiectil1_y] = PROIECTIL;
					printf("%c", PROIECTIL);

					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					gotoXY(proiectil1_x + 1, proiectil1_y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
					scena[proiectil1_x + 1][proiectil1_y] = CHR_BIT_GRADAT;
					printf("%c", CHR_BIT_GRADAT);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				}

				proiectil1_x--;
				if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_GRADAT)
				{
					gotoXY(proiectil1_x, proiectil1_y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31);
					scena[proiectil1_x][proiectil1_y] = PROIECTIL;
					printf("%c", PROIECTIL);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

					gotoXY(proiectil1_x + 1, proiectil1_y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
					scena[proiectil1_x + 1][proiectil1_y] = CHR_BIT_GRADAT;
					printf("%c", CHR_BIT_GRADAT);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				}
				Sleep(185.25);
			}
			for (int g = 1; g <= putere * 8; g++)
			{
				proiectil1_y--;
				if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_PLIN && proiectil1_x == tank2_x[0][1] && proiectil1_y == tank2_y[0][1])
					player1_life -= damage;
				if (scena[proiectil1_x][proiectil1_y] == (char)TEAVA_TUN || scena[proiectil1_x][proiectil1_y] == (char)CAP_TANK)
					player1_life -= damage;
				for (int oo = 0; oo < 2; oo++)
					for (int jj = 0; jj < 3; jj++)
						if (tank1_x[oo][jj] == proiectil1_x && tank1_y[oo][jj] == proiectil1_y)
						{
							player1_life -= damage;
							break;
						}
				if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_PLIN)
				{
					gotoXY(proiectil1_x, proiectil1_y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
					scena[proiectil1_x][proiectil1_y] = CHR_BIT_GRADAT;
					printf("%c", CHR_BIT_GRADAT);
					break;
				}
				if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_GRADAT)
				{
					gotoXY(proiectil1_x, proiectil1_y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31);
					scena[proiectil1_x][proiectil1_y] = PROIECTIL;
					printf("%c", PROIECTIL);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

					gotoXY(proiectil1_x, proiectil1_y + 1);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
					scena[proiectil1_x][proiectil1_y + 1] = CHR_BIT_GRADAT;
					printf("%c", CHR_BIT_GRADAT);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				}
				proiectil1_x++;
				if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_PLIN)
				{
					gotoXY(proiectil1_x, proiectil1_y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
					scena[proiectil1_x][proiectil1_y] = CHR_BIT_GRADAT;
					printf("%c", CHR_BIT_GRADAT);
					break;
				}
				if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_GRADAT)
				{
					gotoXY(proiectil1_x, proiectil1_y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31);
					scena[proiectil1_x][proiectil1_y] = PROIECTIL;
					printf("%c", PROIECTIL);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

					gotoXY(proiectil1_x - 1, proiectil1_y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
					scena[proiectil1_x - 1][proiectil1_y] = CHR_BIT_GRADAT;
					printf("%c", CHR_BIT_GRADAT);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				}
				proiectil1_x++;
				if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_PLIN)
				{
					gotoXY(proiectil1_x, proiectil1_y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
					scena[proiectil1_x][proiectil1_y] = CHR_BIT_GRADAT;
					printf("%c", CHR_BIT_GRADAT);
					break;
				}
				if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_GRADAT)
				{
					gotoXY(proiectil1_x, proiectil1_y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31);
					scena[proiectil1_x][proiectil1_y] = PROIECTIL;
					printf("%c", PROIECTIL);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

					gotoXY(proiectil1_x - 1, proiectil1_y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
					scena[proiectil1_x - 1][proiectil1_y] = CHR_BIT_GRADAT;
					printf("%c", CHR_BIT_GRADAT);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				}

				Sleep(185.25);
			}
			gotoXY(proiectil1_x - 1, proiectil1_y);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
			scena[proiectil1_x - 1][proiectil1_y] = CHR_BIT_GRADAT;
			printf("%c", CHR_BIT_GRADAT);
		}
		if (unghi == 3)//90 grade
		{
			gotoXY(proiectil1_x, proiectil1_y - 1);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
			scena[proiectil1_x][proiectil1_y - 1] = CHR_BIT_GRADAT;
			printf("%c", CHR_BIT_GRADAT);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

			for (int g = 1; g <= putere * 4; g++)
			{
				proiectil1_x--;
				if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_GRADAT)
				{
					gotoXY(proiectil1_x, proiectil1_y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31);
					scena[proiectil1_x][proiectil1_y] = PROIECTIL;
					printf("%c", PROIECTIL);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

					gotoXY(proiectil1_x + 1, proiectil1_y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
					scena[proiectil1_x + 1][proiectil1_y] = CHR_BIT_GRADAT;
					printf("%c", CHR_BIT_GRADAT);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				}
				Sleep(185.25);
			}
			gotoXY(tank2_x[0][0], tank2_y[0][0]);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 20);
			printf("%c", TEAVA_TUN);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			for (int g = 1; g <= putere * 4; g++)
			{
				proiectil1_x++;
				if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_PLIN && proiectil1_x == tank1_x[0][1] && proiectil1_y == tank1_y[0][1])
					player2_life -= damage;
				if (scena[proiectil1_x][proiectil1_y] == (char)TEAVA_TUN || scena[proiectil1_x][proiectil1_y] == (char)CAP_TANK)
					player2_life -= damage;
				for (int oo = 0; oo < 2; oo++)
					for (int jj = 0; jj < 3; jj++)
						if (tank2_x[oo][jj] == proiectil1_x && tank2_y[oo][jj] == proiectil1_y)
						{
							player2_life -= damage;
							break;
						}
				if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_PLIN)
				{
					gotoXY(proiectil1_x, proiectil1_y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
					scena[proiectil1_x][proiectil1_y] = CHR_BIT_GRADAT;
					printf("%c", CHR_BIT_GRADAT);
					break;
				}
				if (scena[proiectil1_x][proiectil1_y] == (char)CHR_BIT_GRADAT)
				{
					gotoXY(proiectil1_x, proiectil1_y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31);
					scena[proiectil1_x][proiectil1_y] = PROIECTIL;
					printf("%c", PROIECTIL);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

					gotoXY(proiectil1_x - 1, proiectil1_y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
					scena[proiectil1_x - 1][proiectil1_y] = CHR_BIT_GRADAT;
					printf("%c", CHR_BIT_GRADAT);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				}
				Sleep(185.25);
			}
			gotoXY(proiectil1_x, proiectil1_y);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
			scena[proiectil1_x][proiectil1_y] = CHR_BIT_GRADAT;
			printf("%c", CHR_BIT_GRADAT);
		}
	}
}
//================================

//MECANICA========================
void verifica_tastele_tank1()
{
	char ch;
	if (_kbhit())
	{
		ch = _getch();
		switch (ch)
		{
		case 'a':
			player1_fuel--;
			miscare_stanga_tank1();

			break;
		case 'd':
			player1_fuel--;
			miscare_dreapta_tank1();
			break;
		}
	}
}
void verifica_tastele_tank2()
{
	char ch;
	if (_kbhit())
	{
		ch = _getch();
		switch (ch)
		{
		case 'a':
			player2_fuel--;
			miscare_stanga_tank2();
			break;
		case 'd':
			player2_fuel--;
			miscare_dreapta_tank2();
			break;
		}
	}
}
void verifica_tastele_mecanica()
{
	char ch;
	if (_kbhit())
	{
		ch = _getch();
		switch (ch)
		{
		case 'p':
			if (putere < 5)
				putere++;
			break;
		case 'o':
			if (putere > 0)
				putere--;
			break;
		case 'u':
			if (unghi < 3)
				unghi++;
			break;
		case 'y':
			if (unghi > 0)
				unghi--;
			break;
		}
	}
}
void verifica_tastele_mecanica_ammo()
{
	char ch;
	if (_kbhit())
	{
		ch = _getch();
		switch (ch)
		{
		case 'm':
			if (ammo < 2)
				ammo++;
			break;
		case 'n':
			if (ammo > 0)
				ammo--;
			break;
		}
	}
}
//================================

//PUTERE==========================
void restore_putere_tank1()
{
	for (int i = 0; i < putere; i++)
	{
		gotoXY(putere_x, putere_y + i);
		printf("%c", ' ');
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}
void restore_putere_tank2()
{
	for (int i = 0; i < putere; i++)
	{
		gotoXY(putere_x, putere_y + 19 + i);
		printf("%c", ' ');
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}
void afiseaza_putere_tank2()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	gotoXY(55, 20);
	printf("PUTERE TANK 2:");
	for (int i = 0; i < putere; i++)
	{
		gotoXY(putere_x, putere_y + 19 + i);
		hidecursor();
		printf("%c", CHR_BIT_GRADAT);
	}
	for (int i = putere; i < 5; i++)
	{
		gotoXY(putere_x, putere_y + 19 + i);
		hidecursor();
		printf("%c", ' ');
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);


}
void afiseaza_putere_tank1()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	gotoXY(55, 1);
	printf("PUTERE TANK 1:");
	for (int i = 0; i < putere; i++)
	{
		gotoXY(putere_x, putere_y + i);
		hidecursor();
		printf("%c", CHR_BIT_GRADAT);
	}
	for (int i = putere; i < 5; i++)
	{
		gotoXY(putere_x, putere_y + i);
		hidecursor();
		printf("%c", ' ');
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);


}
//================================

//UNGHI===========================
void afiseaza_unghi_tank1() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	gotoXY(53, 1);
	printf("UNGHI TANK 1:");
	for (int i = 0; i < unghi; i++)
	{
		gotoXY(unghi_x, unghi_y + i);
		hidecursor();
		printf("%c", CHR_BIT_GRADAT);
	}
	for (int i = unghi; i < 4; i++)
	{
		gotoXY(unghi_x, unghi_y + i);
		hidecursor();
		printf("%c", ' ');
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}
void afiseaza_unghi_tank2() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	gotoXY(53, 20);
	printf("UNGHI TANK 2:");
	for (int i = 0; i < unghi; i++)
	{
		gotoXY(unghi_x, unghi_y + 19 + i);
		hidecursor();
		printf("%c", CHR_BIT_GRADAT);
	}
	for (int i = unghi; i < 4; i++)
	{
		gotoXY(unghi_x, unghi_y + 19 + i);
		hidecursor();
		printf("%c", ' ');
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}
void restore_unghi_tank1() {
	for (int i = 0; i < unghi; i++)
	{
		gotoXY(unghi_x, unghi_y + i);
		printf("%c", ' ');
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}
void restore_unghi_tank2() {
	for (int i = 0; i < unghi; i++)
	{
		gotoXY(unghi_x, unghi_y + 19 + i);
		printf("%c", ' ');
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}
// ===============================

//AMMUNITION TYPES================
void afiseaza_ammo_type()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	gotoXY(53, 40);
	printf("AMMO TYPE:");
	for (int i = 0; i < ammo; i++)
	{
		gotoXY(ammo_x, ammo_y + i);
		hidecursor();
		printf("%c", CHR_BIT_GRADAT);
	}
	for (int i = ammo; i < 3; i++)
	{
		gotoXY(ammo_x, ammo_y + i);
		hidecursor();
		printf("%c", ' ');
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}
void restore_ammo_type()
{
	for (int i = 0; i < ammo; i++)
	{
		gotoXY(ammo_x, ammo_y + i);
		printf("%c", ' ');
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}
void restore_air_attack()
{
	for (int i = 0; i < 20; i++)
	{
		gotoXY(55, 40 + i);
		printf("%c", ' ');
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}
void air_attack() {
	//55 40
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	gotoXY(55, 40);
	printf("AIR ASSAULT: ");
	scanf("%d", &coloana);
	air_attack_x = 10;
	air_attack_y = coloana;
	while (1)
	{
		air_attack_x++;
		for (int oo = 0; oo < 2; oo++)
			for (int jj = 0; jj < 3; jj++)
			{
				if (tank2_x[oo][jj] == air_attack_x && tank2_y[oo][jj] == air_attack_y)
				{
					player2_life -= damage;
					break;
				}
				if (tank1_x[oo][jj] == air_attack_x && tank1_y[oo][jj] == air_attack_y)
				{
					player1_life -= damage;
					break;
				}
			}
		if (scena[air_attack_x][air_attack_y] == (char)CHR_BIT_PLIN)
		{
			gotoXY(air_attack_x, air_attack_y);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
			scena[air_attack_x][air_attack_y] = CHR_BIT_GRADAT;
			printf("%c", CHR_BIT_GRADAT);
			break;
		}
		else if (scena[air_attack_x][air_attack_y] == (char)CHR_BIT_GRADAT)
		{
			gotoXY(air_attack_x, air_attack_y);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31);
			scena[air_attack_x][air_attack_y] = PROIECTIL;
			printf("%c", PROIECTIL);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			
			gotoXY(air_attack_x-1, air_attack_y);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
			scena[air_attack_x-1][air_attack_y] = CHR_BIT_GRADAT;
			printf("%c", CHR_BIT_GRADAT);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		}
		
		Sleep(90);
	}

	gotoXY(air_attack_x - 1, air_attack_y);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
	scena[air_attack_x - 1][air_attack_y] = CHR_BIT_GRADAT;
	printf("%c", CHR_BIT_GRADAT);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

	gotoXY(tank1_x[0][0] - 1, tank1_y[0][0]);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
	scena[tank1_x[0][0] - 1][tank1_y[0][0]] = CHR_BIT_GRADAT;
	printf("%c", CHR_BIT_GRADAT);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

	gotoXY(tank1_x[0][1] - 1, tank1_y[0][1]);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
	scena[tank1_x[0][1] - 1][tank1_y[0][1]] = CHR_BIT_GRADAT;
	printf("%c", CHR_BIT_GRADAT);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

	gotoXY(tank1_x[0][2] - 1, tank1_y[0][2]);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
	scena[tank1_x[0][2] - 1][tank1_y[0][2]] = CHR_BIT_GRADAT;
	printf("%c", CHR_BIT_GRADAT);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

	gotoXY(tank2_x[0][0] - 1, tank2_y[0][0]);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
	scena[tank2_x[0][0] - 1][tank2_y[0][0]] = CHR_BIT_GRADAT;
	printf("%c", CHR_BIT_GRADAT);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

	gotoXY(tank2_x[0][1] - 1, tank2_y[0][1]);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
	scena[tank2_x[0][1] - 1][tank2_y[0][1]] = CHR_BIT_GRADAT;
	printf("%c", CHR_BIT_GRADAT);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

	gotoXY(tank2_x[0][2] - 1, tank2_y[0][2]);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
	scena[tank2_x[0][2] - 1][tank2_y[0][2]] = CHR_BIT_GRADAT;
	printf("%c", CHR_BIT_GRADAT);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}
// ===============================

//PLAYERII JOACA PE RAND==========
void player1_turn()
{
	gotoXY(51, 1);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
	printf("LIFE TANK 1: %d", player1_life);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	gotoXY(51, 20);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
	printf("LIFE TANK 2: %d", player2_life);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

	gotoXY(57, 20);
	printf("OUT OF FUEL");
	putere = 0;
	damage = 0;
	ammo = 0;
	unghi = 0;
	while (player1_fuel)
	{
		for (int k = 0; k < 100; k++)
		{
			verifica_tastele_tank1();
			if (player1_fuel == 0)
				break;
			gotoXY(57, 1);
			hidecursor();
			printf("FUEL TANK 1: %d", player1_fuel);
			Sleep(1);
		}
	}
	gotoXY(57, 1);
	printf("OUT OF FUEL");
	gotoXY(57, 14);
	printf(" ");
	player1_fuel = 7;
	while (_getch() != ' ')
	{
		for (int h = 0; h < 100; h++)
		{
			verifica_tastele_mecanica();
			afiseaza_putere_tank1();
			Sleep(1);
		}
	}
	restore_putere_tank1();
	while (_getch() != ' ')
	{
		for (int j = 0; j < 100; j++)
		{
			verifica_tastele_mecanica();
			afiseaza_unghi_tank1();
			Sleep(1);
		}
	}
	restore_unghi_tank1();
	while (_getch() != ' ')
	{
		for (int gg = 0; gg < 100; gg++)
		{
			verifica_tastele_mecanica_ammo();
			afiseaza_ammo_type();
			Sleep(1);
		}
	}
	restore_ammo_type();
	if (ammo == 0)
		damage = 40;
	if (ammo == 1)
		damage = 80;
	if (ammo == 2)
		damage = 20;
	if (ammo == 0 || ammo == 1)
	{
		initializare_proiectil1();
		calculeaza_pozitie_proiectil1();
	}
	else
	{
		air_attack();
	}
	restore_air_attack();

}
void player2_turn()
{
	gotoXY(51, 20);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
	printf("LIFE TANK 2: %d", player2_life);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	gotoXY(51, 1);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
	printf("LIFE TANK 1: %d", player1_life);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	gotoXY(57, 1);
	printf("OUT OF FUEL");
	putere = 0;
	damage = 0;
	unghi = 0;
	ammo = 0;
	while (player2_fuel)
	{
		for (int k = 0; k < 100; k++)
		{
			verifica_tastele_tank2();
			if (player2_fuel == 0)
				break;
			gotoXY(57, 20);
			printf("FUEL TANK 2: %d", player2_fuel);
			Sleep(1);
		}
	}
	gotoXY(57, 20);
	printf("OUT OF FUEL");
	gotoXY(57, 33);
	printf(" ");
	player2_fuel = 5;
	while (_getch() != ' ')
	{
		for (int h = 0; h < 100; h++)
		{
			verifica_tastele_mecanica();
			afiseaza_putere_tank2();
			Sleep(1);
		}
	}
	restore_putere_tank2();
	while (_getch() != ' ')
	{
		for (int j = 0; j < 100; j++)
		{
			verifica_tastele_mecanica();
			afiseaza_unghi_tank2();
			Sleep(1);
		}
	}
	restore_unghi_tank2();
	while (_getch() != ' ')
	{
		for (int gg = 0; gg < 100; gg++)
		{
			verifica_tastele_mecanica_ammo();
			afiseaza_ammo_type();
			Sleep(1);
		}
	}
	restore_ammo_type();
	if (ammo == 0)
		damage = 40;
	if (ammo == 1)
		damage = 80;
	if (ammo == 2)
		damage = 20;
	if (ammo == 0 || ammo == 1)
	{
		initializare_proiectil2();
		calculeaza_pozitie_proiectil2();
	}
	else
	{
		air_attack();
	}
	restore_air_attack();
}
//================================

void main()
{
	hidecursor();
	fullscreen();
	initializare_scena();
	initializare_tank1();
	initializare_tank2();
	afisare_scena();
	pozitioneaza_tank1();
	pozitioneaza_tank2();
	while (!joc_pierdut(player1_life, player2_life))
	{
		player1_turn();
		if (player2_life < 1)
			break;
		if (player1_life < 1)
			break;
		player2_turn();
		if (player1_life < 1)
			break;
		if (player2_life < 1)
			break;
	}
	gotoXY(60, 1);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 71);
	if (player1_life < 1)
		printf("FELICITARI PLAYER 2! AI CASTIGAT!");
	else
		printf("FELICITARI PLAYER 1! AI CASTIGAT!");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}