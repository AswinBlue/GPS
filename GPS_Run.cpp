// GPSRun.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "GPS_Map.h"
#include "GPS_Recieve.h"
#include <math.h>

typedef struct Direction {
	int dir;
	Direction* next;
}Direction;

class GPS_Run
{
private :
	Direction direction;
	Node* current;
	Node* next;
	float a;	//y = ax + b
	float b;

public :
	void getLine() {
		a = (current->latitude - next->latitude) / (current->longitude - next->longitude);
		b = a * current->longitude + current->latitude;
	}
	float calculateError(float lon, float lat) {
		return fabsf(a*lon - lat + b)/sqrt(powf(a,2)+1);
	}

	void main() {//실제 main에서 구현되어야할 부분
		int row, col, start_pos[3] = { 0,0,1 };//시작점은 0,0 번쨰 vertex의 우측통로
		GPS_Map map;
		GPS_Recieve gps;
		Node *start_node;
		Direction *direction;
		
		//블럭 행, 블럭 열, {시작점x,시작점y,시작위치[0=위,1=우,2=아래,3=좌]}, GPS파일
		start_node = map.createMap(row, col, start_pos, "in.txt");

	//경로 받아오기

		//gps 포트 열기
		gps.openPort();

		float longitude, latitude;
		uint8_t minute, second;
		float err;

	//while문으로 끝날떄까지 반복 되어야함

		//현재 좌표 받기
		gps.getSerial(&longitude, &latitude, &minute, &second);

		//direction을 따라 방향을 전환하고 현재 gps좌표가 각 node의 gps좌표에 도착하면 
		//current와 next변환

		err = calculateError(longitude, latitude);
		
		//err에 따른 방향조정
		

		//종료, 닫기
		gps.closePort();

		//동적할당 해제
		map.removeMap(row, col);
	}
};
