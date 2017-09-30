
#pragma once
class GPS_Map
{
private:
	Node** corner;
public:
	GPS_Map();
	~GPS_Map();
	//블럭의 열 갯수, 행 갯수, 시작지점[x좌표][y좌표][위치](위=0 오른쪽=1 아래=2 왼쪽=3), gps값 위치가 들어간 파일 이름
	Node* createMap(int row, int col, int* start_pos, const char* gps_data);
	void removeMap(int row, int col);
};

class Node {
public:
	float longitude;
	float latitude;
	Node* back;
	Node* front;
	Node* right;
	Node* left;
	Node();
	~Node();
};

