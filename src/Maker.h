#pragma once
#include<fstream>
#include<cstring>
#include<conio.h>
#include<iostream>
#include"Struct.h"
#include"AudioClip.h"

using namespace std;

class Maker
{
public:
	Maker();
	void begintype();
	void listening_test(double star_b=0);
	void modify_time(int code);
	void modify_key(int code);
	void del(int code);
	~Maker();
private:
	double m_BPM;
	string m_spectrumfile;
	double m_timeperbeat;
	int counter;
	AudioClip m_ac;
};

