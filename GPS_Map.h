
#pragma once
class GPS_Map
{
private:
	Node** corner;
public:
	GPS_Map();
	~GPS_Map();
	//���� �� ����, �� ����, ��������[x��ǥ][y��ǥ][��ġ](��=0 ������=1 �Ʒ�=2 ����=3), gps�� ��ġ�� �� ���� �̸�
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

