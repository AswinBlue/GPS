#pragma once
class GPS_Recieve {
private:
	HANDLE hComm;
	wchar_t str[1000] = L"COM7";	//포트 설정
	DCB dcbSerialParams;
	COMMTIMEOUTS timeouts;

public:
	void openPort();
	void getSerial(float* lon, float* lat, uint8_t* minute, uint8_t* second);//Buffer for storing Rxed Data
	void closePort();
};
