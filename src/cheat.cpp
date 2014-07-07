/*

	PROJECT:		mod_sa
	LICENSE:		See LICENSE in the top level directory
	COPYRIGHT:		Copyright we_sux, FYP

	mod_sa is available from http://code.google.com/p/m0d-s0beit-sa/

	mod_sa is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	mod_sa is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with mod_sa.  If not, see <http://www.gnu.org/licenses/>.

*/
#include "main.h"
#include <psapi.h>

/* Note that this is 100% C code, not C++. :p */
#ifndef MIN
#define MIN( a, b ) ( (a) > (b) ? (b) : (a) )
#endif

// old camera thing, still used in RenderMapDot()
float						*cam_matrix = (float *)0xB6F99C;

uint64_t					__time_current;
double						g_timeDiff;

struct pool					*pool_actor;
struct pool					*pool_vehicle;

static struct cheat_state	__cheat_state;
struct cheat_state			*cheat_state = NULL;



// debug display function
float g_f_debugDisplay[10];
CVector g_vec_debugDisplay[10];
void cheat_main_display_debug_stuff ( double time_diff )
{

	// drawing some stuff
	/*
	int lineSpace = 0;
	char buf[256];
	*/

	/* template
	
	sprintf( buf, "fPhi: %0.02f", g_f_debugDisplay[0] );
	//sprintf( buf, "CCam Front: 0x%08x", pGameInterface->GetCamera()->GetCam(pGameInterface->GetCamera()->GetActiveCam())->GetFront() );
	pD3DFontFixed->PrintShadow(50, 500 + lineSpace, D3DCOLOR_XRGB(0, 200, 0), buf);
	lineSpace += 12;
	sprintf( buf, "FrontOffset: %0.02f", g_f_debugDisplay[1] );
	pD3DFontFixed->PrintShadow(50, 500 + lineSpace, D3DCOLOR_XRGB(0, 200, 0), buf);
	lineSpace += 12;
	*/
	/*
	sprintf( buf, "My Beta: %0.02f", g_f_debugDisplay[2] );
	pD3DFontFixed->PrintShadow(50, 500 + lineSpace, D3DCOLOR_XRGB(0, 200, 0), buf);
	lineSpace += 12;
	

	/* template

	CVector vecSpeed;
	pPedSelf->GetMoveSpeed(&vecSpeed);
	CVector *position = pPedSelf->GetPosition();
	D3DXVECTOR3 ainfoPos(position->fX, position->fY, position->fZ);
	D3DXVECTOR3 vecSpeedD3D(vecSpeed.fX, vecSpeed.fY, vecSpeed.fZ);
	render->DrawLine( ainfoPos, ainfoPos + (vecSpeedD3D * 10), D3DCOLOR_ARGB(255, 0, 255, 0) );
	*/

	// uncomment templates here and tweak as needed

}


/*
#pragma pack(1)
typedef struct _ONFOOT_SYNC_DATA
{
	WORD lrAnalog;
	WORD udAnalog;
	WORD wKeys;
	float vecPos[3];
	float fQuaternion[4];
	BYTE byteHealth;
	BYTE byteArmour;
	BYTE byteCurrentWeapon;
	BYTE byteSpecialAction;
	float vecMoveSpeed[3];
	float vecSurfOffsets[3];
	WORD wSurfInfo;
	int	iCurrentAnimationID;
} ONFOOT_SYNC_DATA;

void tp2()
{
	while (true) {
		if (cheat_state->antiafk) {
			struct actor_info	*self = actor_info_get(ACTOR_SELF, ACTOR_ALIVE);

			if (self != NULL) {

				ONFOOT_SYNC_DATA ofSync;
				BitStream bsPlayerSync;

				memset(&ofSync, 0, sizeof(ONFOOT_SYNC_DATA));
				ofSync.byteHealth = self->hitpoints;
				ofSync.byteArmour = self->armor;
				ofSync.fQuaternion[0] = self->spin[0];
				ofSync.fQuaternion[1] = self->spin[1];
				ofSync.fQuaternion[2] = self->spin[2];
				ofSync.vecPos[0] = self->step_pos[0];
				ofSync.vecPos[1] = self->step_pos[1];
				ofSync.vecPos[2] = self->step_pos[2];

				bsPlayerSync.Write((BYTE)ID_PLAYER_SYNC);
				bsPlayerSync.Write((PCHAR)&ofSync, sizeof(ONFOOT_SYNC_DATA));

				g_RakClient->Send(&bsPlayerSync, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
				cheat_state_text("ANTIAFK HIT");
			}
		}
		Sleep(40);
	}
}

void tp3()
{
	while (true) {
		if (cheat_state->antiafk) {
			BitStream bsParams;
			int iStrlen = strlen("/nextpaycheck");
			bsParams.Write(iStrlen);
			bsParams.Write("/nextpaycheck", iStrlen);
			g_RakClient->RPC(RPC_ServerCommand, &bsParams);
		}
		Sleep(20000);
	}
}

*/

#pragma pack(1)
typedef struct _BULLET_SYNC_DATA {
	BYTE bHitType;
	unsigned short iHitID;
	float fHitOrigin[3];
	float fHitTarget[3];
	float fCenterOfHit[3];
} BULLET_SYNC_DATA; // by 0x688

bool primeravez = true;
DWORD Ticks = 0;
int TickSay = 18;
float lifehist = 100.0f;

