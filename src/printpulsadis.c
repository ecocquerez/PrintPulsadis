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
	char szOPtTarif[15];
	char szPeriodeTarifaire[30];
	char szDemain[15];
	int mem;
	mem = OpenSharedMem();
	if(mem < 0)
	{
		printf("Error while reading Shared memory");
		return 1;
	}
	switch(pPulsadis->OptTarifaire)
	{
	case Base:
		strcpy(szOPtTarif,"Base");
		break;
	case Creuse:
		strcpy(szOPtTarif,"Heure Creuse");
		break;
	case Ejp:
		strcpy(szOPtTarif,"EJP");
		break;
	case Tempo:
		strcpy(szOPtTarif,"Tempo");
		break;
	default:
		strcpy(szOPtTarif,"Inconnu");
	}
	switch(pPulsadis->periode)
	{
	case ToutesHeures:
		strcpy(szPeriodeTarifaire,"Toutes heures");
		break;
	case HeureCreuse:
		strcpy(szPeriodeTarifaire,"Heures Creuses");
		break;
	case HeurePleine:
		strcpy(szPeriodeTarifaire,"Heures Pleines");
		break;
	case HeureNormale:
		strcpy(szPeriodeTarifaire,"Heures Normales");
		break;
	case JourDePointeMobile:
		strcpy(szPeriodeTarifaire,"Effacement Jour de pointe");
		break;
	case HeureCreuseJourBleu:
		strcpy(szPeriodeTarifaire,"Heures Creuses Jours Bleus");
		break;
	case HeurePleineJourBleu:
		strcpy(szPeriodeTarifaire,"Heures Pleines Jours Bleus");
		break;
	case HeureCreuseJourBlanc:
		strcpy(szPeriodeTarifaire,"Heures Creuses Jours Blancs");
		break;
	case HeurePleineJourBlanc:
		strcpy(szPeriodeTarifaire,"Heures Pleines Jours Blancs");
		break;
	case HeureCreuseJourRouge:
		strcpy(szPeriodeTarifaire,"Heures Creuses Jours Rouges");
		break;
	case HeurePleineJourRouge:
		strcpy(szPeriodeTarifaire,"Heures Pleines Jours Rouges");
		break;
	default:
		strcpy(szPeriodeTarifaire,"Inconnu");
	}
	switch(pPulsadis->demain)
	{
	case Inconnu:
		strcpy(szDemain,"Inconnu");
		break;
	case Bleu:
		strcpy(szDemain,"Bleu");
		break;
	case Blanc:
		strcpy(szDemain,"Blanc");
		break;
	case Rouge:
		strcpy(szDemain,"Rouge");
		break;
	}

	printf("{\"ADCO\":\"%s\",\"OPTARIF\":\"%s\",\"ISOUSC\":%d,\"BASE\":%d,\"HCHC\":%d,\"HCHP\":%d,\"EJPHN\":%d,\"EJPHPM\":%d,\"GAZ\":%d,\"AUTRE\":%d,\"BBRHCJB\":%d,\"BBRHPJB\":%d,\"BBRHCJW\":%d,\"BBRHPJW\":%d,\"BBRHCJR\":%d,\"BBRHPJR\":%d,\"PEJP\":%d,\"PTEC\":\"%s\",\"DEMAIN\":\"%s\",\"IINST\":%d,\"ADPS\":%d,\"IMAX\":%d,\"PAPP\":%d,\"MOTDETAT\":\"%s\"}\n",
			pPulsadis->Adco,szOPtTarif,pPulsadis->IntensiteSouscrite
			,pPulsadis->IndexBase,pPulsadis->IndexHeureCreuse,pPulsadis->IndexHeurePleine
			,pPulsadis->IndexEjpCreuse,pPulsadis->IndexEjpPleine,pPulsadis->IndexGaz,pPulsadis->IndexAutre
			,pPulsadis->IndexBleuCreuse,pPulsadis->IndexBleuPleine,pPulsadis->IndexBlancCreuse,pPulsadis->IndexBlancPleine
			,pPulsadis->IndexRougeCreuse,pPulsadis->IndexRougePleine, pPulsadis->PreavisEjp
			,szPeriodeTarifaire,szDemain,pPulsadis->Instantanee,pPulsadis->Depassement,pPulsadis->IntensiteMaximale
			,pPulsadis->PuissanceApparente,pPulsadis->MotDetat
			);

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
