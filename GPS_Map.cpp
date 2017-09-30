#include "stdafx.h"
#include "GPS_Map.h"


GPS_Map::GPS_Map()
{}


GPS_Map::~GPS_Map()
{}

Node* GPS_Map::createMap(int row, int col, int* start_pos, const char* gps_data){

	
	Node** map;

	corner = new Node*[row+1];
	map = new Node*[row + 1];

	for (int i = 0; i < row+1; ++i) {
		corner[i] = new Node[col + 1];
		map[i] = new Node[col + 1];
	}

	std::ifstream f_in;
	f_in.open(gps_data);
	//making nodes and putting gps info into nodes
	for (int i = 0; i <= row; ++i) {
		for (int j = 0; j <= col; ++j) {
//gps값 넣어야함

			if (i != 0) {
				corner[i][j].front = new Node;
				corner[i][j].front->back = NULL;
				corner[i][j].front->front = NULL;
				corner[i][j].front->left = NULL;
				corner[i][j].front->right = NULL;
				f_in >> corner[i][j].front->longitude;
				f_in >> corner[i][j].front->latitude;

			}
			else corner[i][j].front = NULL;

			if (j != col) {
				corner[i][j].right = new Node;
				corner[i][j].right->back = NULL;
				corner[i][j].right->front = NULL;
				corner[i][j].right->left = NULL;
				corner[i][j].right->right = NULL;
				f_in >> corner[i][j].right->longitude;
				f_in >> corner[i][j].right->latitude;
			}
			else corner[i][j].right = NULL;

			if (i != row) {
				corner[i][j].back = new Node;
				corner[i][j].back->back = NULL;
				corner[i][j].back->front = NULL;
				corner[i][j].back->left = NULL;
				corner[i][j].back->right = NULL;
				f_in >> corner[i][j].back->longitude;
				f_in >> corner[i][j].back->latitude;
			}
			else corner[i][j].back = NULL;

			if (j != 0) {
				corner[i][j].left = new Node;
				corner[i][j].left->back = NULL;
				corner[i][j].left->front = NULL;
				corner[i][j].left->left = NULL;
				corner[i][j].left->right = NULL;
				f_in >> corner[i][j].left->longitude;
				f_in >> corner[i][j].left->latitude;
			}
			else corner[i][j].left = NULL;


		}
	}


	//linking edges 
	for (int i = 0; i <= row; ++i) {
		for (int j = 0; j <= col; ++j) {
			
			//link back to back
			if (i < row && j < col) {
				corner[i][j].right->back = corner[i][j + 1].left;
				corner[i][j + 1].left->back = corner[i][j].right;

				corner[i][j].back->back = corner[i + 1][j].front;
				corner[i + 1][j].front->back = corner[i][j].back;
			}

			//link front to front
			if (i != 0 && i != row) {
				corner[i][j].front->front = corner[i][j].back;
				corner[i][j].back->front = corner[i][j].front;
			}
			if (j != 0 && j != col) {
				corner[i][j].left->front = corner[i][j].right;
				corner[i][j].right->front = corner[i][j].left; 
			}

			//link left & right
			if (i==0 && j==0) {
				corner[i][j].right->left = corner[i][j].back;
				corner[i][j].back->right= corner[i][j].right;
			}
			else if (i == 0 && j == col) {
				corner[i][j].left->right= corner[i][j].back;
				corner[i][j].back->left = corner[i][j].left;
			}
			else if (i == row && j == 0) {
				corner[i][j].front->left = corner[i][j].right;
				corner[i][j].right->right = corner[i][j].front;
			}
			else if (i == row && j == col) {
				corner[i][j].left->left = corner[i][j].front;
				corner[i][j].front->right = corner[i][j].left;
			}
			else if (i == 0) {
				corner[i][j].right->left = corner[i][j].back;
				corner[i][j].back->left = corner[i][j].left;
				corner[i][j].back->right = corner[i][j].right;
				corner[i][j].left->right = corner[i][j].back;
			}
			else if (j == 0) {
				corner[i][j].front->left = corner[i][j].right;
				corner[i][j].right->left = corner[i][j].back;
				corner[i][j].right->right = corner[i][j].front;
				corner[i][j].back->right = corner[i][j].right;
			}
			else if (i == row) {
				corner[i][j].front->left = corner[i][j].right;
				corner[i][j].front->right = corner[i][j].left;
				corner[i][j].right->right = corner[i][j].front;
				corner[i][j].left->left = corner[i][j].front;
			}
			else if (i == col) {
				corner[i][j].front->right = corner[i][j].left;
				corner[i][j].back->left = corner[i][j].left;
				corner[i][j].left->left = corner[i][j].front;
				corner[i][j].left->right = corner[i][j].back;
			}
		}
	}

	switch (start_pos[2]) {
	case 0:
		return corner[start_pos[0]][start_pos[1]].front;
	case 1:
		return corner[start_pos[0]][start_pos[1]].right;
	case 2:
		return corner[start_pos[0]][start_pos[1]].back;
	case 3:
		return corner[start_pos[0]][start_pos[1]].left;
	}

}

void GPS_Map::removeMap(int row, int col) {
	for (int i = 0; i <= row; ++i) {
		for (int j = 0; j <= col; ++j) {
			if (i != 0) 
				delete corner[i][j].front;
			if (i != row)
				delete corner[i][j].back;
			if (j != 0) 
				delete corner[i][j].left;
			if (j != col) 
				delete corner[i][j].right;
		}
	}
}

Node::Node() {}
Node::~Node() {}