float getRandomFloat() {
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

void CalcWorldCoors2(D3DXVECTOR3 *vecScreen, D3DXVECTOR3 *vecWorld)
{
	// Get the static view matrix as D3DXMATRIX
	D3DXMATRIX	m((float *)(0xB6FA2C));

	// Invert the view matrix
	D3DXMATRIX minv;
	memset(&minv, 0, sizeof (D3DXMATRIX));
	m._44 = 1.0f;
	D3DXMatrixInverse(&minv, NULL, &m);

	DWORD		*dwLenX = (DWORD *)(0xC17044);
	DWORD		*dwLenY = (DWORD *)(0xC17048);

	// Reverse screen coordinates
	double fRecip = (double)1.0 / vecScreen->z;
	vecScreen->x /= (float)(fRecip * (*dwLenX));
	vecScreen->y /= (float)(fRecip * (*dwLenY));

	// Do an (inverse) transformation
	vecWorld->x = (vecScreen->z * minv._31) + (vecScreen->y * minv._21) + (vecScreen->x * minv._11) + minv._41;
	vecWorld->y = (vecScreen->z * minv._32) + (vecScreen->y * minv._22) + (vecScreen->x * minv._12) + minv._42;
	vecWorld->z = (vecScreen->z * minv._33) + (vecScreen->y * minv._23) + (vecScreen->x * minv._13) + minv._43;
}

bool isOldAimKeyDown = false;
int aimidf = 9999;
int tickfaia = 0;

void screenshut() {
	char scFileName[200];
	time_t t = time(0);
	struct tm * now = localtime(&t);

	_snprintf_s(scFileName, sizeof(scFileName)-1, "ScreenShots/SS %d-%d-%d %d-%d-%d %s (%d) (%0.2f,%0.2f,%0.2f,%d).jpg", now->tm_year + 1900, now->tm_mon + 1, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec, //
		getPlayerName(g_Players->sLocalPlayerID), g_Players->sLocalPlayerID,
		cheat_state->actor.coords[0], cheat_state->actor.coords[1], cheat_state->actor.coords[2], gta_interior_id_get());

	pGameInterface->TakeScreenshot("tmp.jpg");
	char oldname[] = "tmp.jpg";
	//char newname[] = "newfile.txt";

	rename(oldname, scFileName);
	addMessageToChatWindow("Screenshot: %s", scFileName);
}

void reeplachar2(char *buff, char old, char neo){
	char *ptr;
	for (;;){
		ptr = strchr(buff, old);
		if (ptr == NULL) break;
		buff[(int)(ptr - buff)] = neo;
	}
	return;
}


DWORD AcheckTicks = 0;
extern size_t write_data3(void *ptr, size_t size, size_t nmemb, FILE *stream) {
	size_t written;
	written = fwrite(ptr, size, nmemb, stream);
	return written;
}

void acheck() {
	//while (true) {

	//Sleep(60000);
	//cheat_state_text("modadmins UPDATE");
	char url2[700];
	sprintf(url2, "http://amsspecialist.com/pirula/adm.php");

	CURL *curl;
	FILE *fp;
	CURLcode res;
	char outfilename[FILENAME_MAX] = "aCheck.txt";
	curl = curl_easy_init();
	if (curl) {
		fp = fopen(outfilename, "wb");
		curl_easy_setopt(curl, CURLOPT_URL, url2);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data3);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		fclose(fp);

		FILE* f;
		char line[1000];
		unsigned n = 0;

		f = fopen("aCheck.txt", "r");
		if (f)
		{

			for (int x = 0; x <= 1000; x = x + 1)
			{
				aCheckID[x] = false;
			}

			while (fgets(line, 1000, f))
			{
				for (int x = 0; x <= 1000; x = x + 1)
				{
					const char *pname = NULL;
					pname = getPlayerName(x);
					if (pname != NULL) {
						if (strstr(line, pname)) {
							aCheckID[x] = true;
							//cheat_state_text("modadmins UPDATE");
						}
					}
				}
			}

		}
		fclose(f);
	}


	//}*/
}

