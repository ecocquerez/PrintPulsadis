/*
 * printpulsadis.c
 *
 *  Created on: 17 janv. 2014
 *      Author: eric
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "SerialDefine.h"

Pulsadis * pPulsadis = NULL;

int OpenSharedMem(void);

int main(int argc, char * argv[])
{
	int mem;
	mem = OpenSharedMem();
	if(mem < 0)
	{
		printf("Error while reading Shared memory");
		return 1;
	}
	printf("compteur : %s\n",pPulsadis->Adco);
	printf("Option tarifaire : ");
	switch(pPulsadis->OptTarifaire)
	{
	case Base:
		printf("Base\n");
		break;
	case Creuse:
		printf("Heure Creuse\n");
		break;
	case Ejp:
		printf("EJP\n");
		break;
	case Tempo:
		printf("Tempo\n");
		break;
	}
	printf("Puissance souscrite : %d\n",pPulsadis->IntensiteSouscrite);
	printf("Index Base : %d\n",pPulsadis->IndexBase);
	printf("Index heure Creuse : %d\n",pPulsadis->IndexHeureCreuse);
	printf("Index Heure Pleine : %d\n",pPulsadis->IndexHeurePleine);
	printf("Index EJP Creuse : %d \n",pPulsadis->IndexEjpCreuse);
	printf("Index EJP Pleine : %d \n",pPulsadis->IndexEjpPleine);
	printf("Index Gaz : %d\n",pPulsadis->IndexGaz);
	printf("Index Autre : %d\n",pPulsadis->IndexAutre);
	printf("Index Bleu Creuse : %d\n",pPulsadis->IndexBleuCreuse);
	printf("Index Bleu Pleine : %d\n",pPulsadis->IndexBleuPleine);
	printf("Index Blanc Creuse : %d\n",pPulsadis->IndexBlancCreuse);
	printf("Index Blanc Pleine : %d\n",pPulsadis->IndexBlancPleine);
	printf("Index Rouge Creuse : %d\n",pPulsadis->IndexRougeCreuse);
	printf("Index Rouge Pleine : %d\n",pPulsadis->IndexRougePleine);
	printf("Preavis EJP : %d\n", pPulsadis->PreavisEjp);
	printf("Periode tarifaire : ");
	switch(pPulsadis->periode)
	{
	case ToutesHeures:
		printf("Toutes heures\n");
		break;
	case HeureCreuse:
		printf("Heures Creuses\n");
		break;
	case HeurePleine:
		printf("Heures Pleines\n");
		break;
	case HeureNormale:
		printf("Heures Normales\n");
		break;
	case JourDePointeMobile:
		printf("Effacement Jour de pointe\n");
		break;
	case HeureCreuseJourBleu:
		printf("Bleu Creuse\n");
		break;
	case HeurePleineJourBleu:
		printf("Bleu Pleine\n");
		break;
	case HeureCreuseJourBlanc:
		printf("Blanc Creuse\n");
		break;
	case HeurePleineJourBlanc:
		printf("Blanc Pleine\n");
		break;
	case HeureCreuseJourRouge:
		printf("Rouge Creuse\n");
		break;
	case HeurePleineJourRouge:
		printf("Rouge Pleine\n");
		break;
	}
	printf("Couleur de demain :");
	switch(pPulsadis->demain)
	{
	case Inconnu:
		printf("Inconnu\n");
		break;
	case Bleu:
		printf("Bleu\n");
		break;
	case Blanc:
		printf("Blanc\n");
		break;
	case Rouge:
		printf("Rouge\n");
		break;
	}
	printf("Intensite instantanee : %d\n",pPulsadis->Instantanee);
	printf("Depassement : %d\n",pPulsadis->Depassement);
	printf("Intensite maximale : %d\n",pPulsadis->IntensiteMaximale);
	printf("Puissance apparente : %d\n",pPulsadis->PuissanceApparente);
	printf("Mot d'etat : %s\n",pPulsadis->MotDetat);
	close(mem);
	return 0;
}

int OpenSharedMem(void)
{
	int mem;

	mem = shm_open("SerialInfo.Pulsadis",O_CREAT | O_RDONLY, 0444);
	if(mem < 0)
	{
		mem = shm_open("SerialInfo.Pulsadis",O_RDONLY, 0444);
		if(mem < 0)
		{
			//syslog(LOG_ERR,"Erreur while allocating memory");
			return -1;
		}
	}
	ftruncate(mem,sizeof(Pulsadis));
	pPulsadis = mmap(0,sizeof(Pulsadis),PROT_READ ,MAP_SHARED,mem,0);
	if(pPulsadis == MAP_FAILED)
	{
		//syslog(LOG_ERR,"mmap failed");
		return -2;
	}
	return mem;
}
