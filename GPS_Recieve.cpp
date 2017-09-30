// ParseFromSerial.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//
// Ubloxparser.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "ublox.h"
#include "GPS_Recieve.h"
#include <Windows.h>

using namespace std;

void GPS_Recieve::openPort()
{
	hComm = CreateFile(str, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hComm == INVALID_HANDLE_VALUE)
		printf("Error in opening serial port\n");
	else
		printf("opening serial port successful\n");

	dcbSerialParams = { 0 }; // Initializing DCB structure
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

	bool Status = GetCommState(hComm, &dcbSerialParams);

	dcbSerialParams.BaudRate = CBR_9600;  // Setting BaudRate = 9600
	dcbSerialParams.ByteSize = 8;         // Setting ByteSize = 8
	dcbSerialParams.StopBits = ONESTOPBIT;// Setting StopBits = 1
	dcbSerialParams.Parity = NOPARITY;  // Setting Parity = None

	SetCommState(hComm, &dcbSerialParams);

	timeouts = { 0 };
	timeouts.ReadIntervalTimeout = 50; // in milliseconds
	timeouts.ReadTotalTimeoutConstant = 50; // in milliseconds
	timeouts.ReadTotalTimeoutMultiplier = 10; // in milliseconds
	timeouts.WriteTotalTimeoutConstant = 50; // in milliseconds
	timeouts.WriteTotalTimeoutMultiplier = 10; // in milliseconds
}

void GPS_Recieve::getSerial(float* lon, float* lat, uint8_t* minute, uint8_t* second) {//Buffer for storing Rxed Data

	char TempChar; //Temporary character used for reading
	DWORD NoBytesRead;
	Ublox gps;
	int i = 0;

	while (1) {
		ReadFile(hComm,           //Handle of the Serial port
			&TempChar,       //Temporary character
			sizeof(TempChar),//Size of TempChar
			&NoBytesRead,    //Number of bytes read
			NULL);

		if (NoBytesRead > 0)
			gps.encode(TempChar);

		if (TempChar == '\n') {
			break;
		}
	}
	*lon = gps.longitude;
	*lat = gps.latitude;
	*minute = gps.datetime.minutes;
	*second = gps.datetime.seconds;
}

void GPS_Recieve::closePort() {
	// Close HANDLE Comm
	CloseHandle(hComm);
}
Ublox::Tokeniser::Tokeniser(char* _str, char _token)
{
	str = _str;
	token = _token;
}


bool Ublox::Tokeniser::next(char* out, int len)
{
	uint8_t count = 0;

	if (str[0] == 0)
		return false;

	while (true)
	{
		if (str[count] == '\0')
		{
			out[count] = '\0';
			str = &str[count];
			return true;
		}

		if (str[count] == token)
		{
			out[count] = '\0';
			count++;
			str = &str[count];
			return true;
		}

		if (count < len)
			out[count] = str[count];

		count++;
	}
	return false;
}


bool Ublox::encode(char c)
{
	buf[pos] = c;
	pos++;

	if (c == '\n') //linefeed
	{
		//cout << buf << endl;
		bool ret = process_buf();

		memset(buf, '\0', 120);
		pos = 0;
		return ret;
	}

	if (pos >= 120) //avoid a buffer overrun
	{
		memset(buf, '\0', 120);
		pos = 0;
	}
	return false;
}


bool Ublox::process_buf()
{
	/*
	if (!check_checksum()) //if checksum is bad
	{
	return false; //return
	}
	*/

	//otherwise, what sort of message is it
	if (strncmp(buf, "$GNGGA", 6) == 0)

	{
		read_gga();
	}
	if (strncmp(buf, "$GNGSA", 6) == 0)
	{
		//read_gsa();
	}

	if (strncmp(buf, "$GPGSV", 6) == 0)
	{
		//read_gsv();
	}

	if (strncmp(buf, "$GNRMC", 6) == 0)

	{
		//read_rmc();
	}
	if (strncmp(buf, "$GNVTG", 6) == 0)
	{
		//read_vtg();
	}
	return true;
}

// GNGGA 
void Ublox::read_gga()
{
	int counter = 0;
	char token[20];
	Tokeniser tok(buf, ',');

	while (tok.next(token, 20))
	{
		switch (counter)
		{
		case 1: //time
		{
			float time = atof(token);
			int hms = int(time);

			datetime.millis = time - hms;
			datetime.seconds = fmod(hms, 100);
			hms /= 100;
			datetime.minutes = fmod(hms, 100);
			hms /= 100;
			datetime.hours = hms;

			//time_age = millis();
		}
		break;
		case 2: //latitude
		{
			float llat = atof(token);
			int ilat = llat / 100;
			double mins = fmod(llat, 100);
			latitude = ilat + (mins / 60);
		}
		break;
		case 3: //north/south
		{
			if (token[0] == 'S')
				latitude = -latitude;
		}
		break;
		case 4: //longitude
		{
			float llong = atof(token);
			int ilat = llong / 100;
			double mins = fmod(llong, 100);
			longitude = ilat + (mins / 60);
		}
		break;
		case 5: //east/west
		{
			if (token[0] == 'W')
				longitude = -longitude;
			//latlng_age = millis();
		}
		break;
		case 6:
		{
			fixtype = _fixtype(atoi(token));
		}
		break;
		case 7:
		{
			sats_in_use = atoi(token);
		}
		break;
		case 8:
		{
			hdop = atoi(token);
		}
		break;
		case 9:
		{
			float new_alt = atof(token);
			//vert_speed = (new_alt - altitude) / ((millis() - alt_age) / 1000.0);
			altitude = atof(token);
			//alt_age = millis();
		}
		break;
		}
		counter++;
	}
}


bool Ublox::check_checksum()
{
	if (buf[strlen(buf) - 5] == '*')
	{
		uint16_t sum = parse_hex(buf[strlen(buf) - 4]) * 16;
		sum += parse_hex(buf[strlen(buf) - 3]);

		for (uint8_t i = 1; i < (strlen(buf) - 5); i++)
			sum ^= buf[i];
		if (sum != 0)
			return false;

		return true;
	}
	return false;
}


uint8_t Ublox::parse_hex(char c)
{
	if (c < '0')
		return 0;
	if (c <= '9')
		return c - '0';
	if (c < 'A')
		return 0;
	if (c <= 'F')
		return (c - 'A') + 10;
	return 0;
}