static void cheat_main_actor ( double time_diff )
{
	traceLastFunc( "cheat_main_actor()" );

	struct actor_info	*info = actor_info_get( ACTOR_SELF, 0 );
	if ( info == NULL )
	{
		Log( "wtf, actor_info_get() returned NULL." );
		return;
	}

	
	if (primeravez) {
		primeravez = false;
		cheat_state->firingun = 1;
		cheat_state->damagedivider = 3.0f;
		info->hitpoints = 100.0f;
		snprintf(cheat_state->coordsearch, sizeof(cheat_state->coordsearch), "");
		cheat_state->coordsearch[1] = 0x00;
		//cheat_state_text("ANTIAFK REGISTERED");
		//CreateThread(0, 0, (LPTHREAD_START_ROUTINE)tpinit, 0, 0, 0);
		AcheckTicks = GetTickCount();
		if (g_Players != NULL) {

				CreateThread(0, 0, (LPTHREAD_START_ROUTINE)acheck, 0, 0, 0);

		}
	}


	//
	/*if (g_Players != NULL) {
		if (AcheckTicks + 60000 < GetTickCount()) {
			AcheckTicks = GetTickCount();
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)acheck, 0, 0, 0);
		}
	}*/

	if (lifehist == NULL)
		lifehist = info->hitpoints;

	if (info->hitpoints == 0.0f)
		info->hitpoints = 100.0f;

	if (info->hitpoints > 100.0f)
		info->hitpoints = 100.0f;

	/*if (info->hitpoints < 15.0f)
		cheat_state->actor.invulnerable = 1;*/

	if (lifehist > info->hitpoints)
		info->hitpoints = lifehist - ((lifehist - info->hitpoints) / cheat_state->damagedivider);

	lifehist = info->hitpoints;

	if (KEY_PRESSED(0x31))// 1
	{
		say("/enter");
	}
	if (KEY_PRESSED(0x32))// 2
	{
		say("/exit");
	}
	
	if (KEY_PRESSED(0x4B))// K   0x46
	{
		cheat_state->spawned = true;
	}

	if (KEY_PRESSED(0x4A))// J   0x4A
	{
		cheat_state->_generic.menu ^= 1;
		cheat_state->quickmenu = true;
	}

	if (KEY_PRESSED(0x38)) {  //8
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)screenshut, 0, 0, 0);
	}


	if (/*cheat_state->spawned*/g_Players!=NULL)
	{

		
		
		
		
		uint8_t	*ptr = (uint8_t *)g_Players->pLocalPlayer - 320;
		uint16_t thedata[5];
		memcpy_safe(&thedata, ptr + 0x2b9, 4);



		if (thedata[0] == 0xffff)
		{
			//cheat_state->playerAimed = -1;
		}
		else {
			cheat_state->playerAimed = thedata[0];
		}



		bool						isAimKeyDown = false;
		bool						isAim2KeyDown = false;
		CControllerConfigManager	*pPadConfig = pGameInterface->GetControllerConfigManager();

		// doesnt seem to work in single player with pPadConfig and keyboard input?
		if (pPadConfig->GetInputType())
		{
			// mouse + keyboard
			if (KEY_DOWN(pPadConfig->GetControllerKeyAssociatedWithAction(FIRE, MOUSE)))
			{
				isAimKeyDown = true;
			}
			else if (KEY_DOWN(pPadConfig->GetControllerKeyAssociatedWithAction(FIRE, KEYBOARD)))
			{
				isAimKeyDown = true;
			}
		}
		else
		{
			// gamepad
			if (KEY_DOWN(pPadConfig->GetControllerKeyAssociatedWithAction(FIRE, JOYSTICK)))
			{
				isAimKeyDown = true;
			}
		}



			if (KEY_DOWN(0x02))
			{
				isAim2KeyDown = true;
			}
			else {
				isAim2KeyDown = false;
			}
		

		
			float pos[3];

			if (isAim2KeyDown) {

				CVehicle *pCVehicleTeleport = NULL;
				CPed     *pCPedTeleport = NULL;
				D3DXVECTOR3 poss, screenposs;
				

				screenposs.x = 960 + 58;
				screenposs.y = 540 - 108;

				screenposs.z = 700.0f;

				CalcWorldCoors2(&screenposs, &poss);

				CVector vecTarget(poss.x, poss.y, poss.z);

				// setup variables
				CVector				vecOrigin, vecGroundPos;
				CColPoint			*pCollision = NULL;
				CEntitySAInterface	*pCollisionEntity = NULL;



				// origin = our camera
				vecOrigin = *pGame->GetCamera()->GetCam(pGame->GetCamera()->GetActiveCam())->GetSource();

				// check for collision
				bool bCollision = GTAfunc_ProcessLineOfSight(&vecOrigin, &vecTarget, &pCollision, &pCollisionEntity,
					1, 1, 1, 1, 0, 0, 0, 0);

				float posss[3];

				if (bCollision && pCollision)
				{

					if (pCollisionEntity && pCollisionEntity->nType == ENTITY_TYPE_VEHICLE)
					{
						pCVehicleTeleport = pGameInterface->GetPools()->GetVehicle((DWORD *)pCollisionEntity);
						if (pCVehicleTeleport)
						{
							//vecGroundPos = *pCVehicleTeleport->GetPosition();
							const struct vehicle_entry *vehicleEntry = gta_vehicle_get_by_id(pCVehicleTeleport->GetModelIndex());
							if (vehicleEntry != NULL)
							{
								int iVehicleID = getVehicleGTAIDFromInterface((DWORD*)pCVehicleTeleport->GetInterface());
								//cheat_state_text("collision %d", translateGTASAMP_pedPool.iSAMPID[iActorID]);
								aimidf = translateGTASAMP_vehiclePool.iSAMPID[iVehicleID];
								cheat_state->RealAIMID = aimidf;
								cheat_state->TypeAIMID = 2;
							}
						}
					}
					// setup some stuff for normal warp
					else if (pCollisionEntity && pCollisionEntity->nType == ENTITY_TYPE_PED)
					{

						pCPedTeleport = pGameInterface->GetPools()->GetPed((DWORD *)pCollisionEntity);
						if (pCPedTeleport)
						{
							//vecGroundPos = *pCPedTeleport->GetPosition();
							int iActorID = getPedGTAIDFromInterface((DWORD*)pCPedTeleport->GetInterface());

							if (iActorID != NULL)
							{
								//sprintf(buf, "Aim-menu (%d)", translateGTASAMP_pedPool.iSAMPID[iActorID]);
								aimidf = translateGTASAMP_pedPool.iSAMPID[iActorID];
								cheat_state->RealAIMID = aimidf;
								cheat_state->TypeAIMID = 1;
							}
						}
					}

				}


				pCollision->Destroy();

			}
			
			//vecGroundPos = *pCollision->GetPosition();
			
			
			//cheat_state_text("collision ID(%d)         %0.2f    %0.2f     Distance(%0.2f)", aimidf, (float)poss.x, (float)poss.y, vect3_dist(&vecOrigin.fX, &vecGroundPos.fX));


		
			CVector lol;
			lol.fX = 0;
			lol.fY = 0;
			lol.fZ = 0;

			//save current aim status
			float posss[3];
			if (cheat_state->TypeAIMID == 1) {
				if (getPlayerPos(cheat_state->RealAIMID, posss)) {
					cheat_state->CurrentAIMstreamed = true;
					cheat_state->CurrentAIMstreamed = 1;
					cheat_state->DistanceAIM = vect3_dist(cheat_state->actor.coords, posss);

					lol.fX = posss[0];
					lol.fY = posss[1];
					lol.fZ = posss[2];
					//cheat_state_text("collision ID(%d)         %0.2f    %0.2f     Distance(%0.2f) Distance(%0.2f)", aimidf, (float)poss.x, (float)poss.y, vect3_dist(&vecOrigin.fX, &vecGroundPos.fX), cheat_state->DistanceAIM);
				}
				else {
					cheat_state->CurrentAIMstreamed = false;
					cheat_state->DistanceAIM = 0.0f;
				}
			}

			if (cheat_state->TypeAIMID == 2) {
				if (g_Vehicles->pSAMP_Vehicle[cheat_state->RealAIMID] != NULL) {
					cheat_state->CurrentAIMstreamed = true;
					cheat_state->DistanceAIM = vect3_dist(cheat_state->actor.coords, &g_Vehicles->pSAMP_Vehicle[cheat_state->RealAIMID]->pGTA_Vehicle->base.matrix[4 * 3]);
					lol.fX = g_Vehicles->pSAMP_Vehicle[cheat_state->RealAIMID]->pGTA_Vehicle->base.matrix[4 * 3];
					lol.fY = g_Vehicles->pSAMP_Vehicle[cheat_state->RealAIMID]->pGTA_Vehicle->base.matrix[13];
					lol.fZ = g_Vehicles->pSAMP_Vehicle[cheat_state->RealAIMID]->pGTA_Vehicle->base.matrix[14];
				}
				else {
					cheat_state->CurrentAIMstreamed = false;
					cheat_state->DistanceAIM = 0.0f;
				}
			}
			/*
			CVector loldir = lol;
			loldir.fZ += 2.3f;
			loldir.fY = 90.0f;

			lol.fZ += 1.1f;

			if (tickfaia == 0) {
				if (cheat_state->CurrentAIMstreamed) {
					if (cheat_state->TypeAIMID == 1) {
						pGameInterface->GetFx()->TriggerGunshot(pCPedTeleport, lol, loldir, true);
						
					}
					else {
						pGameInterface->GetFx()->TriggerTankFire(lol, loldir);
					}
				}
				tickfaia = 10;
			}
			else {
				tickfaia--;
			}
			*/

			


		if (KEY_PRESSED(0x4B))// K   0x46
		{
			cheat_state->_generic.menu ^= 1;
			cheat_state->aimmenu = true;
			cheat_state_text("aimmenu on %d", cheat_state->playerAimed);
		}


		if (isOldAimKeyDown == isAimKeyDown) {

		}
		else {
			if (isAimKeyDown) {

				eWeaponSlot selfSlot = pPedSelf->GetCurrentWeaponSlot();
				switch (selfSlot)
				{
				case WEAPONSLOT_TYPE_UNARMED:
				case WEAPONSLOT_TYPE_MELEE:
				case WEAPONSLOT_TYPE_THROWN:
				case WEAPONSLOT_TYPE_SPECIAL:
				case WEAPONSLOT_TYPE_GIFT:
				case WEAPONSLOT_TYPE_PARACHUTE:
				case WEAPONSLOT_TYPE_DETONATOR:
					//no sin armas pls
					break;

				case WEAPONSLOT_TYPE_HANDGUN:
				case WEAPONSLOT_TYPE_SHOTGUN:
				case WEAPONSLOT_TYPE_SMG:
				case WEAPONSLOT_TYPE_MG:
				case WEAPONSLOT_TYPE_RIFLE:
				case WEAPONSLOT_TYPE_HEAVY:

					if (cheat_state->firingun > 0) {
						cheat_state_text("SENT FAKE BULLET ATTACK TO (%d)", cheat_state->RealAIMID);
						BULLET_SYNC_DATA ofSync;
						memset(&ofSync, 0, sizeof(BULLET_SYNC_DATA));

						if (cheat_state->TypeAIMID == 1)
							ofSync.bHitType = 1;

						if (cheat_state->TypeAIMID == 2)
							ofSync.bHitType = 2;

						ofSync.iHitID = (unsigned short)cheat_state->RealAIMID;

						for (int x = 1; x <= cheat_state->firingun; x = x + 1)
						{
							ofSync.fCenterOfHit[0] = getRandomFloat() / 5.0f;
							ofSync.fCenterOfHit[1] = getRandomFloat() / 5.0f;
							ofSync.fCenterOfHit[2] = getRandomFloat() / 5.0f;

							ofSync.fHitOrigin[0] = cheat_state->actor.coords[0];
							ofSync.fHitOrigin[1] = cheat_state->actor.coords[1];
							ofSync.fHitOrigin[2] = cheat_state->actor.coords[2];

							if (cheat_state->CurrentAIMstreamed) {
								if (cheat_state->TypeAIMID == 1) {
									ofSync.fHitTarget[0] = g_Players->pRemotePlayer[cheat_state->RealAIMID]->pPlayerData->fOnFootPos[0];
									ofSync.fHitTarget[1] = g_Players->pRemotePlayer[cheat_state->RealAIMID]->pPlayerData->fOnFootPos[1];
									ofSync.fHitTarget[2] = g_Players->pRemotePlayer[cheat_state->RealAIMID]->pPlayerData->fOnFootPos[2];
								}
								else {
									ofSync.fHitTarget[0] = g_Vehicles->pSAMP_Vehicle[cheat_state->RealAIMID]->pGTA_Vehicle->base.matrix[4 * 3];
									ofSync.fHitTarget[1] = g_Vehicles->pSAMP_Vehicle[cheat_state->RealAIMID]->pGTA_Vehicle->base.matrix[(4 * 3) + 1];
									ofSync.fHitTarget[2] = g_Vehicles->pSAMP_Vehicle[cheat_state->RealAIMID]->pGTA_Vehicle->base.matrix[(4 * 3) + 2];
								}


								BitStream bsSpecSync;

								bsSpecSync.Write((BYTE)ID_BULLET_SYNC);
								bsSpecSync.Write((PCHAR)&ofSync, sizeof(BULLET_SYNC_DATA));

								g_RakClient->Send(&bsSpecSync, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
							}
						}
					}
					/*
					if (cheat_state->CurrentAIMstreamed) {


						if (cheat_state->firingun>1)



							cheat_state_text("SENT FAKE BULLET ATTACK TO (%d) Multiplier (%d)", cheat_state->RealAIMID, cheat_state->firingun);

						BULLET_SYNC_DATA ofSync;
						memset(&ofSync, 0, sizeof(BULLET_SYNC_DATA));

						if (cheat_state->TypeAIMID == 1) {
							switch (getPlayerState(cheat_state->RealAIMID))
							{
							case PLAYER_STATE_NONE:
								goto pasando;
								break;

							case PLAYER_STATE_ONFOOT:
								ofSync.bHitType = 1;
								ofSync.iHitID = (unsigned short)cheat_state->RealAIMID;

								ofSync.fHitTarget[0] = g_Players->pRemotePlayer[cheat_state->RealAIMID]->pPlayerData->fOnFootPos[0];
								ofSync.fHitTarget[1] = g_Players->pRemotePlayer[cheat_state->RealAIMID]->pPlayerData->fOnFootPos[1];
								ofSync.fHitTarget[2] = g_Players->pRemotePlayer[cheat_state->RealAIMID]->pPlayerData->fOnFootPos[2];
								break;

							case PLAYER_STATE_DRIVER:
								ofSync.bHitType = 2;
								ofSync.iHitID = (unsigned short)g_Players->pRemotePlayer[cheat_state->RealAIMID]->pPlayerData->sVehicleID;

								ofSync.fHitTarget[0] = g_Players->pRemotePlayer[cheat_state->RealAIMID]->pPlayerData->fVehiclePosition[0];
								ofSync.fHitTarget[1] = g_Players->pRemotePlayer[cheat_state->RealAIMID]->pPlayerData->fVehiclePosition[1];
								ofSync.fHitTarget[2] = g_Players->pRemotePlayer[cheat_state->RealAIMID]->pPlayerData->fVehiclePosition[2];
								break;

							case PLAYER_STATE_PASSENGER:
								ofSync.bHitType = 2;
								ofSync.iHitID = (unsigned short)g_Players->pRemotePlayer[cheat_state->RealAIMID]->pPlayerData->sVehicleID;

								ofSync.fHitTarget[0] = g_Players->pRemotePlayer[cheat_state->RealAIMID]->pPlayerData->fVehiclePosition[0];
								ofSync.fHitTarget[1] = g_Players->pRemotePlayer[cheat_state->RealAIMID]->pPlayerData->fVehiclePosition[1];
								ofSync.fHitTarget[2] = g_Players->pRemotePlayer[cheat_state->RealAIMID]->pPlayerData->fVehiclePosition[2];
								break;
							}
						}
						else {
							ofSync.bHitType = 2;
							ofSync.iHitID = (unsigned short)cheat_state->RealAIMID;

							ofSync.fHitTarget[0] = g_Vehicles->pSAMP_Vehicle[cheat_state->RealAIMID]->pGTA_Vehicle->base.matrix[4 * 3];
							ofSync.fHitTarget[1] = g_Vehicles->pSAMP_Vehicle[cheat_state->RealAIMID]->pGTA_Vehicle->base.matrix[(4 * 3) + 1];
							ofSync.fHitTarget[2] = g_Vehicles->pSAMP_Vehicle[cheat_state->RealAIMID]->pGTA_Vehicle->base.matrix[(4 * 3) + 2];
						}
						ofSync.fHitOrigin[0] = cheat_state->actor.coords[0];
						ofSync.fHitOrigin[1] = cheat_state->actor.coords[1];
						ofSync.fHitOrigin[2] = cheat_state->actor.coords[2];

						for (int x = 1; x <= cheat_state->firingun; x = x + 1)
						{
							ofSync.fCenterOfHit[0] = getRandomFloat() / 5.0f;
							ofSync.fCenterOfHit[1] = getRandomFloat() / 5.0f;
							ofSync.fCenterOfHit[2] = getRandomFloat() / 5.0f;

							BitStream bsSpecSync;

							bsSpecSync.Write((BYTE)ID_BULLET_SYNC);
							bsSpecSync.Write((PCHAR)&ofSync, sizeof(BULLET_SYNC_DATA));

							g_RakClient->Send(&bsSpecSync, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
						}
						pasando:;
						//}
					}
					*/
					break;
				}
				//cheat_state_text("yayyyayyayaya");

			}
			else {

			}
		}

		isOldAimKeyDown = isAimKeyDown;
		
		//better aim method lulz
		/*
		if (isOldAimKeyDown == isAimKeyDown) {

		}
		else {
			if (isAimKeyDown) {

				eWeaponSlot selfSlot = pPedSelf->GetCurrentWeaponSlot();
				switch (selfSlot)
				{
					case WEAPONSLOT_TYPE_UNARMED:
					case WEAPONSLOT_TYPE_MELEE:
					case WEAPONSLOT_TYPE_THROWN:
					case WEAPONSLOT_TYPE_SPECIAL:
					case WEAPONSLOT_TYPE_GIFT:
					case WEAPONSLOT_TYPE_PARACHUTE:
					case WEAPONSLOT_TYPE_DETONATOR:
						//no sin armas pls
					break;

					case WEAPONSLOT_TYPE_HANDGUN:
					case WEAPONSLOT_TYPE_SHOTGUN:
					case WEAPONSLOT_TYPE_SMG:
					case WEAPONSLOT_TYPE_MG:
					case WEAPONSLOT_TYPE_RIFLE:
					case WEAPONSLOT_TYPE_HEAVY:
					

						if (cheat_state->playerAimed == NULL)
							cheat_state->playerAimed = 0;

						if (cheat_state->playerAimed != 0xffff || 
							g_Players->pRemotePlayer[cheat_state->playerAimed]->pPlayerData != NULL || 
							g_Players->pRemotePlayer[cheat_state->playerAimed]->pPlayerData->pSAMP_Actor != NULL) {


							if (cheat_state->firingun>1)

								

								cheat_state_text("SENT FAKE BULLET ATTACK TO (%d) Multiplier (%d)", cheat_state->playerAimed, cheat_state->firingun);

								BULLET_SYNC_DATA ofSync;
								memset(&ofSync, 0, sizeof(BULLET_SYNC_DATA));


								switch (getPlayerState(cheat_state->playerAimed))
								{
								case PLAYER_STATE_NONE:
									goto pasando;
									break;

								case PLAYER_STATE_ONFOOT:
									ofSync.bHitType = 1;
									ofSync.iHitID = (unsigned short)cheat_state->playerAimed;

									ofSync.fHitTarget[0] = g_Players->pRemotePlayer[cheat_state->playerAimed]->pPlayerData->fOnFootPos[0];
									ofSync.fHitTarget[1] = g_Players->pRemotePlayer[cheat_state->playerAimed]->pPlayerData->fOnFootPos[1];
									ofSync.fHitTarget[2] = g_Players->pRemotePlayer[cheat_state->playerAimed]->pPlayerData->fOnFootPos[2];
									break;

								case PLAYER_STATE_DRIVER:
									ofSync.bHitType = 2;
									ofSync.iHitID = (unsigned short)g_Players->pRemotePlayer[cheat_state->playerAimed]->pPlayerData->sVehicleID;

									ofSync.fHitTarget[0] = g_Players->pRemotePlayer[cheat_state->playerAimed]->pPlayerData->fVehiclePosition[0];
									ofSync.fHitTarget[1] = g_Players->pRemotePlayer[cheat_state->playerAimed]->pPlayerData->fVehiclePosition[1];
									ofSync.fHitTarget[2] = g_Players->pRemotePlayer[cheat_state->playerAimed]->pPlayerData->fVehiclePosition[2];
									break;

								case PLAYER_STATE_PASSENGER:
									ofSync.bHitType = 2;
									ofSync.iHitID = (unsigned short)g_Players->pRemotePlayer[cheat_state->playerAimed]->pPlayerData->sVehicleID;

									ofSync.fHitTarget[0] = g_Players->pRemotePlayer[cheat_state->playerAimed]->pPlayerData->fVehiclePosition[0];
									ofSync.fHitTarget[1] = g_Players->pRemotePlayer[cheat_state->playerAimed]->pPlayerData->fVehiclePosition[1];
									ofSync.fHitTarget[2] = g_Players->pRemotePlayer[cheat_state->playerAimed]->pPlayerData->fVehiclePosition[2];
									break;
								}

								ofSync.fHitOrigin[0] = cheat_state->actor.coords[0];
								ofSync.fHitOrigin[1] = cheat_state->actor.coords[1];
								ofSync.fHitOrigin[2] = cheat_state->actor.coords[2];

								for (int x = 1; x <= cheat_state->firingun; x = x + 1)
								{
									ofSync.fCenterOfHit[0] = getRandomFloat() / 5.0f;
									ofSync.fCenterOfHit[1] = getRandomFloat() / 5.0f;
									ofSync.fCenterOfHit[2] = getRandomFloat() / 5.0f;

									BitStream bsSpecSync;

									bsSpecSync.Write((BYTE)ID_BULLET_SYNC);
									bsSpecSync.Write((PCHAR)&ofSync, sizeof(BULLET_SYNC_DATA));

									g_RakClient->Send(&bsSpecSync, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
								}
							pasando:;
							//}
						}
					break;
				}
				//cheat_state_text("yayyyayyayaya");
				
			}
			else {

			}
		}

		isOldAimKeyDown = isAimKeyDown;
		*/
		//CControllerConfigManager	*pPadConfig = pGameInterface->GetControllerConfigManager();

//bool crossAct = false;
//float crossHair[2];


//cheat_state_text("CROSSHAIR    %d       %0.2f  %0.2f   ", crossAct, crossHair[0], crossHair[1]);




		

		/*if (KEY_PRESSED(0x37)) {  //7
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)screenshut2, 0, 0, 0);

		}*/
		
		if (KEY_PRESSED(0x55)) {  //U
			cheat_state_text("SENT FAKE BULLET ATTACK TO (%d)", cheat_state->RealAIMID);
			BULLET_SYNC_DATA ofSync;
			memset(&ofSync, 0, sizeof(BULLET_SYNC_DATA));

			if (cheat_state->TypeAIMID==1)
				ofSync.bHitType = 1;

			if (cheat_state->TypeAIMID == 2)
				ofSync.bHitType = 2;

			ofSync.iHitID = (unsigned short)cheat_state->RealAIMID;

			int fg = cheat_state->firingun;
			if (fg < 1) fg = 1;

			for (int x = 1; x <= fg; x = x + 1)
			{
				ofSync.fCenterOfHit[0] = getRandomFloat() / 5.0f;
				ofSync.fCenterOfHit[1] = getRandomFloat() / 5.0f;
				ofSync.fCenterOfHit[2] = getRandomFloat() / 5.0f;

				ofSync.fHitOrigin[0] = cheat_state->actor.coords[0];
				ofSync.fHitOrigin[1] = cheat_state->actor.coords[1];
				ofSync.fHitOrigin[2] = cheat_state->actor.coords[2];

				if (cheat_state->CurrentAIMstreamed) {
					if (cheat_state->TypeAIMID == 1) {
						ofSync.fHitTarget[0] = g_Players->pRemotePlayer[cheat_state->RealAIMID]->pPlayerData->fOnFootPos[0];
						ofSync.fHitTarget[1] = g_Players->pRemotePlayer[cheat_state->RealAIMID]->pPlayerData->fOnFootPos[1];
						ofSync.fHitTarget[2] = g_Players->pRemotePlayer[cheat_state->RealAIMID]->pPlayerData->fOnFootPos[2];
					}
					else {
						ofSync.fHitTarget[0] = g_Vehicles->pSAMP_Vehicle[cheat_state->RealAIMID]->pGTA_Vehicle->base.matrix[4 * 3];
						ofSync.fHitTarget[1] = g_Vehicles->pSAMP_Vehicle[cheat_state->RealAIMID]->pGTA_Vehicle->base.matrix[(4 * 3) + 1];
						ofSync.fHitTarget[2] = g_Vehicles->pSAMP_Vehicle[cheat_state->RealAIMID]->pGTA_Vehicle->base.matrix[(4 * 3) + 2];
					}


				BitStream bsSpecSync;

				bsSpecSync.Write((BYTE)ID_BULLET_SYNC);
				bsSpecSync.Write((PCHAR)&ofSync, sizeof(BULLET_SYNC_DATA));

				g_RakClient->Send(&bsSpecSync, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
				}
			}
			
		}




	}

	if (!cheat_state->issetwsban) {
		if (g_Players != NULL) {
			const char* name = getPlayerName(g_Players->sLocalPlayerID);
			if (name != NULL) {
				sprintf(g_wasban, "%s was banned by", name);
				//replace(wasban, sizeof(wasban), '_', " ");
				reeplachar2(g_wasban, '_', ' ');
				cheat_state->issetwsban = true;
				//cheat_state_text("setup wsban %s", g_wasban);
			}
		}
	}

	

	//GetTickCount

	if (Ticks + 1000 < GetTickCount()) {
		Ticks = GetTickCount();
		
		
		
		//cheat_state_text("aimed to: %d -> start: %8x : %8x", thedata[0], ptr, ptr + 0x2b9);

		/*
		if (cheat_state->antiafk && TickSay>19) {
			BitStream bsParams;
			int iStrlen = strlen("/nextpaycheck");
			bsParams.Write(iStrlen);
			bsParams.Write("/nextpaycheck", iStrlen);
			g_RakClient->RPC(RPC_ServerCommand, &bsParams);
			TickSay = 0;
		}
		TickSay++;
		*/
		/*
		if (cheat_state->antiafk) {
			struct actor_info	*self = actor_info_get(ACTOR_SELF, ACTOR_ALIVE);

			if (self == NULL) return;





			ONFOOT_SYNC_DATA ofSync;
			BitStream bsPlayerSync;

			memset(&ofSync, 0, sizeof(ONFOOT_SYNC_DATA));
			ofSync.byteHealth = self->hitpoints;
			ofSync.byteArmour = self->armor;
			ofSync.fQuaternion[0] = self->spin[0];
			ofSync.fQuaternion[1] = self->spin[1];
			ofSync.fQuaternion[2] = self->spin[2];
			ofSync.vecPos[0] = self->step_pos[0];
			ofSync.vecPos[1] = self->step_pos[1];
			ofSync.vecPos[2] = self->step_pos[2];


			bsPlayerSync.Write((BYTE)ID_PLAYER_SYNC);
			bsPlayerSync.Write((PCHAR)&ofSync, sizeof(ONFOOT_SYNC_DATA));

			g_RakClient->Send(&bsPlayerSync, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
			cheat_state_text("ANTIAFK HIT");
		}
		*/


	}


	cheat_handle_antiHijack( info, NULL, time_diff );

	vect3_copy( &info->base.matrix[4 * 3], cheat_state->actor.coords );
	cheat_handle_freeze_vehicles( NULL, info );
	cheat_handle_hp( NULL, info, time_diff );
	cheat_handle_teleport( NULL, info, time_diff );
	cheat_handle_unfreeze( NULL, info, time_diff );
	cheat_handle_emo( NULL, info, time_diff );

	// the following functions can be found in cheat_actor.cpp
	cheat_handle_actor_air_brake( info, time_diff );
	cheat_handle_stick( NULL, info, time_diff );
	cheat_handle_actor_autoaim( info, time_diff );

	// cheat_handle_SpiderFeet(info, time_diff);
	cheat_handle_actor_fly(info, time_diff);

	if ( set.custom_runanimation_enabled )
		pPedSelf_setMoveAnimation__array( set.custom_runanimation_id );


	// these NEED to stay last, because they can remove the player from the vehicle
	if ( info->pedFlags.bInVehicle )
		cheat_handle_vehicle_fast_exit( NULL, time_diff );
	cheat_handle_exit_vehicle ( NULL, info );
}

static void cheat_main_vehicle ( double time_diff )
{
	traceLastFunc( "cheat_main_vehicle()" );

	struct vehicle_info *info = vehicle_info_get( VEHICLE_SELF, 0 );
	if ( info == NULL )
		return;

	cheat_handle_antiHijack( NULL, info, time_diff );

	// copy vehicle coords to cheat_state storage
	vect3_copy( &info->base.matrix[4 * 3], cheat_state->vehicle.coords );

	// the following functions can be found in cheat_generic.cpp
	cheat_handle_unfreeze( info, NULL, time_diff );
	cheat_handle_teleport( info, NULL, time_diff );
	cheat_handle_stick( info, NULL, time_diff );
	cheat_handle_freeze_vehicles( info, NULL );
	cheat_handle_hp( info, NULL, time_diff );
	cheat_handle_emo( info, NULL, time_diff );

	// the following functions can be found in cheat_vehicle.cpp
	cheat_handle_vehicle_protection( info, time_diff );
	cheat_handle_vehicle_unflip( info, time_diff );
	cheat_handle_vehicle_nitro( info, time_diff );
	cheat_handle_vehicle_air_brake( info, time_diff );
	cheat_handle_vehicle_warp( info, time_diff );
	cheat_handle_vehicle_quick_turn( info, time_diff );
	cheat_handle_vehicle_brake( info, time_diff );
	cheat_handle_vehicle_hop( info, time_diff );
	cheat_handle_vehicle_engine( info, time_diff );
	cheat_handle_vehicle_brakedance( info, time_diff );
	cheat_handle_vehicle_blinking_carlights( info, time_diff );
	cheat_handle_vehicle_fly( info, time_diff );
	cheat_handle_vehicle_keepTrailer( info, time_diff );
	cheat_handle_vehicle_repair_car( info, time_diff );
	cheat_handle_vehicle_spiderWheels( info, time_diff );
	//cheat_handle_vehicle_slowTeleport( info, time_diff );

	cheat_handle_vehicle_recording( info, time_diff );


	// these NEED to stay last, because they can remove the player from the vehicle
	cheat_handle_vehicle_fast_exit( info, time_diff );
	cheat_handle_exit_vehicle ( info, NULL );
}

// the main daddyo
int			m_InitStages = 0;
void cheat_hook ( HWND wnd )
{
	traceLastFunc( "cheat_hook()" );

	// get the time
	static uint64_t time_last;
	__time_current = __time_get();
	g_timeDiff = TIME_TO_DOUBLE(time_get() - time_last);

	// for looping
	int i;

	// install keyhook
	keyhook_maybe_install( wnd );

	// setup menu
	menu_maybe_init();
	traceLastFunc( "cheat_hook()" );

	/* initialize state */
	if ( cheat_state == NULL )
	{
		// set default cheat_state variables
		cheat_state = &__cheat_state;
		cheat_state->_generic.spoof_weapon = -1;
		cheat_state->_generic.money = set.money_enabled;
		cheat_state->_generic.weapon = set.weapon_enabled;
		cheat_state->_generic.vehicles_unlock = false;
		cheat_state->_generic.vehicles_warp_invert = true;
		cheat_state->actor.invulnerable = true;
		cheat_state->vehicle.invulnerable = true;
		cheat_state->vehicle.hp_tire_support = true;
		cheat_state->vehicle.hp_minimum_on = 1;
		cheat_state->vehicle.hp_regen_on = 1;
		cheat_state->actor.hp_regen_on = 1;
		cheat_state->vehicle.brkdance = 0;
		cheat_state->game_speed = 1.0f;
		cheat_state->vehicle.is_engine_on = 1;
		cheat_state->_generic.interior_patch_enabled = 0;

		// refreshes any cheat_state stuff set from the INI
		ini_load_setSettings();

		/* install patches from the .ini file */
		for ( i = 0; i < INI_PATCHES_MAX; i++ )
		{
			if ( set.patch[i].name != NULL && set.patch[i].ini_auto )
				patcher_install( &set.patch[i] );
		}

		if ( g_dwSAMP_Addr != NULL )
		{
			for ( i = 0; i < INI_SAMPPATCHES_MAX; i++ )
			{
				if ( set.sampPatch[i].name != NULL && set.sampPatch[i].ini_auto )
					patcher_install( &set.sampPatch[i] );
			}
		}
	}	/* end initialize state */

	// set cheat state to "off"
	cheat_state->state = CHEAT_STATE_NONE;

	/* setup & refresh actor pool */
	pool_actor = *(struct pool **)ACTOR_POOL_POINTER;
	if ( pool_actor == NULL || pool_actor->start == NULL || pool_actor->size <= 0 )
		return;

	/* setup & refresh vehicle pool */
	pool_vehicle = *(struct pool **)VEHICLE_POOL_POINTER;
	if ( pool_vehicle == NULL || pool_vehicle->start == NULL || pool_vehicle->size <= 0 )
		return;

	//////////////////////////////////////////
	// looks like we have pools so continue //
	struct actor_info	*actor_info;
	struct vehicle_info *vehicle_info;
	actor_info = actor_info_get( ACTOR_SELF, ACTOR_ALIVE );
	vehicle_info = vehicle_info_get( VEHICLE_SELF, 0 );

	/* no vehicle, and no actor. exit. */
	if ( vehicle_info == NULL && actor_info == NULL )
	{
		if ( cheat_state->actor.air_brake
		 ||	 cheat_state->actor.stick
		 ||	 cheat_state->vehicle.air_brake
		 ||	 cheat_state->vehicle.stick )
		{
			cheat_state->actor.air_brake = 0;
			cheat_vehicle_air_brake_set( 0 );
			cheat_state->actor.stick = 0;
			cheat_state->vehicle.stick = 0;
			cheat_state_text( "Air brake / stick disabled" );
		}
	}
	else
	{
		if ( vehicle_info == NULL )
		{
			if ( cheat_state->vehicle.air_brake || cheat_state->vehicle.stick )
			{
				cheat_vehicle_air_brake_set( 0 );
				cheat_state->vehicle.stick = 0;
				cheat_state_text( "Air brake / stick disabled" );
			}

			cheat_state->state = CHEAT_STATE_ACTOR;

			// reset infinite NOS toggle state
			if ( cheat_state->vehicle.infNOS_toggle_on )
			{
				cheat_state->vehicle.infNOS_toggle_on = false;
				patcher_remove( &patch_vehicle_inf_NOS );
			}
		}
		else
		{
			if ( cheat_state->actor.air_brake || cheat_state->actor.stick )
			{
				cheat_state->actor.air_brake = 0;
				cheat_state->actor.stick = 0;
				cheat_state_text( "Air brake / stick disabled" );
			}

			cheat_state->state = CHEAT_STATE_VEHICLE;
		}

		// post GTA initialization initizalization routines
		if ( m_InitStages == 0 )
		{
			// setup CGame & friends
			pGameInterface = new CGameSA();
			pPools = pGameInterface->GetPools();

			//pGameInterface->Initialize(); // all this does so far is disable modshops and pay&sprays
			// this seems to fix SAMP's load hang bug
			//pGameInterface->StartGame();

			// we have to add ourself to the pool first so that we are always the 1st ref
			// NEW --> use the global external: pPedSelf
			pPedSelf = pGameInterface->GetPools()->AddPed( (DWORD *)actor_info );
			pPedSelfSA = pPedSelf->GetPedInterface();

			// install all startup hooks
			cheat_hookers_installhooks();

			// increment stage
			m_InitStages++;
		}
		else if ( m_InitStages == 1 )
		{
			// load all the weapon models
			loadAllWeaponModels();

			// get funky
			//pGameInterface->GetAudio()->PlayBeatTrack(2);

			// increment stage
			m_InitStages++;
		}
	}

	if ( cheat_state->state != CHEAT_STATE_NONE )
	{
		// keep this updated, cos something is switching it now
		pPedSelf = pPools->GetPedFromRef( CPOOLS_PED_SELF_REF );
		pPedSelfSA = pPedSelf->GetPedInterface();
	}

	static bool chat_set_once = false;
	if ( !chat_set_once && set.d3dtext_chat )
	{
		if ( g_Chat != NULL && g_Chat->iChatWindowMode )
		{
			//Log("Disabling SA:MP chat text.");
			g_Chat->iChatWindowMode = 0;
			chat_set_once = true;
		}
	}

	// sets kill text to s0beit mode if enabled from INI
	static bool kill_set_once = false;
	if ( !kill_set_once && set.d3dtext_kill )
	{
		if ( g_DeathList != NULL && g_DeathList->iEnabled )
		{
			g_DeathList->iEnabled = 0;
			kill_set_once = true;
		}
	}

	// more random than random
	if ( rand() % 5000 == 0 )
		srand( rand() + time_get() );

	// OMGWTFBBQ?!?
	if ( cheat_panic() )
	{
		cheat_state->state = CHEAT_STATE_NONE;
		goto out;
	}

	// menus are good
	menu_run();

	// install volatile patches from the .ini file
	for ( i = 0; i < INI_PATCHES_MAX; i++ )
	{
		if ( set.patch[i].name != NULL && set.patch[i].has_volatile && set.patch[i].installed )
			patcher_install( &set.patch[i] );
	}

	// generic stuff - these want to run even though we're dead
	cheat_handle_debug( wnd );
	cheat_handle_misc();
	cheat_handle_spoof_weapon();
	cheat_handle_weapon();
	gta_game_speed_set( cheat_state->game_speed );
	cheat_main_display_debug_stuff(g_timeDiff);

	if ( cheat_state->state != CHEAT_STATE_NONE )
	{
		// generic stuff
		cheat_handle_weapon_disable();
		cheat_handle_money();

		//////////////////////////////////////////////////////////////////
		// this should be removed after reworking weapon cheat function //
		//if ( set.restore_weapons_after_death )
		//	* (uint8_t *)0x5E6320 = 0xC3;

		cheat_handle_unlock();
		cheat_handle_checkpoint();

		// do stuff :p
		if ( cheat_state->state == CHEAT_STATE_VEHICLE )
		{
			cheat_main_vehicle( g_timeDiff );
		}
		else if ( cheat_state->state == CHEAT_STATE_ACTOR )
		{
			cheat_main_actor( g_timeDiff );
		}

		

		if ( KEY_PRESSED(set.key_disable_Wall_Collisions) )
		{
			cheat_state->_generic.nocols_walls_enabled ^= 1;
			if ( cheat_state->_generic.nocols_walls_enabled )
				patcher_install( &patch_NoColsWalls );
			else
				patcher_remove( &patch_NoColsWalls );
		}

		for ( i = 0; i < INI_PATCHES_MAX; i++ )
		{
			if ( set.patch[i].name != NULL && KEY_PRESSED(set.patch[i].ini_hotkey) )
			{
				if ( set.patch[i].installed || set.patch[i].failed )
					patcher_remove( &set.patch[i] );
				else
					patcher_install( &set.patch[i] );
			}
		}

		for ( i = 0; i < INI_SAMPPATCHES_MAX; i++ )
		{
			if ( set.sampPatch[i].name != NULL && KEY_PRESSED(set.sampPatch[i].ini_hotkey) )
			{
				if ( set.sampPatch[i].installed || set.sampPatch[i].failed )
					patcher_remove( &set.sampPatch[i] );
				else
					patcher_install( &set.sampPatch[i] );
			}
		}

		if ( KEY_PRESSED(set.key_vehicle_jumper) )
		{
			int iVehicleID = vehicle_find_nearest( VEHICLE_ALIVE + VEHICLE_NOTBURNING );
			vehicleJumper( iVehicleID );
		}

		if ( KEY_PRESSED(set.key_vehicle_occupied_jumper) )
		{
			int iVehicleID = vehicle_find_nearest( VEHICLE_ALIVE + VEHICLE_NOTBURNING + VEHICLE_OCCUPIED );
			vehicleJumper( iVehicleID );
		}
	}	// cheat_state->state != CHEAT_STATE_NONE

	// hack some SA:MP, shall we?
	if ( g_SAMP && g_renderSAMP_initSAMPstructs )
		sampMainCheat();

out: ;
	if ( gta_menu_active() )
		keyhook_clear_states();
	else
		keyhook_run();
	time_last = time_get();
}
